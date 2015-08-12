#pragma once
#include "TCTabListBox.h"

// CTCTabProperty

class CTCTabProperty : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CTCTabProperty)

public:
	CTCTabProperty();
	virtual ~CTCTabProperty();

	void TCSetTabPerproty(CTCTabListBox* pListBox);
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
};


//这里控件都是用来处理下拉的配置的
class CComboBoxTabProp : public CMFCPropertyGridProperty
{
public:
	CComboBoxTabProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0, CTCTabProperty *cWnd=0);

protected:
	virtual CComboBox* CreateCombo(CWnd* pWndParent, CRect rect);
	virtual BOOL OnEdit(LPPOINT lptClick);

	CTCTabProperty *m_CProperTab;
};