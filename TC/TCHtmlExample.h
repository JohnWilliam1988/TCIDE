#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CTCHtmlExample 对话框

class CTCHtmlExample : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CTCHtmlExample)

public:
	CTCHtmlExample(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCHtmlExample();
// 重写
	//HRESULT OnButtonOK(IHTMLElement *pElement);
	//HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIAEXAMPLE, IDH = IDR_HTML_TCHTMLEXAMPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
