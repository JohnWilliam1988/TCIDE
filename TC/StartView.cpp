// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "TC.h"
#include "StartView.h"

//
#include "MainFrm.h"
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartView

#define IDC_START_IMAGE 1

IMPLEMENT_DYNCREATE(CStartView, CView)

CStartView::CStartView()
{

}

CStartView::~CStartView()
{

}

BEGIN_MESSAGE_MAP(CStartView, CView)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnDisableUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnDisableUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnDisableUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnDisableUpdate)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnDisableUpdate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartView diagnostics

#ifdef _DEBUG
void CStartView::AssertValid() const
{
	CView::AssertValid();
}

void CStartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStartView message handlers

void CStartView::OnInitialUpdate()
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	/*if (m_Image.GetCount() == 0)
	{
		m_Image.Load(IDB_MAIN);
		m_Image.SetSingleImage();
		m_Image.SetTransparentColor(RGB(255, 0, 255));
	}*/
	m_startHtmlDia.Create(IDD_STARTHTMLDIA,this);
	m_startHtmlDia.ShowWindow(TRUE);
    CMainFrame* pMain=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pMain->TCStartPageSetting();
	pDoc->SetTitle(_T("示例"));
}

void CStartView::OnDraw(CDC* pDC)
{
	ASSERT_VALID(pDC);

	//CRect rectClient;
	//GetClientRect(rectClient);

	//pDC->FillSolidRect(rectClient, RGB(255, 255, 255));
}

BOOL CStartView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CStartView::OnDisableUpdate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CStartView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || m_startHtmlDia.GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);	
	m_startHtmlDia.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}


void CStartView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: 在此处添加消息处理程序代码
}


BOOL CStartView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	//cs.style &= ~WS_BORDER;
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return CView::PreCreateWindow(cs);
}


void CStartView::OnDestroy()
{
	//delete m_startHtmlDia;
	CView::OnDestroy();	
	// TODO: 在此处添加消息处理程序代码
}
