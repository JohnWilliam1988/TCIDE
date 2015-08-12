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

#pragma once

////////自定义////////
#include "matchLib.h"
////////自定义////////
#include "InPlaceCombo.h"
#include <vector>
using namespace std;

const int MAXERRORLENGTH=128;          //最多记录10个错误记录,不过目前应该最多只会有一个

//三个Tab窗口的ID,用于激活时查找位置
#define  ID_OUTPUT_MSG				1
#define  ID_OUTPUT_VAR				2
#define  ID_OUTPUT_CONTENT			3

//////////////////////变量查看窗口下拉框相关宏
#define FIRST_COLUMN				0
#define MIN_COLUMN_WIDTH			10
#define MAX_DROP_DOWN_ITEM_COUNT	10

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口

class COutputList : public CListBox
{
// 构造
public:
	COutputList();

// 实现
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

struct TCGRAMMARERROR
{
    int nRow;
    TCHAR file[MAX_PATH];
    int fileRow;
    TCGRAMMARERROR()
    {
        nRow=-1;
        fileRow=-1;
        ::memset(file,0,MAX_PATH*sizeof(TCHAR));
    }
};

struct TCARRAYDEBUGINFO
{
	CString arrayName;
	CStringList arrayList;
	TCARRAYDEBUGINFO()
	{
		arrayName.Empty();
		arrayList.RemoveAll();
	}
};

class CTCListCtrl : public CMFCListCtrl
{
    // 构造
public:
    CTCListCtrl();  
    // 实现
public:
    virtual ~CTCListCtrl();
    virtual COLORREF OnGetCellTextColor(int nRow, int nColum);
protected:
    /*afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);*/
    afx_msg void OnViewOutput();
    DECLARE_MESSAGE_MAP()
public:
    void ResetList();
    void SetColor(bool set=true);
    void AddErrorRow(int nRow,CString file,int fileRow);
private:
    TCGRAMMARERROR m_nRowError[MAXERRORLENGTH];
    int m_nErrorLength;
    BOOL m_bColor;
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

class CTCVarListCtrl : public CMFCListCtrl
{
	// 构造
public:
	CTCVarListCtrl();  
	// 实现
public:
	virtual ~CTCVarListCtrl();
protected:
	DECLARE_MESSAGE_MAP()
public:
	void ResetList();
	// Sets/Resets the column which support the in place combo box
	void SetComboColumns(int iColumnIndex, bool bSet = true);

	// Sets/Resets the column which support the in place edit control
	void SetReadOnlyColumns(int iColumnIndex, bool bSet = true);

	// Sets the valid characters for the edit ctrl
	void SetValidEditCtrlCharacters(CString& rstrValidCharacters);
	// Sets the vertical scroll
	void EnableVScroll(bool bEnable = true);

	// Sets the horizontal scroll
	void EnableHScroll(bool bEnable = true);
private:
	// Returns the row & column index of the column on which mouse click event has occured
	bool HitTestEx(CPoint& rHitPoint, int* pRowIndex, int* pColumnIndex) const;
	// Creates and displays the in place combo box
	CInPlaceCombo* ShowInPlaceList(int iRowIndex, int iColumnIndex, CStringList& rComboItemsList, 
		CString strCurSelecetion = _T(""), int iSel = -1);
	// Calculates the cell rect
	void CalculateCellRect(int iColumnIndex, int iRowIndex, CRect& robCellRect);
	// Scrolls the list ctrl to bring the in place ctrl to the view
	void ScrollToView(int iColumnIndex, /*int iOffSet, */CRect& obCellRect);
	// Checks whether column supports in place combo box
	bool IsCombo(int iColumnIndex);
	// Checks whether column is read only
	bool IsReadOnly(int iColumnIndex);
	// The window style of the in place combo ctrl
	DWORD m_dwDropDownCtrlStyle;
	// List of columns that are read only
	CList<int, int> m_ReadOnlyColumnsList;
	// List of columns that support the in place combo box
	CList<int, int> m_ComboSupportColumnsList;
public:
	//所有数组信息
	vector< TCARRAYDEBUGINFO* > m_ArrayList;
	void DeleteInpalaceCombox();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
};


class COutputWnd : public CDockablePane
{
// 构造
public:
	COutputWnd();

	void UpdateFonts();

// 特性
protected:
	CMFCTabCtrl	m_wndTabs;

	CTCListCtrl m_wndOutputMsg;
	CTCVarListCtrl m_wndOutputVar;
	//COutputList m_wndOutputFind;
	CTCListCtrl m_wndOutputFind;

	bool m_isHex;		//标识是否当前显示的是十六进制

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

//外部接口
public:
    void TCClearOutMsg();
	void ClearOutputVar();
    void TCOutMsgAddString(CString csText,bool isError=false,CString file=_T(""),int fileRow=-1);
    //调试的时候加载可见的变量列表
    void TCDebugLoadVarList(TCHAR *pData);  

	void TCClearAllContentWindow();
	void TCAddAllContentWindow(CString csText);

	void TCSetOutputMsgActive();
	void TCSetAllContentWinActive();

	//十进制转十六进制
	CString DecToHex(CString strDec);
	//十六进制转十进制
	CString HexToDec(CString HexNum);

// 实现
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVarSixteen();
	afx_msg void OnUpdateVarSixteen(CCmdUI *pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDebugCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditCopyrow();
	afx_msg void OnUpdateEditCopyrow(CCmdUI *pCmdUI);
	afx_msg void OnContentJump();
	afx_msg void OnContentCopy();
	afx_msg void OnContentRemove();
	afx_msg void OnUpdateContentJump(CCmdUI *pCmdUI);
	afx_msg void OnUpdateContentCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateContentRemove(CCmdUI *pCmdUI);
};

