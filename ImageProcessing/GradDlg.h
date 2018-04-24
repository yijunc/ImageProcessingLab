#pragma once


// CGradDlg dialog

class CGradDlg : public CDialog
{
	DECLARE_DYNAMIC(CGradDlg)

public:
	CGradDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGradDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHARP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int threshold;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();
};
