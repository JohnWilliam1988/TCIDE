#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CStartHtmlDia 对话框

class CStartHtmlDia : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CStartHtmlDia)
public:
	CStartHtmlDia(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartHtmlDia();
// 重写
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
	
// 对话框数据
	enum { IDD = IDD_STARTHTMLDIA, IDH = IDR_HTML_STARTHTMLDIA };


	void   HistroyElement(IHTMLElement *pElement);
	void   ExampleElement(IHTMLElement *pElement);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
private:
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	BOOL	m_colsedAfterLoad;				//加载项目后是否关闭启动页面
	BOOL    m_showWhenStart;				//启动TC时是否显示启动页面
	BOOL    m_isLoaded;						//网页是否已经加载
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	afx_msg void OnDestroy();
	virtual void OnOK();
};
