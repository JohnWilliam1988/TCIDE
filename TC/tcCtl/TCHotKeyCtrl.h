#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
//TC�ȼ��ؼ���
class CTCHotKeyCtrl : public CHotKeyCtrl
{
	DECLARE_DYNAMIC(CTCHotKeyCtrl)

public:
	CTCHotKeyCtrl();
	virtual ~CTCHotKeyCtrl();

public:
	//�ȼ��¼�
	CString m_event;

	//ʧȥ�����¼�
	CString m_killFocusEvent;

	//�ȼ���ϵ��ַ���
	CString m_HotKeyName;

	WORD m_wVirtualKeyCode;
	WORD m_wModifiers;

	bool	m_isAbled;			//��¼�Ƿ���Ч
	//�����ȼ��¼�
	void TCSetKillFocusEvent(CString str)
	{
		m_killFocusEvent = str;
	}
	//ȡ���ȼ��¼�
	CString TCGetKillFocusEvent()
	{
		return m_killFocusEvent;
	}

	//�����ȼ��¼�
	void TCSetHotKeyEvent(CString str)
	{
		m_event = str;
	}
	//ȡ���ȼ��¼�
	CString TCGetHotKeyEvent()
	{
		return m_event;
	}

	//�����ȼ�����
	void SetHotKeyName(CString name);
	//ע���ȼ�
	int RegisterHotKey();
	//ж���ȼ�
	int UnRegisterHotKey();

	//�����ȼ��ļ�ֵ
	void SetHotKeyValue(WORD wVirtualKeyCode,WORD wModifiers);

	//����ȼ��ļ�ֵ
	void GetHotKeyValue(WORD &wVirtualKeyCode,WORD &wModifiers);

	void TCSetAbled(bool set);
	bool TCGetAbled()
	{
		return m_isAbled;
	}
		
protected:
	DECLARE_MESSAGE_MAP()

public:
	//�ȼ��¼�����
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};


