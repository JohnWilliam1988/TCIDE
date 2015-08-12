// TCDate.cpp : ʵ���ļ�
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



// CTCDate ��Ϣ�������

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
