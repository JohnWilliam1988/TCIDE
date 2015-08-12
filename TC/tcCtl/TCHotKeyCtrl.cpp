// TCHotKeyCtrl.cpp : ʵ���ļ�

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

// CTCHotKeyCtrl ��Ϣ�������

//ע���ȼ�
int CTCHotKeyCtrl::RegisterHotKey()
{
	WORD vk, mod;
	GetHotKey(vk, mod);
	return ::RegisterHotKey(this->GetSafeHwnd(), 1, mod, vk);
}

//�����ȼ����ƣ����Կ���
void CTCHotKeyCtrl::SetHotKeyName(CString name)
{
	m_HotKeyName = name;
}

//�����ȼ��ļ�ֵ
void CTCHotKeyCtrl::SetHotKeyValue(WORD wVirtual,WORD wMod)
{
	m_wVirtualKeyCode = wVirtual;
	m_wModifiers = wMod;
}

//��ȡ�ȼ��ļ�ֵ
void CTCHotKeyCtrl::GetHotKeyValue(WORD &wVirtualKey,WORD &wMod)
{
	wVirtualKey = m_wVirtualKeyCode;
	wMod = m_wModifiers;
}

//ж���ȼ�
int CTCHotKeyCtrl::UnRegisterHotKey()
{
	return ::UnregisterHotKey(this->GetSafeHwnd(),1);
}

//�ȼ�����
void CTCHotKeyCtrl::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	AfxMessageBox(_T("�ȼ������ɹ�!"));
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