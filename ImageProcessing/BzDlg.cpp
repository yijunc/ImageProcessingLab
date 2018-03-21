// BzDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "BzDlg.h"
#include "afxdialogex.h"


// CBzDlg dialog

IMPLEMENT_DYNAMIC(CBzDlg, CDialog)

CBzDlg::CBzDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BZ_DLG, pParent)
{
	threshold = 0;
}

CBzDlg::~CBzDlg()
{
}

void CBzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, threshold);
}


BEGIN_MESSAGE_MAP(CBzDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CBzDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CBzDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBzDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CBzDlg message handlers


void CBzDlg::OnEnChangeEdit1()
{
	UpdateData(true);
	if (threshold < 0) {
		threshold = 0;
	}
	else if (threshold>255) {
		threshold = 255;
	}
	UpdateData(false);
}


void CBzDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}


void CBzDlg::OnBnClickedCancel()
{
	threshold = -1;
	CDialog::OnCancel();
}
