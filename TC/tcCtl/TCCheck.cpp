// TCCheck.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../TC.h"
#include "TCCheck.h"


// CTCCheck

IMPLEMENT_DYNAMIC(CTCCheck, CButton)

CTCCheck::CTCCheck()
{
	m_eClick = _T("");
    m_isCheck= false;
	m_isLeftText = false;
	m_isAbled = true;
}

CTCCheck::~CTCCheck()
{
}


BEGIN_MESSAGE_MAP(CTCCheck, CButton)
END_MESSAGE_MAP()


// CTCCheck ��Ϣ�������
BOOL CTCCheck::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_LBUTTONDBLCLK)
		return true;
	return CButton::PreTranslateMessage(pMsg);
}


void CTCCheck::TCSetTextLeft(bool left)
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

void CTCCheck::TCSetAbled(bool set)
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