// TCComboBoxS.cpp : 实现文件
//

#include "stdafx.h"
#include "TCComboBox.h"


// CTCComboBoxS

IMPLEMENT_DYNAMIC(CTCComboBox, CComboBox)

CTCComboBox::CTCComboBox()
{
	m_isDropDown = false;
	m_isAbled = true;
}

CTCComboBox::~CTCComboBox()
{
}


BEGIN_MESSAGE_MAP(CTCComboBox, CComboBox)
END_MESSAGE_MAP()

BOOL CTCComboBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_LBUTTONDBLCLK)
		 return TRUE;
	if(pMsg->message == WM_RBUTTONUP)
	{
		if(pMsg->hwnd == GetWindow(GW_CHILD)->m_hWnd)
		{		
			GetParent()->ScreenToClient(&pMsg->pt);
			//向父窗口传递消息弹出菜单
			::SendMessage(GetParent()->GetSafeHwnd(),WM_RBUTTONUP,0,pMsg->pt.y*65536+pMsg->pt.x);

		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

void CTCComboBox::TCSetDropDown(bool set)
{
	if(set)
	{
		ModifyStyle(CBS_DROPDOWN,NULL,NULL);
		m_isDropDown = true;
	}
	else
	{
		ModifyStyle(CBS_DROPDOWNLIST,CBS_DROPDOWN,NULL);		
		m_isDropDown = false;
	}
	Invalidate(TRUE);
}

void CTCComboBox::TCSetAbled(bool set)
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

