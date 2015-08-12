#pragma once
#include "VipCenterHtmlDia.h"

// CVipCenterView 视图

class CVipCenterView : public CView
{
	DECLARE_DYNCREATE(CVipCenterView)

protected:
	CVipCenterView();           // 动态创建所使用的受保护的构造函数
	virtual ~CVipCenterView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CVipCenterHtmlDia m_vipCenterDia;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void AdjustLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


