#pragma once


// CTCRadio

class CTCRadio : public CButton
{
	DECLARE_DYNAMIC(CTCRadio)

public:
	CTCRadio();
	virtual ~CTCRadio();
private:
     CString m_eClick;
     bool m_isCheck;
	 bool m_isLeftText;			//标识是否文字居左
	 bool	m_isAbled;			//记录是否有效
	 bool m_isGroup;			//是否分组属性
public:
    void TCSetRadioEvent(CString csClick){
		m_eClick=csClick;}

	CString TCGetRadioEvent(){
		return m_eClick;}

    void TCSetCheck(bool isCheck)
    {
        m_isCheck=isCheck;
        this->SetCheck(m_isCheck);
    }

    bool TCGetCheck()
    {
        return m_isCheck;
    }
	void TCSetTextLeft(bool left);
	bool TCGetTextLeft()
	{
		return m_isLeftText;
	}
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}

	void TCSetGroup(bool set);
	bool TCGetGroup()
	{
		return m_isGroup;
	}
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


