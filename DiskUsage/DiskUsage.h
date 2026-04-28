#pragma once
#include "PluginInterface.h"
#include "DiskUsageItem.h"
#include <string>
#include <vector>

class CDiskUsagePlugin : public ITMPlugin
{
private:
    CDiskUsagePlugin();

public:
    static CDiskUsagePlugin& Instance();

    // --- ITMPlugin ---
    virtual IPluginItem* GetItem(int index) override;
    virtual const wchar_t* GetTooltipInfo() override;
    virtual void DataRequired() override;
    virtual OptionReturn ShowOptionsDialog(void* hParent) override;
    virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
    virtual void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;

private:
    static CDiskUsagePlugin m_instance;

    CDiskUsageItem m_item;

    // 60-second refresh throttle
    ULONGLONG m_last_update_tick{ 0 };
    static const ULONGLONG UPDATE_INTERVAL_MS = 60000ULL;

    std::wstring m_tooltip_info;

    // Enumerate fixed drives and return their letters (without trailing \)
    static std::vector<std::wstring> EnumerateFixedDrives();

    // Compute usage percentage for a single drive (e.g. L"C:\\")
    static float GetDriveUsagePercent(const std::wstring& root);

    // Compute total usage across all fixed drives
    static float GetTotalUsagePercent(const std::vector<std::wstring>& drives);
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) ITMPlugin* TMPluginGetInstance();
#ifdef __cplusplus
}
#endif
