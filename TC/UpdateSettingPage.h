#pragma once


// CUpdateSettingPage 对话框

class CUpdateSettingPage : public CDialogEx
{
	friend class CTCSettingDia;
	DECLARE_DYNAMIC(CUpdateSettingPage)

public:
	CUpdateSettingPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateSettingPage();

// 对话框数据
	enum { IDD = IDD_UPDATESETTING_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CButton m_updateTrue;
	CButton m_updateFalse;
public:
	virtual BOOL OnInitDialog();
};
