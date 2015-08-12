#pragma once


// CTCDate

class CTCDate : public CDateTimeCtrl
{
	DECLARE_DYNAMIC(CTCDate)
private:
	bool	m_isAbled;			//记录是否有效
public:
	CTCDate();
	virtual ~CTCDate();
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
protected:
	DECLARE_MESSAGE_MAP()
};


