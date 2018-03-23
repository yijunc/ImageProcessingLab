// ShiftDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ShiftDlg.h"
#include "afxdialogex.h"


// CShiftDlg dialog

IMPLEMENT_DYNAMIC(CShiftDlg, CDialog)

CShiftDlg::CShiftDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SHIFT_DLG, pParent)
{

}

CShiftDlg::~CShiftDlg()
{
}

void CShiftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, x);
	DDX_Text(pDX, IDC_EDIT2, y);
}


BEGIN_MESSAGE_MAP(CShiftDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CShiftDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CShiftDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CShiftDlg message handlers


void CShiftDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}


void CShiftDlg::OnBnClickedCancel()
{
	x = 0;
	y = 0;
	CDialog::OnCancel();
}
