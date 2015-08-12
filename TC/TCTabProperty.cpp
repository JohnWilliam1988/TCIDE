// TCTabProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "TCTableProperty.h"


// CTCTabProperty

IMPLEMENT_DYNAMIC(CTCTabProperty, CMFCPropertyGridCtrl)

CTCTabProperty::CTCTabProperty()
{

}

CTCTabProperty::~CTCTabProperty()
{
}


BEGIN_MESSAGE_MAP(CTCTabProperty, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()

void CTCTabProperty::TCSetTabPerproty(CTCTabListBox* pListBox)
{
	int curSel = pListBox->GetSelItem();
	if(curSel==-1)
		return;

	RemoveAll();

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("外观"));
	//CMFCPropertyGridProperty* columName = new CMFCPropertyGridProperty(_T("名称"),(_variant_t)((CTCGridItems*)GetParent())->m_TCGridStruct[curSel].title, _T("当前选中列的名称"));

	CMFCPropertyGridColorProperty* dlgColorProp = new CMFCPropertyGridColorProperty(_T("背景颜色"),((CTCTableProperty*)GetParent())->m_List->m_pagePerty[curSel].m_color, NULL, _T("指定默认的窗口背景颜色"));
	dlgColorProp->EnableOtherButton(_T("其他..."));
	dlgColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_BTNFACE));

	ctlBase->AddSubItem(dlgColorProp);
	
	//CComboBoxTabProp* dlgPic = new CComboBoxTabProp(_T("背景图片"),(_variant_t)((CTCTableProperty*)GetParent())->m_List->m_pagePerty[curSel].m_pic, _T("指定默认的窗口背景图片"),0,this);	
	//dlgPic->AddOption(_T(""));
	//dlgPic->AllowEdit(FALSE);
	//ctlBase->AddSubItem(dlgPic);
	AddProperty(ctlBase);
}

// CTCTabProperty 消息处理程序
void CTCTabProperty::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	// TODO: 在此添加专用代码和/或调用基类
	CString title = pProp->GetName();

	int curSel = ((CTCTableProperty*)GetParent())->m_List->GetSelItem();
	if(curSel==-1)
		return;

	if(title == _T("背景颜色"))
	{
		CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
		COLORREF color=pColorProp->GetColor();
		if(color == 0xffffffff)
			color = ::GetSysColor(COLOR_BTNFACE);
		((CTCTableProperty*)GetParent())->m_List->m_pagePerty[curSel].m_color = color;
	}
	/*else if(title == _T("背景图片"))
	{
		CString newValue=pProp->GetValue().bstrVal;
		if(newValue==_T("空"))
			((CTCTableProperty*)GetParent())->m_List->m_pagePerty[curSel].m_pic = _T("");
		else
			((CTCTableProperty*)GetParent())->m_List->m_pagePerty[curSel].m_pic = newValue;
		pProp->SetOriginalValue(pProp->GetValue());
	}*/
}



CComboBoxTabProp::CComboBoxTabProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr, DWORD dwData, CTCTabProperty *cWnd) :
CMFCPropertyGridProperty(strName, (LPCTSTR) strValue, lpszDescr, dwData)
{
	m_CProperTab=cWnd;
}

CComboBox* CComboBoxTabProp::CreateCombo(CWnd* pWndParent, CRect rect)
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

BOOL CComboBoxTabProp::OnEdit(LPPOINT lptClick)
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
	if(title == _T("背景图片"))
	{
		pWndComboEx->ResetContent();
		CString lpPath;
		lpPath.Format(_T("%s\\资源\\"),CTCRunTimeManager::g_ProjSourcePath);
		COMBOBOXEXITEM   Item; 
		Item.mask   =   CBEIF_TEXT ; 
		int i=0;

		Item.pszText   = _T("空"); 
		Item.iItem   =   i;//index 
		i++;
		pWndComboEx->InsertItem(&Item);
		{
			TCHAR szFind[MAX_PATH]={0};
			TCHAR szFile[MAX_PATH]={0};
			WIN32_FIND_DATA FindFileData;
			wcscpy_s(szFind,lpPath);
			wcscat_s(szFind,_T("*.*"));
			HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
			if(INVALID_HANDLE_VALUE == hFind)
				return TRUE;
			while(TRUE)
			{
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(FindFileData.cFileName[0]!=_T('.'))
					{

					}
				}
				else
				{
					CString csFileName=FindFileData.cFileName;
					csFileName.Trim();
					csFileName.MakeLower();
					CString csFileEx=csFileName.Right(5);
					CString csPictEx=csFileName.Right(4);
					if(0==::wcscmp(csPictEx,_T(".jpg"))||
						0==::wcscmp(csPictEx,_T(".bmp"))||
						0==::wcscmp(csPictEx,_T(".png"))||
						0==::wcscmp(csPictEx,_T(".ico")))
					{
						Item.pszText   = FindFileData.cFileName; 
						Item.iItem   =   i;//index 
						i++;
						pWndComboEx->InsertItem(&Item);
					}
				}
				if(!FindNextFile(hFind,&FindFileData))
					break;
			}
			FindClose(hFind);
		}
	}
	
	return TRUE;
}
