#include "pch.h"
#include "DiskUsageItem.h"
#include "DataManager.h"

void CDiskUsageItem::UpdateData(float usagePercent)
{
    m_usage_value = usagePercent / 100.0f;
    if (m_usage_value < 0.0f) m_usage_value = 0.0f;
    if (m_usage_value > 1.0f) m_usage_value = 1.0f;
    swprintf_s(m_value_text, L"%.0f%%", usagePercent);
}

const wchar_t* CDiskUsageItem::GetItemName() const
{
    // The name shown in the plugin manager and display settings.
    // We distinguish based on selected_disk_index:
    //   0 = Total, 1+ = specific drive
    const int idx = g_data.m_setting_data.selected_disk_index;
    if (idx == 0)
        return g_data.StringRes(IDS_ITEM_TOTAL_DISK).GetString();

    // Build "Disk C" style label
    static wchar_t buf[64];
    const auto& letters = g_data.m_drive_letters;
    const int driveIdx = idx - 1;  // 0-based into drive_letters
    if (driveIdx >= 0 && driveIdx < (int)letters.size())
    {
        swprintf_s(buf, L"%s%s",
            g_data.StringRes(IDS_ITEM_DISK_PREFIX).GetString(),
            letters[driveIdx].c_str());
        return buf;
    }
    return g_data.StringRes(IDS_ITEM_TOTAL_DISK).GetString();
}

const wchar_t* CDiskUsageItem::GetItemId() const
{
    return L"DiskUsagePlugin_DiskUsage";
}

const wchar_t* CDiskUsageItem::GetItemLableText() const
{
    const int idx = g_data.m_setting_data.selected_disk_index;
    if (idx == 0)
        return g_data.StringRes(IDS_LABEL_TOTAL).GetString();

    static wchar_t buf[32];
    const auto& letters = g_data.m_drive_letters;
    const int driveIdx = idx - 1;
    if (driveIdx >= 0 && driveIdx < (int)letters.size())
    {
        swprintf_s(buf, L"%s%s",
            g_data.StringRes(IDS_LABEL_DISK_PREFIX).GetString(),
            letters[driveIdx].c_str());
        return buf;
    }
    return g_data.StringRes(IDS_LABEL_TOTAL).GetString();
}

const wchar_t* CDiskUsageItem::GetItemValueText() const
{
    return m_value_text;
}

const wchar_t* CDiskUsageItem::GetItemValueSampleText() const
{
    return L"100%";
}
