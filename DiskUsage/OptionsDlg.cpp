// OptionsDlg.cpp

#include "pch.h"
#include "DiskUsage.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"
#include "DataManager.h"

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent)
    : CDialog(IDD_OPTIONS_DIALOG, pParent)
{
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DISK, m_combo_disk);
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
END_MESSAGE_MAP()

BOOL COptionsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    g_data.DPIFromWindow(this);

    // Populate the combo-box:
    //   item 0 = Total
    //   items 1..N = drive letters
    CString totalStr = g_data.StringRes(IDS_LABEL_TOTAL);
    m_combo_disk.AddString(totalStr);

    const auto& letters = g_data.m_drive_letters;
    for (const auto& letter : letters)
    {
        CString entry;
        entry.Format(L"%s%s",
            g_data.StringRes(IDS_LABEL_DISK_PREFIX).GetString(),
            letter.c_str());
        m_combo_disk.AddString(entry);
    }

    // Select the currently configured item.
    int sel = m_data.selected_disk_index;
    if (sel < 0 || sel >= m_combo_disk.GetCount())
        sel = 0;
    m_combo_disk.SetCurSel(sel);

    return TRUE;
}

void COptionsDlg::OnOK()
{
    m_data.selected_disk_index = m_combo_disk.GetCurSel();
    if (m_data.selected_disk_index < 0)
        m_data.selected_disk_index = 0;

    CDialog::OnOK();
}
