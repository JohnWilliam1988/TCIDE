#pragma once


// CTCExplorer

class CTCExplorer : public CEdit
{
	DECLARE_DYNAMIC(CTCExplorer)

public:
	CTCExplorer();
private:
	CString m_url;//地址
	CString m_loadComplete;//加载完成
public:
	CString TCGetURL()
	{
		return m_url;
	}
	CString TCGetloadComplete()
	{
		return m_loadComplete;
	}
	void TCSetURL(CString str);
	void TCSetloadComplete(CString str);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual ~CTCExplorer();

protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};


