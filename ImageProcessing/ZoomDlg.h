#pragma once


// CZoomDlg dialog

class CZoomDlg : public CDialog
{
DECLARE_DYNAMIC(CZoomDlg)

public:
	CZoomDlg(CWnd* pParent = NULL); // standard constructor
	virtual ~CZoomDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZOOM_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

DECLARE_MESSAGE_MAP()
public:
	double ratio;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();
};
