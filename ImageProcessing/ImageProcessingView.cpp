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
		ON_COMMAND(ID_CLOSE_SECOND, &CImageProcessingView::OnCloseSecond)
		ON_COMMAND(ID_SAVE, &CImageProcessingView::OnSave)
		ON_COMMAND(ID_CLOSE, &CImageProcessingView::OnClose)
		ON_COMMAND(ID_OPEN_SECOND, &CImageProcessingView::OnOpenSecond)
		ON_COMMAND(ID_SAVE_SECOND, &CImageProcessingView::OnSaveSecond)
		ON_COMMAND(ID_CHANGE_COLOR, &CImageProcessingView::OnChangeColor)
		ON_COMMAND(ID_GRAY, &CImageProcessingView::OnGray)
		ON_COMMAND(ID_BINARYZATION, &CImageProcessingView::OnBinaryzation)
		ON_COMMAND(ID_MINUS, &CImageProcessingView::OnMinus)
END_MESSAGE_MAP()

// CImageProcessingView 构造/析构

CImageProcessingView::CImageProcessingView()
{
	// TODO: 在此处添加构造代码
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
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if(!mybmp.IsEmpty()) mybmp.Draw(pDC, CPoint(0, 0), sizeDibDisplay);
	if(!mybmp2.IsEmpty()) mybmp2.Draw(pDC, CPoint(sizeDibDisplay.cx, 0), sizeDibDisplay2);
	if(!resultBmp.IsEmpty()) resultBmp.Draw(pDC, CPoint(0, sizeDibDisplay.cy + 10), sizeDibResult);
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
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", nullptr);
	char title[] = {"Open Image"};
	FileDlg.m_ofn.lpstrTitle = title;

	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!mybmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeDibDisplay = mybmp.GetDimensions();

	Invalidate(TRUE);
}


void CImageProcessingView::OnCloseSecond()
{
	// TODO: Add your command handler code here
	mybmp2.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnSave()
{
	mybmp.Save("mybmp.bmp");
}


void CImageProcessingView::OnClose()
{
	mybmp.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnOpenSecond()
{
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", nullptr);
	char title[] = {"Open Image"};
	FileDlg.m_ofn.lpstrTitle = title;

	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!mybmp2.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp2.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeDibDisplay2 = mybmp2.GetDimensions();

	Invalidate(TRUE);
}


void CImageProcessingView::OnSaveSecond()
{
	mybmp2.Save("mybmp2.bmp");
}


void CImageProcessingView::OnChangeColor()
{
	CSize ImageSize = mybmp.GetDimensions();

	for (int y = 0; y < ImageSize.cy; y++)
	{
		for (int x = 0; x < ImageSize.cx; x++)
		{
			RGBQUAD RedColor = mybmp.GetPixel(x, y);
			if (y < 70 && y > 50)
			{
				RedColor.rgbRed = 255;
				RedColor.rgbGreen = 0;
				RedColor.rgbBlue = 0;
				mybmp.WritePixel(x, y, RedColor);
			}

			if (x < 60 && x > 30)
			{
				RedColor.rgbRed = 0;
				RedColor.rgbGreen = 0;
				RedColor.rgbBlue = 255;
				mybmp.WritePixel(x, y, RedColor);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnGray()
{
	// 对图象1的象素值进行变换
	// 每行
	for (int y = 0; y < sizeDibDisplay.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay.cx; x++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = color.rgbRed * 0.299 + color.rgbGreen * 0.587 + color.rgbBlue * 0.114;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp.WritePixel(x, y, color);
		}
	}

	// 对图象2的象素值进行变换
	// 每行
	for (int y = 0; y < sizeDibDisplay2.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay2.cx; x++)
		{
			RGBQUAD color;
			color = mybmp2.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = color.rgbRed * 0.299 + color.rgbGreen * 0.587 + color.rgbBlue * 0.114;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp2.WritePixel(x, y, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnBinaryzation()
{
	const int thresh = 120;

	// 每行
	for (int y = 0; y < sizeDibDisplay.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay.cx; x++)
		{
			RGBQUAD color;

			color = mybmp.GetPixel(x, y);

			if (color.rgbBlue < thresh)
			{
				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
			}
			else
			{
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
			}
			mybmp.WritePixel(x, y, color);
		}
	}

	// 每行
	for (int y = 0; y < sizeDibDisplay2.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay2.cx; x++)
		{
			RGBQUAD color;

			color = mybmp2.GetPixel(x, y);

			if (color.rgbBlue < thresh)
			{
				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
			}
			else
			{
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
			}
			mybmp2.WritePixel(x, y, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnMinus()
{
	resultBmp.CreateCDib(sizeDibDisplay, mybmp.m_lpBMIH->biBitCount);

	// 每行
	for (int y = 0; y < sizeDibDisplay.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay.cx; x++)
		{
			RGBQUAD color1;
			RGBQUAD color2;

			color1 = mybmp.GetPixel(x, y);
			color2 = mybmp2.GetPixel(x, y);


			RGBQUAD color;
			
			int number = (int)color1.rgbBlue - (int)color2.rgbBlue;
			color.rgbBlue = number > 0 ? number : 0;
			number = (int)color1.rgbGreen - (int)color2.rgbGreen;
			color.rgbGreen = number > 0 ? number : 0;
			number = (int)color1.rgbRed - (int)color2.rgbRed;
			color.rgbRed = number > 0 ? number : 0;
			
			resultBmp.WritePixel(x, y, color);
		}
	}
	resultBmp.Save("resultBmp.bmp");
	sizeDibResult = resultBmp.GetDimensions();
	Invalidate(TRUE);
}
