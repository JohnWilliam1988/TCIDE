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
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "TC.h"

//添加TC运行时头文件
#include "TCRunTimeManager.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
	m_clickCount = 0;
}

CClassView::~CClassView()
{
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	ON_EN_CHANGE(1,OnEditChange)
	ON_BN_CLICKED(3,OnBnClicked)
	ON_UPDATE_COMMAND_UI(3,OnUpdateFoundButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 消息处理程序

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("未能创建类视图\n");
		return -1;                                     //未能创建
	}

	//m_wndClassView.ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	const DWORD dwComboStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS ;
	const DWORD dwButtonStyle = WS_CHILD | WS_VISIBLE | BS_CENTER | WS_CLIPSIBLINGS ;

	if (!m_foundEdit.Create(dwComboStyle, rectDummy, this, 1))
	{
		TRACE0("未能创建搜索框 \n");
		return -1;      // 未能创建
	}

	if( !m_foundButton.Create(_T("查找"),dwButtonStyle, rectDummy, this, 3))
	{
		TRACE0("未能创建搜索按钮 \n");
		return -1;      // 未能创建
	}

	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	m_foundEdit.SetFont(pFont);
	m_foundButton.SetFont(pFont);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// 填充TC库
	FillClassView();

	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassView::FillClassView()
{
    //加载api.xml到TC库的树中
	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("TC库"), 2, 2);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
	{
		CString helpDllName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].name;
		//添加一个DLL节点
		HTREEITEM hRootDll = m_wndClassView.InsertItem(helpDllName,0,0,hRoot);

		for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
		{
			CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
			//添加一个命名空间节点
			HTREEITEM hNameSP = m_wndClassView.InsertItem(helpSPName,1,1,hRootDll);
			m_wndClassView.SetItemData(hNameSP,FCTREENODETYPE::FCTREE_NAMESPACE);

			for( int func = 0; func < CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun.size(); func++ )
			{
				CString funName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].fun[ func ].funcName;
				HTREEITEM hFun = m_wndClassView.InsertItem(funName,3,3,hNameSP);
				m_wndClassView.SetItemData(hFun,FCTREENODETYPE::FCTREE_FUNCTION);
			}
		}
	}

	HTREEITEM hRootKeyCode = m_wndClassView.InsertItem(_T("键码"),3,3,hRoot);	
	HTREEITEM hRootNumKeyCode = m_wndClassView.InsertItem(_T("数字键"),1,1,hRootKeyCode);

	m_wndClassView.InsertItem(_T("0/) (48)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("1/! (49)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("2/@ (50)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("3/# (51)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("4/$ (52)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("5/% (53)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("6/^ (54)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("7/& (55)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("8/* (56)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("9/( (57)"),4,4,hRootNumKeyCode);

	m_wndClassView.InsertItem(_T("小键盘0 (96)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘1 (97)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘2 (98)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘3 (99)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘4 (100)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘5 (101)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘6 (102)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘7 (103)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘8 (104)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("小键盘9 (105)"),4,4,hRootNumKeyCode);

	HTREEITEM hRootAlphaKeyCode = m_wndClassView.InsertItem(_T("字母键"),1,1,hRootKeyCode);

	m_wndClassView.InsertItem(_T("A (65)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("B (66)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("C (67)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("D (68)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("E (69)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("F (70)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("G (71)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("H (72)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("I (73)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("J (74)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("K (75)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("L (76)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("M (77)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("N (78)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("O (79)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("P (80)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("Q (81)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("R (82)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("S (83)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("T (84)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("U (85)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("V (86)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("W (87)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("X (88)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("Y (89)"),4,4,hRootAlphaKeyCode);
	m_wndClassView.InsertItem(_T("Z (90)"),4,4,hRootAlphaKeyCode);


	HTREEITEM hRootFunctionKeyCode = m_wndClassView.InsertItem(_T("功能键"),1,1,hRootKeyCode);

	m_wndClassView.InsertItem(_T("F1  (112)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F2  (113)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F3  (114)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F4  (115)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F5  (116)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F6  (117)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F7  (118)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F8  (119)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F9  (120)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F10 (121)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F11 (122)"),4,4,hRootFunctionKeyCode);
	m_wndClassView.InsertItem(_T("F12 (123)"),4,4,hRootFunctionKeyCode);

	HTREEITEM hRootDirectionKeyCode = m_wndClassView.InsertItem(_T("方向键"),1,1,hRootKeyCode);

	m_wndClassView.InsertItem(_T("←  (37)"),4,4,hRootDirectionKeyCode);
	m_wndClassView.InsertItem(_T("↑  (38)"),4,4,hRootDirectionKeyCode);
	m_wndClassView.InsertItem(_T("→  (39)"),4,4,hRootDirectionKeyCode);
	m_wndClassView.InsertItem(_T("↓  (40)"),4,4,hRootDirectionKeyCode);

	HTREEITEM hRootCtrlKeyCode = m_wndClassView.InsertItem(_T("控制键"),1,1,hRootKeyCode);

	m_wndClassView.InsertItem(_T("BackSpace	(8)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Tab		(9)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Enter		(13)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Shift		(16)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Ctrl		(17)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Pause Break(19)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Caps Lock	(20)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Esc		(27)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Space		(32)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("PageUp		(33)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("PageDown	(34)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("End		(35)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Home		(36)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Insert		(45)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Delete		(46)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Windows	(91)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Num Lock	(144)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("Scroll Lock(145)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("左Alt		(164)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("右Alt		(165)"),4,4,hRootCtrlKeyCode);


	HTREEITEM hRootSymbolKeyCode = m_wndClassView.InsertItem(_T("符号键"),1,1,hRootKeyCode);
	

	m_wndClassView.InsertItem(_T("小键盘*  (106)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("小键盘+  (107)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("小键盘-  (109)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("小键盘.  (110)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("小键盘/  (111)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("= (187)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T(", (188)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("- (189)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T(". (190)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("/ (191)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("~ (192)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("[ (219)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("\\ (220)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("]  (221)"),4,4,hRootSymbolKeyCode);
	
	HTREEITEM hRootDataTypeCode = m_wndClassView.InsertItem(_T("数据类型"),3,3,hRoot);

	m_wndClassView.InsertItem(_T("字符型(在引号中可以填：数字、中文、字母、特殊字符、以及他们的组合)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("整型(-2147483647 ~ 2147483647)"),4,4,hRootDataTypeCode);	
	m_wndClassView.InsertItem(_T("逻辑型(true 或 false)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("浮点型(-1.79E+308 ~ +1.79E+308)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("插件(标准COM)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("数组(多个变量的集合)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("null(空值)"),4,4,hRootDataTypeCode);

	HTREEITEM hRootEscapeChar = m_wndClassView.InsertItem(_T("转义字符"),3,3,hRoot);
	m_wndClassView.InsertItem(_T("\\n   (换行(LF) ，将当前位置移到下一行开头)"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\r   (回车(CR) ，将当前位置移到本行开头)"),4,4,hRootEscapeChar);	
	m_wndClassView.InsertItem(_T("\\r\\n(回车换行(CRLF) ，有些文件编辑器,需要\\r\\n 才能有换行效果)"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\t   (水平制表(HT) （跳到下一个TAB位置))"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\\\  (代表一个反斜线字符 \\)"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\\"  (代表一个双引号字符 \")"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\\'  (代表一个单引号字符 \')"),4,4,hRootEscapeChar);

	HTREEITEM hRootTimeFormat = m_wndClassView.InsertItem(_T("时间格式"),3,3,hRoot);
	m_wndClassView.InsertItem(_T("时间格式  2012/12/3 11:20:30"),4,4,hRootTimeFormat);

    //展开操作要在所有节点插入完成后最后展开
	m_wndClassView.Expand(hRoot,TVE_EXPAND);
}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectEdit;
	GetClientRect(rectClient);
	m_foundEdit.GetClientRect(&rectEdit);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	int cyEdit = rectEdit.Size().cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_foundEdit.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width() - 40, cyTlb - 6, SWP_NOACTIVATE | SWP_NOZORDER);
	m_foundButton.SetWindowPos(NULL, rectClient.Width() - 40, rectClient.top + cyTlb, 38, cyTlb - 5, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + cyEdit + 1, rectClient.Width() - 2, rectClient.Height() - (cyTlb + cyEdit + 2), SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->hwnd == m_foundEdit.GetSafeHwnd() )
	{
		if( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN) )
		{
			//调用查找功能
			OnBnClicked();
		}
	}
	
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: 在此处添加命令处理程序代码
}

void CClassView::OnClassDefinition()
{
	// TODO: 在此处添加命令处理程序代码
}

void CClassView::OnClassProperties()
{
	// TODO: 在此处添加命令处理程序代码
}

void CClassView::OnNewFolder()
{
	//AfxMessageBox(_T("新建文件夹..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}


BOOL CClassView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	NMHDR* pNMHDR = (NMHDR*)lParam;
    ASSERT(pNMHDR != NULL);

    if(pNMHDR->hwndFrom==m_wndClassView.m_hWnd)
    {
        switch (pNMHDR->code)
        {
		case TVN_SELCHANGED:
			OnTvnSelchangedClassView(pNMHDR, pResult);
			break;
        default:
            break;
        }
    }
	return CDockablePane::OnNotify(wParam, lParam, pResult);
}


void CClassView::OnTvnSelchangedClassView(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;

	CMainFrame *pMain=(CMainFrame*)AfxGetMainWnd();

    HTREEITEM mySelected = m_wndClassView.GetSelectedItem() ;
    CString strSelectedText = m_wndClassView.GetItemText(mySelected) ;
    HTREEITEM mySelectedPar = m_wndClassView.GetParentItem(mySelected) ;
    CString strSelectedParText = m_wndClassView.GetItemText(mySelectedPar);

    FCTREENODETYPE nodeType=(FCTREENODETYPE)m_wndClassView.GetItemData(mySelected);

    if(nodeType==FCTREENODETYPE::FCTREE_FUNCTION)
    {	
		//点击不是函数的时候不用激活函数输出窗口
		pMain->ActiveDecWnd();
		CString example = CTCRunTimeManager::g_TCAip.GetExample(strSelectedParText,strSelectedText);
		example.Trim();
		CString XMLStr=example;
		//得到函数原形
		CString str;
		CString tempFunProto;
		int n;
		n=XMLStr.Find(L"|");
		str=XMLStr.Right(XMLStr.GetLength()-n-1);
		n=str.Find(L"|");
		tempFunProto=str.Left(n);
		tempFunProto.Trim();
		//得到函数原形
		//函数说明
		CString tempFunDes;
		n=str.Find(L"|");
		str=XMLStr.Right(str.GetLength()-n-1);
		n=str.Find(L"|");
		str=XMLStr.Right(str.GetLength()-n-1);
		n=str.Find(L"|");
		tempFunDes=str.Left(n);
		tempFunDes.Trim();
		//得到举列
		CString tempExample;
		n=str.Find(L"|");
		str=XMLStr.Right(str.GetLength()-n-1);
		n=str.Find(L"|");
		str=XMLStr.Right(str.GetLength()-n-1);
		tempExample=str;
		tempExample.Trim();
		tempExample.Replace(_T("|"),_T("\r\n"));
		pMain->SetDecText(strSelectedParText,strSelectedText,tempFunProto,tempFunDes,tempExample);
	}

	//设置查找游标
	for (int size = 0;size < m_claViewFound.allItems.size();size++)
	{
		if(m_claViewFound.allItems[size] == mySelected)
		{
			m_clickCount = size + 1;
		}
	}

	m_wndClassView.SelectItem(mySelected);
	m_wndClassView.Select(mySelected,TVGN_CARET);
	m_wndClassView.Select(mySelected, TVGN_DROPHILITE);
}

void CClassView::OnEditChange()
{
	HTREEITEM root = m_wndClassView.GetRootItem();
	m_wndClassView.SelectItem(root);
	m_wndClassView.Select(root,TVGN_CARET);
	m_wndClassView.Select(root, TVGN_DROPHILITE);
} 

void CClassView::OnBnClicked()
{
	CString strSeek;
	m_foundEdit.GetWindowTextW(strSeek);
	if( strSeek.IsEmpty() )
		return;
	HTREEITEM selectItem = m_wndClassView.GetSelectedItem();
	 
	//如果找的字串已经是查找过的则不用查找
	if((selectItem != NULL) && (0 != m_claViewFound.seekStr.CompareNoCase(strSeek)))
	{
		m_claViewFound.Reset();
		m_clickCount = 0;
		TravelTCTreeView(selectItem,strSeek);
	}
	//如果越界，则复位点击数
	if(m_clickCount > m_claViewFound.allItems.size() - 1)
		m_clickCount = 0;

	if(m_claViewFound.allItems.size() > 0)
	{
		HTREEITEM hItem = m_claViewFound.allItems[m_clickCount];
		m_wndClassView.Expand(m_wndClassView.GetRootItem(), TVE_EXPAND);
		//所有父节点展开
		HTREEITEM parentItem = m_wndClassView.GetParentItem(hItem);
		while (parentItem != NULL)
		{
			m_wndClassView.Expand(parentItem, TVE_EXPAND);
			parentItem = m_wndClassView.GetParentItem(parentItem);
		}
		//如果有子节点，展开当前节点
		if (FCTREE_NAMESPACE == m_wndClassView.GetItemData(hItem))
		{
			m_wndClassView.Expand(hItem, TVE_EXPAND);
			m_wndClassView.SelectSetFirstVisible(hItem);
		}
		m_wndClassView.SelectItem(hItem);
		m_wndClassView.Select(hItem,TVGN_CARET);
		m_wndClassView.Select(hItem, TVGN_DROPHILITE);
	}
}

/***********************************************************************/
/*  \函数名称：TravelTCTreeView
/*	\函数功能：遍历TC库树并且高亮查找到的节点
/*	\函数返回：void
/*	\参数说明：@HTREEITEM hItem查找节点 @CString inStr要匹配的字串
/***********************************************************************/

void CClassView::TravelTCTreeView(HTREEITEM hItem,CString inStr)
{
	if (m_wndClassView.ItemHasChildren(hItem))
	{
		HTREEITEM childItem = m_wndClassView.GetChildItem(hItem);
		while (childItem != NULL)
		{
			//如果是空间，也添加到查找信息结构中
			if (FCTREE_NAMESPACE == m_wndClassView.GetItemData(childItem))
			{
				CString selStr = m_wndClassView.GetItemText(childItem);	
				if ( !selStr.IsEmpty() && (-1 != selStr.Find(inStr)))
				{
					m_claViewFound.seekStr = inStr;
					m_claViewFound.allItems.push_back(childItem);
				}
			}
			TravelTCTreeView(childItem, inStr);
			childItem = m_wndClassView.GetNextItem(childItem ,TVGN_NEXT);
		}
	}
	else
	{
		if (hItem != NULL)
		{
			CString selStr = m_wndClassView.GetItemText(hItem);	
			if (FCTREE_FUNCTION == m_wndClassView.GetItemData(hItem))
			{
				
				if ( !selStr.IsEmpty() && (-1 != selStr.Find(inStr)))
				{
					m_claViewFound.seekStr = inStr;
					m_claViewFound.allItems.push_back(hItem);
				}
			}	
		}
	}
}

void CClassView::OnUpdateFoundButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}