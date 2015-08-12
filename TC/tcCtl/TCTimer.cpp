// TCTimer.cpp : 实现文件
//

#include "stdafx.h"
#include "../TC.h"
#include "TCTimer.h"

// CTCTimer

IMPLEMENT_DYNAMIC(CTCTimer, CStatic)

CTCTimer::CTCTimer()
{
	m_TimeEvent = _T("");
	m_Time = 0;
	img.LoadFromResource(AfxGetInstanceHandle(),IDB_BITMAP_TIMER);
	if(!img.IsNull())
	{
		m_bmp = img.Detach();
	}
}

CTCTimer::~CTCTimer()
{
}

BEGIN_MESSAGE_MAP(CTCTimer, CStatic)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTCTimer 消息处理程序

void CTCTimer::SetTimer(UINT time)
{
	//这里在设计的时候不要触发
	//m_nID = ::SetTimer(this->m_hWnd,1,time,NULL);
	m_Time = time;
}

void CTCTimer::StopTimer()
{
	::KillTimer(this->m_hWnd,m_nID);
}

void CTCTimer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CStatic::OnTimer(nIDEvent);
}

