// ZoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ZoomDlg.h"
#include "afxdialogex.h"


// CZoomDlg dialog

IMPLEMENT_DYNAMIC(CZoomDlg, CDialog)

CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
{
}

CZoomDlg::~CZoomDlg()
{
}

void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ratio);
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
		ON_BN_CLICKED(IDOK, &CZoomDlg::OnBnClickedOk)
		ON_BN_CLICKED(IDCANCEL, &CZoomDlg::OnBnClickedCancel)
		ON_EN_CHANGE(IDC_EDIT1, &CZoomDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CZoomDlg message handlers


void CZoomDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}


void CZoomDlg::OnBnClickedCancel()
{
	ratio = 0;
	CDialog::OnCancel();
}


void CZoomDlg::OnEnChangeEdit1()
{
	UpdateData(true);
	if (ratio < 0)
	{
		ratio = 0;
	}
	UpdateData(false);
}
