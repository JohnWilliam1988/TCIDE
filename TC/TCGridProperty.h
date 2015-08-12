#pragma once
#include "TCGridListBox.h"
#include "tcCtl/GridCtrl_src/GridCtrl.h"


// CTCGridProperty

class CTCGridListBox;

class CTCGridProperty : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CTCGridProperty)

public:
	CTCGridProperty();
	virtual ~CTCGridProperty();
	
	void TCSetGridColumPerproty(CTCGridListBox* pGridListBox);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
};


//这里控件都是用来处理下拉的配置的
class CComboBoxGridProp : public CMFCPropertyGridProperty
{
public:
	CComboBoxGridProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0, CTCGridProperty *cWnd=0);

protected:
	virtual CComboBox* CreateCombo(CWnd* pWndParent, CRect rect);
	virtual BOOL OnEdit(LPPOINT lptClick);

	CTCGridProperty *m_CProperGrid;
};
