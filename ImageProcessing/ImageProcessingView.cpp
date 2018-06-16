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
#include "BzDlg.h"
#include "RotationDlg.h"
#include "ShiftDlg.h"
#include "AboutBox.h"
#include "ZoomDlg.h"
#include <complex>
#include <algorithm>
#include "GradDlg.h"
#include "FilterDlg.h"

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
		ON_COMMAND(ID_ROTATE, &CImageProcessingView::OnRotate)
		ON_COMMAND(ID_MIRROR_H, &CImageProcessingView::OnMirrorH)
		ON_COMMAND(ID_MIRROR_V, &CImageProcessingView::OnMirrorV)
		ON_COMMAND(ID_ABOUT, &CImageProcessingView::OnAbout)
		ON_COMMAND(ID_ZOOM_FORWARD, &CImageProcessingView::OnZoomForward)
		ON_COMMAND(ID_ZOOM_CLOSEST, &CImageProcessingView::OnZoomClosest)
		ON_COMMAND(ID_ZOOM_BI, &CImageProcessingView::OnZoomBi)
		ON_COMMAND(ID_FFT, &CImageProcessingView::OnFft)
		ON_COMMAND(ID_IFFT, &CImageProcessingView::OnIfft)
		ON_COMMAND(ID_LINEAR_TRANS, &CImageProcessingView::OnLinearTrans)
		ON_COMMAND(ID_POW_TRANS, &CImageProcessingView::OnPowTrans)
		ON_COMMAND(ID_AVERAGE, &CImageProcessingView::OnAverageTrans)
		ON_COMMAND(ID_COLOR_LEVEL, &CImageProcessingView::OnColorLevel)
		ON_COMMAND(ID_COLOR_GRAY, &CImageProcessingView::OnColorGray)
		ON_COMMAND(ID_AVERAGE_FILTER, &CImageProcessingView::OnAverageFilter)
		ON_COMMAND(ID_MID_FILTER, &CImageProcessingView::OnMidFilter)
		ON_COMMAND(ID_GRADIENT, &CImageProcessingView::OnGradient)
		ON_COMMAND(ID_WEIGHT_FILTER, &CImageProcessingView::OnWeightFilter)
		ON_COMMAND(ID_PREWITT, &CImageProcessingView::OnPrewitt)
		ON_COMMAND(ID_SOBEL, &CImageProcessingView::OnSobel)
		ON_COMMAND(ID_LAPLACIAN, &CImageProcessingView::OnLaplacian)
		ON_COMMAND(ID_IDEAL, &CImageProcessingView::OnIdeal)
		ON_COMMAND(ID_BUTTERWORTH, &CImageProcessingView::OnButterworth)
		ON_COMMAND(ID_GAUSS, &CImageProcessingView::OnGauss)
		ON_COMMAND(ID_EXP, &CImageProcessingView::OnExp)
		ON_COMMAND(ID_TI, &CImageProcessingView::OnTi)
		ON_COMMAND(ID_SWAP, &CImageProcessingView::OnSwap)
		ON_COMMAND(ID_IDEAL_HIGH, &CImageProcessingView::OnIdealHigh)
		ON_COMMAND(ID_BUTTERWORTH_HIGH, &CImageProcessingView::OnButterworthHigh)
		ON_COMMAND(ID_GAUSS_HIGH, &CImageProcessingView::OnGaussHigh)
		ON_COMMAND(ID_EXP_HIGH, &CImageProcessingView::OnExpHigh)
		ON_COMMAND(ID_TI_HIGH, &CImageProcessingView::OnTiHigh)
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
		for (int i = 0; i < imageCount; i++)
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

void CImageProcessingView::OnAbout()
{
	CAboutBox aboutBox;
	aboutBox.DoModal();
}

void CImageProcessingView::OnOpen()
{
	//重置当前读入的图片
	imageCount = 0;

	//为打开文件对话框设置可选中多个图片
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",
	                    OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",
	                    nullptr);
	char title[] = {"打开图像"};
	FileDlg.m_ofn.lpstrTitle = title;
	if (FileDlg.DoModal() == IDOK)
	{
		POSITION filePos = FileDlg.GetStartPosition();
		while (filePos != nullptr)
		{
			if (imageCount == MAX_NUM)
			{
				//最多只允许打开十个图像
				AfxMessageBox("图片个数超过上限！");
				return;
			}
			file[imageCount] = new CFile();
			CString filename = FileDlg.GetNextPathName(filePos);
			if (!(*(file[imageCount])).Open(filename, CFile::modeRead))
			{
				AfxMessageBox("cannot open the file");
				return;
			}
			if (!mybmp[imageCount].Read(file[imageCount]))
			{
				AfxMessageBox("cannot read the file");
				return;
			}
			imageCount++;
		}
	}
	else
	{
		return;
	}

	for (int i = 0; i < imageCount; i++)
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
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",
	                    nullptr);
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
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (long y = 0; y < (long)newbmp.GetDimensions().cy; y++)
	{
		for (long x = 0; x < (long)newbmp.GetDimensions().cx; x++)
		{
			if (y > 30 && y < 50)
			{
				RGBQUAD color;
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(x, y, color);
			}
			if (x > 30 && x < 50)
			{
				RGBQUAD color;
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(x, y, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnGray()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (long i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (long j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tmp = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			color.rgbGreen = tmp;
			color.rgbBlue = tmp;
			color.rgbRed = tmp;
			newbmp.WritePixel(i, j, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnBinaryzation()
{
	int threshold;
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	CBzDlg threditdlg;
	if (threditdlg.DoModal() == IDOK)
	{
		threshold = threditdlg.threshold;
	}
	else
	{
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (long i = 0; i < (long)newbmp.GetDimensions().cx; i++)
	{
		for (long j = 0; j < (long)newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tem = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			if (tem >= threshold)
			{
				color.rgbGreen = 255;
				color.rgbBlue = 255;
				color.rgbRed = 255;
			}
			else
			{
				color.rgbGreen = 0;
				color.rgbBlue = 0;
				color.rgbRed = 0;
			}
			newbmp.WritePixel(i, j, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnMinus()
{
	imageCount = 1;

	//打开相减操作的图片
	CDib tembmp;
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "打开相减操作的图片", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",
	                    nullptr);
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
		if (!tembmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	else
	{
		return;
	}
	if (tembmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	newbmp.Empty();
	newbmp.CopyDib(&mybmp[0]);
	RGBQUAD color, minusColor;
	int result;
	for (long i = 0; i < (long)newbmp.GetDimensions().cx; i++)
	{
		for (long j = 0; j < (long)newbmp.GetDimensions().cy; j++)
		{
			color = newbmp.GetPixel(i, j);
			minusColor = tembmp.GetPixel(i, j);
			if ((int)color.rgbBlue - (int)minusColor.rgbBlue < 0)
			{
				color.rgbBlue = 0;
			}
			else
			{
				color.rgbBlue -= minusColor.rgbBlue;
			}
			if ((int)color.rgbGreen - (int)minusColor.rgbGreen < 0)
			{
				color.rgbGreen = 0;
			}
			else
			{
				color.rgbGreen -= minusColor.rgbGreen;
			}
			if ((int)color.rgbRed - (int)minusColor.rgbRed < 0)
			{
				color.rgbRed = 0;
			}
			else
			{
				color.rgbRed -= minusColor.rgbRed;
			}
			newbmp.WritePixel(i, j, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnShift()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	CShiftDlg shiftdlg;
	int xoffset = 0, yoffset = 0;
	if (shiftdlg.DoModal() == IDOK)
	{
		xoffset = shiftdlg.x;
		yoffset = shiftdlg.y;
	}
	else
	{
		return;
	}
	newbmp.Empty();
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	RGBQUAD color;
	for (long x = 0; x < (long)newbmp.GetDimensions().cx; x++)
	{
		for (long y = 0; y < (long)newbmp.GetDimensions().cy; y++)
		{
			color = mybmp[0].GetPixel(x, y);
			int newx = x + xoffset;
			int newy = y + yoffset;

			if (newx < 0 || newx >= newbmp.GetDimensions().cx || newy < 0 || newy >= newbmp.GetDimensions().cy)
			{
				//Do nothing.
			}
			else
			{
				newbmp.WritePixel(newx, newy, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnRotate()
{
	imageCount = 1;
	double angle;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	CRotationDlg rotationdlg;
	if (rotationdlg.DoModal() == IDOK)
	{
		angle = rotationdlg.angle;
		angle = angle * pi / 180.0;
	}
	else
	{
		return;
	}

	// 源图四个角的坐标（以图像中心为坐标系原点）
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	long lWidth = sizeDibDisplay[0].cx; // 获取图像的宽度
	long lHeight = sizeDibDisplay[0].cy; // 获取图像的高度

	// 将旋转角度从度转换到弧度
	float fRotateAngle = angle; // 旋转角度（弧度）	
	float fSina, fCosa; // 旋转角度的正弦和余弦	
	fSina = (float)sin((double)fRotateAngle); // 计算旋转角度的正弦
	fCosa = (float)cos((double)fRotateAngle); // 计算旋转角度的余弦

	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float)(-lWidth / 2);
	fSrcY1 = (float)(lHeight / 2);
	fSrcX2 = (float)(lWidth / 2);
	fSrcY2 = (float)(lHeight / 2);
	fSrcX3 = (float)(-lWidth / 2);
	fSrcY3 = (float)(-lHeight / 2);
	fSrcX4 = (float)(lWidth / 2);
	fSrcY4 = (float)(-lHeight / 2);

	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// 计算旋转后的图像实际宽度
	long lNewWidth = (long)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	// 计算旋转后的图像高度
	long lNewHeight = (long)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	newbmp.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp[0].m_lpBMIH->biBitCount);

	// 每行
	for (int y = 0; y < lNewHeight; y++)
	{
		// 每列
		for (int x = 0; x < lNewWidth; x++)
		{
			RGBQUAD color;

			//计算新点在原图像上的位置
			int x0 = (x - lNewWidth / 2) * fCosa - (y - lNewHeight / 2) * fSina + sizeDibDisplay[0].cx / 2.0;
			int y0 = (x - lNewWidth / 2) * fSina + (y - lNewHeight / 2) * fCosa + sizeDibDisplay[0].cy / 2.0;

			if ((x0 >= 0) && (x0 < sizeDibDisplay[0].cx) && (y0 >= 0) && (y0 < sizeDibDisplay[0].cy))
			{
				color = mybmp[0].GetPixel(x0, y0);
			}
			else
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			newbmp.WritePixel(x, y, color);
		}
	}


	Invalidate(TRUE);
}


void CImageProcessingView::OnMirrorH()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.Empty();
	newbmp.CopyDib(&mybmp[0]);
	for (long x = 0; x < (long)newbmp.GetDimensions().cx / 2; x++)
	{
		for (long y = 0; y < (long)newbmp.GetDimensions().cy; y++)
		{
			RGBQUAD color1, color2;
			color1 = newbmp.GetPixel(x, y);
			color2 = newbmp.GetPixel(newbmp.GetDimensions().cx - x - 1, y);

			newbmp.WritePixel(newbmp.GetDimensions().cx - x - 1, y, color1);
			newbmp.WritePixel(x, y, color2);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnMirrorV()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.Empty();
	newbmp.CopyDib(&mybmp[0]);
	for (long x = 0; x < (long)newbmp.GetDimensions().cx; x++)
	{
		for (long y = 0; y < (long)newbmp.GetDimensions().cy / 2; y++)
		{
			RGBQUAD color1, color2;
			color1 = newbmp.GetPixel(x, y);
			color2 = newbmp.GetPixel(x, newbmp.GetDimensions().cy - y - 1);

			newbmp.WritePixel(x, newbmp.GetDimensions().cy - y - 1, color1);
			newbmp.WritePixel(x, y, color2);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnZoomForward()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	CZoomDlg zoomDlg;
	double zoomRatio;
	if (zoomDlg.DoModal() == IDOK)
	{
		zoomRatio = zoomDlg.ratio;
	}
	else
	{
		return;
	}
	newbmp.Empty();
	CSize mySize = mybmp[0].GetDimensions();

	//计算缩放后图像大小，设立标记数组
	long x = ((double)(mySize.cx - 1) * zoomRatio + 0.5) + 1;
	long y = ((double)(mySize.cy - 1) * zoomRatio + 0.5) + 1;
	int** flag = (int **)malloc(sizeof(int*) * x);
	for (long i = 0; i < x; i++)
	{
		flag[i] = (int *)malloc(sizeof(int*) * y);
		memset(flag[i], 0, sizeof(int) * y);
	}

	newbmp.CreateCDib(CSize(x, y), mybmp[0].m_lpBMIH->biBitCount);

	//前向映射
	for (long i = 0; i < mySize.cx; i++)
	{
		for (long j = 0; j < mySize.cy; j++)
		{
			RGBQUAD color;
			long nx = (double)i * zoomRatio + 0.5;
			long ny = (double)j * zoomRatio + 0.5;
			color = mybmp[0].GetPixel(i, j);
			newbmp.WritePixel(nx, ny, color);
			flag[nx][ny] = 1;
		}
	}

	//邻近补色（周围八色）
	for (long i = 0; i < x; i++)
	{
		for (long j = 0; j < y; j++)
		{
			if (!flag[i][j])
			{
				int co = 0;
				int gr = 0, re = 0, bl = 0;
				RGBQUAD color;
				for (int z = -1; z < 2; z++)
				{
					for (int k = -1; k < 2; k++)
					{
						if (i + z < 0 || i + z >= x || j + k < 0 || j + k >= y || (!flag[i + z][j + k]))
						{
							continue;
						}
						co++;
						color = newbmp.GetPixel(i + z, j + k);
						gr += color.rgbGreen;
						re += color.rgbRed;
						bl += color.rgbBlue;
					}
				}
				if (co != 0)
				{
					gr /= co;
					re /= co;
					bl /= co;
				}
				color.rgbBlue = bl;
				color.rgbRed = re;
				color.rgbGreen = gr;
				newbmp.WritePixel(i, j, color);
			}
		}
	}

	//释放标记数组内存
	for (long i = 0; i < x; i++)
	{
		free(flag[i]);
	}
	free(flag);
	Invalidate(TRUE);
}


void CImageProcessingView::OnZoomClosest()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	CZoomDlg zoomDlg;
	double zoomRatio;
	if (zoomDlg.DoModal() == IDOK)
	{
		zoomRatio = zoomDlg.ratio;
	}
	else
	{
		return;
	}
	newbmp.Empty();

	// 缩放比率
	float fXZoomRatio, fYZoomRatio;
	fXZoomRatio = fYZoomRatio = zoomRatio;

	// 源图像的宽度和高度
	long lWidth = sizeDibDisplay[0].cx; // 获取图像的宽度
	long lHeight = sizeDibDisplay[0].cy; // 获取图像的高度

	// 缩放后图像的宽度和高度
	long lNewWidth = (long)(lWidth * fXZoomRatio + 0.5);
	long lNewHeight = (long)(lHeight * fYZoomRatio + 0.5);

	newbmp.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp[0].m_lpBMIH->biBitCount);

	// 每列
	for (int x = 0; x < lNewWidth; x++)
	{
		// 每行
		for (int y = 0; y < lNewHeight; y++)
		{
			RGBQUAD color;

			//计算新点在原图像上的位置
			int x0 = (long)(x / fXZoomRatio + 0.5);
			int y0 = (long)(y / fYZoomRatio + 0.5);

			if ((x0 >= 0) && (x0 < sizeDibDisplay[0].cx) && (y0 >= 0) && (y0 < sizeDibDisplay[0].cy))
			{
				color = mybmp[0].GetPixel(x0, y0);
			}
			else
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			newbmp.WritePixel(x, y, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnZoomBi()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	CZoomDlg zoomDlg;
	double zoomRatio;
	if (zoomDlg.DoModal() == IDOK)
	{
		zoomRatio = zoomDlg.ratio;
	}
	else
	{
		return;
	}
	newbmp.Empty();

	// 缩放比率
	float fXZoomRatio, fYZoomRatio;
	fXZoomRatio = fYZoomRatio = zoomRatio;

	// 源图像的宽度和高度
	long lWidth = sizeDibDisplay[0].cx; // 获取图像的宽度
	long lHeight = sizeDibDisplay[0].cy; // 获取图像的高度

	// 缩放后图像的宽度和高度
	long lNewWidth = (long)(lWidth * fXZoomRatio + 0.5);
	long lNewHeight = (long)(lHeight * fYZoomRatio + 0.5);

	newbmp.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp[0].m_lpBMIH->biBitCount);

	// 每列
	for (int x = 0; x < lNewWidth; x++)
	{
		// 每行
		for (int y = 0; y < lNewHeight; y++)
		{
			RGBQUAD color;

			//计算新点在原图像上的位置
			float cx = x / fXZoomRatio;
			float cy = y / fYZoomRatio;

			if (((int)(cx) - 1) >= 0 && ((int)(cx) + 1) < sizeDibDisplay[0].cx && ((int)(cy) - 1) >= 0 && ((int)(cy) + 1
				) <
				sizeDibDisplay[0].cy)
			{
				float u = cx - (int)cx;
				float v = cy - (int)cy;
				int i = (int)cx;
				int j = (int)cy;

				int Green = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbGreen + (1 - u) * v * mybmp[0]
				                                                                                 .GetPixel(i, j + 1).
				                                                                                 rgbGreen
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbGreen + u * v * mybmp[0]
					                                                               .GetPixel(i + 1, j + 1).rgbGreen;

				int Blue = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbBlue + (1 - u) * v * mybmp[0]
				                                                                               .GetPixel(i, j + 1).
				                                                                               rgbBlue
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbBlue + u * v * mybmp[0]
					                                                              .GetPixel(i + 1, j + 1).rgbBlue;

				int Red = (1 - u) * (1 - v) * mybmp[0].GetPixel(i, j).rgbRed + (1 - u) * v * mybmp[0]
				                                                                             .GetPixel(i, j + 1).rgbRed
					+ u * (1 - v) * mybmp[0].GetPixel(i + 1, j).rgbRed + u * v * mybmp[0].GetPixel(i + 1, j + 1).rgbRed;

				color.rgbGreen = Green;
				color.rgbRed = Red;
				color.rgbBlue = Blue;
			}
			else
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			newbmp.WritePixel(x, y, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnFft()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.Empty();

	int width = sizeDibDisplay[0].cx;
	int height = sizeDibDisplay[0].cy;
	int i, j;

	double dTmpOne; // 临时变量 
	double dTmpTwo;


	int nTransWidth;
	int nTransHeight;

	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth];

	CSize fftsize;
	fftsize = CSize(nTransWidth, nTransHeight);

	newbmp.CreateCDib(fftsize, mybmp[0].m_lpBMIH->biBitCount);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);


	double* tmp = new double[nTransWidth * nTransHeight];


	for (int x = 0; x < nTransWidth; x++)
	{
		for (int y = 0; y < nTransHeight; y++)
		{
			RGBQUAD color;
			int pos = y * nTransWidth + x;

			double dTemp = sqrt(t[pos].real() * t[pos].real() +
				t[pos].imag() * t[pos].imag()) / 100;

			int gray;

			if (dTemp > 255)
			{
				gray = 255;
			}
			else
				gray = (int)dTemp;


			color.rgbGreen = gray;
			color.rgbRed = gray;
			color.rgbBlue = gray;

			newbmp.WritePixel(x < nTransWidth / 2 ? x + nTransWidth / 2 : x - nTransWidth / 2,
			                  y < nTransHeight / 2 ? y + nTransHeight / 2 : y - nTransHeight / 2, color);
		}
	}

	Invalidate(TRUE);
}

/*************************************************************************
*
* \函数名称：
*   FFT_2D()
*
* \输入参数:
*   complex<double> * pCTData - 图像数据
*   int    nWidth - 数据宽度
*   int    nHeight - 数据高度
*   complex<double> * pCFData - 傅立叶变换后的结果
*
* \返回值:
*   无
*
* \说明:
*   二维傅立叶变换。
*
************************************************************************
*/
void CImageProcessingView::FFT_2D(std::complex<double>* pCTData, int nWidth, int nHeight, std::complex<double>* pCFData)
{
	int x, y;
	int nTransWidth = nWidth;
	int nTransHeight = nHeight;

	// x，y（行列）方向上的迭代次数 
	int nXLev;
	int nYLev;

	/*	while(nWidth * 2 <= sizeimage.cx)
	{
	nWidth *= 2;
	nYLev++;
	}
	while(nHeight * 2 <= sizeimage.cy)
	{
	nHeight *= 2;
	nXLev++;
	}*/
	// 计算x，y（行列）方向上的迭代次数 
	nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
	nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

	for (y = 0; y < nTransHeight; y++)
	{
		// y方向进行快速傅立叶变换 
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}

	// pCFData中目前存储了pCTData经过行变换的结果 
	// 为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行 
	// 傅立叶行变换（实际上相当于对列进行傅立叶变换） 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}

	for (y = 0; y < nTransWidth; y++)
	{
		// 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的 
		// 傅立叶变换 
		FFT_1D(&pCTData[y * nTransHeight], &pCFData[y * nTransHeight], nYLev);
	}

	// pCFData中目前存储了pCTData经过二维傅立叶变换的结果，但是为了方便列方向 
	// 的傅立叶变换，对其进行了转置，现在把结果转置回来 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransWidth * y + x] = pCFData[nTransHeight * x + y];
		}
	}

	memcpy(pCTData, pCFData, sizeof(std::complex<double>) * nTransHeight * nTransWidth);
}

/*************************************************************************
*
* \函数名称：
*   IFFT_2D()
*
* \输入参数:
*   complex<double> * pCFData - 频域数据
*   complex<double> * pCTData - 时域数据
*   int    nWidth - 图像数据宽度
*   int    nHeight - 图像数据高度
*
* \返回值:
*   无
*
* \说明:
*   二维傅立叶反变换。
*
************************************************************************
*/
void CImageProcessingView::IFFT_2D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nWidth,
                                   int nHeight)
{
	// 循环控制变量 
	int x;
	int y;

	int nTransWidth = nWidth;
	int nTransHeight = nHeight;

	// 分配工作需要的内存空间 
	std::complex<double>* pCWork = new std::complex<double>[nTransWidth * nTransHeight];

	//临时变量 
	std::complex<double>* pCTmp;

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭 
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x];
			pCWork[nTransWidth * y + x] = std::complex<double>(pCTmp->real(), -pCTmp->imag());
		}
	}

	// 调用傅立叶正变换 
	FFT_2D(pCWork, nWidth, nHeight, pCTData);

	// 求时域点的共轭，求得最终结果 
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据 
	// 相差一个系数 
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCTData[nTransWidth * y + x];
			pCTData[nTransWidth * y + x] =
				std::complex<double>(pCTmp->real() / (nTransWidth * nTransHeight),
				                     -pCTmp->imag() / (nTransWidth * nTransHeight));
		}
	}
	delete pCWork;
	pCWork = nullptr;
}

/*************************************************************************
*
* \函数名称：
*   FFT_1D()
*
* \输入参数:
*   complex<double> * pCTData - 指向时域数据的指针，输入的需要变换的数据
*   complex<double> * pCFData - 指向频域数据的指针，输出的经过变换的数据
*   nLevel －傅立叶变换蝶形算法的级数，2的幂数，
*
* \返回值:
*   无
*
* \说明:
*   一维快速傅立叶变换。
*
*************************************************************************
*/

void CImageProcessingView::FFT_1D(std::complex<double>* pCTData, std::complex<double>* pCFData, int nLevel)
{
	// 循环控制变量 */
	int i;
	int j;
	int k;

	//double PI = 3.1415926;  

	// 傅立叶变换点数 
	int nCount = 0;

	// 计算傅立叶变换点数 
	nCount = 1 << nLevel;

	// 某一级的长度 
	int nBtFlyLen;
	nBtFlyLen = 0;

	// 变换系数的角度 ＝2 * PI * i / nCount 
	double dAngle;

	std::complex<double>* pCW;

	// 分配内存，存储傅立叶变化需要的系数表 
	pCW = new std::complex<double>[nCount / 2];

	// 计算傅立叶变换的系数 
	for (i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * 3.141592 * i / nCount;
		pCW[i] = std::complex<double>(cos(dAngle), sin(dAngle));
	}

	// 变换需要的工作空间 
	std::complex<double> *pCWork1, *pCWork2;

	// 分配工作空间 
	pCWork1 = new std::complex<double>[nCount];

	pCWork2 = new std::complex<double>[nCount];


	// 临时变量 
	std::complex<double>* pCTmp;

	// 初始化，写入数据 
	memcpy(pCWork1, pCTData, sizeof(std::complex<double>) * nCount);

	// 临时变量 
	int nInter;
	nInter = 0;

	// 蝶形算法进行快速傅立叶变换 
	for (k = 0; k < nLevel; k++)
	{
		for (j = 0; j < (int)pow(2, k); j++)
		{
			//计算长度 
			nBtFlyLen = (int)pow(2, (nLevel - k));

			//倒序重排，加权计算 
			for (i = 0; i < nBtFlyLen / 2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] =
					pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2])
					* pCW[(int)(i * pow(2, k))];
			}
		}

		// 交换 pCWork1和pCWork2的数据 
		pCTmp = pCWork1;
		pCWork1 = pCWork2;
		pCWork2 = pCTmp;
	}

	// 重新排序 
	for (j = 0; j < nCount; j++)
	{
		nInter = 0;
		for (i = 0; i < nLevel; i++)
		{
			if (j & (1 << i))
			{
				nInter += 1 << (nLevel - i - 1);
			}
		}
		pCFData[j] = pCWork1[nInter];
	}

	// 释放内存空间 
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW = nullptr;
	pCWork1 = nullptr;
	pCWork2 = nullptr;
}

/*************************************************************************

* \函数名称：
*    IFFT_1D()
*
* \输入参数:
*   complex<double> * pCTData - 指向时域数据的指针，输入的需要反变换的数据
*   complex<double> * pCFData - 指向频域数据的指针，输出的经过反变换的数据
*   nLevel －傅立叶变换蝶形算法的级数，2的幂数，
*
* \返回值:
*   无
*
* \说明:
*   一维快速傅立叶反变换。
*
************************************************************************
*/
void CImageProcessingView::IFFT_1D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nLevel)
{
	// 循环控制变量 
	int i;

	// 傅立叶反变换点数 
	int nCount;

	// 计算傅立叶变换点数 
	//nCount = (int)pow(2,nLevel) ; 
	nCount = 1 << nLevel;
	// 变换需要的工作空间 
	std::complex<double>* pCWork;

	// 分配工作空间 
	pCWork = new std::complex<double>[nCount];

	// 将需要反变换的数据写入工作空间pCWork 
	memcpy(pCWork, pCFData, sizeof(std::complex<double>) * nCount);

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭 
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭 
	for (i = 0; i < nCount; i++)
	{
		pCWork[i] = std::complex<double>(pCWork[i].real(), -pCWork[i].imag());
	}

	// 调用快速傅立叶变换实现反变换，结果存储在pCTData中 
	FFT_1D(pCWork, pCTData, nLevel);

	// 求时域点的共轭，求得最终结果 
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据 
	// 相差一个系数nCount 
	for (i = 0; i < nCount; i++)
	{
		pCTData[i] =
			std::complex<double>(pCTData[i].real() / nCount, -pCTData[i].imag() / nCount);
	}

	// 释放内存 
	delete pCWork;
	pCWork = nullptr;
}

//////////////////////////////////////////////////////////////
//该函数用来实现二维傅立叶变换
//参数height、width分别表示图象的高度和宽度，ising表示正反变换
//////////////////////////////////////////////////////////////
void CImageProcessingView::fourier(std::complex<double>* data, int nTransHeight, int nTransWidth, int isign)
{
	std::complex<double>* pCTData = new std::complex<double>[nTransHeight * nTransWidth];
	std::complex<double>* pCFData = new std::complex<double>[nTransHeight * nTransWidth];

	if (isign == 1)
	{
		for (int j = 0; j < nTransHeight; j++)
			for (int i = 0; i < nTransWidth; i++)
			{
				pCTData[j * nTransWidth + i] = data[j * nTransWidth + i];
			}
		FFT_2D(pCTData, nTransWidth, nTransHeight, pCFData);

		for (int i = 0; i < nTransWidth; i++)
			for (int j = 0; j < nTransHeight; j++)
				data[j * nTransWidth + i] = pCFData[j * nTransWidth + i];
	}
	else if (isign == -1)
	{
		for (int j = 0; j < nTransHeight; j++)
			for (int i = 0; i < nTransWidth; i++)
			{
				pCFData[j * nTransWidth + i] = data[j * nTransWidth + i];
			}
		IFFT_2D(pCFData, pCTData, nTransWidth, nTransHeight);
		for (int i = 0; i < nTransWidth; i++)
			for (int j = 0; j < nTransHeight; j++)
			{
				data[j * nTransWidth + i] = pCTData[j * nTransWidth + i];
			}
	}
}


void CImageProcessingView::OnIfft()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.Empty();

	int width = sizeDibDisplay[0].cx;
	int height = sizeDibDisplay[0].cy;
	int i, j;


	double dTmpOne; // 临时变量 
	double dTmpTwo;

	int nTransWidth;
	int nTransHeight;

	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth];

	CSize fftsize;
	fftsize = CSize(nTransWidth, nTransHeight);

	newbmp.CreateCDib(fftsize, mybmp[0].m_lpBMIH->biBitCount);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//傅里叶反变换
	fourier(t, nTransHeight, nTransWidth, -1);


	newbmp.CreateCDib(fftsize, mybmp[0].m_lpBMIH->biBitCount);

	double max = 0.0;
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < t[nTransWidth * j + i].real())
				max = t[nTransWidth * j + i].real();
		}


	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					t[nTransWidth * j + i].real() * t[nTransWidth * j + i].real() + t[nTransWidth * j + i].imag() * t[
						nTransWidth * j +
						i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnLinearTrans()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.Empty();

	CSize sizeimage = mybmp[0].GetDimensions();
	newbmp.CopyDib(&mybmp[0]);


	//在第一个区域取两个点(x1,y1)=（109,0）  (x2,y2)=（115,255）
	float x1 = 109, y1 = 0;
	float x2 = 115, y2 = 255;

	//计算a1,b1 
	float a1 = (y1 - y2) * 1.0 / (x1 - x2);
	float b1 = y1 - a1 * x1;


	//在第二个区域取两个点(x11,y11)=（105,0）  (x22,y22)=（115,255）
	float x11 = 105, y11 = 0;
	float x22 = 115, y22 = 255;

	//计算a2,b2 
	float a2 = (y11 - y22) * 1.0 / (x11 - x22);
	float b2 = y11 - a2 * x11;

	//在第三个区域取两个点(x111,y111)=（91,0）  (x222,y222)=（96,20）
	float x111 = 91, y111 = 0;
	float x222 = 96, y222 = 0;

	//计算a3,b3 
	float a3 = (y111 - y222) * 1.0 / (x111 - x222);
	float b3 = y111 - a3 * x111;

	for (int y = 0; y < sizeimage.cy; y++)
	{
		for (int x = 0; x < sizeimage.cx; x++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(x, y);
			//s = ag + b
			double g = color.rgbRed;

			if (g > x11 && g < x22)
				g = a2 * g + b2;
			else if (g > x111 && g < x222)
				g = a3 * g + b3;
			else if (g > x1 && g < x2)
				g = a1 * g + b1;
			else
				g = (a1 + a2 + a3) / 3 * g + (b1 + b2 + b3) / 3;

			if (g > 255) g = 255;
			if (g < 0) g = 0;


			color.rgbBlue = (unsigned char)g;
			color.rgbGreen = (unsigned char)g;
			color.rgbRed = (unsigned char)g;
			newbmp.WritePixel(x, y, color);
		}
	}
	Invalidate(TRUE);
}

void CImageProcessingView::OnPowTrans()
{
	imageCount = 1;

	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.Empty();

	//灰度化
	newbmp.CopyDib(&mybmp[0]);
	for (int i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (int j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tmp = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			color.rgbGreen = tmp;
			color.rgbBlue = tmp;
			color.rgbRed = tmp;
			newbmp.WritePixel(i, j, color);
			//TRACE("Gray: %d\n", newbmp.GetPixel(i,j).rgbRed);
		}
	}

	// 对图象的象素值进行变换
	// 每行
	for (int i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (int j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(i, j);
			//s = ag + b
			//TODO:（参数未调）
			double g = color.rgbRed;
			double c = 6.0;
			double a = 0.7;
			//TRACE("Before: %lf\n", g);
			g = c * pow(g, a);
			//g = c * log(1 + g);

			if (g > 255)
			{
				g = 255;
			}
			//TRACE("Afther: %lf\n", g);

			color.rgbBlue = (unsigned char)g;
			color.rgbGreen = (unsigned char)g;
			color.rgbRed = (unsigned char)g;
			newbmp.WritePixel(i, j, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnAverageTrans()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CopyDib(&mybmp[0]);
	for (int i = 0; i < newbmp.GetDimensions().cx; i++)
	{
		for (int j = 0; j < newbmp.GetDimensions().cy; j++)
		{
			RGBQUAD color = mybmp[0].GetPixel(i, j);
			BYTE tmp = color.rgbRed * 0.30 + color.rgbGreen * 0.59 + color.rgbBlue * 0.11;
			color.rgbGreen = tmp;
			color.rgbBlue = tmp;
			color.rgbRed = tmp;
			newbmp.WritePixel(i, j, color);
		}
	}

	CSize mysize = newbmp.GetDimensions();
	long x = mysize.cx;
	long y = mysize.cy;
	int colcolumn[256];
	int maxg = 0, ming = 255;
	memset(colcolumn, 0, sizeof(colcolumn));
	//统计
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			int temp = newbmp.GetPixel(j, i).rgbBlue;
			colcolumn[temp]++;
			maxg = max(maxg, temp);
			ming = min(ming, temp);
		}
	}
	//灰度重置
	int qzh = 0;
	int newGrayMapping[256];
	for (int i = 0; i < 256; i++)
	{
		qzh += colcolumn[i];
		newGrayMapping[i] = (int)(qzh * 1.0 / newbmp.GetDimensions().cx / newbmp.GetDimensions().cy * 255.0 + 0.5);
	}
	for (long i = 0; i < y; i++)
	{
		// 每行
		for (long j = 0; j < x; j++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(j, i);
			int gray = newGrayMapping[color.rgbBlue];
			color.rgbBlue = gray;
			color.rgbGreen = gray;
			color.rgbRed = gray;
			newbmp.WritePixel(j, i, color);
		}
	}

	/*
	double range = maxg - ming;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			double tem = newbmp.GetPixel(j, i).rgbBlue - ming;
			tem = tem / range * 255;
			color.rgbBlue = (int)(tem + 0.5);
			color.rgbRed = (int)(tem + 0.5);
			color.rgbGreen = (int)(tem + 0.5);
			newbmp.WritePixel(j, i, color);
		}
	}
	*/
	Invalidate(TRUE);
}


void CImageProcessingView::OnColorLevel()
{
	int RGBlevel[6][3] =
	{
		{255, 127, 127},
		{142, 219, 255},
		{178, 255, 182},
		{255, 248, 140},
		{237, 155, 255},
		{155, 255, 226}
	};
	double graylevelpixels = 43;
	OnAverageTrans();
	CSize mysize;
	mysize = newbmp.GetDimensions();
	long x = mysize.cx;
	long y = mysize.cy;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			double temp = newbmp.GetPixel(j, i).rgbBlue;
			int colorlevel = (temp / graylevelpixels);
			//TRACE("%d\n", colorlevel);
			color.rgbRed = RGBlevel[colorlevel][0];
			color.rgbGreen = RGBlevel[colorlevel][1];
			color.rgbBlue = RGBlevel[colorlevel][2];
			newbmp.WritePixel(j, i, color);
		}
	}
	Invalidate(TRUE);
}

void CImageProcessingView::OnColorGray()
{
	OnGray();

	//图像分为若干个灰度级
	int graylevel = 7;
	//随机产生颜色
	RGBQUAD* color_tab = new RGBQUAD[graylevel];
	srand((unsigned int)time(NULL));
	for (int i = 0; i < graylevel; i++)
	{
		color_tab[i].rgbBlue = (unsigned char)rand() % 255;
		color_tab[i].rgbGreen = (unsigned char)rand() % 255;
		color_tab[i].rgbRed = (unsigned char)rand() % 255;
	}
	int min = 1000;
	int max = 0;
	// 对图象的象素值进行变换
	for (int x = 0; x < newbmp.GetDimensions().cx; x++)
	{
		for (int y = 0; y < newbmp.GetDimensions().cy; y++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(x, y);
			if (color.rgbBlue > max) max = color.rgbBlue;

			if (color.rgbBlue < min) min = color.rgbBlue;
		}
	}
	int step = (max - min) / graylevel;
	for (int x = 0; x < newbmp.GetDimensions().cx; x++)
	{
		for (int y = 0; y < newbmp.GetDimensions().cy; y++)
		{
			RGBQUAD color;
			color = newbmp.GetPixel(x, y);

			int level = (color.rgbBlue - min) / step;
			newbmp.WritePixel(x, y, color_tab[level]);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnAverageFilter()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	int x = mysize.cx;
	int y = mysize.cy;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			double r = 0;
			for (int w = -1; w < 2; w++)
			{
				for (int k = -1; k < 2; k++)
				{
					if (w != 0 || k != 0)
					{
						int xx = (j + k + x) % x;
						int yy = (i + w + y) % y;
						color = mybmp[1].GetPixel(xx, yy);
						r += color.rgbRed;
					}
				}
			}
			color.rgbRed = r / 8.0 + 0.5;
			color.rgbGreen = r / 8.0 + 0.5;
			color.rgbBlue = r / 8.0 + 0.5;
			newbmp.WritePixel(j, i, color);
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnMidFilter()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	int x = mysize.cx;
	int y = mysize.cy;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color;
			int temp[9];
			int co = 0;
			for (int w = -1; w < 2; w++)
			{
				for (int k = -1; k < 2; k++)
				{
					if (w != 0 || k != 0)
					{
						int xx = (j + k + x) % x;
						int yy = (i + w + y) % y;
						color = mybmp[1].GetPixel(xx, yy);
						temp[co++] = color.rgbRed;
					}
				}
			}
			std::sort(temp, temp + 9);
			color.rgbRed = temp[4];
			color.rgbGreen = temp[4];
			color.rgbBlue = temp[4];
			newbmp.WritePixel(j, i, color);
		}
	}
	Invalidate(TRUE);
}

void CImageProcessingView::OnWeightFilter()
{
	double temp[3][3] = {1.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 4.0 / 16, 2.0 / 16, 1.0 / 16, 2.0 / 16, 1.0 / 16};
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	for (int x = 0 + 1; x < mysize.cx - 1; x++)
	{
		for (int y = 0 + 1; y < mysize.cy - 1; y++)
		{
			RGBQUAD color;

			double gray = 0;
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
				{
					color = mybmp[1].GetPixel(x + i, y + j);
					gray += color.rgbRed * temp[i + 1][j + 1];
				}

			color.rgbBlue = (int)gray;
			color.rgbGreen = (int)gray;
			color.rgbRed = (int)gray;
			newbmp.WritePixel(x, y, color);
		}
	}

	Invalidate(TRUE);
}


void CImageProcessingView::OnGradient()
{
	int threshold;
	CGradDlg threditdlg;
	if (threditdlg.DoModal() == IDOK)
	{
		threshold = threditdlg.threshold;
	}
	else
	{
		return;
	}
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	int x = mysize.cx;
	int y = mysize.cy;
	RGBQUAD color, color1, color2;
	for (int i = 0; i < y - 1; i++)
	{
		for (int j = 0; j < x - 1; j++)
		{
			color = newbmp.GetPixel(j, i);
			color1 = newbmp.GetPixel(j + 1, i);
			color2 = newbmp.GetPixel(j, i + 1);
			int temp = abs(color.rgbRed - color1.rgbRed) + abs(color.rgbRed - color2.rgbRed);
			if (temp < 255 && temp >= threshold)
			{
				color.rgbBlue = temp;
				color.rgbGreen = temp;
				color.rgbRed = temp;
				newbmp.WritePixel(j, i, color);
			}
			else
			{
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::TemplateONE(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW, int iTempMX,
                                       int iTempMY, float* fpArray, float fCoef)
{
	CSize sizeimage = lpDIBBits.GetDimensions();
	newbmp.CreateCDib(sizeimage, lpDIBBits.m_lpBMIH->biBitCount);
	for (int i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		for (int j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			RGBQUAD color;
			float fResult = 0;
			for (int k = 0; k < iTempH; k++)
			{
				for (int l = 0; l < iTempW; l++)
				{
					color = lpDIBBits.GetPixel(j - iTempMX + l, i - iTempMY + k);
					fResult += color.rgbRed * fpArray[k * iTempW + 1];
				}
			}
			fResult *= fCoef;
			fResult = fabs(fResult);
			if (fResult > 255)
			{
				color.rgbGreen = 255;
				color.rgbBlue = 255;
				color.rgbRed = 255;
				newbmp.WritePixel(j, i, color);
			}
			else
			{
				color.rgbGreen = (unsigned char)(fResult + 0.5);
				color.rgbBlue = (unsigned char)(fResult + 0.5);
				color.rgbRed = (unsigned char)(fResult + 0.5);
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	lpDIBBits.CopyDib(&newbmp);
}


void CImageProcessingView::OnPrewitt()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	CDib tmpbmp1, tmpbmp2;
	tmpbmp1.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp2.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp[1]);
	tmpbmp2.CopyDib(&mybmp[1]);
	int x = mysize.cx;
	int y = mysize.cy;
	int iTempH = 3;
	int iTempW = 3;
	float fTempC = 1.0;
	int iTempMX = 1;
	int iTempMY = 1;
	float aTemplate[9] = {
		-1.0, -1.0, -1.0, 0, 0, 0, 1.0, 1.0, 1.0
	};
	float aTemplate1[9] = {
		1.0, 0.0, -1.0, 1.0, 0.0, -1.0, 1.0, 0.0, -1.0
	};
	TemplateONE(tmpbmp1, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC);
	TemplateONE(tmpbmp2, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate1, fTempC);
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color, color1;
			color = tmpbmp1.GetPixel(j, i);
			color1 = tmpbmp2.GetPixel(j, i);
			if (color1.rgbBlue > color.rgbBlue)
			{
				newbmp.WritePixel(j, i, color1);
			}
			else
			{
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::OnSobel()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	mybmp[1].CopyDib(&newbmp);
	CDib tmpbmp1, tmpbmp2;
	tmpbmp1.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp2.CreateCDib(mysize, mybmp[1].m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp[1]);
	tmpbmp2.CopyDib(&mybmp[1]);
	int x = mysize.cx;
	int y = mysize.cy;
	int iTempH = 3;
	int iTempW = 3;
	float fTempC = 1.0;
	int iTempMX = 1;
	int iTempMY = 1;
	float aTemplate[9] = {
		-1.0, -2.0, -1.0, 0.0, 0.0, 0.0, 1.0, 2.0, 1.0
	};
	float aTemplate1[9] = {
		-1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0
	};
	TemplateONE(tmpbmp1, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC);
	TemplateONE(tmpbmp2, mysize.cx, mysize.cy, iTempH, iTempW, iTempMX, iTempMY, aTemplate1, fTempC);
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			RGBQUAD color, color1;
			color = tmpbmp1.GetPixel(j, i);
			color1 = tmpbmp2.GetPixel(j, i);
			if (color1.rgbBlue > color.rgbBlue)
			{
				newbmp.WritePixel(j, i, color1);
			}
			else
			{
				newbmp.WritePixel(j, i, color);
			}
		}
	}
	Invalidate(TRUE);
}


void CImageProcessingView::TemplateTWO(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW,
                                       float* fpArray, float fCoef)
{
	CSize sizeimage = lpDIBBits.GetDimensions();
	newbmp.CreateCDib(sizeimage, lpDIBBits.m_lpBMIH->biBitCount);
	for (int y = 2; y < sizeimage.cy - 2; y++) // 行(除去边缘几行)
	{
		for (int x = 2; x < sizeimage.cx - 2; x++) // 列(除去边缘几列)
		{
			RGBQUAD color;

			double fResult = 0;

			for (int j = -2; j <= 2; j++)
			{
				for (int i = -2; i <= 2; i++)
				{
					color = lpDIBBits.GetPixel(x + i, y + j);
					fResult += color.rgbRed * fpArray[(i + 2) * 5 + (j + 2)];
				}
			}

			fResult = fabs(fResult); // 取绝对值
			if (fResult > 255)
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
			}
			else if (fResult > 3.5)
			{
				color.rgbBlue = fResult;
				color.rgbRed = fResult;
				color.rgbGreen = fResult;
			}
			newbmp.WritePixel(x, y, color);
		}
	}
	lpDIBBits.CopyDib(&newbmp);
}

void CImageProcessingView::OnLaplacian()
{
	OnGray();
	CSize mysize = mybmp[0].GetDimensions();
	// mybmp[0].CopyDib(&newbmp);
	mybmp[1].CopyDib(&newbmp);
	CDib tmpbmp1;
	tmpbmp1.CopyDib(&mybmp[1]);
	int x = mysize.cx;
	int y = mysize.cy;
	int iTempH = 3;
	int iTempW = 3;
	float fTempC = 1.0;
	float aTemplate[25]; // 模板数组

	// 设置模板参数
	aTemplate[0] = -2.0;
	aTemplate[1] = -4.0;
	aTemplate[2] = -4.0;
	aTemplate[3] = -4.0;
	aTemplate[4] = -2.0;
	aTemplate[5] = -4.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 8.0;
	aTemplate[8] = 0.0;
	aTemplate[9] = -4.0;
	aTemplate[10] = -4.0;
	aTemplate[11] = 8.0;
	aTemplate[12] = 24.0;
	aTemplate[13] = 8.0;
	aTemplate[14] = -4.0;
	aTemplate[15] = -4.0;
	aTemplate[16] = 0.0;
	aTemplate[17] = 8.0;
	aTemplate[18] = 0.0;
	aTemplate[19] = -4.0;
	aTemplate[20] = -2.0;
	aTemplate[21] = -4.0;
	aTemplate[22] = -4.0;
	aTemplate[23] = -4.0;
	aTemplate[24] = -2.0;
	TemplateTWO(tmpbmp1, mysize.cx, mysize.cy, iTempH, iTempW, aTemplate, fTempC);
	newbmp.CopyDib(&tmpbmp1);
	Invalidate(TRUE);
}


void CImageProcessingView::OnIdeal()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //中间变量

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 


	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0);

				//理想低通滤波器
				if (sqrt(
						(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2))
					<=
					d0)
					H[position] = std::complex<double>(1.0, 0.0);
				else
					H[position] = std::complex<double>(0.0, 0.0);
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	//滤波
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real()
					+ tmp[nTransWidth * j + i].imag() * tmp[nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnButterworth()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //中间变量
	int n = dlg.n;

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 


	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值


				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = 1 / (1 + (sqrt(2) - 1) * pow((d / d0), (2 * n)));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);


	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnGauss()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0, max = 0.0; //中间变量

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量  
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 


	d0 = u * u + v * v; //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值

				//构造高斯低通滤波器
				double d2 = (i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight /
					2);
				H[position] = exp(-d2 / (2 * d0));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}

	//滤波

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnExp()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int n = dlg.n;


	double d0 = sqrt(u * u + v * v); //计算截止频率d0

	int i, j;
	double max = 0.0;

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 

	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值

				//构造指数低通滤波器
				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = std::complex<double>(exp(-0.347 * pow((d / d0), n)), 0.0);
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);


	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//滤波
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate();
}


void CImageProcessingView::OnTi()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;

	int u1 = dlg.u1;
	int v1 = dlg.v1;

	double d0 = sqrt(u * u + v * v); //计算截止频率d0
	double d1 = sqrt(u1 * u1 + v1 * v1);


	double max = 0.0;
	int i, j;

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 

	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值

				//构造梯形低通滤波器
				double tmp = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));

				if (tmp <= d0)
					H[position] = std::complex<double>(1.0, 0.0);

				else if (tmp > d1)
					H[position] = std::complex<double>(0.0, 0.0);
				else
				{
					double d = tmp;
					H[position] = (d - d1) / (d0 - d1);
				}
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//滤波
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnSwap()
{
	if (mybmp[0].IsEmpty() || newbmp.IsEmpty())
	{
		AfxMessageBox("无可执行操作");
		return;
	}
	imageCount = 1;
	mybmp[0].CopyDib(&newbmp);
	newbmp.Empty();
	Invalidate(TRUE);
}


void CImageProcessingView::OnIdealHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //中间变量


	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;


	////////////////////////////////////////
	using namespace std;

	complex<double>* t = new complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	complex<double>* H = new complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	complex<double>* tmp = new complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 


	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = complex<double>(color.rgbRed, 0.0); //赋予强度值


				if (sqrt(
						(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2))
					<=
					d0)
					H[position] = complex<double>(0.0, 0.0);
				else
					H[position] = complex<double>(1.0, 0.0);
			}
			else
			{
				t[position] = complex<double>(0.0, 0.0);
				H[position] = complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			//构造理想滤波器
			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                       tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			//构造理想滤波器
			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;


	Invalidate(TRUE);
}


void CImageProcessingView::OnButterworthHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0; //中间变量
	int n = dlg.n;

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 


	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值


				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = 1 / (1 + (sqrt(2) - 1) * pow((d0 / d), (2 * n)));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);


	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	double max = 0.0;

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnGaussHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int i, j;
	double d0, max = 0.0; //中间变量

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量  
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 


	d0 = u * u + v * v; //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值

				//构造高斯低通滤波器
				double d2 = (i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight /
					2);
				H[position] = 1 - exp(-d2 / (2 * d0));
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}

	//滤波

	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}


void CImageProcessingView::OnExpHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;
	int n = dlg.n;


	double d0 = sqrt(u * u + v * v); //计算截止频率d0

	int i, j;
	double max = 0.0;

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 

	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值

				//构造指数低通滤波器
				double d = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));
				H[position] = std::complex<double>(exp(-0.347 * pow((d0 / d), n)), 0.0);
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);


	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//滤波
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate();
}


void CImageProcessingView::OnTiHigh()
{
	imageCount = 1;
	if (mybmp[0].IsEmpty())
	{
		AfxMessageBox("尚未打开图片！");
		return;
	}
	newbmp.CreateCDib(mybmp[0].GetDimensions(), mybmp[0].m_lpBMIH->biBitCount);
	CFilterDlg dlg;
	dlg.DoModal();
	int u = dlg.u;
	int v = dlg.v;

	int width = mybmp[0].GetDimensions().cx; //原图象的宽度和高度    
	int height = mybmp[0].GetDimensions().cy;

	int u1 = dlg.u1;
	int v1 = dlg.v1;

	double d0 = sqrt(u * u + v * v); //计算截止频率d0
	double d1 = sqrt(u1 * u1 + v1 * v1);


	double max = 0.0;
	int i, j;

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransWidth = (int)dTmpTwo; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	int nTransHeight = (int)dTmpTwo;

	std::complex<double>* t = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* H = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 
	std::complex<double>* tmp = new std::complex<double>[nTransHeight * nTransWidth]; // 分配工作空间 

	d0 = sqrt(u * u + v * v); //计算截止频率d0

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;
			if (i < width && j < height)
			{
				RGBQUAD color;
				color = mybmp[0].GetPixel(i, j);

				t[position] = std::complex<double>(color.rgbRed, 0.0); //赋予强度值

				//构造梯形低通滤波器
				double tmp = sqrt(
					(i - nTransWidth / 2) * (i - nTransWidth / 2) + (j - nTransHeight / 2) * (j - nTransHeight / 2));

				if (tmp <= d0)
					H[position] = std::complex<double>(0.0, 0.0);

				else if (tmp > d1)
					H[position] = std::complex<double>(1.0, 0.0);
				else
				{
					double d = tmp;
					H[position] = (d - d1) / (d0 - d1);
				}
			}
			else
			{
				t[position] = std::complex<double>(0.0, 0.0);
				H[position] = std::complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//滤波
	for (j = 0; j < nTransHeight * nTransWidth; j++)
		t[j] = std::complex<double>(tmp[j].real() * H[j].real() - tmp[j].imag() * H[j].imag(),
		                            tmp[j].real() * H[j].imag() + tmp[j].imag() * H[j].real());


	//非中心化
	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			int position = j * nTransWidth + i;

			int x = i < nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j < nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j < nTransHeight; j++)
		for (i = 0; i < nTransWidth; i++)
		{
			if (max < tmp[nTransWidth * j + i].real())
				max = tmp[nTransWidth * j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j < nTransHeight; j++)
	{
		for (i = 0; i < nTransWidth; i++)
		{
			if (i < width && j < height)
			{
				int gray = sqrt(
					tmp[nTransWidth * j + i].real() * tmp[nTransWidth * j + i].real() + tmp[nTransWidth * j + i].imag()
					* tmp[
						nTransWidth * j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				newbmp.WritePixel(i, j, color);
			}
		}
	}
	delete[] t;
	delete[] H;
	delete[] tmp;

	Invalidate(TRUE);
}
