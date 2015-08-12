// FileTreeView.cpp : 实现文件
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


// CFileTreeView 诊断

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


// CFileTreeView 消息处理程序


int CFileTreeView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


int CFileTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	const DWORD dwFileViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_EX_AUTOHSCROLL;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndFileView.Create(dwFileViewStyle, rectDummy, this, 5))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	//加载视图图像:
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
	//文件树
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("未选中任何源文件"), 4, 4);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
}


void CFileTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CTreeView::OnPaint()

}




void CFileTreeView::OnSize(UINT nType, int cx, int cy)
{
	CTreeView::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	AdjustLayout();
}


void CFileTreeView::OnChangeVisualStyle()
{
	m_FileViewImages.DeleteImageList();
	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;
	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
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
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加控件通知处理程序代码    
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
	// TODO: 在此添加专用代码和/或调用基类
	cs.style &= ~WS_EX_CLIENTEDGE;		//去掉边框 
	return CTreeView::PreCreateWindow(cs);
}
