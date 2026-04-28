#pragma once
#include <string>
#include <vector>
#include <map>
#include "resource.h"

#define g_data CDataManager::Instance()

// Disk selection mode:
//   -1  = show all disks as separate items
//   ""  = show total (all disks aggregated)
//   "C" = show a specific drive letter
static const int DISK_MODE_ALL_SEPARATE = -2;   // not used as index; kept for clarity

struct SettingData
{
    // Index into the combo-box shown in the options dialog.
    // 0  = "Total" (aggregate of all fixed drives)
    // 1+ = individual drive letter (A, C, D, …)
    int selected_disk_index{ 0 };
};

class CDataManager
{
private:
    CDataManager();
    ~CDataManager();

public:
    static CDataManager& Instance();

    void LoadConfig(const std::wstring& config_dir);
    void SaveConfig() const;
    const CString& StringRes(UINT id);
    void DPIFromWindow(CWnd* pWnd);
    int DPI(int pixel);
    float DPIF(float pixel);
    int RDPI(int pixel);

    SettingData m_setting_data;

    // Populated during DataRequired(); each entry is "C", "D", etc.
    std::vector<std::wstring> m_drive_letters;

private:
    static CDataManager m_instance;
    std::wstring m_config_path;
    std::map<UINT, CString> m_string_table;
    int m_dpi{ 96 };
};
