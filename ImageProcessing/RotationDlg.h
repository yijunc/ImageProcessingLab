#pragma once


// CRotationDlg dialog

class CRotationDlg : public CDialog
{
	DECLARE_DYNAMIC(CRotationDlg)

public:
	CRotationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRotationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROTATION_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double angle;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
