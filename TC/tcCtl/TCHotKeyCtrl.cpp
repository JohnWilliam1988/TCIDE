// TCHotKeyCtrl.cpp : 实现文件

#include "stdafx.h"
#include "TCHotKeyCtrl.h"

// CTCHotKeyCtrl

IMPLEMENT_DYNAMIC(CTCHotKeyCtrl, CHotKeyCtrl)

CTCHotKeyCtrl::CTCHotKeyCtrl()
{
	m_event = _T("");
	m_killFocusEvent=_T("");
	m_wModifiers = 0;
	m_wVirtualKeyCode = 0;
	m_isAbled = true;
}

CTCHotKeyCtrl::~CTCHotKeyCtrl()
{
}

BEGIN_MESSAGE_MAP(CTCHotKeyCtrl, CHotKeyCtrl)
	ON_WM_HOTKEY()
END_MESSAGE_MAP()

// CTCHotKeyCtrl 消息处理程序

//注册热键
int CTCHotKeyCtrl::RegisterHotKey()
{
	WORD vk, mod;
	GetHotKey(vk, mod);
	return ::RegisterHotKey(this->GetSafeHwnd(), 1, mod, vk);
}

//设置热键名称，属性框用
void CTCHotKeyCtrl::SetHotKeyName(CString name)
{
	m_HotKeyName = name;
}

//设置热键的键值
void CTCHotKeyCtrl::SetHotKeyValue(WORD wVirtual,WORD wMod)
{
	m_wVirtualKeyCode = wVirtual;
	m_wModifiers = wMod;
}

//获取热键的键值
void CTCHotKeyCtrl::GetHotKeyValue(WORD &wVirtualKey,WORD &wMod)
{
	wVirtualKey = m_wVirtualKeyCode;
	wMod = m_wModifiers;
}

//卸载热键
int CTCHotKeyCtrl::UnRegisterHotKey()
{
	return ::UnregisterHotKey(this->GetSafeHwnd(),1);
}

//热键启动
void CTCHotKeyCtrl::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	AfxMessageBox(_T("热键启动成功!"));
	CHotKeyCtrl::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CTCHotKeyCtrl::TCSetAbled(bool set)
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