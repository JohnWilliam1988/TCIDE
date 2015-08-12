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

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "TCDoc.h"
#include "TCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
///////////////COutputList1/////////////////////////////////////////////////

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("复制输出"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("清除输出"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
////////////////////CTCListCtrl/////////////////////////////////////
CTCListCtrl::CTCListCtrl()
{
    m_bColor=false;
    ::memset(m_nRowError,0,MAXERRORLENGTH*sizeof(int));
    m_nErrorLength=0;
}

CTCListCtrl::~CTCListCtrl()
{
}

BEGIN_MESSAGE_MAP(CTCListCtrl, CMFCListCtrl)
	/*ON_WM_CONTEXTMENU()*/
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

//void CTCListCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
//{
//	CMenu menu;
//	menu.LoadMenu(IDR_OUTPUT_POPUP);
//
//	CMenu* pSumMenu = menu.GetSubMenu(0);
//
//	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
//	{
//		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
//
//		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
//			return;
//
//		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
//		UpdateDialogControls(this, FALSE);
//	}
//	SetFocus();
//}

COLORREF CTCListCtrl::OnGetCellTextColor(int nRow, int nColum)
{
    if (!m_bColor)
    {
        return CMFCListCtrl::OnGetCellTextColor(nRow, nColum);
    }

    for(int i=0;i<m_nErrorLength;i++)
    {
        if(m_nRowError[i].nRow ==  nRow)
             return RGB(205,55,0);
    }
    return RGB(0, 128, 0);
}

/////////////////////////警告这里切换外观风格的时候 还是会报错,match-警告///
void CTCListCtrl::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
/////////////////////////警告这里切换外观风格的时候 还是会报错,match-警告////

void CTCListCtrl::ResetList()
{
    DeleteAllItems();
    m_nErrorLength=0;
}

void CTCListCtrl::SetColor(bool set)
{
    m_bColor=set;
}

void CTCListCtrl::AddErrorRow(int nRow,CString file,int fileRow)
{
    if(m_nErrorLength>=MAXERRORLENGTH)
        return;

    m_nRowError[m_nErrorLength].nRow=nRow;
    ::wcscpy_s(m_nRowError[m_nErrorLength].file,file);
    m_nRowError[m_nErrorLength].fileRow=fileRow;
    m_nErrorLength++;
    return;
}

void CTCListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int item = this->GetHotItem();
	CString info = _T("");
	info = this->GetItemText(item,0);
	if( _T(">错误:") == info.Left(4)) 
	{
		int statr = info.Find(_T("行号:"));
		int end = info.Find(_T(" 错误信息:"));

		CString strpos;
		strpos = info.Mid(statr+3,end-statr-3);
		int line = _ttoi(strpos); 

		CString filePath = info.Mid(4,statr-5);

		if(line!=-1)
		{
			CTCView *pView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(filePath);
			pView->TCGotoLine(line);
		}
	}
	else if( _T("行号:") == info.Left(3))
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CTCView *pView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[item].path);

		pView->TCGotoLine(pFrame->m_findInfo[item].lineNumber);
		pView->TCContentText(pFrame->m_findInfo[item].strBeginPos,pFrame->m_findInfo[item].strEndPos);

	}
	CMFCListCtrl::OnLButtonDblClk(nFlags, point);
}

////////////////////CTCListCtrl////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
////////////////////CTCVarListCtrl/////////////////////////////////////
CTCVarListCtrl::CTCVarListCtrl()
{
	m_ComboSupportColumnsList.RemoveAll();
	m_ReadOnlyColumnsList.RemoveAll();
	m_dwDropDownCtrlStyle = /*WS_BORDER |*/ WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | 
		CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
}

CTCVarListCtrl::~CTCVarListCtrl()
{
	CInPlaceCombo::DeleteInstance();
}

BEGIN_MESSAGE_MAP(CTCVarListCtrl, CMFCListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginLabelEdit)
END_MESSAGE_MAP()


void CTCVarListCtrl::ResetList()
{
	DeleteAllItems();
}

void CTCVarListCtrl::SetComboColumns(int iColumnIndex, bool bSet /*= true*/)
{
	// If the Column Index is not present && Set flag is false
	// Then do nothing 
	// If the Column Index is present && Set flag is true
	// Then do nothing
	POSITION Pos = m_ComboSupportColumnsList.Find(iColumnIndex);

	// If the Column Index is not present && Set flag is true
	// Then Add to list
	if ((NULL == Pos) && bSet) 
	{
		m_ComboSupportColumnsList.AddTail(iColumnIndex); 
	}

	// If the Column Index is present && Set flag is false
	// Then Remove from list
	if ((NULL != Pos) && !bSet) 
	{
		m_ComboSupportColumnsList.RemoveAt(Pos); 
	}
}

void CTCVarListCtrl::SetReadOnlyColumns(int iColumnIndex, bool bSet /*= true*/)
{
	// If the Column Index is not present && Set flag is false
	// Then do nothing 
	// If the Column Index is present && Set flag is true
	// Then do nothing
	POSITION Pos = m_ReadOnlyColumnsList.Find(iColumnIndex);

	// If the Column Index is not present && Set flag is true
	// Then Add to list
	if ((NULL == Pos) && bSet) 
	{
		m_ReadOnlyColumnsList.AddTail(iColumnIndex); 
	}

	// If the Column Index is present && Set flag is false
	// Then Remove from list
	if ((NULL != Pos) && !bSet) 
	{
		m_ReadOnlyColumnsList.RemoveAt(Pos); 
	}
}

void CTCVarListCtrl::EnableHScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_dwDropDownCtrlStyle |= WS_HSCROLL;
	}
	else
	{
		m_dwDropDownCtrlStyle &= ~WS_HSCROLL;
	}	
}

void CTCVarListCtrl::EnableVScroll(bool bEnable /*= true*/)
{
	if (bEnable)
	{
		m_dwDropDownCtrlStyle |= WS_VSCROLL;
	}
	else
	{
		m_dwDropDownCtrlStyle &= ~WS_VSCROLL;
	}
}

void CTCVarListCtrl::OnHScroll(UINT iSBCode, UINT iPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	if (GetFocus() != this)
	{
		SetFocus();
	}

	CMFCListCtrl::OnHScroll(iSBCode, iPos, pScrollBar);
}

void CTCVarListCtrl::OnVScroll(UINT iSBCode, UINT iPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	if (GetFocus() != this)
	{
		SetFocus();
	}

	CMFCListCtrl::OnVScroll(iSBCode, iPos, pScrollBar);
}

bool CTCVarListCtrl::HitTestEx(CPoint &obPoint, int* pRowIndex, int* pColumnIndex) const
{
	if (!pRowIndex || !pColumnIndex)
	{
		return false;
	}

	// Get the row index
	*pRowIndex = HitTest(obPoint, NULL);

	if (pColumnIndex)
	{
		*pColumnIndex = 0;
	}

	// Make sure that the ListView is in LVS_REPORT
	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
	{
		return false;
	}

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

	int iColumnCount = pHeader->GetItemCount();

	// Get bounding rect of item and check whether obPoint falls in it.
	CRect obCellRect(0,0,0,0);
	GetItemRect(*pRowIndex, &obCellRect, LVIR_BOUNDS);

	if (obCellRect.PtInRect(obPoint))
	{
		// Now find the column
		for (*pColumnIndex = 0; *pColumnIndex < iColumnCount; (*pColumnIndex)++)
		{
			int iColWidth = GetColumnWidth(*pColumnIndex);

			if (obPoint.x >= obCellRect.left && obPoint.x <= (obCellRect.left + iColWidth))
			{
				return true;
			}
			obCellRect.left += iColWidth;
		}
	}
	return false;
}

CInPlaceCombo* CTCVarListCtrl::ShowInPlaceList(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, 
	CString strCurSelecetion /*= ""*/, int iSel /*= -1*/)
{
	// The returned obPointer should not be saved

	// Make sure that the item is visible
	if (!EnsureVisible(iRowIndex, TRUE))
	{
		return NULL;
	}

	// Make sure that iColumnIndex is valid 
	CHeaderCtrl* pHeader = static_cast<CHeaderCtrl*> (GetDlgItem(FIRST_COLUMN));

	int iColumnCount = pHeader->GetItemCount();

	if (iColumnIndex >= iColumnCount || GetColumnWidth(iColumnIndex) < MIN_COLUMN_WIDTH) 
	{
		return NULL;
	}

	// Calculate the rectangle specifications for the combo box
	CRect obCellRect(0, 0, 0, 0);
	CalculateCellRect(iColumnIndex, iRowIndex, obCellRect);

	int iHeight = obCellRect.Height();  
	int iCount = rComboItemsList.GetCount();

	iCount = (iCount < MAX_DROP_DOWN_ITEM_COUNT) ? 
		iCount + MAX_DROP_DOWN_ITEM_COUNT : (MAX_DROP_DOWN_ITEM_COUNT + 1); 

	obCellRect.bottom += iHeight * iCount; 

	// Create the in place combobox
	CInPlaceCombo* pInPlaceCombo = CInPlaceCombo::GetInstance();
	pInPlaceCombo->ShowComboCtrl(m_dwDropDownCtrlStyle, obCellRect, this, 0, iRowIndex, iColumnIndex, &rComboItemsList, 
		strCurSelecetion, iSel);

	return pInPlaceCombo;
}

void CTCVarListCtrl::CalculateCellRect(int iColumnIndex, int iRowIndex, CRect& robCellRect)
{
	GetItemRect(iRowIndex, &robCellRect, LVIR_BOUNDS);

	CRect rcClient;
	GetClientRect(&rcClient);

	if (robCellRect.right > rcClient.right) 
	{
		robCellRect.right = rcClient.right;
	}

	ScrollToView(iColumnIndex, robCellRect); 
}

void CTCVarListCtrl::DeleteInpalaceCombox()
{
	CInPlaceCombo* pInPlaceCombo = CInPlaceCombo::GetCurCombox();
	if( pInPlaceCombo != NULL )
	{
		//pInPlaceCombo->Clear();
		if( pInPlaceCombo->m_hWnd )
			pInPlaceCombo->SendMessage(WM_CLOSE,(WPARAM)GetParent()->m_hWnd,NULL);
		
	}
	return;
}


void CTCVarListCtrl::ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& robCellRect)
{
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(&rcClient);

	int iColumnWidth = GetColumnWidth(iColumnIndex);

	// Get the column iOffset
	int iOffSet = 0;
	for (int iIndex_ = 0; iIndex_ < iColumnIndex; iIndex_++)
	{
		iOffSet += GetColumnWidth(iIndex_);
	}

	// If x1 of cell rect is < x1 of ctrl rect or
	// If x1 of cell rect is > x1 of ctrl rect or **Should not ideally happen**
	// If the width of the cell extends beyond x2 of ctrl rect then
	// Scroll

	CSize obScrollSize(0, 0);

	if (((iOffSet + robCellRect.left) < rcClient.left) || 
		((iOffSet + robCellRect.left) > rcClient.right))
	{
		obScrollSize.cx = iOffSet + robCellRect.left;
	}
	else if ((iOffSet + robCellRect.left + iColumnWidth) > rcClient.right)
	{
		obScrollSize.cx = iOffSet + robCellRect.left + iColumnWidth - rcClient.right;
	}

	Scroll(obScrollSize);
	robCellRect.left -= obScrollSize.cx;

	// Set the width to the column width
	robCellRect.left += iOffSet;
	robCellRect.right = robCellRect.left + iColumnWidth;
}

bool CTCVarListCtrl::IsCombo(int iColumnIndex)
{
	if (m_ComboSupportColumnsList.Find(iColumnIndex))
	{
		return true;
	}

	return false;
}

void CTCVarListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int iColumnIndex = -1;
	int iRowIndex = -1;

	// Get the current column and row
	if (!HitTestEx(point, &iRowIndex, &iColumnIndex))
	{
		return;
	}

	CMFCListCtrl::OnLButtonDown(nFlags, point);

	// If column is not read only then
	// If the SHIFT or CTRL key is down call the base class
	// Check the high bit of GetKeyState to determine whether SHIFT or CTRL key is down
	if ((GetKeyState(VK_SHIFT) & 0x80) || (GetKeyState(VK_CONTROL) & 0x80))
	{
		return;
	}

	// Get the current selection before creating the in place combo box
	CString strCurSelection = GetItemText(iRowIndex, iColumnIndex);

	if (-1 != iRowIndex)
	{
		UINT flag = LVIS_FOCUSED;

		if ((GetItemState(iRowIndex, flag ) & flag) == flag)
		{
			// Add check for LVS_EDITLABELS
			if (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS)
			{
				// If combo box is supported
				// Create and show the in place combo box
				if (IsCombo(iColumnIndex) && (_T("数组") == GetItemText(iRowIndex ,2 )) )
				{
					for( int count = 0; count < m_ArrayList.size(); count ++ )
					{
						if( (m_ArrayList[ count ]->arrayName == GetItemText(iRowIndex , 1)) && (m_ArrayList[ count ]->arrayList.GetCount() > 1))
						{ 

							CInPlaceCombo* pInPlaceComboBox = ShowInPlaceList(iRowIndex ,iColumnIndex, m_ArrayList[ count ]->arrayList, strCurSelection);
							ASSERT(pInPlaceComboBox); 

							
							// Set the selection to previous selection
							pInPlaceComboBox->SelectString(-1, strCurSelection);
							
						}
					}				
				}
				// If combo box is not read only
			}
		}
	}  
	//CMFCListCtrl::OnLButtonDown(nFlags, point);
}

bool CTCVarListCtrl::IsReadOnly(int iColumnIndex)
{
	if (m_ReadOnlyColumnsList.Find(iColumnIndex))
	{
		return true;
	}

	return false;
}


void CTCVarListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMFCListCtrl::OnLButtonDblClk(nFlags, point);
}

void CTCVarListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	// Update the item text with the new text
	SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);

	//GetParent()->SendMessage(WM_VALIDATE, GetDlgCtrlID(), (LPARAM)pDispInfo); 

	*pResult = 0;
}


void CTCVarListCtrl::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (IsReadOnly(pDispInfo->item.iSubItem))
	{
		*pResult = 1;
		return;
	}

	*pResult = 0;
}

////////////////////CTCVarListCtrl////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
	m_isHex = false;
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VAR_HEX, &COutputWnd::OnVarSixteen)
	ON_UPDATE_COMMAND_UI(ID_VAR_HEX, &COutputWnd::OnUpdateVarSixteen)   
	ON_COMMAND(ID_DEBUG_COPY, &COutputWnd::OnDebugCopy)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_COPY, &COutputWnd::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPYROW, &COutputWnd::OnEditCopyrow)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPYROW, &COutputWnd::OnUpdateEditCopyrow)
	ON_COMMAND(ID_CONTENT_JUMP, &COutputWnd::OnContentJump)
	ON_COMMAND(ID_CONTENT_COPY, &COutputWnd::OnContentCopy)
	ON_COMMAND(ID_CONTENT_REMOVE, &COutputWnd::OnContentRemove)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_JUMP, &COutputWnd::OnUpdateContentJump)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_COPY, &COutputWnd::OnUpdateContentCopy)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_REMOVE, &COutputWnd::OnUpdateContentRemove)
END_MESSAGE_MAP()

void COutputWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{

	int index = m_wndTabs.GetActiveTab();
	CString tabLable;
	m_wndTabs.GetTabLabel(index,tabLable);
	if(tabLable == _T("变量查看"))
	{
		CMenu menu;
		menu.LoadMenu(IDR_OUTPUT_POPUP);

		CMenu* pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
		{
			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
				return;

			((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}
		//SetFocus();
	}	
	else if(tabLable == _T("查找"))
	{
		CMenu menu;
		menu.LoadMenu(IDR_CONTENT_POPUP);

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
}

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_3D_VS2005, rectDummy, this, 1))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	// 创建输出窗格:
	//const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	const DWORD dwMsgStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_SINGLESEL | LVS_REPORT |LVS_NOCOLUMNHEADER;
	const DWORD dwVarStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_SINGLESEL | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE;

	if ( !m_wndOutputMsg.Create(dwMsgStyle, rectDummy, &m_wndTabs, ID_OUTPUT_MSG)||
		 !m_wndOutputVar.Create(dwVarStyle, rectDummy, &m_wndTabs, ID_OUTPUT_VAR)||
		 !m_wndOutputFind.Create(dwMsgStyle, rectDummy, &m_wndTabs, ID_OUTPUT_CONTENT))
	{
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}
  
    m_wndOutputMsg.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, /*LVS_EX_FULLROWSELECT |*/LVS_EX_INFOTIP);
    m_wndOutputVar.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

	 m_wndOutputFind.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, /*LVS_EX_FULLROWSELECT |*/LVS_EX_INFOTIP);


    UpdateFonts();

    m_wndOutputMsg.InsertColumn(0, _T(""), LVCFMT_LEFT, 1000);
    m_wndOutputMsg.SetColor();

	m_wndOutputFind.InsertColumn(0,_T(""), LVCFMT_LEFT, 1000);
	m_wndOutputFind.SetColor();

    //m_wndOutputMsg.m_nRowError=1;
    m_wndOutputVar.InsertColumn(0, _T("空间名"), LVCFMT_LEFT, 80);
    m_wndOutputVar.InsertColumn(1, _T("变量名"), LVCFMT_LEFT, 80);
    m_wndOutputVar.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 80);
    m_wndOutputVar.InsertColumn(3, _T("值"), LVCFMT_LEFT, 360);

	m_wndOutputVar.SetReadOnlyColumns(0);
	m_wndOutputVar.SetReadOnlyColumns(1);
	m_wndOutputVar.SetReadOnlyColumns(2);
	m_wndOutputVar.SetComboColumns(3);
	m_wndOutputVar.EnableVScroll();

	//m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString strTabName;
	BOOL bNameValid;

	// 将列表窗口附加到选项卡:
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputMsg, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputVar, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	// 使用一些虚拟文本填写输出选项卡(无需复杂数据)
	FillBuildWindow();
	FillDebugWindow();
	FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
    //这里是输出框信息
    //TCOutMsgAddString(_T("生成输出正显示在此处1"));
    //TCOutMsgAddString(_T("错误:生成输出正显示在此处2 文件...路径......"));
    //TCOutMsgAddString(_T("生成输出正显示在此处3"));
    //TCOutMsgAddString(_T("生成输出正显示在此处4"));
}

void COutputWnd::FillDebugWindow()
{
	//m_wndOutputDebug.AddString(_T("调试输出正显示在此处。"));
	//m_wndOutputDebug.AddString(_T("输出正显示在列表视图的行中"));
	//m_wndOutputDebug.AddString(_T("但您可以根据需要更改其显示方式..."));
	//m_wndOutputVar.InsertItem(0, _T("我的程序"));
	//m_wndOutputVar.SetItemText(0, 1, _T("坐标"));
    //m_wndOutputVar.SetItemText(0, 2, _T("1004444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444"));
}

void COutputWnd::FillFindWindow()
{
	m_wndOutputFind.InsertItem(0,_T("显示查看内容"));
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputMsg.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputVar.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::TCClearOutMsg()
{
    m_wndOutputMsg.ResetList();
	m_isHex = false;
}

void COutputWnd::ClearOutputVar()
{
	m_wndOutputVar.DeleteAllItems();
	//如果有显示下拉框,则关闭下拉框
	m_wndOutputVar.DeleteInpalaceCombox();
	m_isHex = false;
}

void COutputWnd::TCClearAllContentWindow()
{
	m_wndOutputFind.DeleteAllItems();
}

void COutputWnd::TCSetOutputMsgActive()
{
	int iTab = m_wndTabs.GetTabByID(ID_OUTPUT_MSG);
	m_wndTabs.SetActiveTab(iTab);
}

void COutputWnd::TCSetAllContentWinActive()
{
	int iTab = m_wndTabs.GetTabByID(ID_OUTPUT_CONTENT);
	m_wndTabs.SetActiveTab(iTab);
}

void COutputWnd::TCOutMsgAddString(CString csText,bool isError,CString file,int fileRow)
{
    int index=m_wndOutputMsg.GetItemCount();
    m_wndOutputMsg.InsertItem(index,csText);
    if(isError)
    {
        m_wndOutputMsg.AddErrorRow(index,file,fileRow);
        m_wndOutputMsg.RedrawWindow();
    }
	    //把滑轮滑到底部
    m_wndOutputMsg.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
}


void COutputWnd::TCAddAllContentWindow(CString csText)
{
	int index=m_wndOutputFind.GetItemCount();
	m_wndOutputFind.InsertItem(index,csText);
	m_wndOutputFind.SendMessage(WM_VSCROLL,SB_BOTTOM,0);
}

void COutputWnd::TCDebugLoadVarList(TCHAR *pData)
{
	m_wndOutputVar.DeleteAllItems();
	m_wndOutputVar.m_ArrayList.clear();
	m_wndOutputVar.DeleteInpalaceCombox();
	while((!XMLString::startsWith(pData,L"null"))&&((*pData)))
	{
		int nCurrentLength=m_wndOutputVar.GetItemCount();
		int nItem;

		//空间名
		nItem=m_wndOutputVar.InsertItem(nCurrentLength,pData);
		pData+=XMLString::stringLen(pData)+1;

		TCHAR *tempArrayName = NULL;
		//变量名
		m_wndOutputVar.SetItemText(nItem,1,pData);
		tempArrayName = pData;
		pData+=XMLString::stringLen(pData)+1;

		//类型
		m_wndOutputVar.SetItemText(nItem,2,pData);
		bool isArray=false;
		bool isString=false;
		if(XMLString::equals(pData,L"数组"))
			isArray=true;

		if(XMLString::equals(pData,L"字符型"))
			isString=true;

		pData+=XMLString::stringLen(pData)+1;

		//数据
		if(isArray)
		{
			TCARRAYDEBUGINFO *arrayInfo = new TCARRAYDEBUGINFO;
			arrayInfo->arrayName = tempArrayName;
			//判读数组的头
			if(*pData==0XA5A6)
				pData+=2;

			//判读数组的尾部,遇到就结束
			while(*pData!=0XA6A5)
			{
				arrayInfo->arrayList.AddTail(pData);
				pData+=XMLString::stringLen(pData)+1;
			}

			if(arrayInfo->arrayList.GetCount() > 0)
			{
				m_wndOutputVar.SetItemText(nItem,3,arrayInfo->arrayList.GetHead());
				m_wndOutputVar.m_ArrayList.push_back( arrayInfo );
			}	
			pData+=2;
		}
		else
		{
			if( m_isHex && (m_wndOutputVar.GetItemText(nItem,2) == _T("int") || m_wndOutputVar.GetItemText(nItem,2) == _T("整型")) )
			{
				CString hexstr;
				hexstr.Format(_T("#%s"),DecToHex(pData));
				m_wndOutputVar.SetItemText(nItem,3,hexstr);
				pData+=XMLString::stringLen(pData)+1;
			}
			else if(isString)
			{
				CString csData;
				csData.Format(L"\"%s\"",pData);
				m_wndOutputVar.SetItemText(nItem,3,csData);
				pData+=XMLString::stringLen(pData)+1;
			}
			else
			{
				m_wndOutputVar.SetItemText(nItem,3,pData);
				pData+=XMLString::stringLen(pData)+1;
			}
		}
	}
}




BOOL COutputWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//拦截错误输出窗口的
	if(pMsg->message == WM_LBUTTONDBLCLK && pMsg->hwnd == m_wndOutputMsg.m_hWnd)
	{
		
	}
	return CDockablePane::PreTranslateMessage(pMsg);
}




void COutputWnd::OnVarSixteen()
{
	// TODO: 在此添加命令处理程序代码
	for(int count = 0;count < m_wndOutputVar.GetItemCount();count++ )
	{
		if(m_wndOutputVar.GetItemText(count,2) == _T("int") || m_wndOutputVar.GetItemText(count,2) == _T("整型"))
		{
			if(!m_isHex)
			{
				//十进制转十六进制
				CString hexstr;
				hexstr.Format(_T("#%s"),DecToHex(m_wndOutputVar.GetItemText(count,3)));
				m_wndOutputVar.SetItemText(count,3,hexstr);
			}
			else
			{
				//十六进制转十进制
				CString decstr = m_wndOutputVar.GetItemText(count,3).Right(m_wndOutputVar.GetItemText(count,3).GetLength()-1);
				m_wndOutputVar.SetItemText(count,3,HexToDec(decstr));
			}
		}
	}
	m_isHex = !m_isHex;
}

CString COutputWnd::DecToHex(CString strDec)
{
	//int test=::_wtoi(strDec);
	CStringA strTemp;
	strTemp = strDec;
	int dblData = 0;
	CString strHec;	
	::sscanf(strTemp.GetBuffer(),"%d",&dblData);
	strHec.Format(_T("%x"),dblData);
	return strHec;
}

CString COutputWnd::HexToDec(CString HexNum)
{
	CStringA strTemp;
	strTemp = HexNum;
	int dblData = 0;
	CString strDec;	
	::sscanf(strTemp.GetBuffer(),"%x",&dblData);
	strDec.Format(_T("%d"),dblData);
	return strDec;
}


void COutputWnd::OnUpdateVarSixteen(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	BOOL needToTrans = FALSE;
	for(int count = 0;count < m_wndOutputVar.GetItemCount();count++ )
	{
		if(m_wndOutputVar.GetItemText(count,2) == _T("int") || m_wndOutputVar.GetItemText(count,2) == _T("整型"))
		{
			needToTrans = TRUE;
		}
	}
	pCmdUI->Enable(needToTrans);
	pCmdUI->SetCheck(m_isHex);
}


void COutputWnd::OnDebugCopy()
{
	// TODO: 在此添加命令处理程序代码
	//定义字符型变量用来保存用户输入的数据
	int index = m_wndOutputVar.GetHotItem();
	CString source = m_wndOutputVar.GetItemText(index,3);
	CStringA str;
	str = source;
	if(OpenClipboard()) 
	{ 
		HGLOBAL clipbuffer; 
		char* buffer; 
		EmptyClipboard(); 
		clipbuffer = GlobalAlloc(GMEM_DDESHARE,str.GetLength()+1); 
		buffer = (char*)GlobalLock(clipbuffer); 
		strcpy(buffer,str.GetBuffer()); 
		GlobalUnlock(clipbuffer); 
		SetClipboardData(CF_TEXT,clipbuffer); 
		CloseClipboard(); 
	}
}



void COutputWnd::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_wndOutputVar.GetSelectedCount() > 0);
}


void COutputWnd::OnEditCopyrow()
{
	// TODO: 在此添加命令处理程序代码
	int index = m_wndOutputVar.GetHotItem();
	CString source = m_wndOutputVar.GetItemText(index,0) + _T("\t")+ m_wndOutputVar.GetItemText(index,1) + _T("\t")+ m_wndOutputVar.GetItemText(index,2) + _T("\t")+ m_wndOutputVar.GetItemText(index,3);
	CStringA str;
	str = source;
	if(OpenClipboard()) 
	{ 
		HGLOBAL clipbuffer; 
		char* buffer; 
		EmptyClipboard(); 
		clipbuffer = GlobalAlloc(GMEM_DDESHARE,str.GetLength()+1); 
		buffer = (char*)GlobalLock(clipbuffer); 
		strcpy(buffer,str.GetBuffer()); 
		GlobalUnlock(clipbuffer); 
		SetClipboardData(CF_TEXT,clipbuffer); 
		CloseClipboard(); 
	}
}


void COutputWnd::OnUpdateEditCopyrow(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_wndOutputVar.GetSelectedCount() > 0);
}


void COutputWnd::OnContentJump()
{
	// TODO: 在此添加命令处理程序代码
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int item = m_wndOutputFind.GetHotItem();
	if( -1 != item )
	{
		CTCView *pView=(CTCView *)((CTCApp*)AfxGetApp())->TCOpenTFileReturnView(pFrame->m_findInfo[item].path);
		pView->TCGotoLine(pFrame->m_findInfo[item].lineNumber);
		pView->TCContentText(pFrame->m_findInfo[item].strBeginPos,pFrame->m_findInfo[item].strEndPos);
	}
}


void COutputWnd::OnContentCopy()
{
	// TODO: 在此添加命令处理程序代码
	int index = m_wndOutputFind.GetHotItem();
	CString source = m_wndOutputFind.GetItemText(index,0);
	source.Trim();
	CStringA str;
	str = source;
	if(OpenClipboard()) 
	{ 
		HGLOBAL clipbuffer; 
		char* buffer; 
		EmptyClipboard(); 
		clipbuffer = GlobalAlloc(GMEM_DDESHARE,str.GetLength()+1); 
		buffer = (char*)GlobalLock(clipbuffer); 
		strcpy(buffer,str.GetBuffer()); 
		GlobalUnlock(clipbuffer); 
		SetClipboardData(CF_TEXT,clipbuffer); 
		CloseClipboard(); 
	}
}


void COutputWnd::OnContentRemove()
{
	// TODO: 在此添加命令处理程序代码
	m_wndOutputFind.DeleteAllItems();
}


void COutputWnd::OnUpdateContentJump(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_wndOutputFind.GetSelectedCount() > 0);
}


void COutputWnd::OnUpdateContentCopy(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_wndOutputFind.GetSelectedCount() > 0);
}


void COutputWnd::OnUpdateContentRemove(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_wndOutputFind.GetItemCount() > 0);
}
