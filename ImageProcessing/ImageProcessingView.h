// ImageProcessingView.h : CImageProcessingView 类的接口
//

#pragma once

#include "CDib.h"
const int MAX_NUM = 5;
const double pi = 3.1415926;
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
	CFile ** file;

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
};

#ifndef _DEBUG  // ImageProcessingView.cpp 中的调试版本
inline CImageProcessingDoc* CImageProcessingView::GetDocument() const
   { return reinterpret_cast<CImageProcessingDoc*>(m_pDocument); }
#endif
