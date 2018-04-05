// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "AboutBox.h"
#include "afxdialogex.h"


// CAboutBox dialog

IMPLEMENT_DYNAMIC(CAboutBox, CDialog)

CAboutBox::CAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ABOUT, pParent)
{

}

CAboutBox::~CAboutBox()
{
}

void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutBox::OnBnClickedOk)
END_MESSAGE_MAP()


// CAboutBox message handlers


void CAboutBox::OnBnClickedOk()
{
	CDialog::OnOK();
}
