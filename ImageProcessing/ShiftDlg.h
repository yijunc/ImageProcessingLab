#pragma once


// CShiftDlg dialog

class CShiftDlg : public CDialog
{
	DECLARE_DYNAMIC(CShiftDlg)

public:
	CShiftDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShiftDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHIFT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int x;
	int y;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
