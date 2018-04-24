// GradDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "GradDlg.h"
#include "afxdialogex.h"


// CGradDlg dialog

IMPLEMENT_DYNAMIC(CGradDlg, CDialog)

CGradDlg::CGradDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SHARP_DLG, pParent)
{

}

CGradDlg::~CGradDlg()
{
}

void CGradDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, threshold);
}


BEGIN_MESSAGE_MAP(CGradDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGradDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGradDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CGradDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CGradDlg message handlers


void CGradDlg::OnBnClickedOk()
{
	UpdateData(true);
	CDialog::OnOK();
}


void CGradDlg::OnBnClickedCancel()
{
	threshold = -1;
	CDialog::OnCancel();
}


void CGradDlg::OnEnChangeEdit1()
{
	UpdateData(true);
	if (threshold < 0)
	{
		threshold = 0;
	}
	UpdateData(false);
}
