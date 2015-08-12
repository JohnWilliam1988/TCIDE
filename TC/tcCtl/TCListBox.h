#pragma once

// CTCListBox

class CTCListBox : public CVSListBox
{
	//DECLARE_DYNAMIC(CTCListBox)
public:
	CTCListBox();
private:
	CString m_strSelChange;
	CString m_strDoubleClick;
	bool	m_isAbled;			//记录是否有效
	bool	m_isDefaultRoll;	//添加项时滚动到最下
	virtual ~CTCListBox();
public:
	CString TCGetListBoxCaption()
	{
		return this->m_strCaption;
	}
	void TCSetListBoxSelChange(CString selChange)
	{
		m_strSelChange = selChange;
	}
	CString TCGetListBoxSelChange()
	{
		return m_strSelChange;
	}
	void TCSetListBoxDoubleClick(CString clicked)
	{
		m_strDoubleClick = clicked;
	}
	CString TCGetListBoxDoubleClick()
	{
		return m_strDoubleClick;
	}
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
	void TCSetDefaultRool(bool rool);
	bool TCGetDefaultRool()
	{
		return m_isDefaultRoll;
	}
protected:
	DECLARE_MESSAGE_MAP()
};


