// TCButton.cpp : 实现文件
//

#include "stdafx.h"
#include "../TC.h"
#include "TCButton.h"
#include "../MainFrm.h"
#include "../TCRunTimeManager.h"


// CTCButton

IMPLEMENT_DYNAMIC(CTCButton, CButton)

CTCButton::CTCButton()
{
	m_eClick = _T("");
	m_id = _T("");
	m_isMulityLine = false;
	m_defButton = false;
	m_isAbled = true;
}

CTCButton::~CTCButton()
{
}


BEGIN_MESSAGE_MAP(CTCButton, CButton)
	ON_CONTROL_REFLECT(BN_DOUBLECLICKED, &CTCButton::OnBnDoubleclicked)
	ON_CONTROL_REFLECT(BN_CLICKED, &CTCButton::OnBnClicked)
END_MESSAGE_MAP()



// CTCButton 消息处理程序


void CTCButton::OnBnDoubleclicked()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

	if(CTCRunTimeManager::g_LangType==0)
	{
		m_eClick.Format(_T("%s.%s_点击"),pFrame->m_pCurrentWndID/*CTCRunTimeManager::g_ProjName*/,m_id);
	}
	else if(CTCRunTimeManager::g_LangType==1)
	{
		m_eClick.Format(_T("%s.%s_click"),pFrame->m_pCurrentWndID/*CTCRunTimeManager::g_ProjName*/,m_id);
	}

	pFrame->TCAddEventCode(m_eClick);
}

void CTCButton::TCSetButtonMulity(bool set)
{
	if(set)
	{
		ModifyStyle(NULL,BS_MULTILINE,NULL);
		m_isMulityLine = true;
	}
	else
	{
		ModifyStyle(BS_MULTILINE,NULL,NULL);
		m_isMulityLine = false;
	}
	Invalidate(TRUE);
}

void CTCButton::TCSetAbled(bool set)
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


void CTCButton::OnBnClicked()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTCButton::TCSetDefButton(bool set)
{
	m_defButton = set;
}
