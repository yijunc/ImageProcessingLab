#pragma once


// CBzDlg dialog

class CBzDlg : public CDialog
{
DECLARE_DYNAMIC(CBzDlg)

public:
	CBzDlg(CWnd* pParent = NULL); // standard constructor
	virtual ~CBzDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BZ_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

DECLARE_MESSAGE_MAP()

public:
	int threshold;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
