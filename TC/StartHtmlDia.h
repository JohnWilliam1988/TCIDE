#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CStartHtmlDia �Ի���

class CStartHtmlDia : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CStartHtmlDia)
public:
	CStartHtmlDia(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartHtmlDia();
// ��д
	/*HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);*/
	HRESULT ClosedAfterLoadCheck(IHTMLElement *pElement);
	HRESULT ShowWhenStartCheck(IHTMLElement *pElement);
	HRESULT OnProHistroyButton(IHTMLElement *pElement);
	HRESULT OnProUnNamed(IHTMLElement *pElement);
	HRESULT OnDllCommand(IHTMLElement *pElement);
	HRESULT OnBasicCommand(IHTMLElement *pElement);
	HRESULT OnScripSkill(IHTMLElement *pElement);
	HRESULT OnWebGame(IHTMLElement *pElement);
	HRESULT OnWebControl(IHTMLElement *pElement);
	
// �Ի�������
	enum { IDD = IDD_STARTHTMLDIA, IDH = IDR_HTML_STARTHTMLDIA };


	void   HistroyElement(IHTMLElement *pElement);
	void   ExampleElement(IHTMLElement *pElement);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
private:
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	BOOL	m_colsedAfterLoad;				//������Ŀ���Ƿ�ر�����ҳ��
	BOOL    m_showWhenStart;				//����TCʱ�Ƿ���ʾ����ҳ��
	BOOL    m_isLoaded;						//��ҳ�Ƿ��Ѿ�����
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	afx_msg void OnDestroy();
	virtual void OnOK();
};
