#pragma once

// CTCTimer
class CTCTimer : public CStatic
{
	DECLARE_DYNAMIC(CTCTimer)

public:
	CTCTimer();
	virtual ~CTCTimer();

protected:
	DECLARE_MESSAGE_MAP()
public:

	CString m_TimeEvent;

	CImage img;	
	HBITMAP m_bmp;

	void TCSetTimeEvent(CString str) {m_TimeEvent = str; }

	CString TCGetTimeEvent()         {return m_TimeEvent;}

	UINT m_Time;

	int m_nID;

	void SetTimer(UINT time);

	void StopTimer();

	UINT GetTimer(){
	return  m_Time;}

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


