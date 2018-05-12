// ImageProcessingView.h : CImageProcessingView 类的接口
//

#pragma once

#include "CDib.h"
#include <complex>
const int MAX_NUM = 5;
const double pi = 3.1415926535898;

class CImageProcessingView : public CView
{
public:
	//多张图片
	CDib mybmp[MAX_NUM];

	//多张图片大小
	CSize sizeDibDisplay[MAX_NUM];

	//处理后结果
	CDib newbmp;

	//读入文件指针
	CFile** file;

	//读入图片大小
	int imageCount;

protected: // 仅从序列化创建
	CImageProcessingView();
DECLARE_DYNCREATE(CImageProcessingView)

	// 特性
public:
	CImageProcessingDoc* GetDocument() const;

	// 操作
public:

	// 重写
public:
	void OnDraw(CDC* pDC) override; // 重写以绘制该视图
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
protected:
	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

	// 实现
public:
	virtual ~CImageProcessingView();
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

protected:

	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnChangeColor();
	afx_msg void OnGray();
	afx_msg void OnBinaryzation();
	afx_msg void OnMinus();
	afx_msg void OnShift();
	afx_msg void OnRotate();
	afx_msg void OnMirrorH();
	afx_msg void OnMirrorV();
	afx_msg void OnAbout();
	afx_msg void OnZoomForward();
	afx_msg void OnZoomClosest();
	afx_msg void OnZoomBi();
	afx_msg void OnFft();
	void FFT_2D(std::complex<double>* pCTData, int nWidth, int nHeight, std::complex<double>* pCFData);
	void IFFT_2D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nWidth, int nHeight);
	void FFT_1D(std::complex<double>* pCTData, std::complex<double>* pCFData, int nLevel);
	void IFFT_1D(std::complex<double>* pCFData, std::complex<double>* pCTData, int nLevel);
	void fourier(std::complex<double>* data, int nTransHeight, int nTransWidth, int isign);
	afx_msg void OnIfft();
	afx_msg void OnLinearTrans();
	afx_msg void OnPowTrans();
	afx_msg void OnAverageTrans();
	afx_msg void OnColorLevel();
	afx_msg void OnColorGray();
	afx_msg void OnAverageFilter();
	afx_msg void OnMidFilter();
	afx_msg void OnGradient();
	void TemplateONE(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY,
	              float* fpArray, float fCoef);
	afx_msg void OnWeightFilter();
	afx_msg void OnPrewitt();
	afx_msg void OnSobel();
	void TemplateTWO(CDib& lpDIBBits, long lWidth, long lHeight, int iTempH, int iTempW,
	                 float* fpArray, float fCoef);
	afx_msg void OnLaplacian();
	afx_msg void OnIdeal();
};

#ifndef _DEBUG  // ImageProcessingView.cpp 中的调试版本
inline CImageProcessingDoc* CImageProcessingView::GetDocument() const
   { return reinterpret_cast<CImageProcessingDoc*>(m_pDocument); }
#endif
