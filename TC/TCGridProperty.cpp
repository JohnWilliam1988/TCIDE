// TCGridProperty.cpp : ʵ���ļ�
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



// CTCGridProperty ��Ϣ�������

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
		type = _T("Ĭ��");
		break;
	case TCGRID_CTRL_COMBOX:
		type = _T("������");
		break;
	case TCGRID_CTRL_DATE:
		type = _T("���ڿ�");
		break;
	/*case TCGRID_CTRL_URL:
		type = _T("URL");
		break;*/
	case TCGRID_CTRL_NUM:
		type = _T("���ֿ�");
		break;
	case TCGRID_CTRL_CHECK:
		type = _T("��ѡ��");
		break;
	default:
		break;
	}

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* columName = new CMFCPropertyGridProperty(_T("����"),(_variant_t)((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].title, _T("��ǰѡ���е�����"));
	CMFCPropertyGridProperty* columWidth = new CMFCPropertyGridProperty(_T("�п�"),(_variant_t)((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].colwidth, _T("��ǰѡ���е��п�"));
	CMFCPropertyGridProperty* columReadOnly = new CMFCPropertyGridProperty(_T("ֻ��"),(_variant_t)((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].readonly, _T("��ǰѡ�����Ƿ�ֻ��"));
	columReadOnly->SetData(DWORD_PTR(_T("ֻ������")));
	ctlBase->AddSubItem(columName);
	ctlBase->AddSubItem(columWidth);
	ctlBase->AddSubItem(columReadOnly);
	AddProperty(ctlBase);

	

	CMFCPropertyGridProperty* design = new CMFCPropertyGridProperty(_T("���"));
	CComboBoxGridProp* ctlType = new CComboBoxGridProp(_T("����"),(_variant_t)type, _T("��ǰ�еĿؼ�����"),0,this);	
	ctlType->AddOption(_T(""));

	ctlType->AllowEdit(FALSE);
	design->AddSubItem(ctlType);
	AddProperty(design);
}



void CTCGridProperty::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	// TODO: �ڴ����ר�ô����/����û���
	CString title = pProp->GetName();

	int select = ((CTCGridItems*)GetParent())->m_curSelectItem;
	if(select == -1)
		return;
	if(title == _T("����"))
	{
		CString newValue=pProp->GetValue().bstrVal;
		if(!newValue.IsEmpty())
		{
			((CTCGridItems*)GetParent())->m_columList->TCSetItemText(select,newValue);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = newValue;
		}
	}
	else if(title == _T("�п�"))
	{
		int width=pProp->GetValue().intVal;
		if(width!=0)
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].colwidth = width;
	}
	else if(title == _T("ֻ��"))
	{
		bool newValue,oldValue;   
		oldValue = pProp->GetOriginalValue().boolVal;
		newValue = pProp->GetValue().boolVal;
		((CTCGridItems*)GetParent())->m_TCGridStruct[select].readonly = newValue;
	}
	else if(title == _T("����"))
	{
		CString newValue,oldValue;   
		oldValue = pProp->GetOriginalValue().bstrVal;
		newValue = pProp->GetValue().bstrVal;
		newValue.Trim();
		
		if(newValue == _T("Ĭ��"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_NOMAL;
		}
		else if(newValue == _T("������"))
		{		
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_COMBOX;
		}
		/*else if(newValue == _T("ֻ��"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_READONLY;
		}*/
		else if(newValue == _T("��ѡ��"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_CHECK;
		}
		else if(newValue == _T("���ֿ�"))
		{
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].title = ((CTCGridItems*)GetParent())->m_columList->GetItemText(select);
			((CTCGridItems*)GetParent())->m_TCGridStruct[select].type = TCGRID_CTRL_NUM;
		}
		else if(newValue == _T("���ڿ�"))
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
	ctrlType.Add(_T("Ĭ��"));
	ctrlType.Add(_T("������"));
	ctrlType.Add(_T("��ѡ��"));
	ctrlType.Add(_T("���ڿ�"));
	//ctrlType.Add(_T("URL"));
	ctrlType.Add(_T("���ֿ�"));
	if(title == _T("����"))
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