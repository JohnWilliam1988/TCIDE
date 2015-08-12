// VipCenterDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "VipCenterDoc.h"
#include "VipCenterView.h"


// CVipCenterDoc

IMPLEMENT_DYNCREATE(CVipCenterDoc, CDocument)

CVipCenterDoc::CVipCenterDoc()
{
}

BOOL CVipCenterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CVipCenterDoc::~CVipCenterDoc()
{
}


BEGIN_MESSAGE_MAP(CVipCenterDoc, CDocument)
END_MESSAGE_MAP()


// CVipCenterDoc 诊断

#ifdef _DEBUG
void CVipCenterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CVipCenterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CVipCenterDoc 序列化

void CVipCenterDoc::Serialize(CArchive& ar)
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


// CVipCenterDoc 命令


BOOL CVipCenterDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

	CVipCenterView *pView = NULL;

	POSITION pos = GetFirstViewPosition();

	pView = (CVipCenterView *) GetNextView(pos);
	SetTitle(_T("VIP服务中心"));
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/

	// TODO:  在此添加您专用的创建代码

	return TRUE;
}
