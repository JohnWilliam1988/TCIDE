#pragma once


// CTCComboBoxS

class CTCComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CTCComboBox)

public:
	CTCComboBox();
	virtual ~CTCComboBox();
private:
	bool m_isDropDown;		//标识显示风格
	bool	m_isAbled;			//记录是否有效
public:
	public:
	CString m_strLeftDown;
	CString m_strSelChange;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	void TCSetDropDown(bool set);
	bool TCGetDropDown(){ return m_isDropDown; }
	void SetEnChangeEvent(CString csEvent){m_strSelChange = csEvent;}

	CString GetEnChangeEvent(){ return m_strSelChange ;}

	void SetLeftDownEvent(CString csEvent){ m_strLeftDown=csEvent;}
	CString GetLeftDownEvent(){ return m_strLeftDown ;}
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
};


