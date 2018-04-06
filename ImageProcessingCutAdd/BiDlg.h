#pragma once


// CBiDlg dialog

class CBiDlg : public CDialog
{
	DECLARE_DYNAMIC(CBiDlg)

public:
	CBiDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBiDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int threshold;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
};
