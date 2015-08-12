/*===========================================================================
====                                                                     ====
====    File name           :  EditTrans.cpp                             ====
====    Creation date       :  7/10/2001                                 ====
====    Author(s)           :  Dany Cantin                               ====
====                                                                     ====
===========================================================================*/

#include "stdafx.h"
#include "TCEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTCEdit

CTCEdit::CTCEdit()
{
    m_TextColor = RGB(0, 0, 0);
    m_BackColor = RGB(255,255,255);//TRANS_BACK;
    m_isPassword = false;
	m_isReadOnly = false;
	m_isAbled = true;
	m_isMulityLine = false;
	m_isNumber = false;
	m_Verti = false;
	m_Horiz = false;
	m_eEnChange.Empty();
	m_eSetFocus.Empty();
	m_eKillfocus.Empty();
}

CTCEdit::~CTCEdit()
{
}


BEGIN_MESSAGE_MAP(CTCEdit, CEdit)
    //{{AFX_MSG_MAP(CTCEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
    ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
    ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
    ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCEdit message handlers

HBRUSH CTCEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    m_Brush.DeleteObject();

    if (m_BackColor == TRANS_BACK) {
        m_Brush.CreateStockObject(HOLLOW_BRUSH);
        pDC->SetBkMode(TRANSPARENT);
    }
    else {
        m_Brush.CreateSolidBrush(m_BackColor);
        pDC->SetBkColor(m_BackColor);
    }

    pDC->SetTextColor(m_TextColor);

    return (HBRUSH)m_Brush;
}

void CTCEdit::OnKillfocus() 
{
    UpdateCtrl();
}

void CTCEdit::OnUpdate() 
{
    UpdateCtrl();
}

void CTCEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
    UpdateCtrl();

    CEdit::OnLButtonDown(nFlags, point);
}

void CTCEdit::OnEnChange()
{

}

void CTCEdit::UpdateCtrl()
{
    CWnd* pParent = GetParent();
    CRect rect;

    GetWindowRect(rect);
    pParent->ScreenToClient(rect);
    rect.DeflateRect(2, 2);

    pParent->InvalidateRect(rect, FALSE);    
}

CString CTCEdit::TCGetEnChangeEvent()
{
    return m_eEnChange;
}
CString CTCEdit::TCGetSetFocusEvent()
{
	return m_eSetFocus;
}
CString CTCEdit::TCGetKillfocusEvent()
{
    return m_eKillfocus;
}

void CTCEdit::TCSetEnChangeEvent(CString csEvent)
{
    m_eEnChange = csEvent;
}
void CTCEdit::TCSetFocusEvent(CString csEvent)
{
	m_eSetFocus = csEvent;
}
void CTCEdit::TCSetKillfocusEvent(CString csEvent)
{
    m_eKillfocus = csEvent;
}
BOOL CTCEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_LBUTTONDBLCLK)
		 return TRUE; 
	return CEdit::PreTranslateMessage(pMsg);
}

void CTCEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	 
	CRect rect;
	GetWindowRect(&rect);
	point.x+=rect.left;
	point.y+=rect.top;
	GetParent()->ScreenToClient(&point);
	//向父窗口传递消息
	::SendMessage(GetParent()->GetSafeHwnd(),WM_RBUTTONUP,nFlags,point.y*65536+point.x);	
	CEdit::OnRButtonUp(nFlags, point);
}

void CTCEdit::TCSetAbled(bool set)
{
	if(set)
	{
		ModifyStyle(WS_DISABLED,NULL,NULL);
		m_isAbled = true;
	}
	else
	{
		ModifyStyle(NULL,WS_DISABLED,NULL);
		m_isAbled = false;
	}
	Invalidate(TRUE);
}

void CTCEdit::TCSetMulityLine(bool set)
{
	m_isMulityLine = set;
	//如果没有多行属性那么就没有滚动条出现
	if(m_isMulityLine)
	{	
		if(m_Verti)
			ShowScrollBar(SB_VERT,TRUE);
		else
			ShowScrollBar(SB_VERT,FALSE);

		if(m_Horiz)
			ShowScrollBar(SB_HORZ,TRUE);
		else
			ShowScrollBar(SB_HORZ,FALSE);
	}	
	else
	{
		ShowScrollBar(SB_VERT,FALSE);
		ShowScrollBar(SB_HORZ,FALSE);
	}
}

void CTCEdit::TCSetReadOnly(bool set)
{
	if(set)
		m_isReadOnly = true;
	else
		m_isReadOnly = false;
	SetReadOnly(set);
}

void CTCEdit::TCSetPassWord(bool password)
{
	if(password)
	{
		this->SetPasswordChar(L'*');
		m_isPassword = TRUE;
	}
	else
	{
		this->SetPasswordChar(0);
		m_isPassword = FALSE;
	}
	Invalidate(TRUE);
}

void CTCEdit::TCSetNumber(bool set)
{
	if(set)
	{
		ModifyStyle(NULL,ES_NUMBER,NULL);
		m_isNumber = true;
	}
	else
	{	
		ModifyStyle(ES_NUMBER,NULL,NULL);
		m_isNumber = false;
	}
	Invalidate(TRUE);
}

void CTCEdit::TCSetVertiScroll(bool set)
{
	m_Verti = set;
	if(m_isMulityLine && m_Verti)
		ShowScrollBar(SB_VERT,TRUE);
	else
		ShowScrollBar(SB_VERT,FALSE);
}

void CTCEdit::TCSetHorizScroll(bool set)
{
	m_Horiz = set;
	if(m_isMulityLine && m_Horiz)
		ShowScrollBar(SB_HORZ,TRUE);
	else
		ShowScrollBar(SB_HORZ,FALSE);
}
