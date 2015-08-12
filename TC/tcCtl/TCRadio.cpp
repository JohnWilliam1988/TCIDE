// TCRadio.cpp : 实现文件
//

#include "stdafx.h"
#include "../TC.h"
#include "TCRadio.h"


// CTCRadio

IMPLEMENT_DYNAMIC(CTCRadio, CButton)

CTCRadio::CTCRadio()
{
	m_eClick = _T("");
    m_isCheck = false;
	m_isLeftText = false;
	m_isAbled = true;
	m_isGroup = false;
}

CTCRadio::~CTCRadio()
{
}


BEGIN_MESSAGE_MAP(CTCRadio, CButton)
END_MESSAGE_MAP()



// CTCRadio 消息处理程序



BOOL CTCRadio::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if( pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDBLCLK)
		return true;
	return CButton::PreTranslateMessage(pMsg);
}


void CTCRadio::TCSetTextLeft(bool left)
{
	if(left)
	{
		ModifyStyle(NULL,BS_LEFTTEXT,NULL);
		m_isLeftText = true;
	}
	else
	{
		ModifyStyle(BS_LEFTTEXT,NULL,NULL);
		m_isLeftText = false;
	}
	Invalidate(TRUE);
}

void CTCRadio::TCSetAbled(bool set)
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

void CTCRadio::TCSetGroup(bool set)
{
	if(set)
	{
		ModifyStyle(NULL,WS_GROUP,NULL);
		m_isGroup = true;
	}
	else
	{
		ModifyStyle(WS_GROUP,NULL,NULL);
		m_isGroup = false;
	}
	Invalidate(TRUE);
}
