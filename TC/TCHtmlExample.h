#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CTCHtmlExample �Ի���

class CTCHtmlExample : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CTCHtmlExample)

public:
	CTCHtmlExample(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCHtmlExample();
// ��д
	//HRESULT OnButtonOK(IHTMLElement *pElement);
	//HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIAEXAMPLE, IDH = IDR_HTML_TCHTMLEXAMPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()

public:
	IHTMLElement* pLinkElement;
	void SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	/*afx_msg void OnBnClickedOk();*/
};
