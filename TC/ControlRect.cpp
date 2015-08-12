#include "stdafx.h"

#include "ControlRect.h"
#include "TC.h"
#include "MainFrm.h"
#include "TCCtlHeads.h"
#include "DiaTCDesign.h"
#include "TCTabPage.h"

CControlRect::CControlRect( int x1, int y1, int x2, int y2,CWnd *cwnd,TCTOOLTYPE type,CString id) : 
				CRectTracker( & CRect( x1, y1, x2, y2 ), CRectTracker::resizeOutside /*| CRectTracker::dottedLine*/  )
{
    m_ctlWnd=cwnd;
    m_ToolType=type;
    m_id=id;
	Initialize();
}

CControlRect::CControlRect( const CRect & rect,CWnd *cwnd,TCTOOLTYPE type,CString id,TCPARENTTYPE parentwndType,CWnd *parent)
{
    m_ctlWnd=cwnd;
    m_ToolType=type;
    m_id=id;
    m_rect.CopyRect(&rect );
	m_parentWndType = parentwndType;
	m_ctrlParent = parent;
	Initialize();	
}

CControlRect::~CControlRect()
{
    if(m_ctlWnd)
    {
        delete m_ctlWnd;
        m_ctlWnd=NULL;
    }
}

UINT CControlRect::GetHandleMask() const
{
	return ( 255 );
}

void CControlRect::Initialize()
{
	m_bSelected = FALSE;
	m_highLight = FALSE;
	m_bLockedUp = FALSE;
	m_nHandleSize = 5;
	m_nStyle = CRectTracker::resizeOutside /*| CRectTracker::dottedLine*/;
    m_sizeMin.SetSize(20,20);
}

void CControlRect::TCOnSizeCtrlCommand()
{
	if(m_parentWndType == TCPARENT_DESDIN)
	{
		switch(m_ToolType)
		{
		case TCTT_STATIC:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_STATIC,TCPARENT_DESDIN);	
			break;
		case TCTT_BUTTON:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_BUTTON,TCPARENT_DESDIN);	
			break;
		case TCTT_EDIT:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_EDIT,TCPARENT_DESDIN);	
			break;
		case TCTT_RADIO:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_RADIO,TCPARENT_DESDIN);
			break;
		case TCTT_CHECK:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_CHECK,TCPARENT_DESDIN);
			break;
		case TCTT_COMBOBOX:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_COMBOBOX,TCPARENT_DESDIN);
			break;
		case TCTT_GROUP:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_GROUP,TCPARENT_DESDIN);
			break;
		case TCTT_LISTBOX:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_LISTBOX,TCPARENT_DESDIN);
			break;
		case TCTT_PROGRESS:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_PROGRESS,TCPARENT_DESDIN);
			break;
		case TCTT_DATE:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_DATE,TCPARENT_DESDIN);
			break;
		case TCTT_PICTURE:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_PICTURE,TCPARENT_DESDIN);
			break;
		case TCTT_SLIDER:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_SLIDER,TCPARENT_DESDIN);
			break;
		case TCTT_PATH:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_PATH,TCPARENT_DESDIN);
			break;
		case TCTT_TIMER:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_TIMER,TCPARENT_DESDIN);
			break;
		case TCTT_HOTKEY:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_HOTKEY,TCPARENT_DESDIN);
			break;
		case TCTT_TAB:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_TAB,TCPARENT_DESDIN);
			break;
		case TCTT_EXPLORER:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_EXPLORER,TCPARENT_DESDIN);
			break;
		case TCTT_GRIDCTRL:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_GRIDCTRL,TCPARENT_DESDIN);
			break;
		default:
			m_ctlWnd->MoveWindow(m_rect);
			break;
		}
	}
	else if(m_parentWndType == TCPARENT_TABPAGE)
	{
		switch(m_ToolType)
		{
		case TCTT_STATIC:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_STATIC,TCPARENT_TABPAGE);
			break;
		case TCTT_BUTTON:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_BUTTON,TCPARENT_TABPAGE);
			break;
		case TCTT_EDIT:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_EDIT,TCPARENT_TABPAGE);
			break;
		case TCTT_RADIO:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_RADIO,TCPARENT_TABPAGE);
			break;
		case TCTT_CHECK:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_CHECK,TCPARENT_TABPAGE);
			break;
		case TCTT_COMBOBOX:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_COMBOBOX,TCPARENT_TABPAGE);
			break;
		case TCTT_GROUP:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_GROUP,TCPARENT_TABPAGE);
			break;
		case TCTT_LISTBOX:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_LISTBOX,TCPARENT_TABPAGE);
			break;
		case TCTT_PROGRESS:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_PROGRESS,TCPARENT_TABPAGE);
			break;
		case TCTT_DATE:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_DATE,TCPARENT_TABPAGE);
			break;
		case TCTT_PICTURE:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_PICTURE,TCPARENT_TABPAGE);
			break;
		case TCTT_SLIDER:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_SLIDER,TCPARENT_TABPAGE);
			break;
		case TCTT_PATH:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_PATH,TCPARENT_TABPAGE);
			break;
		case TCTT_HOTKEY:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_HOTKEY,TCPARENT_TABPAGE);
			break;
		case TCTT_EXPLORER:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_EXPLORER,TCPARENT_DESDIN);
			break;
		case TCTT_GRIDCTRL:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,m_rect,TCTT_GRIDCTRL,TCPARENT_DESDIN);
			break;
		default:
			m_ctlWnd->MoveWindow(m_rect);
			break;
		}
	}
}

void CControlRect::SetRect( int x1, int y1, int x2, int y2 , bool isupdate)
{ 
	m_rect.SetRect( x1, y1, x2, y2 );
	m_ctlWnd->MoveWindow(m_rect);
	//TCOnSizeCtrlCommand();
	//加一个isupdate参数是为了橡皮筋选中多个控件后,会造成多个控件同时刷新混乱的问题,默认为刷新
	if (isupdate)
		TCUpdateProperty();
}

void CControlRect::SetRect( const CRect & rect , bool isupdate)
{
	m_rect.SetRect( rect.left , rect.top , rect.right , rect.bottom ); 
	m_ctlWnd->MoveWindow(m_rect);
	//m_ctlWnd->MoveWindow(&m_rect);
	//TCOnSizeCtrlCommand();
	//加一个isupdate参数是为了橡皮筋选中多个控件后,会造成多个控件同时刷新混乱的问题,默认为刷新
	if (isupdate)
		TCUpdateProperty();
}

////////////////////////////SetRect的时候和属性框同步//////////////////////////////
void CControlRect::TCUpdateProperty()
{
	switch(m_ToolType)
	{
	case TCTT_STATIC:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(m_id,(CTCStatic *)m_ctlWnd,TRUE);
		break;
	case TCTT_BUTTON:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(m_id,(CTCButton *)m_ctlWnd,TRUE);
		break;
	case TCTT_EDIT:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(m_id,(CTCEdit *)m_ctlWnd,TRUE);
		break; 
	case TCTT_RADIO:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(m_id,(CTCRadio *)m_ctlWnd,TRUE);
		break;
	case TCTT_CHECK:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(m_id,(CTCCheck *)m_ctlWnd,TRUE);
		break;
	case TCTT_COMBOBOX:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(m_id,(CTCComboBox *)m_ctlWnd,TRUE);
		break;
	case TCTT_GROUP:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(m_id,(CTCGroupBox *)m_ctlWnd,TRUE);
		break;
	case TCTT_LISTBOX:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(m_id,(CTCListBox *)m_ctlWnd,TRUE);
		break;
	case TCTT_PROGRESS:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(m_id,(CTCProgress *)m_ctlWnd,TRUE);
		break;
	case TCTT_DATE:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(m_id,(CTCDate *)m_ctlWnd,TRUE);
		break;
	case TCTT_PICTURE:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(m_id,(CTCPicture *)m_ctlWnd,TRUE);
		break;
	case TCTT_SLIDER:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(m_id,(CTCSlider *)m_ctlWnd,TRUE);
		break;
	case TCTT_PATH:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(m_id,(CTCBrowse *)m_ctlWnd,TRUE);
		break;
	case TCTT_TIMER:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(m_id,(CTCTimer *)m_ctlWnd,TRUE);
		break;
	case TCTT_HOTKEY:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(m_id,(CTCHotKeyCtrl *)m_ctlWnd,TRUE);
		break;
	case TCTT_TAB:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(m_id,(CTCTab *)m_ctlWnd,TRUE);
		break;
	case TCTT_EXPLORER:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(m_id,(CTCExplorer *)m_ctlWnd,TRUE);
		break;
	case TCTT_GRIDCTRL:
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(m_id,(CGridCtrl *)m_ctlWnd,TRUE);
		break;
	default:
		break;
	}
}
////////////////////////////SetRect的时候和属性框同步//////////////////////////////

void CControlRect::Track( CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo )
{
	CRectTracker::Track( pWnd, point, bAllowInvert, pWndClipTo );
	//m_ctlWnd->MoveWindow(&m_rect);
	TCOnSizeCtrlCommand();
	//加一个isupdate参数是为了橡皮筋选中多个控件后,会造成多个控件同时刷新混乱的问题,默认为刷新
	TCUpdateProperty();
}


void   CControlRect::MyDraw(CDC* pDC,CPen* pPen,BOOL locked) 
{
	if((m_nStyle /*& dottedLine*/)   !=   0)
	{
		VERIFY(pDC-> SaveDC()   !=   0);
		pDC-> SetMapMode(MM_TEXT);
		pDC-> SetViewportOrg(0,   0);
		pDC-> SetWindowOrg(0,   0);
		//get normalized rectangle
		CRect   rect   =   m_rect;
		rect.NormalizeRect();
		rect.InflateRect(1,1);
		CPen*   pOldPen   =   NULL;
		CBrush*   pOldBrush   =   NULL;
		//CGdiObject*  pTemp;
		int   nOldROP;
		pOldPen   =   (CPen*)pDC-> SelectObject(pPen);
		pOldBrush  =  (CBrush*)pDC-> SelectStockObject(NULL_BRUSH);
		nOldROP   =   pDC-> SetROP2(R2_COPYPEN);
		rect.InflateRect(+1,   +1);       //   borders   are   one   pixel   outside
		pDC-> Rectangle(rect.left,   rect.top,   rect.right,   rect.bottom);
		pDC-> SetROP2(nOldROP);          
		//m_nStyle   ^=   dottedLine;
		if(!locked)
		{
			CRectTracker::Draw(pDC);  
		//m_nStyle   |=   dottedLine;
		}
		else
		{
			UINT mask = GetHandleMask();
			for (int i = 0; i < 8; ++i)
			{
				if (mask & (1<<i))
				{
					CRect rect(0,0,0,0);
					GetHandleRect((TrackerHit)i, &rect);
					//改变当前调整手柄矩形颜色,也就是那八个点
					pDC->FillSolidRect(rect, RGB(192,192,192));
					//CRectTracker::Draw(pDC); 
				}
			}
		}
	}
	else
		CRectTracker::Draw(pDC);   
}

void CControlRect::DrawLocked(CDC* pDC)
{
	if((m_nStyle /*& dottedLine*/)   !=   0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				CRect rect(0,0,0,0);
				GetHandleRect((TrackerHit)i, &rect);
				//改变当前调整手柄矩形颜色,也就是那八个点
				pDC->FillSolidRect(rect, RGB(192,192,192));
				//CRectTracker::Draw(pDC); 
			}
		}
	}  
}

CWnd *CControlRect::GetCWnd() const
{
    return m_ctlWnd;
}

CString CControlRect::GetCtlID()
{
    return m_id;
}
void CControlRect::SetCtlID(CString id)
{
    id.Trim();
    m_id=id;
}

void CControlRect::TCMoveWindos( const CRect & rect )
{
	//m_ctlWnd->MoveWindow(&rect);
    //this->SetRect(rect);
}

void CControlRect::TCMoveWindos(int x, int y, int nWidth, int nHeight)
{
	CRect rect(x,y,nWidth,nHeight);
	if(m_parentWndType == TCPARENT_DESDIN)
	{
		switch(m_ToolType)
		{
		case TCTT_STATIC:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_STATIC,TCPARENT_DESDIN);	
			break;
		case TCTT_BUTTON:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_BUTTON,TCPARENT_DESDIN);	
			break;
		case TCTT_EDIT:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_EDIT,TCPARENT_DESDIN);	
			break;
		case TCTT_RADIO:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_RADIO,TCPARENT_DESDIN);
			break;
		case TCTT_CHECK:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_CHECK,TCPARENT_DESDIN);
			break;
		case TCTT_COMBOBOX:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_COMBOBOX,TCPARENT_DESDIN);
			break;
		case TCTT_GROUP:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_GROUP,TCPARENT_DESDIN);
			break;
		case TCTT_LISTBOX:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_LISTBOX,TCPARENT_DESDIN);
			break;
		case TCTT_PROGRESS:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_PROGRESS,TCPARENT_DESDIN);
			break;
		case TCTT_DATE:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_DATE,TCPARENT_DESDIN);
			break;
		case TCTT_PICTURE:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_PICTURE,TCPARENT_DESDIN);
			break;
		case TCTT_SLIDER:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_SLIDER,TCPARENT_DESDIN);
			break;
		case TCTT_PATH:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_PATH,TCPARENT_DESDIN);
			break;
		case TCTT_TIMER:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_TIMER,TCPARENT_DESDIN);
			break;
		case TCTT_HOTKEY:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_HOTKEY,TCPARENT_DESDIN);
			break;
		case TCTT_TAB:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_TAB,TCPARENT_DESDIN);
			break;
		case TCTT_EXPLORER:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_EXPLORER,TCPARENT_DESDIN);
			break;
		case TCTT_GRIDCTRL:
			((CDiaTCDesign*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_GRIDCTRL,TCPARENT_DESDIN);
			break;
		default:
			m_ctlWnd->MoveWindow(rect);
			break;
		}
	}
	else if(m_parentWndType == TCPARENT_TABPAGE)
	{
		switch(m_ToolType)
		{
		case TCTT_STATIC:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_STATIC,TCPARENT_TABPAGE);
			break;
		case TCTT_BUTTON:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_BUTTON,TCPARENT_TABPAGE);
			break;
		case TCTT_EDIT:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_EDIT,TCPARENT_TABPAGE);
			break;
		case TCTT_RADIO:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_RADIO,TCPARENT_TABPAGE);
			break;
		case TCTT_CHECK:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_CHECK,TCPARENT_TABPAGE);
			break;
		case TCTT_COMBOBOX:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_COMBOBOX,TCPARENT_TABPAGE);
			break;
		case TCTT_GROUP:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_GROUP,TCPARENT_TABPAGE);
			break;
		case TCTT_LISTBOX:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_LISTBOX,TCPARENT_TABPAGE);
			break;
		case TCTT_PROGRESS:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_PROGRESS,TCPARENT_TABPAGE);
			break;
		case TCTT_DATE:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_DATE,TCPARENT_TABPAGE);
			break;
		case TCTT_PICTURE:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_PICTURE,TCPARENT_TABPAGE);
			break;
		case TCTT_SLIDER:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_SLIDER,TCPARENT_TABPAGE);
			break;
		case TCTT_PATH:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_PATH,TCPARENT_TABPAGE);
			break;
		case TCTT_HOTKEY:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_HOTKEY,TCPARENT_TABPAGE);
			break;
		case TCTT_EXPLORER:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_EXPLORER,TCPARENT_TABPAGE);
			break;
		case TCTT_GRIDCTRL:
			((CTCTabPage*)m_ctrlParent)->OnSizeCtrl(m_ctlWnd,rect,TCTT_GRIDCTRL,TCPARENT_TABPAGE);
			break;
		default:
			m_ctlWnd->MoveWindow(rect);
			break;
		}
	}
    //m_ctlWnd->MoveWindow(x,y,nWidth,nHeight);
   // this->SetRect(x,y,nWidth,nHeight);
}

TCTOOLTYPE CControlRect::GetCtlType()
{
    return m_ToolType;
}

TCPARENTTYPE CControlRect::GetParentType()
{
	return m_parentWndType;
}

bool CControlRect::TCGetHighSelected()
{
	return m_highLight;
}
