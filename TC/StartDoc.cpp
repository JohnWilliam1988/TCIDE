// StartDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "StartDoc.h"
#include "StartView.h"


// CStartDoc

IMPLEMENT_DYNCREATE(CStartDoc, CDocument)

CStartDoc::CStartDoc()
{
}

BOOL CStartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CStartDoc::~CStartDoc()
{
}


BEGIN_MESSAGE_MAP(CStartDoc, CDocument)
END_MESSAGE_MAP()


// CStartDoc 诊断

#ifdef _DEBUG
void CStartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CStartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CStartDoc 序列化

void CStartDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
#endif


// CStartDoc 命令


BOOL CStartDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

	CStartView *pView = NULL;

	POSITION pos = GetFirstViewPosition();

	pView = (CStartView *) GetNextView(pos);
	SetTitle(_T("示例"));
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/

	// TODO:  在此添加您专用的创建代码

	return TRUE;
}
