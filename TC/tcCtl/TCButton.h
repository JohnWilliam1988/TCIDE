#pragma once


// CTCButton

class CTCButton : public CButton
{
	DECLARE_DYNAMIC(CTCButton)

public:
	CTCButton();
	virtual ~CTCButton();
protected:
	CString m_eClick;           //��¼����ִ�е��¼�
	CString m_id;               //��¼�ؼ���ID
	bool	m_isMulityLine;		//��¼��ť�����Ƿ����
	bool	m_defButton;		//��¼�Ƿ�Ĭ�ϰ�ť(��ӦEnter��)
	bool	m_isAbled;			//��¼�Ƿ���Ч
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


