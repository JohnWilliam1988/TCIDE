// TCGridProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCGridItems.h"
#include "TCGridProperty.h"
#include "TCRunTimeManager.h"


// CTCGridProperty

IMPLEMENT_DYNAMIC(CTCGridProperty, CMFCPropertyGridCtrl)

CTCGridProperty::CTCGridProperty()
{
}

CTCGridProperty::~CTCGridProperty()
{
}


BEGIN_MESSAGE_MAP(CTCGridProperty, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()



// CTCGridProperty 消息处理程序

void CTCGridProperty::TCSetGridColumPerproty(CTCGridListBox* pGridListBox)
{
	RemoveAll();

	CString type;
	int curSel = pGridListBox->GetSelItem();
	if(curSel==-1)
		return;
	TCGRIDCTRLTYPE ctrltype=((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].type;
	switch(ctrltype)
	{
	case TCGRID_CTRL_NOMAL:
		type = _T("默认");
		break;
	case TCGRID_CTRL_COMBOX:
		type = _T("下拉框");
		break;
	case TCGRID_CTRL_DATE:
		type = _T("日期框");
		break;
	/*case TCGRID_CTRL_URL:
		type = _T("URL");
		break;*/
	case TCGRID_CTRL_NUM:
		type = _T("数字框");
		break;
	case TCGRID_CTRL_CHECK:
		type = _T("复选框");
		break;
	default:
		break;
	}

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
	CMFCPropertyGridProperty* columName = new CMFCPropertyGridProperty(_T("列名"),(_variant_t)((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].title, _T("当前选中列的名称"));
	CMFCPropertyGridProperty* columWidth = new CMFCPropertyGridProperty(_T("列宽"),(_variant_t)((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].colwidth, _T("当前选中列的列宽"));
	CMFCPropertyGridProperty* columReadOnly = new CMFCPropertyGridProperty(_T("只读"),(_variant_t)((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].readonly, _T("当前选中列是否只读"));
	columReadOnly->SetData(DWORD_PTR(_T("只读属性")));
	ctlBase->AddSubItem(columName);
	ctlBase->AddSubItem(columWidth);
	ctlBase->AddSubItem(columReadOnly);
	AddProperty(ctlBase);

	

	CMFCPropertyGridProperty* design = new CMFCPropertyGridProperty(_T("设计"));
	CComboBoxGridProp* ctlType = new CComboBoxGridProp(_T("类型"),(_variant_t)type, _T("当前列的控件类型"),0,this);	
	ctlType->AddOption(_T(""));

	ctlType->AllowEdit(FALSE);
	design->AddSubItem(ctlType);
	AddProperty(design);
}



void CTCGridProperty::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	// TODO: 在此添加专用代码和/或调用基类
	CString title = pProp->GetName();

	int select = ((CTCGridItems*)GetParent())->m_curSelectItem;
	if(select == -1)
		return;
	if(title == _T("列名"))
	{
		CString newValue=pProp->GetValue().bstrVal;
		if(!newValue.IsEmpty())
		{
			((CTCGridItems*)GetParent())->m_columList->TCSetItemText(select,newValue);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = newValue;
		}
	}
	else if(title == _T("列宽"))
	{
		int width=pProp->GetValue().intVal;
		if(width!=0)
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].colwidth = width;
	}
	else if(title == _T("只读"))
	{
		bool newValue,oldValue;   
		oldValue = pProp->GetOriginalValue().boolVal;
		newValue = pProp->GetValue().boolVal;
		((CTCGridItems*)GetParent())->m_TCGridStruct[select].readonly = newValue;
	}
	else if(title == _T("类型"))
	{
		CString newValue,oldValue;   
		oldValue = pProp->GetOriginalValue().bstrVal;
		newValue = pProp->GetValue().bstrVal;
		newValue.Trim();
		
		if(newValue == _T("默认"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_NOMAL;
		}
		else if(newValue == _T("下拉框"))
		{		
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_COMBOX;
		}
		/*else if(newValue == _T("只读"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_READONLY;
		}*/
		else if(newValue == _T("复选框"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_CHECK;
		}
		else if(newValue == _T("数字框"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_NUM;
		}
		else if(newValue == _T("日期框"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_DATE;
		}
		/*else if(newValue == _T("URL"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_URL;
		}*/
	}
	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

CComboBoxGridProp::CComboBoxGridProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr, DWORD dwData, CTCGridProperty *cWnd) :
CMFCPropertyGridProperty(strName, (LPCTSTR) strValue, lpszDescr, dwData)
{
	m_CProperGrid=cWnd;

}

CComboBox* CComboBoxGridProp::CreateCombo(CWnd* pWndParent, CRect rect)
{
	rect.bottom = rect.top + 400;

	CComboBoxEx* pWndCombo = new CComboBoxEx;

	if(!pWndCombo->Create(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, rect, pWndParent, AFX_PROPLIST_ID_INPLACE))
	{
		delete pWndCombo;
		return NULL;
	}

	return pWndCombo;
}

BOOL CComboBoxGridProp::OnEdit(LPPOINT lptClick)
{
	if (!CMFCPropertyGridProperty::OnEdit(lptClick))
	{
		return FALSE;
	}
	CComboBoxEx* pWndComboEx = DYNAMIC_DOWNCAST(CComboBoxEx, m_pWndCombo);
	if (pWndComboEx == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CString title=this->GetName();
	CStringArray ctrlType;
	ctrlType.Add(_T("默认"));
	ctrlType.Add(_T("下拉框"));
	ctrlType.Add(_T("复选框"));
	ctrlType.Add(_T("日期框"));
	//ctrlType.Add(_T("URL"));
	ctrlType.Add(_T("数字框"));
	if(title == _T("类型"))
	{
		pWndComboEx->ResetContent();
		COMBOBOXEXITEM   Item; 
		Item.mask   =   CBEIF_TEXT ; 
		for(int i=0;i<ctrlType.GetSize();i++)
		{
			Item.pszText = (LPWSTR)(LPCWSTR)ctrlType[i];
			Item.iItem = i;
			pWndComboEx->InsertItem(&Item);
		}
	}
	return TRUE;
}