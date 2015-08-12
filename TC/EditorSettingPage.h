#pragma once
#include "resource.h"
#include "afxwin.h"
// CEditorSettingPage 对话框

//编辑器相关设置信息结构体
typedef struct EditorSetInfo
{
	CString fontFace;		//字体名称
	int fontSize;			//字体大小
	bool fontBold;			//字体加粗
	int tabWidth;			//Tab键字符宽度
	bool replaceSign;		//替换中文字符

	EditorSetInfo()
	{
		fontFace = _T("");
		fontSize = 0;
		fontBold = false;
		tabWidth = 8;
		replaceSign = false;
	}
};

class CEditorSettingPage : public CDialogEx
{
	friend class CTCSettingDia;
	DECLARE_DYNAMIC(CEditorSettingPage)

public:
	CEditorSettingPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditorSettingPage();

// 对话框数据
	enum { IDD = IDD_EDITORSETTING_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	//编辑器配置界面控件对象
	//CMFCFontComboBox m_fontNameCtrl;	
	CComboBox m_fontCombox;
	CComboBox m_fontSizeCtrl;
	CButton m_editorBoldTrue;
	CButton m_editorBoldFalse;
	//CMFCColorButton m_eidtorBack;
	CEdit m_tabWidthCtrl;
	CSpinButtonCtrl m_tabWidthSplinCtrl;
	CButton m_editorSignTrue;
	CButton m_editorSignFalse;

	//保存编辑器文字信息结构体对象
	EditorSetInfo m_EditInfo;
public:
	afx_msg void OnBnClickedEditorReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
