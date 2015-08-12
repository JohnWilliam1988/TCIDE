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
// CResourceViewBar 消息处理程序

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

    // 创建组合:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_VSCROLL | WS_CLIPCHILDREN;

    if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
    {
        TRACE0("未能创建属性组合 \n");
        return -1;      // 未能创建
    }

    //m_wndObjectCombo.AddString(_T("设计面板"));
    //m_wndObjectCombo.AddString(_T("属性窗口"));
    m_wndObjectCombo.SetCurSel(0);

    if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
    {
        TRACE0("未能创建属性网格\n");
        return -1;      // 未能创建
    }

    InitPropList();

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
    m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // 所有命令将通过此控件路由，而不是通过主框架路由:
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
}

///////////////////////////添加控件名称到属性窗口下拉框///////////////
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

	if(_T("设计面板") == selectName || _T("属性窗口") == selectName )
	{
		::AfxMessageBox(_T("激活窗口"));
	}
	else 
	{
		//控件类型
		TCTOOLTYPE ctrlType = TCTT_MAX;
		CControlRect *pCtrlRect = NULL;

		//遍历所有控件的ID与选择的项对比
		int size = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll.GetSize();
		for(int i = 0;i < size ; i++)
		{
			if(pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
			{
				CString idTab = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();
				//如果是Tab的ID
				if( idTab == selectName )
				{
					pCtrlRect = pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(idTab);
					//先在这儿Deselect掉Tab中的
					DeSelectCtrlsInTab();
					//还有不是Tab的
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
							//Tab里面元素的ID
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
					//先在这儿Deselect掉Tab中的
					DeSelectCtrlsInTab();
					pCurrent->m_DiaDesign.m_ControlTracker.SetCreateCtrlSelected(pCtrlRect);
					ctrlType = pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType();
				}
			}
		}
		//刷新一下界面
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
	//清空下拉框
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
				//Tab的ID
				m_wndObjectCombo.AddString(idTab);
				CTCTab* pTab = (CTCTab*)pCurrent->m_DiaDesign.m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
				for(int j=0;j<pTab->getTabPageCount();j++)
				{
					int pageIndex=j;
					int pageSize=((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					for(int k = 0;k < pageSize; k++)
					{
						CString idInTab = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlID();
						//Tab里面元素的ID
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
    // TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
    // TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::OnProperties2()
{
    // TODO: 在此处添加命令处理程序代码
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
    // TODO: 在此处添加命令更新 UI 处理程序代码
}

void CPropertiesWnd::InitPropList()
{
    SetPropListFont();

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea();
    m_wndPropList.SetVSDotNetLook();
    m_wndPropList.MarkModifiedProperties();

    return;

    CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("外观"));

    pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("三维外观"), (_variant_t) false, _T("指定窗口的字体不使用粗体，并且控件将使用三维边框")));

    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("边框"), _T("对话框外框"), _T("其中之一:“无”、“细”、“可调整大小”或“对话框外框”"));
    pProp->AddOption(_T("无"));
    pProp->AddOption(_T("细"));
    pProp->AddOption(_T("可调整大小"));
    pProp->AddOption(_T("对话框外框"));
    pProp->AllowEdit(FALSE);

    pGroup1->AddSubItem(pProp);
    pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("标题"), (_variant_t) _T("关于"), _T("指定窗口标题栏中显示的文本")));

    m_wndPropList.AddProperty(pGroup1);

    CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("窗口大小"), 0, TRUE);

    pProp = new CMFCPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定窗口的高度"));
    pProp->EnableSpinControl(TRUE, 50, 300);
    pSize->AddSubItem(pProp);

    pProp = new CMFCPropertyGridProperty( _T("宽度"), (_variant_t) 150l, _T("指定窗口的宽度"));
    pProp->EnableSpinControl(TRUE, 50, 200);
    pSize->AddSubItem(pProp);

    m_wndPropList.AddProperty(pSize);

    CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("字体"));

    LOGFONT lf;
    CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    font->GetLogFont(&lf);

    lstrcpy(lf.lfFaceName, _T("宋体, Arial"));

    pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定窗口的默认字体")));
    pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定窗口使用“MS Shell Dlg”字体")));

    m_wndPropList.AddProperty(pGroup2);

    CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("杂项"));
    pProp = new CMFCPropertyGridProperty(_T("(名称)"), _T("应用程序"));
    pProp->Enable(FALSE);
    pGroup3->AddSubItem(pProp);
    CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("窗口颜色"), RGB(210, 192, 254), NULL, _T("指定默认的窗口颜色"));
    pColorProp->EnableOtherButton(_T("其他..."));
    pColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
    pGroup3->AddSubItem(pColorProp);
    static const TCHAR szFilter[] = _T("图标文件(*.ico)|*.ico|所有文件(*.*)|*.*||");
    pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("图标"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定窗口图标")));
    pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("文件夹"), _T("c:\\")));
    m_wndPropList.AddProperty(pGroup3);
    CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("层次结构"));
    CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("第一个子级"));
    pGroup4->AddSubItem(pGroup41);
    CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("第二个子级"));
    pGroup41->AddSubItem(pGroup411);
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 1"), (_variant_t) _T("值 1"), _T("此为说明")));
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 2"), (_variant_t) _T("值 2"), _T("此为说明")));
    pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("项 3"), (_variant_t) _T("值 3"), _T("此为说明")));
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

    CMFCPropertyGridProperty* Basic = new CMFCPropertyGridProperty(_T("基本"));

    CMFCPropertyGridProperty* FileName = new CMFCPropertyGridProperty(_T("文件名"), (_variant_t)name, _T("当前源文件的名称"));
    FileName->SetData(DWORD_PTR(_T("文件名")));
    Basic->AddSubItem(FileName);

    CMFCPropertyGridProperty* FilePath = new CMFCPropertyGridProperty(_T("完整路径"),path, _T("文件的完整路径"));
    FilePath->Enable(FALSE);
    FilePath->SetData(DWORD_PTR(_T("完整路径")));
    Basic->AddSubItem(FilePath);
    m_wndPropList.AddProperty(Basic);

    CMFCPropertyGridProperty* proj = new CMFCPropertyGridProperty(_T("项目"));

    CMFCPropertyGridProperty* enterFile = new CMFCPropertyGridProperty(_T("启动文件"),CTCRunTimeManager::g_projEnterFileName, _T("项目的启动文件"));
    CMFCPropertyGridProperty* projName = new CMFCPropertyGridProperty(_T("项目名称"),CTCRunTimeManager::g_ProjName, _T("当前项目的名称"));
    enterFile->AllowEdit(FALSE);
    projName->AllowEdit(FALSE);
    enterFile->SetData(DWORD_PTR(_T("启动文件")));
    projName->SetData(DWORD_PTR(_T("项目名称")));
    proj->AddSubItem(enterFile);
    proj->AddSubItem(projName);
    m_wndPropList.AddProperty(proj);

	//项目VIP属性
	CMFCPropertyGridProperty* projInfo = new CMFCPropertyGridProperty(_T("项目信息"));

	CMFCPropertyGridProperty* productName = new CMFCPropertyGridProperty(_T("产品名称"),CTCExeVersionManager::m_productName, _T("当前项目生成项目的名称"));
	CMFCPropertyGridProperty* fileDesCri = new CMFCPropertyGridProperty(_T("文件描述"),CTCExeVersionManager::m_verDesInfo, _T("当前项目生成文件的文件描述"));
	CMFCPropertyGridProperty* fileInfo = new CMFCPropertyGridProperty(_T("文件版本"),CTCExeVersionManager::m_fileInfo, _T("当前项目生成文件的文件版本信息"));
	CMFCPropertyGridProperty* productInfo = new CMFCPropertyGridProperty(_T("产品版本"),CTCExeVersionManager::m_productInfo, _T("当前项目的生成文件的产品版本信息"));
	CMFCPropertyGridProperty* internalName = new CMFCPropertyGridProperty(_T("内部名称"),CTCExeVersionManager::m_internalName, _T("当前项目生成文件的内部名称"));
	CMFCPropertyGridProperty* companyName = new CMFCPropertyGridProperty(_T("公司名称"),CTCExeVersionManager::m_companyName, _T("当前项目生成文件的公司名称"));
	CMFCPropertyGridProperty* legalCopyright = new CMFCPropertyGridProperty(_T("版权信息"),CTCExeVersionManager::m_leagalInfo, _T("当前项目生成文件的文件的版权信息"));


	productName->AllowEdit(TRUE);
	fileDesCri->AllowEdit(TRUE);
	fileInfo->AllowEdit(TRUE);
	productInfo->AllowEdit(TRUE);
	internalName->AllowEdit(TRUE);
	companyName->AllowEdit(TRUE);
	legalCopyright->AllowEdit(TRUE);



	productName->SetData(DWORD_PTR(_T("产品名称")));
	fileDesCri->SetData(DWORD_PTR(_T("文件描述")));
	fileInfo->SetData(DWORD_PTR(_T("文件版本号")));
	productInfo->SetData(DWORD_PTR(_T("产品版本号")));
	internalName->SetData(DWORD_PTR(_T("内部名称")));
	companyName->SetData(DWORD_PTR(_T("公司名称")));
	legalCopyright->SetData(DWORD_PTR(_T("版权信息")));

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

	CMFCPropertyGridProperty* dlgBase = new CMFCPropertyGridProperty(_T("杂项"));
	CMFCPropertyGridProperty* dlgName = new CMFCPropertyGridProperty(_T("<名称>"), (_variant_t)_T("窗口节点"), _T("当前窗口节点"));
	CMFCPropertyGridProperty* dlgID = new CMFCPropertyGridProperty(_T("窗口名称"), (_variant_t)name, _T("当前节点所代表窗口的名称"));
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
    CMFCPropertyGridProperty* dlgBase = new CMFCPropertyGridProperty(_T("基本"));
	CMFCPropertyGridProperty* dlgID = new CMFCPropertyGridProperty(_T("窗口名称"), (_variant_t)dlg->m_TcDlgConfig.m_dlgID, _T("当前窗口的名称"));
    CMFCPropertyGridProperty* dlgTitle = new CMFCPropertyGridProperty(_T("窗口标题"), (_variant_t)csTitle, _T("当前的窗口标题"));
    dlgBase->AddSubItem(dlgID);
	dlgBase->AddSubItem(dlgTitle);
    m_wndPropList.AddProperty(dlgBase);

    CMFCPropertyGridProperty* dlgLook = new CMFCPropertyGridProperty(_T("外观"));
    CMFCPropertyGridProperty* dlgWidth = new CMFCPropertyGridProperty(_T("窗口宽度"), (_variant_t)(rc.right-rc.left), _T("当前窗口的宽度"));
    CMFCPropertyGridProperty* dlgHeight = new CMFCPropertyGridProperty(_T("窗口高度"), (_variant_t)(rc.bottom-rc.top), _T("当前窗口的高度"));
	CCustomDlgProp* dlgIcon = new CCustomDlgProp(_T("窗口图标"),(_variant_t)dlg->m_TcDlgConfig.m_icon,this);
    //dlgIcon->Enable(false);
    //CComboBoxExProp* dlgBkgrBitmap = new CComboBoxExProp(_T("窗口背景图"), (_variant_t)dlg->m_TcDlgConfig.m_BkgrBitmap, _T("当前的窗口背景图"),NULL,this);
    //dlgBkgrBitmap->AddOption(_T(""));

	CMFCPropertyGridProperty* ctlMin = new CMFCPropertyGridProperty(_T("最小化"),(_variant_t)dlg->m_TcDlgConfig.m_minBtn, _T("当前面板设置最小化按钮"));
	CMFCPropertyGridProperty* ctlMax = new CMFCPropertyGridProperty(_T("最大化"),(_variant_t)dlg->m_TcDlgConfig.m_maxBtn, _T("当前面板设置最大化按钮"));


	dlgIcon->Enable(true);

    //加载资源文件里面的图的列表
    CStringArray bmpArray;
    CTCRunTimeManager::getRCPathBmpList(bmpArray);
    /*int count=bmpArray.GetCount();
    for(int i=0;i<count;i++)
    {
        dlgBkgrBitmap->AddOption(bmpArray.GetAt(i));
    }*/
    CMFCPropertyGridColorProperty* dlgColorProp = new CMFCPropertyGridColorProperty(_T("窗口背景颜色"), dlg->m_TcDlgConfig.m_BkgrColor, NULL, _T("指定默认的窗口背景颜色"));
    dlgColorProp->EnableOtherButton(_T("其他..."));
    dlgColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_BTNFACE));

    dlgTitle->SetData(DWORD_PTR(_T("窗口标题")));
    dlgWidth->SetData(DWORD_PTR(_T("窗口宽度")));
    dlgHeight->SetData(DWORD_PTR(_T("窗口高度")));
    dlgIcon->SetData(DWORD_PTR(_T("窗口图标")));
    //dlgBkgrBitmap->SetData(DWORD_PTR(_T("窗口背景图")));
    dlgColorProp->SetData(DWORD_PTR(_T("窗口背景颜色")));
	ctlMin->SetData(DWORD_PTR(_T("窗口最小化按钮")));
	ctlMax->SetData(DWORD_PTR(_T("窗口最大化按钮")));

    dlgLook->AddSubItem(dlgWidth);
    dlgLook->AddSubItem(dlgHeight);
    dlgLook->AddSubItem(dlgIcon);
    //dlgLook->AddSubItem(dlgBkgrBitmap);
    dlgLook->AddSubItem(dlgColorProp);
	dlgLook->AddSubItem(ctlMin);
	dlgLook->AddSubItem(ctlMax);

    m_wndPropList.AddProperty(dlgLook);

    CMFCPropertyGridProperty* dlgAction = new CMFCPropertyGridProperty(_T("事件"));
    CMFCPropertyGridProperty* dlgInit = new CMFCPropertyGridProperty(_T("窗口初始化"), (_variant_t)dlg->m_TcDlgConfig.m_eDiaInit, _T("窗口初始化的时候执行的操作"));
    CMFCPropertyGridProperty* dlgDestory = new CMFCPropertyGridProperty(_T("窗口销毁"), (_variant_t)dlg->m_TcDlgConfig.m_eDestory, _T("窗口销毁的时候执行的操作"));
	CMFCPropertyGridProperty* dlgCloseWindow = new CMFCPropertyGridProperty(_T("窗口关闭"), (_variant_t)dlg->m_TcDlgConfig.m_eClose, _T("窗口关闭的时候执行的操作"));
	CMFCPropertyGridProperty* dlgPreTrans = new CMFCPropertyGridProperty(_T("消息路由"), (_variant_t)dlg->m_TcDlgConfig.m_ePreTrans, _T("窗口的消息路由"));
	CMFCPropertyGridProperty* dlgWinPorc = new CMFCPropertyGridProperty(_T("消息过程"), (_variant_t)dlg->m_TcDlgConfig.m_eWinProc, _T("窗口的消息过程"));

    if(CTCRunTimeManager::g_LangType==0)
    {
        CString eDiaInit,eDestory,eClose,ePreTrans,eWinProc;
        eDiaInit.Format(_T("<添加>%s.初始化"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
        eDestory.Format(_T("<添加>%s.销毁"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		eClose.Format(_T("<添加>%s.关闭"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		ePreTrans.Format(_T("<添加>%s.消息路由"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		eWinProc.Format(_T("<添加>%s.消息过程"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);

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
		eClose.Format(_T("<添加>%s.close"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		ePreTrans.Format(_T("<添加>%s.pretranslatemessage"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
		eWinProc.Format(_T("<添加>%s.windowproc"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);

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

    dlgInit->SetData(DWORD_PTR(_T("窗口初始化")));
    dlgDestory->SetData(DWORD_PTR(_T("窗口销毁")));
	dlgCloseWindow->SetData(DWORD_PTR(_T("窗口关闭")));
	dlgPreTrans->SetData(DWORD_PTR(_T("窗口消息路由")));
	dlgWinPorc->SetData(DWORD_PTR(_T("窗口消息过程")));
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

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("标签的名称"));
    CMFCPropertyGridProperty* ctlText = new CMFCPropertyGridProperty(_T("文本"), (_variant_t)csText, _T("标签显示的文本内容"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(ctlText);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("外观"));
    CMFCPropertyGridColorProperty* cltBKColorProp = new CMFCPropertyGridColorProperty(_T("标签背景颜色"), ctl->GetBackground(), NULL, _T("指定默认的标签背景颜色"));
    cltBKColorProp->EnableOtherButton(_T("其他..."));
    cltBKColorProp->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_BTNFACE));

    //CString csDir;
    //ctl->GetStyle();
    //CMFCPropertyGridProperty* dlgDir = new CMFCPropertyGridProperty(_T("对齐"), (_variant_t)csText, _T("标签文本对齐的方向"));
    ctlLook->AddSubItem(cltBKColorProp);
    m_wndPropList.AddProperty(ctlLook);

    CMFCPropertyGridProperty* pGroupFont = new CMFCPropertyGridProperty(_T("字体"));

    LOGFONT lf;
    CFont* font = ctl->GetFont();//CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    font->GetLogFont(&lf);

	CString faceName = ctl->GetFontName();
	

    lstrcpy(lf.lfFaceName, faceName/*_T("宋体, Arial")*/);
    pGroupFont->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定标签的默认字体")));
    pGroupFont->AddSubItem(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定标签使用“MS Shell Dlg”字体")));
    m_wndPropList.AddProperty(pGroupFont);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("标签的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("标签的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("标签的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("标签的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* dlgClick = new CComboBoxExProp(_T("点击"), (_variant_t)ctl->TCGetClickEvent(), _T("标签点击时操作的功能"),0,this);
    
	dlgClick->AddOption(_T(""));
    dlgClick->AllowEdit(false);
	 //if(CTCRunTimeManager::g_LangType==0)
  //  {
  //      CString eClick;
  //      eClick.Format(_T("<添加>%s.点击"),CTCRunTimeManager::g_ProjName);
  //      dlgClick->AddOption(eClick);
  //      dlgClick->AllowEdit(false);
  //  }
  //  else if(CTCRunTimeManager::g_LangType==1)
  //  {
  //      CString eClick;
  //      eClick.Format(_T("<添加>%s.click"),CTCRunTimeManager::g_ProjName);
  //      dlgClick->AddOption(eClick);
  //      dlgClick->AllowEdit(false);
  //  }

  //  dlgClick->SetData(DWORD_PTR(_T("点击")));

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

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("分组框的名称"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("标题"), (_variant_t)csText, _T("分组框显示的标题内容"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("外观"));

    CTCGroupBox::TCGroupBoxStyle style;
    CString csStyle;
    style=ctl->GetGroupBoxStyle();
    if(style==CTCGroupBox::TCGB_FRAME)
        csStyle=_T("边框风格");
    else if(style==CTCGroupBox::TCGB_WINDOW)
        csStyle=_T("窗口风格");
    else
        csStyle=_T("边框风格");

    CMFCPropertyGridProperty* groupBoxStyle = new CMFCPropertyGridProperty(_T("窗口风格"), (_variant_t)csStyle, _T("分组框显示的风格"));
    groupBoxStyle->AddOption(_T("边框风格"));
    groupBoxStyle->AddOption(_T("窗口风格"));
    groupBoxStyle->AllowEdit(false);

    //   CMFCPropertyGridColorProperty* titleTextColorProp = new CMFCPropertyGridColorProperty(_T("标题文本颜色"), ctl->GetTitleTextColor(), NULL, _T("指定默认的标签文本颜色"));
    //titleTextColorProp->EnableOtherButton(_T("其他..."));
    //   titleTextColorProp->EnableAutomaticButton(_T("默认"),ctl->GetTitleTextColor());

    CMFCPropertyGridColorProperty* titleBKColorProp = new CMFCPropertyGridColorProperty(_T("标题背景颜色"), ctl->GetTitleBackgroundColor(), NULL, _T("指定默认的分组框标题背景颜色"));
    titleBKColorProp->EnableOtherButton(_T("其他..."));
    titleBKColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_BTNFACE)/*ctl->GetTitleBackgroundColor()*/);

    CMFCPropertyGridColorProperty* borderColorProp = new CMFCPropertyGridColorProperty(_T("边框颜色"), ctl->GetBorderColor(), NULL, _T("指定默认的边框颜色"));
    borderColorProp->EnableOtherButton(_T("其他..."));
    borderColorProp->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DSHADOW)/*ctl->GetBorderColor()*/);

    CMFCPropertyGridColorProperty* clientBKColorProp = new CMFCPropertyGridColorProperty(_T("背景颜色"), ctl->GetClientBackgroundColor(), NULL, _T("指定默认的分组框背景颜色"));
    clientBKColorProp->EnableOtherButton(_T("其他..."));
    clientBKColorProp->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_BTNFACE));

    ctlLook->AddSubItem(groupBoxStyle);
    //ctlLook->AddSubItem(titleTextColorProp);
    ctlLook->AddSubItem(titleBKColorProp);
    ctlLook->AddSubItem(borderColorProp);
    ctlLook->AddSubItem(clientBKColorProp);
    m_wndPropList.AddProperty(ctlLook);

    CMFCPropertyGridProperty* pGroupFont = new CMFCPropertyGridProperty(_T("字体"));

	LOGFONT lf = ctl->GetGroupFont();
	//CFont* font = ctl->GetFont();/*CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));*/
 //   font->GetLogFont(&lf);

	CString fontName;
	ctl->GetFontFaceName(fontName);
	lstrcpy(lf.lfFaceName,fontName/* _T("宋体, Arial")*/);
    pGroupFont->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定控件的默认字体")));
    pGroupFont->AddSubItem(new CMFCPropertyGridProperty(_T("使用系统字体"), (_variant_t) true, _T("指定控件使用“MS Shell Dlg”字体")));
    m_wndPropList.AddProperty(pGroupFont);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("分组框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("分组框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("分组框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("分组框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    DWORD dir;
    CString csDir;
    dir=ctl->GetAlignment();

    if(dir==SS_LEFT)
        csDir=_T("左对齐");
    else if(dir==SS_CENTER)
        csDir=_T("中间对齐");
    else if(dir==SS_RIGHT)
        csDir=_T("右对齐");
    else 
        csDir=_T("默认");

    CMFCPropertyGridProperty* ctlDir = new CMFCPropertyGridProperty(_T("对齐"), (_variant_t)csDir, _T("分组框文本对齐的方向"));
    ctlDir->AddOption(_T("左对齐"));
    ctlDir->AddOption(_T("中间对齐"));
    ctlDir->AddOption(_T("右对齐"));
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

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("按钮的名称"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("文本"), (_variant_t)csText, _T("按钮显示的标题内容"));
	CMFCPropertyGridProperty* defButton = new CMFCPropertyGridProperty(_T("默认按钮"),(_variant_t)ctl->TCGetDefButton(), _T("当前按钮是否默认按钮"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(defButton);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("按钮的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("按钮的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("按钮的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("按钮的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	
	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("其他"));
	CMFCPropertyGridProperty* mulityLine = new CMFCPropertyGridProperty(_T("多行"), (_variant_t)ctl->TCGetMulityLine(), _T("按钮显示的标题内容多行"));
	ctlOther->AddSubItem(mulityLine);
	m_wndPropList.AddProperty(ctlOther);


    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* ctlClick = new CComboBoxExProp(_T("点击"), (_variant_t)ctl->TCGetClickEvent(), _T("按钮的点击时操作的功能"),0,this);
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

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("编辑框的名称"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("文本"), (_variant_t)csText, _T("编辑框显示的内容"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("外观"));

    CMFCPropertyGridColorProperty* cltTextColorProp = new CMFCPropertyGridColorProperty(_T("文本颜色"), ctl->GetTextColor(), NULL, _T("指定默认的编辑框文本颜色"));
    cltTextColorProp->EnableOtherButton(_T("其他..."));
    cltTextColorProp->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_WINDOWTEXT));

    CMFCPropertyGridColorProperty* cltBKColorProp = new CMFCPropertyGridColorProperty(_T("背景颜色"), ctl->GetBackColor(), NULL, _T("指定默认的文本框背景颜色"));
    cltBKColorProp->EnableOtherButton(_T("其他..."));
    cltBKColorProp->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_WINDOW));

    ctlLook->AddSubItem(cltTextColorProp);
    ctlLook->AddSubItem(cltBKColorProp);
    m_wndPropList.AddProperty(ctlLook);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("编辑框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("编辑框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("编辑框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("编辑框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	 ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

    CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("其他"));
	CMFCPropertyGridProperty* ctlMulLine = new CMFCPropertyGridProperty(_T("多行"), (_variant_t)ctl->TCGetMulityLine(), _T("编辑框支持多行输入属性"));
    CMFCPropertyGridProperty* ctlCheck = new CMFCPropertyGridProperty(_T("密文"), (_variant_t)ctl->TCGetPassWord(), _T("编辑框密文属性"));
	CMFCPropertyGridProperty* ctlReadOnly = new CMFCPropertyGridProperty(_T("只读"), (_variant_t)ctl->TCGetReadOnly(), _T("编辑框只读属性"));
	CMFCPropertyGridProperty* ctlNumber = new CMFCPropertyGridProperty(_T("数字"), (_variant_t)ctl->TCGetNumber(), _T("编辑框输入数字属性"));
	CMFCPropertyGridProperty* ctlVerti = new CMFCPropertyGridProperty(_T("垂直滚动条"), (_variant_t)ctl->TCGetVerti(), _T("编辑框添加垂直滚动条"));
	CMFCPropertyGridProperty* ctlHoriz = new CMFCPropertyGridProperty(_T("水平滚动条"), (_variant_t)ctl->TCGetHoriz(), _T("编辑框添加水平滚动条"));
	ctlVerti->SetData(DWORD_PTR(_T("编辑框垂直滚动条")));
	ctlHoriz->SetData(DWORD_PTR(_T("编辑框水平滚动条")));
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


    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* ctlChange = new CComboBoxExProp(_T("内容改变"), (_variant_t)ctl->TCGetEnChangeEvent(), _T("编辑框内容改变时操作的功能"),0,this);
	CComboBoxExProp* ctlSetFocus = new CComboBoxExProp(_T("获得焦点"), (_variant_t)ctl->TCGetSetFocusEvent(), _T("编辑框获得焦点时操作的功能"),0,this);
    CComboBoxExProp* ctlKF = new CComboBoxExProp(_T("失去焦点"), (_variant_t)ctl->TCGetKillfocusEvent(), _T("编辑框失去焦点时操作的功能"),0,this);
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

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("单选框的名称"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("标题"), (_variant_t)csText, _T("单选框标题显示的内容"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
	CMFCPropertyGridProperty* groupCtrl = new CMFCPropertyGridProperty(_T("分组"), (_variant_t)ctl->TCGetGroup(), _T("单选框分组"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(ableCtrl);
	ctlBase->AddSubItem(groupCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("单选框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("单选框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("单选框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("单选框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);


    CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("其他"));
    bool isCheck = ctl->TCGetCheck();
	bool isLeft = ctl->TCGetTextLeft();
    CMFCPropertyGridProperty* ctlCheck = new CMFCPropertyGridProperty(_T("选中"), (_variant_t)isCheck, _T(""));
	CMFCPropertyGridProperty* leftText = new CMFCPropertyGridProperty(_T("标题居左"), (_variant_t)isLeft, _T("单选框标题显示内容是否居左"));
    ctlOther->AddSubItem(ctlCheck);
	ctlOther->AddSubItem(leftText);
    m_wndPropList.AddProperty(ctlOther);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* selChange = new CComboBoxExProp(_T("点击"), (_variant_t)ctl->TCGetRadioEvent(), _T("单选框单击时操作的功能"),0,this);
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

    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("复选框的名称"));
    CMFCPropertyGridProperty* titleText = new CMFCPropertyGridProperty(_T("标题"), (_variant_t)csText, _T("复选框标题显示的内容"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(titleText);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("复选框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("复选框Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("复选框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("复选框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

    CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("其他"));
    bool isCheck = ctl->TCGetCheck();
	bool isLeft = ctl->TCGetTextLeft();
    CMFCPropertyGridProperty* ctlCheck = new CMFCPropertyGridProperty(_T("选中"),(_variant_t)isCheck, _T("单选框是否默认选中"));
	CMFCPropertyGridProperty* leftText = new CMFCPropertyGridProperty(_T("标题居左"),(_variant_t)isLeft,_T("单选框标题显示内容是否居左"));
    ctlOther->AddSubItem(ctlCheck);	
	ctlOther->AddSubItem(leftText);
    m_wndPropList.AddProperty(ctlOther);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* selChange = new CComboBoxExProp(_T("点击"), (_variant_t)ctl->TCGetCheckEvent(), _T("复选框单击时操作的功能"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("图片框的名称"));
	CComboBoxExProp* ctlPath = new CComboBoxExProp(_T("图片名称"), (_variant_t)ctl->GetName(), _T("图片框要显示图片的名称"),0,this);
	ctlBase->AddSubItem(ctlID);
	ctlPath->AddOption(_T(""));
	ctlBase->AddSubItem(ctlPath);
    m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("图片框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("图片框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("图片框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("图片框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
	CComboBoxExProp* click = new CComboBoxExProp(_T("左键单击"), (_variant_t)ctl->TCGetClickEvent(), _T("左键单击图片框时操作的功能"),0,this);
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


    CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("下拉框的名称"));
    CCustomDlgProp* addItem = new CCustomDlgProp(_T("选项"),(_variant_t)(_T("<编辑下拉选项>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
    ctlBase->AddSubItem(ctlID);
    ctlBase->AddSubItem(addItem);
	ctlBase->AddSubItem(ableCtrl);
    m_wndPropList.AddProperty(ctlBase);

    CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("下拉框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("下拉框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("下拉框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("下拉框的高度"));
	//下拉框的高度是一定的
	ctlHeight->AllowEdit(false);
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);


    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* click = new CComboBoxExProp(_T("左键按下"), (_variant_t)ctl->GetLeftDownEvent(), _T("左键按下下拉框时操作的功能"),0,this);
    CComboBoxExProp* selChange = new CComboBoxExProp(_T("选择改变"), (_variant_t)ctl->GetEnChangeEvent(), _T("下拉框选择改变时操作的功能"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("进度条的名称"));
	ctlBase->AddSubItem(ctlID);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("进度条的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("进度条的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("进度条的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("进度条的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("其他"));
	CMFCPropertyGridProperty* vertical = new CMFCPropertyGridProperty(_T("垂直"), (_variant_t)ctl->TCGetVertical(), _T("进度条的方向是垂直还是水平"));
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("路径框的名称"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
	CMFCEditBrowseCtrl::BrowseMode m_browmode = ctl->GetMode();
	CString strMode = _T("");
	if( m_browmode == CMFCEditBrowseCtrl::BrowseMode_File)
		strMode = _T("文件");
	else if( m_browmode == CMFCEditBrowseCtrl::BrowseMode_Folder )
		strMode = _T("文件夹");
	CMFCPropertyGridProperty* PahtStyle = new CMFCPropertyGridProperty(_T("浏览模式"), (_variant_t)strMode, _T("路径框浏览的模式"));
    PahtStyle->AddOption(_T("文件"));
    PahtStyle->AddOption(_T("文件夹"));
	//不允许用户输入，只能选择
	PahtStyle->AllowEdit(FALSE);
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(PahtStyle);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("路径框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("路径框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("路径框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("路径框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* enChange = new CComboBoxExProp(_T("选择改变"), (_variant_t)ctl->TCGetEnChangeEvent(), _T("路径框选择改变时操作的功能"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("滚动条的名称"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("滚动条的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("滚动条的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("滚动条的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("滚动条的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);
    
    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("其他"));
	CMFCPropertyGridProperty* vertical = new CMFCPropertyGridProperty(_T("垂直"), (_variant_t)ctl->TCGetVertical(), _T("滚动条的方向是垂直还是水平"));
	ctlOther->AddSubItem(vertical);
	m_wndPropList.AddProperty(ctlOther);

    CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
    CComboBoxExProp* enChange = new CComboBoxExProp(_T("鼠标释放"), (_variant_t)ctl->TCGetSliderChangeEvent(), _T("滚动条鼠标释放时操作的功能"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("日期框的名称"));
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("日期框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("日期框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("日期框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("日期框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("列表框的名称"));
	CMFCPropertyGridProperty* ctlTitle = new CMFCPropertyGridProperty(_T("标题"), (_variant_t)csText, _T("列表框的标题"));
	CCustomDlgProp* addItem = new CCustomDlgProp(_T("选项"),(_variant_t)(_T("<编辑下拉选项>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
	/*CMFCPropertyGridProperty* buttonNew = new CMFCPropertyGridProperty(_T("新建"), (_variant_t)isNew, _T("是否添加新建按钮"));
	CMFCPropertyGridProperty* buttonRemove = new CMFCPropertyGridProperty(_T("删除"), (_variant_t)isRemove, _T("是否添加删除按钮"));
	CMFCPropertyGridProperty* buttonUp = new CMFCPropertyGridProperty(_T("上移"), (_variant_t)isUp, _T("是否添加上移按钮"));
	CMFCPropertyGridProperty* buttonDown = new CMFCPropertyGridProperty(_T("下移"), (_variant_t)isDown, _T("是否添加下移按钮"));*/
    ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlTitle);
	ctlBase->AddSubItem(addItem);
	ctlBase->AddSubItem(ableCtrl);
	/*ctlBase->AddSubItem(buttonNew);
	ctlBase->AddSubItem(buttonRemove);
	ctlBase->AddSubItem(buttonUp);
	ctlBase->AddSubItem(buttonDown);*/
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("列表框的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("列表框的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("列表框的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("列表框的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOther = new CMFCPropertyGridProperty(_T("其他"));
	CMFCPropertyGridProperty* defaultRoll = new CMFCPropertyGridProperty(_T("默认滚动"), (_variant_t)ctl->TCGetDefaultRool(), _T("添加新项时默认滚动"));
	ctlOther->AddSubItem(defaultRoll);
	m_wndPropList.AddProperty(ctlOther);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
	CComboBoxExProp* selChange = new CComboBoxExProp(_T("选择改变"), (_variant_t)ctl->TCGetListBoxSelChange(), _T("列表框选择改变时操作的功能"),0,this);
	CComboBoxExProp* clicked = new CComboBoxExProp(_T("左键双击"), (_variant_t)ctl->TCGetListBoxDoubleClick(), _T("列表框左键双击击时操作的功能"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
	CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("选项卡控件的名称"));
	CCustomDlgProp* tabItem = new CCustomDlgProp(_T("编辑选项"),(_variant_t)(_T("<编辑分页属性>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(tabItem);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
	CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("选项卡控件的X坐标"));
	CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("选项卡控件的Y坐标"));
	CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("选项卡控件的宽度"));
	CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("选项卡控件的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

	ctlPostion->AddSubItem(ctlLeft);
	ctlPostion->AddSubItem(ctlTop);
	ctlPostion->AddSubItem(ctlWidth);
	ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
	CComboBoxExProp* clock = new CComboBoxExProp(_T("选择改变"), (_variant_t)ctl->TCGetSelChange(), _T("选择改变时触发的事件"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("定时器的名称"));
	CMFCPropertyGridProperty* ctlTimer = new CMFCPropertyGridProperty(_T("频率"), (_variant_t)ctl->GetTimer(), _T("定时器的频率(毫秒),多久触发一次事件"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlTimer);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("定时器的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("定时器的Y坐标"));

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);

    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
	CComboBoxExProp* clock = new CComboBoxExProp(_T("时钟"), (_variant_t)ctl->TCGetTimeEvent(), _T("定时触发的事件"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
    CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("热键控件的名称"));
	CCustomDlgProp* hotKeyItem = new CCustomDlgProp(_T("热键键值"),(_variant_t)(_T("<指定热键键值>")),this);
	CMFCPropertyGridProperty* ableCtrl = new CMFCPropertyGridProperty(_T("有效"), (_variant_t)ctl->TCGetAbled(), _T("当前是否有效"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(hotKeyItem);
	ctlBase->AddSubItem(ableCtrl);
	m_wndPropList.AddProperty(ctlBase);


	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
    CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("热键控件的X坐标"));
    CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("热键控件的Y坐标"));
    CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("热键控件的宽度"));
    CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("热键控件的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

    ctlPostion->AddSubItem(ctlLeft);
    ctlPostion->AddSubItem(ctlTop);
    ctlPostion->AddSubItem(ctlWidth);
    ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
    m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
	CComboBoxExProp* clock = new CComboBoxExProp(_T("热键"), (_variant_t)ctl->TCGetHotKeyEvent(), _T("热键控件操作的功能"),0,this);
	CComboBoxExProp* change = new CComboBoxExProp(_T("失去焦点"), (_variant_t)ctl->TCGetKillFocusEvent(), _T("热键控件失去焦点时操作的功能"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
	CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("浏览器控件的名称"));
	CMFCPropertyGridProperty* ctlURL = new CMFCPropertyGridProperty(_T("URL"), (_variant_t)csText, _T("浏览器控件的默认网址"));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlURL);
	m_wndPropList.AddProperty(ctlBase);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
	CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("浏览器控件的X坐标"));
	CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("浏览器控件的Y坐标"));
	CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("浏览器控件的宽度"));
	CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("浏览器控件的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	//ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

	ctlPostion->AddSubItem(ctlLeft);
	ctlPostion->AddSubItem(ctlTop);
	ctlPostion->AddSubItem(ctlWidth);
	ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
	CComboBoxExProp* loaded = new CComboBoxExProp(_T("加载完成"), (_variant_t)ctl->TCGetloadComplete(), _T("浏览器控件加载完成操作的功能"),0,this);
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

	CMFCPropertyGridProperty* ctlBase = new CMFCPropertyGridProperty(_T("基本"));
	CMFCPropertyGridProperty* ctlID = new CMFCPropertyGridProperty(_T("名称"), (_variant_t)id, _T("表格控件的名称"));
	CCustomDlgProp* ctlEdit = new CCustomDlgProp(_T("编辑表格"),(_variant_t)(_T("<编辑表格选项>")),this);
	ctlEdit->SetData(DWORD_PTR(_T("编辑表格")));
	ctlBase->AddSubItem(ctlID);
	ctlBase->AddSubItem(ctlEdit);
	m_wndPropList.AddProperty(ctlBase);

	/*CMFCPropertyGridProperty* ctlMode = new CMFCPropertyGridProperty(_T("模式"));
	CMFCPropertyGridProperty* cltVirualMode = new CMFCPropertyGridProperty(_T("虚拟模式"),(_variant_t)(ctl->GetVirtualMode()? true : false), _T("设置表格控件为虚拟模式(设置虚拟模式后当前模式的属性会丢失)"));
	ctlMode->AddSubItem(cltVirualMode);
	m_wndPropList.AddProperty(ctlMode);*/

	CMFCPropertyGridProperty* ctlLook = new CMFCPropertyGridProperty(_T("外观"));
	CMFCPropertyGridColorProperty* cltBKColorProp = new CMFCPropertyGridColorProperty(_T("普通项背景颜色"), ctl->GetDefaultCell(FALSE,FALSE)->GetBackClr(), NULL, _T("指定默认的背景颜色"));
	CMFCPropertyGridColorProperty* cltTextColorProp = new CMFCPropertyGridColorProperty(_T("普通项文本颜色"), ctl->GetDefaultCell(FALSE,FALSE)->GetTextClr(), NULL, _T("指定默认的文本颜色"));
	CMFCPropertyGridColorProperty* cltFixedBKColorProp = new CMFCPropertyGridColorProperty(_T("固定项背景颜色"), ctl->GetFixedBkColor(), NULL, _T("指定固定项默认的背景颜色"));
	CMFCPropertyGridColorProperty* cltFixedTextColorProp = new CMFCPropertyGridColorProperty(_T("固定项文本颜色"), ctl->GetFixedTextColor(), NULL, _T("指定固定项默认的文本颜色"));

	CMFCPropertyGridProperty* ctlVerti = new CMFCPropertyGridProperty(_T("垂直网格"),(_variant_t)ctl->IsShowVertiLines(), _T("表格显示垂直网格"));
	CMFCPropertyGridProperty* ctlHoriz = new CMFCPropertyGridProperty(_T("水平网格"),(_variant_t)ctl->IsShowHorizLines(), _T("表格显示水平网格"));
	
	cltBKColorProp->EnableOtherButton(_T("其他..."));
	cltTextColorProp->EnableOtherButton(_T("其他..."));
	cltFixedBKColorProp->EnableOtherButton(_T("其他..."));
	cltFixedTextColorProp->EnableOtherButton(_T("其他..."));

	cltBKColorProp->EnableAutomaticButton(_T("默认"),RGB(0xFF,0xFF,0xE0));
	cltTextColorProp->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_WINDOWTEXT)/*ctl->GetDefaultCell(FALSE,FALSE)->GetTextClr()*/);
	cltFixedBKColorProp->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_BTNFACE)/*ctl->GetFixedBkColor()*/);
	cltFixedTextColorProp->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_WINDOWTEXT));

	ctlLook->AddSubItem(cltBKColorProp);
	ctlLook->AddSubItem(cltTextColorProp);
	ctlLook->AddSubItem(cltFixedBKColorProp);
	ctlLook->AddSubItem(cltFixedTextColorProp);
	ctlLook->AddSubItem(ctlVerti);
	ctlLook->AddSubItem(ctlHoriz);
	m_wndPropList.AddProperty(ctlLook);

	CMFCPropertyGridProperty* pGroupFont = new CMFCPropertyGridProperty(_T("字体"));

	LOGFONT lf;
	CFont* font = ctl->GetFont();//CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	pGroupFont->AddSubItem(new CMFCPropertyGridFontProperty(_T("字体"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定标签的默认字体")));
	m_wndPropList.AddProperty(pGroupFont);

	CMFCPropertyGridProperty* ctlPostion = new CMFCPropertyGridProperty(_T("位置"));
	CMFCPropertyGridProperty* ctlLeft = new CMFCPropertyGridProperty(_T("X坐标"), (_variant_t)(rc.left), _T("表格控件的X坐标"));
	CMFCPropertyGridProperty* ctlTop = new CMFCPropertyGridProperty(_T("Y坐标"), (_variant_t)(rc.top), _T("表格控件的Y坐标"));
	CMFCPropertyGridProperty* ctlWidth = new CMFCPropertyGridProperty(_T("宽度"), (_variant_t)(rc.right-rc.left), _T("表格控件的宽度"));
	CMFCPropertyGridProperty* ctlHeight = new CMFCPropertyGridProperty(_T("高度"), (_variant_t)(rc.bottom-rc.top), _T("表格控件的高度"));
	CMFCPropertyGridProperty* ctlShowOrder = new CMFCPropertyGridProperty(_T("显示顺序"),(_variant_t)order.InControlAllPos, _T("当前控件的显示顺序"));
	ctlShowOrder->AllowEdit(FALSE);
	ctlShowOrder->EnableSpinControl(TRUE,1,order.ControlSize);

	ctlPostion->AddSubItem(ctlLeft);
	ctlPostion->AddSubItem(ctlTop);
	ctlPostion->AddSubItem(ctlWidth);
	ctlPostion->AddSubItem(ctlHeight);
	ctlPostion->AddSubItem(ctlShowOrder);
	m_wndPropList.AddProperty(ctlPostion);

	CMFCPropertyGridProperty* ctlOthers = new CMFCPropertyGridProperty(_T("其他"));
	CMFCPropertyGridProperty* ctlEditAble = new CMFCPropertyGridProperty(_T("可编辑"),(_variant_t)(ctl->TCGetAllSellsEditAbled()? true : false), _T("表格是否可以编辑"));
	CMFCPropertyGridProperty* ctlHeadSort = new CMFCPropertyGridProperty(_T("排序"),(_variant_t)(ctl->GetHeaderSort()? true : false), _T("表格是否可以编辑"));
	CMFCPropertyGridProperty* ctlFillSell = new CMFCPropertyGridProperty(_T("填充"),(_variant_t)(ctl->TCGetExpandToFit()? true : false), _T("表格是否填充"));
	CMFCPropertyGridProperty* rowCount = new CMFCPropertyGridProperty(_T("行数"),(_variant_t)ctl->GetRowCount(), _T("表格控件的行数"));
	CMFCPropertyGridProperty* colCount = new CMFCPropertyGridProperty(_T("列数"),(_variant_t)ctl->GetColumnCount(), _T("表格控件的列数"));
	CMFCPropertyGridProperty* fixedRowCount = new CMFCPropertyGridProperty(_T("固定行数"),(_variant_t)ctl->GetFixedRowCount(), _T("表格控件的固定行数"));
	CMFCPropertyGridProperty* fixedColCount = new CMFCPropertyGridProperty(_T("固定列数"),(_variant_t)ctl->GetFixedColumnCount(), _T("表格控件的固定列数"));
	CMFCPropertyGridProperty* ctrlSelectAble = new CMFCPropertyGridProperty(_T("选中"),(_variant_t)(ctl->IsSelectable()? true : false), _T("表格项是否支持选中"));
	CMFCPropertyGridProperty* ctrlFixRowSelect = new CMFCPropertyGridProperty(_T("固定行选中"),(_variant_t)(ctl->GetFixedRowSelection()? true : false),_T("点击表格项固定行是否支持选中"),DWORD_PTR(_T("固定行选中")));
	CMFCPropertyGridProperty* ctrlFixColSelect = new CMFCPropertyGridProperty(_T("固定列选中"),(_variant_t)(ctl->GetFixedColumnSelection()? true : false), _T("点击表格项固定列是否支持选中"),DWORD_PTR(_T("固定列选中")));
	CMFCPropertyGridProperty* ctrlListMode = new CMFCPropertyGridProperty(_T("列表模式"),(_variant_t)(ctl->GetListMode()? true : false), _T("表格项支持列表模式选中"),DWORD_PTR(_T("列表模式选中")));
	
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

	CMFCPropertyGridProperty* ctlAction = new CMFCPropertyGridProperty(_T("事件"));
	CComboBoxExProp* leftClicked = new CComboBoxExProp(_T("左键单击"), (_variant_t)ctl->TCGetClickEvent(), _T("表格控件左键单击操作的功能"),0,this);
	CComboBoxExProp* beforeEdit = new CComboBoxExProp(_T("编辑开始"), (_variant_t)ctl->TCGetBeforeEditEvent(), _T("表格控件开始编辑操作的功能"),0,this);
	CComboBoxExProp* afterEdit = new CComboBoxExProp(_T("编辑结束"), (_variant_t)ctl->TCGetAfterEditEvent(), _T("表格控件结束编辑操作的功能"),0,this);
	CComboBoxExProp* headSort = new CComboBoxExProp(_T("点击排序"), (_variant_t)ctl->TCGetHeadSortEvent(), _T("表格控件点击排序"),0,this);
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
		//注意这儿是反过来的
		info.InControlAllPos = info.ControlSize - pCurrent->m_DiaDesign.m_ControlTracker.IsIncludeControl(pCtrl);
	}
}

LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM,LPARAM lParam) 
{
    CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;

    CString title = pProp->GetName();

    if(m_CurrentType.type == TCPROPERTY_PROJ)
    {
        if( _T("文件名") == title )
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

            //下面开始重名文件的操作
            if( newValue.Right(2) != _T(".t"))
            {
                pProp->SetValue(pProp->GetOriginalValue());		
                return 0;
            }
            else
            {
                if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->FileReNameByPropWnd(m_CurrentType.name,newValue))
                {
                    AfxMessageBox(_T("重命名失败"));
                    pProp->SetValue(pProp->GetOriginalValue());		
                    pProp->SetValue(oldValue);
                }
                else
                {
                    pProp->SetOriginalValue(pProp->GetValue());
                }		
            }
        }
		else if(title==_T("产品名称"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				CTCExeVersionManager::TCSetVersionProName(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("文件描述"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				 CTCExeVersionManager::TCSetVersionDes(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("文件版本"))
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
					AfxMessageBox(_T("请输入4位数字,中间用'.'隔开,例如:1.2.3.4"));
					pProp->SetValue(oldValue); 
				}
				else
					CTCExeVersionManager::TCSetVersionFile(newValue);
			}
			else
				pProp->SetValue(pProp->GetOriginalValue()); 
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("产品版本"))
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
					AfxMessageBox(_T("请输入4位数字,中间用'.'隔开，如:1.2.3.4"));
					pProp->SetValue(oldValue); 
				}
				else
					CTCExeVersionManager::TCSetVersionProduct(newValue);
			}
			else
				pProp->SetValue(pProp->GetOriginalValue()); 
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("内部名称"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				CTCExeVersionManager::TCSetVersionInternal(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("公司名称"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(!newValue.IsEmpty())
				CTCExeVersionManager::TCSetVersionCompany(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		else if(title==_T("版权信息"))
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
		if(title==_T("窗口名称"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();

			//项目主窗口不能重命名
			if(oldValue == CTCRunTimeManager::g_ProjName)
			{
				pProp->SetValue(pProp->GetOriginalValue());
				return 0;
			}

			if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.isGoodID(newValue))
			{
				//找到全局窗口管理类中的当前窗口节点，修改节点中的ID值
				int index  = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getIndexByID(oldValue);
				if(-1 == index)
					return 0;
				wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(index);
				wcscpy_s(temp->id,newValue);


				//如果当前视图是活动的，则修改Title
				CTCDesignView *pCurrent = (CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(oldValue);
				if(pCurrent != NULL)
				{
					pCurrent->m_TWinID = newValue;
					pCurrent->m_DiaDesign.m_TcDlgConfig.m_dlgID = newValue;
					((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID = newValue;
					pCurrent->GetDocument()->SetTitle(newValue);
				}
	
				//修改项目管理窗口中的对应节点
				if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->FileReNameByPropWnd(oldValue/*m_CurrentType.name*/,newValue,FALSE))
				{
					AfxMessageBox(_T("重命名节点失败"));
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

        //属性标题
        if(title==_T("窗口标题"))
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
		else if(title==_T("窗口名称"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();

			//项目主窗口不能重命名
			if(pCurrent->m_DiaDesign.m_TcDlgConfig.m_dlgID == CTCRunTimeManager::g_ProjName)
			{
				pProp->SetValue(pProp->GetOriginalValue());
				return 0;
			}
				
			if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.isGoodID(newValue))
			{
				////////////////找到全局窗口管理类中的当前窗口节点，修改节点中的ID值///////////////////
				int index  = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getIndexByID(oldValue);
				if(-1 == index)
					return 0;
				wndNode *temp = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(index);
				wcscpy_s(temp->id,newValue);
				////////////////找到全局窗口管理类中的当前窗口节点，修改节点中的ID值///////////////////

				pCurrent->m_TWinID = newValue;
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_dlgID = newValue;
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID = newValue;
				pCurrent->GetDocument()->SetTitle(newValue);
				//pDoc->SetTitle(newValue);		

				/////////////////修改项目管理窗口中的对应节点////////////////////////
				if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->FileReNameByPropWnd(oldValue/*m_CurrentType.name*/,newValue,FALSE))
				{
					AfxMessageBox(_T("重命名节点失败"));
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
        else if(title==_T("窗口宽度"))
        {
            int width=pProp->GetValue().intVal;
            RECT rc;
            pCurrent->m_DiaDesign.GetWindowRect(&rc);
            //pView->m_DiaDesign.ScreenToClient(&rc);
			pCurrent->SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LEFT,0),0);
            pCurrent->m_DiaDesign.MoveWindow(0,0,width,rc.bottom-rc.top);
            pProp->SetOriginalValue(pProp->GetValue());
        }
        else if(title==_T("窗口高度"))
        {
            int height=pProp->GetValue().intVal;
            RECT rc;
            pCurrent->m_DiaDesign.GetWindowRect(&rc);
           // pCurrent->m_DiaDesign.ClientToScreen(&rc);
			pCurrent->SendMessage(WM_VSCROLL,MAKEWPARAM(SB_TOP,0),0);
            pCurrent->m_DiaDesign.MoveWindow(0,0,rc.right-rc.left,height);
            pProp->SetOriginalValue(pProp->GetValue());
        }
		////// 窗口VIP属性///////////
		else if(title==_T("窗口图标"))
		{
			CString newValue=pProp->GetValue().bstrVal;
			if(newValue==_T(""))
				pCurrent->m_DiaDesign.TCSetIcon(_T(""));
			else
				pCurrent->m_DiaDesign.TCSetIcon(newValue);
			pProp->SetOriginalValue(pProp->GetValue());
		}
		////// 窗口VIP属性///////////
        else if(title==_T("窗口背景图"))
        {
            CString newValue=pProp->GetValue().bstrVal;
            if(newValue==_T("空"))
                pCurrent->m_DiaDesign.TCSetBackgroundImage(_T(""));
            else
                pCurrent->m_DiaDesign.TCSetBackgroundImage(newValue);
            pProp->SetOriginalValue(pProp->GetValue());
        }
        else if(title==_T("窗口背景颜色"))
        {
            CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
            COLORREF color=pColorProp->GetColor();
			if(color == 0xffffffff)
				color = ::GetSysColor(COLOR_BTNFACE);
            pCurrent->m_DiaDesign.TCSetBkgrColor(color);
            pProp->SetOriginalValue(pProp->GetValue());
        }
		else if(title==_T("最小化"))
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
		else if(title==_T("最大化"))
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
        else if(title==_T("窗口初始化"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            newValue.Trim();
            CString eDiaInit;
            if(CTCRunTimeManager::g_LangType==0)
            {
                eDiaInit.Format(_T("%s.初始化"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }
            else if(CTCRunTimeManager::g_LangType==1)
            {
                eDiaInit.Format(_T("%s.init"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }

            if(!newValue.IsEmpty())
            {
                //xxxxxxxxxxxxxx这里还要加打开T文件,并添加事件代码的操作
                pCurrent->m_DiaDesign.m_TcDlgConfig.m_eDiaInit=eDiaInit;
                pProp->SetOriginalValue(eDiaInit);
                pProp->SetValue(eDiaInit);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eDiaInit);
            }
        }
        else if(title==_T("窗口销毁"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            newValue.Trim();
            CString eDiaDestory;
            if(CTCRunTimeManager::g_LangType==0)
            {
                eDiaDestory.Format(_T("%s.销毁"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }
            else if(CTCRunTimeManager::g_LangType==1)
            {
                eDiaDestory.Format(_T("%s.destroy"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
            }

            if(!newValue.IsEmpty())
            {
                //xxxxxxxxxxxxxx这里还要加打开T文件,并添加事件代码的操作
                pCurrent->m_DiaDesign.m_TcDlgConfig.m_eDestory=eDiaDestory;
                pProp->SetOriginalValue(eDiaDestory);
                pProp->SetValue(eDiaDestory);
				//添加Dia事件代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eDiaDestory);
            }
        }
		else if(title==_T("窗口关闭"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eDiaClose;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaClose.Format(_T("%s.%s_关闭"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaClose.Format(_T("%s.%s_close"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			if(!newValue.IsEmpty())
			{
				//xxxxxxxxxxxxxx这里还要加打开T文件,并添加事件代码的操作
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_eClose=eDiaClose;
				pProp->SetOriginalValue(eDiaClose);
				pProp->SetValue(eDiaClose);
				//添加Dia事件代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eDiaClose,TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW);
			}
		}
		else if(title==_T("消息路由"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString ePreTrans;
			if(CTCRunTimeManager::g_LangType==0)
			{
				ePreTrans.Format(_T("%s.%s_消息路由"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				ePreTrans.Format(_T("%s.%s_pretranslatemessage"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			if(!newValue.IsEmpty())
			{
				//xxxxxxxxxxxxxx这里还要加打开T文件,并添加事件代码的操作
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_ePreTrans=ePreTrans;
				pProp->SetOriginalValue(ePreTrans);
				pProp->SetValue(ePreTrans);
				//添加Dia事件代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(ePreTrans,TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG);
			}
		}
		else if(title==_T("消息过程"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eWinPorc;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eWinPorc.Format(_T("%s.%s_消息过程"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eWinPorc.Format(_T("%s.%s_windowproc"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}

			if(!newValue.IsEmpty())
			{
				//xxxxxxxxxxxxxx这里还要加打开T文件,并添加事件代码的操作
				pCurrent->m_DiaDesign.m_TcDlgConfig.m_eWinProc=eWinPorc;
				pProp->SetOriginalValue(eWinPorc);
				pProp->SetValue(eWinPorc);
				//添加Dia事件代码
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

        if(title==_T("名称"))
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
        //属性标题
        else if(title==_T("文本"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pStatic,TCTT_STATIC,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
           /* CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pStatic->SetText(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
        else if(title==_T("标签背景颜色"))
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
        else if(title==_T("字体"))
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
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
			pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
            pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
            pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pStatic->GetWindowRect(&rc);
			pStatic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pStatic);
		}
        else if(title==_T("点击"))
        {
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pStatic->TCSetClickEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
        }
        //属性值
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
        if(title==_T("名称"))
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
        //属性标题
        else if(title==_T("文本"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pButton,TCTT_BUTTON,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pButton->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//属性标题
        else if(title==_T("默认按钮"))
        {
			//将所有的默认按钮置为false
			pCurrent->m_DiaDesign.SetAllDefButtonFalse();

			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_BUTTON,pButton,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
        }
		//属性有效
		else if(title==_T("有效"))
        {
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_BUTTON,pButton,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
        }
		else if(title==_T("多行"))
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
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
			pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
            pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
            pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pButton->GetWindowRect(&rc);
            pButton->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pButton);
		}		
        else if(title==_T("点击"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pButton->TCSetClickEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
        }
        //属性值
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

        if(title==_T("名称"))
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
        //属性标题
        else if(title==_T("标题"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pRadio,TCTT_RADIO,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pRadio->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//属性有效
		else if(title==_T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_RADIO,pRadio,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("分组"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			pRadio->TCSetGroup(newValue);
		}
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
			pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
            pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
            pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pRadio->GetWindowRect(&rc);
            pRadio->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pRadio);
		}
        else if(title==_T("选中"))
        {
            bool newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().boolVal;
            newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_RADIO,pRadio,m_CurrentType.name,newValue);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pRadio->TCSetCheck(newValue);
            //pProp->SetOriginalValue((_variant_t)newValue);
        }
		else if(title==_T("标题居左"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_RADIO,pRadio,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("点击"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pRadio->TCSetRadioEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
			//pRadio->TCSetRadioEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
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

        if(title==_T("名称"))
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
        //属性标题
        else if(title==_T("标题"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pCheck,TCTT_CHECK,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pCheck->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//属性有效
		else if(title==_T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_CHECK,pCheck,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pCheck->GetWindowRect(&rc);
            pCheck->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pCheck);
		}
        else if(title==_T("选中"))
        {
            bool newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().boolVal;
            newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_CHECK,pCheck,m_CurrentType.name,newValue);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pCheck->TCSetCheck(newValue);
            //pProp->SetOriginalValue((_variant_t)newValue);
        }
		else if(title==_T("标题居左"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_CHECK,pCheck,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("点击"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString eClick;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pCheck->TCSetCheckEvent(eClick);
				pProp->SetOriginalValue(eClick);
				pProp->SetValue(eClick);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(eClick);
			}
			//pCheck->TCSetCheckEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
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

        if(title==_T("名称"))
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
        //属性标题
        else if(title==_T("文本"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pEdit,TCTT_EDIT,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pEdit->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//属性有效
		else if(title==_T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		//是否多行
		else if(title == _T("多行"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CMFCPropertyGridProperty* pPropVerti= m_wndPropList.FindItemByData(DWORD_PTR(_T("编辑框垂直滚动条")));
			CMFCPropertyGridProperty* pPropHoniz = m_wndPropList.FindItemByData(DWORD_PTR(_T("编辑框水平滚动条")));

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
        else if(title==_T("文本颜色"))
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
        else if(title==_T("背景颜色"))
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

        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pEdit->GetWindowRect(&rc);
            pEdit->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pEdit);
		}
        else if(title==_T("密文"))
        {
            bool newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().boolVal;
            newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            //pEdit->SetPassWord(newValue);
            //pProp->SetOriginalValue((_variant_t)newValue);
        }
		else if(title==_T("数字"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
		else if(title==_T("只读"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
		else if(title==_T("垂直滚动条"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
		else if(title==_T("水平滚动条"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_EDIT,pEdit,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			//pEdit->SetPassWord(newValue);
			//pProp->SetOriginalValue((_variant_t)newValue);
		}
        else if(title==_T("内容改变"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString enChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				enChange.Format(_T("%s.%s_内容改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				enChange.Format(_T("%s.%s_enchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pEdit->TCSetEnChangeEvent(enChange);
				pProp->SetOriginalValue(enChange);
				pProp->SetValue(enChange);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(enChange);
			}
   //         pEdit->TCSetEnChangeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pEdit->TCGetEnChangeEvent());
        }
		else if(title==_T("获得焦点"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString setFocus;
			if(CTCRunTimeManager::g_LangType==0)
			{
				setFocus.Format(_T("%s.%s_获得焦点"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				setFocus.Format(_T("%s.%s_setfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pEdit->TCSetFocusEvent(setFocus);
				pProp->SetOriginalValue(setFocus);
				pProp->SetValue(setFocus);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(setFocus);
			}
			//         pEdit->TCSetKillfocusEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////添加事件代码
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pEdit->TCGetKillfocusEvent());
		}
        else if(title==_T("失去焦点"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString killFocus;
			if(CTCRunTimeManager::g_LangType==0)
			{
				killFocus.Format(_T("%s.%s_失去焦点"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				killFocus.Format(_T("%s.%s_killfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pEdit->TCSetKillfocusEvent(killFocus);
				pProp->SetOriginalValue(killFocus);
				pProp->SetValue(killFocus);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(killFocus);
			}
   //         pEdit->TCSetKillfocusEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
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

        if(title==_T("名称"))
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
        //属性标题
        else if(title==_T("标题"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pGroup,TCTT_GROUP,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            pGroup->SetText(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
        else if(title==_T("窗口风格"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            if(newValue==_T("窗口风格"))
                pGroup->SetTCGroupStyle(CTCGroupBox::TCGB_WINDOW);
            else if(newValue==_T("边框风格"))
                pGroup->SetTCGroupStyle(CTCGroupBox::TCGB_FRAME);
            else
            {
                newValue=_T("边框风格");
                pGroup->SetTCGroupStyle(CTCGroupBox::TCGB_FRAME);
            }
            pProp->SetOriginalValue(newValue);
        }
        else if(title==_T("对齐"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
            DWORD dir;  //=pGroup->GetAlignment();
            if(newValue==_T("左对齐"))
                dir=SS_LEFT;
            else if(newValue==_T("中间对齐"))
                dir=SS_CENTER;
            else if(newValue==_T("右对齐"))
                dir=SS_RIGHT;
            else 
                dir=SS_LEFT;

            pGroup->SetAlignment(dir);

            pProp->SetOriginalValue(newValue);
        }
        else if(title==_T("标题背景颜色"))
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
        else if(title==_T("背景颜色"))
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
        else if(title==_T("边框颜色"))
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

        else if(title==_T("字体"))
        {
            CMFCPropertyGridFontProperty* pFontProp=(CMFCPropertyGridFontProperty*)pProp;
            LPLOGFONT font=pFontProp->GetLogFont();
            COLORREF color=pFontProp->GetColor();
			pGroup->SetFontFaceName(font->lfFaceName);
            pGroup->SetCatptionTextColor(color);
            pGroup->SetFont(*font);
            pFontProp->SetOriginalValue(pProp->GetValue());
        }

        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pGroup->GetWindowRect(&rc);
            pGroup->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
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

        if(title==_T("名称"))
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
		else if(title==_T("图片名称"))
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
		else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pPic->GetWindowRect(&rc);
            pPic->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pPic);
		}
		else if(title==_T("左键单击"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_左键单击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pPic->TCSetClickEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////添加事件代码
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

        if(title==_T("名称"))
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
		//属性有效
		else if(title==_T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_COMBOBOX,pCombox,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x坐标"))
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
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pCombox->GetWindowRect(&rc);
			pCombox->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pCombox->GetWindowRect(&rc);
			pCombox->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
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
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pCombox);
		}
		else if(title==_T("左键按下"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString lBtnDown;
			if(CTCRunTimeManager::g_LangType==0)
			{
				lBtnDown.Format(_T("%s.%s_左键按下"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				lBtnDown.Format(_T("%s.%s_lbuttondown"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pCombox->SetLeftDownEvent(lBtnDown);
				pProp->SetOriginalValue(lBtnDown);
				pProp->SetValue(lBtnDown);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(lBtnDown);
			}
			//pCombox->SetLeftDownEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pCombox->GetLeftDownEvent());
        }
		else if(title==_T("选择改变"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchanged"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pCombox->SetEnChangeEvent(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
			//pCombox->SetEnChangeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
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

        if(title==_T("名称"))
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
		else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
			pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
            pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
            pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pProgress->GetWindowRect(&rc);
            pProgress->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pProgress);
		}
		else if(title==_T("垂直"))
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

        if(title==_T("名称"))
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
		else if(title==_T("浏览模式"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CtrlProptyLogicCommand* pCommand;
			if( newValue == _T("文件") )
				pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_PATH,pPath,m_CurrentType.name,true,title);
			else if( newValue == _T("文件夹"))
				pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_PATH,pPath,m_CurrentType.name,false,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
			
                /*pPath->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_File);
			
                pPath->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_Folder);*/

           // pProp->SetOriginalValue(newValue);
        }
		//属性有效
		else if(title==_T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_PATH,pPath,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pPath->GetWindowRect(&rc);
            pPath->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pPath);
		}
        else if(title==_T("选择改变"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchanged"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pPath->TCSetEnChangeEvent(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
   //         pPath->TCSetEnChangeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
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

        if(title==_T("名称"))
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
		//属性有效
		else if(title == _T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_SLIDER,pSlider,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pSlider->GetWindowRect(&rc);
            pSlider->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pSlider);
		}
		else if(title==_T("垂直"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_SLIDER,pSlider,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("鼠标释放"))
		{
			CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_鼠标释放"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_releasedcapture"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pSlider->TCSetSliderChangeEvent(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(selChange);
			}
   //         pSlider->TCSetSliderChangeEvent(newValue);
   //         pProp->SetOriginalValue((_variant_t)newValue);
			////添加事件代码
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

        if(title==_T("名称"))
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
		//属性有效
		else if(title == _T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_DATE,pDate,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
			pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
            pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
            pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pDate->GetWindowRect(&rc);
            pDate->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
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

        if(title==_T("名称"))
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
		if(title==_T("标题"))
        {
			CtrlProptyTitleCommand* pCommand = new CtrlProptyTitleCommand(pProp,pList,TCTT_LISTBOX,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
            /*CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			pList->SetWindowTextW(newValue);
            pProp->SetOriginalValue(newValue);*/
        }
		//属性有效
		else if(title == _T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_LISTBOX,pList,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
            pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
			pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
            pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pList->GetWindowRect(&rc);
            pList->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pList);
		}
		else if(title == _T("默认滚动"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_LISTBOX,pList,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("左键双击"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString clicked;
			if(CTCRunTimeManager::g_LangType==0)
			{
				clicked.Format(_T("%s.%s_左键双击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				clicked.Format(_T("%s.%s_doubleclicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pList->TCSetListBoxDoubleClick(clicked);
				pProp->SetOriginalValue(clicked);
				pProp->SetValue(clicked);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(clicked);
			}
		}
		else if(title==_T("选择改变"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pList->TCSetListBoxSelChange(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//添加事件处理代码
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

        if(title==_T("名称"))
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
		//属性有效
		else if(title==_T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_TAB,pTab,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
            //pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pTab->GetWindowRect(&rc);
            pTab->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pTab);
		}
		else if(title==_T("选择改变"))
		{
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString selChange;
			if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pTab->TCSetSelChange(selChange);
				pProp->SetOriginalValue(selChange);
				pProp->SetValue(selChange);
				//添加事件处理代码
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

        if(title==_T("名称"))
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
		else if(title==_T("频率"))
        {
			UINT time=pProp->GetValue().uintVal;
			pTimer->SetTimer(time);
            pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pTimer->GetWindowRect(&rc);
            pTimer->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pTimer->GetWindowRect(&rc);
            pTimer->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
         //   pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("时钟"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString timer;
			if(CTCRunTimeManager::g_LangType==0)
			{
				timer.Format(_T("%s.%s_时钟"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				timer.Format(_T("%s.%s_ontime"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pTimer->TCSetTimeEvent(timer);
				pProp->SetOriginalValue(timer);
				pProp->SetValue(timer);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(timer);
			}
			//pTimer->TCSetTimeEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
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
        if(title==_T("名称"))
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
		//属性有效
		else if(title==_T("有效"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_HOTKEY,pHotKey,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("键值"))
        {
			CString newValue,oldValue;
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			//这儿设置热键字符串
			pHotKey->SetHotKeyName(newValue);
			pProp->SetOriginalValue((_variant_t)newValue);

        }
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pHotKey->GetWindowRect(&rc);
            pHotKey->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pHotKey);
		}
		else if(title==_T("热键"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString hotkey;
			if(CTCRunTimeManager::g_LangType==0)
			{
				hotkey.Format(_T("%s.%s_热键"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				hotkey.Format(_T("%s.%s_onhotkey"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pHotKey->TCSetHotKeyEvent(hotkey);
				pProp->SetOriginalValue(hotkey);
				pProp->SetValue(hotkey);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(hotkey);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
        }
		else if(title==_T("失去焦点"))
        {
            CString newValue,oldValue;   
            oldValue = pProp->GetOriginalValue().bstrVal;
            newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString killfocus;
			if(CTCRunTimeManager::g_LangType==0)
			{
				killfocus.Format(_T("%s.%s_失去焦点"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				killfocus.Format(_T("%s.%s_killfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pHotKey->TCSetKillFocusEvent(killfocus);
				pProp->SetOriginalValue(killfocus);
				pProp->SetValue(killfocus);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(killfocus);
			}
			//pHotKey->TCSetKillFocusEvent(newValue);
   //         pProp->SetOriginalValue(newValue);
			////添加事件代码
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
        if(title==_T("名称"))
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
		//属性网址
		else if(title.CompareNoCase(_T("URL"))==0)
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			pExplor->TCSetURL(newValue);
		}	
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pExplor->GetWindowRect(&rc);
            pExplor->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pExplor);
		}
		else if(title==_T("加载完成"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_加载完成"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_load"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pExplor->TCSetloadComplete(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////添加事件代码
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
        if(title==_T("名称"))
        {
			CtrlProptyNameCommand* pCommand = new CtrlProptyNameCommand(pCurrent,pProp,pGridCtrl,TCTT_GRIDCTRL,this);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
        }
		else if(title==_T("虚拟模式"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CMFCPropertyGridProperty* pPropEdit= m_wndPropList.FindItemByData(DWORD_PTR(_T("编辑表格")));
			CMFCPropertyGridProperty* pPropRow = m_wndPropList.FindItemByData(DWORD_PTR(_T("固定行选中")));
			CMFCPropertyGridProperty* pPropCol = m_wndPropList.FindItemByData(DWORD_PTR(_T("固定列选中")));
			
			if(newValue)
			{	pPropEdit->Enable(FALSE);	pPropRow->Enable(FALSE);	pPropCol->Enable(FALSE);}
			else
			{	pPropEdit->Enable(TRUE);	pPropRow->Enable(TRUE);		pPropCol->Enable(TRUE); }

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("普通项文本颜色"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = ::GetSysColor(COLOR_WINDOWTEXT);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("普通项背景颜色"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = RGB(0xFF,0xFF,0xE0);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("固定项背景颜色"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = ::GetSysColor(COLOR_BTNFACE);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("固定项文本颜色"))
		{
			CMFCPropertyGridColorProperty* pColorProp=(CMFCPropertyGridColorProperty*)pProp;
			COLORREF color=pColorProp->GetColor();
			if(0xffffffff==color)
				color = ::GetSysColor(COLOR_WINDOWTEXT);
			CtrlProptyColorCommand* pCommand = new CtrlProptyColorCommand(TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,color,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("垂直网格"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("水平网格"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("字体"))
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
        else if(title==_T("x坐标"))
        {
            int x=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(x,rc.top,x+rc.Width(),rc.bottom);
          //  pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("y坐标"))
        {
            int y=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,y,rc.right,y+rc.Height());
        //    pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("宽度"))
        {
            int width=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.left+width,rc.bottom);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
        else if(title==_T("高度"))
        {
            int height=pProp->GetValue().intVal;
            CRect rc;
            pGridCtrl->GetWindowRect(&rc);
            pGridCtrl->GetParent()->ScreenToClient(&rc);
            pRect->TCMoveWindos(rc.left,rc.top,rc.right,rc.top+height);
           // pProp->SetOriginalValue(pProp->GetValue());
            pCurrent->m_DiaDesign.Invalidate();
        }
		else if(title==_T("显示顺序"))
		{
			int order=pProp->GetValue().intVal;
			pCurrent->m_DiaDesign.m_ControlTracker.TCSetCurrentCtrlZorder(order,pGridCtrl);
		}
		else if(title==_T("可编辑"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("排序"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("填充"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("行数"))
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
		else if(title==_T("列数"))
		{
			int colCount=pProp->GetValue().intVal;
			int oldColumCount = pGridCtrl->GetColumnCount();
			pGridCtrl->SetColumnCount(colCount);

			CString title;
			for (int i = oldColumCount; i < colCount; i++)
			{
				title.Format(_T("第 %d 列"), i);
				pGridCtrl->SetItemText(0,i,title);
				pGridCtrl->SetItemData(0,i,TCGRID_CTRL_NOMAL);
				pGridCtrl->TCSetColReadInfo(0,i,FALSE);
			}
		}
		else if(title==_T("固定行数"))
		{
			int fixedRowCount=pProp->GetValue().intVal;
			pGridCtrl->SetFixedRowCount(fixedRowCount);
		}
		else if(title==_T("固定列数"))
		{
			int fixedColCount=pProp->GetValue().intVal;
			pGridCtrl->SetFixedColumnCount(fixedColCount);
		}
		else if(title==_T("选中"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CMFCPropertyGridProperty* pPropRow = m_wndPropList.FindItemByData(DWORD_PTR(_T("固定行选中")));
			CMFCPropertyGridProperty* pPropCol = m_wndPropList.FindItemByData(DWORD_PTR(_T("固定列选中")));
			CMFCPropertyGridProperty* pPropListMode = m_wndPropList.FindItemByData(DWORD_PTR(_T("列表模式选中")));

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
		else if(title==_T("固定行选中"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("固定列选中"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("列表模式"))
		{
			bool newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().boolVal;
			newValue = pProp->GetValue().boolVal;
			CMFCPropertyGridProperty* pPropRow = m_wndPropList.FindItemByData(DWORD_PTR(_T("固定行选中")));
			if(newValue)
				pPropRow->Enable(FALSE);
			else
				pPropRow->Enable(TRUE);
				

			CtrlProptyLogicCommand* pCommand = new CtrlProptyLogicCommand(pCurrent,TCTT_GRIDCTRL,pGridCtrl,m_CurrentType.name,newValue,title);
			pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		}
		else if(title==_T("左键单击"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_左键单击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pGridCtrl->TCSetClickEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////添加事件代码
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
		else if(title==_T("编辑开始"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_编辑开始"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_beforeedit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pGridCtrl->TCSetBeforeEditEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////添加事件代码
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
		else if(title==_T("编辑结束"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_编辑结束"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_afteredit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pGridCtrl->TCSetAfterEditEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////添加事件代码
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(pHotKey->TCGetHotKeyEvent());
		}
		else if(title==_T("点击排序"))
		{
			CString newValue,oldValue;   
			oldValue = pProp->GetOriginalValue().bstrVal;
			newValue = pProp->GetValue().bstrVal;
			newValue.Trim();
			CString loaded;
			if(CTCRunTimeManager::g_LangType==0)
			{
				loaded.Format(_T("%s.%s_点击排序"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				loaded.Format(_T("%s.%s_headsort"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,pRect->GetCtlID());
			}

			if(!newValue.IsEmpty())
			{
				//////////////这里还要加打开T文件,并添加事件代码的操作//////////////
				pGridCtrl->TCSetHeadSortEvent(loaded);
				pProp->SetOriginalValue(loaded);
				pProp->SetValue(loaded);
				//添加事件处理代码
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCAddEventCode(loaded);
			}
			//pHotKey->TCSetHotKeyEvent(newValue);
			//         pProp->SetOriginalValue(newValue);
			////添加事件代码
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


//设置特定项的属性(参数1：属性类型，参数2：当前名称，参数3：标题，参数4：新值)
void CPropertiesWnd::SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue)
{
    if( protype != m_CurrentType.type )
        return;
    if( m_CurrentType.name != name )
		return;
	//得到要编辑的项
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
        if(title==_T("窗口背景图"))
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
		else if(title==_T("窗口初始化"))
		{

			CString eDiaInit;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaInit.Format(_T("<添加>%s.初始化"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaInit.Format(_T("<添加>%s.init"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
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
		else if(title==_T("窗口销毁"))
		{

			CString eDiaDestory;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaDestory.Format(_T("<添加>%s.销毁"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaDestory.Format(_T("<添加>%s.destroy"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
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
		else if(title==_T("窗口关闭"))
		{

			CString eDiaClose;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eDiaClose.Format(_T("<添加>%s.关闭"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eDiaClose.Format(_T("<添加>%s.close"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
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
		else if(title==_T("消息路由"))
		{

			CString ePreTrans;
			if(CTCRunTimeManager::g_LangType==0)
			{
				ePreTrans.Format(_T("<添加>%s.消息路由"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				ePreTrans.Format(_T("<添加>%s.pretranslatemessage"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID);
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
        if(title==_T("点击"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;

			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
        if(title==_T("点击"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
        if(title==_T("点击"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
        if(title==_T("点击"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_点击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_click"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
        if(title==_T("内容改变"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_内容改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_enchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
		else if( title==_T("获得焦点"))
		{
			CString eSetFocus;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eSetFocus.Format(_T("<添加>%s.%s_获得焦点"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eSetFocus.Format(_T("<添加>%s.%s_setfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
        else if( title==_T("失去焦点"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_失去焦点"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_killfocus"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
		if( title==_T("左键双击"))
		{
			CString doubleclicked;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				doubleclicked.Format(_T("<添加>%s.%s_左键双击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				doubleclicked.Format(_T("<添加>%s.%s_doubleclicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
		else if( title==_T("选择改变"))
        {
            CString selChange;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("<添加>%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("<添加>%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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

        if( title==_T("选择改变"))
        {
            CString selChange;
            CString id=m_CProperWnd->m_CurrentType.name;
            if(CTCRunTimeManager::g_LangType==0)
			{
				selChange.Format(_T("<添加>%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				selChange.Format(_T("<添加>%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
		if(title==_T("左键按下"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_左键按下"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_lbuttondown"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
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
		else if(title==_T("选择改变"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_selectchanged"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
           // eClick.Format(_T("%s.%s_选择改变"),CTCRunTimeManager::g_ProjName,id);
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
        if(title==_T("图片名称"))
        {
            pWndComboEx->ResetContent();
            CString lpPath;
            lpPath.Format(_T("%s\\资源\\"),CTCRunTimeManager::g_ProjSourcePath);
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
		else if(title==_T("左键单击"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<添加>%s.%s_左键单击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<添加>%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_热键"),CTCRunTimeManager::g_ProjName,id);
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
        if(title==_T("选择改变"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_选择改变"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_selectchange"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_选择改变"),CTCRunTimeManager::g_ProjName,id);
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
        if(title==_T("鼠标释放"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_鼠标释放"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_releasedcapture"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_选择改变"),CTCRunTimeManager::g_ProjName,id);
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
		if(title==_T("时钟"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_时钟"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_ontime"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_时钟"),CTCRunTimeManager::g_ProjName,id);
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
		if(title==_T("热键"))
        {
            CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_热键"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_onhotkey"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
            //eClick.Format(_T("%s.%s_热键"),CTCRunTimeManager::g_ProjName,id);
            TCHAR csClickEvent[MAXCTRLNAME+3];
            ::memset(csClickEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
            ::lstrcpyW(csClickEvent,eClick); 
            COMBOBOXEXITEM   Item; 
            Item.mask   =   CBEIF_TEXT ; 
            Item.pszText=csClickEvent;
            Item.iItem   =   0;//index 
            pWndComboEx->InsertItem(&Item);
        }
		else if(title==_T("失去焦点"))
		{
			CString eClick;
            CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eClick.Format(_T("<添加>%s.%s_失去焦点"),CTCRunTimeManager::g_ProjName,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eClick.Format(_T("<添加>%s.%s_killfocus"),CTCRunTimeManager::g_ProjName,id);
			}
            //eClick.Format(_T("%s.%s_失去焦点"),CTCRunTimeManager::g_ProjName,id);
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
		if(title==_T("加载完成"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<添加>%s.%s_加载完成"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<添加>%s.%s_load"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_热键"),CTCRunTimeManager::g_ProjName,id);
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
		if(title==_T("左键单击"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<添加>%s.%s_左键单击"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<添加>%s.%s_clicked"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_热键"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("编辑开始"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<添加>%s.%s_编辑开始"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<添加>%s.%s_beforeedit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_热键"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("编辑结束"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<添加>%s.%s_编辑结束"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<添加>%s.%s_afteredit"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_热键"),CTCRunTimeManager::g_ProjName,id);
			TCHAR csLoadEvent[MAXCTRLNAME+3];
			::memset(csLoadEvent,NULL,(MAXCTRLNAME+3)*sizeof(TCHAR));
			::lstrcpyW(csLoadEvent,eLoad); 
			COMBOBOXEXITEM   Item; 
			Item.mask   =   CBEIF_TEXT ; 
			Item.pszText=csLoadEvent;
			Item.iItem   =   0;//index 
			pWndComboEx->InsertItem(&Item);
		}
		else if(title==_T("点击排序"))
		{
			CString eLoad;
			CString id=m_CProperWnd->m_CurrentType.name;
			if(CTCRunTimeManager::g_LangType==0)
			{
				eLoad.Format(_T("<添加>%s.%s_点击排序"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				eLoad.Format(_T("<添加>%s.%s_headsort"),((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID,id);
			}
			//eClick.Format(_T("%s.%s_热键"),CTCRunTimeManager::g_ProjName,id);
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
			// 注意过滤器的写法
			_T("ICON Files (*.ico)|*.ico||"),NULL);
		
		if(IDOK == filedlg.DoModal())
		{
			CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
			CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);

			if(!pCurrent)
				return;

			pCurrent->m_DiaDesign.m_TcDlgConfig.m_icon = filedlg.GetPathName();
			m_CProperWnd->SetPropertyString(TCPROPERTY_TWIN,CTCRunTimeManager::g_ProjName,DWORD_PTR(_T("窗口图标")),pCurrent->m_DiaDesign.m_TcDlgConfig.m_icon);			
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

