#pragma once


// CTCButton

class CTCButton : public CButton
{
	DECLARE_DYNAMIC(CTCButton)

public:
	CTCButton();
	virtual ~CTCButton();
protected:
	CString m_eClick;           //记录单击执行的事件
	CString m_id;               //记录控件的ID
	bool	m_isMulityLine;		//记录按钮标题是否多行
	bool	m_defButton;		//记录是否默认按钮(响应Enter键)
	bool	m_isAbled;			//记录是否有效
protected:
	DECLARE_MESSAGE_MAP()

public:
	void TCSetID(CString csID){m_id=csID;}
	void TCSetClickEvent(CString eClick)
	{
		m_eClick = eClick;
	}
    CString TCGetClickEvent()
	{
		return m_eClick;
	}
	void TCSetButtonMulity(bool set);
	bool TCGetMulityLine()
	{
		return m_isMulityLine;
	}
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
	void TCSetDefButton(bool set);
	bool TCGetDefButton()
	{
		return m_defButton;
	}
public:
	afx_msg void OnBnDoubleclicked();
	afx_msg void OnBnClicked();
};


