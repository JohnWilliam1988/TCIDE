// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// TCDesignDoc.cpp : CTCDesignDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TC.h"
#endif

#include "TCDesignDoc.h"

#include <propkey.h>

//
#include "TCDesignView.h"
//

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTCDesignDoc

IMPLEMENT_DYNCREATE(CTCDesignDoc, CDocument)

BEGIN_MESSAGE_MAP(CTCDesignDoc, CDocument)
END_MESSAGE_MAP()


// CTCDesignDoc ����/����

CTCDesignDoc::CTCDesignDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CTCDesignDoc::~CTCDesignDoc()
{
}

BOOL CTCDesignDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CTCDesignDoc ���л�

void CTCDesignDoc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CTCDesignDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CTCDesignDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CTCDesignDoc::SetSearchContent(const CString& value)
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

// CTCDesignDoc ���

#ifdef _DEBUG
void CTCDesignDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTCDesignDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTCDesignDoc ����


BOOL CTCDesignDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    //if (!CDocument::OnOpenDocument(lpszPathName))
    //    return FALSE;

    CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

    // Get the active MDI child window.
    CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

    CTCDesignView *pView = NULL;

    POSITION pos = GetFirstViewPosition();

    pView = (CTCDesignView *) GetNextView(pos);

    pView->m_TWinID=lpszPathName;

    this->SetTitle(lpszPathName);

    // TODO:  �ڴ������ר�õĴ�������

    return TRUE;
}

BOOL CTCDesignDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: �ڴ����ר�ô����/����û���

    //return CDocument::OnSaveDocument(lpszPathName);
    CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

    // Get the active MDI child window.
    CMDIChildWnd *pChild =(CMDIChildWnd *)pFrame->GetActiveFrame();

    // Get the active view attached to the active MDI child window.
    CTCDesignView *pView = (CTCDesignView *) pChild->GetActiveView();
    pView->OnFileSave();
    return TRUE;
}
