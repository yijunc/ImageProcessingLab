// ImageProcessingCutAddView.cpp : CImageProcessingCutAddView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageProcessingCutAdd.h"
#endif

#include "ImageProcessingCutAddDoc.h"
#include "ImageProcessingCutAddView.h"
#include "BiDlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingCutAddView

IMPLEMENT_DYNCREATE(CImageProcessingCutAddView, CView)

BEGIN_MESSAGE_MAP(CImageProcessingCutAddView, CView)
		// 标准打印命令
		ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
		//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessingCutAddView::OnFilePrintPreview)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_COMMAND(ID_OPEN1, &CImageProcessingCutAddView::OnOpen1)
		ON_COMMAND(ID_OPEN2, &CImageProcessingCutAddView::OnOpen2)
		ON_WM_LBUTTONDOWN()
		ON_WM_SETCURSOR()
		ON_COMMAND(ID_BI, &CImageProcessingCutAddView::OnBi)
		ON_COMMAND(ID_OK, &CImageProcessingCutAddView::OnOk)
		ON_COMMAND(ID_SAVE, &CImageProcessingCutAddView::OnSave)
END_MESSAGE_MAP()

// CImageProcessingCutAddView 构造/析构

CImageProcessingCutAddView::CImageProcessingCutAddView()
{
	m_tracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside | CRectTracker::hatchedBorder;
	m_tracker.m_nHandleSize = 7;
	pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_tracker.m_rect.SetRect(0, 0, 0, 0);
	state = 1;
	ok = false;
}

CImageProcessingCutAddView::~CImageProcessingCutAddView()
{
}

BOOL CImageProcessingCutAddView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageProcessingCutAddView 绘制

void CImageProcessingCutAddView::OnDraw(CDC* pDC)
{
	CImageProcessingCutAddDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_origin2.x = m_origin1.x + sizeImage1.cx + 10;
	m_origin2.y = m_origin1.y;

	switch (state)
	{
	case 1:
		myBmp1.Draw(pDC, m_origin1, sizeImage1);
		myBmp2.Draw(pDC, m_origin2, sizeImage2);
		break;
	case 2:
	case 3:
		//绘制临时合成结果
		tmp1.Draw(pDC, m_origin1, sizeImage1);

		//绘制掩模
		tmp2.Draw(pDC, m_origin2, sizeImage2);
	default:
		break;
	}

	//未确认情况下，更新选择框边框线条
	if (!ok)
	{
		CClientDC dc(this);
		m_tracker.Draw(&dc, &pen);
	}
}


// CImageProcessingCutAddView 打印


//void CImageProcessingCutAddView::OnFilePrintPreview()
//{
//#ifndef SHARED_HANDLERS
//	AFXPrintPreview(this);
//#endif
//}

BOOL CImageProcessingCutAddView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageProcessingCutAddView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageProcessingCutAddView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImageProcessingCutAddView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessingCutAddView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessingCutAddView 诊断

#ifdef _DEBUG
void CImageProcessingCutAddView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcessingCutAddView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcessingCutAddDoc* CImageProcessingCutAddView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingCutAddDoc)));
	return (CImageProcessingCutAddDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessingCutAddView 消息处理程序


void CImageProcessingCutAddView::OnOpen1()
{
	//读入图像1
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "打开第一幅图像", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"打开图像"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!myBmp1.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	else
	{
		return;
	}
	if (myBmp1.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}

	//初始化图像1
	sizeImage1 = myBmp1.GetDimensions();
	m_origin1.x = 0;
	m_origin1.y = 0;
	//初始化临时图像
	tmp1.CreateCDib(sizeImage1, myBmp1.m_lpBMIH->biBitCount);
	//初始化选择框位置
	start1.x = sizeImage1.cx / 4;
	start1.y = sizeImage1.cy / 4;
	end1.x = sizeImage1.cx * 3 / 4;
	end1.y = sizeImage1.cy * 3 / 4;
	m_tracker.m_rect.SetRect(start1.x, start1.y, end1.x, end1.y);
	//获得操作窗口
	CClientDC dc(this);
	//绘制选择框
	m_tracker.Draw(&dc, &pen);

	//设定状态
	state = 1;

	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnOpen2()
{
	//读入图像2
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "打开第二幅图像", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"打开图像"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!myBmp2.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	else
	{
		return;
	}
	if (myBmp2.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}

	//初始化图像2
	sizeImage2 = myBmp2.GetDimensions();
	//初始化掩模
	tmp2.CreateCDib(sizeImage2, myBmp2.m_lpBMIH->biBitCount);
	//设定状态
	state = 1;
	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//已完成或者图像未二值化时不可操作
	if (ok || state == 1)
		return;

	int nIn; //定义一个鼠标的点击值；
	nIn = m_tracker.HitTest(point); //看看点到了哪了

	CRect Trect; //定义橡皮筋框的矩形
	CRect Prect; //图片矩形框

	//得到图片的矩形大小，此处大小为相对窗口位置
	Prect.left = m_origin1.x;
	Prect.top = m_origin1.y;
	Prect.right = sizeImage1.cx;
	Prect.bottom = sizeImage1.cy;

	//CPoint传入也为相对窗口位置
	if (point.x < Prect.left || point.x > Prect.right || point.y < Prect.top || point.y > Prect.bottom)
	{
		return; //判断是否在图片框内，不处理不在图片框内的点击
	}

	m_tracker.SetCursor(this, nFlags); //改变鼠标的形状
	if (nIn < 0) //不在四边形区域内；
	{
		Invalidate(true);
		m_tracker.TrackRubberBand(this, point, false); //不在矩形框内则画橡皮筋框    

		Trect = m_tracker.m_rect; //得到画好的橡皮筋框

		//调整大小
		Trect.top = (Trect.top < Prect.top ? Prect.top : Trect.top);
		Trect.left = (Trect.left < Prect.left ? Prect.left : Trect.left);
		Trect.bottom = (Trect.bottom > Prect.bottom ? Prect.bottom : Trect.bottom);
		Trect.right = (Trect.right > Prect.right ? Prect.right : Trect.right);

		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom); //画出调整好的矩形框

		CClientDC dc(this);

		UpdateImage();
		m_tracker.Draw(&dc, &pen);

		Invalidate(false);
	}
	else //在四边形区域内：       
	{
		Invalidate(); //重画界面
		CClientDC dc(this);

		//api函数，将鼠标限制在图片框内     
		//由于鼠标限制为相对系统屏幕坐标，此处需要进行转换
		ClientToScreen(&Prect);
		ClipCursor(&Prect); 

		//画tracker
		m_tracker.Draw(&dc, &pen);
		Invalidate(false);
		m_tracker.Track(this, point, false);

		//得到画好的橡皮筋框，此处获得的tracker矩形坐标为相对窗口坐标
		Trect = m_tracker.m_rect; 
		ScreenToClient(&Prect);

		if (Trect.top < Prect.top) //调整矩形框的位置
		{
			//超出图片框顶部的位置
			Trect.bottom = Prect.top - Trect.top + Trect.bottom;
			Trect.top = Prect.top;
		}
		if (Trect.bottom > Prect.bottom)
		{
			//超出底部的位置
			Trect.top = Prect.bottom - Trect.bottom + Trect.top;
			Trect.bottom = Prect.bottom;
		}
		if (Trect.right > Prect.right)
		{
			//超出右边
			Trect.left = Prect.right - Trect.right + Trect.left;
			Trect.right = Prect.right;
		}
		if (Trect.left < Prect.left)
		{
			//超出左边
			Trect.right = Prect.left - Trect.left + Trect.right;
			Trect.left = Prect.left;
		}
		//设置矩形框大小
		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom);
		
		//更新结果缓存与tracker矩形
		UpdateImage();
		m_tracker.Draw(&dc, &pen);
		Invalidate(TRUE);
		//释放对鼠标的限制
		ClipCursor(NULL); 
	}
	CView::OnLButtonDown(nFlags, point);
}

void CImageProcessingCutAddView::ScaleImage(float scalex, float scaley)
{
	CSize newSize;
	newSize.cx = (end2.x - start2.x) * scalex;
	newSize.cy = (end2.y - start2.y) * scaley;
	TRACE("start1.x: %d\n", start1.x);
	TRACE("start1.y: %d\n", start1.y);
	TRACE("end1.x: %d\n", end1.x);
	TRACE("end1.y: %d\n", end1.y);
	TRACE("start2.x: %d\n", start2.x);
	TRACE("start2.y: %d\n", start2.y);
	TRACE("end2.x: %d\n", end2.x);
	TRACE("end2.x: %d\n", end2.y);
	for (int x = 0; x < newSize.cx; x++)
	{
		for (int y = 0; y < newSize.cy; y++)
		{
			RGBQUAD color;
			//后向映射
			int x0 = (long)(x * 1.0 / scalex + 0.5);
			int y0 = (long)(y * 1.0 / scaley + 0.5);
			//确保原图像位置合法
			if (x0 >= 0 && x0 < end2.x - start2.x + 1 && y0 >= 0 && y0 < end2.y - start2.y + 1)
			{
				color = tmp2.GetPixel(x0 + start2.x, y0 + start2.y);
				//判断掩模是否有效
				if (color.rgbRed == 255 && color.rgbGreen == 255 && color.rgbBlue == 255)
				{
					RGBQUAD colorReal;
					colorReal = myBmp2.GetPixel(x0 + start2.x, y0 + start2.y);
					//颜色映射
					tmp1.WritePixel(start1.x + x, start1.y + y, colorReal);
				}
			}
		}
	}
}

void CImageProcessingCutAddView::UpdateImage()
{
	//获得图像二的掩模在图像一的大小
	start1.x = m_tracker.m_rect.left;
	start1.y = m_tracker.m_rect.top;
	end1.x = m_tracker.m_rect.right;
	end1.y = m_tracker.m_rect.bottom;
	//刷新缓存结果
	tmp1.CopyDib(&myBmp1);
	//计算缩放倍数
	float scalex = (end1.x - start1.x + 1) * 1.0 / (end2.x - start2.x + 1);
	float scaley = (end1.y - start1.y + 1) * 1.0 / (end2.y - start2.y + 1);
	ScaleImage(scalex, scaley);
	state = 3;
}


BOOL CImageProcessingCutAddView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == this && m_tracker.SetCursor(this, nHitTest))
	{
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CImageProcessingCutAddView::OnBi()
{
	int maxX = 0, maxY = 0;
	int minX = 0x3f3f3f3f, minY = 0x3f3f3f3f;
	int threshold;
	if (myBmp2.IsEmpty())
	{
		AfxMessageBox("尚未打开第二张图片！");
		return;
	}
	CBiDlg threditdlg;
	if (threditdlg.DoModal() == IDOK)
	{
		threshold = threditdlg.threshold;
	}
	else
	{
		return;
	}
	tmp2.CreateCDib(sizeImage2, myBmp2.m_lpBMIH->biBitCount);
	for (long i = 0; i < (long)tmp2.GetDimensions().cx; i++)
	{
		for (long j = 0; j < (long)tmp2.GetDimensions().cy; j++)
		{
			RGBQUAD color = myBmp2.GetPixel(i, j);
			//灰度化
			BYTE tem = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			//二值化
			if (tem >= threshold)
			{
				color.rgbGreen = 255;
				color.rgbBlue = 255;
				color.rgbRed = 255;

				//取得掩模位置
				minX = min(minX, i);
				minY = min(minY, j);
				maxX = max(maxX, i);
				maxY = max(maxY, j);
			}
			else
			{
				color.rgbGreen = 0;
				color.rgbBlue = 0;
				color.rgbRed = 0;
			}
			tmp2.WritePixel(i, j, color);
		}
	}
	//确定掩模位置
	start2.x = minX;
	start2.y = minY;
	end2.x = maxX;
	end2.y = maxY;

	//重置临时图像
	tmp1.CopyDib(&myBmp1);
	state = 2;
	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnOk()
{
	ok = true;
	Invalidate(TRUE);
}


void CImageProcessingCutAddView::OnSave()
{
	//储存处理后的图像
	if (!ok)
	{
		AfxMessageBox("尚未确认！");
		return;
	}
	CFileDialog FileDlg(FALSE, _T("*.bmp"), "处理后图片", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"保存图像"};
	FileDlg.m_ofn.lpstrTitle = title;
	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox("cannot save the file");
			return;
		}
		if (!tmp1.Write(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	Invalidate(TRUE);
}
