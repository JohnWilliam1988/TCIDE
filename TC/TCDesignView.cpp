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

// TCDesignView.cpp : CTCDesignView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TC.h"
#endif

#include "TCDesignDoc.h"
#include "TCDesignView.h"
#include "TCCtlHeads.h"
//
#include "MainFrm.h"

#include "Docommand.h"
//

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTCDesignView

IMPLEMENT_DYNCREATE(CTCDesignView, CView)

BEGIN_MESSAGE_MAP(CTCDesignView, CView)
        //��׼��ӡ����ȫ��������Ч
        ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnDisableUpdate)
        ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnDisableUpdate)
        ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnDisableUpdate)
        ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnDisableUpdate)
        ON_WM_CONTEXTMENU()
        ON_WM_RBUTTONUP()
        ON_UPDATE_COMMAND_UI(ID_TC_ALIGNLEFT, &CTCDesignView::OnUpdateTcAlignleft)
        ON_UPDATE_COMMAND_UI(ID_TC_CENTERALIGNH, &CTCDesignView::OnUpdateTcCenteralignh)
        ON_UPDATE_COMMAND_UI(ID_TC_ALIGNRIGHT, &CTCDesignView::OnUpdateTcAlignright)
        ON_UPDATE_COMMAND_UI(ID_TC_ALIGNTOP, &CTCDesignView::OnUpdateTcAligntop)
        ON_UPDATE_COMMAND_UI(ID_TC_CENTERALIGNV, &CTCDesignView::OnUpdateTcCenteralignv)
        ON_UPDATE_COMMAND_UI(ID_TC_ALIGNBOTTOM, &CTCDesignView::OnUpdateTcAlignbottom)
        ON_UPDATE_COMMAND_UI(ID_TC_SAMEHEIGHT, &CTCDesignView::OnUpdateTcSameheight)
        ON_UPDATE_COMMAND_UI(ID_TC_SAMEWIDTH, &CTCDesignView::OnUpdateTcSamewidth)
        ON_UPDATE_COMMAND_UI(ID_TC_SAMESIZE, &CTCDesignView::OnUpdateTcSamesize)
        ON_UPDATE_COMMAND_UI(ID_TC_UNIFORMSPACEH, &CTCDesignView::OnUpdateTcUniformspaceh)
        ON_UPDATE_COMMAND_UI(ID_TC_UNIFORMSPACEV, &CTCDesignView::OnUpdateTcUniformspacev)
        ON_UPDATE_COMMAND_UI(ID_TC_CENTERWINV, &CTCDesignView::OnUpdateTcCenterwinv)
        ON_UPDATE_COMMAND_UI(ID_TC_CENTERWINH, &CTCDesignView::OnUpdateTcCenterwinh)
        ON_COMMAND(ID_TC_ALIGNLEFT, &CTCDesignView::OnTcAlignleft)
        ON_COMMAND(ID_TC_CENTERALIGNH, &CTCDesignView::OnTcCenteralignh)
        ON_COMMAND(ID_TC_ALIGNRIGHT, &CTCDesignView::OnTcAlignright)
        ON_COMMAND(ID_TC_ALIGNTOP, &CTCDesignView::OnTcAligntop)
        ON_COMMAND(ID_TC_CENTERALIGNV, &CTCDesignView::OnTcCenteralignv)
        ON_COMMAND(ID_TC_ALIGNBOTTOM, &CTCDesignView::OnTcAlignbottom)
        ON_COMMAND(ID_TC_SAMEHEIGHT, &CTCDesignView::OnTcSameheight)
        ON_COMMAND(ID_TC_SAMEWIDTH, &CTCDesignView::OnTcSamewidth)
        ON_COMMAND(ID_TC_SAMESIZE, &CTCDesignView::OnTcSamesize)
        ON_COMMAND(ID_TC_UNIFORMSPACEH, &CTCDesignView::OnTcUniformspaceh)
        ON_COMMAND(ID_TC_UNIFORMSPACEV, &CTCDesignView::OnTcUniformspacev)
        ON_COMMAND(ID_TC_CENTERWINV, &CTCDesignView::OnTcCenterwinv)
        ON_COMMAND(ID_TC_CENTERWINH, &CTCDesignView::OnTcCenterwinh)
        ON_WM_VSCROLL()
        ON_WM_HSCROLL()
        ON_COMMAND(ID_FILE_SAVE, &CTCDesignView::OnFileSave)
		ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, &CTCDesignView::OnUpdateEditDelete)
		ON_COMMAND(ID_EDIT_DELETE, &CTCDesignView::OnEditDelete)
		ON_COMMAND(ID_CTLPERTIES, &CTCDesignView::OnCtlperties)
		ON_WM_DESTROY()
		ON_COMMAND(ID_EDIT_UNDO, &CTCDesignView::OnEditUndo)
		ON_COMMAND(ID_EDIT_REDO, &CTCDesignView::OnEditRedo)
		ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CTCDesignView::OnUpdateEditUndo)
		ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CTCDesignView::OnUpdateEditRedo)
		ON_COMMAND(ID_EDIT_CUT, &CTCDesignView::OnEditCut)
		ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CTCDesignView::OnUpdateEditCut)
		ON_COMMAND(ID_EDIT_COPY, &CTCDesignView::OnEditCopy)
		ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CTCDesignView::OnUpdateEditCopy)
		ON_COMMAND(ID_EDIT_PASTE, &CTCDesignView::OnEditPaste)
		ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CTCDesignView::OnUpdateEditPaste)
		ON_WM_MOUSEWHEEL()
		ON_COMMAND(ID_EDIT_SELECT_ALL, &CTCDesignView::OnEditSelectAll)
		ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, &CTCDesignView::OnUpdateEditSelectAll)
		ON_COMMAND(ID_EDIT_CTLJUMPTOFILE, &CTCDesignView::OnEditCtljumptofile)
		ON_COMMAND(ID_EDIT_LOCK, &CTCDesignView::OnEditLock)
		ON_UPDATE_COMMAND_UI(ID_EDIT_LOCK, &CTCDesignView::OnUpdateEditLock)
		ON_COMMAND(ID_EDIT_UNLOCK, &CTCDesignView::OnEditUnlock)
		ON_UPDATE_COMMAND_UI(ID_EDIT_UNLOCK, &CTCDesignView::OnUpdateEditUnlock)
END_MESSAGE_MAP()

// CTCDesignView ����/����
CTCDesignView::CTCDesignView()
{
    // TODO: �ڴ˴���ӹ������
    m_TWinID=_T("");
}

CTCDesignView::~CTCDesignView()
{
}

BOOL CTCDesignView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: �ڴ˴�ͨ���޸�
    //  CREATESTRUCT cs ���޸Ĵ��������ʽ
    cs.style |= WS_HSCROLL |
        WS_VSCROLL;

    return CView::PreCreateWindow(cs);
}

// CTCDesignView ����

void CTCDesignView::OnDraw(CDC* /*pDC*/)
{
    CTCDesignDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CTCDesignView ��ӡ


void CTCDesignView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
    AFXPrintPreview(this);
#endif
}

BOOL CTCDesignView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // Ĭ��׼��
    return DoPreparePrinting(pInfo);
}

void CTCDesignView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CTCDesignView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: ��Ӵ�ӡ����е��������
}

void CTCDesignView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CTCDesignView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTCDesignView ���

#ifdef _DEBUG
void CTCDesignView::AssertValid() const
{
    CView::AssertValid();
}

void CTCDesignView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CTCDesignDoc* CTCDesignView::GetDocument() const // �ǵ��԰汾��������
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTCDesignDoc)));
    return (CTCDesignDoc*)m_pDocument;
}
#endif //_DEBUG


// CTCDesignView ��Ϣ�������


void CTCDesignView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    // TODO: �ڴ����ר�ô����/����û���
    ASSERT(!m_TWinID.IsEmpty());

    m_DiaDesign.Create(IDD_DIATCDESIGN,this);
	m_DiaDesign.m_TcDlgConfig.m_dlgID = m_TWinID;
    m_DiaDesign.ShowWindow(TRUE);
    //m_DiaDesign.SetFocus();

	int index = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getIndexByID(m_TWinID);
	if(-1 == index)
		return;
	wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(index);

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID = m_TWinID;

	CString winInfo = temp->winInfo;
	m_DiaDesign.LoadTWinRCFileToMem(temp->id,winInfo);
}

void CTCDesignView::OnUpdateTcAlignleft(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcCenteralignh(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcAlignright(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcAligntop(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcCenteralignv(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcAlignbottom(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcSameheight(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcSamewidth(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcSamesize(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2||TabCtrlSelectMoreTwo())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcUniformspaceh(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if( m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize() > 2 || TabCtrlSelectMoreThree())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcUniformspacev(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize() > 2 || TabCtrlSelectMoreThree())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcCenterwinv(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1||TabCtrlSelectMoreOne())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateTcCenterwinh(CCmdUI *pCmdUI)
{
    // TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1||TabCtrlSelectMoreOne())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}

void CTCDesignView::OnDisableUpdate(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(FALSE);
}

void CTCDesignView::OnTcAlignleft()
{
    // TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.AlignLeft();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignLeft();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignLeft();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.AlignLeft();*/
}


void CTCDesignView::OnTcCenteralignh()
{
    // TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.CenterAlignH();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterAlignH();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterAlignH();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.CenterAlignH();*/
}


void CTCDesignView::OnTcAlignright()
{
    // TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.AlignRight();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignRight();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignRight();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.AlignRight();*/
}

void CTCDesignView::OnTcAligntop()
{
    // TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.AlignTop();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignTop();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignTop();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.AlignTop();*/
}

void CTCDesignView::OnTcCenteralignv()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.CenterAlignV();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterAlignV();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterAlignV();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.CenterAlignV();*/
}

void CTCDesignView::OnTcAlignbottom()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.AlignBottom();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignBottom();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.AlignBottom();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.AlignBottom();*/
}


void CTCDesignView::OnTcSameheight()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.SetSameHeight();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.SetSameHeight();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.SetSameHeight();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.SetSameHeight();*/
}


void CTCDesignView::OnTcSamewidth()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.SetSameWidth();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.SetSameWidth();
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.SetSameWidth();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.SetSameWidth();*/
}


void CTCDesignView::OnTcSamesize()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 2 )
	{
		m_DiaDesign.m_ControlTracker.SetSameSize();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.SetSameSize();	
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.SetSameSize();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.SetSameSize();*/
}

void CTCDesignView::OnTcUniformspaceh()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num > 2 )
	{
		m_DiaDesign.m_ControlTracker.UniformSpaceH();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.UniformSpaceH();			
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.UniformSpaceH();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.UniformSpaceH();*/
}


void CTCDesignView::OnTcUniformspacev()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num > 2 )
	{
		m_DiaDesign.m_ControlTracker.UniformSpaceV();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.UniformSpaceV();			
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreTwo())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.UniformSpaceV();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.UniformSpaceV();*/
}


void CTCDesignView::OnTcCenterwinv()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 1 )
	{
		m_DiaDesign.m_ControlTracker.CenterWindowV();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterWindowV();			
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreOne())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterWindowV();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.CenterWindowV();*/
}


void CTCDesignView::OnTcCenterwinh()
{
	// TODO: �ڴ���������������
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if( num >= 1 )
	{
		m_DiaDesign.m_ControlTracker.CenterWindowH();
	}
	else
	{
		int numAll = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<numAll;i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1)
					{
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterWindowH();			
					}
				}
			}
		}
	}
	/*if(TabCtrlSelectMoreOne())
	{
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
		{
			if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{			
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.CenterWindowH();									
				}
			}
		}
	}
	else 
		m_DiaDesign.m_ControlTracker.CenterWindowH();*/
}


BOOL CTCDesignView::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���

    if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_LEFT)
    {
		if(TabCtrlSelectMoreOne())
		{
			for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
			{
				if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
				{
					CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{			
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.MoveLeftOne();									
					}
				}
			}
		}
		else 
			m_DiaDesign.m_ControlTracker.MoveLeftOne();

        return TRUE;
    }
    if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RIGHT)
    {
		if(TabCtrlSelectMoreOne())
		{
			for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
			{
				if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
				{
					CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{			
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.MoveRightOne();									
					}
				}
			}
		}
		else 
			m_DiaDesign.m_ControlTracker.MoveRightOne();

        return TRUE;
    }
    if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_UP)
    {
		if(TabCtrlSelectMoreOne())
		{
			for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
			{
				if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
				{
					CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{			
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.MoveTopOne();									
					}
				}
			}
		}
		else 
			m_DiaDesign.m_ControlTracker.MoveTopOne();
        return TRUE;
    }
    if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_DOWN)
    {
		if(TabCtrlSelectMoreOne())
		{
			for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
			{
				if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
				{
					CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{			
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.MoveBottomOne();									
					}
				}
			}
		}
		else 
			m_DiaDesign.m_ControlTracker.MoveBottomOne();
        return TRUE;
    }
	/////////////////////////////����Deleteɾ���ؼ�����/////////////////////////
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_DELETE)
	{
	   	DeleteCtrl();
		return TRUE;
	}

	/////////////////////////////////////////////���Undo/Redo�������////////////////////////////////////////
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'Z' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
	{
		//�����Undo����Ϊ��
		if(m_DiaDesign.CanUndo())
		{
			m_DiaDesign.Undo();
			m_DiaDesign.Invalidate(TRUE);
			///////////////////////////////////ɾ���Ժ���ʾ�����Ը���Ϊ�����������/////////////////////////////////////////////
			CString title;
			m_DiaDesign.GetWindowTextW(title);
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(title,&m_DiaDesign);
			///////////////////////////////////ɾ���Ժ���ʾ�����Ը���Ϊ�����������/////////////////////////////////////////////
		}
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'Y' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
	{
		//�ɽ���Redo����
		if(m_DiaDesign.CanRedo())
		{
			m_DiaDesign.Redo();
			//ˢ����Ƥ����ʾ
			m_DiaDesign.Invalidate(TRUE);
		}
		return TRUE;
	}
	/////////////////////////////////////////////���Undo/Redo�������////////////////////////////////////////

	////////////////////////////////////////////Ctrl+A���пؼ�ȫ��ѡ�еĲ���//////////////////////////////////
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'A' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
	{
		int ctrlCount = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		if(ctrlCount<1)
			return TRUE;

		for(int i=0;i<ctrlCount;i++)
		{
			m_DiaDesign.m_ControlTracker.Select(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]);		
		}
		//����Ĭ�ϻ�׼�ؼ�
		m_DiaDesign.m_ControlTracker.CreateDefaultBasicCtrl();
		return TRUE;
	}
    
	if( pMsg->message == WM_RBUTTONDOWN )
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndDesignToolBox.ResetSelectToolType();
	}
	return CView::PreTranslateMessage(pMsg);
}


void CTCDesignView::OnEditUndo()
{
	// TODO: �ڴ���������������
	m_DiaDesign.Undo();
	m_DiaDesign.Invalidate(TRUE);
	///////////////////////////////////ɾ���Ժ���ʾ�����Ը���Ϊ�����������/////////////////////////////////////////////
	CString title;
	m_DiaDesign.GetWindowTextW(title);
	//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(title,&m_DiaDesign);
}


void CTCDesignView::OnEditRedo()
{
	// TODO: �ڴ���������������
	m_DiaDesign.Redo();
}


void CTCDesignView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_DiaDesign.CanUndo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������	
	if(m_DiaDesign.CanRedo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}




void CTCDesignView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    SCROLLINFO scrollinfo;  
    GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
    switch (nSBCode)  
    {  
    case SB_BOTTOM:  
        ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);  
        scrollinfo.nPos = scrollinfo.nMax;  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    case SB_TOP:  
        ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);  
        scrollinfo.nPos = scrollinfo.nMin;  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    case SB_LINEUP:  
        scrollinfo.nPos -= 1;  
        if(scrollinfo.nPos < scrollinfo.nMin)
        {  
            scrollinfo.nPos = scrollinfo.nMin;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,10);  
        break;  
    case SB_LINEDOWN:  
        scrollinfo.nPos += 1;  
        if (scrollinfo.nPos>scrollinfo.nMax)  
        {  
            scrollinfo.nPos = scrollinfo.nMax;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,-10);  
        break;  
    case SB_PAGEUP:  
        scrollinfo.nPos -= 5;  
        if (scrollinfo.nPos < scrollinfo.nMin)
        {  
            scrollinfo.nPos = scrollinfo.nMin;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,10*5);  
        break;  
    case SB_PAGEDOWN:  
        scrollinfo.nPos += 5;  
        if (scrollinfo.nPos>scrollinfo.nMax)  
        {  
            scrollinfo.nPos = scrollinfo.nMax;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,-10*5);  
        break;  
    case SB_ENDSCROLL:  
        // MessageBox("SB_ENDSCROLL");  
        break;  
    case SB_THUMBPOSITION:  
        // ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
        // scrollinfo.nPos = nPos;  
        // SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    case SB_THUMBTRACK:  
        ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
        scrollinfo.nPos = nPos;  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    }
    CView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CTCDesignView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    SCROLLINFO scrollinfo; 
    GetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
    //this->Invalidate(false);
    //InvalidateRect(NULL,TRUE); 
    switch (nSBCode) 
    { 
    case SB_LEFT: 
        ScrollWindow((scrollinfo.nPos-scrollinfo.nMin)*10,0); 
        scrollinfo.nPos = scrollinfo.nMin; 
        SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
        break; 
    case SB_RIGHT: 
        ScrollWindow((scrollinfo.nPos-scrollinfo.nMax)*10,0); 
        scrollinfo.nPos = scrollinfo.nMax; 
        SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
        break; 
    case SB_LINELEFT: 
        scrollinfo.nPos -= 1; 
        if (scrollinfo.nPos)
        { 
            scrollinfo.nPos = scrollinfo.nMin; 
            break; 
        } 
        SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
        ScrollWindow(10,0); 
        break; 
    case SB_LINERIGHT: 
        scrollinfo.nPos += 1; 
        if (scrollinfo.nPos>scrollinfo.nMax) 
        { 
            scrollinfo.nPos = scrollinfo.nMax; 
            break; 
        } 
        SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
        ScrollWindow(-10,0); 
        break; 
    case SB_PAGELEFT: 
        scrollinfo.nPos -= 5; 
        if (scrollinfo.nPos)
        { 
            scrollinfo.nPos = scrollinfo.nMin; 
            break; 
        } 
        SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
        ScrollWindow(10*5,0); 
        break; 
    case SB_PAGERIGHT: 
        scrollinfo.nPos += 5; 
        if (scrollinfo.nPos>scrollinfo.nMax) 
        { 
            scrollinfo.nPos = scrollinfo.nMax; 
            break; 
        } 
        SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
        ScrollWindow(-10*5,0); 
        break; 
    case SB_THUMBPOSITION: 
        break; 
    case SB_THUMBTRACK: 
        ScrollWindow((scrollinfo.nPos-nPos)*10,0); 
        scrollinfo.nPos = nPos; 
        SetScrollInfo(SB_HORZ,&scrollinfo,SIF_ALL); 
        break; 
    case SB_ENDSCROLL: 
        break; 
    } 
    //m_nHScrollPos = scrollinfo.nPos*10;

    CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTCDesignView::OnFileSave()
{
    // TODO: �ڴ���������������
    m_DiaDesign.SaveTWinRCFileToMem();
}


void CTCDesignView::DeleteCtrl()
{
	int select = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if(select>0)
	{
		while(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>0)
		{
			CWnd *pSelectWnd;
			pSelectWnd = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCWnd();
			CString currentID = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlID();
			CControlRect * pSelectRect =  m_DiaDesign.m_ControlTracker.GetRectByID(currentID);
			TCTOOLTYPE type = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlType();
			/////////////////////////��m_arrRectanglesSelected���Ƴ�/////////////////////
			if(m_DiaDesign.m_ControlTracker.DeSelect(pSelectRect))
			{
				//��Command�л��Ƴ���
				//////////////////////��m_arrRectanglesAll���Ƴ�/////////////////////
				//m_DiaDesign.m_ControlTracker.RemoveFromAll(pSelectWnd);
				switch(type)
				{
				case TCTT_STATIC:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_STATIC,TCPARENT_DESDIN);
					break;
				case TCTT_BUTTON:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_BUTTON,TCPARENT_DESDIN);
					break;
				case TCTT_EDIT:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_EDIT,TCPARENT_DESDIN);
					break;
				case TCTT_RADIO:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_RADIO,TCPARENT_DESDIN);
					break;
				case TCTT_CHECK:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_CHECK,TCPARENT_DESDIN);
					break;
				case TCTT_COMBOBOX:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_COMBOBOX,TCPARENT_DESDIN);
					break;
				case TCTT_GROUP:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_GROUP,TCPARENT_DESDIN);
					break;
				case TCTT_LISTBOX:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_LISTBOX,TCPARENT_DESDIN);
					break;
				case TCTT_PROGRESS:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_PROGRESS,TCPARENT_DESDIN);
					break;
				case TCTT_DATE:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_DATE,TCPARENT_DESDIN);
					break;
				case TCTT_PICTURE:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_PICTURE,TCPARENT_DESDIN);
					break;
				case TCTT_SLIDER:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_SLIDER,TCPARENT_DESDIN);
					break;
				case TCTT_PATH:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_PATH,TCPARENT_DESDIN);
					break;
				case TCTT_TIMER:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_TIMER,TCPARENT_DESDIN);
					break;
				case TCTT_HOTKEY:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_HOTKEY,TCPARENT_DESDIN);
					break;
				case TCTT_EXPLORER:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_EXPLORER,TCPARENT_DESDIN);
					break;
				case TCTT_GRIDCTRL:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_GRIDCTRL,TCPARENT_DESDIN);
					break;
				//Tabֻ��DiaTCDesign�в���
				case TCTT_TAB:
					//for(int i=0;i<((CTCTab*)pSelectWnd)->getTabPageCount();i++)
					//{
					//	while(((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize()>0)
					//	{
					//			CWnd *pTabDelCtrl;
					//			pTabDelCtrl = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCWnd();
					//			CString delCtrlId = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlID();
					//			CControlRect * pdelCtrlRect = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.GetRectByID(delCtrlId);
					//			TCTOOLTYPE delCtrlType = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlType();
					//			//..�Ӷ�Ӧ��m_ControlTracker���Ƴ�,����������Ƴ�
					//			//((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.RemoveFromAll(pTabDelCtrl);
					//			//�������ɾ��Tab�����ӿؼ�
					//			DeleteCtrlsInTab((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i],pTabDelCtrl,delCtrlType);
					//	}
					//}
					
					//ɾ��Tab�ؼ��Լ�TabPage�е��ӿؼ�
					m_DiaDesign.ClearTabCtrl((CTCTab*)pSelectWnd,pSelectRect->m_rect);
					m_DiaDesign.Invalidate(TRUE);
					break;
				default:
					break;
				}			
				
				//delete pSelectWnd;
				pSelectWnd=NULL;
				pSelectRect=NULL;
				currentID=_T("");
			}
		}
	}
	else
	{
		////////////////////////////////���������Table�ؼ���ɾ�������Ļ�////////////////////////
		int ctrlNum = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<ctrlNum;i++)
		{
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
			if( ctrlType == TCTT_TAB)
			{
				CTCTab* pTab = (CTCTab*)(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int tabSelect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
					if( tabSelect > 0 )
					{
						while(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>0)
						{
							CWnd *pTabSelectCtrl;
							pTabSelectCtrl = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCWnd();
							CString tabCurrentID = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlID();
							CControlRect * ptabSelectRect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.GetRectByID(tabCurrentID);
							TCTOOLTYPE curselectType = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlType();
							if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelect(ptabSelectRect))
							{
								//��Command�л��Ƴ���
								//((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.RemoveFromAll(pTabSelectCtrl);
								switch(curselectType)
								{
								case TCTT_STATIC:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_STATIC,TCPARENT_TABPAGE);
									break;
								case TCTT_BUTTON:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_BUTTON,TCPARENT_TABPAGE);
									break;
								case TCTT_EDIT:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_EDIT,TCPARENT_TABPAGE);
									break;
								case TCTT_RADIO:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_RADIO,TCPARENT_TABPAGE);
									break;
								case TCTT_CHECK:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_CHECK,TCPARENT_TABPAGE);
									break;
								case TCTT_COMBOBOX:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_COMBOBOX,TCPARENT_TABPAGE);
									break;
								case TCTT_GROUP:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_GROUP,TCPARENT_TABPAGE);
									break;
								case TCTT_LISTBOX:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_LISTBOX,TCPARENT_TABPAGE);
									break;
								case TCTT_PROGRESS:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_PROGRESS,TCPARENT_TABPAGE);
									break;
								case TCTT_DATE:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_DATE,TCPARENT_TABPAGE);
									break;
								case TCTT_PICTURE:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_PICTURE,TCPARENT_TABPAGE);
									break;
								case TCTT_SLIDER:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_SLIDER,TCPARENT_TABPAGE);
									break;
								case TCTT_PATH:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_PATH,TCPARENT_TABPAGE);
									break;
								case TCTT_HOTKEY:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_HOTKEY,TCPARENT_TABPAGE);
									break;
								case TCTT_EXPLORER:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_EXPLORER,TCPARENT_TABPAGE);
									break;
								case TCTT_GRIDCTRL:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_GRIDCTRL,TCPARENT_TABPAGE);
									break;
								default:
									break;
								}		
								//////////////////////��m_arrRectanglesAll���Ƴ�/////////////////////
								//delete pTabSelectCtrl;
								pTabSelectCtrl=NULL;
								tabCurrentID=_T("");
								ptabSelectRect=NULL;

							}
						}
					}
				}
			}
		}
	}			
	m_DiaDesign.Invalidate(true);

	//9-3�޸�
	///////////////////////////////////ɾ���Ժ���ʾ�����Ը���Ϊ�����������/////////////////////////////////////////////
	CString title;
	m_DiaDesign.GetWindowTextW(title);
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(title,&m_DiaDesign);
	///////////////////////////////////ɾ���Ժ���ʾ�����Ը���Ϊ�����������/////////////////////////////////////////////

	return;
}


void CTCDesignView::DeleteCtrlsInTab(CTCTabPage *pTabPage,CWnd *pCtrl,TCTOOLTYPE ctrlType)
{
	switch(ctrlType)
	{
	case TCTT_STATIC:
		pTabPage->ClearCtrl(pCtrl,TCTT_STATIC,TCPARENT_TABPAGE);
		break;
	case TCTT_BUTTON:
		pTabPage->ClearCtrl(pCtrl,TCTT_BUTTON,TCPARENT_TABPAGE);
		break;
	case TCTT_EDIT:
		pTabPage->ClearCtrl(pCtrl,TCTT_EDIT,TCPARENT_TABPAGE);
		break;
	case TCTT_RADIO:
		pTabPage->ClearCtrl(pCtrl,TCTT_RADIO,TCPARENT_TABPAGE);
		break;
	case TCTT_CHECK:
		pTabPage->ClearCtrl(pCtrl,TCTT_CHECK,TCPARENT_TABPAGE);
		break;
	case TCTT_COMBOBOX:
		pTabPage->ClearCtrl(pCtrl,TCTT_COMBOBOX,TCPARENT_TABPAGE);
		break;
	case TCTT_GROUP:
		pTabPage->ClearCtrl(pCtrl,TCTT_GROUP,TCPARENT_TABPAGE);
		break;
	case TCTT_LISTBOX:
		pTabPage->ClearCtrl(pCtrl,TCTT_LISTBOX,TCPARENT_TABPAGE);
		break;
	case TCTT_PROGRESS:
		pTabPage->ClearCtrl(pCtrl,TCTT_PROGRESS,TCPARENT_TABPAGE);
		break;
	case TCTT_DATE:
		pTabPage->ClearCtrl(pCtrl,TCTT_DATE,TCPARENT_TABPAGE);
		break;
	case TCTT_PICTURE:
		pTabPage->ClearCtrl(pCtrl,TCTT_PICTURE,TCPARENT_TABPAGE);
		break;
	case TCTT_SLIDER:
		pTabPage->ClearCtrl(pCtrl,TCTT_SLIDER,TCPARENT_TABPAGE);
		break;
	case TCTT_PATH:
		pTabPage->ClearCtrl(pCtrl,TCTT_PATH,TCPARENT_TABPAGE);
		break;
	case TCTT_HOTKEY:
		pTabPage->ClearCtrl(pCtrl,TCTT_HOTKEY,TCPARENT_TABPAGE);
		break;
	case TCTT_EXPLORER:
		pTabPage->ClearCtrl(pCtrl,TCTT_EXPLORER,TCPARENT_TABPAGE);
		break;
	case TCTT_GRIDCTRL:
		pTabPage->ClearCtrl(pCtrl,TCTT_GRIDCTRL,TCPARENT_TABPAGE);
		break;
	default:
		break;
	}
}

BOOL CTCDesignView::TabCtrlSelectMoreOne()
{
	for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
	{
		if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
			for(int j=0;j<pTab->getTabPageCount();j++)
			{			
				if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1)
					return true;				
			}
		}
	}
	return false;
}

BOOL CTCDesignView::TabCtrlCopyMoreOne()
{
	for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
	{
		if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
			for(int j=0;j<pTab->getTabPageCount();j++)
			{			
				if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy.GetSize()>=1)
					return true;				
			}
		}
	}
	return false;
}

BOOL CTCDesignView::TabCtrlSelectMoreTwo()
{
	for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
	{
		if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
			for(int j=0;j<pTab->getTabPageCount();j++)
			{			
				if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>=2)
					return true;				
			}
		}
	}
	return false;
}

BOOL CTCDesignView::TabCtrlSelectMoreThree()
{
	for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
	{
		if( m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab *pTab = (CTCTab *)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
			for(int j=0;j<pTab->getTabPageCount();j++)
			{			
				if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize() >= 3)
					return true;				
			}
		}
	}
	return false;
}

void CTCDesignView::OnUpdateEditDelete(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
    if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1||TabCtrlSelectMoreOne())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnEditDelete()
{
	// TODO: �ڴ���������������
	DeleteCtrl();
}
void CTCDesignView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: �ڴ����ר�ô����/����û���
	//bActivateָ������ͼ��Ҫ�������Ҫ��������״̬
	//pActivateView ָ��Ҫ�������ͼ��ָ��
	//pDeactiveView ָ��Ҫ��������״̬����ͼ�����ָ��
	if(bActivate)
	{
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID = m_TWinID;

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCSetFileInfoVisible(FALSE);

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_currentViewType = TCVIEW_DESIGN;

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->ResetPropertyCombox();

		BOOL winPerpro = FALSE;

		int index = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i = 0;i < index ; i++)
		{
			if(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CString idTab = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();

				//Tab��ID
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->InsertCtrlToProCom(idTab);

				//m_wndObjectCombo.AddString(idTab);
				CTCTab* pTab = (CTCTab*)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int pageIndex = j;
					int pageSize = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					for(int k = 0;k < pageSize; k++)
					{
						CString idInTab = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlID();
						//Tab����Ԫ�ص�ID
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->InsertCtrlToProCom(idInTab);
						//m_wndObjectCombo.AddString(idInTab);
					}
					
					///////////////////////////////��ʱ���´��ڻ��߿ؼ����ԵĲ���/////////////////////////////
					
					///////////////////////////////��ʱ���´��ڻ��߿ؼ����ԵĲ���/////////////////////////////
				}
			}
			else
			{
				CString idCtrls = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->InsertCtrlToProCom(idCtrls);
				//m_wndObjectCombo.AddString(idCtrls);
				
			}

		}
		int selectSize = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
		if(selectSize == 0)
		{
			for( int count = 0; count < index ; count ++ )
			{
				if(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[count]->GetCtlType()==TCTT_TAB)
				{
					CTCTab* pTab = (CTCTab*)m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[count]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{
						int pageSelectSize = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
						if(pageSelectSize == 0)
							continue;
						else if(pageSelectSize == 1)
						{
							winPerpro = TRUE;
							TCTOOLTYPE ctrlType = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlType();
							CString ctrlID = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlID();
							CWnd * pCtrl = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCWnd();
							switch(ctrlType)
							{
							case TCTT_STATIC:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(ctrlID,(CTCStatic*)pCtrl);	
								break;
							case TCTT_BUTTON:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(ctrlID,(CTCButton*)pCtrl);
								break;
							case TCTT_EDIT:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(ctrlID,(CTCEdit*)pCtrl);	
								break;
							case TCTT_RADIO:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(ctrlID,(CTCRadio*)pCtrl);
								break;
							case TCTT_CHECK:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(ctrlID,(CTCCheck*)pCtrl);
								break;
							case TCTT_COMBOBOX:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(ctrlID,(CTCComboBox*)pCtrl);
								break;
							case TCTT_GROUP:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(ctrlID,(CTCGroupBox*)pCtrl);
								break;
							case TCTT_LISTBOX:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(ctrlID,(CTCListBox*)pCtrl);
								break;
							case TCTT_PROGRESS:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(ctrlID,(CTCProgress*)pCtrl);
								break;
							case TCTT_DATE:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(ctrlID,(CTCDate*)pCtrl);
								break;
							case TCTT_PICTURE:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(ctrlID,(CTCPicture*)pCtrl);
								break;
							case TCTT_SLIDER:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(ctrlID,(CTCSlider*)pCtrl);
								break;
							case TCTT_PATH:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(ctrlID,(CTCBrowse*)pCtrl);
								break;
							case TCTT_TIMER:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(ctrlID,(CTCTimer*)pCtrl);
								break;
							case TCTT_HOTKEY:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(ctrlID,(CTCHotKeyCtrl*)pCtrl);
								break;
							case TCTT_TAB:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(ctrlID,(CTCTab*)pCtrl);
								break;
							case TCTT_EXPLORER:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(ctrlID,(CTCExplorer*)pCtrl);
								break;
							case TCTT_GRIDCTRL:
								((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(ctrlID,(CGridCtrl*)pCtrl);
								break;
							default:
								break;

							}
						}
						else if(pageSelectSize > 1)
						{
							winPerpro = TRUE;
							for(int k = 0;k < pageSelectSize; k++)
							{
								TCTOOLTYPE ctrlType = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[k]->GetCtlType();
								CString ctrlID = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[k]->GetCtlID();
								CWnd * pCtrl = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[k]->GetCWnd();
								if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[k]->TCGetHighSelected())
								{
									switch(ctrlType)
									{
									case TCTT_STATIC:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(ctrlID,(CTCStatic*)pCtrl);	
										break;
									case TCTT_BUTTON:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(ctrlID,(CTCButton*)pCtrl);
										break;
									case TCTT_EDIT:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(ctrlID,(CTCEdit*)pCtrl);	
										break;
									case TCTT_RADIO:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(ctrlID,(CTCRadio*)pCtrl);
										break;
									case TCTT_CHECK:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(ctrlID,(CTCCheck*)pCtrl);
										break;
									case TCTT_COMBOBOX:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(ctrlID,(CTCComboBox*)pCtrl);
										break;
									case TCTT_GROUP:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(ctrlID,(CTCGroupBox*)pCtrl);
										break;
									case TCTT_LISTBOX:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(ctrlID,(CTCListBox*)pCtrl);
										break;
									case TCTT_PROGRESS:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(ctrlID,(CTCProgress*)pCtrl);
										break;
									case TCTT_DATE:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(ctrlID,(CTCDate*)pCtrl);
										break;
									case TCTT_PICTURE:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(ctrlID,(CTCPicture*)pCtrl);
										break;
									case TCTT_SLIDER:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(ctrlID,(CTCSlider*)pCtrl);
										break;
									case TCTT_PATH:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(ctrlID,(CTCBrowse*)pCtrl);
										break;
									case TCTT_TIMER:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(ctrlID,(CTCTimer*)pCtrl);
										break;
									case TCTT_HOTKEY:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(ctrlID,(CTCHotKeyCtrl*)pCtrl);
										break;
									case TCTT_TAB:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(ctrlID,(CTCTab*)pCtrl);
										break;
									case TCTT_EXPLORER:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(ctrlID,(CTCExplorer*)pCtrl);
										break;
									case TCTT_GRIDCTRL:
										((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(ctrlID,(CGridCtrl*)pCtrl);
										break;
									default:
										break;
									}
								}

							}
						}
					}
				}
			}
		}
		else if(selectSize == 1)
		{
			winPerpro = TRUE;
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlType();
			CString ctrlID = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlID();
			CWnd * pCtrl = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCWnd();
			switch(ctrlType)
			{
			case TCTT_STATIC:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(ctrlID,(CTCStatic*)pCtrl);	
				break;
			case TCTT_BUTTON:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(ctrlID,(CTCButton*)pCtrl);
				break;
			case TCTT_EDIT:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(ctrlID,(CTCEdit*)pCtrl);	
				break;
			case TCTT_RADIO:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(ctrlID,(CTCRadio*)pCtrl);
				break;
			case TCTT_CHECK:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(ctrlID,(CTCCheck*)pCtrl);
				break;
			case TCTT_COMBOBOX:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(ctrlID,(CTCComboBox*)pCtrl);
				break;
			case TCTT_GROUP:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(ctrlID,(CTCGroupBox*)pCtrl);
				break;
			case TCTT_LISTBOX:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(ctrlID,(CTCListBox*)pCtrl);
				break;
			case TCTT_PROGRESS:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(ctrlID,(CTCProgress*)pCtrl);
				break;
			case TCTT_DATE:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(ctrlID,(CTCDate*)pCtrl);
				break;
			case TCTT_PICTURE:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(ctrlID,(CTCPicture*)pCtrl);
				break;
			case TCTT_SLIDER:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(ctrlID,(CTCSlider*)pCtrl);
				break;
			case TCTT_PATH:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(ctrlID,(CTCBrowse*)pCtrl);
				break;
			case TCTT_TIMER:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(ctrlID,(CTCTimer*)pCtrl);
				break;
			case TCTT_HOTKEY:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(ctrlID,(CTCHotKeyCtrl*)pCtrl);
				break;
			case TCTT_TAB:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(ctrlID,(CTCTab*)pCtrl);
				break;
			case TCTT_EXPLORER:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(ctrlID,(CTCExplorer*)pCtrl);
				break;
			case TCTT_GRIDCTRL:
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(ctrlID,(CGridCtrl*)pCtrl);
				break;
			default:
				break;
			}
		}
		//if(selectSize > 1)
		else
		{
			winPerpro = TRUE;
			for( int num = 0;num < selectSize ;num ++)
			{
				if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[num]->TCGetHighSelected())
				{
					TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[num]->GetCtlType();
					CString ctrlID = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[num]->GetCtlID();
					CWnd *pCtrl = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[num]->GetCWnd();
					switch(ctrlType)
					{
					case TCTT_STATIC:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(ctrlID,(CTCStatic*)pCtrl);	
						break;
					case TCTT_BUTTON:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(ctrlID,(CTCButton*)pCtrl);
						break;
					case TCTT_EDIT:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(ctrlID,(CTCEdit*)pCtrl);	
						break;
					case TCTT_RADIO:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(ctrlID,(CTCRadio*)pCtrl);
						break;
					case TCTT_CHECK:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(ctrlID,(CTCCheck*)pCtrl);
						break;
					case TCTT_COMBOBOX:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(ctrlID,(CTCComboBox*)pCtrl);
						break;
					case TCTT_GROUP:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(ctrlID,(CTCGroupBox*)pCtrl);
						break;
					case TCTT_LISTBOX:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(ctrlID,(CTCListBox*)pCtrl);
						break;
					case TCTT_PROGRESS:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(ctrlID,(CTCProgress*)pCtrl);
						break;
					case TCTT_DATE:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(ctrlID,(CTCDate*)pCtrl);
						break;
					case TCTT_PICTURE:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(ctrlID,(CTCPicture*)pCtrl);
						break;
					case TCTT_SLIDER:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(ctrlID,(CTCSlider*)pCtrl);
						break;
					case TCTT_PATH:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(ctrlID,(CTCBrowse*)pCtrl);
						break;
					case TCTT_TIMER:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(ctrlID,(CTCTimer*)pCtrl);
						break;
					case TCTT_HOTKEY:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(ctrlID,(CTCHotKeyCtrl*)pCtrl);
						break;
					case TCTT_TAB:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(ctrlID,(CTCTab*)pCtrl);
						break;
					case TCTT_EXPLORER:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(ctrlID,(CTCExplorer*)pCtrl);
						break;
					case TCTT_GRIDCTRL:
						((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(ctrlID,(CGridCtrl*)pCtrl);
						break;
					default:
						break;
					}
				}
			}
		}		
		if(!winPerpro)   
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(m_TWinID,&m_DiaDesign);
	}	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CTCDesignView::OnCtlperties()
{
	// TODO: �ڴ���������������
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->ActivePerprotyWnd();
}


void CTCDesignView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	 m_DiaDesign.SaveTWinRCFileToMem();
}


void CTCDesignView::OnEditCut()
{
	// TODO: �ڴ���������������
	//���е���ɾ��
	//DeleteCtrl();
	int select = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if(select>0)
	{
		//���������и��������ÿ�
		RemoveCopyRetangles();
		//m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.RemoveAll();
		while(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>0)
		{
			CWnd *pSelectWnd;
			pSelectWnd = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCWnd();
			CString currentID = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlID();
			CControlRect* pSelectRect =  m_DiaDesign.m_ControlTracker.GetRectByID(currentID);

			//��ӵ�������������
			m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.Add(pSelectRect);

			TCTOOLTYPE type = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlType();
			/////////////////////////��m_arrRectanglesSelected���Ƴ�/////////////////////
			if(m_DiaDesign.m_ControlTracker.DeSelect(pSelectRect))
			{
				//��Command�л��Ƴ���
				//////////////////////��m_arrRectanglesAll���Ƴ�/////////////////////
				//m_DiaDesign.m_ControlTracker.RemoveFromAll(pSelectWnd);
				switch(type)
				{
				case TCTT_STATIC:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_STATIC,TCPARENT_DESDIN);
					break;
				case TCTT_BUTTON:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_BUTTON,TCPARENT_DESDIN);
					break;
				case TCTT_EDIT:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_EDIT,TCPARENT_DESDIN);
					break;
				case TCTT_RADIO:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_RADIO,TCPARENT_DESDIN);
					break;
				case TCTT_CHECK:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_CHECK,TCPARENT_DESDIN);
					break;
				case TCTT_COMBOBOX:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_COMBOBOX,TCPARENT_DESDIN);
					break;
				case TCTT_GROUP:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_GROUP,TCPARENT_DESDIN);
					break;
				case TCTT_LISTBOX:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_LISTBOX,TCPARENT_DESDIN);
					break;
				case TCTT_PROGRESS:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_PROGRESS,TCPARENT_DESDIN);
					break;
				case TCTT_DATE:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_DATE,TCPARENT_DESDIN);
					break;
				case TCTT_PICTURE:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_PICTURE,TCPARENT_DESDIN);
					break;
				case TCTT_SLIDER:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_SLIDER,TCPARENT_DESDIN);
					break;
				case TCTT_PATH:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_PATH,TCPARENT_DESDIN);
					break;
				case TCTT_TIMER:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_TIMER,TCPARENT_DESDIN);
					break;
				case TCTT_HOTKEY:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_HOTKEY,TCPARENT_DESDIN);
					break;
				case TCTT_EXPLORER:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_EXPLORER,TCPARENT_DESDIN);
					break;
				case TCTT_GRIDCTRL:
					m_DiaDesign.ClearCtrl(pSelectWnd,TCTT_GRIDCTRL,TCPARENT_DESDIN);
					break;
				//Tabֻ��DiaTCDesign�в���
				case TCTT_TAB:
					//for(int i=0;i<((CTCTab*)pSelectWnd)->getTabPageCount();i++)
					//{
					//	while(((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize()>0)
					//	{
					//			CWnd *pTabDelCtrl;
					//			pTabDelCtrl = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCWnd();
					//			CString delCtrlId = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlID();
					//			CControlRect * pdelCtrlRect = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.GetRectByID(delCtrlId);
					//			TCTOOLTYPE delCtrlType = ((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlType();
					//			//..�Ӷ�Ӧ��m_ControlTracker���Ƴ�,����������Ƴ�
					//			//((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i])->m_ControlTracker.RemoveFromAll(pTabDelCtrl);
					//			//�������ɾ��Tab�����ӿؼ�
					//			DeleteCtrlsInTab((CTCTabPage*)((CTCTab*)pSelectWnd)->m_Pages[i],pTabDelCtrl,delCtrlType);
					//	}
					//}
					
					//ɾ��Tab�ؼ��Լ�TabPage�е��ӿؼ�
					m_DiaDesign.ClearTabCtrl((CTCTab*)pSelectWnd,pSelectRect->m_rect);
					m_DiaDesign.Invalidate(TRUE);
					break;
				default:
					break;
				}			
				
				//delete pSelectWnd;
				pSelectWnd=NULL;
				pSelectRect=NULL;
				currentID=_T("");
			}
		}
	}
	else
	{
		////////////////////////////////���������Table�ؼ���ɾ�������Ļ�////////////////////////
		int ctrlNum = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<ctrlNum;i++)
		{
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
			if( ctrlType == TCTT_TAB)
			{
				CTCTab* pTab = (CTCTab*)(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int tabSelect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
					if( tabSelect > 0 )
					{
						//������Tab�еĸ��ƵĿؼ�ȫ���ÿ�
						RemoveCopyRetangles();
						//m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.RemoveAll();
						//((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy.RemoveAll();
						while(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()>0)
						{
							CWnd *pTabSelectCtrl;
							pTabSelectCtrl = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCWnd();
							CString tabCurrentID = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlID();
							CControlRect* ptabSelectRect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.GetRectByID(tabCurrentID);

							/////��ӵ�Tab�еĸ���������
							((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy.Add(ptabSelectRect);

							TCTOOLTYPE curselectType = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[0]->GetCtlType();
							if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelect(ptabSelectRect))
							{
								//��Command�л��Ƴ���
								//((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.RemoveFromAll(pTabSelectCtrl);
								switch(curselectType)
								{
								case TCTT_STATIC:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_STATIC,TCPARENT_TABPAGE);
									break;
								case TCTT_BUTTON:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_BUTTON,TCPARENT_TABPAGE);
									break;
								case TCTT_EDIT:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_EDIT,TCPARENT_TABPAGE);
									break;
								case TCTT_RADIO:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_RADIO,TCPARENT_TABPAGE);
									break;
								case TCTT_CHECK:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_CHECK,TCPARENT_TABPAGE);
									break;
								case TCTT_COMBOBOX:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_COMBOBOX,TCPARENT_TABPAGE);
									break;
								case TCTT_GROUP:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_GROUP,TCPARENT_TABPAGE);
									break;
								case TCTT_LISTBOX:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_LISTBOX,TCPARENT_TABPAGE);
									break;
								case TCTT_PROGRESS:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_PROGRESS,TCPARENT_TABPAGE);
									break;
								case TCTT_DATE:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_DATE,TCPARENT_TABPAGE);
									break;
								case TCTT_PICTURE:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_PICTURE,TCPARENT_TABPAGE);
									break;
								case TCTT_SLIDER:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_SLIDER,TCPARENT_TABPAGE);
									break;
								case TCTT_PATH:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_PATH,TCPARENT_TABPAGE);
									break;
								case TCTT_HOTKEY:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_HOTKEY,TCPARENT_TABPAGE);
									break;
								case TCTT_EXPLORER:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_EXPLORER,TCPARENT_TABPAGE);
									break;
								case TCTT_GRIDCTRL:
									((CTCTabPage*)pTab->m_Pages[j])->ClearCtrl(pTabSelectCtrl,TCTT_GRIDCTRL,TCPARENT_TABPAGE);
									break;
								default:
									break;
								}		
								//////////////////////��m_arrRectanglesAll���Ƴ�/////////////////////
								//delete pTabSelectCtrl;
								pTabSelectCtrl=NULL;
								tabCurrentID=_T("");
								ptabSelectRect=NULL;

							}
						}
					}
				}
			}
		}
	}			
	m_DiaDesign.Invalidate(true);

	//9-3�޸�
	///////////////////////////////////ɾ���Ժ���ʾ�����Ը���Ϊ�����������/////////////////////////////////////////////
	CString title;
	m_DiaDesign.GetWindowTextW(title);
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(title,&m_DiaDesign);
	///////////////////////////////////ɾ���Ժ���ʾ�����Ը���Ϊ�����������/////////////////////////////////////////////

	return;
}


void CTCDesignView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1||TabCtrlSelectMoreOne())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnEditCopy()
{
	// TODO: �ڴ���������������
	CopyCtrl();
}


void CTCDesignView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1||TabCtrlSelectMoreOne())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnEditPaste()
{
	// TODO: �ڴ���������������
	int select = m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.GetSize();
	if(select>0)
	{
		//�����ԭ��ѡ�е�DeSelect��
		m_DiaDesign.m_ControlTracker.DeSelectAll();
		CtrlPasteCommand* pCommand = new CtrlPasteCommand(TCPARENT_DESDIN,&m_DiaDesign);
		pCommand->SetTCCtrlNum(select);
		for(int i=0;i<m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.GetSize();i++)
		{
			TCTOOLTYPE curCtrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy[i]->GetCtlType();

			CWnd* pCtrl = m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy[i]->GetCWnd();

			CRect rc = m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy[i]->m_rect;
			
			

			//�����½��ƶ�10��λ���ȱ��⸲��
			rc.left = rc.left + 10;
			rc.right = rc.right + 10;
			rc.top = rc.top + 10;
			rc.bottom = rc.bottom + 10;

			pCommand->m_TCCtrlSort[i].SetTCCtrl(rc,curCtrlType);
			//m_DiaDesign.AddCtrl(rc,curCtrlType,TCPARENT_DESDIN,true);
		}
		m_DiaDesign.m_undoManager.DoCommand(pCommand);
	}
	else
	{
		int ctrlNum = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<ctrlNum;i++)
		{
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
			if(ctrlType == TCTT_TAB)
			{
				CTCTab* pTab = (CTCTab*)(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int tabSelect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy.GetSize();
					if( tabSelect > 0 )
					{			
						//��Ҫ���ƵĿؼ���Ϊδѡ��״̬
						((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelectAll();
						CtrlPasteCommand* pCommand = new CtrlPasteCommand(TCPARENT_TABPAGE,(CTCTabPage*)pTab->m_Pages[j]);
						pCommand->SetTCCtrlNum(tabSelect);
						for(int k=0;k<tabSelect;k++)
						{
							TCTOOLTYPE curTabCtrlType = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy[k]->GetCtlType();

							CWnd* pTabCtrl = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy[k]->GetCWnd();

							CRect rect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy[k]->m_rect;

							rect.left = rect.left + 10;
							rect.right = rect.right + 10;
							rect.top = rect.top + 10;
							rect.bottom = rect.bottom + 10;

							pCommand->m_TCCtrlSort[k].SetTCCtrl(rect,curTabCtrlType);

							//((CTCTabPage*)pTab->m_Pages[j])->AddCtrl(rect,curTabCtrlType,TCPARENT_TABPAGE,true);
						}
						m_DiaDesign.m_undoManager.DoCommand(pCommand);
					}
				}
			}
		}
	}
}    


void CTCDesignView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if( m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.GetSize()>=1||TabCtrlCopyMoreOne())
		 pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::CopyCtrl()
{
	int select = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if(select>0)
	{
		//m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.RemoveAll();
		RemoveCopyRetangles();
		for(int i=0;i<select;i++)
		{
			CString currentID = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[i]->GetCtlID();

			CControlRect* pCopyRect = m_DiaDesign.m_ControlTracker.GetRectByID(currentID);

			m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.Add(pCopyRect);
		}
	}
	else
	{
		////////////////////////////////���������Table�ؼ���ɾ�������Ļ�////////////////////////
		int ctrlNum = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i=0;i<ctrlNum;i++)
		{
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
			if(ctrlType == TCTT_TAB)
			{
				CTCTab* pTab = (CTCTab*)(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int tabSelect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
					if( tabSelect > 0 )
					{
						RemoveCopyRetangles();
						for(int k=0;k<tabSelect;k++)
						{
							CString tabCurrentID = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[k]->GetCtlID();

							CControlRect* ptabCopyRect =  ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.GetRectByID(tabCurrentID);

							((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy.Add(ptabCopyRect);
						}
					}
				}
			}
		}
	}			
	return;
}

void CTCDesignView::OnEditSelectAll()
{
	// TODO: �ڴ���������������
	int ctrlCount = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
	if(ctrlCount<1)
		return;

	for(int i=0;i<ctrlCount;i++)
	{
		m_DiaDesign.m_ControlTracker.Select(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]);		
	}
	//����Ĭ�ϻ�׼�ؼ�
	m_DiaDesign.m_ControlTracker.CreateDefaultBasicCtrl();
}


void CTCDesignView::OnUpdateEditSelectAll(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize()>=1);
}

///////////////////////�����л��Ƶ��������//////////////////////////
void CTCDesignView::RemoveCopyRetangles()
{
	int num = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
	if(0<num)
	{
		m_DiaDesign.m_ControlTracker.m_arrRectanglesCopy.RemoveAll();
		for(int i=0;i<num;i++)
		{
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
			if(ctrlType == TCTT_TAB)
			{
				CTCTab* pTab = (CTCTab*)(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesCopy.RemoveAll();
				}
			}		
		}
	}
}

BOOL CTCDesignView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SCROLLINFO scrollinfo;  
    GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
       
	CRect clientRect(0,0,0,0);
	m_DiaDesign.GetWindowRect(&clientRect);
	ScreenToClient(&clientRect);
	//CPoint point = GetScrollPosition();
	if(zDelta==120)
	{
		scrollinfo.nPos -= 1;  
		if(scrollinfo.nPos < scrollinfo.nMin)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			//break;  
			return TRUE;
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 
		//ScrollWindow(0,10);
		m_DiaDesign.MoveWindow(clientRect.left,clientRect.top + 10 ,clientRect.Width(),clientRect.Height());
	}
	if(zDelta==-120)
	{
		scrollinfo.nPos += 1;  
		if (scrollinfo.nPos>scrollinfo.nMax)  
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			//break;  
			return TRUE;
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		m_DiaDesign.MoveWindow(clientRect.left,clientRect.top - 10 ,clientRect.Width(),clientRect.Height());
		//ScrollWindow(0,-10); 

	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CTCDesignView::OnEditCtljumptofile()
{
	// TODO: �ڴ���������������
	if( !m_TWinID.IsEmpty() )
	{
		CString TFilePath;
		TFilePath.Format(_T("%s\\%s.t"),CTCRunTimeManager::g_ProjSourcePath,m_TWinID);
		((CTCApp*)AfxGetApp())->TCOpenTFile(TFilePath);
	}
	
}


void CTCDesignView::OnEditLock()
{
	// TODO: �ڴ���������������
	int select = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if(select > 0)
	{
		for(int i = 0; i < select; i++)
		{
			CString currentID = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[i]->GetCtlID();
			CControlRect* pCopyRect = m_DiaDesign.m_ControlTracker.GetRectByID(currentID);
			m_DiaDesign.m_ControlTracker.Lock(pCopyRect);
		}
	}
	else
	{
		////////////////////////////////���������Table�ؼ���ɾ�������Ļ�////////////////////////
		int ctrlNum = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i = 0; i < ctrlNum; i++)
		{
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
			if(ctrlType == TCTT_TAB)
			{
				CTCTab* pTab = (CTCTab*)(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int tabSelect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
					if( tabSelect > 0 )
					{
						for(int k = 0;k < tabSelect; k++)
						{
							CString tabCurrentID = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[k]->GetCtlID();
							CControlRect* ptabCopyRect =  ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.GetRectByID(tabCurrentID);
							((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.Lock(ptabCopyRect);
						}
					}
				}
			}
		}
	}
	m_DiaDesign.Invalidate();
}


void CTCDesignView::OnUpdateEditLock(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1||TabCtrlSelectMoreOne())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}


void CTCDesignView::OnEditUnlock()
{
	// TODO: �ڴ���������������
	int select = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize();
	if(select > 0)
	{
		for(int i = 0; i < select; i++)
		{
			CString currentID = m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected[i]->GetCtlID();
			CControlRect* pCopyRect = m_DiaDesign.m_ControlTracker.GetRectByID(currentID);
			m_DiaDesign.m_ControlTracker.UnLock(pCopyRect);
		}
	}
	else
	{
		////////////////////////////////���������Table�ؼ���ɾ�������Ļ�////////////////////////
		int ctrlNum = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i = 0; i < ctrlNum; i++)
		{
			TCTOOLTYPE ctrlType = m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
			if(ctrlType == TCTT_TAB)
			{
				CTCTab* pTab = (CTCTab*)(m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int tabSelect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
					if( tabSelect > 0 )
					{
						for(int k = 0;k < tabSelect; k++)
						{
							CString tabCurrentID = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected[k]->GetCtlID();
							CControlRect* ptabCopyRect =  ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.GetRectByID(tabCurrentID);
							((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.UnLock(ptabCopyRect);
						}
					}
				}
			}
		}
	}
	m_DiaDesign.Invalidate();
}


void CTCDesignView::OnUpdateEditUnlock(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_DiaDesign.m_ControlTracker.m_arrRectanglesSelected.GetSize()>=1||TabCtrlSelectMoreOne())
        pCmdUI->Enable(TRUE);
    else
        pCmdUI->Enable(FALSE);
}
