// VipCenterView.cpp : ʵ���ļ�
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


// CVipCenterView ��ͼ

void CVipCenterView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CVipCenterView ���

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


// CVipCenterView ��Ϣ�������


void CVipCenterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_vipCenterDia.Create(IDD_STARTHTMLDIA,this);
	m_vipCenterDia.ShowWindow(TRUE);
	pDoc->SetTitle(_T("��Ա��������"));
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ˴������Ϣ����������
}
