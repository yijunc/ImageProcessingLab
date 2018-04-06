// BiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingCutAdd.h"
#include "BiDlg.h"
#include "afxdialogex.h"


// CBiDlg dialog

IMPLEMENT_DYNAMIC(CBiDlg, CDialog)

CBiDlg::CBiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CBiDlg::~CBiDlg()
{
}

void CBiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, threshold);
}


BEGIN_MESSAGE_MAP(CBiDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CBiDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CBiDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CBiDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBiDlg message handlers


void CBiDlg::OnBnClickedCancel()
{
	threshold = -1;
	CDialog::OnCancel();
}


void CBiDlg::OnEnChangeEdit1()
{
	UpdateData(true);
	if (threshold < 0)
	{
		threshold = 0;
	}
	else if (threshold > 255)
	{
		threshold = 255;
	}
	UpdateData(false);
}


void CBiDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}
