#pragma once

// CTCCheck

class CTCCheck : public CButton
{
	DECLARE_DYNAMIC(CTCCheck)

public:
	CTCCheck();
	virtual ~CTCCheck();
private:
     CString m_eClick;
     bool m_isCheck;
	 bool m_isLeftText;
	 bool	m_isAbled;			//记录是否有效
public:
    void TCSetCheckEvent(CString csClick){
		m_eClick=csClick;}

	CString TCGetCheckEvent(){
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
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	DECLARE_MESSAGE_MAP()
};


