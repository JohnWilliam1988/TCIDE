// StartDoc.cpp : ʵ���ļ�
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


// CStartDoc ���

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
// CStartDoc ���л�

void CStartDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}
#endif


// CStartDoc ����


BOOL CStartDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

	CStartView *pView = NULL;

	POSITION pos = GetFirstViewPosition();

	pView = (CStartView *) GetNextView(pos);
	SetTitle(_T("ʾ��"));
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/

	// TODO:  �ڴ������ר�õĴ�������

	return TRUE;
}
