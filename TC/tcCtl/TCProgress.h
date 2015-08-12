#pragma once


// CTCProgress

class CTCProgress : public CProgressCtrl
{
	DECLARE_DYNAMIC(CTCProgress)

public:
	CTCProgress();
	virtual ~CTCProgress();
	void TCSetVertical(bool set);
	bool TCGetVertical()
	{return m_isVertical;}
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
private:
	bool m_isVertical;		//标识滑块方向是否垂直
	bool	m_isAbled;			//记录是否有效
protected:
	DECLARE_MESSAGE_MAP()
};


