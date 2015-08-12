// TCTimer.cpp : ʵ���ļ�
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

// CTCTimer ��Ϣ�������

void CTCTimer::SetTimer(UINT time)
{
	//��������Ƶ�ʱ��Ҫ����
	//m_nID = ::SetTimer(this->m_hWnd,1,time,NULL);
	m_Time = time;
}

void CTCTimer::StopTimer()
{
	::KillTimer(this->m_hWnd,m_nID);
}

void CTCTimer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CStatic::OnTimer(nIDEvent);
}

