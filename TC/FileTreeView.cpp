// FileTreeView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "TCView.h"
#include "FileTreeView.h"
#include "ProjTreeView.h"
#include "CxSplitterWnd.h"
// CFileTreeView

IMPLEMENT_DYNCREATE(CFileTreeView, CTreeView)

CFileTreeView::CFileTreeView()
{
	m_hCurTreeItem = NULL;
}

CFileTreeView::~CFileTreeView()
{
}

BEGIN_MESSAGE_MAP(CFileTreeView, CTreeView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFileTreeView ���

#ifdef _DEBUG
void CFileTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileTreeView ��Ϣ�������


int CFileTreeView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


int CFileTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	const DWORD dwFileViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_EX_AUTOHSCROLL;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndFileView.Create(dwFileViewStyle, rectDummy, this, 5))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	//������ͼͼ��:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	OnChangeVisualStyle();
	//FillFileView();
	AdjustLayout();

	return 0;
}

void CFileTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndFileView.SetWindowPos(NULL,rectClient.left + 1,rectClient.top + 1,rectClient.Width() - 2,rectClient.Height() - 1, SWP_NOACTIVATE | SWP_NOZORDER);

}

void CFileTreeView::FillFileView()
{
	//�ļ���
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("δѡ���κ�Դ�ļ�"), 4, 4);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
}


void CFileTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CTreeView::OnPaint()

}




void CFileTreeView::OnSize(UINT nType, int cx, int cy)
{
	CTreeView::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	AdjustLayout();
}


void CFileTreeView::OnChangeVisualStyle()
{
	m_FileViewImages.DeleteImageList();
	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	//m_wndProjView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}



BOOL CFileTreeView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);
	if(pNMHDR->hwndFrom==m_wndFileView.m_hWnd)
	{
		switch (pNMHDR->code)
		{
		case NM_DBLCLK:
			OnNMDblclkFileView(pNMHDR, pResult);
			break;
		default:
			break;
		}
	}
	return CTreeView::OnNotify(wParam, lParam, pResult);
}

void CFileTreeView::OnNMDblclkFileView(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������    
	CProjTreeView* pProjView = (CProjTreeView*)(((CxSplitterWnd*)GetParent())->GetPane(0,0));
	if( pProjView == NULL )
		return;

	HTREEITEM helpitem = m_wndFileView.GetSelectedItem();

	if(FCTREENODETYPE::FCTREE_FUNCTION == (FCTREENODETYPE)m_wndFileView.GetItemData(helpitem))
	{
		CString csFileName = m_wndFileView.GetItemText(helpitem);
		HTREEITEM parentItem = m_wndFileView.GetParentItem(helpitem);
		CString csNSName = m_wndFileView.GetItemText(parentItem);

		csFileName.Trim();

		//int start=csFileName.Find(L' ',0);
		int end=csFileName.Find(L'(',0);

		//CString fun = csFileName.Mid(start,end-start);
		CString fun = csFileName.Mid(0,end);
		fun.Trim();
		int line = pProjView->m_FunTreeInfor.getLine(csNSName,fun);

		if(line!=-1)
		{
			CTCView *pView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView( pProjView->fFunTreePath );
			//::SendMessage(this->m_hWnd,WM_KILLFOCUS,(WPARAM)pView->m_hWnd,0);
			pView->TCActiveEditView();
			pView->TCGotoLine(line);
			return;
		}
	}

	*pResult = 0;
}

BOOL CFileTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style &= ~WS_EX_CLIENTEDGE;		//ȥ���߿� 
	return CTreeView::PreCreateWindow(cs);
}
