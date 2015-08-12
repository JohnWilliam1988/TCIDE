#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//TC热键控件类
class CTCHotKeyCtrl : public CHotKeyCtrl
{
	DECLARE_DYNAMIC(CTCHotKeyCtrl)

public:
	CTCHotKeyCtrl();
	virtual ~CTCHotKeyCtrl();

public:
	//热键事件
	CString m_event;

	//失去焦点事件
	CString m_killFocusEvent;

	//热键组合的字符串
	CString m_HotKeyName;

	WORD m_wVirtualKeyCode;
	WORD m_wModifiers;

	bool	m_isAbled;			//记录是否有效
	//设置热键事件
	void TCSetKillFocusEvent(CString str)
	{
		m_killFocusEvent = str;
	}
	//取得热键事件
	CString TCGetKillFocusEvent()
	{
		return m_killFocusEvent;
	}

	//设置热键事件
	void TCSetHotKeyEvent(CString str)
	{
		m_event = str;
	}
	//取得热键事件
	CString TCGetHotKeyEvent()
	{
		return m_event;
	}

	//设置热键名称
	void SetHotKeyName(CString name);
	//注册热键
	int RegisterHotKey();
	//卸载热键
	int UnRegisterHotKey();

	//设置热键的键值
	void SetHotKeyValue(WORD wVirtualKeyCode,WORD wModifiers);

	//获得热键的键值
	void GetHotKeyValue(WORD &wVirtualKeyCode,WORD &wModifiers);

	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
		
protected:
	DECLARE_MESSAGE_MAP()

public:
	//热键事件驱动
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};


