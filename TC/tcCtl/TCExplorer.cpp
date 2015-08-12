// TCExplorer.cpp : 实现文件
//

#include "stdafx.h"
#include "TCExplorer.h"


// CTCExplorer

IMPLEMENT_DYNAMIC(CTCExplorer, CEdit)

CTCExplorer::CTCExplorer()
{
	m_url = _T("");
	m_loadComplete = _T("");
}

CTCExplorer::~CTCExplorer()
{
}


BEGIN_MESSAGE_MAP(CTCExplorer, CEdit)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

void CTCExplorer::TCSetURL(CString str)
{
	m_url = str;
}

void CTCExplorer::TCSetloadComplete(CString str)
{
	m_loadComplete = str;
}

// CTCExplorer 消息处理程序

BOOL CTCExplorer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_LBUTTONDBLCLK)
		return TRUE; 
	return CEdit::PreTranslateMessage(pMsg);
}


void CTCExplorer::OnRButtonUp(UINT nFlags, CPoint point)
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