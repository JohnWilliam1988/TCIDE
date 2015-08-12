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
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "TC.h"

//���TC����ʱͷ�ļ�
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
// ����/����
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
// CClassView ��Ϣ�������

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("δ�ܴ�������ͼ\n");
		return -1;                                     //δ�ܴ���
	}

	//m_wndClassView.ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	// ����ͼ��:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* ������*/);

	const DWORD dwComboStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS ;
	const DWORD dwButtonStyle = WS_CHILD | WS_VISIBLE | BS_CENTER | WS_CLIPSIBLINGS ;

	if (!m_foundEdit.Create(dwComboStyle, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ��������� \n");
		return -1;      // δ�ܴ���
	}

	if( !m_foundButton.Create(_T("����"),dwButtonStyle, rectDummy, this, 3))
	{
		TRACE0("δ�ܴ���������ť \n");
		return -1;      // δ�ܴ���
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

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
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

	// ���TC��
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
    //����api.xml��TC�������
	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("TC��"), 2, 2);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	for( int size = 0; size < CTCRunTimeManager::g_TCAip.m_allApi.size(); size++ )
	{
		CString helpDllName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].name;
		//���һ��DLL�ڵ�
		HTREEITEM hRootDll = m_wndClassView.InsertItem(helpDllName,0,0,hRoot);

		for( int nspa = 0; nspa <  CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp.size();  nspa++ )
		{
			CString helpSPName = CTCRunTimeManager::g_TCAip.m_allApi[ size ].nasp[ nspa ].namespaceName;
			//���һ�������ռ�ڵ�
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

	HTREEITEM hRootKeyCode = m_wndClassView.InsertItem(_T("����"),3,3,hRoot);	
	HTREEITEM hRootNumKeyCode = m_wndClassView.InsertItem(_T("���ּ�"),1,1,hRootKeyCode);

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

	m_wndClassView.InsertItem(_T("С����0 (96)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����1 (97)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����2 (98)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����3 (99)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����4 (100)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����5 (101)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����6 (102)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����7 (103)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����8 (104)"),4,4,hRootNumKeyCode);
	m_wndClassView.InsertItem(_T("С����9 (105)"),4,4,hRootNumKeyCode);

	HTREEITEM hRootAlphaKeyCode = m_wndClassView.InsertItem(_T("��ĸ��"),1,1,hRootKeyCode);

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


	HTREEITEM hRootFunctionKeyCode = m_wndClassView.InsertItem(_T("���ܼ�"),1,1,hRootKeyCode);

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

	HTREEITEM hRootDirectionKeyCode = m_wndClassView.InsertItem(_T("�����"),1,1,hRootKeyCode);

	m_wndClassView.InsertItem(_T("��  (37)"),4,4,hRootDirectionKeyCode);
	m_wndClassView.InsertItem(_T("��  (38)"),4,4,hRootDirectionKeyCode);
	m_wndClassView.InsertItem(_T("��  (39)"),4,4,hRootDirectionKeyCode);
	m_wndClassView.InsertItem(_T("��  (40)"),4,4,hRootDirectionKeyCode);

	HTREEITEM hRootCtrlKeyCode = m_wndClassView.InsertItem(_T("���Ƽ�"),1,1,hRootKeyCode);

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
	m_wndClassView.InsertItem(_T("��Alt		(164)"),4,4,hRootCtrlKeyCode);
	m_wndClassView.InsertItem(_T("��Alt		(165)"),4,4,hRootCtrlKeyCode);


	HTREEITEM hRootSymbolKeyCode = m_wndClassView.InsertItem(_T("���ż�"),1,1,hRootKeyCode);
	

	m_wndClassView.InsertItem(_T("С����*  (106)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("С����+  (107)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("С����-  (109)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("С����.  (110)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("С����/  (111)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("= (187)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T(", (188)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("- (189)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T(". (190)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("/ (191)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("~ (192)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("[ (219)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("\\ (220)"),4,4,hRootSymbolKeyCode);
	m_wndClassView.InsertItem(_T("]  (221)"),4,4,hRootSymbolKeyCode);
	
	HTREEITEM hRootDataTypeCode = m_wndClassView.InsertItem(_T("��������"),3,3,hRoot);

	m_wndClassView.InsertItem(_T("�ַ���(�������п�������֡����ġ���ĸ�������ַ����Լ����ǵ����)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("����(-2147483647 ~ 2147483647)"),4,4,hRootDataTypeCode);	
	m_wndClassView.InsertItem(_T("�߼���(true �� false)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("������(-1.79E+308 ~ +1.79E+308)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("���(��׼COM)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("����(��������ļ���)"),4,4,hRootDataTypeCode);
	m_wndClassView.InsertItem(_T("null(��ֵ)"),4,4,hRootDataTypeCode);

	HTREEITEM hRootEscapeChar = m_wndClassView.InsertItem(_T("ת���ַ�"),3,3,hRoot);
	m_wndClassView.InsertItem(_T("\\n   (����(LF) ������ǰλ���Ƶ���һ�п�ͷ)"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\r   (�س�(CR) ������ǰλ���Ƶ����п�ͷ)"),4,4,hRootEscapeChar);	
	m_wndClassView.InsertItem(_T("\\r\\n(�س�����(CRLF) ����Щ�ļ��༭��,��Ҫ\\r\\n �����л���Ч��)"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\t   (ˮƽ�Ʊ�(HT) ��������һ��TABλ��))"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\\\  (����һ����б���ַ� \\)"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\\"  (����һ��˫�����ַ� \")"),4,4,hRootEscapeChar);
	m_wndClassView.InsertItem(_T("\\\'  (����һ���������ַ� \')"),4,4,hRootEscapeChar);

	HTREEITEM hRootTimeFormat = m_wndClassView.InsertItem(_T("ʱ���ʽ"),3,3,hRoot);
	m_wndClassView.InsertItem(_T("ʱ���ʽ  2012/12/3 11:20:30"),4,4,hRootTimeFormat);

    //չ������Ҫ�����нڵ������ɺ����չ��
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
		// ѡ���ѵ�������:
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
			//���ò��ҹ���
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
	AfxMessageBox(_T("��ӳ�Ա����..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: �ڴ˴���������������
}

void CClassView::OnClassDefinition()
{
	// TODO: �ڴ˴���������������
}

void CClassView::OnClassProperties()
{
	// TODO: �ڴ˴���������������
}

void CClassView::OnNewFolder()
{
	//AfxMessageBox(_T("�½��ļ���..."));
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

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
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
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
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* ����*/);
}


BOOL CClassView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    *pResult = 0;

	CMainFrame *pMain=(CMainFrame*)AfxGetMainWnd();

    HTREEITEM mySelected = m_wndClassView.GetSelectedItem() ;
    CString strSelectedText = m_wndClassView.GetItemText(mySelected) ;
    HTREEITEM mySelectedPar = m_wndClassView.GetParentItem(mySelected) ;
    CString strSelectedParText = m_wndClassView.GetItemText(mySelectedPar);

    FCTREENODETYPE nodeType=(FCTREENODETYPE)m_wndClassView.GetItemData(mySelected);

    if(nodeType==FCTREENODETYPE::FCTREE_FUNCTION)
    {	
		//������Ǻ�����ʱ���ü�����������
		pMain->ActiveDecWnd();
		CString example = CTCRunTimeManager::g_TCAip.GetExample(strSelectedParText,strSelectedText);
		example.Trim();
		CString XMLStr=example;
		//�õ�����ԭ��
		CString str;
		CString tempFunProto;
		int n;
		n=XMLStr.Find(L"|");
		str=XMLStr.Right(XMLStr.GetLength()-n-1);
		n=str.Find(L"|");
		tempFunProto=str.Left(n);
		tempFunProto.Trim();
		//�õ�����ԭ��
		//����˵��
		CString tempFunDes;
		n=str.Find(L"|");
		str=XMLStr.Right(str.GetLength()-n-1);
		n=str.Find(L"|");
		str=XMLStr.Right(str.GetLength()-n-1);
		n=str.Find(L"|");
		tempFunDes=str.Left(n);
		tempFunDes.Trim();
		//�õ�����
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

	//���ò����α�
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
	 
	//����ҵ��ִ��Ѿ��ǲ��ҹ������ò���
	if((selectItem != NULL) && (0 != m_claViewFound.seekStr.CompareNoCase(strSeek)))
	{
		m_claViewFound.Reset();
		m_clickCount = 0;
		TravelTCTreeView(selectItem,strSeek);
	}
	//���Խ�磬��λ�����
	if(m_clickCount > m_claViewFound.allItems.size() - 1)
		m_clickCount = 0;

	if(m_claViewFound.allItems.size() > 0)
	{
		HTREEITEM hItem = m_claViewFound.allItems[m_clickCount];
		m_wndClassView.Expand(m_wndClassView.GetRootItem(), TVE_EXPAND);
		//���и��ڵ�չ��
		HTREEITEM parentItem = m_wndClassView.GetParentItem(hItem);
		while (parentItem != NULL)
		{
			m_wndClassView.Expand(parentItem, TVE_EXPAND);
			parentItem = m_wndClassView.GetParentItem(parentItem);
		}
		//������ӽڵ㣬չ����ǰ�ڵ�
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
/*  \�������ƣ�TravelTCTreeView
/*	\�������ܣ�����TC�������Ҹ������ҵ��Ľڵ�
/*	\�������أ�void
/*	\����˵����@HTREEITEM hItem���ҽڵ� @CString inStrҪƥ����ִ�
/***********************************************************************/

void CClassView::TravelTCTreeView(HTREEITEM hItem,CString inStr)
{
	if (m_wndClassView.ItemHasChildren(hItem))
	{
		HTREEITEM childItem = m_wndClassView.GetChildItem(hItem);
		while (childItem != NULL)
		{
			//����ǿռ䣬Ҳ��ӵ�������Ϣ�ṹ��
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