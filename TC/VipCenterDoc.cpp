// VipCenterDoc.cpp : ʵ���ļ�
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


// CVipCenterDoc ���

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
// CVipCenterDoc ���л�

void CVipCenterDoc::Serialize(CArchive& ar)
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


// CVipCenterDoc ����


BOOL CVipCenterDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

	CVipCenterView *pView = NULL;

	POSITION pos = GetFirstViewPosition();

	pView = (CVipCenterView *) GetNextView(pos);
	SetTitle(_T("VIP��������"));
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/

	// TODO:  �ڴ������ר�õĴ�������

	return TRUE;
}
