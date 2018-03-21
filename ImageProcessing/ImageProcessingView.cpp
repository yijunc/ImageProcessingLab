// ImageProcessingView.cpp : CImageProcessingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageProcessing.h"
#endif

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CView)
		// 标准打印命令
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessingView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_COMMAND(ID_OPEN, &CImageProcessingView::OnOpen)
		ON_COMMAND(ID_SAVE, &CImageProcessingView::OnSave)
		ON_COMMAND(ID_CHANGE_COLOR, &CImageProcessingView::OnChangeColor)
		ON_COMMAND(ID_GRAY, &CImageProcessingView::OnGray)
		ON_COMMAND(ID_BINARYZATION, &CImageProcessingView::OnBinaryzation)
		ON_COMMAND(ID_MINUS, &CImageProcessingView::OnMinus)
		ON_COMMAND(ID_SHIFT, &CImageProcessingView::OnShift)
END_MESSAGE_MAP()

// CImageProcessingView 构造/析构

CImageProcessingView::CImageProcessingView()
{
	file = (CFile **)malloc(sizeof(CFile *) * MAX_NUM);
	newbmp.Empty();
}

CImageProcessingView::~CImageProcessingView()
{
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageProcessingView 绘制

void CImageProcessingView::OnDraw(CDC* pDC)
{
	//取得画布
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//取得界面大小
	CRect rectDlg;
	GetClientRect(rectDlg);
	int sysX = rectDlg.Width();

	int startX = 20;
	int startY = 20, line = 0;

	//可打开多张图片
	if (newbmp.IsEmpty())
	{
		for (int i = 0; i < imageNumber; i++)
		{
			if (startX + sizeDibDisplay[i].cx > sysX)
			{
				startX = 20;
				startY += line + 20;
				line = 0;
			}
			mybmp[i].Draw(pDC, CPoint(startX, startY), sizeDibDisplay[i]);
			startX += sizeDibDisplay[i].cx + 20;
			if (sizeDibDisplay[i].cy > line)
			{
				line = sizeDibDisplay[i].cy;
			}
		}
	}

	//针对第一张图片进行数字图像处理
	if (!newbmp.IsEmpty())
	{
		mybmp[0].Draw(pDC, CPoint(startX, 20), sizeDibDisplay[0]);
		newbmp.Draw(pDC, CPoint(50 + sizeDibDisplay[0].cx, 20), newbmp.GetDimensions());
	}
}


// CImageProcessingView 打印


void CImageProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImageProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessingView 诊断

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessingView 消息处理程序


void CImageProcessingView::OnOpen()
{
	//重置当前读入的图片
	imageNumber = 0;

	//为打开文件对话框设置可选中多个图片
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",
	                    OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"Open Image"};
	FileDlg.m_ofn.lpstrTitle = title;
	if (FileDlg.DoModal() == IDOK)
	{
		POSITION filePos = FileDlg.GetStartPosition();
		while (filePos != NULL)
		{
			if (imageNumber == MAX_NUM)
			{
				//最多只允许打开十个图像
				AfxMessageBox("图片个数超过上限！");
				return;
			}
			file[imageNumber] = new CFile();
			CString filename = FileDlg.GetNextPathName(filePos);
			if (!(*(file[imageNumber])).Open(filename, CFile::modeRead))
			{
				AfxMessageBox("cannot open the file");
				return;
			}
			if (!mybmp[imageNumber].Read(file[imageNumber]))
			{
				AfxMessageBox("cannot read the file");
				return;
			}
			imageNumber++;
		}
	}
	else
	{
		return;
	}

	for (int i = 0; i < imageNumber; i++)
	{
		if (mybmp[i].m_lpBMIH->biCompression != BI_RGB)
		{
			AfxMessageBox("Can not read compressed file.");
			return;
		}
		sizeDibDisplay[i] = mybmp[i].GetDimensions();
		file[i]->Close();
	}

	//重置处理后图像，清空
	newbmp.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnSave()
{
	//储存处理后的图像
	if (newbmp.IsEmpty())
	{
		AfxMessageBox("尚未处理图片！");
		return;
	}
	CFileDialog FileDlg(FALSE, _T("*.bmp"), "处理后图片", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"Save Image"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox("cannot save the file");
			return;
		}
		if (!newbmp.Write(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnChangeColor()
{
	imageNumber = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (int y = 0; y < newbmp.GetDimensions().cy; y++)
	{
		for (int x = 0; x < newbmp.GetDimensions().cx; x++)
		{
			if (y > 30 && y < 50)
			{
				RGBQUAD color;
				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
				newbmp.WritePixel(x, y, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnGray()
{
	Invalidate(TRUE);
}


void CImageProcessingView::OnBinaryzation()
{
	Invalidate(TRUE);
}


void CImageProcessingView::OnMinus()
{
	Invalidate(TRUE);
}


void CImageProcessingView::OnShift()
{
	Invalidate(TRUE);
}
