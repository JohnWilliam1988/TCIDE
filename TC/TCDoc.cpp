// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// TCDoc.cpp : CTCDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TC.h"
#endif

#include "TCDoc.h"
#include "TCView.h"
#include "MainFrm.h"

#include <propkey.h>


#include "TCRunTimeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTCDoc

IMPLEMENT_DYNCREATE(CTCDoc, CDocument)

BEGIN_MESSAGE_MAP(CTCDoc, CDocument)
END_MESSAGE_MAP()


// CTCDoc 构造/析构

CTCDoc::CTCDoc()
{
	// TODO: 在此添加一次性构造代码
}

CTCDoc::~CTCDoc()
{
}

BOOL CTCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CTCDoc 序列化

void CTCDoc::Serialize(CArchive& ar)
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


#ifdef SHARED_HANDLERS

// 缩略图的支持
void CTCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CTCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CTCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTCDoc 诊断

#ifdef _DEBUG
void CTCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTCDoc 命令


BOOL CTCDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    //if (!CDocument::OnOpenDocument(lpszPathName))
    //    return FALSE;

    // TODO:  在此添加您专用的创建代码
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

	CTCView *pView = NULL;

    POSITION pos = GetFirstViewPosition();

    pView = (CTCView *) GetNextView(pos);

	if(!pView->m_ScintillaWnd.OpenFile(lpszPathName))
	{
		return false;
	}

	//pView->TCToggleFoldLoad();

	//这儿设置状态栏的修改时间
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCSetHeadPanelText(_T("就绪"),_T("项目已打开"));
    return TRUE;
}


BOOL CTCDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类

	/*return*/ CDocument::OnSaveDocument(lpszPathName);
	//CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	//// Get the active MDI child window.
	//CMDIChildWnd *pChild =(CMDIChildWnd *)pFrame->GetActiveFrame();

	//// Get the active view attached to the active MDI child window.
	//CTCView *pView = (CTCView *) pChild->GetActiveView();
	//pView->m_ScintillaWnd.SaveFile(lpszPathName);
	POSITION ps=this->GetFirstViewPosition();
	if(ps!=NULL)
	{ 
		CTCView * pView=(CTCView *)GetNextView(ps);

		//pView->TCToggleFoldSave();

		if(pView!=NULL)
			pView->m_ScintillaWnd.SaveFile(lpszPathName);
		///////////////////设置状态栏///////////////////////////
		//这儿获取文件保存后文件大小
		int lenth=pView->TCGetLength();
		//设置文件大小状态栏
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCSetFileInfo(lenth);
		///////////////////设置状态栏///////////////////////////
	}
	return true;/*CDocument::OnSaveDocument(lpszPathName);*/
}