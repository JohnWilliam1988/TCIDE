#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CVipCenterHtmlDia �Ի���

class CVipCenterHtmlDia : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CVipCenterHtmlDia)

public:
	CVipCenterHtmlDia(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVipCenterHtmlDia();
// ��д
	/*HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);*/
	HRESULT OnSkin1(IHTMLElement *pElement);
	HRESULT OnSkin2(IHTMLElement *pElement);
	HRESULT OnSkin3(IHTMLElement *pElement);
	HRESULT OnSkin4(IHTMLElement *pElement);

	HRESULT OnPlugImport(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_STARTHTMLDIA, IDH = IDR_HTML_VIPCENTERHTMLDIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
