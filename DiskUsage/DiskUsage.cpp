#include "pch.h"
#include "DiskUsage.h"
#include "DataManager.h"
#include "OptionsDlg.h"
#include <vector>

CDiskUsagePlugin CDiskUsagePlugin::m_instance;

CDiskUsagePlugin::CDiskUsagePlugin()
{
}

CDiskUsagePlugin& CDiskUsagePlugin::Instance()
{
    return m_instance;
}

// ---------------------------------------------------------------------------
// Disk enumeration and measurement helpers
// ---------------------------------------------------------------------------

std::vector<std::wstring> CDiskUsagePlugin::EnumerateFixedDrives()
{
    std::vector<std::wstring> drives;

    wchar_t buf[512]{};
    DWORD len = GetLogicalDriveStringsW(static_cast<DWORD>(_countof(buf) - 1), buf);
    if (len == 0 || len >= _countof(buf))
        return drives;

    const wchar_t* p = buf;
    while (*p)
    {
        if (GetDriveTypeW(p) == DRIVE_FIXED)
        {
            // p is e.g. L"C:\\" – store just the letter
            std::wstring letter(p, 1);
            drives.push_back(letter);
        }
        p += wcslen(p) + 1;
    }
    return drives;
}

float CDiskUsagePlugin::GetDriveUsagePercent(const std::wstring& root)
{
    // root is a drive letter like L"C" – append backslash
    std::wstring path = root + L":\\";

    ULARGE_INTEGER freeBytesAvailable{};
    ULARGE_INTEGER totalBytes{};
    ULARGE_INTEGER totalFreeBytes{};

    if (!GetDiskFreeSpaceExW(path.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes))
        return 0.0f;

    if (totalBytes.QuadPart == 0)
        return 0.0f;

    const double used = static_cast<double>(totalBytes.QuadPart - totalFreeBytes.QuadPart);
    return static_cast<float>(used / static_cast<double>(totalBytes.QuadPart) * 100.0);
}

float CDiskUsagePlugin::GetTotalUsagePercent(const std::vector<std::wstring>& drives)
{
    ULONGLONG totalUsed = 0;
    ULONGLONG totalSize = 0;

    for (const auto& letter : drives)
    {
        std::wstring path = letter + L":\\";
        ULARGE_INTEGER freeBytesAvailable{};
        ULARGE_INTEGER totalBytes{};
        ULARGE_INTEGER totalFreeBytes{};
        if (GetDiskFreeSpaceExW(path.c_str(), &freeBytesAvailable, &totalBytes, &totalFreeBytes))
        {
            totalUsed += totalBytes.QuadPart - totalFreeBytes.QuadPart;
            totalSize += totalBytes.QuadPart;
        }
    }

    if (totalSize == 0)
        return 0.0f;

    return static_cast<float>(static_cast<double>(totalUsed) / static_cast<double>(totalSize) * 100.0);
}

// ---------------------------------------------------------------------------
// ITMPlugin interface
// ---------------------------------------------------------------------------

IPluginItem* CDiskUsagePlugin::GetItem(int index)
{
    if (index == 0)
        return &m_item;
    return nullptr;
}

const wchar_t* CDiskUsagePlugin::GetTooltipInfo()
{
    return m_tooltip_info.c_str();
}

void CDiskUsagePlugin::DataRequired()
{
    // Throttle to once every 60 seconds.
    const ULONGLONG now = GetTickCount64();
    if (now - m_last_update_tick < UPDATE_INTERVAL_MS && m_last_update_tick != 0)
        return;

    m_last_update_tick = now;

    // Refresh the drive list (drives can be added/removed at runtime).
    g_data.m_drive_letters = EnumerateFixedDrives();

    float usage = 0.0f;
    const int idx = g_data.m_setting_data.selected_disk_index;

    if (idx == 0)
    {
        // Total usage across all fixed drives.
        usage = GetTotalUsagePercent(g_data.m_drive_letters);

        // Build tooltip: list each drive.
        std::wstring tip;
        for (const auto& letter : g_data.m_drive_letters)
        {
            float pct = GetDriveUsagePercent(letter);
            wchar_t line[64];
            swprintf_s(line, L"%s: %.0f%%\n", letter.c_str(), pct);
            tip += line;
        }
        // Remove trailing newline.
        if (!tip.empty() && tip.back() == L'\n')
            tip.pop_back();
        m_tooltip_info = tip;
    }
    else
    {
        const int driveIdx = idx - 1;  // 0-based into drive_letters
        if (driveIdx >= 0 && driveIdx < (int)g_data.m_drive_letters.size())
        {
            const std::wstring& letter = g_data.m_drive_letters[driveIdx];
            usage = GetDriveUsagePercent(letter);
            wchar_t line[64];
            swprintf_s(line, L"%s: %.0f%%", letter.c_str(), usage);
            m_tooltip_info = line;
        }
        else
        {
            // Index out of range – fall back to total.
            usage = GetTotalUsagePercent(g_data.m_drive_letters);
            m_tooltip_info = L"";
        }
    }

    m_item.UpdateData(usage);
}

ITMPlugin::OptionReturn CDiskUsagePlugin::ShowOptionsDialog(void* hParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CWnd* pParent = CWnd::FromHandle(static_cast<HWND>(hParent));
    COptionsDlg dlg(pParent);
    dlg.m_data = g_data.m_setting_data;
    if (dlg.DoModal() == IDOK)
    {
        g_data.m_setting_data = dlg.m_data;
        g_data.SaveConfig();
        // Force an immediate refresh on next DataRequired() call.
        m_last_update_tick = 0;
        return ITMPlugin::OR_OPTION_CHANGED;
    }
    return ITMPlugin::OR_OPTION_UNCHANGED;
}

const wchar_t* CDiskUsagePlugin::GetInfo(PluginInfoIndex index)
{
    switch (index)
    {
    case TMI_NAME:
        return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
    case TMI_DESCRIPTION:
        return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
    case TMI_AUTHOR:
        return L"LessUp";
    case TMI_COPYRIGHT:
        return L"Copyright (C) 2024";
    case TMI_URL:
        return L"https://github.com/LessUp/traffic-monitor-plugin-disk-usage";
    case TMI_VERSION:
        return L"1.00";
    default:
        break;
    }
    return L"";
}

void CDiskUsagePlugin::OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data)
{
    if (index == ITMPlugin::EI_CONFIG_DIR)
        g_data.LoadConfig(std::wstring(data));
}

ITMPlugin* TMPluginGetInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return &CDiskUsagePlugin::Instance();
}
