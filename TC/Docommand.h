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

//////////////////��ӿؼ��Ļ���//////////////////////////
//													   //
//*************�����ؼ��ĳ����ͻָ�����******************//
//			                                           //
//////////////////��ӿؼ��Ļ���//////////////////////////
class AddCtrlCommand : public UNDO::Command
{
private:
	// Data members
	CWnd* m_pParent;
	CWnd* m_pCtrl; 
	CRect m_rc;
	TCTOOLTYPE m_CtrlType;
	TCPARENTTYPE m_ParentWndtype;
	bool m_isPaste;			//��ʶ�Ƿ���ճ�����ܵ��õ�
public:
	// Constructor
	//����1:�����ڵ�ָ�룬2���ؼ���λ��,3:�ؼ�������,4:�����ڵ�����
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
				((CTCButton*)m_pCtrl)->Create(_T("��ť"),WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCButton((CTCButton*)m_pCtrl,m_rc);
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_pCtrl)->Create(_T("��ǩ"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_rc,m_pParent);
				((CDiaTCDesign*)m_pParent)->SetTCStatic((CTCStatic*)m_pCtrl,m_rc);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCEdit((CTCEdit*)m_pCtrl,m_rc);
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCRadio((CTCRadio*)m_pCtrl,m_rc);
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCCheck((CTCCheck*)m_pCtrl,m_rc);
				break;
			case TCTT_COMBOBOX:
				((CTCComboBox*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CDiaTCDesign*)m_pParent)->SetTCCombox((CTCComboBox*)m_pCtrl,m_rc);
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_pCtrl)->Create(NULL,_T("�����"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535,NULL);
				((CTCGroupBox*)m_pCtrl)->SetCatptionTextColor(RGB(100,149,237));
				((CDiaTCDesign*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_pCtrl,m_rc);
				break;
			case TCTT_LISTBOX:
				((CTCListBox*)m_pCtrl)->Create(_T("�б��"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCListBox*)m_pCtrl)->SetWindowTextW(_T("�б��"));
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
				((CTCPicture*)m_pCtrl)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
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
				((CTCTimer*)m_pCtrl)->Create(_T("ʱ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|SS_REALSIZEIMAGE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
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
				//��ʼû�й̶���
				((CGridCtrl*)m_pCtrl)->SetFixedColumnCount(0);
				((CGridCtrl*)m_pCtrl)->SetRowCount(5);
				//��ʼ����һ���̶���
				((CGridCtrl*)m_pCtrl)->SetFixedRowCount(1);
				for(int row = 0;row < ((CGridCtrl*)m_pCtrl)->GetRowCount(); row++)
				{
					((CGridCtrl*)m_pCtrl)->SetRowHeight(row,25);
				}
				for(int col = 0;col < ((CGridCtrl*)m_pCtrl)->GetColumnCount(); col++)
				{
					CString title;
					title.Format(_T("�� %d ��"),col);
					((CGridCtrl*)m_pCtrl)->SetItemText(0,col,title);
					((CGridCtrl*)m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);				
				}
				//���ñ�����Ĭ����ʾ��ɫ
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
			//�����ѡ��״̬�ÿ�
			((CTCTabPage*)m_pParent)->DeSelectAll();
			//��ʾ��ǰ�����ռ����ڵ�Tabҳ��
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
				((CTCButton*)m_pCtrl)->Create(_T("��ť"),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCButton((CTCButton*)m_pCtrl,m_rc);
				break;
			case TCTT_STATIC:
				((CTCStatic*)m_pCtrl)->Create(_T("��ǩ"), WS_CHILD|WS_VISIBLE|WS_EX_TRANSPARENT|SS_NOTIFY|WS_CLIPSIBLINGS ,m_rc,m_pParent);
				((CTCTabPage*)m_pParent)->SetTCStatic((CTCStatic*)m_pCtrl,m_rc);
				break;
			case TCTT_EDIT:
				((CTCEdit*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCEdit((CTCEdit*)m_pCtrl,m_rc);
				break;
			case TCTT_RADIO:
				((CTCRadio*)m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCRadio((CTCRadio*)m_pCtrl,m_rc);
				break;
			case TCTT_CHECK:
				((CTCCheck*)m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCCheck((CTCCheck*)m_pCtrl,m_rc);
				break;
			case TCTT_COMBOBOX:
				((CTCComboBox*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCTabPage*)m_pParent)->SetTCCombox((CTCComboBox*)m_pCtrl,m_rc);
				break;
			case TCTT_GROUP:
				((CTCGroupBox*)m_pCtrl)->Create(NULL,_T("�����"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535,NULL);
				((CTCTabPage*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_pCtrl,m_rc);
				break;
			case TCTT_LISTBOX:
				((CTCListBox*)m_pCtrl)->Create(_T("�б��"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCListBox*)m_pCtrl)->SetWindowTextW(_T("�б��"));
				((CTCTabPage*)m_pParent)->SetTCListBox((CTCListBox*)m_pCtrl,m_rc);
				break;
			case TCTT_PROGRESS:
				((CTCProgress*)m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
				((CTCProgress*)m_pCtrl)->SetPos(70);
				((CTCTabPage*)m_pParent)->SetTCProgress((CTCProgress*)m_pCtrl,m_rc);
				break;
			case TCTT_PICTURE:
				((CTCPicture*)m_pCtrl)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|WS_BORDER|SS_NOTIFY|WS_CLIPSIBLINGS,m_rc,m_pParent,65535);
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
				//��ʼû�й̶���
				((CGridCtrl*)m_pCtrl)->SetFixedColumnCount(0);
				((CGridCtrl*)m_pCtrl)->SetRowCount(5);
				//��ʼ����һ���̶���
				((CGridCtrl*)m_pCtrl)->SetFixedRowCount(1);
				for(int row = 0;row < ((CGridCtrl*)m_pCtrl)->GetRowCount(); row++)
				{
					((CGridCtrl*)m_pCtrl)->SetRowHeight(row,25);
				}
				//����������������
				for(int col = 0;col < ((CGridCtrl*)m_pCtrl)->GetColumnCount(); col++)
				{
					CString title;
					title.Format(_T("�� %d ��"),col);
					((CGridCtrl*)m_pCtrl)->SetItemText(0,col,title);
					((CGridCtrl*)m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);			
				}
				//���ñ�����Ĭ����ʾ��ɫ
				((CGridCtrl*)m_pCtrl)->SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));
				((CTCTabPage*)m_pParent)->SetTCGridCtrl((CGridCtrl*)m_pCtrl,m_rc);
				break;
			default:
				break;
			}
			//ˢ��
			((CTCTabPage*)m_pParent)->Invalidate(TRUE);
		}
		return true;
	}

	virtual bool Unexecute()
	{
		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			//ɾ���ؼ���ʱ������ѡ�пؼ��ÿ�
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
				//�����ֱ�ӽ�ListBox�е�ListCtrlָ���ÿ�,�Ϳ���Redo���������ǲ�֪����û��ʲô�׶�
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

			//�Ƴ��ؼ�����ʾ�������ԣ������û��������Ա���
			CString title;
			((CDiaTCDesign*)m_pParent)->GetWindowTextW(title);
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(title,(CDiaTCDesign*)m_pParent);
			//�Ƴ��ؼ�����ʾ�������ԣ������û��������Ա���
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
				//�����ֱ�ӽ�ListBox�е�ListCtrlָ���ÿ�,�Ϳ���Redo���������ǲ�֪����û��ʲô�׶�
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

			/////////��ʾTab���Ա���ؼ����Ա�¶//////////////
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



//////////////////���Tab�ؼ��Ķ���///////////////////////
//													   //
//************����Tab�ؼ��ĳ����ͻָ�����****************//
//													   //
//////////////////���Tab�ؼ��Ķ���///////////////////////
class AddTabCommand : public UNDO::Command
{
private:
	//Data members
	CWnd* m_pParent;
	CWnd* m_pCtrl;
	CRect m_rc;
	CStringArray m_pageTitle;  //��¼Tab�ı�ǩ����
	CTCTabPage *m_pTabPage[32]; //ָ��Tab��ҳ���ָ������
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
		//���˷�Χ
		((CTCTab*)m_pCtrl)->TCAdjustRect();
		//����Tab�����Ϣ
		((CDiaTCDesign*)m_pParent)->SetTCTab((CTCTab*)m_pCtrl,m_rc);
		return TRUE;
	}
	virtual bool Unexecute()
	{
		////ɾ����Ӧ����ҳ��
		for(int i=0;i<((CTCTab*)m_pCtrl)->getTabPageCount();i++)
		{
			((CTCTab*)m_pCtrl)->m_Pages[i]->DestroyWindow();
		}
		//���ɾ��Tab�ؼ�
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



//////////////////////�ƶ������϶��ؼ���С////////////////////
//***************�϶��ؼ���С�ĳ����ͻָ�*******************//
//*************�϶��ؼ���Сֻ�ǶԵ�һ�ؼ��Ĳ���**************//
//////////////////////�ƶ������϶��ؼ���С////////////////////
class OnSizeCtrlCommand : public UNDO::Command
{
private:
	// Data members
	CWnd* m_pCtrl;
	CWnd* m_pParent;
	CRect m_rc;						//�ؼ��Ĵ�С
	CRect m_newrc;					//�ؼ�Ҫ�ƶ�����λ��
	TCTOOLTYPE m_CtrlType;			//�ؼ�����
	TCPARENTTYPE m_ParentWndtype;	//�����ڵ�����


public:
	// Constructor & destructor

	//����1��������ָ��
	//����2���ؼ�ָ��
	//����3���ؼ�Ҫ�ƶ�����λ��
	//����4���ؼ�����
	//����5������������
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
			//���ؿؼ���m_ControlTrackerAll�е�λ��
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
				//ˢ�´���,������Ƥ����ʾ����ʱ
				((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
			}
		}
		else if( m_ParentWndtype == TCPARENT_TABPAGE )
		{
			//���ؿؼ���m_ControlTrackerAll�е�λ��
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
				//ˢ�´���,������Ƥ����ʾ����ʱ
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
				//ˢ�´���,������Ƥ����ʾ����ʱ
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
				//ˢ�´���,������Ƥ����ʾ����ʱ
				((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->Invalidate(TRUE);
			}
		}

		return true;
	}
};


/////////////////////////�ؼ�������//////////////////////////
//**********����ѡȡ����ؼ��Ժ������Լ��϶�λ��
//*�ܹ���Redo �� Undo ��ʱ��ѭ���ƶ�
//*������һ��һ��ȥ�ƶ�
////////////////////////�ؼ�������///////////////////////////
class SortCtrlCommand : public UNDO::Command
{
private:
	//Data members
	TCPARENTTYPE m_ParentWndtype;	//�����ڵ�����
	CWnd* m_pParent;				//�����ڵ�ָ��
	int m_TCCtrlNum;				//�ؼ�������

	//����Ҫ����Ŀؼ���Ϣ�Ľṹ��
	struct TCCTRLSORT
	{
		CWnd* m_pCtrl;
		CRect m_rc;						//�ؼ��Ĵ�С
		CRect m_newrc;					//�ؼ�Ҫ�ƶ�����λ��

		TCTOOLTYPE m_CtrlType;			//�ؼ�����

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
		//ѭ���ƶ�
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
			//���ƻ��� 
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



/////////////////////////�ؼ�������//////////////////////////
//**********����ѡȡ����ؼ��Ժ������������
//*�ܹ���Redo �� Undo ��ʱ��ѭ���ƶ�
//*������һ��һ��ȥ�ƶ�
////////////////////////�ؼ�������///////////////////////////
class UniformCtrlCommand : public UNDO::Command
{
private:
	//Data members
	TCPARENTTYPE m_ParentWndtype;	//�����ڵ�����
	CWnd* m_pParent;				//�����ڵ�ָ��
	int m_TCCtrlNum;				//�ؼ�������

	//����Ҫ����Ŀؼ���Ϣ�Ľṹ��
	struct TCCTRLSORT
	{	
		CWnd* m_pCtrl;
		CRect m_rc;						//�ؼ��Ĵ�С
		CRect m_newrc;					//�ؼ�Ҫ�ƶ�����λ��
		TCTOOLTYPE m_CtrlType;			//�ؼ�����

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
		//ѭ���ƶ�
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
			//���ƻ��� 
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


/////////////////ɾ���ؼ�����/////////////////////
//*ɾ�������ؼ�
/////////////////ɾ���ؼ�����/////////////////////
class ClearCtrlCommand : public UNDO::Command
{

	struct TCCTRLPROPERTY
	{
		CWnd* m_pCtrl;								//�ؼ���ָ��
		TCTOOLTYPE m_CtrlType;						//�ؼ�����
		CString m_title;							//�ؼ��ı���
		CString m_id;								//�ؼ���id
		CRect m_rc;									//�ؼ��Ĵ�С
		TCPARENTTYPE m_ParentWndtype;				//�����ڵ�����
		CWnd* m_pParent;

		WORD m_wVirtualKeyCode;						//�ȼ��ļ�ֵ
		WORD m_wModifiers;							//�����ȼ��ļ�ֵ

		CString m_info;								//�ؼ����������(�б������)
		int m_mode;									//��������ģʽ

		bool m_ckecked;								//�ؼ��Ƿ�ѡ��(Radio/Check)
		bool m_able;								//�ؼ��Ƿ���Ч(able/disable)
		bool m_textleft;							//�������
		bool m_mulityline;							//�������
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
	//����ؼ���Ϣ�Ľṹ��
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
			//�ؼ���m_ControlTracker�е����
			int index = ((CDiaTCDesign*)pParent)->m_ControlTracker.IsIncludeControl(pCtrl);
			if( -1 != index )
			{
				//����ؼ���ID 
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
			//�ؼ���m_ControlTracker�е����
			int index = ((CTCTabPage*)pParent)->m_ControlTracker.IsIncludeControl(pCtrl);
			if( -1 != index )
			{
				//����ؼ���ID 
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
	//ִ��ɾ���ӿ�
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
				//�Ƿ�����
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
			//���ٴ���
			m_tcCtrlPerproty.m_pCtrl->DestroyWindow();

			//����ǰ�ؼ���ѡ���������Ƴ�
			for(int i=0;i<((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected.GetSize();i++)
			{
				if(((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected[i]->GetCWnd() == m_tcCtrlPerproty.m_pCtrl)
					((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.DeSelect(((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.m_arrRectanglesSelected[i]);
			}

			((CDiaTCDesign*)m_tcCtrlPerproty.m_pParent)->m_ControlTracker.RemoveFromAll(m_tcCtrlPerproty.m_pCtrl);
			
			//��ʾ�������
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
				//�Ƿ�����
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
			//���ٴ���
			m_tcCtrlPerproty.m_pCtrl->DestroyWindow();

			//��ѡ���������Ƴ�
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
				((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
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
				((CTCTimer*)m_tcCtrlPerproty.m_pCtrl)->Create(_T("ʱ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|SS_REALSIZEIMAGE|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
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
				((CTCPicture*)m_tcCtrlPerproty.m_pCtrl)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcCtrlPerproty.m_rc,m_tcCtrlPerproty.m_pParent,65535);
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



/////////////////////ɾ��Tab�ؼ��Ķ���////////////////////////
//*Ҫ������ҳ���Ӧ�Ŀؼ�Ҳ����ɾ������ 
//*��������Redo��Tab�ؼ��Ժ�ɾ��������
//*�ؼ�Ҳ�ָܻ�
/////////////////////ɾ��Tab�ؼ��Ķ���////////////////////////
class ClearTabCommand : public UNDO::Command
{
private:
	//Data members
	CWnd* m_pParent;
	CTCTab* m_pCtrl;
	CRect m_rc;
	CString m_id;	//Tab��ID

	//��������TabPage�еĿؼ���Ϣ�Ľṹ��
	struct	TCCTRLINTABPAGE 
	{
		CWnd* m_pCtrlInTab;
		TCTOOLTYPE m_CtrlType;    //��¼Tab�еĿؼ�����
		CRect m_ctrlRcInTab;	  //�ؼ���λ��
		CString m_title;		  //�ؼ��ı���
		CString m_Ctrlid;		  //�ؼ���ID

		bool m_isTrue;			  //��ʶ�ؼ��Ƿ�ѡ�У��Ƿ����ĵȱ���	

		CString m_info;			 //���ؼ����б�����������ʱ������������

		int m_mode;					//��������ģʽ

		WORD m_wVirtualKeyCode;		//�ȼ��ļ�ֵ
		WORD m_wModifiers;			//�ȼ��ļ�ֵ

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
		int m_num;			//Tab�пؼ�������
		TCCTRLINTABPAGE m_TCCtrlsInTab[MAXSELECTCTRLS]; //����Tab�еĿؼ���Ϣ
	};

public:
	CStringArray m_pageTitle;  //��¼Tab�ı�ǩ����
	TCTABPAGE m_tcTabPage[32];

public:
	ClearTabCommand(CWnd *parent,CTCTab* pCtrl,CRect rc)
	{
		m_pParent = parent;
		m_pCtrl = pCtrl;
		m_rc = rc;
		int index = ((CDiaTCDesign*)parent)->m_ControlTracker.IsIncludeControl(pCtrl);
		//Tab��ID
		m_id = ((CDiaTCDesign*)parent)->m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
	}
	virtual ~ClearTabCommand()
	{

	}
	virtual bool Execute()
	{
		////ɾ����Ӧ����ҳ���Լ�ҳ���Ӧ���ӿؼ�
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
					//����õ��ؼ���ID������
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
						//�Ƿ�����
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
		//���ɾ��Tab�ؼ�
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
					((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
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
				//�������Tab�е����пؼ�����Ϊδѡ��״̬����Ϊ������Tab��ѡ�е�״̬
				((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->m_ControlTracker.DeSelectAll();
			}
		}	
		//��ʾTab�����Բ���Tab��ID���û���
		((CDiaTCDesign*)m_pParent)->SetTCTab((CTCTab*)m_pCtrl,m_rc,m_id);
		((CTCTab*)m_pCtrl)->TCAdjustRect();
		((CDiaTCDesign*)m_pParent)->Invalidate(TRUE);
		return TRUE;
	}

};



/////////////////////Tab�ؼ������Ӻ�ɾ��ҳ�涯��////////////////////////
//*Ҫ������ҳ���Ӧ�Ŀؼ�Ҳ����ɾ������ 
//*��������Redo��Tab�ؼ��Ժ�ɾ��������
//*�ؼ�Ҳ�ָܻ�
/////////////////////ɾ��Tab����ӿؼ�ҳ��Ķ���////////////////////////
class TabPropertyCommand : public UNDO::Command
{
private:
	//Data members
	CWnd* m_pParent;		//ָ�򸸴���
	CTCTab* m_pCtrl;		//ָ��ؼ�
	CRect m_rc;				//�ؼ��Ĵ�С

	int m_newCount;			//������ʶ�µ�ҳ�������
	int m_oldCount;			//�ɵ�Tabҳ�������

	struct	TCCTRLINTABPAGE		//��������TabPage�еĿؼ���Ϣ�Ľṹ��
	{
		CWnd* m_pCtrlInTab;
		TCTOOLTYPE m_CtrlType;    //��¼Tab�еĿؼ�����
		CRect m_ctrlRcInTab;	  //�ؼ���λ��

		CString m_title;		  //�ؼ��ı���
		CString m_Ctrlid;		  //�ؼ���ID

		bool m_isTrue;			  //��ʶ�ؼ��Ƿ�ѡ�У��Ƿ����ĵȱ���	
		CString m_info;			 //���ؼ����б�����������ʱ������������

		int m_mode;					//��������ģʽ
		vector< int > m_colWidth;	//�����п�
		vector< int > m_rowHeight;	//�����и�

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
		COLORREF m_color;	//TabPage��ɫֵ
		CString m_pic;		//TabpageͼƬ
		int m_num;			//Tab�пؼ�������
		TCCTRLINTABPAGE m_TCCtrlsInTab[MAXSELECTCTRLS]; //����Tab�еĿؼ���Ϣ
		TCTABPAGE()
		{
			m_pTabPage = NULL;
			m_color = ::GetSysColor(COLOR_BTNFACE);
			m_pic = _T("");
			m_num = 0;
		}
	};


public:
	CStringArray m_pageTitle;        //��¼Tab�ı�ǩ����
	CStringArray m_pageNewTitle;     //��¼��Tab�ı�ǩ����
	TCTABPAGE m_tcTabPage[32];       //��¼Tab��ǰ��ҳ�����Ϣ
	TCTABPAGE m_tcNewTabPage[32];    //��¼�µ�Ҫ�����Tabҳ����Ϣ

public:
	TabPropertyCommand(CWnd *parent,CTCTab* pCtrl,CVSListBox *listBox)
	{
		m_pParent = parent;
		m_pCtrl = pCtrl;
		m_pCtrl->GetWindowRect(m_rc);
		m_pParent->ScreenToClient(&m_rc);

		m_oldCount=m_pCtrl->getTabPageCount();

		//�����ɵ�Tab��Ϣ
		for( int i=0;i<m_oldCount;i++ )
		{
			//////����Tab�����б���
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
			//�༭���������
			if(data==0)
			{
				CTCTabPage *page=new CTCTabPage();
				m_tcNewTabPage[i].m_pTabPage = page;
				m_tcNewTabPage[i].m_num = 0;
				//�µľ�Ĭ�ϴ�����ɫ
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
							//�Ƿ�����
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

		//��¼�����پɵ�Tab�еĿؼ�
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
					//�õ��ؼ���ID
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
						//�Ƿ�����
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
		//��ɾ����ҳ���еĿؼ�
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
					//�õ��ؼ���ID
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
						//�Ƿ�����
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

		//��ӱ�ǩ
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
					((CTCPicture*)m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcNewTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcNewTabPage[i].m_pTabPage,65535);
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
			//�������Tab�е����пؼ�����Ϊδѡ��״̬����Ϊ������Tab��ѡ�е�״̬
			((CTCTabPage*)m_tcNewTabPage[i].m_pTabPage)->m_ControlTracker.DeSelectAll();
		}	
		return TRUE;
	}
	// Methods
	virtual bool Unexecute()
	{
		//��ɾ����ҳ���еĿؼ�
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
						//�Ƿ�����
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

		//��ӱ�ǩ
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
					((CTCPicture*)m_tcTabPage[i].m_TCCtrlsInTab[count].m_pCtrlInTab)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_tcTabPage[i].m_TCCtrlsInTab[count].m_ctrlRcInTab,m_tcTabPage[i].m_pTabPage,65535);
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
			//�������Tab�е����пؼ�����Ϊδѡ��״̬����Ϊ������Tab��ѡ�е�״̬
			((CTCTabPage*)m_tcTabPage[i].m_pTabPage)->m_ControlTracker.DeSelectAll();
		}
		return TRUE;
	}

};


/////////////////////////�ؼ�����(ID)���Ըı䶯��//////////////////////////
//**********�ؼ������Ըı���ܹ������ָ�*************************//
//**********�����ָ���ʱ��Ҫ�ı����Կ��е�ֵ*******************//
////////////////////////�ؼ�����(ID)���Ըı䶯��///////////////////////////
class CtrlProptyNameCommand : public UNDO::Command
{
private:
	CTCDesignView* m_pView;				//ָ�������ָ��

	CMFCPropertyGridProperty* m_pProp;	//ָ��Ҫ�޸ĵ���	
	TCTOOLTYPE m_ctrlType;				//�ؼ�������
	CWnd* m_pCtrl;						//ָ��ؼ�

	CString m_oldCtrlID;				//�ؼ�������
	CString m_newCtrlID;				//�µĿؼ��ı���

	CPropertiesWnd* m_pProWnd;			//���Դ���	
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


/////////////////////////�ؼ��ı����Ըı䶯��//////////////////////////
//**********�ؼ������Ըı���ܹ������ָ�*************************//
//**********�����ָ���ʱ��Ҫ�ı����Կ��е�ֵ*******************//
////////////////////////�ؼ��ı����Ըı䶯��///////////////////////////
class CtrlProptyTitleCommand : public UNDO::Command
{
private:
	CMFCPropertyGridProperty* m_pProp;	//ָ��Ҫ�޸ĵ���
	TCTOOLTYPE m_ctrlType;				//�ؼ�������
	CWnd* m_pCtrl;
	CString m_id;						//�ؼ���ID

	CString m_oldWndText;				//�ؼ�������
	CString m_newWndText;				//�µĿؼ��ı���	
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


/////////////////////////�ؼ���ɫ���Ըı䶯��//////////////////////////
//**********�ؼ�����ɫ���Ըı���ܹ������ָ�*************************//
//**********�����ָ���ʱ��Ҫ�ı����Կ��е�ֵ*******************//
////////////////////////�ؼ���ɫ���Ըı䶯��///////////////////////////
class CtrlProptyColorCommand : public UNDO::Command
{
private:	
	TCTOOLTYPE m_ctrlType;				//�ؼ�������
	CWnd* m_pCtrl;						//�ؼ���ָ��
	CString m_id;						//�ؼ���ID
	
	COLORREF m_oldColor;				//��ǰ����ɫ
	COLORREF m_newColor;				//�µ���ɫ

	CString m_title;					//��ʶ�ı��������
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
			if( m_title == _T("�ı���ɫ"))
				m_oldColor = ((CTCEdit*)m_pCtrl)->GetTextColor();
			else if( m_title == _T("������ɫ"))
				m_oldColor = ((CTCEdit*)m_pCtrl)->GetBackColor();
			break;
		case TCTT_GROUP:
			if( m_title == _T("���ⱳ����ɫ"))
				m_oldColor = ((CTCGroupBox*)m_pCtrl)->GetTitleBackgroundColor();
			else if( m_title ==_T("������ɫ"))
				m_oldColor = ((CTCGroupBox*)m_pCtrl)->GetClientBackgroundColor();
			else if( m_title ==_T("�߿���ɫ"))
				m_oldColor = ((CTCGroupBox*)m_pCtrl)->GetBorderColor();
			break;
		case TCTT_GRIDCTRL:
			if( m_title == _T("��ͨ�����ɫ"))
				m_oldColor = ((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->GetBackClr();
			else if( m_title == _T("��ͨ���ı���ɫ"))
				m_oldColor = ((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->GetTextClr();
			else if( m_title == _T("�̶������ɫ"))
				m_oldColor = ((CGridCtrl*)m_pCtrl)->GetFixedBkColor();
			else if( m_title == _T("�̶����ı���ɫ"))
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
			if( m_title == _T("�ı���ɫ"))
				((CTCEdit*)m_pCtrl)->SetTextColor(m_newColor);
			else if( m_title == _T("������ɫ"))
				((CTCEdit*)m_pCtrl)->SetBackColor(m_newColor);
			break;
		case TCTT_RADIO:
			
			break;
		case TCTT_CHECK:
			
			break;
		case TCTT_COMBOBOX:

			break;
		case TCTT_GROUP:
			if( m_title == _T("���ⱳ����ɫ"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(m_newColor,((CTCGroupBox*)m_pCtrl)->GetClientBackgroundColor());
			else if( m_title == _T("������ɫ"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(((CTCGroupBox*)m_pCtrl)->GetTitleBackgroundColor(),m_newColor);
			else if( m_title == _T("�߿���ɫ"))
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
			if( m_title == _T("��ͨ�����ɫ"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetBackClr(m_newColor);
			else if( m_title == _T("��ͨ���ı���ɫ"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetTextClr(m_newColor);
			else if( m_title == _T("�̶������ɫ"))
				((CGridCtrl*)m_pCtrl)->SetFixedBkColor(m_newColor);
			else if( m_title == _T("�̶����ı���ɫ"))
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
			if( m_title == _T("�ı���ɫ"))
				((CTCEdit*)m_pCtrl)->SetTextColor(m_oldColor);
			else if( m_title == _T("������ɫ"))
				((CTCEdit*)m_pCtrl)->SetBackColor(m_oldColor);
			break;
		case TCTT_RADIO:
			break;
		case TCTT_CHECK:
			break;
		case TCTT_GROUP:
			if( m_title == _T("���ⱳ����ɫ"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(m_oldColor,((CTCGroupBox*)m_pCtrl)->GetClientBackgroundColor());
			else if( m_title == _T("������ɫ"))
				((CTCGroupBox*)m_pCtrl)->SetBackgroundColor(((CTCGroupBox*)m_pCtrl)->GetTitleBackgroundColor(),m_oldColor);
			else if( m_title == _T("�߿���ɫ"))
				((CTCGroupBox*)m_pCtrl)->SetBorderColor(m_oldColor);
			break;
		case TCTT_LISTBOX:
			
			break;
		case TCTT_PICTURE:
		
			break;
		case TCTT_HOTKEY:
		
			break;
		case TCTT_GRIDCTRL:
			if( m_title == _T("��ͨ�����ɫ"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetBackClr(m_oldColor);
			else if( m_title == _T("��ͨ���ı���ɫ"))
				((CGridCtrl*)m_pCtrl)->GetDefaultCell(FALSE,FALSE)->SetTextClr(m_oldColor);
			else if( m_title == _T("�̶������ɫ"))
				((CGridCtrl*)m_pCtrl)->SetFixedBkColor(m_oldColor);
			else if( m_title == _T("�̶����ı���ɫ"))
				((CGridCtrl*)m_pCtrl)->SetFixedTextColor(m_oldColor);
			((CGridCtrl*)m_pCtrl)->Invalidate(TRUE);
			break;
		default:
			break;
		}
		return TRUE;
	}
};


/////////////////////////�ؼ��߼����Ըı䶯��//////////////////////////
//**********�ؼ����߼����Ըı���ܹ������ָ�*************************//
//**********�����ָ���ʱ��Ҫ�ı����Կ��е�ֵ*******************//
////////////////////////�ؼ��߼����Ըı䶯��///////////////////////////
class CtrlProptyLogicCommand : public UNDO::Command
{
private:
	CTCDesignView* m_pView;				//ָ�������ָ��
	TCTOOLTYPE m_ctrlType;				//�ؼ�������
	
	CWnd* m_pCtrl;						//�ؼ���ָ��
	bool m_oldLogic;					//�ؼ����߼����Ա���  �磺ѡ�У��Ƿ����ĵ�
	bool m_newLogic;
	CString m_id;						//�ؼ���ID,������ʾ����

	CString m_title;					//��ʶ�ı��������

	bool m_showProperty;                //�������Ӳ���������������
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
			if(m_title == _T("����"))
				m_oldLogic = ((CTCButton*)m_pCtrl)->TCGetMulityLine();
			else if(m_title == _T("Ĭ�ϰ�ť"))
				m_oldLogic = ((CTCButton*)m_pCtrl)->TCGetDefButton();
			else if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCButton*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_EDIT:
			if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("����"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetMulityLine();
			else if(m_title == _T("����"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetPassWord();
			else if(m_title == _T("ֻ��"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetReadOnly();
			else if(m_title == _T("����"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetNumber();
			else if(m_title == _T("��ֱ������"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetVerti();
			else if(m_title == _T("ˮƽ������"))
				m_oldLogic = ((CTCEdit*)m_pCtrl)->TCGetHoriz();
				break;
		case TCTT_RADIO:
			if(m_title.IsEmpty())
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetCheck();
			else if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("�������"))
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetTextLeft();
			else if(m_title == _T("����"))
				m_oldLogic = ((CTCRadio*)m_pCtrl)->TCGetGroup();
			break;
		case TCTT_CHECK:
			if(m_title.IsEmpty())
				m_oldLogic = ((CTCCheck*)m_pCtrl)->TCGetCheck();
			else if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCCheck*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("�������"))
				m_oldLogic = ((CTCCheck*)m_pCtrl)->TCGetTextLeft();
			break;
		case TCTT_LISTBOX:
			if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCListBox*)m_pCtrl)->TCGetAbled();
			else if(m_title == _T("Ĭ�Ϲ���"))
				m_oldLogic = ((CTCListBox*)m_pCtrl)->TCGetDefaultRool();
			break;
		case TCTT_COMBOBOX:

			break;
		case TCTT_PROGRESS:
			if(m_title == _T("��ֱ"))
				m_oldLogic = ((CTCProgress*)m_pCtrl)->TCGetVertical();
			break;
		case TCTT_SLIDER:
			if(m_title == _T("��ֱ"))
				m_oldLogic = ((CTCSlider*)m_pCtrl)->TCGetVertical();
			else if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCSlider*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_PATH:
			if(m_title == _T("���ģʽ"))
				m_oldLogic = !m_newLogic;
			else if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCBrowse*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_HOTKEY:
			if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCHotKeyCtrl*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_DATE:
			if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCDate*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_TAB:
			if(m_title == _T("��Ч"))
				m_oldLogic = ((CTCTab*)m_pCtrl)->TCGetAbled();
			break;
		case TCTT_GRIDCTRL:
			if(m_title == _T("�ɱ༭"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->TCGetAllSellsEditAbled()? true : false;
			else if(m_title == _T("����"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->GetHeaderSort()? true : false;
			else if(m_title == _T("���"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->TCGetExpandToFit()? true : false;
			else if(m_title == _T("��ֱ����"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->IsShowVertiLines();
			else if(m_title == _T("ˮƽ����"))
				m_oldLogic = ((CGridCtrl*)m_pCtrl)->IsShowHorizLines();
			else if(m_title == _T("ѡ��"))
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
			if(m_title == _T("����"))
				((CTCButton*)m_pCtrl)->TCSetButtonMulity(m_newLogic);
			else if(m_title == _T("Ĭ�ϰ�ť"))
				((CTCButton*)m_pCtrl)->TCSetDefButton(m_newLogic);
			else if(m_title == _T("��Ч"))
				((CTCButton*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_EDIT:
			if(m_title == _T("��Ч"))
				((CTCEdit*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("����"))
				((CTCEdit*)m_pCtrl)->TCSetMulityLine(m_newLogic);
			else if(m_title == _T("����"))
				((CTCEdit*)m_pCtrl)->TCSetPassWord(m_newLogic);
			else if(m_title == _T("ֻ��"))
				((CTCEdit*)m_pCtrl)->TCSetReadOnly(m_newLogic);
			else if(m_title == _T("����"))
				((CTCEdit*)m_pCtrl)->TCSetNumber(m_newLogic);
			else if(m_title == _T("��ֱ������"))
				((CTCEdit*)m_pCtrl)->TCSetVertiScroll(m_newLogic);
			else if(m_title == _T("ˮƽ������"))
				((CTCEdit*)m_pCtrl)->TCSetHorizScroll(m_newLogic);
			break;
		case TCTT_RADIO:
			if(m_title.IsEmpty())
				((CTCRadio*)m_pCtrl)->TCSetCheck(m_newLogic);
			else if(m_title == _T("��Ч"))
				((CTCRadio*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("�������"))
				((CTCRadio*)m_pCtrl)->TCSetTextLeft(m_newLogic);
			else if(m_title == _T("����"))
				((CTCRadio*)m_pCtrl)->TCSetGroup(m_newLogic);
			break;
		case TCTT_CHECK:
			if(m_title.IsEmpty())
				((CTCCheck*)m_pCtrl)->TCSetCheck(m_newLogic);
			else if(m_title == _T("��Ч"))
				((CTCCheck*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("�������"))
				((CTCCheck*)m_pCtrl)->TCSetTextLeft(m_newLogic);
			break;
		case TCTT_COMBOBOX:
			if(m_title == _T("��Ч"))
				((CTCComboBox*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_GROUP:
			break;
		case TCTT_LISTBOX:	
			if(m_title == _T("��Ч"))
				((CTCListBox*)m_pCtrl)->TCSetAbled(m_newLogic);
			else if(m_title == _T("Ĭ�Ϲ���"))
				((CTCListBox*)m_pCtrl)->TCSetDefaultRool(m_newLogic);
			break;
		case TCTT_PROGRESS:
			if(m_title == _T("��ֱ"))
				((CTCProgress*)m_pCtrl)->TCSetVertical(m_newLogic);
			break;
		case TCTT_DATE:
			if(m_title == _T("��Ч"))
				((CTCDate*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_PICTURE:			
			break;
		case TCTT_SLIDER:
			if(m_title == _T("��ֱ"))
				((CTCSlider*)m_pCtrl)->TCSetVertical(m_newLogic);
			else if(m_title == _T("��Ч"))
				((CTCSlider*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_PATH:
			if(m_title == _T("���ģʽ"))
			{
				if( m_newLogic )
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_File);
				else
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_Folder);
			}
			else if(m_title == _T("��Ч"))
				((CTCBrowse*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_TAB:
			if(m_title == _T("��Ч"))
				((CTCTab*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_TIMER:
			break;
		case TCTT_HOTKEY:	
			if(m_title == _T("��Ч"))
				((CTCHotKeyCtrl*)m_pCtrl)->TCSetAbled(m_newLogic);
			break;
		case TCTT_GRIDCTRL:
				if(m_title == _T("�ɱ༭"))
					((CGridCtrl*)m_pCtrl)->TCSetAllSellsEditAbled(m_newLogic);
				else if(m_title == _T("����ģʽ"))
				{
					((CGridCtrl*)m_pCtrl)->SetVirtualMode(m_newLogic);
					((CGridCtrl*)m_pCtrl)->SetRowCount(100);
					((CGridCtrl*)m_pCtrl)->SetColumnCount(10);
					((CGridCtrl*)m_pCtrl)->SetFixedColumnCount(1);
					((CGridCtrl*)m_pCtrl)->SetFixedRowCount(1);
				}
				else if(m_title == _T("����"))
					((CGridCtrl*)m_pCtrl)->SetHeaderSort(m_newLogic);
				else if(m_title == _T("���"))
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
				else if(m_title == _T("��ֱ����"))
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
				else if(m_title == _T("ˮƽ����"))
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
				else if(m_title == _T("ѡ��"))
					((CGridCtrl*)m_pCtrl)->EnableSelection(m_newLogic);
				else if(m_title == _T("�̶���ѡ��"))
					((CGridCtrl*)m_pCtrl)->SetFixedRowSelection(m_newLogic);
				else if(m_title == _T("�̶���ѡ��"))
					((CGridCtrl*)m_pCtrl)->SetFixedColumnSelection(m_newLogic);
				else if(m_title == _T("�б�ģʽ"))
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
			if(m_title == _T("����"))
				((CTCButton*)m_pCtrl)->TCSetButtonMulity(m_oldLogic);
			else if(m_title == _T("Ĭ�ϰ�ť"))
				((CTCButton*)m_pCtrl)->TCSetDefButton(m_oldLogic);
			else if(m_title == _T("��Ч"))
				((CTCButton*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_EDIT:
			if(m_title == _T("��Ч"))
				((CTCEdit*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("����"))
				((CTCEdit*)m_pCtrl)->TCSetMulityLine(m_oldLogic);
			else if(m_title == _T("����"))
				((CTCEdit*)m_pCtrl)->TCSetPassWord(m_oldLogic);
			else if(m_title == _T("ֻ��"))
				((CTCEdit*)m_pCtrl)->TCSetReadOnly(m_oldLogic);
			else if(m_title == _T("����"))
				((CTCEdit*)m_pCtrl)->TCSetNumber(m_oldLogic);
			else if(m_title == _T("��ֱ������"))
				((CTCEdit*)m_pCtrl)->TCSetVertiScroll(m_oldLogic);
			else if(m_title == _T("ˮƽ������"))
				((CTCEdit*)m_pCtrl)->TCSetHorizScroll(m_oldLogic);
			break;
		case TCTT_RADIO:
			if(m_title.IsEmpty())
				((CTCRadio*)m_pCtrl)->TCSetCheck(m_oldLogic);
			else if(m_title == _T("��Ч"))
				((CTCRadio*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("�������"))
				((CTCCheck*)m_pCtrl)->TCSetTextLeft(m_oldLogic);
			else if(m_title == _T("����"))
				((CTCRadio*)m_pCtrl)->TCSetGroup(m_oldLogic);
			break;
		case TCTT_CHECK:
			if(m_title.IsEmpty())
				((CTCCheck*)m_pCtrl)->TCSetCheck(m_oldLogic);
			else if(m_title == _T("��Ч"))
				((CTCCheck*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("�������"))
				((CTCCheck*)m_pCtrl)->TCSetTextLeft(m_oldLogic);
			break;
		case TCTT_GROUP:
			break;
		case TCTT_LISTBOX:
			if(m_title == _T("��Ч"))
				((CTCListBox*)m_pCtrl)->TCSetAbled(m_oldLogic);
			else if(m_title == _T("Ĭ�Ϲ���"))
				((CTCListBox*)m_pCtrl)->TCSetDefaultRool(m_oldLogic);
			break;
		case TCTT_COMBOBOX:
			if(m_title == _T("��Ч"))
				((CTCComboBox*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_PROGRESS:
			if(m_title == _T("��ֱ"))
				((CTCProgress*)m_pCtrl)->TCSetVertical(m_oldLogic);
			break;
		case TCTT_PATH:
			if(m_title == _T("���ģʽ"))
			{
				if( m_oldLogic )
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_File);
				else
					((CTCBrowse*)m_pCtrl)->TCSetMode(CMFCEditBrowseCtrl::BrowseMode_Folder);
			}
			else if(m_title == _T("��Ч"))
				((CTCBrowse*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_SLIDER:
			((CTCSlider*)m_pCtrl)->TCSetVertical(m_oldLogic);
			break;
		case TCTT_PICTURE:		
			break;
		case TCTT_HOTKEY:	
			if(m_title == _T("��Ч"))
				((CTCHotKeyCtrl*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_DATE:
			if(m_title == _T("��Ч"))
				((CTCDate*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_TAB:
			if(m_title == _T("��Ч"))
				((CTCTab*)m_pCtrl)->TCSetAbled(m_oldLogic);
			break;
		case TCTT_GRIDCTRL:
			if(m_title == _T("�ɱ༭"))
				((CGridCtrl*)m_pCtrl)->TCSetAllSellsEditAbled(m_oldLogic);
			else if(m_title == _T("����ģʽ"))
			{
				((CGridCtrl*)m_pCtrl)->SetVirtualMode(m_oldLogic);
				for(int col = 0;col < ((CGridCtrl*)m_pCtrl)->GetColumnCount(); col ++)
				{
					CString title;
					title.Format(_T("�� %d ��"),col);
					((CGridCtrl*)m_pCtrl)->SetItemText(0,col,title);
				}
			}
			else if(m_title == _T("����"))
				((CGridCtrl*)m_pCtrl)->SetHeaderSort(m_oldLogic);
			else if(m_title == _T("���"))
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
			else if(m_title == _T("��ֱ����"))
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
			else if(m_title == _T("ˮƽ����"))
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
			else if(m_title == _T("ѡ��"))
				((CGridCtrl*)m_pCtrl)->EnableSelection(m_oldLogic);
			else if(m_title == _T("�̶���ѡ��"))
				((CGridCtrl*)m_pCtrl)->SetFixedRowSelection(m_oldLogic);
			else if(m_title == _T("�̶���ѡ��"))
				((CGridCtrl*)m_pCtrl)->SetFixedColumnSelection(m_oldLogic);
			else if(m_title == _T("�б�ģʽ"))
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


/////////////////////////�ؼ��������Ըı䶯��//////////////////////////
//**********�ؼ����������Ըı���ܹ������ָ�*************************//
//**********�����ָ���ʱ��Ҫ�ı����Կ��е�ֵ*******************//
////////////////////////�ؼ��������Ըı䶯��///////////////////////////
class CtrlProptyInfoCommand : public UNDO::Command
{
private:
	TCTOOLTYPE m_ctrlType;				//�ؼ�������
	
	CWnd* m_pCtrl;						//�ؼ���ָ��
	CString m_oldInfo;					//�ؼ��ɵ�����
	CString m_newInfo;					//�ؼ��µ�����
	CString m_id;						//�ؼ���ID,������ʾ����

	WORD m_wOldVirtualKeyCode;			//�����ȼ��ļ�ֵ
	WORD m_wOldModifiers;				//�����ȼ��ļ�ֵ

	WORD m_wNewVirtualKeyCode;			//�����ȼ��ļ�ֵ
	WORD m_wNewModifiers;				//�����ȼ��ļ�ֵ
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


/////////////////////////�ؼ�ճ������//////////////////////////
//**********����ѡȡ����ؼ��Ժ������Լ��϶�λ��
//*�ܹ���Redo �� Undo ��ʱ��ѭ���ƶ�
//*������һ��һ��ȥ�ƶ�
////////////////////////�ؼ�ճ������///////////////////////////
class CtrlPasteCommand : public UNDO::Command
{
private:
	//Data members
	int m_TCCtrlNum;				//�ؼ�������
	TCPARENTTYPE m_ParentWndtype;	//�����ڵ�����
	CWnd* m_pParent;

	//����Ҫ����Ŀؼ���Ϣ�Ľṹ��
	struct TCCTRLPASTE
	{
		CWnd* m_pCtrl;
		CRect m_rc;						//�ؼ��Ĵ�С

		TCTOOLTYPE m_CtrlType;			//�ؼ�����
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
	TCCTRLPASTE m_TCCtrlSort[MAXSELECTCTRLS];    //Ҫճ���Ŀؼ�����

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
		//ѭ���ƶ�

		if( m_ParentWndtype == TCPARENT_DESDIN )
		{
			((CDiaTCDesign*)m_pParent)->m_ControlTracker.DeSelectAll();
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				switch(m_TCCtrlSort[i].m_CtrlType)
				{
				case TCTT_STATIC:
					((CTCStatic*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ǩ"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent);
					((CDiaTCDesign*)m_pParent)->SetTCStatic((CTCStatic*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_BUTTON:
					((CTCButton*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ť"),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCButton((CTCButton*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_EDIT:
					((CTCEdit*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCEdit((CTCEdit*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_RADIO:
					((CTCRadio*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCRadio((CTCRadio*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_CHECK:
					((CTCCheck*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCCheck((CTCCheck*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_COMBOBOX:
					((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CDiaTCDesign*)m_pParent)->SetTCCombox((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->Create(NULL,_T("�����"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535,NULL);
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->SetCatptionTextColor(RGB(100,149,237));
					((CDiaTCDesign*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,_T(""),true);
					break;
				case TCTT_LISTBOX:
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("�б��"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->SetWindowTextW(_T("�б��"));
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
					((CTCPicture*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
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
					((CTCTimer*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("ʱ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|SS_REALSIZEIMAGE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
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
					//��ʼû�й̶���
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedColumnCount(0);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetRowCount(5);
					//��ʼ����һ���̶���
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedRowCount(1);
					for(int col = 0;col < ((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->GetColumnCount(); col++)
					{
						CString title;
						title.Format(_T("�� %d ��"),col);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemText(0,col,title);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);
					}
					//���ñ�����Ĭ����ʾ��ɫ
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
					((CTCStatic*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ǩ"), WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent);
					((CTCTabPage*)m_pParent)->SetTCStatic((CTCStatic*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_BUTTON:
					((CTCButton*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ť"),WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCButton((CTCButton*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_EDIT:
					((CTCEdit*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCEdit((CTCEdit*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_RADIO:
					((CTCRadio*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCRadio((CTCRadio*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_CHECK:
					((CTCCheck*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("��ѡ��"), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_CLIPSIBLINGS ,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCCheck((CTCCheck*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_COMBOBOX:
					((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl)->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCTabPage*)m_pParent)->SetTCCombox((CTCComboBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_GROUP:
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->Create(NULL,_T("�����"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535,NULL);
					((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl)->SetCatptionTextColor(RGB(100,149,237));
					((CTCTabPage*)m_pParent)->SetTCGroupBox((CTCGroupBox*)m_TCCtrlSort[i].m_pCtrl,m_TCCtrlSort[i].m_rc,false,_T(""),true);
					break;
				case TCTT_LISTBOX:
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("�б��"),WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
					((CTCListBox*)m_TCCtrlSort[i].m_pCtrl)->SetWindowTextW(_T("�б��"));
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
					((CTCPicture*)m_TCCtrlSort[i].m_pCtrl)->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|WS_BORDER|WS_CLIPSIBLINGS,m_TCCtrlSort[i].m_rc,m_pParent,65535);
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
					//��ʼû�й̶���
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedColumnCount(0);
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetRowCount(5);
					//��ʼ����һ���̶���
					((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetFixedRowCount(1);
					for(int col = 0;col < ((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->GetColumnCount(); col++)
					{
						CString title;
						title.Format(_T("�� %d ��"),col);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemText(0,col,title);
						((CGridCtrl*)m_TCCtrlSort[i].m_pCtrl)->SetItemData(0,col,TCGRIDCTRLTYPE::TCGRID_CTRL_NOMAL);
					}
					//���ñ�����Ĭ����ʾ��ɫ
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
			//���ƻ��� 
			for(int i=0;i<m_TCCtrlNum;i++)
			{
				switch(m_TCCtrlSort[i].m_CtrlType)
				{
					//�����´δ���ListBoxʱ����
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
			//��ʾ�������
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
					//�����´δ���ListBoxʱ����
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
			/////////��ʾTab���Ա���ؼ����Ա�¶//////////////
			CString tabID;
			int size = ((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll.GetSize();
			for(int k=0;k<size;k++)
			{
				if(((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll[k]->GetCWnd() == m_pParent->GetParent() )
					tabID = ((CDiaTCDesign*)m_pParent->GetParent()->GetParent())->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlID();
			}

			((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(tabID,(CTCTab*)m_pParent->GetParent());
			/////////��ʾTab���Ա���ؼ����Ա�¶//////////////

			((CTCTabPage*)m_pParent)->Invalidate(TRUE);
		}
		return TRUE;
	}
};