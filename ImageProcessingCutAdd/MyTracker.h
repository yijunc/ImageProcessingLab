#pragma once
#include "afxext.h"

class CMyTracker :
	public CRectTracker
{
public:
	CMyTracker(void);
	~CMyTracker(void);

	void Draw(CDC* pDC, CPen* pPen) const;
};
