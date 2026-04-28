#pragma once
#include "DataManager.h"

// COptionsDlg – lets the user choose which disk to monitor.

class COptionsDlg : public CDialog
{
    DECLARE_DYNAMIC(COptionsDlg)

public:
    explicit COptionsDlg(CWnd* pParent = nullptr);
    virtual ~COptionsDlg();

    SettingData m_data;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_OPTIONS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override;

private:
    CComboBox m_combo_disk;
};
