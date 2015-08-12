// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

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
	int InControlAllPos; //��ǰ�ؼ�����Ƥ�������е�λ�� 
	int ControlSize;	 //��ǰ��Ƥ������Ĵ�С
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
// ����
public:
	CPropertiesWnd();

	void AdjustLayout();

// ����
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

    //����
public:
      TCPROPERTYMSG m_CurrentType;

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
// ʵ��
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
    //////////////////////////////�޸ĵ�������ֵ�Ľӿ�//////////////////////////////////////
    void SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue);
	/////�޸ĵ������Կ��ֵ,�ַ���
	void SetPropertyString(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue);
	//Z����غ���
	void TCGetZOrderInfo(TCGETZORDERINFO &info,CWnd* pCtrl);
	//////////////////////////////��ӿؼ����Ƶ����Դ���������//////////////////////////////
	void InsertCtrlToProCom(CString ctrlName);
	/////////////////////////////����������////////////////////
	void ResetPropertiesCombox();
	/////////////////////////////����������ǰѡ����///////////////////
	void SetPropComboxCursel(CString ctrlID);
	////////////////////////////ȥ��Tab�е�ѡ��״̬//////////////////////
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


//����ؼ��������������¼������õ�
class CComboBoxExProp : public CMFCPropertyGridProperty
{
public:
	CComboBoxExProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0, CPropertiesWnd *cWnd=0);

protected:
	virtual CComboBox* CreateCombo(CWnd* pWndParent, CRect rect);
    virtual BOOL OnEdit(LPPOINT lptClick);

	CPropertiesWnd *m_CProperWnd;
};

//�Զ��尴ť����
class CCustomDlgProp : public CMFCPropertyGridProperty
{
public:
	CCustomDlgProp(const CString& strName, const COleVariant& varValue, CPropertiesWnd *cWnd);

protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);

	CPropertiesWnd *m_CProperWnd;
};
