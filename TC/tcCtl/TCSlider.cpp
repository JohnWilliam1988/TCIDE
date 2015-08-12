// TCSlider.cpp : 实现文件
//

#include "stdafx.h"
#include "../TC.h"
#include "TCSlider.h"


// CTCSlider

IMPLEMENT_DYNAMIC(CTCSlider, CSliderCtrl)

CTCSlider::CTCSlider()
{
	m_isVertical = false;
	m_isAbled = true;
}

CTCSlider::~CTCSlider()
{
}


BEGIN_MESSAGE_MAP(CTCSlider, CSliderCtrl)
END_MESSAGE_MAP()



// CTCSlider 消息处理程序

void CTCSlider::TCSetVertical(bool set)
{
	if(set)
	{
		ModifyStyle(NULL,TBS_VERT,NULL);
			m_isVertical = true;
	}
	else
	{
		ModifyStyle(TBS_VERT,NULL,NULL);
			m_isVertical = false;
	}
}

void CTCSlider::TCSetAbled(bool set)
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


