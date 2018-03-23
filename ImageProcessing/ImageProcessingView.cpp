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


void CImageProcessingView::OnOpen()
{
	//重置当前读入的图片
	imageCount = 0;

	//为打开文件对话框设置可选中多个图片
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",
	                    OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = {"打开图像"};
	FileDlg.m_ofn.lpstrTitle = title;
	if (FileDlg.DoModal() == IDOK)
	{
		POSITION filePos = FileDlg.GetStartPosition();
		while (filePos != NULL)
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
			RGBQUAD nowcolor = mybmp[0].GetPixel(i, j);
			BYTE tem = nowcolor.rgbRed * 0.30 + nowcolor.rgbGreen * 0.59 + nowcolor.rgbBlue * 0.11;
			if (tem >= threshold)
			{
				nowcolor.rgbGreen = 255;
				nowcolor.rgbBlue = 255;
				nowcolor.rgbRed = 255;
			}
			else
			{
				nowcolor.rgbGreen = 0;
				nowcolor.rgbBlue = 0;
				nowcolor.rgbRed = 0;
			}
			newbmp.WritePixel(i, j, nowcolor);
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
	                    "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
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

	long lWidth = mybmp[0].GetDimensions().cx; // 获取图像的宽度
	long lHeight = mybmp[0].GetDimensions().cy; // 获取图像的高度

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

	//图像中心


	// 计算旋转后的图像实际宽度
	long lNewWidth = (long)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	// 计算旋转后的图像高度
	long lNewHeight = (long)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	newbmp.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp[0].m_lpBMIH->biBitCount);

	RGBQUAD color;

	// 每行
	for (int y = 0; y < mybmp[0].GetDimensions().cy; y++)
	{
		// 每列
		for (int x = 0; x < mybmp[0].GetDimensions().cx; x++)
		{
			color = mybmp[0].GetPixel(x, y);

			// 计算该象素在源DIB中的坐标
			int x0 = fCosa * (x - mybmp[0].GetDimensions().cx / 2.0) + fSina * (y - mybmp[0].GetDimensions().cy / 2.0);
			int y0 = -fSina * (x - mybmp[0].GetDimensions().cx / 2.0) + fCosa * (y - mybmp[0].GetDimensions().cy / 2.0);

			x0 = x0 + lNewWidth / 2;
			y0 = y0 + lNewHeight / 2;


			newbmp.WritePixel(x0, y0, color);
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
