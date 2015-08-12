// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

#pragma once

#include "TCRunTimeManager.h"
#include "TCCtlHeads.h"

class CDiaTCDesign;
struct TCPROPERTYMSG
{
    TCPROPERTYTYPE type;
    CString        name;
	HWND pWnd;
    TCPROPERTYMSG()
    {
        type=TCPROPERTY_MAX;
        name=_T("");
		pWnd=NULL;
    }
};

struct TCGETZORDERINFO
{
	int InControlAllPos; //当前控件在橡皮筋数组中的位置 
	int ControlSize;	 //当前橡皮筋数组的大小
	TCGETZORDERINFO()
	{
		InControlAllPos = 0;
		ControlSize = 0;
	}
};

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


class CPropertiesWnd : public CDockablePane
{
// 构造
public:
	CPropertiesWnd();

	void AdjustLayout();

// 特性
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

    //属性
public:
      TCPROPERTYMSG m_CurrentType;

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
// 实现
public:
	virtual ~CPropertiesWnd();
    void SetProjProperty(CString name,CString path,BOOL upDate = FALSE);
	void SetRcNodeDiaProperty(CString name);
    void SetTWinDiaProperty(CString name,const CDiaTCDesign * const dlg,BOOL upDate = FALSE);
    void SetTWinStaticProperty(CString id, CTCStatic *  ctl,BOOL upDate = FALSE);
    void SetTWinGroupProperty(CString id, CTCGroupBox * ctl,BOOL upDate = FALSE);
    void SetTWinButtonProperty(CString id, CTCButton *  ctl,BOOL upDate = FALSE);
    void SetTWinEditProperty(CString id, CTCEdit *  ctl,BOOL upDate = FALSE);
    void SetTWinCheckProperty(CString id, CTCCheck *  ctl,BOOL upDate = FALSE);
    void SetTWinRadioProperty(CString id, CTCRadio *  ctl,BOOL upDate = FALSE);
    void SetTWinComboxProperty(CString id, CTCComboBox *  ctl,BOOL upDate = FALSE);
    void SetTWinPictureProperty(CString id, CTCPicture *  ctl,BOOL upDate = FALSE);
    void SetTWinProgressProperty(CString id, CTCProgress *  ctl,BOOL upDate = FALSE);
    void SetTWinPathProperty(CString id, CTCBrowse *  ctl,BOOL upDate = FALSE);
    void SetTWinSliderProperty(CString id, CTCSlider *  ctl,BOOL upDate = FALSE);
    void SetTWinDateProperty(CString id, CTCDate *  ctl,BOOL upDate = FALSE);
    void SetTWinListProperty(CString id, CTCListBox *  ctl,BOOL upDate = FALSE);
    void SetTWinTableProperty(CString id, CTCTab*  ctl,BOOL upDate = FALSE);
	void SetTWinTimerProperty(CString id, CTCTimer*  ctl,BOOL upDate = FALSE);
	void SetTWinHotKeyProperty(CString id, CTCHotKeyCtrl *  ctl,BOOL upDate = FALSE);
	void SetTWinExplorerProperty(CString id, CTCExplorer *  ctl,BOOL upDate = FALSE);
	void SetTWinGridCtrlProperty(CString id, CGridCtrl *  ctl,BOOL upDate = FALSE);
    //////////////////////////////修改单个属性值的接口//////////////////////////////////////
    void SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue);
	/////修改单个属性框的值,字符型
	void SetPropertyString(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue);
	//Z序相关函数
	void TCGetZOrderInfo(TCGETZORDERINFO &info,CWnd* pCtrl);
	//////////////////////////////添加控件名称到属性窗口下拉框//////////////////////////////
	void InsertCtrlToProCom(CString ctrlName);
	/////////////////////////////重置下拉框////////////////////
	void ResetPropertiesCombox();
	/////////////////////////////设置下拉框当前选中项///////////////////
	void SetPropComboxCursel(CString ctrlID);
	////////////////////////////去掉Tab中的选中状态//////////////////////
	void DeSelectCtrlsInTab();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg LRESULT OnPropertyChanged (WPARAM,LPARAM);
	afx_msg void OnSelchangeCombo1(); 
	afx_msg void OnDropDownCombo1();

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};


//这里控件都是用来处理事件的配置的
class CComboBoxExProp : public CMFCPropertyGridProperty
{
public:
	CComboBoxExProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0, CPropertiesWnd *cWnd=0);

protected:
	virtual CComboBox* CreateCombo(CWnd* pWndParent, CRect rect);
    virtual BOOL OnEdit(LPPOINT lptClick);

	CPropertiesWnd *m_CProperWnd;
};

//自定义按钮属性
class CCustomDlgProp : public CMFCPropertyGridProperty
{
public:
	CCustomDlgProp(const CString& strName, const COleVariant& varValue, CPropertiesWnd *cWnd);

protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);

	CPropertiesWnd *m_CProperWnd;
};
