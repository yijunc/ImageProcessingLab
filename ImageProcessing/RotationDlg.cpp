// RotationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "RotationDlg.h"
#include "afxdialogex.h"


// CRotationDlg dialog

IMPLEMENT_DYNAMIC(CRotationDlg, CDialog)

CRotationDlg::CRotationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ROTATION_DLG, pParent)
{
	angle = 0;
}

CRotationDlg::~CRotationDlg()
{
}

void CRotationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, angle);
}


BEGIN_MESSAGE_MAP(CRotationDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CRotationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRotationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRotationDlg message handlers


void CRotationDlg::OnBnClickedCancel()
{
	angle = 0;
	CDialog::OnCancel();
}


void CRotationDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}
