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
	bool m_isVertical;		//��ʶ���鷽���Ƿ�ֱ
	bool	m_isAbled;			//��¼�Ƿ���Ч
protected:
	DECLARE_MESSAGE_MAP()
};


