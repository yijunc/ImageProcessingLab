// ImageProcessingCutAddView.h : CImageProcessingCutAddView 类的接口
//

#pragma once
#include "cdib.h"
#include "MyTracker.h"


class CImageProcessingCutAddView : public CView
{
protected: // 仅从序列化创建
	CImageProcessingCutAddView();
DECLARE_DYNCREATE(CImageProcessingCutAddView)

	// 特性
public:
	//第一幅图像
	CDib myBmp1;

	//第二幅图像
	CDib myBmp2;

	//第一幅图像大小
	CSize sizeImage1;

	//第二幅图像大小
	CSize sizeImage2;

	//第一幅画面绘制位置
	CPoint m_origin1;

	//第二幅画面绘制位置
	CPoint m_origin2;

	//矩形框
	CMyTracker m_tracker;

	//边界画笔
	CPen pen;

	//合成临时结果
	CDib tmp1;

	//掩模
	CDib tmp2;

	//前景在第一幅图中起始点
	CPoint start1, end1;

	//前景在第二幅中的起始点
	CPoint start2, end2;

	//状态
	int state;

	//确认标识
	bool ok;

	CImageProcessingCutAddDoc* GetDocument() const;

	// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC); // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CImageProcessingCutAddView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	//	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen1();
	afx_msg void OnOpen2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void ScaleImage(float scalex, float scaley);
	void UpdateImage();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBi();
	afx_msg void OnOk();
	afx_msg void OnSave();
};

#ifndef _DEBUG  // ImageProcessingCutAddView.cpp 中的调试版本
inline CImageProcessingCutAddDoc* CImageProcessingCutAddView::GetDocument() const
   { return reinterpret_cast<CImageProcessingCutAddDoc*>(m_pDocument); }
#endif
