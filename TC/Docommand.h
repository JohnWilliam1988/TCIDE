#include <vector>
#include "RedoUndo\undo.h"
#include "TCCtlHeads.h"
#include "TCTabPage.h"
#include "TCTabListBox.h"
#include "DiaTCDesign.h"
#include "TCRunTimeManager.h"
#include "MainFrm.h"
#include "TC.h"
#include "TCDesignView.h"
#pragma once

const int MAXSELECTCTRLS=280;
const int CTRLEVENTMAXSIZE=10;

using namespace std;

//////////////////添加控件的基类//////////////////////////
//													   //
//*************创建控件的撤销和恢复操作******************//
//			                                           //
//////////////////添加控件的基类//////////////////////////
class AddCtrlCommand : public UNDO::Command
{
private:
	// Data members
	CWnd* m_pParent;
	CWnd* m_pCtrl; 
	CRect m_rc;
	TCTOOLTYPE m_CtrlType;
	TCPARENTTYPE m_ParentWndtype;
	bool m_isPaste;			//标识是否是粘贴功能调用的
public:
	// Constructor
	//参数1:父窗口的指针，2：控件的位置,3:控件的类型,4:父窗口的类型
	AddCtrlCommand(CWnd *parent,CRect rc,TCTOOLTYPE type,TCPARENTTYPE parentType)
	{
		m_pParent = parent;
		m_CtrlType = type;
		m_ParentWndtype = parentType;
		m_rc = rc;
		switch(m_CtrlType)
		{
		case TCTT_STATIC:
			m_pCtrl = new CTCStatic();
			break;
		case TCTT_BUTTON:
			m_pCtrl = new CTCButton();
			break;
		case TCTT_EDIT:
			m_pCtrl = new CTCEdit();
			break;
		case TCTT_RADIO:
			m_pCtrl = new CTCRadio();
			break;
		case TCTT_CHECK:
			m_pCtrl = new CTCCheck();
			break;
		case TCTT_COMBOBOX:
			m_pCtrl = new CTCComboBox();
			break;
		case TCTT_GROUP:
			m_pCtrl = new CTCGroupBox();
			break;
		case TCTT_LISTBOX:
			m_pCtrl = new CTCListBox();
			break;
		case TCTT_PROGRESS:
			m_pCtrl = new CTCProgress();
			break;
		case TCTT_DATE:
			m_pCtrl = new CTCDate();
			break;
		case TCTT_PICTURE:
			m_pCtrl = new CTCPicture();
			break;
		case TCTT_SLIDER:
			m_pCtrl = new CTCSlider();
			break;
		case TCTT_PATH:
			m_pCtrl = new CTCBrowse();
			break;
		case TCTT_TIMER:
			m_pCtrl = new CTCTimer();
			break;
		case TCTT_HOTKEY:
			m_pCtrl = new CTCHotKeyCtrl();
			break;
		case TCTT_TAB:
			m_pCtrl = new CTCTab();
			break;
		case TCTT_EXPLORER:
			m_pCtrl = new CTCExplorer();
			break;
		case TCTT_GRIDCTRL:
			m_pCtrl = new CGridCtrl();
			break;
		default:
			break;
		}		
	}

	virtual ~AddCtrlCommand()
	{
	}

	// Methods
	virtual bool Execute()
	{
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			((CDiaTCDesign*)m_pParent)->DeSelectAll();
			switch(m_CtrlType)
			{
			case TCTT_BUTTON:
				((CTCButton*)m_pCtrl)->Create(_T("按钮"),WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCButton((CTCButton*)m_pCtrl,m_rc);
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_pCtrl)->Create(_T("标签"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_rc,m_pParent);
				((CDiaTCDesign*)m_pParent)->SetTCStatic((CTCStatic*)m_pCtrl,m_rc);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCEdit((CTCEdit*)m_pCtrl,m_rc);
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_pCtrl)->Create(_T("单选框"), WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCRadio((CTCRadio*)m_pCtrl,m_rc);
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_pCtrl)->Create(_T("复选框"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCCheck((CTCCheck*)m_pCtrl,m_rc);
				break;
			case TCTT_COMBOBOX:
				((CTCComboBox*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCCombox((CTCComboBox*)m_pCtrl,m_rc);
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_pCtrl)->Create(NULL,_T("分组框"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535,NULL);
				((CTCGroupBox*)m_pCtrl)->SetCatptionTextColor(RGB(100,149,237));
				((CDiaTCDesign*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_pCtrl,m_rc);
				break;
			case TCTT_LISTBOX:
				((CTCListBox*)m_pCtrl)->Create(_T("列表框"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCListBox*)m_pCtrl)->SetWindowTextW(_T("列表框"));
				((CDiaTCDesign*)m_pParent)->SetTCListBox((CTCListBox*)m_pCtrl,m_rc);
				break;
			case TCTT_PROGRESS:
				((CTCProgress*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCProgress*)m_pCtrl)->SetPos(70);
				((CDiaTCDesign*)m_pParent)->SetTCProgress((CTCProgress*)m_pCtrl,m_rc);
				break;
			case TCTT_DATE:
				((CTCDate*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCDate((CTCDate*)m_pCtrl,m_rc);
				break;
			case TCTT_PICTURE:
				((CTCPicture*)m_pCtrl)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCPicture((CTCPicture*)m_pCtrl,m_rc);
				break;
			case TCTT_SLIDER:
				((CTCSlider*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCSlider((CTCSlider*)m_pCtrl,m_rc);
				break;
			case TCTT_PATH:
				((CTCBrowse*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCBrowse*)m_pCtrl)->EnableFolderBrowseButton();
				//enable the browse button and put the control in the folder browse mode
				((CTCBrowse*)m_pCtrl)->EnableFileBrowseButton();
				((CDiaTCDesign*)m_pParent)->SetTCPath((CTCBrowse*)m_pCtrl,m_rc);
				break;
			case TCTT_TIMER:
				((CTCTimer*)m_pCtrl)->Create(_T("时钟"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|SS_REALSIZEIMAGE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCTimer*)m_pCtrl)->SetBitmap(((CTCTimer*)m_pCtrl)->m_bmp);
				((CDiaTCDesign*)m_pParent)->SetTCTimer((CTCTimer*)m_pCtrl,m_rc);
				break;
			case TCTT_HOTKEY:
				((CTCHotKeyCtrl*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCHotKey((CTCHotKeyCtrl*)m_pCtrl,m_rc);
				break;
			case TCTT_TAB:
				((CTCTab*)m_pCtrl)->Create(WS_VISIBLE|TCS_MULTILINE|TCS_TABS|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCTab*)m_pCtrl)->addPage(_T("tab1"));
				((CTCTab*)m_pCtrl)->addPage(_T("tab2"));
				((CDiaTCDesign*)m_pParent)->SetTCTab((CTCTab*)m_pCtrl,m_rc);
				break;
			case TCTT_EXPLORER:
				((CTCExplorer*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCExplorer((CTCExplorer*)m_pCtrl,m_rc);
				break;
			case TCTT_GRIDCTRL:
				((CGridCtrl*)m_pCtrl)->Create(m_rc,m_pParent,65535);
				((CGridCtrl*)m_pCtrl)->EnableDragAndDrop(TRUE);
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
				//pGridCtrl->GetDefaultCell(FALSE,FALSE)->SetTextClr(colorTexr);
				((CGridCtrl*)m_pCtrl)->SetVirtualMode(FALSE);
				((CGridCtrl*)m_pCtrl)->SetColumnCount(5);
				//初始没有固定列
				((CGridCtrl*)m_pCtrl)->SetFixedColumnCount(0);
				((CGridCtrl*)m_pCtrl)->SetRowCount(5);
				//初始设置一个固定行
				((CGridCtrl*)m_pCtrl)->SetFixedRowCount(1);
				for(int row = 0;row < ((CGridCtrl*)m_pCtrl)->GetRowCount(); row++)
				{
					((CGridCtrl*)m_pCtrl)->SetRowHeight(row,25);
				}
				for(int col = 0;col < ((CGridCtrl*)m_pCtrl)->GetColumnCount(); col++)
				{
					CString title;
					title.Format(_T("第 %d 列"),col);
					((CGridCtrl*)m_pCtrl)->SetItemText(0,col,title);
					((CGridCtrl*)m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);				
				}
				//设置标题项默认显示颜色
				((CGridCtrl*)m_pCtrl)->SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));
				((CDiaTCDesign*)m_pParent)->SetTCGridCtrl((CGridCtrl*)m_pCtrl,m_rc);
				break;
			default:
				break;
			}
			((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			//将别的选中状态置空
			((CTCTabPage*)m_pParent)->DeSelectAll();
			//显示当前操作空间所在的Tab页面
			for(int i=0;i<((CTCTab*)m_pParent->GetParent())->getTabPageCount();i++)
			{
				if( ((CTCTab*)m_pParent->GetParent())->m_Pages[i] == m_pParent )
				{
					((CTCTab*)m_pParent->GetParent())->SetCurSel(i);
					((CTCTab*)m_pParent->GetParent())->showIndex(i);
				}

			}
			switch(m_CtrlType)
			{
			case TCTT_BUTTON:
				((CTCButton*)m_pCtrl)->Create(_T("按钮"),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCButton((CTCButton*)m_pCtrl,m_rc);
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_pCtrl)->Create(_T("标签"), WS_CHILD|WS_VISIBLE|WS_EX_TRANSPARENT|SS_NOTIFY|WS_CLIPSIBLINGS ,m_rc,m_pParent);
				((CTCTabPage*)m_pParent)->SetTCStatic((CTCStatic*)m_pCtrl,m_rc);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCEdit((CTCEdit*)m_pCtrl,m_rc);
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_pCtrl)->Create(_T("单选框"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCRadio((CTCRadio*)m_pCtrl,m_rc);
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_pCtrl)->Create(_T("复选框"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCCheck((CTCCheck*)m_pCtrl,m_rc);
				break;
			case TCTT_COMBOBOX:
				((CTCComboBox*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCCombox((CTCComboBox*)m_pCtrl,m_rc);
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_pCtrl)->Create(NULL,_T("分组框"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535,NULL);
				((CTCTabPage*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_pCtrl,m_rc);
				break;
			case TCTT_LISTBOX:
				((CTCListBox*)m_pCtrl)->Create(_T("列表框"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCListBox*)m_pCtrl)->SetWindowTextW(_T("列表框"));
				((CTCTabPage*)m_pParent)->SetTCListBox((CTCListBox*)m_pCtrl,m_rc);
				break;
			case TCTT_PROGRESS:
				((CTCProgress*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCProgress*)m_pCtrl)->SetPos(70);
				((CTCTabPage*)m_pParent)->SetTCProgress((CTCProgress*)m_pCtrl,m_rc);
				break;
			case TCTT_PICTURE:
				((CTCPicture*)m_pCtrl)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|WS_BORDER|SS_NOTIFY|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCPicture((CTCPicture*)m_pCtrl,m_rc);
				break;
			case TCTT_SLIDER:
				((CTCSlider*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCSlider((CTCSlider*)m_pCtrl,m_rc);
				break;
			case TCTT_PATH:
				((CTCBrowse*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCBrowse*)m_pCtrl)->EnableFolderBrowseButton();
				////enable the browse button and put the control in the folder browse mode
				((CTCBrowse*)m_pCtrl)->EnableFileBrowseButton();
				((CTCTabPage*)m_pParent)->SetTCPath((CTCBrowse*)m_pCtrl,m_rc);
				break;
			case TCTT_HOTKEY:
				((CTCHotKeyCtrl*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCHotKey((CTCHotKeyCtrl*)m_pCtrl,m_rc);
				break;
			case TCTT_DATE:
				((CTCDate*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
                ((CTCTabPage*)m_pParent)->SetTCDate((CTCDate*)m_pCtrl,m_rc);
				break;
			//case TCTT_EXPLORER:
			//	((CTCExplorer*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
			//	//((CTCExplorer*)m_pCtrl)->Navigate(L"www.tcmade.com",NULL,NULL,NULL,NULL);
			//	((CTCTabPage*)m_pParent)->SetTCExplorer((CTCExplorer*)m_pCtrl,m_rc);
			//	break;
			case TCTT_GRIDCTRL:
				((CGridCtrl*)m_pCtrl)->Create(m_rc,m_pParent,65535);
				((CGridCtrl*)m_pCtrl)->EnableDragAndDrop(TRUE);
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
				((CGridCtrl*)m_pCtrl)->SetVirtualMode(FALSE);
				((CGridCtrl*)m_pCtrl)->SetColumnCount(5);
				//初始没有固定列
				((CGridCtrl*)m_pCtrl)->SetFixedColumnCount(0);
				((CGridCtrl*)m_pCtrl)->SetRowCount(5);
				//初始设置一个固定行
				((CGridCtrl*)m_pCtrl)->SetFixedRowCount(1);
				for(int row = 0;row < ((CGridCtrl*)m_pCtrl)->GetRowCount(); row++)
				{
					((CGridCtrl*)m_pCtrl)->SetRowHeight(row,25);
				}
				//设置列名跟列属性
				for(int col = 0;col < ((CGridCtrl*)m_pCtrl)->GetColumnCount(); col++)
				{
					CString title;
					title.Format(_T("第 %d 列"),col);
					((CGridCtrl*)m_pCtrl)->SetItemText(0,col,title);
					((CGridCtrl*)m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);			
				}
				//设置标题项默认显示颜色
				((CGridCtrl*)m_pCtrl)->SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));
				((CTCTabPage*)m_pParent)->SetTCGridCtrl((CGridCtrl*)m_pCtrl,m_rc);
				break;
			default:
				break;
			}
			//刷新
			((CTCTabPage*)m_pParent)->Invalidate(TRUE);
		}
		return true;
	}

	virtual bool Unexecute()
	{
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			//删除控件的时候将所有选中控件置空
			((CDiaTCDesign*)m_pParent)->m_ControlTracker.DeSelectAll();
			switch(m_CtrlType)
			{
			case TCTT_BUTTON:			
				break;
			case TCTT_STATIC:
				break;
			case TCTT_EDIT:
				break;
			case TCTT_LISTBOX:
				//在这儿直接将ListBox中的ListCtrl指针置空,就可以Redo不报错，但是不知道有没有什么弊端
				((CTCListBox*)m_pCtrl)->m_pWndList = NULL;
				break;
			case TCTT_PROGRESS:
				((CTCProgress*)m_pCtrl)->SetPos(70);
				break;
			case TCTT_TAB:
				for(int i=0;i<((CTCTab*)m_pCtrl)->getTabPageCount();i++)
				{
					((CTCTab*)m_pCtrl)->m_Pages[i]->DestroyWindow();
				}
				break;	
			default:
				break;
			}
			((CDiaTCDesign*)m_pParent)->m_ControlTracker.RemoveFromAll(m_pCtrl);
			((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);

			//移除控件后显示面板的属性，避免用户设置属性报错
			CString title;
			((CDiaTCDesign*)m_pParent)->GetWindowTextW(title);
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(title,(CDiaTCDesign*)m_pParent);
			//移除控件后显示面板的属性，避免用户设置属性报错
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			for(int i=0;i<((CTCTab*)m_pParent->GetParent())->getTabPageCount();i++)
			{
				if( ((CTCTab*)m_pParent->GetParent())->m_Pages[i] == m_pParent )
				{
					((CTCTab*)m_pParent->GetParent())->SetCurSel(i);
					((CTCTab*)m_pParent->GetParent())->showIndex(i);
				}

			}
			switch(m_CtrlType)
			{
			case TCTT_BUTTON:				
				break;
			case TCTT_STATIC:
				break;
			case TCTT_EDIT:
				break;
			case TCTT_LISTBOX:
				//在这儿直接将ListBox中的ListCtrl指针置空,就可以Redo不报错，但是不知道有没有什么弊端
				((CTCListBox*)m_pCtrl)->m_pWndList = NULL;
				break;
			case TCTT_PROGRESS:
				((CTCProgress*)m_pCtrl)->SetPos(70);
				break;
			case TCTT_TAB:
				break;
			default:
				break;
			}
			((CTCTabPage*)m_pParent)->m_ControlTracker.RemoveFromAll(m_pCtrl);
			((CTCTabPage*)m_pParent)->Invalidate(TRUE);

			/////////显示Tab属性避免控件属性暴露//////////////
			CString tabID;
			int size = ((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll.GetSize();
			for(int i=0;i<size;i++)
			{
				if(((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd() == m_pParent->GetParent() )
					tabID = ((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll[i]->GetCtlID();
			}

			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(tabID,(CTCTab*)m_pParent->GetParent());
			}
			m_pCtrl->DestroyWindow();
		
		return true;
	}
};



//////////////////添加Tab控件的动作///////////////////////
//													   //
//************创建Tab控件的撤销和恢复操作****************//
//													   //
//////////////////添加Tab控件的动作///////////////////////
class AddTabCommand : public UNDO::Command
{
private:
	//Data members
	CWnd* m_pParent;
	CWnd* m_pCtrl;
	CRect m_rc;
	CStringArray m_pageTitle;  //记录Tab的标签名称
	CTCTabPage *m_pTabPage[32]; //指向Tab子页面的指针数组
public:
	AddTabCommand(CWnd *parent,CRect rc)
	{
		m_pParent = parent;
		m_pCtrl = new CTCTab();
		m_rc = rc;
		m_pTabPage[0] = new CTCTabPage();
		m_pTabPage[1] = new CTCTabPage();
		m_pageTitle.Add(_T("tab1"));
		m_pageTitle.Add(_T("tab2"));
	}
	virtual ~AddTabCommand()
	{

	}

	// Methods
	virtual bool Execute()
	{
		((CTCTab*)m_pCtrl)->Create(WS_VISIBLE|TCS_MULTILINE|TCS_TABS|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);		
		for(int i=0;i<m_pageTitle.GetSize();i++)
		{
			((CTCTab*)m_pCtrl)->addItem(m_pageTitle[i]);
			((CTCTabPage*)m_pTabPage[i])->Create(IDD_TCTABPAGE,((CTCTab*)m_pCtrl));
			((CTCTab*)m_pCtrl)->m_Pages[i] = m_pTabPage[i];
		}
		//适宜范围
		((CTCTab*)m_pCtrl)->TCAdjustRect();
		//设置Tab相关信息
		((CDiaTCDesign*)m_pParent)->SetTCTab((CTCTab*)m_pCtrl,m_rc);
		return TRUE;
	}
	virtual bool Unexecute()
	{
		////删除对应的子页面
		for(int i=0;i<((CTCTab*)m_pCtrl)->getTabPageCount();i++)
		{
			((CTCTab*)m_pCtrl)->m_Pages[i]->DestroyWindow();
		}
		//最后删除Tab控件
		((CTCTab*)m_pCtrl)->DestroyWindow();
		((CDiaTCDesign*)m_pParent)->m_ControlTracker.DeSelectAll();
		((CDiaTCDesign*)m_pParent)->m_ControlTracker.RemoveFromAll(m_pCtrl);
		((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);


		CString title;
		((CDiaTCDesign*)m_pParent)->GetWindowTextW(title);
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(title,(CDiaTCDesign*)m_pParent);

		return TRUE;
	}
};



//////////////////////移动或者拖动控件大小////////////////////
//***************拖动控件大小的撤销和恢复*******************//
//*************拖动控件大小只是对单一控件的操作**************//
//////////////////////移动或者拖动控件大小////////////////////
class OnSizeCtrlCommand : public UNDO::Command
{
private:
	// Data members
	CWnd* m_pCtrl;
	CWnd* m_pParent;
	CRect m_rc;						//控件的大小
	CRect m_newrc;					//控件要移动到的位置
	TCTOOLTYPE m_CtrlType;			//控件类型
	TCPARENTTYPE m_ParentWndtype;	//父窗口的类型


public:
	// Constructor & destructor

	//参数1：父窗口指针
	//参数2：控件指针
	//参数3：控件要移动到的位置
	//参数4：控件类型
	//参数5：父窗口类型
	OnSizeCtrlCommand(CWnd* pParent,CWnd* pCtrl,CRect rc,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType)
	{
		m_pParent = pParent;
		m_pCtrl = pCtrl;
		m_newrc = rc;
		m_CtrlType = ctrlType;
		m_ParentWndtype = parentWndType;
		m_pCtrl->GetWindowRect(m_rc);
		m_pParent->ScreenToClient(&m_rc);
	}

	virtual ~OnSizeCtrlCommand()
	{
	}

	// Methods
	virtual bool Execute()
	{
		m_pCtrl->MoveWindow(m_newrc);
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			//返回控件在m_ControlTrackerAll中的位置
			int index = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.IsIncludeControl(m_pCtrl);

			if( -1!=index )
			{
				CString ctrlID;
				ctrlID = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_CtrlType,ctrlID,m_pCtrl,TRUE);
				int size = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll.GetSize();
				for(int i=0;i<size;i++)
				{
					CControlRect* pCurrentControl = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[i];
					if( pCurrentControl->GetCWnd() == m_pCtrl )
					{
						pCurrentControl->m_rect = m_newrc;
						((CDiaTCDesign*)m_pParent)->DeSelectAll();
						((CDiaTCDesign*)m_pParent)->m_ControlTracker.Select(pCurrentControl);
					}
				}
				//刷新窗口,避免橡皮筋显示不及时
				((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
			}
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			//返回控件在m_ControlTrackerAll中的位置
			int index = ((CTCTabPage*)m_pParent)->m_ControlTracker.IsIncludeControl(m_pCtrl);

			if( -1!=index )
			{
				CString ctrlID;
				ctrlID = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_CtrlType,ctrlID,m_pCtrl,TRUE);
				int size = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll.GetSize();
				for(int i=0;i<size;i++)
				{
					CControlRect* pCurrentControl = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[i];
					if( pCurrentControl->GetCWnd() == m_pCtrl )
					{
						pCurrentControl->m_rect = m_newrc;
						((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->DeSelectAll();
						((CTCTabPage*)m_pParent)->m_ControlTracker.Select(pCurrentControl);
					}
				}
				//刷新窗口,避免橡皮筋显示不及时
				((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->Invalidate(TRUE);
			}
		}

		return true;
	}

	virtual bool Unexecute()
	{
		if(m_pCtrl != NULL)
			m_pCtrl->MoveWindow(m_rc);
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{  
			int ctrlIndex = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.IsIncludeControl(m_pCtrl);
			if( -1 != ctrlIndex)
			{
				CString ctrlID;
				ctrlID = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[ctrlIndex]->GetCtlID();
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_CtrlType,ctrlID,m_pCtrl,TRUE);
				int size = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll.GetSize();
				for(int i=0;i<size;i++)
				{
					CControlRect* pCurrentControl = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[i];
					if( pCurrentControl->GetCWnd() == m_pCtrl )
					{
						pCurrentControl->m_rect = m_rc;
						((CDiaTCDesign*)m_pParent)->DeSelectAll();
						((CDiaTCDesign*)m_pParent)->m_ControlTracker.Select(pCurrentControl);
					}
				}
				//刷新窗口,避免橡皮筋显示不及时
				((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
			}
		}
		else if(m_ParentWndtype == TCPARENT_TABPAGE)
		{
			int index = ((CTCTabPage*)m_pParent)->m_ControlTracker.IsIncludeControl(m_pCtrl);

			if( -1!=index )
			{
				CString ctrlID;
				ctrlID = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_CtrlType,ctrlID,m_pCtrl,TRUE);
				int size = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll.GetSize();
				for(int i=0;i<size;i++)
				{
					CControlRect* pCurrentControl = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[i];
					if( pCurrentControl->GetCWnd() == m_pCtrl )
					{
						pCurrentControl->m_rect = m_rc;
						((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->DeSelectAll();
						((CTCTabPage*)m_pParent)->m_ControlTracker.Select(pCurrentControl);
					}
				}
				//刷新窗口,避免橡皮筋显示不及时
				((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->Invalidate(TRUE);
			}
		}

		return true;
	}
};


/////////////////////////控件排序动作//////////////////////////
//**********对于选取多个控件以后排序以及拖动位置
//*能够在Redo 和 Undo 的时候循环移动
//*而不是一个一个去移动
////////////////////////控件排序动作///////////////////////////
class SortCtrlCommand : public UNDO::Command
{
private:
	//Data members
	TCPARENTTYPE m_ParentWndtype;	//父窗口的类型
	CWnd* m_pParent;				//父窗口的指针
	int m_TCCtrlNum;				//控件的数量

	//保存要排序的控件信息的结构体
	struct TCCTRLSORT
	{
		CWnd* m_pCtrl;
		CRect m_rc;						//控件的大小
		CRect m_newrc;					//控件要移动到的位置

		TCTOOLTYPE m_CtrlType;			//控件类型

		TCCTRLSORT()
		{
			m_pCtrl = NULL;
			m_rc.SetRect(0,0,0,0);
			m_newrc.SetRect(0,0,0,0);
			m_CtrlType = TCTT_MAX;
		}
		void SetTCCtrl(CWnd* pCtrl,CRect newrc,TCTOOLTYPE ctrlType)
		{
			m_pCtrl = pCtrl;
			m_pCtrl->GetWindowRect(m_rc);
			m_pCtrl->GetParent()->ScreenToClient(&m_rc);
			m_newrc = newrc;
			m_CtrlType = ctrlType;
		}
	};
public:
	TCCTRLSORT m_TCCtrlSort[MAXSELECTCTRLS];
	void SetTCCtrlNum(int num){
		m_TCCtrlNum = num; }
public:
	SortCtrlCommand(TCPARENTTYPE parentWndtype,CWnd* pParent)
	{		
		m_ParentWndtype = parentWndtype;
		m_pParent = pParent;
	}

	virtual ~SortCtrlCommand()
	{

	}
	// Methods
	virtual bool Execute()
	{
		//循环移动
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			((CDiaTCDesign*)m_pParent)->DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_newrc);
				int index = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1 != index )
				{
					CString ctrlID = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_newrc;
					((CDiaTCDesign*)m_pParent)->m_ControlTracker.Select(((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			if(1 == m_TCCtrlNum)
				((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[ 0 ]->TCUpdateProperty();
			else
				((CDiaTCDesign*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			((CTCTabPage*)m_pParent)->DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_newrc);
				int index = ((CTCTabPage*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1!=index )
				{
					CString ctrlID = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_newrc;	
					((CTCTabPage*)m_pParent)->m_ControlTracker.Select(((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			if(1 == m_TCCtrlNum)
				((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[ 0 ]->TCUpdateProperty();
			else
				((CTCTabPage*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->Invalidate(TRUE);
		}
		return TRUE;
	}
	virtual bool Unexecute()
	{
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			//再移回来 
			((CDiaTCDesign*)m_pParent)->DeSelectAll();	
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_rc);
				int index = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1!=index )
				{
					CString ctrlID = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_rc;
					((CDiaTCDesign*)m_pParent)->m_ControlTracker.Select(((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			if(1 == m_TCCtrlNum)
				((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[ 0 ]->TCUpdateProperty();
			else
				((CDiaTCDesign*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			((CTCTabPage*)m_pParent)->DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_rc);
				int index = ((CTCTabPage*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1!=index )
				{
					CString ctrlID = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_rc;
					((CTCTabPage*)m_pParent)->m_ControlTracker.Select(((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			if(1 == m_TCCtrlNum)
				((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[ 0 ]->TCUpdateProperty();
			else
				((CTCTabPage*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->Invalidate(TRUE);
		}
		return TRUE;
	}
};



/////////////////////////控件排序动作//////////////////////////
//**********对于选取多个控件以后均匀适中排序
//*能够在Redo 和 Undo 的时候循环移动
//*而不是一个一个去移动
////////////////////////控件排序动作///////////////////////////
class UniformCtrlCommand : public UNDO::Command
{
private:
	//Data members
	TCPARENTTYPE m_ParentWndtype;	//父窗口的类型
	CWnd* m_pParent;				//父窗口的指针
	int m_TCCtrlNum;				//控件的数量

	//保存要排序的控件信息的结构体
	struct TCCTRLSORT
	{	
		CWnd* m_pCtrl;
		CRect m_rc;						//控件的大小
		CRect m_newrc;					//控件要移动到的位置
		TCTOOLTYPE m_CtrlType;			//控件类型

		TCCTRLSORT()
		{
			m_pCtrl = NULL;
			m_rc.SetRect(0,0,0,0);
			m_newrc.SetRect(0,0,0,0);
			m_CtrlType = TCTT_MAX;
		}
		void SetTCCtrl(CWnd* pCtrl,CRect oldrc,CRect newrc,TCTOOLTYPE ctrlType)
		{
			m_pCtrl = pCtrl;		
			m_rc = oldrc;
			m_pCtrl->GetParent()->ScreenToClient(&m_rc);
			m_newrc = newrc;
			m_CtrlType = ctrlType;
		}
	};
public:
	TCCTRLSORT m_TCCtrlSort[MAXSELECTCTRLS];
	void SetTCCtrlNum(int num){
		m_TCCtrlNum = num; }
public:
	UniformCtrlCommand(TCPARENTTYPE parentWndtype,CWnd* pParent)
	{		
		m_ParentWndtype = parentWndtype;
		m_pParent = pParent;
	}

	virtual ~UniformCtrlCommand()
	{

	}
	// Methods
	virtual bool Execute()
	{
		//循环移动
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			//((CDiaTCDesign*)m_pParent)->DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_newrc);
				int index = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1!=index )
				{
					CString ctrlID = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_newrc;
					//((CDiaTCDesign*)m_pParent)->m_ControlTracker.Select(((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			//((CDiaTCDesign*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			//((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_newrc);
				int index = ((CTCTabPage*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1!=index )
				{
					CString ctrlID = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_newrc;
					//((CTCTabPage*)m_pParent)->m_ControlTracker.Select(((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			//((CTCTabPage*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->Invalidate(TRUE);
		}
		return TRUE;
	}
	virtual bool Unexecute()
	{
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			//再移回来 
			//((CDiaTCDesign*)m_pParent)->DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_rc);
				int index = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1!=index )
				{
					CString ctrlID = ((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_rc;
					//((CDiaTCDesign*)m_pParent)->m_ControlTracker.Select(((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			//((CDiaTCDesign*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			//((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				m_TCCtrlSort[i].m_pCtrl->MoveWindow(m_TCCtrlSort[i].m_rc);
				int index = ((CTCTabPage*)m_pParent)->m_ControlTracker.IsIncludeControl(m_TCCtrlSort[i].m_pCtrl);
				if( -1!=index )
				{
					CString ctrlID = ((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]->m_rect = m_TCCtrlSort[i].m_rc;
					//((CTCTabPage*)m_pParent)->m_ControlTracker.Select(((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesAll[index]);
				}
			}
			//((CTCTabPage*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->Invalidate(TRUE);
		}
		return TRUE;
	}
};


/////////////////删除控件动作/////////////////////
//*删除单个控件
/////////////////删除控件动作/////////////////////
class ClearCtrlCommand : public UNDO::Command
{

	struct TCCTRLPROPERTY
	{
		CWnd* m_pCtrl;								//控件的指针
		TCTOOLTYPE m_CtrlType;						//控件类型
		CString m_title;							//控件的标题
		CString m_id;								//控件的id
		CRect m_rc;									//控件的大小
		TCPARENTTYPE m_ParentWndtype;				//父窗口的类型
		CWnd* m_pParent;

		WORD m_wVirtualKeyCode;						//热键的键值
		WORD m_wModifiers;							//保存热键的键值

		CString m_info;								//控件里面的内容(列表框内容)
		int m_mode;									//浏览框浏览模式

		bool m_ckecked;								//控件是否选中(Radio/Check)
		bool m_able;								//控件是否有效(able/disable)
		bool m_textleft;							//标题居左
		bool m_mulityline;							//标题多行
		TCCTRLPROPERTY()
		{
			m_pCtrl = NULL;
			m_CtrlType = TCTT_MAX;
			m_title = _T("");
			m_id = _T("");
			m_info = _T("");
			m_rc.SetRect(0,0,0,0);
			m_ParentWndtype = TCPARENT_MAX;
			m_ckecked = false;
			m_able = true;
			m_textleft = false;
			m_mulityline = false;
			m_mode = 0;
			m_wVirtualKeyCode = 0;
			m_wModifiers = 0;
		}
	};
private:
	// Data membersss
	//保存控件信息的结构体
	TCCTRLPROPERTY m_tcCtrlPerproty;

public:
	// Constructor & destructor
	ClearCtrlCommand(CWnd* pParent,CWnd* pCtrl,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType)
	{

		m_tcCtrlPerproty.m_pCtrl = pCtrl;
		m_tcCtrlPerproty.m_CtrlType = ctrlType;
		m_tcCtrlPerproty.m_ParentWndtype = parentWndType;
		m_tcCtrlPerproty.m_pParent = pParent;
		m_tcCtrlPerproty.m_pCtrl->GetWindowRect(m_tcCtrlPerproty.m_rc);
		m_tcCtrlPerproty.m_pParent->ScreenToClient(&m_tcCtrlPerproty.m_rc);
		if( m_tcCtrlPerproty.m_ParentWndtype == TCPARENT_DESDIN )
		{
			//控件在m_ControlTracker中的序号
			int index = ((CDiaTCDesign*)pParent)->m_ControlTracker.IsIncludeControl(pCtrl);
			if( -1 != index )
			{
				//保存控件的ID 
				m_tcCtrlPerproty.m_id = ((CDiaTCDesign*)pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
				switch(ctrlType)
				{
				case TCTT_BUTTON:
					((CTCButton*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					m_tcCtrlPerproty.m_able=((CTCButton*)pCtrl)->TCGetAbled();
					m_tcCtrlPerproty.m_mulityline=((CTCButton*)pCtrl)->TCGetMulityLine();
					break;
				case TCTT_STATIC:
					((CTCStatic*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					break;
				case TCTT_EDIT:
					((CTCEdit*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					m_tcCtrlPerproty.m_ckecked = ((CTCEdit*)pCtrl)->TCGetPassWord();
					break;
				case TCTT_RADIO:
					((CTCRadio*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					m_tcCtrlPerproty.m_ckecked = ((CTCRadio*)pCtrl)->TCGetCheck();
					m_tcCtrlPerproty.m_able=((CTCRadio*)pCtrl)->TCGetAbled();
					m_tcCtrlPerproty.m_textleft=((CTCRadio*)pCtrl)->TCGetTextLeft();
					break;
				case TCTT_CHECK:
					((CTCCheck*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					m_tcCtrlPerproty.m_ckecked = ((CTCCheck*)pCtrl)->TCGetCheck();
					m_tcCtrlPerproty.m_able=((CTCCheck*)pCtrl)->TCGetAbled();
					m_tcCtrlPerproty.m_textleft=((CTCCheck*)pCtrl)->TCGetTextLeft();
					break;
				case TCTT_COMBOBOX:
					/*for(int i=0;i<((CTCComboBox*)pCtrl)->GetCount();i++)
					{
					CString node;
					((CTCComboBox*)pCtrl)->GetLBText(i,node);
					node=node+_T("\n");
					m_tcCtrlPerproty.m_title+=node;
					}*/
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)pCtrl)->GetText(m_tcCtrlPerproty.m_title);
					break;
				case TCTT_LISTBOX:
					m_tcCtrlPerproty.m_title = ((CTCListBox*)pCtrl)->TCGetListBoxCaption();
					/*for(int i=0;i<((CTCListBox*)pCtrl)->GetCount();i++)
					{
					CString node;
					node = ((CTCListBox*)pCtrl)->GetItemText(i);
					node=node+_T("\n");
					m_tcCtrlPerproty.m_info+=node;
					}*/
					break;
				case TCTT_PICTURE:
					m_tcCtrlPerproty.m_title = ((CTCPicture*)pCtrl)->GetName();
					break;
				case TCTT_PATH:
					m_tcCtrlPerproty.m_mode = ((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCGetMode();
					break;
				case TCTT_PROGRESS:
					m_tcCtrlPerproty.m_ckecked = ((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->TCGetVertical();
					break;
				case TCTT_HOTKEY:
					((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->GetHotKey(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
					break;
				case TCTT_EXPLORER:
					m_tcCtrlPerproty.m_info = ((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->TCGetURL();
					break;
				case TCTT_GRIDCTRL:
					break;
				default:
					m_tcCtrlPerproty.m_ckecked = false;
					break;
				}
			}
		}
		else if( m_tcCtrlPerproty.m_ParentWndtype == TCPARENT_TABPAGE )
		{
			//控件在m_ControlTracker中的序号
			int index = ((CTCTabPage*)pParent)->m_ControlTracker.IsIncludeControl(pCtrl);
			if( -1 != index )
			{
				//保存控件的ID 
				m_tcCtrlPerproty.m_id = ((CTCTabPage*)pParent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
				switch(ctrlType)
				{
				case TCTT_BUTTON:
					((CTCButton*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					break;
				case TCTT_STATIC:
					((CTCStatic*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					break;
				case TCTT_EDIT:
					((CTCEdit*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				case TCTT_RADIO:
					((CTCRadio*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					m_tcCtrlPerproty.m_ckecked = ((CTCRadio*)pCtrl)->TCGetCheck();
					break;
				case TCTT_CHECK:
					((CTCCheck*)pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
					m_tcCtrlPerproty.m_ckecked = ((CTCCheck*)pCtrl)->TCGetCheck();
					break;
				case TCTT_COMBOBOX:
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)pCtrl)->GetText(m_tcCtrlPerproty.m_title);
					break;
				case TCTT_LISTBOX:
					m_tcCtrlPerproty.m_title = ((CTCListBox*)pCtrl)->TCGetListBoxCaption();					
					break;
				case TCTT_PICTURE:
					m_tcCtrlPerproty.m_title = ((CTCPicture*)pCtrl)->GetName();
					break;
				case TCTT_PATH:
					m_tcCtrlPerproty.m_mode = ((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCGetMode();
					break;
				case TCTT_PROGRESS:
					m_tcCtrlPerproty.m_ckecked = ((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->TCGetVertical();
					break;
				case TCTT_HOTKEY:
					((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->GetHotKey(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
					break;
				case TCTT_EXPLORER:
					m_tcCtrlPerproty.m_info = ((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->TCGetURL();
					break;
				case TCTT_GRIDCTRL:
					break;
				default:
					m_tcCtrlPerproty.m_ckecked = false;
					break;
				}
			}
		}
	}

	virtual ~ClearCtrlCommand()
	{
	}

	// Methods
	//执行删除接口
	virtual bool Execute()
	{
		if( m_tcCtrlPerproty.m_ParentWndtype == TCPARENT_DESDIN )
		{
			switch(m_tcCtrlPerproty.m_CtrlType)
			{
			case TCTT_BUTTON:
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				m_tcCtrlPerproty.m_able=((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_mulityline=((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCGetMulityLine();
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				//是否密文
				m_tcCtrlPerproty.m_ckecked = ((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->TCGetPassWord();
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				m_tcCtrlPerproty.m_ckecked = ((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCGetCheck();
				m_tcCtrlPerproty.m_able=((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_textleft=((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCGetTextLeft();
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				m_tcCtrlPerproty.m_ckecked = ((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCGetCheck();
				m_tcCtrlPerproty.m_able=((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_textleft=((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCGetTextLeft();
				break;
			case TCTT_COMBOBOX:
				for(int i=0;i<((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->GetCount();i++)
				{
					CString node;
					((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->GetLBText(i,node);
					node=node+_T("\n");
					m_tcCtrlPerproty.m_title+=node;
				}
				m_tcCtrlPerproty.m_able=((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_tcCtrlPerproty.m_pCtrl)->GetText(m_tcCtrlPerproty.m_title);
				break;
			case TCTT_LISTBOX:
				m_tcCtrlPerproty.m_title = ((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->TCGetListBoxCaption();
				m_tcCtrlPerproty.m_able=((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				for(int i=0;i<((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->GetCount();i++)
				{
					CString node;
					node = ((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->GetItemText(i);
					node=node+_T("\n");
					m_tcCtrlPerproty.m_info+=node;
				}
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->m_pWndList->DestroyWindow();
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->m_pWndList = 0;
				break;
			case TCTT_PICTURE:
				m_tcCtrlPerproty.m_title = ((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->GetName();
				break;
			case TCTT_PATH:
				m_tcCtrlPerproty.m_mode = ((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCGetMode();
				m_tcCtrlPerproty.m_able=((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				break;
			case TCTT_PROGRESS:
				m_tcCtrlPerproty.m_ckecked = ((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->TCGetVertical();
				break;
			case TCTT_SLIDER:
				m_tcCtrlPerproty.m_able=((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_ckecked = ((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCGetVertical();
				break;
			case TCTT_HOTKEY:
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->GetHotKey(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
				m_tcCtrlPerproty.m_able=((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				break;
			case TCTT_EXPLORER:
				m_tcCtrlPerproty.m_info = ((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->TCGetURL();
				break;
			case TCTT_GRIDCTRL:
				break;
			default:
				m_tcCtrlPerproty.m_mode = 0;
				m_tcCtrlPerproty.m_ckecked = false;
				m_tcCtrlPerproty.m_able = true;
				break;
			}
			//销毁窗口
			m_tcCtrlPerproty.m_pCtrl->DestroyWindow();

			//将当前控件从选中链表中移除
			for(int i=0;i<((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected.GetSize();i++)
			{
				if(((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected[i]->GetCWnd() == m_tcCtrlPerproty.m_pCtrl)
					((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.DeSelect(((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected[i]);
			}

			((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.RemoveFromAll(m_tcCtrlPerproty.m_pCtrl);
			
			//显示面板属性
			CString name;
			((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->GetWindowTextW(name);
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(name,(CDiaTCDesign*)m_tcCtrlPerproty.m_pParent);
			((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->Invalidate(TRUE);
		}
		else if(m_tcCtrlPerproty.m_ParentWndtype == TCPARENT_TABPAGE)
		{

			switch(m_tcCtrlPerproty.m_CtrlType)
			{
			case TCTT_BUTTON:
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				m_tcCtrlPerproty.m_able=((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_mulityline=((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCGetMulityLine();
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				//是否密文
				m_tcCtrlPerproty.m_ckecked = ((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->TCGetPassWord();
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				m_tcCtrlPerproty.m_ckecked = ((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCGetCheck();
				m_tcCtrlPerproty.m_able=((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_textleft=((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCGetTextLeft();
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->GetWindowTextW(m_tcCtrlPerproty.m_title);
				m_tcCtrlPerproty.m_ckecked = ((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCGetCheck();
				m_tcCtrlPerproty.m_able=((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_textleft=((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCGetTextLeft();
				break;
			case TCTT_COMBOBOX:
				for(int i=0;i<((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->GetCount();i++)
				{
					CString node;
					((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->GetLBText(i,node);
					node=node+_T("\n");
					m_tcCtrlPerproty.m_title+=node;
				}
				m_tcCtrlPerproty.m_able=((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_tcCtrlPerproty.m_pCtrl)->GetText(m_tcCtrlPerproty.m_title);
				break;
			case TCTT_LISTBOX:
				m_tcCtrlPerproty.m_title = ((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->TCGetListBoxCaption();
				m_tcCtrlPerproty.m_able=((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				for(int i=0;i<((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->GetCount();i++)
				{
					CString node;
					node = ((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->GetItemText(i);
					node=node+_T("\n");
					m_tcCtrlPerproty.m_info+=node;
				}
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->m_pWndList->DestroyWindow();
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->m_pWndList = 0;
				break;
			case TCTT_PICTURE:
				m_tcCtrlPerproty.m_title = ((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->GetName();
				break;
			case TCTT_PATH:
				m_tcCtrlPerproty.m_mode = ((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCGetMode();
				m_tcCtrlPerproty.m_able=((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				break;
			case TCTT_PROGRESS:
				m_tcCtrlPerproty.m_ckecked = ((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->TCGetVertical();
				break;
			case TCTT_SLIDER:
				m_tcCtrlPerproty.m_able=((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				m_tcCtrlPerproty.m_ckecked = ((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCGetVertical();
				break;
			case TCTT_HOTKEY:
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->GetHotKey(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
				m_tcCtrlPerproty.m_able=((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->TCGetAbled();
				break;
			case TCTT_EXPLORER:
				m_tcCtrlPerproty.m_info = ((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->TCGetURL();
				break;
			case TCTT_GRIDCTRL:
				break;
			default:
				m_tcCtrlPerproty.m_mode = 0;
				m_tcCtrlPerproty.m_ckecked = false;
				m_tcCtrlPerproty.m_able = true;
				break;
			}
			//销毁窗口
			m_tcCtrlPerproty.m_pCtrl->DestroyWindow();

			//从选中链表中移除
			for(int i=0;i<((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected.GetSize();i++)
			{
				if(((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected[i]->GetCWnd() == m_tcCtrlPerproty.m_pCtrl)
					((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.DeSelect(((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected[i]);
			}

			((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.RemoveFromAll(m_tcCtrlPerproty.m_pCtrl);

			((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->Invalidate(TRUE);
		}

		return true;
	}

	virtual bool Unexecute()
	{

		if( m_tcCtrlPerproty.m_ParentWndtype == TCPARENT_DESDIN )
		{
			int i=0;
			((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->DeSelectAll();
			switch(m_tcCtrlPerproty.m_CtrlType)
			{
			case TCTT_BUTTON:
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title,WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCSetButtonMulity(m_tcCtrlPerproty.m_mulityline);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCButton((CTCButton*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCStatic((CTCStatic*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->SetWindowTextW(m_tcCtrlPerproty.m_title);
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->TCSetPassWord(m_tcCtrlPerproty.m_ckecked);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCEdit((CTCEdit*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->SetCheck(m_tcCtrlPerproty.m_ckecked);
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCSetTextLeft(m_tcCtrlPerproty.m_textleft);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCRadio((CTCRadio*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->SetCheck(m_tcCtrlPerproty.m_ckecked);
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCSetTextLeft(m_tcCtrlPerproty.m_textleft);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCCheck((CTCCheck*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_COMBOBOX:
				((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				if(!m_tcCtrlPerproty.m_title.IsEmpty())
				{
					CString csHelpText;
					int index=m_tcCtrlPerproty.m_title.Find(L'\n',0);

					while(index!=-1)
					{
						csHelpText=m_tcCtrlPerproty.m_title.Left(index);
						m_tcCtrlPerproty.m_title.Delete(0,index+1);
						((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->AddString(csHelpText);
						index=m_tcCtrlPerproty.m_title.Find(L'\n',0);
					}
				}
				((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCCombox((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_tcCtrlPerproty.m_pCtrl)->Create(NULL,m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535,NULL);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCGroupBox((CTCGroupBox*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_LISTBOX:
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title,WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				if(!m_tcCtrlPerproty.m_info.IsEmpty())
				{
					CString csHelpText;
					int index=m_tcCtrlPerproty.m_info.Find(L'\n',0);

					while(index!=-1)
					{
						csHelpText=m_tcCtrlPerproty.m_info.Left(index);
						m_tcCtrlPerproty.m_info.Delete(0,index+1);
						((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->AddItem(csHelpText);
						// pListbox->AddString(csHelpText,colorTexr);
						index=m_tcCtrlPerproty.m_info.Find(L'\n',0);
					}
				}	
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCListBox((CTCListBox*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_PROGRESS:
				((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->SetPos(70);
				((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->TCSetVertical(m_tcCtrlPerproty.m_ckecked);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCProgress((CTCProgress*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_DATE:
				((CTCDate*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCDate((CTCDate*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_PICTURE:
				((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->SetRCPicture(m_tcCtrlPerproty.m_title);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCPicture((CTCPicture*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_SLIDER:
				((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCSetVertical(m_tcCtrlPerproty.m_ckecked);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCSlider((CTCSlider*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_PATH:
				((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCSetMode((CMFCEditBrowseCtrl::BrowseMode)m_tcCtrlPerproty.m_mode);
				((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCPath((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_TIMER:
				((CTCTimer*)m_tcCtrlPerproty.m_pCtrl)->Create(_T("时钟"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|SS_REALSIZEIMAGE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCTimer*)m_tcCtrlPerproty.m_pCtrl)->SetBitmap(((CTCTimer*)m_tcCtrlPerproty.m_pCtrl)->m_bmp);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCTimer((CTCTimer*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_HOTKEY:
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->SetHotKey(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->SetHotKeyValue(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCHotKey((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_EXPLORER:
				((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->TCSetURL(m_tcCtrlPerproty.m_info);
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCExplorer((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc);
				break;
			case TCTT_GRIDCTRL:
				((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				if(!((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->TCGetExpandToFit())
				{
					for(int row = 0;row < ((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->GetRowCount(); row++)
					{
						((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->SetRowHeight(row,25);
					}
				}
				((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->SetTCGridCtrl((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc);
				break;
			default:
				break;
			}
		}
		else if( m_tcCtrlPerproty.m_ParentWndtype == TCPARENT_TABPAGE )
		{
			((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->DeSelectAll();
			switch(m_tcCtrlPerproty.m_CtrlType)
			{
			case TCTT_BUTTON:
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title,WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCButton*)m_tcCtrlPerproty.m_pCtrl)->TCSetButtonMulity(m_tcCtrlPerproty.m_mulityline);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCButton((CTCButton*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCStatic((CTCStatic*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->SetWindowTextW(m_tcCtrlPerproty.m_title);
				((CTCEdit*)m_tcCtrlPerproty.m_pCtrl)->TCSetPassWord(m_tcCtrlPerproty.m_ckecked);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCEdit((CTCEdit*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->SetCheck(m_tcCtrlPerproty.m_ckecked);
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCRadio*)m_tcCtrlPerproty.m_pCtrl)->TCSetTextLeft(m_tcCtrlPerproty.m_textleft);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCRadio((CTCRadio*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->SetCheck(m_tcCtrlPerproty.m_ckecked);
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCCheck*)m_tcCtrlPerproty.m_pCtrl)->TCSetTextLeft(m_tcCtrlPerproty.m_textleft);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCCheck((CTCCheck*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_COMBOBOX:
				((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				if(!m_tcCtrlPerproty.m_title.IsEmpty())
				{
					CString csHelpText;
					int index=m_tcCtrlPerproty.m_title.Find(L'\n',0);

					while(index!=-1)
					{
						csHelpText=m_tcCtrlPerproty.m_title.Left(index);
						m_tcCtrlPerproty.m_title.Delete(0,index+1);
						((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->AddString(csHelpText);
						index=m_tcCtrlPerproty.m_title.Find(L'\n',0);
					}
				}
				((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCCombox((CTCComboBox*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_tcCtrlPerproty.m_pCtrl)->Create(NULL,m_tcCtrlPerproty.m_title, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535,NULL);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCGroupBox((CTCGroupBox*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_LISTBOX:
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_title,WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				if(!m_tcCtrlPerproty.m_info.IsEmpty())
				{
					CString csHelpText;
					int index=m_tcCtrlPerproty.m_info.Find(L'\n',0);

					while(index!=-1)
					{
						csHelpText=m_tcCtrlPerproty.m_info.Left(index);
						m_tcCtrlPerproty.m_info.Delete(0,index+1);
						((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->AddItem(csHelpText);
						// pListbox->AddString(csHelpText,colorTexr);
						index=m_tcCtrlPerproty.m_info.Find(L'\n',0);
					}
				}	
				((CTCListBox*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCListBox((CTCListBox*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_PROGRESS:
				((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->SetPos(70);
				((CTCProgress*)m_tcCtrlPerproty.m_pCtrl)->TCSetVertical(m_tcCtrlPerproty.m_ckecked);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCProgress((CTCProgress*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_DATE:
				((CTCDate*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCDate((CTCDate*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_PICTURE:
				((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->SetRCPicture(m_tcCtrlPerproty.m_title);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCPicture((CTCPicture*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_SLIDER:
				((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCSlider*)m_tcCtrlPerproty.m_pCtrl)->TCSetVertical(m_tcCtrlPerproty.m_ckecked);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCSlider((CTCSlider*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_PATH:
				((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCSetMode((CMFCEditBrowseCtrl::BrowseMode)m_tcCtrlPerproty.m_mode);
				((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCPath((CTCBrowse*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_HOTKEY:
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->SetHotKey(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->SetHotKeyValue(m_tcCtrlPerproty.m_wVirtualKeyCode,m_tcCtrlPerproty.m_wModifiers);
				((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl)->TCSetAbled(m_tcCtrlPerproty.m_able);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCHotKey((CTCHotKeyCtrl*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc,true,m_tcCtrlPerproty.m_id);
				break;
			case TCTT_EXPLORER:
				((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl)->TCSetURL(m_tcCtrlPerproty.m_info);
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCExplorer((CTCExplorer*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc);
				break;
			case TCTT_GRIDCTRL:
				((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->Create(m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
				if(!((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->TCGetExpandToFit())
				{
					for(int row = 0;row < ((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->GetRowCount(); row++)
					{
						((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl)->SetRowHeight(row,25);
					}
				}
				((CTCTabPage*)m_tcCtrlPerproty.m_pParent)->SetTCGridCtrl((CGridCtrl*)m_tcCtrlPerproty.m_pCtrl,m_tcCtrlPerproty.m_rc);
				break;
			default:
				break;
			}
		}
		return true;
	}
};



/////////////////////删除Tab控件的动作////////////////////////
//*要对其子页面对应的控件也进行删除操作 
//*并且能在Redo完Tab控件以后删除掉的子
//*控件也能恢复
/////////////////////删除Tab控件的动作////////////////////////
class ClearTabCommand : public UNDO::Command
{
private:
	//Data members
	CWnd* m_pParent;
	CTCTab* m_pCtrl;
	CRect m_rc;
	CString m_id;	//Tab的ID

	//用来保存TabPage中的控件信息的结构体
	struct	TCCTRLINTABPAGE 
	{
		CWnd* m_pCtrlInTab;
		TCTOOLTYPE m_CtrlType;    //记录Tab中的控件类型
		CRect m_ctrlRcInTab;	  //控件的位置
		CString m_title;		  //控件的标题
		CString m_Ctrlid;		  //控件的ID

		bool m_isTrue;			  //标识控件是否选中，是否密文等变量	

		CString m_info;			 //当控件既有标题又有内容时用来保存内容

		int m_mode;					//浏览框浏览模式

		WORD m_wVirtualKeyCode;		//热键的键值
		WORD m_wModifiers;			//热键的键值

		TCCTRLINTABPAGE()
		{
			m_pCtrlInTab = NULL;
			m_CtrlType = TCTT_MAX;
			m_ctrlRcInTab.SetRect(0,0,0,0);

			m_title = _T("");
			m_Ctrlid = _T("");

			m_info = _T("");


			m_isTrue = false;

			m_mode = 0;

			m_wVirtualKeyCode = 0;
			m_wModifiers = 0;
		}
		void SetTCCtrlInTab(CWnd *pCtrl,TCTOOLTYPE ctrlType,CRect rc)
		{
			m_pCtrlInTab = pCtrl;
			m_CtrlType = ctrlType;
			m_ctrlRcInTab = rc;
		}
	};

	struct TCTABPAGE
	{
		CWnd* m_pTabPage;
		int m_num;			//Tab中控件的数量
		TCCTRLINTABPAGE m_TCCtrlsInTab[MAXSELECTCTRLS]; //保存Tab中的控件信息
	};

public:
	CStringArray m_pageTitle;  //记录Tab的标签名称
	TCTABPAGE m_tcTabPage[32];

public:
	ClearTabCommand(CWnd *parent,CTCTab* pCtrl,CRect rc)
	{
		m_pParent = parent;
		m_pCtrl = pCtrl;
		m_rc = rc;
		int index = ((CDiaTCDesign*)parent)->m_ControlTracker.IsIncludeControl(pCtrl);
		//Tab的ID
		m_id = ((CDiaTCDesign*)parent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
	}
	virtual ~ClearTabCommand()
	{

	}
	virtual bool Execute()
	{
		////删除对应的子页面以及页面对应的子控件
		for(int i=0;i<((CTCTab*)m_pCtrl)->getTabPageCount();i++)
		{
			m_tcTabPage[i].m_num = ((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();
			m_tcTabPage[i].m_pTabPage = (CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i];
			int j=0;
			while(((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize()>0 )
			{
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab = ((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCWnd();
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_CtrlType = ((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlType();
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_ctrlRcInTab = ((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[0]->m_rect;

				int index = ((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.IsIncludeControl(m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);

				if( -1 != index )
				{
					//这儿得到控件的ID并保存
					m_tcTabPage[i].m_TCCtrlsInTab[j].m_Ctrlid = ((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					switch( m_tcTabPage[i].m_TCCtrlsInTab[j].m_CtrlType )
					{
					case TCTT_BUTTON:
						((CTCButton*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_STATIC:
						((CTCStatic*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_EDIT:
						((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						//是否密文
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetPassWord();
						break;
					case TCTT_RADIO:
						((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_CHECK:
						((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_COMBOBOX:
						for(int k=0;k<((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();k++)
						{
							CString node;
							((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetLBText(k,node);
							node=node+_T("\n");
							m_tcTabPage[i].m_TCCtrlsInTab[j].m_title+=node;
						}
						break;
					case TCTT_GROUP:
						((CTCGroupBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetText(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_LISTBOX:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetListBoxCaption();
						for(int n=0;n<((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();n++)
						{
							CString node;
							node = ((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetItemText(n);
							node=node+_T("\n");
							m_tcTabPage[i].m_TCCtrlsInTab[j].m_info+=node;
						}
						((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList->DestroyWindow();
						((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList = 0;
						break;
					case TCTT_PICTURE:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetName();
						break;
					case TCTT_PATH:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_mode = ((CTCBrowse*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetMode();
						break;
					case TCTT_HOTKEY:
						((CTCHotKeyCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetHotKey(m_tcTabPage[i].m_TCCtrlsInTab[j].m_wVirtualKeyCode,m_tcTabPage[i].m_TCCtrlsInTab[j].m_wModifiers);
						break;
					case TCTT_GRIDCTRL:

						break;
					default:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_mode = 0;
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = false;
						break;
					}
				}
				((CTCTabPage*)((CTCTab*)m_pCtrl)->m_Pages[i])->m_ControlTracker.RemoveFromAll(m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);	
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab->DestroyWindow();
				j++;		
			}
			((CTCTab*)m_pCtrl)->m_Pages[i]->DestroyWindow();
		}
		//最后删除Tab控件
		((CTCTab*)m_pCtrl)->DestroyWindow();
		((CDiaTCDesign*)m_pParent)->m_ControlTracker.RemoveFromAll(m_pCtrl);
		((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		return TRUE;
	}
	// Methods
	virtual bool Unexecute()
	{
		((CTCTab*)m_pCtrl)->Create(WS_VISIBLE|TCS_MULTILINE|TCS_TABS|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);		
		for(int i=0;i<m_pageTitle.GetSize();i++)
		{
			((CTCTab*)m_pCtrl)->addItem(m_pageTitle[i]);
			((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->Create(IDD_TCTABPAGE,((CTCTab*)m_pCtrl));
			((CTCTab*)m_pCtrl)->m_Pages[i] = (CTCTabPage*)m_tcTabPage[i].m_pTabPage;
			//for(int count=0;count<m_tcTabPage[i].m_num;count++)
			for(int count = m_tcTabPage[i].m_num - 1; count >= 0; count--)
			{
				switch(m_tcTabPage[i].m_TCCtrlsInTab[count].m_CtrlType)
				{
				case TCTT_BUTTON:
					((CTCButton*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title,WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCButton((CTCButton*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;			
				case TCTT_STATIC:
					((CTCStatic*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCStatic((CTCStatic*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_EDIT:
					((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title);
					((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->TCSetPassWord(m_tcTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCEdit((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_RADIO:
					((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetCheck(m_tcTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCRadio((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_CHECK:
					((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetCheck(m_tcTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCCheck((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_COMBOBOX:
					((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					if(!m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.IsEmpty())
					{
						CString csHelpText;
						int index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Find(L'\n',0);

						while(index!=-1)
						{
							csHelpText=m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Left(index);
							m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Delete(0,index+1);
							((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->AddString(csHelpText);
							index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Find(L'\n',0);
						}
					}
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCCombox((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(NULL,m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535,NULL);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCGroupBox((CTCGroupBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_LISTBOX:
					((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title,WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					if(!m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.IsEmpty())
					{
						CString csHelpText;
						int index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Find(L'\n',0);

						while(index!=-1)
						{
							csHelpText=m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Left(index);
							m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Delete(0,index+1);
							((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->AddItem(csHelpText);
							// pListbox->AddString(csHelpText,colorTexr);
							index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Find(L'\n',0);
						}
					}	
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCListBox((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PROGRESS:
					((CTCProgress*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCProgress((CTCProgress*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PICTURE:
					((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRCPicture(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCPicture((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_SLIDER:
					((CTCSlider*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCSlider((CTCSlider*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PATH:
					((CTCBrowse*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCPath((CTCBrowse*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_HOTKEY:
					((CTCHotKeyCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCHotKeyCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetHotKey(m_tcTabPage[i].m_TCCtrlsInTab[count].m_wVirtualKeyCode,m_tcTabPage[i].m_TCCtrlsInTab[count].m_wModifiers);
					((CTCHotKeyCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetHotKeyValue(m_tcTabPage[i].m_TCCtrlsInTab[count].m_wVirtualKeyCode,m_tcTabPage[i].m_TCCtrlsInTab[count].m_wModifiers);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCHotKey((CTCHotKeyCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_DATE:
					((CTCDate*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCDate((CTCDate*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_GRIDCTRL:
					((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					if(!((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->TCGetExpandToFit())
					{
						for(int row = 0;row < ((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->GetRowCount(); row++)
						{
							((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRowHeight(row,25);
						}
					}
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCGridCtrl((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,false,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				default:
					break;
				}
				//在这儿让Tab中的所有控件都置为未选中状态，因为创建的Tab是选中的状态
				((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->m_ControlTracker.DeSelectAll();
			}
		}	
		//显示Tab的属性并将Tab的ID设置回来
		((CDiaTCDesign*)m_pParent)->SetTCTab((CTCTab*)m_pCtrl,m_rc,m_id);
		((CTCTab*)m_pCtrl)->TCAdjustRect();
		((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		return TRUE;
	}

};



/////////////////////Tab控件的增加和删除页面动作////////////////////////
//*要对其子页面对应的控件也进行删除操作 
//*并且能在Redo完Tab控件以后删除掉的子
//*控件也能恢复
/////////////////////删除Tab和添加控件页面的动作////////////////////////
class TabPropertyCommand : public UNDO::Command
{
private:
	//Data members
	CWnd* m_pParent;		//指向父窗口
	CTCTab* m_pCtrl;		//指向控件
	CRect m_rc;				//控件的大小

	int m_newCount;			//用来标识新的页面的数量
	int m_oldCount;			//旧的Tab页面的数量

	struct	TCCTRLINTABPAGE		//用来保存TabPage中的控件信息的结构体
	{
		CWnd* m_pCtrlInTab;
		TCTOOLTYPE m_CtrlType;    //记录Tab中的控件类型
		CRect m_ctrlRcInTab;	  //控件的位置

		CString m_title;		  //控件的标题
		CString m_Ctrlid;		  //控件的ID

		bool m_isTrue;			  //标识控件是否选中，是否密文等变量	
		CString m_info;			 //当控件既有标题又有内容时用来保存内容

		int m_mode;					//浏览框浏览模式
		vector< int > m_colWidth;	//表格的列宽
		vector< int > m_rowHeight;	//表格的行高

		TCCTRLINTABPAGE()
		{
			m_pCtrlInTab = NULL;
			m_CtrlType = TCTT_MAX;
			m_ctrlRcInTab.SetRect(0,0,0,0);

			m_title = _T("");
			m_Ctrlid = _T("");
			m_info = _T("");
			m_isTrue = false;
			m_mode = 0;
		}
		void SetTCCtrlInTab(CWnd *pCtrl,TCTOOLTYPE ctrlType,CRect rc)
		{
			m_pCtrlInTab = pCtrl;
			m_CtrlType = ctrlType;
			m_ctrlRcInTab = rc;
		}
	};

	struct TCTABPAGE
	{
		CWnd* m_pTabPage;
		COLORREF m_color;	//TabPage颜色值
		CString m_pic;		//Tabpage图片
		int m_num;			//Tab中控件的数量
		TCCTRLINTABPAGE m_TCCtrlsInTab[MAXSELECTCTRLS]; //保存Tab中的控件信息
		TCTABPAGE()
		{
			m_pTabPage = NULL;
			m_color = ::GetSysColor(COLOR_BTNFACE);
			m_pic = _T("");
			m_num = 0;
		}
	};


public:
	CStringArray m_pageTitle;        //记录Tab的标签名称
	CStringArray m_pageNewTitle;     //记录新Tab的标签名称
	TCTABPAGE m_tcTabPage[32];       //记录Tab以前子页面的信息
	TCTABPAGE m_tcNewTabPage[32];    //记录新的要处理的Tab页面信息

public:
	TabPropertyCommand(CWnd *parent,CTCTab* pCtrl,CVSListBox *listBox)
	{
		m_pParent = parent;
		m_pCtrl = pCtrl;
		m_pCtrl->GetWindowRect(m_rc);
		m_pParent->ScreenToClient(&m_rc);

		m_oldCount=m_pCtrl->getTabPageCount();

		//拷贝旧的Tab信息
		for( int i=0;i<m_oldCount;i++ )
		{
			//////保存Tab的所有标题
			TCITEM item;
			wchar_t buffer[256] = {0};	
			item.pszText = buffer;
			item.cchTextMax = 100;
			item.mask = TCIF_TEXT;
			m_pCtrl->GetItem(i, &item);
			CString tabStr = buffer;
			m_pageTitle.Add(tabStr);

			m_tcTabPage[i].m_num = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll.GetSize();
			m_tcTabPage[i].m_pTabPage = m_pCtrl->m_Pages[i];

			m_tcTabPage[i].m_color = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->TCGetBkgrColor();
			m_tcTabPage[i].m_pic = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->TCGetBackgroundImage();
		}

		m_newCount = listBox->GetCount();	

		for(int i=0;i<m_newCount;i++)
		{
			CString text=listBox->GetItemText(i);
			m_pageNewTitle.Add(text);
			int data=listBox->GetItemData(i);

			m_tcNewTabPage[i].m_color = ((CTCTabListBox*)listBox)->m_pagePerty[i].m_color;
			m_tcNewTabPage[i].m_pic = ((CTCTabListBox*)listBox)->m_pagePerty[i].m_pic;
			//编辑的项在最后
			if(data==0)
			{
				CTCTabPage *page=new CTCTabPage();
				m_tcNewTabPage[i].m_pTabPage = page;
				m_tcNewTabPage[i].m_num = 0;
				//新的就默认窗口颜色
				m_tcNewTabPage[i].m_color = ((CTCTabListBox*)listBox)->m_pagePerty[i].m_color;
				m_tcNewTabPage[i].m_pic = ((CTCTabListBox*)listBox)->m_pagePerty[i].m_pic;
			}
			else
			{
				m_tcNewTabPage[i].m_pTabPage = m_pCtrl->m_Pages[data-1];
				m_tcNewTabPage[i].m_num = ((CTCTabPage*)m_pCtrl->m_Pages[data-1])->m_ControlTracker.m_arrRectanglesAll.GetSize();
				//*m_tcNewTabPage[i].m_num = m_tcTabPage[i].m_num;
				for( int j=0;j<m_tcNewTabPage[i].m_num;j++ )
				{
					m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab = ((CTCTabPage*)m_pCtrl->m_Pages[data-1])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd();
					m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_CtrlType = ((CTCTabPage*)m_pCtrl->m_Pages[data-1])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlType();
					m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_ctrlRcInTab = ((CTCTabPage*)m_pCtrl->m_Pages[data-1])->m_ControlTracker.m_arrRectanglesAll[j]->m_rect;


					int index = ((CTCTabPage*)(m_pCtrl->m_Pages[data-1]))->m_ControlTracker.IsIncludeControl(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);

					if( -1!=index )
					{
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_Ctrlid = ((CTCTabPage*)(m_pCtrl->m_Pages[data-1]))->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
						switch(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_CtrlType)
						{
						case TCTT_BUTTON:
							((CTCButton*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
							break;
						case TCTT_STATIC:
							((CTCStatic*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
							break;
						case TCTT_EDIT:
							((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
							//是否密文
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetPassWord();
							break;
						case TCTT_RADIO:
							((CTCRadio*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCRadio*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
							break;
						case TCTT_CHECK:
							((CTCCheck*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCCheck*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
							break;
						case TCTT_COMBOBOX:
							for(int k=0;k<((CTCComboBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();k++)
							{
								CString node;
								((CTCComboBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetLBText(k,node);
								node=node+_T("\n");
								m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title+=node;
							}
							break;
						case TCTT_GROUP:
							((CTCGroupBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetText(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
							break;
						case TCTT_LISTBOX:
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetListBoxCaption();
							for(int n=0;n<((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();n++)
							{
								CString node;
								node = ((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetItemText(n);
								node=node+_T("\n");
								m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_info+=node;
							}
							break;
						case TCTT_PICTURE:
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCPicture*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetName();
							break;
						case TCTT_PATH:
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_mode = ((CTCBrowse*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetMode();
							break;
						case TCTT_GRIDCTRL:
							{
								CGridCtrl* pCtrl = (CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab;
								for( int col = 0; col < pCtrl->GetColumnCount(); col++ )
								{
									m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_colWidth.push_back(pCtrl->GetColumnWidth(col));
								}
								for( int row = 0; row < pCtrl->GetRowCount(); row++ )
								{
									m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_rowHeight.push_back( pCtrl->GetRowHeight(row));
								}
							}						
							break;
						default:
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_mode = 0;
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = false;
							break;
						}
					}
				}

				//pageArray[i]=m_pCtrl->m_Pages[data-1];
				//tabArray[data-1]=false;
			}
			//m_pCtrl->InsertItem(m_pCtrl->GetItemCount(),text);
		}

		//记录并销毁旧的Tab中的控件
		for(int i=0;i<m_oldCount;i++)
		{
			int j=0;
			while(((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll.GetSize()>0)
			{
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->GetCWnd();
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_CtrlType = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlType();
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_ctrlRcInTab = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->m_rect;			
				
				int index = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.IsIncludeControl(m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);

				if(-1 != index)
				{
					//得到控件的ID
					m_tcTabPage[i].m_TCCtrlsInTab[j].m_Ctrlid = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					switch(m_tcTabPage[i].m_TCCtrlsInTab[j].m_CtrlType)
					{
					case TCTT_BUTTON:
						((CTCButton*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_STATIC:
						((CTCStatic*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_EDIT:
						((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						//是否密文
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetPassWord();
						break;
					case TCTT_RADIO:
						((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_CHECK:
						((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_COMBOBOX:
						for(int k=0;k<((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();k++)
						{
							CString node;
							((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetLBText(k,node);
							node=node+_T("\n");
							m_tcTabPage[i].m_TCCtrlsInTab[j].m_title+=node;
						}
						break;
					case TCTT_GROUP:
						((CTCGroupBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetText(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_LISTBOX:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetListBoxCaption();
						for(int n=0;n<((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();n++)
						{
							CString node;
							node = ((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetItemText(n);
							node=node+_T("\n");
							m_tcTabPage[i].m_TCCtrlsInTab[j].m_info+=node;
						}
						((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList->DestroyWindow();
						((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList = 0;
						break;
					case TCTT_PICTURE:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetName();
						break;
					case TCTT_PATH:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_mode = ((CTCBrowse*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetMode();
						break;
					case TCTT_GRIDCTRL:
						{
							CGridCtrl* pCtrl = (CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab;
							for( int col = 0; col < pCtrl->GetColumnCount(); col++ )
							{
								m_tcTabPage[i].m_TCCtrlsInTab[j].m_colWidth.push_back(pCtrl->GetColumnWidth(col));
							}
							for( int row = 0; row < pCtrl->GetRowCount(); row++ )
							{
								m_tcTabPage[i].m_TCCtrlsInTab[j].m_rowHeight.push_back( pCtrl->GetRowHeight(row));
							}
						}		
						break;
					default:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_mode = 0;
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = false;
						break;
					}
				}

				
				((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->m_ControlTracker.RemoveFromAll(m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);

				m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab->DestroyWindow();
				j++;
			}
		}

	}
	virtual ~TabPropertyCommand()
	{

	}
	virtual bool Execute()
	{
		//先删除新页面中的控件
		for(int i=0;i<m_oldCount;i++)
		{
			int j=0;
			while(((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->m_ControlTracker.m_arrRectanglesAll.GetSize()>0)
			{
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->GetCWnd();
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_CtrlType = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlType();
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_ctrlRcInTab = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->m_rect;

				int index = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.IsIncludeControl(m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);

				if(-1 != index)
				{
					//得到控件的ID
					m_tcTabPage[i].m_TCCtrlsInTab[j].m_Ctrlid = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					switch(m_tcTabPage[i].m_TCCtrlsInTab[j].m_CtrlType)
					{
					case TCTT_BUTTON:
						((CTCButton*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_STATIC:
						((CTCStatic*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_EDIT:
						((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						//是否密文
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetPassWord();
						break;
					case TCTT_RADIO:
						((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_CHECK:
						((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_COMBOBOX:
						for(int k=0;k<((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();k++)
						{
							CString node;
							((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetLBText(k,node);
							node=node+_T("\n");
							m_tcTabPage[i].m_TCCtrlsInTab[j].m_title+=node;
						}
						break;
					case TCTT_GROUP:
						((CTCGroupBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetText(m_tcTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_LISTBOX:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetListBoxCaption();
						for(int n=0;n<((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();n++)
						{
							CString node;
							node = ((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetItemText(n);
							node=node+_T("\n");
							m_tcTabPage[i].m_TCCtrlsInTab[j].m_info+=node;
						}
						((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList->DestroyWindow();
						((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList = 0;
						break;
					case TCTT_PICTURE:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetName();
						break;
					case TCTT_PATH:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_mode = ((CTCBrowse*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetMode();
						break;
					case TCTT_GRIDCTRL:
						{
							CGridCtrl* pCtrl = (CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab;
							for( int col = 0; col < pCtrl->GetColumnCount(); col++ )
							{
								m_tcTabPage[i].m_TCCtrlsInTab[j].m_colWidth.push_back(pCtrl->GetColumnWidth(col));
							}
							for( int row = 0; row < pCtrl->GetRowCount(); row++ )
							{
								m_tcTabPage[i].m_TCCtrlsInTab[j].m_rowHeight.push_back( pCtrl->GetRowHeight(row));
							}
						}
						break;
					default:
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_mode = 0;
						m_tcTabPage[i].m_TCCtrlsInTab[j].m_isTrue = false;
						break;
					}
				}

				((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->m_ControlTracker.RemoveFromAll(m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);
				m_tcTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab->DestroyWindow();
				j++;
			}
		}

		m_pCtrl->DeleteAllItems();

		for(int size=0;size<m_oldCount;size++)
		{
			m_pCtrl->m_Pages[size]->DestroyWindow();
		}

		//添加标签
		for(int i=0;i<m_newCount;i++)
		{
			m_pCtrl->InsertItem(m_pCtrl->GetItemCount(),m_pageNewTitle[i]);
			((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->Create(IDD_TCTABPAGE, m_pCtrl);
			m_pCtrl->m_Pages[i] = (CTCTabPage*)m_tcNewTabPage[i].m_pTabPage;
			((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->TCSetBkgrColor(m_tcNewTabPage[i].m_color);
			((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->TCSetBackgroundImage(m_tcNewTabPage[i].m_pic);
		}

		m_pCtrl->TCAdjustRect();
		int selIndex=m_pCtrl->getTabPageCount();
		m_pCtrl->SetCurSel(selIndex-1);
		m_pCtrl->showIndex(selIndex-1);


		for(int i=0;i<m_newCount;i++)
		{
			//for(int count=0;count<m_tcNewTabPage[i].m_num;count++)
			for (int count = m_tcNewTabPage[i].m_num - 1; count >= 0;count--)		
			{	
				switch(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_CtrlType)
				{
				case TCTT_BUTTON:
					((CTCButton*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title,WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCButton((CTCButton*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;			
				case TCTT_STATIC:
					((CTCStatic*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCStatic((CTCStatic*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_EDIT:
					((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title);
					((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->TCSetPassWord(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCEdit((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_RADIO:
					((CTCRadio*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCRadio*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetCheck(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCRadio((CTCRadio*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_CHECK:
					((CTCCheck*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCCheck*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetCheck(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCCheck((CTCCheck*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_COMBOBOX:
					((CTCComboBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					if(!m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title.IsEmpty())
					{
						CString csHelpText;
						int index=m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title.Find(L'\n',0);

						while(index!=-1)
						{
							csHelpText=m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title.Left(index);
							m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title.Delete(0,index+1);
							((CTCComboBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->AddString(csHelpText);
							index=m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title.Find(L'\n',0);
						}
					}
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCCombox((CTCComboBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(NULL,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535,NULL);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCGroupBox((CTCGroupBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_LISTBOX:
					((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title,WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					if(!m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_info.IsEmpty())
					{
						CString csHelpText;
						int index=m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_info.Find(L'\n',0);

						while(index!=-1)
						{
							csHelpText=m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_info.Left(index);
							m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_info.Delete(0,index+1);
							((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->AddItem(csHelpText);
							// pListbox->AddString(csHelpText,colorTexr);
							index=m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_info.Find(L'\n',0);
						}
					}	
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCListBox((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PROGRESS:
					((CTCProgress*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCProgress((CTCProgress*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PICTURE:
					((CTCPicture*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCPicture*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRCPicture(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_title);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCPicture((CTCPicture*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_SLIDER:
					((CTCSlider*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCSlider((CTCSlider*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PATH:
					((CTCBrowse*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					//((CTCBrowse*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->TCSetMode(
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCPath((CTCBrowse*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_HOTKEY:
					((CTCHotKeyCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCHotKey((CTCHotKeyCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_DATE:
					((CTCDate*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCDate((CTCDate*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_GRIDCTRL:
					((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
					if(!((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->TCGetExpandToFit())
					{
						for(int row = 0;row < ((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->GetRowCount(); row++)
						{
							int  height = m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_rowHeight[row];
							((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRowHeight(row,height);
						}
						for(int col = 0;col < ((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->GetColumnCount(); col++)
						{
							int width = m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_colWidth[col];
							((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetColumnWidth(col,width);
						}
					}
					/*((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRowCount(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_row);
					((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetColumnCount(m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_col);*/
					((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->SetTCGridCtrl((CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				default:
					break;
				}
			}
			//在这儿让Tab中的所有控件都置为未选中状态，因为创建的Tab是选中的状态
			((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->m_ControlTracker.DeSelectAll();
		}	
		return TRUE;
	}
	// Methods
	virtual bool Unexecute()
	{
		//先删除新页面中的控件
		for(int i=0;i<m_newCount;i++)
		{
			int j=0;
			while(((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->m_ControlTracker.m_arrRectanglesAll.GetSize()>0)
			{
				m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->GetCWnd();
				m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_CtrlType = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->GetCtlType();
				m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_ctrlRcInTab = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[0]->m_rect;

				int index = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.IsIncludeControl(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);

				if( -1!=index )
				{
					m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_Ctrlid = ((CTCTabPage*)(m_pCtrl->m_Pages[i]))->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
					switch(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_CtrlType)
					{
					case TCTT_BUTTON:
						((CTCButton*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_STATIC:
						((CTCStatic*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_EDIT:
						((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
						//是否密文
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCEdit*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetPassWord();
						break;
					case TCTT_RADIO:
						((CTCRadio*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCRadio*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_CHECK:
						((CTCCheck*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetWindowTextW(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = ((CTCCheck*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetCheck();
						break;
					case TCTT_COMBOBOX:
						for(int k=0;k<((CTCComboBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();k++)
						{
							CString node;
							((CTCComboBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetLBText(k,node);
							node=node+_T("\n");
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title+=node;
						}
						break;
					case TCTT_GROUP:
						((CTCGroupBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetText(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title);
						break;
					case TCTT_LISTBOX:
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetListBoxCaption();
						for(int n=0;n<((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetCount();n++)
						{
							CString node;
							node = ((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetItemText(n);
							node=node+_T("\n");
							m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_info+=node;
						}
						((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList->DestroyWindow();
						((CTCListBox*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->m_pWndList = 0;
						break;
					case TCTT_PICTURE:
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_title = ((CTCPicture*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->GetName();
						break;
					case TCTT_PATH:
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_mode = ((CTCBrowse*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab)->TCGetMode();
						break;
					case TCTT_GRIDCTRL:
						{
							CGridCtrl* pCtrl = (CGridCtrl*)m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab;
							for( int col = 0; col < pCtrl->GetColumnCount(); col++ )
							{
								m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_colWidth.push_back(pCtrl->GetColumnWidth(col));
							}
							for( int row = 0; row < pCtrl->GetRowCount(); row++ )
							{
								m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_rowHeight.push_back( pCtrl->GetRowHeight(row));
							}
						}	
						break;
					default:
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_mode = 0;
						m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_isTrue = false;
						break;
					}
				}

				((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->m_ControlTracker.RemoveFromAll(m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab);
				m_tcNewTabPage[i].m_TCCtrlsInTab[j].m_pCtrlInTab->DestroyWindow();
				j++;
			}
		}

		m_pCtrl->DeleteAllItems();

		for(int size=0;size<m_newCount;size++)
		{
			m_pCtrl->m_Pages[size]->DestroyWindow();
		}

		//添加标签
		for(int i=0;i<m_oldCount;i++)
		{
			m_pCtrl->InsertItem(m_pCtrl->GetItemCount(),m_pageTitle[i]);
			((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->Create(IDD_TCTABPAGE, m_pCtrl);
			m_pCtrl->m_Pages[i] = (CTCTabPage*)m_tcTabPage[i].m_pTabPage;
			((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->TCSetBkgrColor(m_tcTabPage[i].m_color);
			((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->TCSetBackgroundImage(m_tcTabPage[i].m_pic);
		}


		m_pCtrl->TCAdjustRect();
		int selIndex=m_pCtrl->getTabPageCount();
		m_pCtrl->SetCurSel(selIndex-1);
		m_pCtrl->showIndex(selIndex-1);

		for(int i=0;i<m_oldCount;i++)
		{
			//for(int count=0;count<m_tcTabPage[i].m_num;count++)
			for (int count = m_tcTabPage[i].m_num - 1; count >= 0; count--)
			{	
				switch(m_tcTabPage[i].m_TCCtrlsInTab[count].m_CtrlType)
				{
				case TCTT_BUTTON:
					((CTCButton*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title,WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCButton((CTCButton*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;			
				case TCTT_STATIC:
					((CTCStatic*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCStatic((CTCStatic*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_EDIT:
					((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetWindowTextW(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title);
					((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->TCSetPassWord(m_tcTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCEdit((CTCEdit*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_RADIO:
					((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetCheck(m_tcTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCRadio((CTCRadio*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_CHECK:
					((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetCheck(m_tcTabPage[i].m_TCCtrlsInTab[count].m_isTrue);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCCheck((CTCCheck*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_COMBOBOX:
					((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					if(!m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.IsEmpty())
					{
						CString csHelpText;
						int index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Find(L'\n',0);

						while(index!=-1)
						{
							csHelpText=m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Left(index);
							m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Delete(0,index+1);
							((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->AddString(csHelpText);
							index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_title.Find(L'\n',0);
						}
					}
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCCombox((CTCComboBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(NULL,m_tcTabPage[i].m_TCCtrlsInTab[count].m_title, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535,NULL);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCGroupBox((CTCGroupBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_LISTBOX:
					((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title,WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					if(!m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.IsEmpty())
					{
						CString csHelpText;
						int index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Find(L'\n',0);

						while(index!=-1)
						{
							csHelpText=m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Left(index);
							m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Delete(0,index+1);
							((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->AddItem(csHelpText);
							// pListbox->AddString(csHelpText,colorTexr);
							index=m_tcTabPage[i].m_TCCtrlsInTab[count].m_info.Find(L'\n',0);
						}
					}	
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCListBox((CTCListBox*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PROGRESS:
					((CTCProgress*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCProgress((CTCProgress*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PICTURE:
					((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRCPicture(m_tcTabPage[i].m_TCCtrlsInTab[count].m_title);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCPicture((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_SLIDER:
					((CTCSlider*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCSlider((CTCSlider*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_PATH:
					((CTCBrowse*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCPath((CTCBrowse*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_HOTKEY:
					((CTCHotKeyCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCHotKey((CTCHotKeyCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_DATE:
					((CTCDate*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCDate((CTCDate*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				case TCTT_GRIDCTRL:
					((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
					if(!((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->TCGetExpandToFit())
					{
						CGridCtrl* pCtrl = (CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab;
						for(int row = 0;row < pCtrl->GetRowCount(); row++)
						{
							int height = m_tcTabPage[i].m_TCCtrlsInTab[count].m_rowHeight[row];
							((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRowHeight(row,height);
						}
						for(int col = 0;col < pCtrl->GetColumnCount(); col++)
						{
							int width = m_tcTabPage[i].m_TCCtrlsInTab[count].m_colWidth[col];
							((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetColumnWidth(col,width);
						}
					}
					/*((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetRowCount(m_tcTabPage[i].m_TCCtrlsInTab[count].m_row);
					((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->SetColumnCount(m_tcTabPage[i].m_TCCtrlsInTab[count].m_col);*/
					((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->SetTCGridCtrl((CGridCtrl*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,FALSE,m_tcTabPage[i].m_TCCtrlsInTab[count].m_Ctrlid);
					break;
				default:
					break;
				}
			}
			//在这儿让Tab中的所有控件都置为未选中状态，因为创建的Tab是选中的状态
			((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->m_ControlTracker.DeSelectAll();
		}
		return TRUE;
	}

};


/////////////////////////控件名称(ID)属性改变动作//////////////////////////
//**********控件的属性改变后能够撤销恢复*************************//
//**********撤销恢复的时候还要改变属性框中的值*******************//
////////////////////////控件名称(ID)属性改变动作///////////////////////////
class CtrlProptyNameCommand : public UNDO::Command
{
private:
	CTCDesignView* m_pView;				//指向视类的指针

	CMFCPropertyGridProperty* m_pProp;	//指向要修改的项	
	TCTOOLTYPE m_ctrlType;				//控件的类型
	CWnd* m_pCtrl;						//指向控件

	CString m_oldCtrlID;				//控件的名称
	CString m_newCtrlID;				//新的控件的标题

	CPropertiesWnd* m_pProWnd;			//属性窗口	
public:
	CtrlProptyNameCommand(CTCDesignView* pView,CMFCPropertyGridProperty* pProp,CWnd* pCtrl,TCTOOLTYPE type,CPropertiesWnd* pWnd)
	{		
		m_pView = pView;
		m_ctrlType = type;
		m_pCtrl = pCtrl;	
		m_pProp = pProp;
			
		m_oldCtrlID = m_pProp->GetOriginalValue().bstrVal;
		m_newCtrlID = m_pProp->GetValue().bstrVal;
		m_pProWnd = pWnd;
	}

	virtual ~CtrlProptyNameCommand()
	{

	}

	// Methods
	virtual bool Execute()
	{
		if(m_pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(m_oldCtrlID,m_newCtrlID))
        {
			m_pView->m_DiaDesign.DeSelectAll();
			CControlRect* pControlTracker = m_pView->m_DiaDesign.m_ControlTracker.GetRectByID(m_newCtrlID);
			TCPARENTTYPE type = pControlTracker->GetParentType();
			switch(type)
			{
			case TCPARENT_DESDIN:
				((CDiaTCDesign*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
				break;
			case TCPARENT_TABPAGE:
				((CTCTabPage*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
				break;
			default:
				break;
			}
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_ctrlType,m_newCtrlID,m_pCtrl,TRUE);
			m_pProWnd->m_CurrentType.name = m_newCtrlID;     
			m_pView->m_DiaDesign.Invalidate();
        }
		else
			m_pProp->SetValue(m_pProp->GetOriginalValue());
		return TRUE;
	}
	virtual bool Unexecute()
	{
		if(m_pView->m_DiaDesign.m_ControlTracker.ChangeCtlID(m_newCtrlID,m_oldCtrlID))
        {
			m_pView->m_DiaDesign.DeSelectAll();
			CControlRect* pControlTracker = m_pView->m_DiaDesign.m_ControlTracker.GetRectByID(m_oldCtrlID);
			TCPARENTTYPE type = pControlTracker->GetParentType();
			switch(type)
			{
			case TCPARENT_DESDIN:
				((CDiaTCDesign*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
				break;
			case TCPARENT_TABPAGE:
				((CTCTabPage*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
				break;
			default:
				break;
			}
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_ctrlType,m_oldCtrlID,m_pCtrl,TRUE);
			m_pProWnd->m_CurrentType.name = m_oldCtrlID;       
			m_pView->m_DiaDesign.Invalidate();
        }		
		return TRUE;
	}
};


/////////////////////////控件文本属性改变动作//////////////////////////
//**********控件的属性改变后能够撤销恢复*************************//
//**********撤销恢复的时候还要改变属性框中的值*******************//
////////////////////////控件文本属性改变动作///////////////////////////
class CtrlProptyTitleCommand : public UNDO::Command
{
private:
	CMFCPropertyGridProperty* m_pProp;	//指向要修改的项
	TCTOOLTYPE m_ctrlType;				//控件的类型
	CWnd* m_pCtrl;
	CString m_id;						//控件的ID

	CString m_oldWndText;				//控件的名称
	CString m_newWndText;				//新的控件的标题	
public:
	CtrlProptyTitleCommand(CMFCPropertyGridProperty* pProp,CWnd* pCtrl,TCTOOLTYPE type,CPropertiesWnd* proWnd)
	{		
		m_pProp = pProp;
		m_pCtrl = pCtrl;
		
		m_oldWndText = m_pProp->GetOriginalValue().bstrVal;
		m_newWndText = m_pProp->GetValue().bstrVal;

		m_ctrlType = type;
		m_id = proWnd->m_CurrentType.name;
	}

	virtual ~CtrlProptyTitleCommand()
	{

	}

	// Methods
	virtual bool Execute()
	{
		switch(m_ctrlType)
		{
		case TCTT_BUTTON:
			((CTCButton*)m_pCtrl)->SetWindowTextW(m_newWndText);	
			break;
		case TCTT_STATIC:
			((CTCStatic*)m_pCtrl)->SetText(m_newWndText);
			break;
		case TCTT_EDIT:
			((CTCEdit*)m_pCtrl)->SetWindowTextW(m_newWndText);	
			break;
		case TCTT_RADIO:
			{
			((CTCRadio*)m_pCtrl)->SetWindowTextW(m_newWndText);
			CRect rt;
			((CTCRadio *)m_pCtrl)->GetWindowRect(&rt);
			((CTCRadio *)m_pCtrl)->GetParent()->ScreenToClient(&rt);
			((CTCRadio *)m_pCtrl)->GetParent()->InvalidateRect(&rt);
			}
			break;
		case TCTT_CHECK:
			{
			((CTCCheck*)m_pCtrl)->SetWindowTextW(m_newWndText);
			CRect rt;
			((CTCRadio *)m_pCtrl)->GetWindowRect(&rt);
			((CTCRadio *)m_pCtrl)->GetParent()->ScreenToClient(&rt);
			((CTCRadio *)m_pCtrl)->GetParent()->InvalidateRect(&rt);
			}
			break;
		case TCTT_COMBOBOX:
			break;
		case TCTT_GROUP:
			((CTCGroupBox*)m_pCtrl)->SetText(m_newWndText);
			break;
		case TCTT_LISTBOX:
			((CTCListBox*)m_pCtrl)->SetWindowTextW(m_newWndText);
			break;
		case TCTT_PROGRESS:
			break;
		case TCTT_DATE:
			break;
		case TCTT_PICTURE:
			((CTCPicture*)m_pCtrl)->SetRCPicture(m_newWndText);
			break;
		case TCTT_SLIDER:
			break;
		case TCTT_PATH:
			break;
		case TCTT_TAB:
			break;
		case TCTT_TIMER:
			break;
		case TCTT_HOTKEY:
			((CTCHotKeyCtrl*)m_pCtrl)->SetHotKeyName(m_newWndText);
			break;
		default:
			break;
		}

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_ctrlType,m_id,m_pCtrl,TRUE);
		return TRUE;
	}

	virtual bool Unexecute()
	{
		switch(m_ctrlType)
		{
		case TCTT_BUTTON:
			((CTCButton*)m_pCtrl)->SetWindowTextW(m_oldWndText);
			break;
		case TCTT_STATIC:
			((CTCStatic*)m_pCtrl)->SetWindowTextW(m_oldWndText);	
			break;
		case TCTT_EDIT:
			((CTCEdit*)m_pCtrl)->SetWindowTextW(m_oldWndText);	
			break;
		case TCTT_RADIO:
			((CTCRadio*)m_pCtrl)->SetWindowTextW(m_oldWndText);
			break;
		case TCTT_CHECK:
			((CTCCheck*)m_pCtrl)->SetWindowTextW(m_oldWndText);
			break;
		case TCTT_GROUP:
			((CTCGroupBox*)m_pCtrl)->SetText(m_oldWndText);
			break;
		case TCTT_LISTBOX:
			((CTCListBox*)m_pCtrl)->SetWindowTextW(m_oldWndText);
			break;
		case TCTT_PICTURE:
			((CTCPicture*)m_pCtrl)->SetRCPicture(m_oldWndText);
			break;
		case TCTT_HOTKEY:
			((CTCHotKeyCtrl*)m_pCtrl)->SetHotKeyName(m_oldWndText);
			break;
		default:
			break;
		}
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_ctrlType,m_id,m_pCtrl,TRUE);
		return TRUE;
	}
};


/////////////////////////控件颜色属性改变动作//////////////////////////
//**********控件的颜色属性改变后能够撤销恢复*************************//
//**********撤销恢复的时候还要改变属性框中的值*******************//
////////////////////////控件颜色属性改变动作///////////////////////////
class CtrlProptyColorCommand : public UNDO::Command
{
private:	
	TCTOOLTYPE m_ctrlType;				//控件的类型
	CWnd* m_pCtrl;						//控件的指针
	CString m_id;						//控件的ID
	
	COLORREF m_oldColor;				//以前的颜色
	COLORREF m_newColor;				//新的颜色

	CString m_title;					//标识改变项的名称
public:
	CtrlProptyColorCommand(TCTOOLTYPE type,CWnd* pCtrl,CString id,COLORREF color,CString title = _T(""))
	{		
		m_ctrlType = type;
		m_pCtrl = pCtrl;
		m_id = id;
		m_newColor = color;

		m_title = title;

		switch(type)
		{
		case TCTT_STATIC:
			m_oldColor = ((CTCStatic*)pCtrl)->GetBackground();
			break;
		case TCTT_EDIT:
			if( m_title == _T("文本颜色"))
				m_oldColor = ((CTCEdit*)m_pCtrl)->GetTextColor();
			else if( m_title == _T("背景颜色"))
				m_oldColor = ((CTCEdit*)m_pCtrl)->GetBackColor();
			break;
		case TCTT_GROUP:
			if( m_title == _T("标题背景颜色"))
				m_oldColor = ((CTCGroupBox*)m_pCtrl)->GetTitleBackgroundColor();
			else if( m_title ==_T("背景颜色"))
				m_oldColor = ((CTCGroupBox*)m_pCtrl)->GetClientBackgroundColor();
			else if( m_title ==_T("边框颜色"))
				m_oldColor = ((CTCGroupBox*)m_pCtrl)->GetBorderColor();
			break;
		case TCTT_GRIDCTRL:
			if( m_title == _T("普通项背景颜色"))
				m_oldColor = ((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->GetBackClr();
			else if( m_title == _T("普通项文本颜色"))
				m_oldColor = ((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->GetTextClr();
			else if( m_title == _T("固定项背景颜色"))
				m_oldColor = ((CGridCtrl*)m_pCtrl)->GetFixedBkColor();
			else if( m_title == _T("固定项文本颜色"))
				m_oldColor = ((CGridCtrl*)m_pCtrl)->GetFixedTextColor();
			break;
		default:
			break;
		}
	}

	virtual ~CtrlProptyColorCommand()
	{

	}

	// Methods
	virtual bool Execute()
	{
		switch(m_ctrlType)
		{
		case TCTT_BUTTON:
			break;
		case TCTT_STATIC:
			((CTCStatic*)m_pCtrl)->SetBackground(m_newColor);
			break;
		case TCTT_EDIT:
			if( m_title == _T("文本颜色"))
				((CTCEdit*)m_pCtrl)->SetTextColor(m_newColor);
			else if( m_title == _T("背景颜色"))
				((CTCEdit*)m_pCtrl)->SetBackColor(m_newColor);
			break;
		case TCTT_RADIO:
			
			break;
		case TCTT_CHECK:
			
			break;
		case TCTT_COMBOBOX:

			break;
		case TCTT_GROUP:
			if( m_title == _T("标题背景颜色"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(m_newColor,((CTCGroupBox*)m_pCtrl)->GetClientBackgroundColor());
			else if( m_title == _T("背景颜色"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(((CTCGroupBox*)m_pCtrl)->GetTitleBackgroundColor(),m_newColor);
			else if( m_title == _T("边框颜色"))
				((CTCGroupBox*)m_pCtrl)->SetBorderColor(m_newColor);
			break;
		case TCTT_LISTBOX:
			
			break;
		case TCTT_PROGRESS:
			break;
		case TCTT_DATE:
			break;
		case TCTT_PICTURE:
			
			break;
		case TCTT_SLIDER:
			break;
		case TCTT_PATH:
			break;
		case TCTT_TAB:
			break;
		case TCTT_TIMER:
			break;
		case TCTT_HOTKEY:
			break;
		case TCTT_GRIDCTRL:
			if( m_title == _T("普通项背景颜色"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetBackClr(m_newColor);
			else if( m_title == _T("普通项文本颜色"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetTextClr(m_newColor);
			else if( m_title == _T("固定项背景颜色"))
				((CGridCtrl*)m_pCtrl)->SetFixedBkColor(m_newColor);
			else if( m_title == _T("固定项文本颜色"))
				((CGridCtrl*)m_pCtrl)->SetFixedTextColor(m_newColor);
			((CGridCtrl*)m_pCtrl)->Invalidate(TRUE);
			break;
		default:
			break;
		}
		return TRUE;
	}

	virtual bool Unexecute()
	{
		switch(m_ctrlType)
		{
		case TCTT_BUTTON:
		
			break;
		case TCTT_STATIC:
			((CTCStatic*)m_pCtrl)->SetBackground(m_oldColor);
			break;
		case TCTT_EDIT:
			if( m_title == _T("文本颜色"))
				((CTCEdit*)m_pCtrl)->SetTextColor(m_oldColor);
			else if( m_title == _T("背景颜色"))
				((CTCEdit*)m_pCtrl)->SetBackColor(m_oldColor);
			break;
		case TCTT_RADIO:
			break;
		case TCTT_CHECK:
			break;
		case TCTT_GROUP:
			if( m_title == _T("标题背景颜色"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(m_oldColor,((CTCGroupBox*)m_pCtrl)->GetClientBackgroundColor());
			else if( m_title == _T("背景颜色"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(((CTCGroupBox*)m_pCtrl)->GetTitleBackgroundColor(),m_oldColor);
			else if( m_title == _T("边框颜色"))
				((CTCGroupBox*)m_pCtrl)->SetBorderColor(m_oldColor);
			break;
		case TCTT_LISTBOX:
			
			break;
		case TCTT_PICTURE:
		
			break;
		case TCTT_HOTKEY:
		
			break;
		case TCTT_GRIDCTRL:
			if( m_title == _T("普通项背景颜色"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetBackClr(m_oldColor);
			else if( m_title == _T("普通项文本颜色"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetTextClr(m_oldColor);
			else if( m_title == _T("固定项背景颜色"))
				((CGridCtrl*)m_pCtrl)->SetFixedBkColor(m_oldColor);
			else if( m_title == _T("固定项文本颜色"))
				((CGridCtrl*)m_pCtrl)->SetFixedTextColor(m_oldColor);
			((CGridCtrl*)m_pCtrl)->Invalidate(TRUE);
			break;
		default:
			break;
		}
		return TRUE;
	}
};


/////////////////////////控件逻辑属性改变动作//////////////////////////
//**********控件的逻辑属性改变后能够撤销恢复*************************//
//**********撤销恢复的时候还要改变属性框中的值*******************//
////////////////////////控件逻辑属性改变动作///////////////////////////
class CtrlProptyLogicCommand : public UNDO::Command
{
private:
	CTCDesignView* m_pView;				//指向视类的指针
	TCTOOLTYPE m_ctrlType;				//控件的类型
	
	CWnd* m_pCtrl;						//控件的指针
	bool m_oldLogic;					//控件的逻辑属性变量  如：选中，是否密文等
	bool m_newLogic;
	CString m_id;						//控件的ID,用来显示属性

	CString m_title;					//标识改变项的名称

	bool m_showProperty;                //如果是添加操作则不用设置属性
public:
	CtrlProptyLogicCommand(CTCDesignView* pView,TCTOOLTYPE type,CWnd* pCtrl,CString id,bool logic,CString title = _T(""))
	{		
		m_pView = pView;
		m_pCtrl = pCtrl;
		m_id = id;

		m_title = title;
		m_newLogic = logic;

		m_showProperty = false;

		switch(type)
		{
		case TCTT_STATIC:
			break;
		case TCTT_BUTTON:
			if(m_title == _T("多行"))
				m_oldLogic = ((CTCButton*)m_pCtrl)->TCGetMulityLine();
			else if(m_title == _T("默认按钮"))
				m_oldLogic = ((CTCButton*)m_pCtrl)->TCGetDefButton();
			else if(m_title == _T("有效"))
				m_oldLogic = ((CTCButton*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_EDIT:
			if(m_title == _T("有效"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("多行"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetMulityLine();
			else if(m_title == _T("密文"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetPassWord();
			else if(m_title == _T("只读"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetReadOnly();
			else if(m_title == _T("数字"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetNumber();
			else if(m_title == _T("垂直滚动条"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetVerti();
			else if(m_title == _T("水平滚动条"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetHoriz();
				break;
		case TCTT_RADIO:
			if(m_title.IsEmpty())
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetCheck();
			else if(m_title == _T("有效"))
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("标题居左"))
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetTextLeft();
			else if(m_title == _T("分组"))
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetGroup();
			break;
		case TCTT_CHECK:
			if(m_title.IsEmpty())
				m_oldLogic = ((CTCCheck*)m_pCtrl)->TCGetCheck();
			else if(m_title == _T("有效"))
				m_oldLogic = ((CTCCheck*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("标题居左"))
				m_oldLogic = ((CTCCheck*)m_pCtrl)->TCGetTextLeft();
			break;
		case TCTT_LISTBOX:
			if(m_title == _T("有效"))
				m_oldLogic = ((CTCListBox*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("默认滚动"))
				m_oldLogic = ((CTCListBox*)m_pCtrl)->TCGetDefaultRool();
			break;
		case TCTT_COMBOBOX:

			break;
		case TCTT_PROGRESS:
			if(m_title == _T("垂直"))
				m_oldLogic = ((CTCProgress*)m_pCtrl)->TCGetVertical();
			break;
		case TCTT_SLIDER:
			if(m_title == _T("垂直"))
				m_oldLogic = ((CTCSlider*)m_pCtrl)->TCGetVertical();
			else if(m_title == _T("有效"))
				m_oldLogic = ((CTCSlider*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_PATH:
			if(m_title == _T("浏览模式"))
				m_oldLogic = !m_newLogic;
			else if(m_title == _T("有效"))
				m_oldLogic = ((CTCBrowse*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_HOTKEY:
			if(m_title == _T("有效"))
				m_oldLogic = ((CTCHotKeyCtrl*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_DATE:
			if(m_title == _T("有效"))
				m_oldLogic = ((CTCDate*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_TAB:
			if(m_title == _T("有效"))
				m_oldLogic = ((CTCTab*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_GRIDCTRL:
			if(m_title == _T("可编辑"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->TCGetAllSellsEditAbled()? true : false;
			else if(m_title == _T("排序"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->GetHeaderSort()? true : false;
			else if(m_title == _T("填充"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->TCGetExpandToFit()? true : false;
			else if(m_title == _T("垂直网格"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->IsShowVertiLines();
			else if(m_title == _T("水平网格"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->IsShowHorizLines();
			else if(m_title == _T("选中"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->IsSelectable();
		default:
			break;
		}
		
		m_ctrlType = type;
	}

	virtual ~CtrlProptyLogicCommand()
	{

	}

	// Methods
	virtual bool Execute()
	{
		switch(m_ctrlType)
		{
		case TCTT_STATIC:
			break;
		case TCTT_BUTTON:
			if(m_title == _T("多行"))
				((CTCButton*)m_pCtrl)->TCSetButtonMulity(m_newLogic);
			else if(m_title == _T("默认按钮"))
				((CTCButton*)m_pCtrl)->TCSetDefButton(m_newLogic);
			else if(m_title == _T("有效"))
				((CTCButton*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_EDIT:
			if(m_title == _T("有效"))
				((CTCEdit*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("多行"))
				((CTCEdit*)m_pCtrl)->TCSetMulityLine(m_newLogic);
			else if(m_title == _T("密文"))
				((CTCEdit*)m_pCtrl)->TCSetPassWord(m_newLogic);
			else if(m_title == _T("只读"))
				((CTCEdit*)m_pCtrl)->TCSetReadOnly(m_newLogic);
			else if(m_title == _T("数字"))
				((CTCEdit*)m_pCtrl)->TCSetNumber(m_newLogic);
			else if(m_title == _T("垂直滚动条"))
				((CTCEdit*)m_pCtrl)->TCSetVertiScroll(m_newLogic);
			else if(m_title == _T("水平滚动条"))
				((CTCEdit*)m_pCtrl)->TCSetHorizScroll(m_newLogic);
			break;
		case TCTT_RADIO:
			if(m_title.IsEmpty())
				((CTCRadio*)m_pCtrl)->TCSetCheck(m_newLogic);
			else if(m_title == _T("有效"))
				((CTCRadio*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("标题居左"))
				((CTCRadio*)m_pCtrl)->TCSetTextLeft(m_newLogic);
			else if(m_title == _T("分组"))
				((CTCRadio*)m_pCtrl)->TCSetGroup(m_newLogic);
			break;
		case TCTT_CHECK:
			if(m_title.IsEmpty())
				((CTCCheck*)m_pCtrl)->TCSetCheck(m_newLogic);
			else if(m_title == _T("有效"))
				((CTCCheck*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("标题居左"))
				((CTCCheck*)m_pCtrl)->TCSetTextLeft(m_newLogic);
			break;
		case TCTT_COMBOBOX:
			if(m_title == _T("有效"))
				((CTCComboBox*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_GROUP:
			break;
		case TCTT_LISTBOX:	
			if(m_title == _T("有效"))
				((CTCListBox*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("默认滚动"))
				((CTCListBox*)m_pCtrl)->TCSetDefaultRool(m_newLogic);
			break;
		case TCTT_PROGRESS:
			if(m_title == _T("垂直"))
				((CTCProgress*)m_pCtrl)->TCSetVertical(m_newLogic);
			break;
		case TCTT_DATE:
			if(m_title == _T("有效"))
				((CTCDate*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_PICTURE:			
			break;
		case TCTT_SLIDER:
			if(m_title == _T("垂直"))
				((CTCSlider*)m_pCtrl)->TCSetVertical(m_newLogic);
			else if(m_title == _T("有效"))
				((CTCSlider*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_PATH:
			if(m_title == _T("浏览模式"))
			{
				if( m_newLogic )
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_File);
				else
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_Folder);
			}
			else if(m_title == _T("有效"))
				((CTCBrowse*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_TAB:
			if(m_title == _T("有效"))
				((CTCTab*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_TIMER:
			break;
		case TCTT_HOTKEY:	
			if(m_title == _T("有效"))
				((CTCHotKeyCtrl*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_GRIDCTRL:
				if(m_title == _T("可编辑"))
					((CGridCtrl*)m_pCtrl)->TCSetAllSellsEditAbled(m_newLogic);
				else if(m_title == _T("虚拟模式"))
				{
					((CGridCtrl*)m_pCtrl)->SetVirtualMode(m_newLogic);
					((CGridCtrl*)m_pCtrl)->SetRowCount(100);
					((CGridCtrl*)m_pCtrl)->SetColumnCount(10);
					((CGridCtrl*)m_pCtrl)->SetFixedColumnCount(1);
					((CGridCtrl*)m_pCtrl)->SetFixedRowCount(1);
				}
				else if(m_title == _T("排序"))
					((CGridCtrl*)m_pCtrl)->SetHeaderSort(m_newLogic);
				else if(m_title == _T("填充"))
				{
					if(m_newLogic)
					{
						((CGridCtrl*)m_pCtrl)->AutoSize();
						((CGridCtrl*)m_pCtrl)->ExpandToFit(m_newLogic);
					}
					else 
					{
						((CGridCtrl*)m_pCtrl)->ExpandToFit(m_newLogic);
						for(int row = 0; row < ((CGridCtrl*)m_pCtrl)->GetRowCount(); row ++)
						{
							((CGridCtrl*)m_pCtrl)->SetRowHeight(row,25);
						}
						((CGridCtrl*)m_pCtrl)->Invalidate(TRUE);
					}
				}
				else if(m_title == _T("垂直网格"))
				{
					if(m_newLogic && ((CGridCtrl*)m_pCtrl)->IsShowHorizLines())
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_BOTH);
					else if(m_newLogic && !((CGridCtrl*)m_pCtrl)->IsShowHorizLines())
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_VERT);
					else if(!m_newLogic && ((CGridCtrl*)m_pCtrl)->IsShowHorizLines())
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_HORZ);
					else
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_NONE);
				}
				else if(m_title == _T("水平网格"))
				{
					if(m_newLogic && ((CGridCtrl*)m_pCtrl)->IsShowVertiLines())
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_BOTH);
					else if(m_newLogic && !((CGridCtrl*)m_pCtrl)->IsShowVertiLines())
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_HORZ);
					else if(!m_newLogic && ((CGridCtrl*)m_pCtrl)->IsShowVertiLines())
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_VERT);
					else 
						((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_NONE);
				}
				else if(m_title == _T("选中"))
					((CGridCtrl*)m_pCtrl)->EnableSelection(m_newLogic);
				else if(m_title == _T("固定行选中"))
					((CGridCtrl*)m_pCtrl)->SetFixedRowSelection(m_newLogic);
				else if(m_title == _T("固定列选中"))
					((CGridCtrl*)m_pCtrl)->SetFixedColumnSelection(m_newLogic);
				else if(m_title == _T("列表模式"))
					((CGridCtrl*)m_pCtrl)->SetListMode(m_newLogic);
			break;
		default:
			break;
		}
		m_pView->m_DiaDesign.DeSelectAll();
		CControlRect* pControlTracker = m_pView->m_DiaDesign.m_ControlTracker.GetRectByID(m_id);
		TCPARENTTYPE type = pControlTracker->GetParentType();
		switch(type)
		{
		case TCPARENT_DESDIN:
			((CDiaTCDesign*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
			break;
		case TCPARENT_TABPAGE:
			((CTCTabPage*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
			break;
		default:
			break;
		}
		if(m_showProperty)
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_ctrlType,m_id,m_pCtrl,TRUE);
		m_pView->m_DiaDesign.Invalidate();
		m_showProperty = true;
		return TRUE;
	}

	virtual bool Unexecute()
	{
		switch(m_ctrlType)
		{
		case TCTT_STATIC:
			break;
		case TCTT_BUTTON:	
			if(m_title == _T("多行"))
				((CTCButton*)m_pCtrl)->TCSetButtonMulity(m_oldLogic);
			else if(m_title == _T("默认按钮"))
				((CTCButton*)m_pCtrl)->TCSetDefButton(m_oldLogic);
			else if(m_title == _T("有效"))
				((CTCButton*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_EDIT:
			if(m_title == _T("有效"))
				((CTCEdit*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("多行"))
				((CTCEdit*)m_pCtrl)->TCSetMulityLine(m_oldLogic);
			else if(m_title == _T("密文"))
				((CTCEdit*)m_pCtrl)->TCSetPassWord(m_oldLogic);
			else if(m_title == _T("只读"))
				((CTCEdit*)m_pCtrl)->TCSetReadOnly(m_oldLogic);
			else if(m_title == _T("数字"))
				((CTCEdit*)m_pCtrl)->TCSetNumber(m_oldLogic);
			else if(m_title == _T("垂直滚动条"))
				((CTCEdit*)m_pCtrl)->TCSetVertiScroll(m_oldLogic);
			else if(m_title == _T("水平滚动条"))
				((CTCEdit*)m_pCtrl)->TCSetHorizScroll(m_oldLogic);
			break;
		case TCTT_RADIO:
			if(m_title.IsEmpty())
				((CTCRadio*)m_pCtrl)->TCSetCheck(m_oldLogic);
			else if(m_title == _T("有效"))
				((CTCRadio*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("标题居左"))
				((CTCCheck*)m_pCtrl)->TCSetTextLeft(m_oldLogic);
			else if(m_title == _T("分组"))
				((CTCRadio*)m_pCtrl)->TCSetGroup(m_oldLogic);
			break;
		case TCTT_CHECK:
			if(m_title.IsEmpty())
				((CTCCheck*)m_pCtrl)->TCSetCheck(m_oldLogic);
			else if(m_title == _T("有效"))
				((CTCCheck*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("标题居左"))
				((CTCCheck*)m_pCtrl)->TCSetTextLeft(m_oldLogic);
			break;
		case TCTT_GROUP:
			break;
		case TCTT_LISTBOX:
			if(m_title == _T("有效"))
				((CTCListBox*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("默认滚动"))
				((CTCListBox*)m_pCtrl)->TCSetDefaultRool(m_oldLogic);
			break;
		case TCTT_COMBOBOX:
			if(m_title == _T("有效"))
				((CTCComboBox*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_PROGRESS:
			if(m_title == _T("垂直"))
				((CTCProgress*)m_pCtrl)->TCSetVertical(m_oldLogic);
			break;
		case TCTT_PATH:
			if(m_title == _T("浏览模式"))
			{
				if( m_oldLogic )
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_File);
				else
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_Folder);
			}
			else if(m_title == _T("有效"))
				((CTCBrowse*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_SLIDER:
			((CTCSlider*)m_pCtrl)->TCSetVertical(m_oldLogic);
			break;
		case TCTT_PICTURE:		
			break;
		case TCTT_HOTKEY:	
			if(m_title == _T("有效"))
				((CTCHotKeyCtrl*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_DATE:
			if(m_title == _T("有效"))
				((CTCDate*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_TAB:
			if(m_title == _T("有效"))
				((CTCTab*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_GRIDCTRL:
			if(m_title == _T("可编辑"))
				((CGridCtrl*)m_pCtrl)->TCSetAllSellsEditAbled(m_oldLogic);
			else if(m_title == _T("虚拟模式"))
			{
				((CGridCtrl*)m_pCtrl)->SetVirtualMode(m_oldLogic);
				for(int col = 0;col < ((CGridCtrl*)m_pCtrl)->GetColumnCount(); col ++)
				{
					CString title;
					title.Format(_T("第 %d 列"),col);
					((CGridCtrl*)m_pCtrl)->SetItemText(0,col,title);
				}
			}
			else if(m_title == _T("排序"))
				((CGridCtrl*)m_pCtrl)->SetHeaderSort(m_oldLogic);
			else if(m_title == _T("填充"))
			{
				if(m_oldLogic)
					((CGridCtrl*)m_pCtrl)->ExpandToFit(m_oldLogic);
				else 
				{
					((CGridCtrl*)m_pCtrl)->ExpandToFit(m_oldLogic);
					for(int row = 0; row < ((CGridCtrl*)m_pCtrl)->GetRowCount(); row ++)
					{
						((CGridCtrl*)m_pCtrl)->SetRowHeight(row,25);
					}
					((CGridCtrl*)m_pCtrl)->Invalidate(TRUE);
				}
			}
			else if(m_title == _T("垂直网格"))
			{
				if(m_oldLogic && ((CGridCtrl*)m_pCtrl)->IsShowHorizLines())
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_BOTH);
				else if(m_oldLogic && !((CGridCtrl*)m_pCtrl)->IsShowHorizLines())
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_VERT);
				else if(!m_oldLogic && ((CGridCtrl*)m_pCtrl)->IsShowHorizLines())
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_HORZ);
				else
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_NONE);
			}
			else if(m_title == _T("水平网格"))
			{
				if(m_oldLogic && ((CGridCtrl*)m_pCtrl)->IsShowVertiLines())
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_BOTH);
				else if(m_oldLogic && !((CGridCtrl*)m_pCtrl)->IsShowVertiLines())
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_HORZ);
				else if(!m_oldLogic && ((CGridCtrl*)m_pCtrl)->IsShowVertiLines())
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_VERT);
				else 
					((CGridCtrl*)m_pCtrl)->SetGridLines(GVL_NONE);
			}
			else if(m_title == _T("选中"))
				((CGridCtrl*)m_pCtrl)->EnableSelection(m_oldLogic);
			else if(m_title == _T("固定行选中"))
				((CGridCtrl*)m_pCtrl)->SetFixedRowSelection(m_oldLogic);
			else if(m_title == _T("固定列选中"))
				((CGridCtrl*)m_pCtrl)->SetFixedColumnSelection(m_oldLogic);
			else if(m_title == _T("列表模式"))
				((CGridCtrl*)m_pCtrl)->SetListMode(m_oldLogic);
			break;
		default:
			break;
		}
		m_pView->m_DiaDesign.DeSelectAll();
		CControlRect* pControlTracker = m_pView->m_DiaDesign.m_ControlTracker.GetRectByID(m_id);
		TCPARENTTYPE type = pControlTracker->GetParentType();
		switch(type)
		{
		case TCPARENT_DESDIN:
			((CDiaTCDesign*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
			break;
		case TCPARENT_TABPAGE:
			((CTCTabPage*)m_pCtrl->GetParent())->m_ControlTracker.Select(pControlTracker);
			break;
		default:
			break;
		}
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCUpdateProperty(m_ctrlType,m_id,m_pCtrl,TRUE);
		m_pView->m_DiaDesign.Invalidate();
		return TRUE;
	}
};


/////////////////////////控件内容属性改变动作//////////////////////////
//**********控件的内容属性改变后能够撤销恢复*************************//
//**********撤销恢复的时候还要改变属性框中的值*******************//
////////////////////////控件内容属性改变动作///////////////////////////
class CtrlProptyInfoCommand : public UNDO::Command
{
private:
	TCTOOLTYPE m_ctrlType;				//控件的类型
	
	CWnd* m_pCtrl;						//控件的指针
	CString m_oldInfo;					//控件旧的内容
	CString m_newInfo;					//控件新的内容
	CString m_id;						//控件的ID,用来显示属性

	WORD m_wOldVirtualKeyCode;			//保存热键的键值
	WORD m_wOldModifiers;				//保存热键的键值

	WORD m_wNewVirtualKeyCode;			//保存热键的键值
	WORD m_wNewModifiers;				//保存热键的键值
public:
	CtrlProptyInfoCommand(TCTOOLTYPE type,CWnd* pCtrl, CString id,CString newInfo=_T(""),WORD virKey =0,WORD Mod=0)
	{		
		m_pCtrl = pCtrl;
		m_id = id;	

		m_newInfo = newInfo;

		m_wNewVirtualKeyCode = virKey;
		m_wNewModifiers = Mod;

		int count=0;
		CString helpStr = _T("");

		switch(type)
		{
		case TCTT_EDIT:
			break;
		case TCTT_RADIO:
			break;
		case TCTT_CHECK:
			break;
		case TCTT_LISTBOX:
			count = ((CTCListBox*)m_pCtrl)->GetCount();
			for(int i=0;i<count;i++)
			{
				helpStr = ((CTCListBox*)m_pCtrl)->GetItemText(i);
				helpStr = helpStr + _T("\n");
				m_oldInfo+=helpStr;
			}
			break;
		case TCTT_PATH:
			break;
		case TCTT_COMBOBOX:
			count = ((CTCComboBox*)m_pCtrl)->GetCount();
			for(int i=0;i<count;i++)
			{
				((CTCComboBox*)m_pCtrl)->GetLBText(i,m_oldInfo);
				m_oldInfo = m_oldInfo + _T("\n");
			}
			break;
		case TCTT_HOTKEY:
			((CTCHotKeyCtrl*)m_pCtrl)->GetHotKey(m_wOldVirtualKeyCode,m_wOldModifiers);
			break;
		default:
			break;
		}
		
		m_ctrlType = type;
	}

	virtual ~CtrlProptyInfoCommand()
	{

	}

	// Methods
	virtual bool Execute()
	{
		int size = 0;
		int index = 0;
		CString csHelpText = _T("");
		CString csHelpNew = _T("");
		switch(m_ctrlType)
		{
		case TCTT_BUTTON:
			break;
		case TCTT_STATIC:
			break;
		case TCTT_EDIT:
			break;
		case TCTT_RADIO:
			break;
		case TCTT_CHECK:	
			break;
		case TCTT_COMBOBOX:
			((CTCComboBox*)m_pCtrl)->ResetContent();
			csHelpNew = m_newInfo;
			index = csHelpNew.Find(L'\n',0);
			while(index!=-1)
			{
				csHelpText=csHelpNew.Left(index);
				csHelpNew.Delete(0,index+1);
				((CTCComboBox*)m_pCtrl)->AddString(csHelpNew);
				 //pListbox->AddString(csHelpText,colorTexr);
				index=csHelpNew.Find(L'\n',0);
			}
			break;
		case TCTT_GROUP:
			break;
		case TCTT_LISTBOX:	
			size = ((CTCListBox*)m_pCtrl)->GetCount();
			for(int i=0;i<size;i++)
			{
				((CTCListBox*)m_pCtrl)->RemoveItem(0);
			}
			
			csHelpNew = m_newInfo;
			index = csHelpNew.Find(L'\n',0);
			while(index!=-1)
			{
				csHelpText=csHelpNew.Left(index);
				csHelpNew.Delete(0,index+1);
				((CTCListBox*)m_pCtrl)->AddItem(csHelpText);
				 //pListbox->AddString(csHelpText,colorTexr);
				index=csHelpNew.Find(L'\n',0);
			}
			break;
		case TCTT_PROGRESS:
			break;
		case TCTT_DATE:
			break;
		case TCTT_PICTURE:			
			break;
		case TCTT_SLIDER:
			break;
		case TCTT_PATH:
			break;
		case TCTT_TAB:
			break;
		case TCTT_TIMER:
			break;
		case TCTT_HOTKEY:	
			((CTCHotKeyCtrl*)m_pCtrl)->SetHotKey(m_wNewVirtualKeyCode,m_wNewModifiers);
			((CTCHotKeyCtrl*)m_pCtrl)->SetHotKeyValue(m_wNewVirtualKeyCode,m_wNewModifiers);
			break;
		default:
			break;
		}
		return TRUE;
	}

	virtual bool Unexecute()
	{
		int size = 0;
		int index = 0;
		CString csHelpText = _T("");
		CString csHelpOld = _T("");
		switch(m_ctrlType)
		{
		case TCTT_BUTTON:	
			break;
		case TCTT_STATIC:	
			break;
		case TCTT_EDIT:
			break;
		case TCTT_RADIO:
			break;
		case TCTT_CHECK:
			break;
		case TCTT_GROUP:
			break;
		case TCTT_COMBOBOX:
			((CTCComboBox*)m_pCtrl)->ResetContent();
			csHelpOld = m_oldInfo;
			index = csHelpOld.Find(L'\n',0);
			while(index!=-1)
			{
				csHelpText=csHelpOld.Left(index);
				csHelpOld.Delete(0,index+1);
				((CTCComboBox*)m_pCtrl)->AddString(csHelpOld);
				 //pListbox->AddString(csHelpText,colorTexr);
				index=csHelpOld.Find(L'\n',0);
			}
			break;
		case TCTT_LISTBOX:
			size = ((CTCListBox*)m_pCtrl)->GetCount();
			for(int i=0;i<size;i++)
			{
				((CTCListBox*)m_pCtrl)->RemoveItem(0);
			} 
			csHelpOld = m_oldInfo;
			index = csHelpOld.Find(L'\n',0);

			while(index!=-1)
			{
				csHelpText=csHelpOld.Left(index);
				csHelpOld.Delete(0,index+1);
				((CTCListBox*)m_pCtrl)->AddItem(csHelpText);
				 //pListbox->AddString(csHelpText,colorTexr);
				index=csHelpOld.Find(L'\n',0);
			}
			break;
		case TCTT_PATH:
			break;
		case TCTT_PICTURE:		
			break;
		case TCTT_HOTKEY:	
			((CTCHotKeyCtrl*)m_pCtrl)->SetHotKey(m_wOldVirtualKeyCode,m_wOldModifiers);
			((CTCHotKeyCtrl*)m_pCtrl)->SetHotKeyValue(m_wOldVirtualKeyCode,m_wOldModifiers);
			break;
		default:
			break;
		}
		return TRUE;
	}
};


/////////////////////////控件粘贴动作//////////////////////////
//**********对于选取多个控件以后排序以及拖动位置
//*能够在Redo 和 Undo 的时候循环移动
//*而不是一个一个去移动
////////////////////////控件粘贴动作///////////////////////////
class CtrlPasteCommand : public UNDO::Command
{
private:
	//Data members
	int m_TCCtrlNum;				//控件的数量
	TCPARENTTYPE m_ParentWndtype;	//父窗口的类型
	CWnd* m_pParent;

	//保存要排序的控件信息的结构体
	struct TCCTRLPASTE
	{
		CWnd* m_pCtrl;
		CRect m_rc;						//控件的大小

		TCTOOLTYPE m_CtrlType;			//控件类型
		TCCTRLPASTE()
		{
			m_rc.SetRect(0,0,0,0);
			m_CtrlType = TCTT_MAX;
		}
		void SetTCCtrl(CRect rc,TCTOOLTYPE ctrlType)
		{
			m_rc = rc;
			m_CtrlType = ctrlType;

			switch(m_CtrlType)
			{
			case TCTT_STATIC:
				m_pCtrl = new CTCStatic();
				break;
			case TCTT_BUTTON:
				m_pCtrl = new CTCButton();
				break;
			case TCTT_EDIT:
				m_pCtrl = new CTCEdit();
				break;
			case TCTT_RADIO:
				m_pCtrl = new CTCRadio();
				break;
			case TCTT_CHECK:
				m_pCtrl = new CTCCheck();
				break;
			case TCTT_COMBOBOX:
				m_pCtrl = new CTCComboBox();
				break;
			case TCTT_GROUP:
				m_pCtrl = new CTCGroupBox();
				break;
			case TCTT_LISTBOX:
				m_pCtrl = new CTCListBox();
				break;
			case TCTT_PROGRESS:
				m_pCtrl = new CTCProgress();
				break;
			case TCTT_DATE:
				m_pCtrl = new CTCDate();
				break;
			case TCTT_PICTURE:
				m_pCtrl = new CTCPicture();
				break;
			case TCTT_SLIDER:
				m_pCtrl = new CTCSlider();
				break;
			case TCTT_PATH:
				m_pCtrl = new CTCBrowse();
				break;
			case TCTT_TIMER:
				m_pCtrl = new CTCTimer();
				break;
			case TCTT_HOTKEY:
				m_pCtrl = new CTCHotKeyCtrl();
				break;
			case TCTT_TAB:
				m_pCtrl = new CTCTab();
				break;
			case TCTT_EXPLORER:
				m_pCtrl = new CTCExplorer();
				break;
			case TCTT_GRIDCTRL:
				m_pCtrl = new CGridCtrl();
				break;
			default:
				break;
			}
		}
	};
public:
	TCCTRLPASTE m_TCCtrlSort[MAXSELECTCTRLS];    //要粘贴的控件数组

	void SetTCCtrlNum(int num){
		m_TCCtrlNum = num; }
	
public:
	CtrlPasteCommand(TCPARENTTYPE parentWndtype,CWnd *parent)
	{		
		m_pParent = parent;
		m_ParentWndtype = parentWndtype;
	}

	virtual ~CtrlPasteCommand()
	{

	}
	// Methods
	virtual bool Execute()
	{
		//循环移动

		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			((CDiaTCDesign*)m_pParent)->m_ControlTracker.DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				switch(m_TCCtrlSort[i].m_CtrlType)
				{
				case TCTT_STATIC:
					((CTCStatic*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("标签"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent);
					((CDiaTCDesign*)m_pParent)->SetTCStatic((CTCStatic*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_BUTTON:
					((CTCButton*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("按钮"),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCButton((CTCButton*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_EDIT:
					((CTCEdit*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCEdit((CTCEdit*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_RADIO:
					((CTCRadio*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("单选框"), WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCRadio((CTCRadio*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_CHECK:
					((CTCCheck*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("复选框"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCCheck((CTCCheck*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_COMBOBOX:
					((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCCombox((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->Create(NULL,_T("分组框"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535,NULL);
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->SetCatptionTextColor(RGB(100,149,237));
					((CDiaTCDesign*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_LISTBOX:
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("列表框"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->SetWindowTextW(_T("列表框"));
					((CDiaTCDesign*)m_pParent)->SetTCListBox((CTCListBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_PROGRESS:
					((CTCProgress*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCProgress((CTCProgress*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_DATE:
					((CTCDate*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCDate((CTCDate*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_PICTURE:
					((CTCPicture*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCPicture((CTCPicture*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_SLIDER:
					((CTCSlider*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCSlider((CTCSlider*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_PATH:
					((CTCBrowse*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCPath((CTCBrowse*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					((CTCBrowse*)m_TCCtrlSort[i].m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode::BrowseMode_File);
					break;
				case TCTT_TIMER:
					((CTCTimer*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("时钟"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|SS_REALSIZEIMAGE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTimer*)m_TCCtrlSort[i].m_pCtrl)->SetBitmap(((CTCTimer*)m_TCCtrlSort[i].m_pCtrl)->m_bmp);
					((CDiaTCDesign*)m_pParent)->SetTCTimer((CTCTimer*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_HOTKEY:
					((CTCHotKeyCtrl*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCHotKey((CTCHotKeyCtrl*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_TAB:
					((CTCTab*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_VISIBLE|TCS_MULTILINE|TCS_TABS|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTab*)m_TCCtrlSort[i].m_pCtrl)->addPage(_T("tab1"));
					((CTCTab*)m_TCCtrlSort[i].m_pCtrl)->addPage(_T("tab2"));
					((CTCTab*)m_TCCtrlSort[i].m_pCtrl)->showIndex(0);
					((CDiaTCDesign*)m_pParent)->SetTCTab((CTCTab*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_EXPLORER:
					((CTCExplorer*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCExplorer((CTCExplorer*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc);
					break;
				case TCTT_GRIDCTRL:
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->Create(m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->EnableDragAndDrop(TRUE);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
					//pGridCtrl->GetDefaultCell(FALSE,FALSE)->SetTextClr(colorTexr);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetVirtualMode(FALSE);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetColumnCount(5);
					//初始没有固定列
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedColumnCount(0);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetRowCount(5);
					//初始设置一个固定行
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedRowCount(1);
					for(int col = 0;col < ((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->GetColumnCount(); col++)
					{
						CString title;
						title.Format(_T("第 %d 列"),col);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemText(0,col,title);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);
					}
					//设置标题项默认显示颜色
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));
					((CDiaTCDesign*)m_pParent)->SetTCGridCtrl((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc);
					break;
				default:
					break;
				}
			}

			((CDiaTCDesign*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			((CTCTabPage*)m_pParent)->m_ControlTracker.DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				switch(m_TCCtrlSort[i].m_CtrlType)
				{
				case TCTT_STATIC:
					((CTCStatic*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("标签"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent);
					((CTCTabPage*)m_pParent)->SetTCStatic((CTCStatic*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_BUTTON:
					((CTCButton*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("按钮"),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCButton((CTCButton*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_EDIT:
					((CTCEdit*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCEdit((CTCEdit*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_RADIO:
					((CTCRadio*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("单选框"), WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCRadio((CTCRadio*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_CHECK:
					((CTCCheck*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("复选框"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCCheck((CTCCheck*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_COMBOBOX:
					((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCCombox((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->Create(NULL,_T("分组框"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535,NULL);
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->SetCatptionTextColor(RGB(100,149,237));
					((CTCTabPage*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_LISTBOX:
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("列表框"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->SetWindowTextW(_T("列表框"));
					((CTCTabPage*)m_pParent)->SetTCListBox((CTCListBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_PROGRESS:
					((CTCProgress*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCProgress((CTCProgress*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_DATE:
					((CTCDate*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCDate((CTCDate*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_PICTURE:
					((CTCPicture*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("图片框"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCPicture((CTCPicture*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_SLIDER:
					((CTCSlider*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|TB_LINEUP,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCSlider((CTCSlider*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_PATH:
					((CTCBrowse*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCPath((CTCBrowse*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					((CTCBrowse*)m_TCCtrlSort[i].m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode::BrowseMode_File);
					break;
				case TCTT_HOTKEY:
					((CTCHotKeyCtrl*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCHotKey((CTCHotKeyCtrl*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_EXPLORER:
					((CTCExplorer*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCExplorer((CTCExplorer*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc);
					break;
				case TCTT_GRIDCTRL:
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->Create(m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->EnableDragAndDrop(TRUE);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
					//pGridCtrl->GetDefaultCell(FALSE,FALSE)->SetTextClr(colorTexr);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetVirtualMode(FALSE);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetColumnCount(5);
					//初始没有固定列
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedColumnCount(0);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetRowCount(5);
					//初始设置一个固定行
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedRowCount(1);
					for(int col = 0;col < ((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->GetColumnCount(); col++)
					{
						CString title;
						title.Format(_T("第 %d 列"),col);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemText(0,col,title);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);
					}
					//设置标题项默认显示颜色
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));
					((CTCTabPage*)m_pParent)->SetTCGridCtrl((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc);
					break;
				default:
					break;
				}
				((CTCTabPage*)m_pParent)->m_ControlTracker.CreateDefaultBasicCtrl();
			}
		}
		return TRUE;
	}
	virtual bool Unexecute()
	{
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			//再移回来 
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				switch(m_TCCtrlSort[i].m_CtrlType)
				{
					//避免下次创建ListBox时报错
				case TCTT_LISTBOX:
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->m_pWndList->DestroyWindow();
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->m_pWndList = NULL;
					break;
				case TCTT_TAB:
					for(int j=0;j<((CTCTab*)m_TCCtrlSort[i].m_pCtrl)->getTabPageCount();j++)
					{
						((CTCTab*)m_TCCtrlSort[i].m_pCtrl)->m_Pages[j]->DestroyWindow();
					}
					break;
				default:
					break;
				}
				m_TCCtrlSort[i].m_pCtrl->DestroyWindow();

				for(int j=0;j<((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected.GetSize();j++)
				{
					if(((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[j]->GetCWnd() == m_TCCtrlSort[i].m_pCtrl)
						((CDiaTCDesign*)m_pParent)->m_ControlTracker.DeSelect(((CDiaTCDesign*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[j]);
				}

				((CDiaTCDesign*)m_pParent)->m_ControlTracker.RemoveFromAll(m_TCCtrlSort[i].m_pCtrl);
			}
			//显示面板属性
			CString name;
			((CDiaTCDesign*)m_pParent)->GetWindowTextW(name);
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(name,(CDiaTCDesign*)m_pParent);
			((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				switch(m_TCCtrlSort[i].m_CtrlType)
				{
					//避免下次创建ListBox时报错
				case TCTT_LISTBOX:
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->m_pWndList->DestroyWindow();
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->m_pWndList = NULL;
					break;
				default:
					break;
				}
				m_TCCtrlSort[i].m_pCtrl->DestroyWindow();

				for(int j=0;j<((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected.GetSize();j++)
				{
					if(((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[j]->GetCWnd() == m_TCCtrlSort[i].m_pCtrl)
						((CTCTabPage*)m_pParent)->m_ControlTracker.DeSelect(((CTCTabPage*)m_pParent)->m_ControlTracker.m_arrRectanglesSelected[j]);
				}

				((CTCTabPage*)m_pParent)->m_ControlTracker.RemoveFromAll(m_TCCtrlSort[i].m_pCtrl);
			}
			/////////显示Tab属性避免控件属性暴露//////////////
			CString tabID;
			int size = ((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll.GetSize();
			for(int k=0;k<size;k++)
			{
				if(((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll[k]->GetCWnd() == m_pParent->GetParent() )
					tabID = ((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlID();
			}

			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(tabID,(CTCTab*)m_pParent->GetParent());
			/////////显示Tab属性避免控件属性暴露//////////////

			((CTCTabPage*)m_pParent)->Invalidate(TRUE);
		}
		return TRUE;
	}
};