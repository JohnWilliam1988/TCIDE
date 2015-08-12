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

#include "stdafx.h"
#include "TCExeVersionManager.h"
#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "TC.h"

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"

#include "TCGridItems.h"
#include "TCListBoxItem.h"
#include "TCComBoxItem.h"
#include "TCTableProperty.h"
#include "TCHotKeySetting.h"
#include "Docommand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
    m_CurrentType.name=_T("");
    m_CurrentType.type=TCPROPERTY_MAX;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
    ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
    ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
    ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    ON_COMMAND(ID_PROPERTIES1, OnProperties1)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
    ON_COMMAND(ID_PROPERTIES2, OnProperties2)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_CBN_SELCHANGE(1,OnSelchangeCombo1) 
	ON_CBN_DROPDOWN(1,OnDropDownCombo1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar ��Ϣ�������

void CPropertiesWnd::AdjustLayout()
{
    if (GetSafeHwnd() == NULL)
    {
        return;
    }

    CRect rectClient,rectCombo;
    GetClientRect(rectClient);

    m_wndObjectCombo.GetWindowRect(&rectCombo);

    int cyCmb = rectCombo.Size().cy;
    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // �������:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_VSCROLL | WS_CLIPCHILDREN;

    if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
    {
        TRACE0("δ�ܴ���������� \n");
        return -1;      // δ�ܴ���
    }

    //m_wndObjectCombo.AddString(_T("������"));
    //m_wndObjectCombo.AddString(_T("���Դ���"));
    m_wndObjectCombo.SetCurSel(0);

    if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
    {
        TRACE0("δ�ܴ�����������\n");
        return -1;      // δ�ܴ���
    }

    InitPropList();

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
    m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ������*/);
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ����*/);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
}

///////////////////////////��ӿؼ����Ƶ����Դ���������///////////////
void CPropertiesWnd::InsertCtrlToProCom(CString ctrlName)
{
	m_wndObjectCombo.SetCurSel(0);
	m_wndObjectCombo.AddString(ctrlName);
}

void CPropertiesWnd::OnSelchangeCombo1()
{
	int index = m_wndObjectCombo.GetCurSel();
	if( -1 == index )
		return;

	CString selectName;
	m_wndObjectCombo.GetLBText(index,selectName);

	selectName.Trim();
	selectName.MakeLower();

	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(!pCurrent)
	{
		m_wndObjectCombo.SendMessage(WM_KILLFOCUS);
		pCurrent->SetFocus();
		return;
	}

	if(_T("������") == selectName || _T("���Դ���") == selectName )
	{
		::AfxMessageBox(_T("�����"));
	}
	else 
	{
		//�ؼ�����
		TCTOOLTYPE ctrlType = TCTT_MAX;
		CControlRect *pCtrlRect = NULL;

		//�������пؼ���ID��ѡ�����Ա�
		int size = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i = 0;i < size ; i++)
		{
			if(pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CString idTab = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();
				//�����Tab��ID
				if( idTab == selectName )
				{
					pCtrlRect = pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(idTab);
					//�������Deselect��Tab�е�
					DeSelectCtrlsInTab();
					//���в���Tab��
					pCurrent->m_DiaDesign.m_ControlTracker.DeSelectAll();

					pCurrent->m_DiaDesign.m_ControlTracker.SetCreateCtrlSelected(pCtrlRect);
					ctrlType = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
				}
				else
				{
					CTCTab* pTab = (CTCTab*)pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{
						int pageIndex=j;
						int pageSize=((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll.GetSize();
						for(int k = 0;k < pageSize; k++)
						{
							CString idInTab = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlID();
							//Tab����Ԫ�ص�ID
							if( idInTab == selectName )
							{
								pCtrlRect = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.GetRectByID(idInTab);

								pCurrent->m_DiaDesign.m_ControlTracker.DeSelectAll();
								
								pTab->SetCurSel(j);
								pTab->showIndex(j);

								((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.SetCreateCtrlSelected(pCtrlRect);
								ctrlType = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlType();
							}
						}
					}
				}
			}
			else
			{
				CString idCtrls = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();
				if( idCtrls == selectName )
				{
					pCtrlRect = pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(idCtrls);
					//�������Deselect��Tab�е�
					DeSelectCtrlsInTab();
					pCurrent->m_DiaDesign.m_ControlTracker.SetCreateCtrlSelected(pCtrlRect);
					ctrlType = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
				}
			}
		}
		//ˢ��һ�½���
		pCurrent->m_DiaDesign.Invalidate(TRUE);

		if( ctrlType == TCTT_MAX || pCtrlRect == NULL)
			return;
	
		switch(ctrlType)
		{
		case TCTT_STATIC:
			//CTCStatic *pStatic = (CTCStatic*)pCtrlRect->GetCWnd();
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(selectName,(CTCStatic*)pCtrlRect->GetCWnd());
			break;
		case TCTT_BUTTON:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(selectName,(CTCButton *)pCtrlRect->GetCWnd());
			break;
		case TCTT_EDIT:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(selectName,(CTCEdit *)pCtrlRect->GetCWnd());
			break;
		case TCTT_RADIO:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(selectName,(CTCRadio *)pCtrlRect->GetCWnd());
			break;
		case TCTT_CHECK:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(selectName,(CTCCheck *)pCtrlRect->GetCWnd());
			break;
		case TCTT_COMBOBOX:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(selectName,(CTCComboBox *)pCtrlRect->GetCWnd());
			break;
		case TCTT_GROUP:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(selectName,(CTCGroupBox *)pCtrlRect->GetCWnd());
			break;
		case TCTT_LISTBOX:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(selectName,(CTCListBox *)pCtrlRect->GetCWnd());
			break;
		case TCTT_PROGRESS:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(selectName,(CTCProgress *)pCtrlRect->GetCWnd());
			break;
		case TCTT_DATE:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(selectName,(CTCDate *)pCtrlRect->GetCWnd());
			break;
		case TCTT_PICTURE:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(selectName,(CTCPicture *)pCtrlRect->GetCWnd());
			break;
		case TCTT_SLIDER:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(selectName,(CTCSlider *)pCtrlRect->GetCWnd());
			break;
		case TCTT_PATH:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(selectName,(CTCBrowse *)pCtrlRect->GetCWnd());
			break;
		case TCTT_TAB:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(selectName,(CTCTab *)pCtrlRect->GetCWnd());
			break;
		case TCTT_TIMER:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(selectName,(CTCTimer *)pCtrlRect->GetCWnd());
			break;
		case TCTT_HOTKEY:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(selectName,(CTCHotKeyCtrl *)pCtrlRect->GetCWnd());
			break;
		case TCTT_GRIDCTRL:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(selectName,(CGridCtrl *)pCtrlRect->GetCWnd());
			break;
		case  TCTT_EXPLORER:
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(selectName,(CTCExplorer *)pCtrlRect->GetCWnd());
			break;
		}
	}  
	
}

void CPropertiesWnd::DeSelectCtrlsInTab()
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

	if(!pCurrent)
	{
		return;
	}
	int size = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
	for(int i = 0;i < size ; i++)
	{
		if(pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab* pTab = (CTCTab*)pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
			for(int j=0;j< pTab->getTabPageCount();j++)
			{
				((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelectAll();
			}
		}
	}
}

void CPropertiesWnd::ResetPropertiesCombox()
{
	m_wndObjectCombo.ResetContent();
}

void CPropertiesWnd::SetPropComboxCursel(CString ctrlID)
{
	for( int i=0;i<m_wndObjectCombo.GetCount();i++ )
	{
		CString selectStr;
		m_wndObjectCombo.GetLBText(i,selectStr);
		if( selectStr == ctrlID )
			m_wndObjectCombo.SetCurSel(i);
	}
}

void CPropertiesWnd::OnDropDownCombo1()
{	
	//���������
	m_wndObjectCombo.ResetContent();

	if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_currentViewType == TCVIEW_DESIGN )
	{
		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

		if(!pCurrent)
		{
			return;
		}

		int index = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
	
		for(int i = 0;i < index ; i++)
		{
			if(pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CString idTab = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();
				//Tab��ID
				m_wndObjectCombo.AddString(idTab);
				CTCTab* pTab = (CTCTab*)pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int pageIndex=j;
					int pageSize=((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					for(int k = 0;k < pageSize; k++)
					{
						CString idInTab = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlID();
						//Tab����Ԫ�ص�ID
						m_wndObjectCombo.AddString(idInTab);
					}
				}
			}
			else
			{
				CString idCtrls = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();
				m_wndObjectCombo.AddString(idCtrls);
			}

		}
	}

}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
    m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
    m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
    // TODO: �ڴ˴���������������
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
    // TODO: �ڴ˴����������� UI ����������
}

void CPropertiesWnd::OnProperties2()
{
    // TODO: �ڴ˴���������������
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
    // TODO: �ڴ˴����������� UI ����������
}

void CPropertiesWnd::InitPropList()
{
    SetPropListFont();

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea();
    m_wndPropList.SetVSDotNetLook();
    m_wndPropList.MarkModifiedProperties();

    return;

    CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("���"));

    pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��ά���"), (_variant_t) false, _T("ָ�����ڵ����岻ʹ�ô��壬���ҿؼ���ʹ����ά�߿�")));

    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("�߿�"), _T("�Ի������"), _T("����֮һ:���ޡ�����ϸ�������ɵ�����С���򡰶Ի������"));
    pProp->AddOption(_T("��"));
    pProp->AddOption(_T("ϸ"));
    pProp->AddOption(_T("�ɵ�����С"));
    pProp->AddOption(_T("�Ի������"));
    pProp->AllowEdit(FALSE);

    pGroup1->AddSubItem(pProp);
    pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));

    m_wndPropList.AddProperty(pGroup1);

    CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("���ڴ�С"), 0, TRUE);

    pProp = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t) 250l, _T("ָ�����ڵĸ߶�"));
    pProp->EnableSpinControl(TRUE, 50, 300);
    pSize->AddSubItem(pProp);

    pProp = new CMFCPropertyGridProperty( _T("���"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
    pProp->EnableSpinControl(TRUE, 50, 200);
    pSize->AddSubItem(pProp);

    m_wndPropList.AddProperty(pSize);

    CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("����"));

    LOGFONT lf;
    CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    font->GetLogFont(&lf);

    lstrcpy(lf.lfFaceName, _T("����, Arial"));

    pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ�����ڵ�Ĭ������")));
    pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ʹ��ϵͳ����"), (_variant_t) true, _T("ָ������ʹ�á�MS Shell Dlg������")));

    m_wndPropList.AddProperty(pGroup2);

    CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("����"));
    pProp = new CMFCPropertyGridProperty(_T("(����)"), _T("Ӧ�ó���"));
    pProp->Enable(FALSE);
    pGroup3->AddSubItem(pProp);
    CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), RGB(210, 192, 254), NULL, _T("ָ��Ĭ�ϵĴ�����ɫ"));
    pColorProp->EnableOtherButton(_T("����..."));
    pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
    pGroup3->AddSubItem(pColorProp);
    static const TCHAR szFilter[] = _T("ͼ���ļ�(*.ico)|*.ico|�����ļ�(*.*)|*.*||");
    pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("ͼ��"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("ָ������ͼ��")));
    pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("�ļ���"), _T("c:\\")));
    m_wndPropList.AddProperty(pGroup3);
    CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("��νṹ"));
    CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("��һ���Ӽ�"));
    pGroup4->AddSubItem(pGroup41);
    CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�ڶ����Ӽ�"));
    pGroup41->AddSubItem(pGroup411);
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 1"), (_variant_t) _T("ֵ 1"), _T("��Ϊ˵��")));
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 2"), (_variant_t) _T("ֵ 2"), _T("��Ϊ˵��")));
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 3"), (_variant_t) _T("ֵ 3"), _T("��Ϊ˵��")));
    pGroup4->Expand(FALSE);
    m_wndPropList.AddProperty(pGroup4);
    //m_wndPropList.RemoveAll();
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);
    m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
    ::DeleteObject(m_fntPropList.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;

    m_fntPropList.CreateFontIndirect(&lf);

    m_wndPropList.SetFont(&m_fntPropList);
    m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CPropertiesWnd::SetProjProperty(CString name,CString path,BOOL upDate)
{
    m_CurrentType.type=TCPROPERTY_PROJ;
    m_CurrentType.name=path;
    m_wndPropList.RemoveAll();

    CMFCPropertyGridProperty* Basic = new CMFCPropertyGridProperty(_T("����"));

    CMFCPropertyGridProperty* FileName = new CMFCPropertyGridProperty(_T("�ļ���"), (_variant_t)name, _T("��ǰԴ�ļ�������"));
    FileName->SetData(DWORD_PTR(_T("�ļ���")));
    Basic->AddSubItem(FileName);

    CMFCPropertyGridProperty* FilePath = new CMFCPropertyGridProperty(_T("����·��"),path, _T("�ļ�������·��"));
    FilePath->Enable(FALSE);
    FilePath->SetData(DWORD_PTR(_T("����·��")));
    Basic->AddSubItem(FilePath);
    m_wndPropList.AddProperty(Basic);

    CMFCPropertyGridProperty* proj = new CMFCPropertyGridProperty(_T("��Ŀ"));

    CMFCPropertyGridProperty* enterFile = new CMFCPropertyGridProperty(_T("�����ļ�"),CTCRunTimeManager::g_projEnterFileName, _T("��Ŀ�������ļ�"));
    CMFCPropertyGridProperty* projName = new CMFCPropertyGridProperty(_T("��Ŀ����"),CTCRunTimeManager::g_ProjName, _T("��ǰ��Ŀ������"));
    enterFile->AllowEdit(FALSE);
    projName->AllowEdit(FALSE);
    enterFile->SetData(DWORD_PTR(_T("�����ļ�")));
    projName->SetData(DWORD_PTR(_T("��Ŀ����")));
    proj->AddSubItem(enterFile);
    proj->AddSubItem(projName);
    m_wndPropList.AddProperty(proj);

	//��ĿVIP����
	CMFCPropertyGridProperty* projInfo = new CMFCPropertyGridProperty(_T("��Ŀ��Ϣ"));

	CMFCPropertyGridProperty* productName = new CMFCPropertyGridProperty(_T("��Ʒ����"),CTCExeVersionManager::m_productName, _T("��ǰ��Ŀ������Ŀ������"));
	CMFCPropertyGridProperty* fileDesCri = new CMFCPropertyGridProperty(_T("�ļ�����"),CTCExeVersionManager::m_verDesInfo, _T("��ǰ��Ŀ�����ļ����ļ�����"));
	CMFCPropertyGridProperty* fileInfo = new CMFCPropertyGridProperty(_T("�ļ��汾"),CTCExeVersionManager::m_fileInfo, _T("��ǰ��Ŀ�����ļ����ļ��汾��Ϣ"));
	CMFCPropertyGridProperty* productInfo = new CMFCPropertyGridProperty(_T("��Ʒ�汾"),CTCExeVersionManager::m_productInfo, _T("��ǰ��Ŀ�������ļ��Ĳ�Ʒ�汾��Ϣ"));
	CMFCPropertyGridProperty* internalName = new CMFCPropertyGridProperty(_T("�ڲ�����"),CTCExeVersionManager::m_internalName, _T("��ǰ��Ŀ�����ļ����ڲ�����"));
	CMFCPropertyGridProperty* companyName = new CMFCPropertyGridProperty(_T("��˾����"),CTCExeVersionManager::m_companyName, _T("��ǰ��Ŀ�����ļ��Ĺ�˾����"));
	CMFCPropertyGridProperty* legalCopyright = new CMFCPropertyGridProperty(_T("��Ȩ��Ϣ"),CTCExeVersionManager::m_leagalInfo, _T("��ǰ��Ŀ�����ļ����ļ��İ�Ȩ��Ϣ"));


	productName->AllowEdit(TRUE);
	fileDesCri->AllowEdit(TRUE);
	fileInfo->AllowEdit(TRUE);
	productInfo->AllowEdit(TRUE);
	internalName->AllowEdit(TRUE);
	companyName->AllowEdit(TRUE);
	legalCopyright->AllowEdit(TRUE);



	productName->SetData(DWORD_PTR(_T("��Ʒ����")));
	fileDesCri->SetData(DWORD_PTR(_T("�ļ�����")));
	fileInfo->SetData(DWORD_PTR(_T("�ļ��汾��")));
	productInfo->SetData(DWORD_PTR(_T("��Ʒ�汾��")));
	internalName->SetData(DWORD_PTR(_T("�ڲ�����")));
	companyName->SetData(DWORD_PTR(_T("��˾����")));
	legalCopyright->SetData(DWORD_PTR(_T("��Ȩ��Ϣ")));

	projInfo->AddSubItem(productName);
	projInfo->AddSubItem(fileDesCri);
	projInfo->AddSubItem(fileInfo);
	projInfo->AddSubItem(productInfo);
	projInfo->AddSubItem(internalName);
	projInfo->AddSubItem(companyName);
	projInfo->AddSubItem(legalCopyright);

	m_wndPropList.AddProperty(projInfo);
}

void CPropertiesWnd::SetRcNodeDiaProperty(CString name)
{
	m_CurrentType.type=TCPROPERTY_TWIN_NODE;
	m_CurrentType.name=name;
	m_wndPropList.RemoveAll();

	CMFCPropertyGridProperty* dlgBase = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* dlgName = new CMFCPropertyGridProperty(_T("<����>"), (_variant_t)_T("���ڽڵ�"), _T("��ǰ���ڽڵ�"));
	CMFCPropertyGridProperty* dlgID = new CMFCPropertyGridProperty(_T("��������"), (_variant_t)name, _T("��ǰ�ڵ��������ڵ�����"));
	dlgBase->AddSubItem(dlgName);
	dlgBase->AddSubItem(dlgID);
	dlgName->Enable(FALSE);
	m_wndPropList.AddProperty(dlgBase);
}

void CPropertiesWnd::SetTWinDiaProperty(CString name,const CDiaTCDesign * const dlg,BOOL upDate)
{
	if(m_CurrentType.type == TCPROPERTY_TWIN && m_CurrentType.pWnd == dlg->m_hWnd && upDate == FALSE)
		return ;

    m_CurrentType.type=TCPROPERTY_TWIN;
    m_CurrentType.name=name;
	m_CurrentType.pWnd=dlg->m_hWnd;
    m_wndPropList.RemoveAll();

    ASSERT(dlg!=0);

    CString csTitle;
    RECT rc;
    dlg->GetWindowTextW(csTitle);
    dlg->GetWindowRect(&rc);
    //dlg->m_TcDlgConfig.m_BkgrBitmap;
    CMFCPropertyGridProperty* dlgBase = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* dlgID = new CMFCPropertyGridProperty(_T("��������"), (_variant_t)dlg->m_TcDlgConfig.m_dlgID, _T("��ǰ���ڵ�����"));
    CMFCPropertyGridProperty* dlgTitle = new CMFCPropertyGridProperty(_T("���ڱ���"), (_variant_t)csTitle, _T("��ǰ�Ĵ��ڱ���"));
    dlgBase->AddSubItem(dlgID);
	dlgBase->AddSubItem(dlgTitle);
    m_wndPropList.AddProperty(dlgBase);

    CMFCPropertyGridProperty* dlgLook = new CMFCPropertyGridProperty(_T("���"));
    CMFCPropertyGridProperty* dlgWidth = new CMFCPropertyGridProperty(_T("���ڿ��"), (_variant_t)(rc.right-rc.left), _T("��ǰ���ڵĿ��"));
    CMFCPropertyGridProperty* dlgHeight = new CMFCPropertyGridProperty(_T("���ڸ߶�"), (_variant_t)(rc.bottom-rc.top), _T("��ǰ���ڵĸ߶�"));
	CCustomDlgProp* dlgIcon = new CCustomDlgProp(_T("����ͼ��"),(_variant_t)dlg->m_TcDlgConfig.m_icon,this);
    //dlgIcon->Enable(false);
    //CComboBoxExProp* dlgBkgrBitmap = new CComboBoxExProp(_T("���ڱ���ͼ"), (_variant_t)dlg->m_TcDlgConfig.m_BkgrBitmap, _T("��ǰ�Ĵ��ڱ���ͼ"),NULL,this);
    //dlgBkgrBitmap->AddOption(_T(""));

	CMFCPropertyGridProperty* ctlMin = new CMFCPropertyGridProperty(_T("��С��"),(_variant_t)dlg->m_TcDlgConfig.m_minBtn, _T("��ǰ���������С����ť"));
	CMFCPropertyGridProperty* ctlMax = new CMFCPropertyGridProperty(_T("���"),(_variant_t)dlg->m_TcDlgConfig.m_maxBtn, _T("��ǰ���������󻯰�ť"));


	dlgIcon->Enable(true);

    //������Դ�ļ������ͼ���б�
    CStringArray bmpArray;
    CTCRunTimeManager::getRCPathBmpList(bmpArray);
    /*int count=bmpArray.GetCount();
    for(int i=0;i<count;i++)
    {
        dlgBkgrBitmap->AddOption(bmpArray.GetAt(i));
    }*/
    CMFCPropertyGridColorProperty* dlgColorProp = new CMFCPropertyGridColorProperty(_T("���ڱ�����ɫ"), dlg->m_TcDlgConfig.m_BkgrColor, NULL, _T("ָ��Ĭ�ϵĴ��ڱ�����ɫ"));
    dlgColorProp->EnableOtherButton(_T("����..."));
    dlgColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_BTNFACE));

    dlgTitle->SetData(DWORD_PTR(_T("���ڱ���")));
    dlgWidth->SetData(DWORD_PTR(_T("���ڿ��")));
    dlgHeight->SetData(DWORD_PTR(_T("���ڸ߶�")));
    dlgIcon->SetData(DWORD_PTR(_T("����ͼ��")));
    //dlgBkgrBitmap->SetData(DWORD_PTR(_T("���ڱ���ͼ")));
    dlgColorProp->SetData(DWORD_PTR(_T("���ڱ�����ɫ")));
	ctlMin->SetData(DWORD_PTR(_T("������С����ť")));
	ctlMax->SetData(DWORD_PTR(_T("������󻯰�ť")));

    dlgLook->AddSubItem(dlgWidth);
    dlgLook->AddSubItem(dlgHeight);
    dlgLook->AddSubItem(dlgIcon);
    //dlgLook->AddSubItem(dlgBkgrBitmap);
    dlgLook->AddSubItem(dlgColorProp);
	dlgLook->AddSubItem(ctlMin);
	dlgLook->AddSubItem(ctlMax);

    m_wndPropList.AddProperty(dlgLook);

    CMFCPropertyGridProperty* dlgAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CMFCPropertyGridProperty* dlgInit = new CMFCPropertyGridProperty(_T("���ڳ�ʼ��"), (_variant_t)dlg->m_TcDlgConfig.m_eDiaInit, _T("���ڳ�ʼ����ʱ��ִ�еĲ���"));
    CMFCPropertyGridProperty* dlgDestory = new CMFCPropertyGridProperty(_T("��������"), (_variant_t)dlg->m_TcDlgConfig.m_eDestory, _T("�������ٵ�ʱ��ִ�еĲ���"));
	CMFCPropertyGridProperty* dlgCloseWindow = new CMFCPropertyGridProperty(_T("���ڹر�"), (_variant_t)dlg->m_TcDlgConfig.m_eClose, _T("���ڹرյ�ʱ��ִ�еĲ���"));
	CMFCPropertyGridProperty* dlgPreTrans = new CMFCPropertyGridProperty(_T("��Ϣ·��"), (_variant_t)dlg->m_TcDlgConfig.m_ePreTrans, _T("���ڵ���Ϣ·��"));
	CMFCPropertyGridProperty* dlgWinPorc = new CMFCPropertyGridProperty(_T("��Ϣ����"), (_variant_t)dlg->m_TcDlgConfig.m_eWinProc, _T("���ڵ���Ϣ����"));

    if(CTCRunTimeManager::g_LangType==0)
    {
        CString eDiaInit,eDestory,eClose,ePreTrans,eWinProc;
        eDiaInit.Format(_T("<���>%s.��ʼ��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
        eDestory.Format(_T("<���>%s.����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		eClose.Format(_T("<���>%s.�ر�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		ePreTrans.Format(_T("<���>%s.��Ϣ·��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		eWinProc.Format(_T("<���>%s.��Ϣ����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);

        dlgInit->AddOption(eDiaInit);
        dlgDestory->AddOption(eDestory);
		dlgCloseWindow->AddOption(eClose);
		dlgPreTrans->AddOption(ePreTrans);
		dlgWinPorc->AddOption(eWinProc);

        dlgInit->AllowEdit(false);
        dlgDestory->AllowEdit(false);
		dlgCloseWindow->AllowEdit(false);
		dlgPreTrans->AllowEdit(false);
		dlgWinPorc->AllowEdit(false);
    }
    else if(CTCRunTimeManager::g_LangType==1)
    {
        CString eDiaInit,eKeydown,eDestory,eClose,ePreTrans,eWinProc;
        eDiaInit.Format(_T("<add>%s.init"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
        eDestory.Format(_T("<add>%s.destroy"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		eClose.Format(_T("<���>%s.close"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		ePreTrans.Format(_T("<���>%s.pretranslatemessage"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		eWinProc.Format(_T("<���>%s.windowproc"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);

        dlgInit->AddOption(eDiaInit);
        dlgDestory->AddOption(eDestory);
		dlgCloseWindow->AddOption(eClose);
		dlgPreTrans->AddOption(ePreTrans);
		dlgWinPorc->AddOption(eWinProc);

        dlgInit->AllowEdit(false);
        dlgDestory->AllowEdit(false);
		dlgCloseWindow->AllowEdit(false);
		dlgPreTrans->AllowEdit(false);
		dlgWinPorc->AllowEdit(false);
    }

    dlgInit->SetData(DWORD_PTR(_T("���ڳ�ʼ��")));
    dlgDestory->SetData(DWORD_PTR(_T("��������")));
	dlgCloseWindow->SetData(DWORD_PTR(_T("���ڹر�")));
	dlgPreTrans->SetData(DWORD_PTR(_T("������Ϣ·��")));
	dlgWinPorc->SetData(DWORD_PTR(_T("������Ϣ����")));
    dlgAction->AddSubItem(dlgInit);
    dlgAction->AddSubItem(dlgDestory);
	dlgAction->AddSubItem(dlgCloseWindow);
	dlgAction->AddSubItem(dlgPreTrans);
	dlgAction->AddSubItem(dlgWinPorc);

    m_wndPropList.AddProperty(dlgAction);
}

void CPropertiesWnd::SetTWinStaticProperty(CString id, CTCStatic *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;

    m_CurrentType.type=TCPROPERTY_CTRL_STATIC;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

    CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("��ǩ������"));
    CMFCPropertyGridProperty* ctlText = new CMFCPropertyGridProperty(_T("�ı�"), (_variant_t)csText, _T("��ǩ��ʾ���ı�����"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(ctlText);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("���"));
    CMFCPropertyGridColorProperty* cltBKColorProp = new CMFCPropertyGridColorProperty(_T("��ǩ������ɫ"), ctl->GetBackground(), NULL, _T("ָ��Ĭ�ϵı�ǩ������ɫ"));
    cltBKColorProp->EnableOtherButton(_T("����..."));
    cltBKColorProp->EnableAutomaticButton(_T("Ĭ��"),::GetSysColor(COLOR_BTNFACE));

    //CString csDir;
    //ctl->GetStyle();
    //CMFCPropertyGridProperty* dlgDir = new CMFCPropertyGridProperty(_T("����"), (_variant_t)csText, _T("��ǩ�ı�����ķ���"));
    ctlLook->AddSubItem(cltBKColorProp);
    m_wndPropList.AddProperty(ctlLook);

    CMFCPropertyGridProperty* pGroupFont = new CMFCPropertyGridProperty(_T("����"));

    LOGFONT lf;
    CFont* font = ctl->GetFont();//CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    font->GetLogFont(&lf);

	CString faceName = ctl->GetFontName();
	

    lstrcpy(lf.lfFaceName, faceName/*_T("����, Arial")*/);
    pGroupFont->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ����ǩ��Ĭ������")));
    pGroupFont->AddSubItem(new CMFCPropertyGridProperty(_T("ʹ��ϵͳ����"), (_variant_t) true, _T("ָ����ǩʹ�á�MS Shell Dlg������")));
    m_wndPropList.AddProperty(pGroupFont);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("��ǩ��X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("��ǩ��Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("��ǩ�Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("��ǩ�ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* dlgClick = new CComboBoxExProp(_T("���"), (_variant_t)ctl->TCGetClickEvent(), _T("��ǩ���ʱ�����Ĺ���"),0,this);
    
	dlgClick->AddOption(_T(""));
    dlgClick->AllowEdit(false);
	 //if(CTCRunTimeManager::g_LangType==0)
  //  {
  //      CString eClick;
  //      eClick.Format(_T("<���>%s.���"),CTCRunTimeManager::g_ProjName);
  //      dlgClick->AddOption(eClick);
  //      dlgClick->AllowEdit(false);
  //  }
  //  else if(CTCRunTimeManager::g_LangType==1)
  //  {
  //      CString eClick;
  //      eClick.Format(_T("<���>%s.click"),CTCRunTimeManager::g_ProjName);
  //      dlgClick->AddOption(eClick);
  //      dlgClick->AllowEdit(false);
  //  }

  //  dlgClick->SetData(DWORD_PTR(_T("���")));

    ctlAction->AddSubItem(dlgClick);
    m_wndPropList.AddProperty(ctlAction);

}

void CPropertiesWnd::SetTWinGroupProperty(CString id, CTCGroupBox *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
    m_CurrentType.type=TCPROPERTY_CTRL_GROUP;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

    CString csText;
    RECT rc;
    ctl->GetText(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("����������"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("����"), (_variant_t)csText, _T("�������ʾ�ı�������"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("���"));

    CTCGroupBox::TCGroupBoxStyle style;
    CString csStyle;
    style=ctl->GetGroupBoxStyle();
    if(style==CTCGroupBox::TCGB_FRAME)
        csStyle=_T("�߿���");
    else if(style==CTCGroupBox::TCGB_WINDOW)
        csStyle=_T("���ڷ��");
    else
        csStyle=_T("�߿���");

    CMFCPropertyGridProperty* groupBoxStyle = new CMFCPropertyGridProperty(_T("���ڷ��"), (_variant_t)csStyle, _T("�������ʾ�ķ��"));
    groupBoxStyle->AddOption(_T("�߿���"));
    groupBoxStyle->AddOption(_T("���ڷ��"));
    groupBoxStyle->AllowEdit(false);

    //   CMFCPropertyGridColorProperty* titleTextColorProp = new CMFCPropertyGridColorProperty(_T("�����ı���ɫ"), ctl->GetTitleTextColor(), NULL, _T("ָ��Ĭ�ϵı�ǩ�ı���ɫ"));
    //titleTextColorProp->EnableOtherButton(_T("����..."));
    //   titleTextColorProp->EnableAutomaticButton(_T("Ĭ��"),ctl->GetTitleTextColor());

    CMFCPropertyGridColorProperty* titleBKColorProp = new CMFCPropertyGridColorProperty(_T("���ⱳ����ɫ"), ctl->GetTitleBackgroundColor(), NULL, _T("ָ��Ĭ�ϵķ������ⱳ����ɫ"));
    titleBKColorProp->EnableOtherButton(_T("����..."));
    titleBKColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_BTNFACE)/*ctl->GetTitleBackgroundColor()*/);

    CMFCPropertyGridColorProperty* borderColorProp = new CMFCPropertyGridColorProperty(_T("�߿���ɫ"), ctl->GetBorderColor(), NULL, _T("ָ��Ĭ�ϵı߿���ɫ"));
    borderColorProp->EnableOtherButton(_T("����..."));
    borderColorProp->EnableAutomaticButton(_T("Ĭ��"),::GetSysColor(COLOR_3DSHADOW)/*ctl->GetBorderColor()*/);

    CMFCPropertyGridColorProperty* clientBKColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), ctl->GetClientBackgroundColor(), NULL, _T("ָ��Ĭ�ϵķ���򱳾���ɫ"));
    clientBKColorProp->EnableOtherButton(_T("����..."));
    clientBKColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_BTNFACE));

    ctlLook->AddSubItem(groupBoxStyle);
    //ctlLook->AddSubItem(titleTextColorProp);
    ctlLook->AddSubItem(titleBKColorProp);
    ctlLook->AddSubItem(borderColorProp);
    ctlLook->AddSubItem(clientBKColorProp);
    m_wndPropList.AddProperty(ctlLook);

    CMFCPropertyGridProperty* pGroupFont = new CMFCPropertyGridProperty(_T("����"));

	LOGFONT lf = ctl->GetGroupFont();
	//CFont* font = ctl->GetFont();/*CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));*/
 //   font->GetLogFont(&lf);

	CString fontName;
	ctl->GetFontFaceName(fontName);
	lstrcpy(lf.lfFaceName,fontName/* _T("����, Arial")*/);
    pGroupFont->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ���ؼ���Ĭ������")));
    pGroupFont->AddSubItem(new CMFCPropertyGridProperty(_T("ʹ��ϵͳ����"), (_variant_t) true, _T("ָ���ؼ�ʹ�á�MS Shell Dlg������")));
    m_wndPropList.AddProperty(pGroupFont);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("������X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("������Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("�����Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("�����ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    DWORD dir;
    CString csDir;
    dir=ctl->GetAlignment();

    if(dir==SS_LEFT)
        csDir=_T("�����");
    else if(dir==SS_CENTER)
        csDir=_T("�м����");
    else if(dir==SS_RIGHT)
        csDir=_T("�Ҷ���");
    else 
        csDir=_T("Ĭ��");

    CMFCPropertyGridProperty* ctlDir = new CMFCPropertyGridProperty(_T("����"), (_variant_t)csDir, _T("������ı�����ķ���"));
    ctlDir->AddOption(_T("�����"));
    ctlDir->AddOption(_T("�м����"));
    ctlDir->AddOption(_T("�Ҷ���"));
    ctlPostion->AddSubItem(ctlDir);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);
}

void  CPropertiesWnd::SetTWinButtonProperty(CString id, CTCButton *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
    m_CurrentType.type=TCPROPERTY_CTRL_BUTTON;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

    CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("��ť������"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("�ı�"), (_variant_t)csText, _T("��ť��ʾ�ı�������"));
	CMFCPropertyGridProperty* defButton = new CMFCPropertyGridProperty(_T("Ĭ�ϰ�ť"),(_variant_t)ctl->TCGetDefButton(), _T("��ǰ��ť�Ƿ�Ĭ�ϰ�ť"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(defButton);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("��ť��X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("��ť��Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("��ť�Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("��ť�ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	
	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* mulityLine = new CMFCPropertyGridProperty(_T("����"), (_variant_t)ctl->TCGetMulityLine(), _T("��ť��ʾ�ı������ݶ���"));
	ctlOther->AddSubItem(mulityLine);
	m_wndPropList.AddProperty(ctlOther);


    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* ctlClick = new CComboBoxExProp(_T("���"), (_variant_t)ctl->TCGetClickEvent(), _T("��ť�ĵ��ʱ�����Ĺ���"),0,this);
    ctlClick->AddOption(_T(""));
	ctlClick->AllowEdit(FALSE);
    ctlAction->AddSubItem(ctlClick);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinEditProperty(CString id, CTCEdit *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;

    m_CurrentType.type=TCPROPERTY_CTRL_EDIT;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

    CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("�༭�������"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("�ı�"), (_variant_t)csText, _T("�༭����ʾ������"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("���"));

    CMFCPropertyGridColorProperty* cltTextColorProp = new CMFCPropertyGridColorProperty(_T("�ı���ɫ"), ctl->GetTextColor(), NULL, _T("ָ��Ĭ�ϵı༭���ı���ɫ"));
    cltTextColorProp->EnableOtherButton(_T("����..."));
    cltTextColorProp->EnableAutomaticButton(_T("Ĭ��"),::GetSysColor(COLOR_WINDOWTEXT));

    CMFCPropertyGridColorProperty* cltBKColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), ctl->GetBackColor(), NULL, _T("ָ��Ĭ�ϵ��ı��򱳾���ɫ"));
    cltBKColorProp->EnableOtherButton(_T("����..."));
    cltBKColorProp->EnableAutomaticButton(_T("Ĭ��"),::GetSysColor(COLOR_WINDOW));

    ctlLook->AddSubItem(cltTextColorProp);
    ctlLook->AddSubItem(cltBKColorProp);
    m_wndPropList.AddProperty(ctlLook);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("�༭���X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("�༭���Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("�༭��Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("�༭��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	 ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

    CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* ctlMulLine = new CMFCPropertyGridProperty(_T("����"), (_variant_t)ctl->TCGetMulityLine(), _T("�༭��֧�ֶ�����������"));
    CMFCPropertyGridProperty* ctlCheck = new CMFCPropertyGridProperty(_T("����"), (_variant_t)ctl->TCGetPassWord(), _T("�༭����������"));
	CMFCPropertyGridProperty* ctlReadOnly = new CMFCPropertyGridProperty(_T("ֻ��"), (_variant_t)ctl->TCGetReadOnly(), _T("�༭��ֻ������"));
	CMFCPropertyGridProperty* ctlNumber = new CMFCPropertyGridProperty(_T("����"), (_variant_t)ctl->TCGetNumber(), _T("�༭��������������"));
	CMFCPropertyGridProperty* ctlVerti = new CMFCPropertyGridProperty(_T("��ֱ������"), (_variant_t)ctl->TCGetVerti(), _T("�༭����Ӵ�ֱ������"));
	CMFCPropertyGridProperty* ctlHoriz = new CMFCPropertyGridProperty(_T("ˮƽ������"), (_variant_t)ctl->TCGetHoriz(), _T("�༭�����ˮƽ������"));
	ctlVerti->SetData(DWORD_PTR(_T("�༭��ֱ������")));
	ctlHoriz->SetData(DWORD_PTR(_T("�༭��ˮƽ������")));
	ctlOther->AddSubItem(ctlCheck);
    ctlOther->AddSubItem(ctlReadOnly);
	ctlOther->AddSubItem(ctlNumber);
	ctlOther->AddSubItem(ctlMulLine);
	ctlOther->AddSubItem(ctlVerti);
	ctlOther->AddSubItem(ctlHoriz);
    m_wndPropList.AddProperty(ctlOther);

	if(ctl->TCGetMulityLine())
	{
		ctlVerti->Enable(TRUE);
		ctlHoriz->Enable(TRUE);
	}
	else
	{
		ctlVerti->Enable(FALSE);
		ctlHoriz->Enable(FALSE);
	}


    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* ctlChange = new CComboBoxExProp(_T("���ݸı�"), (_variant_t)ctl->TCGetEnChangeEvent(), _T("�༭�����ݸı�ʱ�����Ĺ���"),0,this);
	CComboBoxExProp* ctlSetFocus = new CComboBoxExProp(_T("��ý���"), (_variant_t)ctl->TCGetSetFocusEvent(), _T("�༭���ý���ʱ�����Ĺ���"),0,this);
    CComboBoxExProp* ctlKF = new CComboBoxExProp(_T("ʧȥ����"), (_variant_t)ctl->TCGetKillfocusEvent(), _T("�༭��ʧȥ����ʱ�����Ĺ���"),0,this);
    ctlChange->AddOption(_T(""));
	ctlChange->AllowEdit(FALSE);
	ctlSetFocus->AddOption(_T(""));
	ctlSetFocus->AllowEdit(FALSE);
    ctlKF->AddOption(_T(""));
	ctlKF->AllowEdit(FALSE);
    ctlAction->AddSubItem(ctlChange);
	ctlAction->AddSubItem(ctlSetFocus);
    ctlAction->AddSubItem(ctlKF);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinRadioProperty(CString id, CTCRadio *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
    m_CurrentType.type=TCPROPERTY_CTRL_RADIO;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

    CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("��ѡ�������"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("����"), (_variant_t)csText, _T("��ѡ�������ʾ������"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
	CMFCPropertyGridProperty* groupCtrl = new CMFCPropertyGridProperty(_T("����"), (_variant_t)ctl->TCGetGroup(), _T("��ѡ�����"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(ableCtrl);
	ctlBase->AddSubItem(groupCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("��ѡ���X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("��ѡ���Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("��ѡ��Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("��ѡ��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);


    CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("����"));
    bool isCheck = ctl->TCGetCheck();
	bool isLeft = ctl->TCGetTextLeft();
    CMFCPropertyGridProperty* ctlCheck = new CMFCPropertyGridProperty(_T("ѡ��"), (_variant_t)isCheck, _T(""));
	CMFCPropertyGridProperty* leftText = new CMFCPropertyGridProperty(_T("�������"), (_variant_t)isLeft, _T("��ѡ�������ʾ�����Ƿ����"));
    ctlOther->AddSubItem(ctlCheck);
	ctlOther->AddSubItem(leftText);
    m_wndPropList.AddProperty(ctlOther);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* selChange = new CComboBoxExProp(_T("���"), (_variant_t)ctl->TCGetRadioEvent(), _T("��ѡ�򵥻�ʱ�����Ĺ���"),0,this);
    selChange->AddOption(_T(""));
	selChange->AllowEdit(FALSE);
    ctlAction->AddSubItem(selChange);
    m_wndPropList.AddProperty(ctlAction);

}

void CPropertiesWnd::SetTWinCheckProperty(CString id, CTCCheck *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
    m_CurrentType.type=TCPROPERTY_CTRL_CHECK;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

    CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("��ѡ�������"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("����"), (_variant_t)csText, _T("��ѡ�������ʾ������"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("��ѡ���X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("��ѡ��Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("��ѡ��Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("��ѡ��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

    CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("����"));
    bool isCheck = ctl->TCGetCheck();
	bool isLeft = ctl->TCGetTextLeft();
    CMFCPropertyGridProperty* ctlCheck = new CMFCPropertyGridProperty(_T("ѡ��"),(_variant_t)isCheck, _T("��ѡ���Ƿ�Ĭ��ѡ��"));
	CMFCPropertyGridProperty* leftText = new CMFCPropertyGridProperty(_T("�������"),(_variant_t)isLeft,_T("��ѡ�������ʾ�����Ƿ����"));
    ctlOther->AddSubItem(ctlCheck);	
	ctlOther->AddSubItem(leftText);
    m_wndPropList.AddProperty(ctlOther);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* selChange = new CComboBoxExProp(_T("���"), (_variant_t)ctl->TCGetCheckEvent(), _T("��ѡ�򵥻�ʱ�����Ĺ���"),0,this);
    selChange->AddOption(_T(""));
	selChange->AllowEdit(FALSE);
    ctlAction->AddSubItem(selChange);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinPictureProperty(CString id, CTCPicture *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_PIC;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("ͼƬ�������"));
	CComboBoxExProp* ctlPath = new CComboBoxExProp(_T("ͼƬ����"), (_variant_t)ctl->GetName(), _T("ͼƬ��Ҫ��ʾͼƬ������"),0,this);
	ctlBase->AddSubItem(ctlID);
	ctlPath->AddOption(_T(""));
	ctlBase->AddSubItem(ctlPath);
    m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("ͼƬ���X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("ͼƬ���Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("ͼƬ��Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("ͼƬ��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
	CComboBoxExProp* click = new CComboBoxExProp(_T("�������"), (_variant_t)ctl->TCGetClickEvent(), _T("�������ͼƬ��ʱ�����Ĺ���"),0,this);
	click->AddOption(_T(""));
	click->AllowEdit(FALSE);
	ctlAction->AddSubItem(click);
	m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinComboxProperty(CString id, CTCComboBox *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
    m_CurrentType.type=TCPROPERTY_CTRL_COMBOX;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

    CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);


    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("�����������"));
    CCustomDlgProp* addItem = new CCustomDlgProp(_T("ѡ��"),(_variant_t)(_T("<�༭����ѡ��>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(addItem);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("�������X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("�������Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("������Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("������ĸ߶�"));
	//������ĸ߶���һ����
	ctlHeight->AllowEdit(false);
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);


    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* click = new CComboBoxExProp(_T("�������"), (_variant_t)ctl->GetLeftDownEvent(), _T("�������������ʱ�����Ĺ���"),0,this);
    CComboBoxExProp* selChange = new CComboBoxExProp(_T("ѡ��ı�"), (_variant_t)ctl->GetEnChangeEvent(), _T("������ѡ��ı�ʱ�����Ĺ���"),0,this);
    click->AddOption(_T(""));
	click->AllowEdit(FALSE);
    selChange->AddOption(_T(""));
	selChange->AllowEdit(FALSE);
    ctlAction->AddSubItem(click);
    ctlAction->AddSubItem(selChange);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinProgressProperty(CString id, CTCProgress *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_PROGRESS;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("������������"));
	ctlBase->AddSubItem(ctlID);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("��������X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("��������Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("�������Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("�������ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* vertical = new CMFCPropertyGridProperty(_T("��ֱ"), (_variant_t)ctl->TCGetVertical(), _T("�������ķ����Ǵ�ֱ����ˮƽ"));
	ctlOther->AddSubItem(vertical);
	m_wndPropList.AddProperty(ctlOther);
}

void CPropertiesWnd::SetTWinPathProperty(CString id, CTCBrowse *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_PATH;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("·���������"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
	CMFCEditBrowseCtrl::BrowseMode m_browmode = ctl->GetMode();
	CString strMode = _T("");
	if( m_browmode == CMFCEditBrowseCtrl::BrowseMode_File)
		strMode = _T("�ļ�");
	else if( m_browmode == CMFCEditBrowseCtrl::BrowseMode_Folder )
		strMode = _T("�ļ���");
	CMFCPropertyGridProperty* PahtStyle = new CMFCPropertyGridProperty(_T("���ģʽ"), (_variant_t)strMode, _T("·���������ģʽ"));
    PahtStyle->AddOption(_T("�ļ�"));
    PahtStyle->AddOption(_T("�ļ���"));
	//�������û����룬ֻ��ѡ��
	PahtStyle->AllowEdit(FALSE);
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(PahtStyle);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("·�����X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("·�����Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("·����Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("·����ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* enChange = new CComboBoxExProp(_T("ѡ��ı�"), (_variant_t)ctl->TCGetEnChangeEvent(), _T("·����ѡ��ı�ʱ�����Ĺ���"),0,this);
	enChange->AddOption(_T(""));
	enChange->AllowEdit(FALSE);
    ctlAction->AddSubItem(enChange);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinSliderProperty(CString id, CTCSlider *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_SLIDER;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("������������"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("��������X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("��������Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("�������Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("�������ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* vertical = new CMFCPropertyGridProperty(_T("��ֱ"), (_variant_t)ctl->TCGetVertical(), _T("�������ķ����Ǵ�ֱ����ˮƽ"));
	ctlOther->AddSubItem(vertical);
	m_wndPropList.AddProperty(ctlOther);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
    CComboBoxExProp* enChange = new CComboBoxExProp(_T("����ͷ�"), (_variant_t)ctl->TCGetSliderChangeEvent(), _T("����������ͷ�ʱ�����Ĺ���"),0,this);
	enChange->AddOption(_T(""));
	enChange->AllowEdit(FALSE);
    ctlAction->AddSubItem(enChange);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinDateProperty(CString id, CTCDate *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_DATE;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("���ڿ������"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("���ڿ��X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("���ڿ��Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("���ڿ�Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("���ڿ�ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);
}

void CPropertiesWnd::SetTWinListProperty(CString id, CTCListBox *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_LIST;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    csText=ctl->TCGetListBoxCaption();
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);
	
	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("�б�������"));
	CMFCPropertyGridProperty* ctlTitle = new CMFCPropertyGridProperty(_T("����"), (_variant_t)csText, _T("�б��ı���"));
	CCustomDlgProp* addItem = new CCustomDlgProp(_T("ѡ��"),(_variant_t)(_T("<�༭����ѡ��>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
	/*CMFCPropertyGridProperty* buttonNew = new CMFCPropertyGridProperty(_T("�½�"), (_variant_t)isNew, _T("�Ƿ�����½���ť"));
	CMFCPropertyGridProperty* buttonRemove = new CMFCPropertyGridProperty(_T("ɾ��"), (_variant_t)isRemove, _T("�Ƿ����ɾ����ť"));
	CMFCPropertyGridProperty* buttonUp = new CMFCPropertyGridProperty(_T("����"), (_variant_t)isUp, _T("�Ƿ�������ư�ť"));
	CMFCPropertyGridProperty* buttonDown = new CMFCPropertyGridProperty(_T("����"), (_variant_t)isDown, _T("�Ƿ�������ư�ť"));*/
    ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlTitle);
	ctlBase->AddSubItem(addItem);
	ctlBase->AddSubItem(ableCtrl);
	/*ctlBase->AddSubItem(buttonNew);
	ctlBase->AddSubItem(buttonRemove);
	ctlBase->AddSubItem(buttonUp);
	ctlBase->AddSubItem(buttonDown);*/
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("�б���X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("�б���Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("�б��Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("�б��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* defaultRoll = new CMFCPropertyGridProperty(_T("Ĭ�Ϲ���"), (_variant_t)ctl->TCGetDefaultRool(), _T("�������ʱĬ�Ϲ���"));
	ctlOther->AddSubItem(defaultRoll);
	m_wndPropList.AddProperty(ctlOther);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
	CComboBoxExProp* selChange = new CComboBoxExProp(_T("ѡ��ı�"), (_variant_t)ctl->TCGetListBoxSelChange(), _T("�б��ѡ��ı�ʱ�����Ĺ���"),0,this);
	CComboBoxExProp* clicked = new CComboBoxExProp(_T("���˫��"), (_variant_t)ctl->TCGetListBoxDoubleClick(), _T("�б�����˫����ʱ�����Ĺ���"),0,this);
	clicked->AddOption(_T(""));
	selChange->AddOption(_T(""));
	clicked->AllowEdit(FALSE);
	selChange->AllowEdit(FALSE);
	ctlAction->AddSubItem(clicked);
    ctlAction->AddSubItem(selChange);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinTableProperty(CString id, CTCTab*  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd == ctl->m_hWnd && upDate == FALSE)
		return;

	m_CurrentType.type=TCPROPERTY_CTRL_TABLE;
	m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
	m_wndPropList.RemoveAll();

	CString csText;
	RECT rc;
	ctl->GetWindowTextW(csText);
	ctl->GetWindowRect(&rc);
	ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("ѡ��ؼ�������"));
	CCustomDlgProp* tabItem = new CCustomDlgProp(_T("�༭ѡ��"),(_variant_t)(_T("<�༭��ҳ����>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(tabItem);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
	CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("ѡ��ؼ���X����"));
	CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("ѡ��ؼ���Y����"));
	CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("ѡ��ؼ��Ŀ��"));
	CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("ѡ��ؼ��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

	ctlPostion->AddSubItem(ctlLeft);
	ctlPostion->AddSubItem(ctlTop);
	ctlPostion->AddSubItem(ctlWidth);
	ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
	CComboBoxExProp* clock = new CComboBoxExProp(_T("ѡ��ı�"), (_variant_t)ctl->TCGetSelChange(), _T("ѡ��ı�ʱ�������¼�"),0,this);
    clock->AddOption(_T(""));
	clock->AllowEdit(FALSE);
    ctlAction->AddSubItem(clock);
    m_wndPropList.AddProperty(ctlAction);

}

void CPropertiesWnd::SetTWinTimerProperty(CString id, CTCTimer*  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_TIMER;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("��ʱ��������"));
	CMFCPropertyGridProperty* ctlTimer = new CMFCPropertyGridProperty(_T("Ƶ��"), (_variant_t)ctl->GetTimer(), _T("��ʱ����Ƶ��(����),��ô���һ���¼�"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlTimer);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("��ʱ����X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("��ʱ����Y����"));

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);

    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
	CComboBoxExProp* clock = new CComboBoxExProp(_T("ʱ��"), (_variant_t)ctl->TCGetTimeEvent(), _T("��ʱ�������¼�"),0,this);
    clock->AddOption(_T(""));
	clock->AllowEdit(FALSE);
    ctlAction->AddSubItem(clock);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinHotKeyProperty(CString id, CTCHotKeyCtrl *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_HOTKEY;
    m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
    m_wndPropList.RemoveAll();

	CString csText;
    RECT rc;
    ctl->GetWindowTextW(csText);
    ctl->GetWindowRect(&rc);
    ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("�ȼ��ؼ�������"));
	CCustomDlgProp* hotKeyItem = new CCustomDlgProp(_T("�ȼ���ֵ"),(_variant_t)(_T("<ָ���ȼ���ֵ>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("��Ч"), (_variant_t)ctl->TCGetAbled(), _T("��ǰ�Ƿ���Ч"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(hotKeyItem);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);


	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("�ȼ��ؼ���X����"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("�ȼ��ؼ���Y����"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("�ȼ��ؼ��Ŀ��"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("�ȼ��ؼ��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
	CComboBoxExProp* clock = new CComboBoxExProp(_T("�ȼ�"), (_variant_t)ctl->TCGetHotKeyEvent(), _T("�ȼ��ؼ������Ĺ���"),0,this);
	CComboBoxExProp* change = new CComboBoxExProp(_T("ʧȥ����"), (_variant_t)ctl->TCGetKillFocusEvent(), _T("�ȼ��ؼ�ʧȥ����ʱ�����Ĺ���"),0,this);
    clock->AddOption(_T(""));
	clock->AllowEdit(FALSE);
	change->AddOption(_T(""));
	change->AllowEdit(FALSE);
    ctlAction->AddSubItem(clock);
	ctlAction->AddSubItem(change);
    m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinExplorerProperty(CString id, CTCExplorer *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_EXPLORER;
	m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
	m_wndPropList.RemoveAll();

	RECT rc;
	CString csText=ctl->TCGetURL();
	ctl->GetWindowRect(&rc);
	ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("������ؼ�������"));
	CMFCPropertyGridProperty* ctlURL = new CMFCPropertyGridProperty(_T("URL"), (_variant_t)csText, _T("������ؼ���Ĭ����ַ"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlURL);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
	CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("������ؼ���X����"));
	CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("������ؼ���Y����"));
	CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("������ؼ��Ŀ��"));
	CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("������ؼ��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	//ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

	ctlPostion->AddSubItem(ctlLeft);
	ctlPostion->AddSubItem(ctlTop);
	ctlPostion->AddSubItem(ctlWidth);
	ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
	CComboBoxExProp* loaded = new CComboBoxExProp(_T("�������"), (_variant_t)ctl->TCGetloadComplete(), _T("������ؼ�������ɲ����Ĺ���"),0,this);
	loaded->AddOption(_T(""));
	loaded->AllowEdit(FALSE);
	ctlAction->AddSubItem(loaded);
	m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::SetTWinGridCtrlProperty(CString id, CGridCtrl *  ctl,BOOL upDate)
{
	if(m_CurrentType.pWnd==ctl->m_hWnd && upDate == FALSE)
		return;
	m_CurrentType.type=TCPROPERTY_CTRL_GRIDCTRL;
	m_CurrentType.name=id;
	m_CurrentType.pWnd=ctl->m_hWnd;
	SetPropComboxCursel(id);
	m_wndPropList.RemoveAll();

	RECT rc;
	ctl->GetWindowRect(&rc);
	ctl->GetParent()->ScreenToClient(&rc);

	TCGETZORDERINFO order;
	TCGetZOrderInfo(order,ctl);

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("����"), (_variant_t)id, _T("���ؼ�������"));
	CCustomDlgProp* ctlEdit = new CCustomDlgProp(_T("�༭���"),(_variant_t)(_T("<�༭���ѡ��>")),this);
	ctlEdit->SetData(DWORD_PTR(_T("�༭���")));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlEdit);
	m_wndPropList.AddProperty(ctlBase);

	/*CMFCPropertyGridProperty* ctlMode = new CMFCPropertyGridProperty(_T("ģʽ"));
	CMFCPropertyGridProperty* cltVirualMode = new CMFCPropertyGridProperty(_T("����ģʽ"),(_variant_t)(ctl->GetVirtualMode()? true : false), _T("���ñ��ؼ�Ϊ����ģʽ(��������ģʽ��ǰģʽ�����Իᶪʧ)"));
	ctlMode->AddSubItem(cltVirualMode);
	m_wndPropList.AddProperty(ctlMode);*/

	CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("���"));
	CMFCPropertyGridColorProperty* cltBKColorProp = new CMFCPropertyGridColorProperty(_T("��ͨ�����ɫ"), ctl->GetDefaultCell(FALSE,FALSE)->GetBackClr(), NULL, _T("ָ��Ĭ�ϵı�����ɫ"));
	CMFCPropertyGridColorProperty* cltTextColorProp = new CMFCPropertyGridColorProperty(_T("��ͨ���ı���ɫ"), ctl->GetDefaultCell(FALSE,FALSE)->GetTextClr(), NULL, _T("ָ��Ĭ�ϵ��ı���ɫ"));
	CMFCPropertyGridColorProperty* cltFixedBKColorProp = new CMFCPropertyGridColorProperty(_T("�̶������ɫ"), ctl->GetFixedBkColor(), NULL, _T("ָ���̶���Ĭ�ϵı�����ɫ"));
	CMFCPropertyGridColorProperty* cltFixedTextColorProp = new CMFCPropertyGridColorProperty(_T("�̶����ı���ɫ"), ctl->GetFixedTextColor(), NULL, _T("ָ���̶���Ĭ�ϵ��ı���ɫ"));

	CMFCPropertyGridProperty* ctlVerti = new CMFCPropertyGridProperty(_T("��ֱ����"),(_variant_t)ctl->IsShowVertiLines(), _T("�����ʾ��ֱ����"));
	CMFCPropertyGridProperty* ctlHoriz = new CMFCPropertyGridProperty(_T("ˮƽ����"),(_variant_t)ctl->IsShowHorizLines(), _T("�����ʾˮƽ����"));
	
	cltBKColorProp->EnableOtherButton(_T("����..."));
	cltTextColorProp->EnableOtherButton(_T("����..."));
	cltFixedBKColorProp->EnableOtherButton(_T("����..."));
	cltFixedTextColorProp->EnableOtherButton(_T("����..."));

	cltBKColorProp->EnableAutomaticButton(_T("Ĭ��"),RGB(0xFF,0xFF,0xE0));
	cltTextColorProp->EnableAutomaticButton(_T("Ĭ��"),::GetSysColor(COLOR_WINDOWTEXT)/*ctl->GetDefaultCell(FALSE,FALSE)->GetTextClr()*/);
	cltFixedBKColorProp->EnableAutomaticButton(_T("Ĭ��"),::GetSysColor(COLOR_BTNFACE)/*ctl->GetFixedBkColor()*/);
	cltFixedTextColorProp->EnableAutomaticButton(_T("Ĭ��"),::GetSysColor(COLOR_WINDOWTEXT));

	ctlLook->AddSubItem(cltBKColorProp);
	ctlLook->AddSubItem(cltTextColorProp);
	ctlLook->AddSubItem(cltFixedBKColorProp);
	ctlLook->AddSubItem(cltFixedTextColorProp);
	ctlLook->AddSubItem(ctlVerti);
	ctlLook->AddSubItem(ctlHoriz);
	m_wndPropList.AddProperty(ctlLook);

	CMFCPropertyGridProperty* pGroupFont = new CMFCPropertyGridProperty(_T("����"));

	LOGFONT lf;
	CFont* font = ctl->GetFont();//CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	pGroupFont->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ����ǩ��Ĭ������")));
	m_wndPropList.AddProperty(pGroupFont);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("λ��"));
	CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X����"), (_variant_t)(rc.left), _T("���ؼ���X����"));
	CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y����"), (_variant_t)(rc.top), _T("���ؼ���Y����"));
	CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("���"), (_variant_t)(rc.right-rc.left), _T("���ؼ��Ŀ��"));
	CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t)(rc.bottom-rc.top), _T("���ؼ��ĸ߶�"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("��ʾ˳��"),(_variant_t)order.InControlAllPos, _T("��ǰ�ؼ�����ʾ˳��"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

	ctlPostion->AddSubItem(ctlLeft);
	ctlPostion->AddSubItem(ctlTop);
	ctlPostion->AddSubItem(ctlWidth);
	ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOthers = new CMFCPropertyGridProperty(_T("����"));
	CMFCPropertyGridProperty* ctlEditAble = new CMFCPropertyGridProperty(_T("�ɱ༭"),(_variant_t)(ctl->TCGetAllSellsEditAbled()? true : false), _T("����Ƿ���Ա༭"));
	CMFCPropertyGridProperty* ctlHeadSort = new CMFCPropertyGridProperty(_T("����"),(_variant_t)(ctl->GetHeaderSort()? true : false), _T("����Ƿ���Ա༭"));
	CMFCPropertyGridProperty* ctlFillSell = new CMFCPropertyGridProperty(_T("���"),(_variant_t)(ctl->TCGetExpandToFit()? true : false), _T("����Ƿ����"));
	CMFCPropertyGridProperty* rowCount = new CMFCPropertyGridProperty(_T("����"),(_variant_t)ctl->GetRowCount(), _T("���ؼ�������"));
	CMFCPropertyGridProperty* colCount = new CMFCPropertyGridProperty(_T("����"),(_variant_t)ctl->GetColumnCount(), _T("���ؼ�������"));
	CMFCPropertyGridProperty* fixedRowCount = new CMFCPropertyGridProperty(_T("�̶�����"),(_variant_t)ctl->GetFixedRowCount(), _T("���ؼ��Ĺ̶�����"));
	CMFCPropertyGridProperty* fixedColCount = new CMFCPropertyGridProperty(_T("�̶�����"),(_variant_t)ctl->GetFixedColumnCount(), _T("���ؼ��Ĺ̶�����"));
	CMFCPropertyGridProperty* ctrlSelectAble = new CMFCPropertyGridProperty(_T("ѡ��"),(_variant_t)(ctl->IsSelectable()? true : false), _T("������Ƿ�֧��ѡ��"));
	CMFCPropertyGridProperty* ctrlFixRowSelect = new CMFCPropertyGridProperty(_T("�̶���ѡ��"),(_variant_t)(ctl->GetFixedRowSelection()? true : false),_T("��������̶����Ƿ�֧��ѡ��"),DWORD_PTR(_T("�̶���ѡ��")));
	CMFCPropertyGridProperty* ctrlFixColSelect = new CMFCPropertyGridProperty(_T("�̶���ѡ��"),(_variant_t)(ctl->GetFixedColumnSelection()? true : false), _T("��������̶����Ƿ�֧��ѡ��"),DWORD_PTR(_T("�̶���ѡ��")));
	CMFCPropertyGridProperty* ctrlListMode = new CMFCPropertyGridProperty(_T("�б�ģʽ"),(_variant_t)(ctl->GetListMode()? true : false), _T("�����֧���б�ģʽѡ��"),DWORD_PTR(_T("�б�ģʽѡ��")));
	
	if(ctl->IsSelectable())
	{
		ctrlFixRowSelect->Enable(TRUE);
		ctrlFixColSelect->Enable(TRUE);
		ctrlListMode->Enable(TRUE);
	}
	else
	{
		ctrlFixRowSelect->Enable(FALSE);
		ctrlFixColSelect->Enable(FALSE);
		ctrlListMode->Enable(FALSE);
	}

	if(ctl->GetListMode())
		ctrlFixRowSelect->Enable(FALSE);
	else
		ctrlFixRowSelect->Enable(TRUE);

	ctlEditAble->Enable(FALSE);

	rowCount->EnableSpinControl(TRUE,0,9999);
	colCount->EnableSpinControl(TRUE,0,9999);
	fixedRowCount->EnableSpinControl(TRUE,0,9999);
	fixedColCount->EnableSpinControl(TRUE,0,9999);

	ctlOthers->AddSubItem(ctlEditAble);
	ctlOthers->AddSubItem(ctlHeadSort);
	ctlOthers->AddSubItem(ctlFillSell);
	ctlOthers->AddSubItem(rowCount);
	ctlOthers->AddSubItem(colCount);
	ctlOthers->AddSubItem(fixedRowCount);
	ctlOthers->AddSubItem(fixedColCount);
	ctlOthers->AddSubItem(ctrlSelectAble);
	ctlOthers->AddSubItem(ctrlFixRowSelect);
	ctlOthers->AddSubItem(ctrlFixColSelect);
	ctlOthers->AddSubItem(ctrlListMode);
	m_wndPropList.AddProperty(ctlOthers);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("�¼�"));
	CComboBoxExProp* leftClicked = new CComboBoxExProp(_T("�������"), (_variant_t)ctl->TCGetClickEvent(), _T("���ؼ�������������Ĺ���"),0,this);
	CComboBoxExProp* beforeEdit = new CComboBoxExProp(_T("�༭��ʼ"), (_variant_t)ctl->TCGetBeforeEditEvent(), _T("���ؼ���ʼ�༭�����Ĺ���"),0,this);
	CComboBoxExProp* afterEdit = new CComboBoxExProp(_T("�༭����"), (_variant_t)ctl->TCGetAfterEditEvent(), _T("���ؼ������༭�����Ĺ���"),0,this);
	CComboBoxExProp* headSort = new CComboBoxExProp(_T("�������"), (_variant_t)ctl->TCGetHeadSortEvent(), _T("���ؼ��������"),0,this);
	leftClicked->AddOption(_T(""));
	beforeEdit->AddOption(_T(""));
	afterEdit->AddOption(_T(""));
	headSort->AddOption(_T(""));
	leftClicked->AllowEdit(FALSE);
	beforeEdit->AllowEdit(FALSE);
	afterEdit->AllowEdit(FALSE);
	headSort->AllowEdit(FALSE);
	ctlAction->AddSubItem(leftClicked);
	ctlAction->AddSubItem(beforeEdit);
	ctlAction->AddSubItem(afterEdit);
	ctlAction->AddSubItem(headSort);
	m_wndPropList.AddProperty(ctlAction);
}

void CPropertiesWnd::TCGetZOrderInfo(TCGETZORDERINFO &info,CWnd* pCtrl)
{
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(pCurrent!=NULL)
	{
		
		info.ControlSize = pCurrent->m_DiaDesign.m_ControlTracker.TCGetControlTrackerSize(pCtrl);
		//ע������Ƿ�������
		info.InControlAllPos = info.ControlSize - pCurrent->m_DiaDesign.m_ControlTracker.IsIncludeControl(pCtrl);
	}
}

LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM,LPARAM lParam) 
{
    CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;

    CString title = pProp->GetName();

    if(m_CurrentType.type == TCPROPERTY_PROJ)
    {
        if( _T("�ļ���") == title )
        {	
            if(CTCRunTimeManager::isSameFile(m_CurrentType.name,CTCRunTimeManager::g_projEnterFile))
            {
                pProp->SetValue(pProp->GetOriginalValue());
                return 0;
            }

            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            newValue.Trim();
            newValue.MakeLower();

            //���濪ʼ�����ļ��Ĳ���
            if( newValue.Right(2) != _T(".t"))
            {
                pProp->SetValue(pProp->GetOriginalValue());		
                return 0;
            }
            else
            {
                if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->FileReNameByPropWnd(m_CurrentType.name,newValue))
                {
                    AfxMessageBox(_T("������ʧ��"));
                    pProp->SetValue(pProp->GetOriginalValue());		
                    pProp->SetValue(oldValue);
                }
                else
                {
                    pProp->SetOriginalValue(pProp->GetValue());
                }		
            }
        }
		else if(title==_T("��Ʒ����"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				CTCExeVersionManager::TCSetVersionProName(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("�ļ�����"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				 CTCExeVersionManager::TCSetVersionDes(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("�ļ��汾"))
		{
			CString newValue,oldValue;
			newValue=pProp->GetValue().bstrVal;
			oldValue = pProp->GetOriginalValue().bstrVal;
			if(!newValue.IsEmpty())	
			{
				wchar_t str[MAX_PATH] = {0};
				wcscpy(str,newValue);
				int nod = 0;
				bool isNum = true;
				for(int i=0;i<wcslen(str);i++)
				{
					if(str[i] == '.' && str[i+1] != '.')
						nod++;
					else if(isdigit(str[i])==0 && isdigit(str[0])!=0)
						isNum = false;
				}
				if(nod != 3 || !isNum)
				{
					AfxMessageBox(_T("������4λ����,�м���'.'����,����:1.2.3.4"));
					pProp->SetValue(oldValue); 
				}
				else
					CTCExeVersionManager::TCSetVersionFile(newValue);
			}
			else
				pProp->SetValue(pProp->GetOriginalValue()); 
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("��Ʒ�汾"))
		{
			CString newValue,oldValue;
			newValue=pProp->GetValue().bstrVal;
			oldValue = pProp->GetOriginalValue().bstrVal;
			if(!newValue.IsEmpty())
			{
				wchar_t str[MAX_PATH] = {0};
				wcscpy(str,newValue);
				int nod = 0;
				bool isNum = true;
				for(int i=0;i<wcslen(str);i++)
				{
					if(str[i] == '.' && str[i+1] != '.' )
						nod++;
					else if(isdigit(str[i])==0 && isdigit(str[0])!=0)
						isNum = false;
				}
				if(nod != 3 || !isNum)
				{
					AfxMessageBox(_T("������4λ����,�м���'.'��������:1.2.3.4"));
					pProp->SetValue(oldValue); 
				}
				else
					CTCExeVersionManager::TCSetVersionProduct(newValue);
			}
			else
				pProp->SetValue(pProp->GetOriginalValue()); 
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("�ڲ�����"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				CTCExeVersionManager::TCSetVersionInternal(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("��˾����"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				CTCExeVersionManager::TCSetVersionCompany(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("��Ȩ��Ϣ"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				CTCExeVersionManager::TCSetVersionLeagle(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
    }  
	else if(m_CurrentType.type==TCPROPERTY_TWIN_NODE)
	{
		title.Trim();
		title.MakeLower();
		if(title==_T("��������"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();

			//��Ŀ�����ڲ���������
			if(oldValue == CTCRunTimeManager::g_ProjName)
			{
				pProp->SetValue(pProp->GetOriginalValue());
				return 0;
			}

			if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.isGoodID(newValue))
			{
				//�ҵ�ȫ�ִ��ڹ������еĵ�ǰ���ڽڵ㣬�޸Ľڵ��е�IDֵ
				int index  = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getIndexByID(oldValue);
				if(-1 == index)
					return 0;
				wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(index);
				wcscpy_s(temp->id,newValue);


				//�����ǰ��ͼ�ǻ�ģ����޸�Title
				CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(oldValue);
				if(pCurrent != NULL)
				{
					pCurrent->m_TWinID = newValue;
					pCurrent->m_DiaDesign.m_TcDlgConfig.m_dlgID = newValue;
					((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID = newValue;
					pCurrent->GetDocument()->SetTitle(newValue);
				}
	
				//�޸���Ŀ�������еĶ�Ӧ�ڵ�
				if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->FileReNameByPropWnd(oldValue/*m_CurrentType.name*/,newValue,FALSE))
				{
					AfxMessageBox(_T("�������ڵ�ʧ��"));
					pProp->SetValue(pProp->GetOriginalValue());
					return 0;
				}
				pProp->SetOriginalValue(newValue);
			}
			else
			{
				pProp->SetValue(pProp->GetOriginalValue());
				return 0;
			}

		}
	}
    else if(m_CurrentType.type==TCPROPERTY_TWIN)
    {
        title.Trim();
        title.MakeLower();
        CTCDesignView * pCurrent=NULL;

        CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTWin;
        POSITION pos=pDocTmp->GetFirstDocPosition();
        CTCDesignView *pView=NULL;
        CTCDesignDoc* pDoc = NULL;
        while (NULL != pos)
        {
            pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
            if(pDoc==NULL)
                continue;
            POSITION posView = pDoc->GetFirstViewPosition();
            while (NULL != posView)
            {
                pView = (CTCDesignView *)pDoc->GetNextView(posView);
                if (NULL == pView )
                    continue;
                //if(CTCRunTimeManager::isSameFile(CTCRunTimeManager::g_ProjName,pView->m_TWinID))
				if(m_CurrentType.pWnd == pView->m_DiaDesign.m_hWnd)
                {
                    pCurrent=pView;
                }
            }
        }

        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        //���Ա���
        if(title==_T("���ڱ���"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pCurrent->m_DiaDesign.SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);
            //pView->m_DiaDesign.ModifyStyle(DS_MODALFRAME,NULL);
            //pView->m_DiaDesign.ModifyStyle(WS_CAPTION,NULL);
            //pView->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��������"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();

			//��Ŀ�����ڲ���������
			if(pCurrent->m_DiaDesign.m_TcDlgConfig.m_dlgID == CTCRunTimeManager::g_ProjName)
			{
				pProp->SetValue(pProp->GetOriginalValue());
				return 0;
			}
				
			if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.isGoodID(newValue))
			{
				////////////////�ҵ�ȫ�ִ��ڹ������еĵ�ǰ���ڽڵ㣬�޸Ľڵ��е�IDֵ///////////////////
				int index  = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getIndexByID(oldValue);
				if(-1 == index)
					return 0;
				wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(index);
				wcscpy_s(temp->id,newValue);
				////////////////�ҵ�ȫ�ִ��ڹ������еĵ�ǰ���ڽڵ㣬�޸Ľڵ��е�IDֵ///////////////////

				pCurrent->m_TWinID = newValue;
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_dlgID = newValue;
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID = newValue;
				pCurrent->GetDocument()->SetTitle(newValue);
				//pDoc->SetTitle(newValue);		

				/////////////////�޸���Ŀ�������еĶ�Ӧ�ڵ�////////////////////////
				if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->FileReNameByPropWnd(oldValue/*m_CurrentType.name*/,newValue,FALSE))
				{
					AfxMessageBox(_T("�������ڵ�ʧ��"));
					pProp->SetValue(pProp->GetOriginalValue());
					return 0;
				}
				pProp->SetOriginalValue(newValue);
			}
			else
			{
				pProp->SetValue(pProp->GetOriginalValue());
				return 0;
			}
			
		}
        else if(title==_T("���ڿ��"))
        {
            int width=pProp->GetValue().intVal;
            RECT rc;
            pCurrent->m_DiaDesign.GetWindowRect(&rc);
            //pView->m_DiaDesign.ScreenToClient(&rc);
			pCurrent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LEFT,0),0);
            pCurrent->m_DiaDesign.MoveWindow(0,0,width,rc.bottom-rc.top);
            pProp->SetOriginalValue(pProp->GetValue());
        }
        else if(title==_T("���ڸ߶�"))
        {
            int height=pProp->GetValue().intVal;
            RECT rc;
            pCurrent->m_DiaDesign.GetWindowRect(&rc);
           // pCurrent->m_DiaDesign.ClientToScreen(&rc);
			pCurrent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_TOP,0),0);
            pCurrent->m_DiaDesign.MoveWindow(0,0,rc.right-rc.left,height);
            pProp->SetOriginalValue(pProp->GetValue());
        }
		////// ����VIP����///////////
		else if(title==_T("����ͼ��"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(newValue==_T(""))
				pCurrent->m_DiaDesign.TCSetIcon(_T(""));
			else
				pCurrent->m_DiaDesign.TCSetIcon(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		////// ����VIP����///////////
        else if(title==_T("���ڱ���ͼ"))
        {
            CString newValue=pProp->GetValue().bstrVal;
            if(newValue==_T("��"))
                pCurrent->m_DiaDesign.TCSetBackgroundImage(_T(""));
            else
                pCurrent->m_DiaDesign.TCSetBackgroundImage(newValue);
            pProp->SetOriginalValue(pProp->GetValue());
        }
        else if(title==_T("���ڱ�����ɫ"))
        {
            CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
			if(color == 0xffffffff)
				color = ::GetSysColor(COLOR_BTNFACE);
            pCurrent->m_DiaDesign.TCSetBkgrColor(color);
            pProp->SetOriginalValue(pProp->GetValue());
        }
		else if(title==_T("��С��"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			if(newValue)
				pCurrent->m_DiaDesign.TCSetMinButton(true);
			else
				pCurrent->m_DiaDesign.TCSetMinButton(false);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
		else if(title==_T("���"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			if(newValue)
				pCurrent->m_DiaDesign.TCSetMaxButton(true);
			else
				pCurrent->m_DiaDesign.TCSetMaxButton(false);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
        else if(title==_T("���ڳ�ʼ��"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            newValue.Trim();
            CString eDiaInit;
            if(CTCRunTimeManager::g_LangType==0)
            {
                eDiaInit.Format(_T("%s.��ʼ��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }
            else if(CTCRunTimeManager::g_LangType==1)
            {
                eDiaInit.Format(_T("%s.init"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }

            if(!newValue.IsEmpty())
            {
                //xxxxxxxxxxxxxx���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���
                pCurrent->m_DiaDesign.m_TcDlgConfig.m_eDiaInit=eDiaInit;
                pProp->SetOriginalValue(eDiaInit);
                pProp->SetValue(eDiaInit);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eDiaInit);
            }
        }
        else if(title==_T("��������"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            newValue.Trim();
            CString eDiaDestory;
            if(CTCRunTimeManager::g_LangType==0)
            {
                eDiaDestory.Format(_T("%s.����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }
            else if(CTCRunTimeManager::g_LangType==1)
            {
                eDiaDestory.Format(_T("%s.destroy"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }

            if(!newValue.IsEmpty())
            {
                //xxxxxxxxxxxxxx���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���
                pCurrent->m_DiaDesign.m_TcDlgConfig.m_eDestory=eDiaDestory;
                pProp->SetOriginalValue(eDiaDestory);
                pProp->SetValue(eDiaDestory);
				//���Dia�¼�����
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eDiaDestory);
            }
        }
		else if(title==_T("���ڹر�"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eDiaClose;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaClose.Format(_T("%s.%s_�ر�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaClose.Format(_T("%s.%s_close"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			if(!newValue.IsEmpty())
			{
				//xxxxxxxxxxxxxx���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_eClose=eDiaClose;
				pProp->SetOriginalValue(eDiaClose);
				pProp->SetValue(eDiaClose);
				//���Dia�¼�����
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eDiaClose,TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW);
			}
		}
		else if(title==_T("��Ϣ·��"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString ePreTrans;
			if(CTCRunTimeManager::g_LangType==0)
			{
				ePreTrans.Format(_T("%s.%s_��Ϣ·��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				ePreTrans.Format(_T("%s.%s_pretranslatemessage"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			if(!newValue.IsEmpty())
			{
				//xxxxxxxxxxxxxx���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_ePreTrans=ePreTrans;
				pProp->SetOriginalValue(ePreTrans);
				pProp->SetValue(ePreTrans);
				//���Dia�¼�����
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(ePreTrans,TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG);
			}
		}
		else if(title==_T("��Ϣ����"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eWinPorc;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eWinPorc.Format(_T("%s.%s_��Ϣ����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eWinPorc.Format(_T("%s.%s_windowproc"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			if(!newValue.IsEmpty())
			{
				//xxxxxxxxxxxxxx���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_eWinProc=eWinPorc;
				pProp->SetOriginalValue(eWinPorc);
				pProp->SetValue(eWinPorc);
				//���Dia�¼�����
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eWinPorc,TC_EVENT_TYPE::TC_EVENT_WINDOWPROC);
			}
		}
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_STATIC)
    {
        title.Trim();
        title.MakeLower();
		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
    //    CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTWin;
    //    POSITION pos=pDocTmp->GetFirstDocPosition();
    //    CTCDesignView *pView=NULL;
    //    CTCDesignDoc* pDoc = NULL;
    //    while (NULL != pos)
    //    {
    //        pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
    //        if(pDoc==NULL)
    //            continue;
    //        POSITION posView = pDoc->GetFirstViewPosition();
    //        while (NULL != posView)
    //        {
    //            pView = (CTCDesignView *)pDoc->GetNextView(posView);
    //            if (NULL == pView )
    //                continue;
    //            //if(CTCRunTimeManager::isSameFile(CTCRunTimeManager::g_ProjName,pView->m_TWinID))
				//if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID == pView->m_TWinID)
    //            {
    //                pCurrent=pView;
    //            }
    //        }
    //    }

        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCStatic *pStatic=(CTCStatic *)pRect->GetCWnd();
        if(!pStatic)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pStatic,TCTT_STATIC,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
           /* CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
        //���Ա���
        else if(title==_T("�ı�"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pStatic,TCTT_STATIC,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
           /* CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pStatic->SetText(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
        else if(title==_T("��ǩ������ɫ"))
        {
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
					if(color == 0xffffffff)
				color = ::GetSysColor(COLOR_BTNFACE);

			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_STATIC,pStatic,m_CurrentType.name,color);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);

			pProp->SetOriginalValue(pProp->GetValue());

           /* CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
            pStatic->SetBackground(color);
            pProp->SetOriginalValue(pProp->GetValue());*/
        }
        else if(title==_T("����"))
        {
            CMFCPropertyGridFontProperty* pFontProp=(CMFCPropertyGridFontProperty*)pProp;
            LPLOGFONT font=pFontProp->GetLogFont();
            COLORREF color=pFontProp->GetColor();
            DWORD style=FS_NORMAL;
            if(font->lfWeight==FW_BOLD)
                style|=FS_BOLD;
            if(font->lfUnderline)
                style|=FS_UNDERLINED;
            if(font->lfItalic)
                style|=FS_ITALIC;
            if(font->lfStrikeOut)
                style|=FS_STRIKETHROUGH;

            pStatic->ResetStyle();
            pStatic->SetFontStatic(font->lfFaceName,font->lfHeight,color,style);
            pFontProp->SetOriginalValue(pProp->GetValue());
        }
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
			pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
            pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
            pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
			pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pStatic);
		}
        else if(title==_T("���"))
        {
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pStatic->TCSetClickEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
        }
        //����ֵ
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_BUTTON)
    {
        title.Trim();
        title.MakeLower();
		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCButton *pButton=(CTCButton *)pRect->GetCWnd();
        if(!pButton)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }
        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pButton,TCTT_BUTTON,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
           /* CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                pButton->TCSetID(newValue); 
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
        //���Ա���
        else if(title==_T("�ı�"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pButton,TCTT_BUTTON,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pButton->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//���Ա���
        else if(title==_T("Ĭ�ϰ�ť"))
        {
			//�����е�Ĭ�ϰ�ť��Ϊfalse
			pCurrent->m_DiaDesign.SetAllDefButtonFalse();

			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_BUTTON,pButton,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
        }
		//������Ч
		else if(title==_T("��Ч"))
        {
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_BUTTON,pButton,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
        }
		else if(title==_T("����"))
        {
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_BUTTON,pButton,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pButton->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
			pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
            pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
            pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
            pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pButton);
		}		
        else if(title==_T("���"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pButton->TCSetClickEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
        }
        //����ֵ
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_RADIO)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
        CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCRadio *pRadio=(CTCRadio *)pRect->GetCWnd();
        if(!pRadio)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pRadio,TCTT_RADIO,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
        //���Ա���
        else if(title==_T("����"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pRadio,TCTT_RADIO,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pRadio->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//������Ч
		else if(title==_T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_RADIO,pRadio,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("����"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			pRadio->TCSetGroup(newValue);
		}
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
			pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
            pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
            pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
            pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pRadio);
		}
        else if(title==_T("ѡ��"))
        {
            bool newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().boolVal;
            newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_RADIO,pRadio,m_CurrentType.name,newValue);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pRadio->TCSetCheck(newValue);
            //pProp->SetOriginalValue((_variant_t)newValue);
        }
		else if(title==_T("�������"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_RADIO,pRadio,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("���"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pRadio->TCSetRadioEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
			//pRadio->TCSetRadioEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pRadio->TCGetRadioEvent());
        }
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_CHECK)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCCheck *pCheck=(CTCCheck *)pRect->GetCWnd();
        if(!pCheck)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pCheck,TCTT_CHECK,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
        //���Ա���
        else if(title==_T("����"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pCheck,TCTT_CHECK,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pCheck->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//������Ч
		else if(title==_T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_CHECK,pCheck,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pCheck);
		}
        else if(title==_T("ѡ��"))
        {
            bool newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().boolVal;
            newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_CHECK,pCheck,m_CurrentType.name,newValue);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pCheck->TCSetCheck(newValue);
            //pProp->SetOriginalValue((_variant_t)newValue);
        }
		else if(title==_T("�������"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_CHECK,pCheck,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("���"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pCheck->TCSetCheckEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
			//pCheck->TCSetCheckEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pCheck->TCGetCheckEvent());
        }
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_EDIT)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCEdit *pEdit=(CTCEdit *)pRect->GetCWnd();
        if(!pEdit)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pEdit,TCTT_EDIT,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
        //���Ա���
        else if(title==_T("�ı�"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pEdit,TCTT_EDIT,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pEdit->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//������Ч
		else if(title==_T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		//�Ƿ����
		else if(title == _T("����"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CMFCPropertyGridProperty* pPropVerti= m_wndPropList.FindItemByData(DWORD_PTR(_T("�༭��ֱ������")));
			CMFCPropertyGridProperty* pPropHoniz = m_wndPropList.FindItemByData(DWORD_PTR(_T("�༭��ˮƽ������")));

			if(newValue)
			{
				pPropVerti->Enable(TRUE);				pPropHoniz->Enable(TRUE);
			}
			else
			{
				pPropVerti->Enable(FALSE);				pPropHoniz->Enable(FALSE);
			}

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);


		}
        else if(title==_T("�ı���ɫ"))
        {
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_EDIT,pEdit,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			 pProp->SetOriginalValue(pProp->GetValue());
            /*CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
            pEdit->SetTextColor(color);
            pProp->SetOriginalValue(pProp->GetValue());*/
        }
        else if(title==_T("������ɫ"))
        {
            CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = RGB(255,255,255);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_EDIT,pEdit,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*pEdit->SetBackColor(color);*/
            pProp->SetOriginalValue(pProp->GetValue());
        }

        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pEdit);
		}
        else if(title==_T("����"))
        {
            bool newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().boolVal;
            newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pEdit->SetPassWord(newValue);
            //pProp->SetOriginalValue((_variant_t)newValue);
        }
		else if(title==_T("����"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
		else if(title==_T("ֻ��"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
		else if(title==_T("��ֱ������"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
		else if(title==_T("ˮƽ������"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
        else if(title==_T("���ݸı�"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString enChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				enChange.Format(_T("%s.%s_���ݸı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				enChange.Format(_T("%s.%s_enchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pEdit->TCSetEnChangeEvent(enChange);
				pProp->SetOriginalValue(enChange);
				pProp->SetValue(enChange);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(enChange);
			}
   //         pEdit->TCSetEnChangeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pEdit->TCGetEnChangeEvent());
        }
		else if(title==_T("��ý���"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString setFocus;
			if(CTCRunTimeManager::g_LangType==0)
			{
				setFocus.Format(_T("%s.%s_��ý���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				setFocus.Format(_T("%s.%s_setfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pEdit->TCSetFocusEvent(setFocus);
				pProp->SetOriginalValue(setFocus);
				pProp->SetValue(setFocus);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(setFocus);
			}
			//         pEdit->TCSetKillfocusEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pEdit->TCGetKillfocusEvent());
		}
        else if(title==_T("ʧȥ����"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString killFocus;
			if(CTCRunTimeManager::g_LangType==0)
			{
				killFocus.Format(_T("%s.%s_ʧȥ����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				killFocus.Format(_T("%s.%s_killfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pEdit->TCSetKillfocusEvent(killFocus);
				pProp->SetOriginalValue(killFocus);
				pProp->SetValue(killFocus);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(killFocus);
			}
   //         pEdit->TCSetKillfocusEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pEdit->TCGetKillfocusEvent());
        }
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_GROUP)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCGroupBox *pGroup=(CTCGroupBox *)pRect->GetCWnd();
        if(!pGroup)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pGroup,TCTT_GROUP,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
        //���Ա���
        else if(title==_T("����"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pGroup,TCTT_GROUP,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pGroup->SetText(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
        else if(title==_T("���ڷ��"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(newValue==_T("���ڷ��"))
                pGroup->SetTCGroupStyle(CTCGroupBox::TCGB_WINDOW);
            else if(newValue==_T("�߿���"))
                pGroup->SetTCGroupStyle(CTCGroupBox::TCGB_FRAME);
            else
            {
                newValue=_T("�߿���");
                pGroup->SetTCGroupStyle(CTCGroupBox::TCGB_FRAME);
            }
            pProp->SetOriginalValue(newValue);
        }
        else if(title==_T("����"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            DWORD dir;  //=pGroup->GetAlignment();
            if(newValue==_T("�����"))
                dir=SS_LEFT;
            else if(newValue==_T("�м����"))
                dir=SS_CENTER;
            else if(newValue==_T("�Ҷ���"))
                dir=SS_RIGHT;
            else 
                dir=SS_LEFT;

            pGroup->SetAlignment(dir);

            pProp->SetOriginalValue(newValue);
        }
        else if(title==_T("���ⱳ����ɫ"))
        {
            CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
			if(color == 0xffffffff)
				color = ::GetSysColor(COLOR_BTNFACE);		
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GROUP,pGroup,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
           /* pGroup->SetBackgroundColor(color,pGroup->GetClientBackgroundColor());*/
            pProp->SetOriginalValue(pProp->GetValue());
        }
        else if(title==_T("������ɫ"))
        {
            CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
			if(color == 0xffffffff)
				color = ::GetSysColor(COLOR_BTNFACE);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GROUP,pGroup,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pGroup->SetBackgroundColor(pGroup->GetTitleBackgroundColor(),color);
            pProp->SetOriginalValue(pProp->GetValue());
        }
        else if(title==_T("�߿���ɫ"))
        {
            CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
			if(color == 0xffffffff)
				color = ::GetSysColor(COLOR_3DSHADOW);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GROUP,pGroup,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pGroup->SetBorderColor(color);
            pProp->SetOriginalValue(pProp->GetValue());
        }

        else if(title==_T("����"))
        {
            CMFCPropertyGridFontProperty* pFontProp=(CMFCPropertyGridFontProperty*)pProp;
            LPLOGFONT font=pFontProp->GetLogFont();
            COLORREF color=pFontProp->GetColor();
			pGroup->SetFontFaceName(font->lfFaceName);
            pGroup->SetCatptionTextColor(color);
            pGroup->SetFont(*font);
            pFontProp->SetOriginalValue(pProp->GetValue());
        }

        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pGroup);
		}
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_PIC)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCPicture *pPic=(CTCPicture *)pRect->GetCWnd();
        if(!pPic)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pPic,TCTT_PICTURE,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
           /* CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		else if(title==_T("ͼƬ����"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pPic,TCTT_PICTURE,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
            pPic->SetRCPicture(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pPic);
		}
		else if(title==_T("�������"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pPic->TCSetClickEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_COMBOX)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCComboBox *pCombox=(CTCComboBox *)pRect->GetCWnd();
        if(!pCombox)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pCombox,TCTT_COMBOBOX,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		//������Ч
		else if(title==_T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_COMBOBOX,pCombox,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pCombox->GetWindowRect(&rc);
			pCombox->GetParent()->ScreenToClient(&rc);
            //pView->m_DiaDesign.ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pCombox->GetWindowRect(&rc);
			pCombox->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pCombox->GetWindowRect(&rc);
			pCombox->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pCombox->GetWindowRect(&rc);
			pCombox->GetParent()->ScreenToClient(&rc);
            //pView->m_DiaDesign.ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pCombox);
		}
		else if(title==_T("�������"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString lBtnDown;
			if(CTCRunTimeManager::g_LangType==0)
			{
				lBtnDown.Format(_T("%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				lBtnDown.Format(_T("%s.%s_lbuttondown"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pCombox->SetLeftDownEvent(lBtnDown);
				pProp->SetOriginalValue(lBtnDown);
				pProp->SetValue(lBtnDown);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(lBtnDown);
			}
			//pCombox->SetLeftDownEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pCombox->GetLeftDownEvent());
        }
		else if(title==_T("ѡ��ı�"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchanged"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pCombox->SetEnChangeEvent(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
			//pCombox->SetEnChangeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pCombox->GetEnChangeEvent());
        }
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_PROGRESS)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCProgress *pProgress=(CTCProgress *)pRect->GetCWnd();
        if(!pProgress)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pProgress,TCTT_PROGRESS,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
			pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
            pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
            pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
            pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pProgress);
		}
		else if(title==_T("��ֱ"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_PROGRESS,pProgress,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_PATH)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCBrowse *pPath=(CTCBrowse *)pRect->GetCWnd();
        if(!pPath)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pPath,TCTT_PATH,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		else if(title==_T("���ģʽ"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CtrlProptyLogicCommand* pCommand;
			if( newValue == _T("�ļ�") )
				pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_PATH,pPath,m_CurrentType.name,true,title);
			else if( newValue == _T("�ļ���"))
				pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_PATH,pPath,m_CurrentType.name,false,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			
                /*pPath->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_File);
			
                pPath->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_Folder);*/

           // pProp->SetOriginalValue(newValue);
        }
		//������Ч
		else if(title==_T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_PATH,pPath,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pPath);
		}
        else if(title==_T("ѡ��ı�"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchanged"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pPath->TCSetEnChangeEvent(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
   //         pPath->TCSetEnChangeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pPath->TCGetEnChangeEvent());
        }
    }
    else if(m_CurrentType.type==TCPROPERTY_CTRL_SLIDER)
    {
        title.Trim();
        title.MakeLower();
		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCSlider *pSlider=(CTCSlider *)pRect->GetCWnd();
        if(!pSlider)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pSlider,TCTT_SLIDER,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		//������Ч
		else if(title == _T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_SLIDER,pSlider,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pSlider);
		}
		else if(title==_T("��ֱ"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_SLIDER,pSlider,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("����ͷ�"))
		{
			CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_����ͷ�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_releasedcapture"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pSlider->TCSetSliderChangeEvent(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
   //         pSlider->TCSetSliderChangeEvent(newValue);
   //         pProp->SetOriginalValue((_variant_t)newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pSlider->TCGetSliderChangeEvent());
		}
	}
    else if(m_CurrentType.type==TCPROPERTY_CTRL_DATE)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCDate *pDate=(CTCDate *)pRect->GetCWnd();
        if(!pDate)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pDate,TCTT_DATE,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		//������Ч
		else if(title == _T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_DATE,pDate,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
			pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
            pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
            pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
            pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pDate);
		}
	}
    else if(m_CurrentType.type==TCPROPERTY_CTRL_LIST)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCListBox *pList=(CTCListBox *)pRect->GetCWnd();
        if(!pList)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pList,TCTT_LISTBOX,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		if(title==_T("����"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pList,TCTT_LISTBOX,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			pList->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//������Ч
		else if(title == _T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_LISTBOX,pList,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
            pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
			pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
            pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
            pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pList);
		}
		else if(title == _T("Ĭ�Ϲ���"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_LISTBOX,pList,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("���˫��"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString clicked;
			if(CTCRunTimeManager::g_LangType==0)
			{
				clicked.Format(_T("%s.%s_���˫��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				clicked.Format(_T("%s.%s_doubleclicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pList->TCSetListBoxDoubleClick(clicked);
				pProp->SetOriginalValue(clicked);
				pProp->SetValue(clicked);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(clicked);
			}
		}
		else if(title==_T("ѡ��ı�"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pList->TCSetListBoxSelChange(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
		}

    }
	else if(m_CurrentType.type==TCPROPERTY_CTRL_TABLE)
    {
        title.Trim();
        title.MakeLower();
		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCTab *pTab=(CTCTab *)pRect->GetCWnd();
        if(!pTab)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pTab,TCTT_TAB,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		//������Ч
		else if(title==_T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_TAB,pTab,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pTab);
		}
		else if(title==_T("ѡ��ı�"))
		{
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pTab->TCSetSelChange(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
		}
	}
	else if(m_CurrentType.type==TCPROPERTY_CTRL_TIMER)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCTimer *pTimer=(CTCTimer *)pRect->GetCWnd();
        if(!pTimer)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pTimer,TCTT_TIMER,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		else if(title==_T("Ƶ��"))
        {
			UINT time=pProp->GetValue().uintVal;
			pTimer->SetTimer(time);
            pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pTimer->GetWindowRect(&rc);
            pTimer->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pTimer->GetWindowRect(&rc);
            pTimer->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("ʱ��"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString timer;
			if(CTCRunTimeManager::g_LangType==0)
			{
				timer.Format(_T("%s.%s_ʱ��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				timer.Format(_T("%s.%s_ontime"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pTimer->TCSetTimeEvent(timer);
				pProp->SetOriginalValue(timer);
				pProp->SetValue(timer);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(timer);
			}
			//pTimer->TCSetTimeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pTimer->TCGetTimeEvent());
        }
	}
	else if(m_CurrentType.type==TCPROPERTY_CTRL_HOTKEY)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCHotKeyCtrl *pHotKey=(CTCHotKeyCtrl *)pRect->GetCWnd();
        if(!pHotKey)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }
        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pHotKey,TCTT_HOTKEY,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		//������Ч
		else if(title==_T("��Ч"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_HOTKEY,pHotKey,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("��ֵ"))
        {
			CString newValue,oldValue;
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			//��������ȼ��ַ���
			pHotKey->SetHotKeyName(newValue);
			pProp->SetOriginalValue((_variant_t)newValue);

        }
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pHotKey);
		}
		else if(title==_T("�ȼ�"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString hotkey;
			if(CTCRunTimeManager::g_LangType==0)
			{
				hotkey.Format(_T("%s.%s_�ȼ�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				hotkey.Format(_T("%s.%s_onhotkey"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pHotKey->TCSetHotKeyEvent(hotkey);
				pProp->SetOriginalValue(hotkey);
				pProp->SetValue(hotkey);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(hotkey);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
        }
		else if(title==_T("ʧȥ����"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString killfocus;
			if(CTCRunTimeManager::g_LangType==0)
			{
				killfocus.Format(_T("%s.%s_ʧȥ����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				killfocus.Format(_T("%s.%s_killfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pHotKey->TCSetKillFocusEvent(killfocus);
				pProp->SetOriginalValue(killfocus);
				pProp->SetValue(killfocus);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(killfocus);
			}
			//pHotKey->TCSetKillFocusEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetKillFocusEvent());
        }
	}
	else if(m_CurrentType.type==TCPROPERTY_CTRL_EXPLORER)
    {
        title.Trim();
        title.MakeLower();

		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
			
			 return 0;
			 
			  }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CTCExplorer *pExplor=(CTCExplorer *)pRect->GetCWnd();
        if(!pExplor)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }
        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pExplor,TCTT_EXPLORER,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(oldValue,newValue))
            {
                this->m_CurrentType.name=newValue;
                pProp->SetOriginalValue(newValue);
            }
            else
                pProp->SetValue(pProp->GetOriginalValue());*/
        }
		//������ַ
		else if(title.CompareNoCase(_T("URL"))==0)
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			pExplor->TCSetURL(newValue);
		}	
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pExplor);
		}
		else if(title==_T("�������"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_load"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pExplor->TCSetloadComplete(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
	}
	else if(m_CurrentType.type==TCPROPERTY_CTRL_GRIDCTRL)
    {
        title.Trim();
        title.MakeLower();
		CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
        if(!pCurrent)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }

        CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_CurrentType.name);
        CGridCtrl *pGridCtrl=(CGridCtrl *)pRect->GetCWnd();
        if(!pGridCtrl)
        {
            pProp->SetValue(pProp->GetOriginalValue());
            return 0;
        }
        if(title==_T("����"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pGridCtrl,TCTT_GRIDCTRL,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
        }
		else if(title==_T("����ģʽ"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CMFCPropertyGridProperty* pPropEdit= m_wndPropList.FindItemByData(DWORD_PTR(_T("�༭���")));
			CMFCPropertyGridProperty* pPropRow = m_wndPropList.FindItemByData(DWORD_PTR(_T("�̶���ѡ��")));
			CMFCPropertyGridProperty* pPropCol = m_wndPropList.FindItemByData(DWORD_PTR(_T("�̶���ѡ��")));
			
			if(newValue)
			{	pPropEdit->Enable(FALSE);	pPropRow->Enable(FALSE);	pPropCol->Enable(FALSE);}
			else
			{	pPropEdit->Enable(TRUE);	pPropRow->Enable(TRUE);		pPropCol->Enable(TRUE); }

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("��ͨ���ı���ɫ"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = ::GetSysColor(COLOR_WINDOWTEXT);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("��ͨ�����ɫ"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = RGB(0xFF,0xFF,0xE0);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("�̶������ɫ"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = ::GetSysColor(COLOR_BTNFACE);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("�̶����ı���ɫ"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = ::GetSysColor(COLOR_WINDOWTEXT);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("��ֱ����"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("ˮƽ����"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("����"))
		{
			CMFCPropertyGridFontProperty* pFontProp=(CMFCPropertyGridFontProperty*)pProp;
			LPLOGFONT font=pFontProp->GetLogFont();
			COLORREF color=pFontProp->GetColor();
			DWORD style=FS_NORMAL;
			if(font->lfWeight==FW_BOLD)
				style|=FS_BOLD;
			if(font->lfUnderline)
				style|=FS_UNDERLINED;
			if(font->lfItalic)
				style|=FS_ITALIC;
			if(font->lfStrikeOut)
				style|=FS_STRIKETHROUGH;
			CFont *pFont = new CFont();
			pFont->CreateFontIndirectW(font);
			pGridCtrl->SetFont(pFont);
			pGridCtrl->AutoSize();
			delete pFont;
			pFontProp->SetOriginalValue(pProp->GetValue());
		}
        else if(title==_T("x����"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y����"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("���"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("�߶�"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("��ʾ˳��"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pGridCtrl);
		}
		else if(title==_T("�ɱ༭"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("����"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("���"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("����"))
		{
			int rowCount=pProp->GetValue().intVal;
			pGridCtrl->SetRowCount(rowCount);
			for(int col = 0;col < pGridCtrl->GetColumnCount();col ++)
			{
				for(int row = 0;row < rowCount;row ++)
				{
					switch(pGridCtrl->GetItemData(0,col))
					{
					case TCGRID_CTRL_NOMAL:
						if(pGridCtrl->TCGetColReadInfo(0,col))
							pGridCtrl->SetItemState(row,col,pGridCtrl->GetItemState(row,col) | GVIS_READONLY);
						pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCell));
						break;
					case TCGRID_CTRL_COMBOX:
						if(!pGridCtrl->IsCellFixed(row,col))
						{		
							if(pGridCtrl->TCGetColReadInfo(0,col))
								pGridCtrl->SetItemState(row,col,pGridCtrl->GetItemState(row,col) | GVIS_READONLY);
							pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellCombo));			
						}
						break;
					case TCGRID_CTRL_READONLY:
						if(!pGridCtrl->IsCellFixed(row,col))
						{
							pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCell));
							pGridCtrl->SetItemState(row,col,pGridCtrl->GetItemState(row,col)|GVIS_READONLY);
						}			
						break;
					case TCGRID_CTRL_CHECK:
						if(!pGridCtrl->IsCellFixed(row,col))
						{
							if(pGridCtrl->TCGetColReadInfo(0,col))
								pGridCtrl->SetItemState(row,col,pGridCtrl->GetItemState(row,col) | GVIS_READONLY);
							pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellCheck));
						}
						break;
					case TCGRID_CTRL_NUM:
						if(!pGridCtrl->IsCellFixed(row,col))
						{
							if(pGridCtrl->TCGetColReadInfo(0,col))
								pGridCtrl->SetItemState(row,col,pGridCtrl->GetItemState(row,col) | GVIS_READONLY);
							pGridCtrl->SetCellType(row,col,RUNTIME_CLASS(CGridCellNumeric));
						}
						break;
					case TCGRID_CTRL_DATE:
						if(!pGridCtrl->IsCellFixed(row,col))
						{
							if(pGridCtrl->TCGetColReadInfo(0,col))
								pGridCtrl->SetItemState(row,col,pGridCtrl->GetItemState(row,col) | GVIS_READONLY);
							pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridCellDateTime));
							CGridCellDateTime* pCell = (CGridCellDateTime*)pGridCtrl->GetCell(row,col);
							pCell->SetTime(CTime::GetCurrentTime());
							pGridCtrl->RedrawCell(row,col);
						}
						break;
					/*case TCGRID_CTRL_URL:
						if(!pGridCtrl->IsCellFixed(row,col))
						{
							if(pGridCtrl->TCGetColReadInfo(0,col))
								pGridCtrl->SetItemState(row,col,pGridCtrl->GetItemState(row,col) | GVIS_READONLY);
							pGridCtrl->SetCellType(row,col, RUNTIME_CLASS(CGridURLCell));
							pGridCtrl->SetItemText(row,col,_T("www.tyuyan.com"));
						}
						break;*/
					default:
						break;
					}		
				}
			}		
			pGridCtrl->Invalidate(TRUE);
		}
		else if(title==_T("����"))
		{
			int colCount=pProp->GetValue().intVal;
			int oldColumCount = pGridCtrl->GetColumnCount();
			pGridCtrl->SetColumnCount(colCount);

			CString title;
			for (int i = oldColumCount; i < colCount; i++)
			{
				title.Format(_T("�� %d ��"), i);
				pGridCtrl->SetItemText(0,i,title);
				pGridCtrl->SetItemData(0,i,TCGRID_CTRL_NOMAL);
				pGridCtrl->TCSetColReadInfo(0,i,FALSE);
			}
		}
		else if(title==_T("�̶�����"))
		{
			int fixedRowCount=pProp->GetValue().intVal;
			pGridCtrl->SetFixedRowCount(fixedRowCount);
		}
		else if(title==_T("�̶�����"))
		{
			int fixedColCount=pProp->GetValue().intVal;
			pGridCtrl->SetFixedColumnCount(fixedColCount);
		}
		else if(title==_T("ѡ��"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CMFCPropertyGridProperty* pPropRow = m_wndPropList.FindItemByData(DWORD_PTR(_T("�̶���ѡ��")));
			CMFCPropertyGridProperty* pPropCol = m_wndPropList.FindItemByData(DWORD_PTR(_T("�̶���ѡ��")));
			CMFCPropertyGridProperty* pPropListMode = m_wndPropList.FindItemByData(DWORD_PTR(_T("�б�ģʽѡ��")));

			if(newValue)
			{
				pPropRow->Enable(TRUE);
				pPropCol->Enable(TRUE);
				pPropListMode->Enable(TRUE);
			}
			else
			{
				pPropRow->Enable(FALSE);
				pPropCol->Enable(FALSE);
				pPropListMode->Enable(FALSE);
			}
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("�̶���ѡ��"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("�̶���ѡ��"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("�б�ģʽ"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CMFCPropertyGridProperty* pPropRow = m_wndPropList.FindItemByData(DWORD_PTR(_T("�̶���ѡ��")));
			if(newValue)
				pPropRow->Enable(FALSE);
			else
				pPropRow->Enable(TRUE);
				

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("�������"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pGridCtrl->TCSetClickEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
		else if(title==_T("�༭��ʼ"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_�༭��ʼ"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_beforeedit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pGridCtrl->TCSetBeforeEditEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
		else if(title==_T("�༭����"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_�༭����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_afteredit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pGridCtrl->TCSetAfterEditEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
		else if(title==_T("�������"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_headsort"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////���ﻹҪ�Ӵ�T�ļ�,������¼�����Ĳ���//////////////
				pGridCtrl->TCSetHeadSortEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//����¼��������
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////����¼�����
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
	}
    return 0;
}	

void CPropertiesWnd::SetPropertyString(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue)
{
	if( protype != m_CurrentType.type )
		return;
	if( m_CurrentType.name != name )
		return;

	CMFCPropertyGridProperty* pProp = m_wndPropList.FindItemByData(title);
	if(pProp)
	{
		pProp->SetValue((_variant_t)newValue);
	}
	else
		return ;

}


//�����ض��������(����1���������ͣ�����2����ǰ���ƣ�����3�����⣬����4����ֵ)
void CPropertiesWnd::SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue)
{
    if( protype != m_CurrentType.type )
        return;
    if( m_CurrentType.name != name )
		return;
	//�õ�Ҫ�༭����
	CMFCPropertyGridProperty* pProp = m_wndPropList.FindItemByData(title);
	if(pProp)
	{
		LONG width = _wtoi(newValue);
		pProp->SetValue((_variant_t)width);
	}
	else
		return ;

}

////////////////////////////////////////////////////////////////////////////////
// CComboBoxExProp class
CComboBoxExProp::CComboBoxExProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr, DWORD dwData, CPropertiesWnd *cWnd) :
CMFCPropertyGridProperty(strName, (LPCTSTR) strValue, lpszDescr, dwData)
{
    m_CProperWnd=cWnd;

}

CComboBox* CComboBoxExProp::CreateCombo(CWnd* pWndParent, CRect rect)
{
    rect.bottom = rect.top + 400;

    CComboBoxEx* pWndCombo = new CComboBoxEx;

    if (!pWndCombo->Create(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, rect, pWndParent, AFX_PROPLIST_ID_INPLACE))
    {
        delete pWndCombo;
        return NULL;
    }

    return pWndCombo;
}

BOOL CComboBoxExProp::OnEdit(LPPOINT lptClick)
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
    if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_TWIN)
    {
        if(title==_T("���ڱ���ͼ"))
        {
            pWndComboEx->ResetContent();
            CString lpPath;
            lpPath.Format(_T("%s\\��Դ\\"),CTCRunTimeManager::g_ProjSourcePath);
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            int i=0;

            Item.pszText   = _T("��"); 
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
		else if(title==_T("���ڳ�ʼ��"))
		{

			CString eDiaInit;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaInit.Format(_T("<���>%s.��ʼ��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaInit.Format(_T("<���>%s.init"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
		
			TCHAR csClickEvent[MAXCTRLNAME+3];
			::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csClickEvent,eDiaInit); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csClickEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("��������"))
		{

			CString eDiaDestory;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaDestory.Format(_T("<���>%s.����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaDestory.Format(_T("<���>%s.destroy"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			TCHAR csClickEvent[MAXCTRLNAME+3];
			::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csClickEvent,eDiaDestory); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csClickEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("���ڹر�"))
		{

			CString eDiaClose;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaClose.Format(_T("<���>%s.�ر�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaClose.Format(_T("<���>%s.close"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			TCHAR csClickEvent[MAXCTRLNAME+3];
			::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csClickEvent,eDiaClose); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csClickEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("��Ϣ·��"))
		{

			CString ePreTrans;
			if(CTCRunTimeManager::g_LangType==0)
			{
				ePreTrans.Format(_T("<���>%s.��Ϣ·��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				ePreTrans.Format(_T("<���>%s.pretranslatemessage"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			TCHAR csClickEvent[MAXCTRLNAME+3];
			::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csClickEvent,ePreTrans); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csClickEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_STATIC)
    {
        if(title==_T("���"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;

			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_BUTTON)
    {
        if(title==_T("���"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_RADIO)
    {
        if(title==_T("���"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_CHECK)
    {
        if(title==_T("���"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_EDIT)
    {
        if(title==_T("���ݸı�"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_���ݸı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_enchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
		else if( title==_T("��ý���"))
		{
			CString eSetFocus;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eSetFocus.Format(_T("<���>%s.%s_��ý���"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eSetFocus.Format(_T("<���>%s.%s_setfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			TCHAR csClickEvent[MAXCTRLNAME+3];
			::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csClickEvent,eSetFocus); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csClickEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
        else if( title==_T("ʧȥ����"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_ʧȥ����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_killfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
	else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_LIST)
    {
		if( title==_T("���˫��"))
		{
			CString doubleclicked;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				doubleclicked.Format(_T("<���>%s.%s_���˫��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				doubleclicked.Format(_T("<���>%s.%s_doubleclicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			TCHAR csClickEvent[MAXCTRLNAME+3];
			::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csClickEvent,doubleclicked); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csClickEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if( title==_T("ѡ��ı�"))
        {
            CString selChange;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("<���>%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("<���>%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,selChange); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
	else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_TABLE)
    {

        if( title==_T("ѡ��ı�"))
        {
            CString selChange;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("<���>%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("<���>%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,selChange); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_COMBOX)
    {
		if(title==_T("�������"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_lbuttondown"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
		else if(title==_T("ѡ��ı�"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_selectchanged"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
           // eClick.Format(_T("%s.%s_ѡ��ı�"),CTCRunTimeManager::g_ProjName,id);
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
	}
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_PIC)
    {
        if(title==_T("ͼƬ����"))
        {
            pWndComboEx->ResetContent();
            CString lpPath;
            lpPath.Format(_T("%s\\��Դ\\"),CTCRunTimeManager::g_ProjSourcePath);
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            int i=0;
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
		else if(title==_T("�������"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<���>%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<���>%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_�ȼ�"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_PATH)
    {
        if(title==_T("ѡ��ı�"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_ѡ��ı�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_ѡ��ı�"),CTCRunTimeManager::g_ProjName,id);
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
    else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_SLIDER)
    {
        if(title==_T("����ͷ�"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_����ͷ�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_releasedcapture"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_ѡ��ı�"),CTCRunTimeManager::g_ProjName,id);
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
    }
	else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_TIMER)
    {
		if(title==_T("ʱ��"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_ʱ��"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_ontime"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_ʱ��"),CTCRunTimeManager::g_ProjName,id);
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
	}
	else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_HOTKEY)
    {
		if(title==_T("�ȼ�"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_�ȼ�"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_onhotkey"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_�ȼ�"),CTCRunTimeManager::g_ProjName,id);
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
		else if(title==_T("ʧȥ����"))
		{
			CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<���>%s.%s_ʧȥ����"),CTCRunTimeManager::g_ProjName,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<���>%s.%s_killfocus"),CTCRunTimeManager::g_ProjName,id);
			}
            //eClick.Format(_T("%s.%s_ʧȥ����"),CTCRunTimeManager::g_ProjName,id);
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
	}
	else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_EXPLORER)
	{
		if(title==_T("�������"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<���>%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<���>%s.%s_load"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_�ȼ�"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
	}
	else if(m_CProperWnd->m_CurrentType.type==TCPROPERTY_CTRL_GRIDCTRL)
	{
		if(title==_T("�������"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<���>%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<���>%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_�ȼ�"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("�༭��ʼ"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<���>%s.%s_�༭��ʼ"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<���>%s.%s_beforeedit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_�ȼ�"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("�༭����"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<���>%s.%s_�༭����"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<���>%s.%s_afteredit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_�ȼ�"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("�������"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<���>%s.%s_�������"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<���>%s.%s_headsort"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_�ȼ�"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
	}
    return TRUE;
}

CCustomDlgProp::CCustomDlgProp(const CString& strName, const COleVariant& varValue, CPropertiesWnd *cWnd) : CMFCPropertyGridProperty(strName, varValue)
{
    m_CProperWnd = cWnd;
}

void CCustomDlgProp::OnClickButton(CPoint /*point*/)
{
	if(m_CProperWnd->m_CurrentType.type == TCPROPERTY_TWIN)
	{
		CFileDialog filedlg(TRUE,
			_T(""),
			_T(""),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			// ע���������д��
			_T("ICON Files (*.ico)|*.ico||"),NULL);
		
		if(IDOK == filedlg.DoModal())
		{
			CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
			CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

			if(!pCurrent)
				return;

			pCurrent->m_DiaDesign.m_TcDlgConfig.m_icon = filedlg.GetPathName();
			m_CProperWnd->SetPropertyString(TCPROPERTY_TWIN,CTCRunTimeManager::g_ProjName,DWORD_PTR(_T("����ͼ��")),pCurrent->m_DiaDesign.m_TcDlgConfig.m_icon);			
		}
	}
    else if(m_CProperWnd->m_CurrentType.type == TCPROPERTY_CTRL_COMBOX)
    {
        CTCComBoxItem dlg;
        dlg.m_ctrlID = m_CProperWnd->m_CurrentType.name;
        dlg.DoModal();
    }
    else if(m_CProperWnd->m_CurrentType.type == TCPROPERTY_CTRL_LIST)
    {
    	CTCListBoxItem dlg;
        dlg.m_ctrlID = m_CProperWnd->m_CurrentType.name;
    	dlg.DoModal();
    }
	 else if(m_CProperWnd->m_CurrentType.type == TCPROPERTY_CTRL_TABLE)
    {
    	CTCTableProperty dlg;
		dlg.m_ctrlID = m_CProperWnd->m_CurrentType.name;
		dlg.DoModal();
    }
	 else if(m_CProperWnd->m_CurrentType.type == TCPROPERTY_CTRL_HOTKEY)
    {
    	CTCHotKeySetting dlg;
		dlg.m_ctrlID = m_CProperWnd->m_CurrentType.name;
		dlg.DoModal();
    }
	else if(m_CProperWnd->m_CurrentType.type == TCPROPERTY_CTRL_GRIDCTRL)
	{
		CTCGridItems dlg;
		dlg.m_ctrlID = m_CProperWnd->m_CurrentType.name;
		dlg.DoModal();
	}
}

