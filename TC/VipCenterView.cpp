// VipCenterView.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "VipCenterView.h"


// CVipCenterView

IMPLEMENT_DYNCREATE(CVipCenterView, CView)

CVipCenterView::CVipCenterView()
{

}

CVipCenterView::~CVipCenterView()
{
}

BEGIN_MESSAGE_MAP(CVipCenterView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CVipCenterView 绘图

void CVipCenterView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CVipCenterView 诊断

#ifdef _DEBUG
void CVipCenterView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CVipCenterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CVipCenterView 消息处理程序


void CVipCenterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_vipCenterDia.Create(IDD_STARTHTMLDIA,this);
	m_vipCenterDia.ShowWindow(TRUE);
	pDoc->SetTitle(_T("会员服务中心"));
	// TODO: 在此添加专用代码和/或调用基类
}


void CVipCenterView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || m_vipCenterDia.GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);	
	m_vipCenterDia.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CVipCenterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: 在此处添加消息处理程序代码
}
