#pragma once


// CTCPath

class CTCBrowse : public CMFCEditBrowseCtrl
{
	//DECLARE_DYNAMIC(CTCPath)

public:
	CTCBrowse();
	virtual ~CTCBrowse();
private:
    CString m_eEnChange;
	bool	m_isAbled;			//记录是否有效
    CMFCEditBrowseCtrl::BrowseMode m_Mode;
public:
    void TCSetMode(CMFCEditBrowseCtrl::BrowseMode mode);

    CMFCEditBrowseCtrl::BrowseMode TCGetMode();

	CString TCGetEnChangeEvent()
	{ 
        return m_eEnChange;
    }

    void TCSetEnChangeEvent(CString csChange)
    {
        m_eEnChange=csChange;
    }
	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
};


