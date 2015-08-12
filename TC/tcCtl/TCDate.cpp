// TCDate.cpp : 实现文件
//

#include "stdafx.h"
#include "../TC.h"
#include "TCDate.h"


// CTCDate

IMPLEMENT_DYNAMIC(CTCDate, CDateTimeCtrl)

CTCDate::CTCDate()
{
	m_isAbled = true;
}

CTCDate::~CTCDate()
{
}


BEGIN_MESSAGE_MAP(CTCDate, CDateTimeCtrl)
END_MESSAGE_MAP()



// CTCDate 消息处理程序

void CTCDate::TCSetAbled(bool set)
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
