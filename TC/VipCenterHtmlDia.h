#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CVipCenterHtmlDia 对话框

class CVipCenterHtmlDia : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CVipCenterHtmlDia)

public:
	CVipCenterHtmlDia(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVipCenterHtmlDia();
// 重写
	/*HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);*/
	HRESULT OnSkin1(IHTMLElement *pElement);
	HRESULT OnSkin2(IHTMLElement *pElement);
	HRESULT OnSkin3(IHTMLElement *pElement);
	HRESULT OnSkin4(IHTMLElement *pElement);

	HRESULT OnPlugImport(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_STARTHTMLDIA, IDH = IDR_HTML_VIPCENTERHTMLDIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
