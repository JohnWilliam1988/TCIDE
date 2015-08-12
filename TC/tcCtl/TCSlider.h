#pragma once


// CTCSlider

class CTCSlider : public CSliderCtrl
{
	DECLARE_DYNAMIC(CTCSlider)

public:
	CTCSlider();
	virtual ~CTCSlider();

private:	
    CString m_eEnChange;
	bool m_isVertical;		//标识滑块方向是否垂直
	bool	m_isAbled;			//记录是否有效
public:
    void TCSetSliderChangeEvent(CString sel)
	{ m_eEnChange = sel;}
	CString TCGetSliderChangeEvent()
	{return m_eEnChange;}

	void TCSetVertical(bool set);
	bool TCGetVertical()
	{return m_isVertical;}
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
protected:
	DECLARE_MESSAGE_MAP()
};


