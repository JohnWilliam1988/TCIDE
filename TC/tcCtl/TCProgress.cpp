// TCProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "../TC.h"
#include "TCProgress.h"


// CTCProgress

IMPLEMENT_DYNAMIC(CTCProgress, CProgressCtrl)

CTCProgress::CTCProgress()
{
	m_isVertical = false;
	m_isAbled = true;
}

CTCProgress::~CTCProgress()
{
}


BEGIN_MESSAGE_MAP(CTCProgress, CProgressCtrl)
END_MESSAGE_MAP()



// CTCProgress 消息处理程序

void CTCProgress::TCSetVertical(bool set)
{
	if(set)
	{
		ModifyStyle(NULL,PBS_VERTICAL,NULL);
		SetPos(70);
		m_isVertical = true;
	}
	else
	{
		ModifyStyle(PBS_VERTICAL,NULL,NULL);
		SetPos(70);
		m_isVertical = false;
	}
}

void CTCProgress::TCSetAbled(bool set)
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
