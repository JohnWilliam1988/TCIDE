// TCExplorer.cpp : ʵ���ļ�
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

// CTCExplorer ��Ϣ�������

BOOL CTCExplorer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_LBUTTONDBLCLK)
		return TRUE; 
	return CEdit::PreTranslateMessage(pMsg);
}


void CTCExplorer::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	 
	CRect rect;
	GetWindowRect(&rect);
	point.x+=rect.left;
	point.y+=rect.top;
	GetParent()->ScreenToClient(&point);
	//�򸸴��ڴ�����Ϣ
	::SendMessage(GetParent()->GetSafeHwnd(),WM_RBUTTONUP,nFlags,point.y*65536+point.x);	
	CEdit::OnRButtonUp(nFlags, point);
}