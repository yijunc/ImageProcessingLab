// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// CFilterDlg dialog

IMPLEMENT_DYNAMIC(CFilterDlg, CDialog)

CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FILER_DLG, pParent)
{
}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, u);
	DDX_Text(pDX, IDC_EDIT2, v);
	DDX_Text(pDX, IDC_EDIT3, n);
	DDX_Text(pDX, IDC_EDIT4, u1);
	DDX_Text(pDX, IDC_EDIT5, v1);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
		ON_BN_CLICKED(IDCANCEL, &CFilterDlg::OnBnClickedCancel)
		ON_BN_CLICKED(IDOK, &CFilterDlg::OnBnClickedOk)
	
END_MESSAGE_MAP()


// CFilterDlg message handlers


void CFilterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void CFilterDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

