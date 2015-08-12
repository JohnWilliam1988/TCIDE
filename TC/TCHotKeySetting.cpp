// TCHotKeySetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "TCHotKeySetting.h"
#include "afxdialogex.h"

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"

#include "Docommand.h"


// CTCHotKeySetting �Ի���

IMPLEMENT_DYNAMIC(CTCHotKeySetting, CDialogEx)

CTCHotKeySetting::CTCHotKeySetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCHotKeySetting::IDD, pParent)
{
	m_ctrlID = _T("");
	m_wVirtualKeyCode=0;
	m_wModifiers=0;
	m_pHotKey = NULL;
}

CTCHotKeySetting::~CTCHotKeySetting()
{
}

void CTCHotKeySetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY1, m_hotKey);
}


BEGIN_MESSAGE_MAP(CTCHotKeySetting, CDialogEx)
	ON_BN_CLICKED(IDC_BTNHOTKEY_OK, &CTCHotKeySetting::OnBnClickedBtnhotkeyOk)
	ON_BN_CLICKED(IDCANCEL, &CTCHotKeySetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_HOTKEY_BTN, &CTCHotKeySetting::OnBnClickedHotkeyBtn)
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(ID_HOTKEY_ESC,ID_HOTKEY_ENTER,&CTCHotKeySetting::OnHotkeySys)
	ON_UPDATE_COMMAND_UI_RANGE(ID_HOTKEY_ESC,ID_HOTKEY_ENTER,&CTCHotKeySetting::OnUpdateHotkeySys)
	ON_COMMAND(ID_HOTKYE_SHIFT, &CTCHotKeySetting::OnHotkyeShift)
	ON_COMMAND(ID_HOTKEY_CTRL, &CTCHotKeySetting::OnHotkeyCtrl)
	ON_COMMAND(ID_HOTKEY_ALT, &CTCHotKeySetting::OnHotkeyAlt)
	ON_UPDATE_COMMAND_UI(ID_HOTKYE_SHIFT, &CTCHotKeySetting::OnUpdateHotkyeShift)
	ON_UPDATE_COMMAND_UI(ID_HOTKEY_CTRL, &CTCHotKeySetting::OnUpdateHotkeyCtrl)
	ON_UPDATE_COMMAND_UI(ID_HOTKEY_ALT, &CTCHotKeySetting::OnUpdateHotkeyAlt)
END_MESSAGE_MAP()


// CTCHotKeySetting ��Ϣ�������


BOOL CTCHotKeySetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
    if(!pCurrent)
    {
        return false;
    }

    CControlRect *pRect= pCurrent->m_DiaDesign.m_ControlTracker.GetRectByID(m_ctrlID);

    m_pHotKey=(CTCHotKeyCtrl *)pRect->GetCWnd();
	if( m_pHotKey != NULL)
	{
		m_pHotKey->GetHotKeyValue(m_wVirtualKeyCode,m_wModifiers);
		m_hotKey.SetHotKey(m_wVirtualKeyCode,m_wModifiers);
	}
	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTCHotKeySetting::OnBnClickedBtnhotkeyOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString curId = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_pCurrentWndID;
	CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(curId);
	if(!pCurrent)
	{
		return;
	}

	m_hotKey.GetHotKey(m_wVirtualKeyCode,m_wModifiers);
	if(m_pHotKey!=NULL)
	{
		CtrlProptyInfoCommand* pCommand = new CtrlProptyInfoCommand(TCTT_HOTKEY,m_pHotKey,m_ctrlID,_T(""),m_wVirtualKeyCode,m_wModifiers);
		pCurrent->m_DiaDesign.m_undoManager.DoCommand(pCommand);
		//((CTCApp*)AfxGetApp())->m_undoManager.DoCommand(pCommand);
		//m_pHotKey->SetHotKeyValue(m_wVirtualKeyCode,m_wModifiers);
		//m_pHotKey->SetHotKey(m_wVirtualKeyCode,m_wModifiers);
	}
	CDialog::EndDialog(IDC_BTNHOTKEY_OK);
}


void CTCHotKeySetting::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialogEx::OnCancel();
}


BOOL CTCHotKeySetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->hwnd ==  m_hotKey.GetSafeHwnd())
		{
			return CDialogEx::PreTranslateMessage(pMsg);
		}
		else
			return TRUE;
	} 
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTCHotKeySetting::OnBnClickedHotkeyBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�������һ�µ�ǰ�����ȼ��ļ�ֵ,ʹ����˵�������ʾһ�� 
	m_hotKey.GetHotKey(m_wVirtualKeyCode,m_wModifiers);
	CPoint point;
	CRect rect(0,0,0,0);
	GetDlgItem(IDC_HOTKEY_BTN)->GetWindowRect(rect);
	point.x = rect.right + 1;
	point.y = rect.bottom - 5;
	OnContextMenu(this, point);
}

void CTCHotKeySetting::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_HOTKEY_POPUP, point.x, point.y,this,TRUE);
#endif
}

void CTCHotKeySetting::OnHotkeySys(UINT id)
{
	switch(id)
	{		
	case ID_HOTKEY_ESC:
		m_wVirtualKeyCode = VK_ESCAPE;
		m_wModifiers = 0;
		m_hotKey.SetHotKey(VK_ESCAPE,0);
		break;
	case ID_HOTKEY_TAB:
		m_wVirtualKeyCode = VK_TAB;
		m_wModifiers = 0;
		m_hotKey.SetHotKey(VK_TAB,0);
		break;
	case ID_HOTKEY_CAP:
		m_wVirtualKeyCode = VK_CAPITAL;
		m_wModifiers = 0;
		m_hotKey.SetHotKey(VK_CAPITAL,0);
		break;
	case ID_HOTKEY_SPACE:
		m_wVirtualKeyCode = VK_SPACE;
		m_wModifiers = 0;
		m_hotKey.SetHotKey(VK_SPACE,0);
		break;
	case ID_HOTKEY_PAUSE:
		m_wVirtualKeyCode = VK_PAUSE;
		m_wModifiers = 0;
		m_hotKey.SetHotKey(VK_PAUSE,0);
		break;
	case ID_HOTKEY_INSERT:
		m_wVirtualKeyCode = VK_INSERT;
		m_wModifiers = 8;					//ֻ��Insert��Delete��ģֵ̬Ϊ8	????
		m_hotKey.SetHotKey(VK_INSERT,8);
		break;
	case  ID_HOTKEY_DELETE:
		m_wVirtualKeyCode = VK_DELETE;
		m_wModifiers = 8;
		m_hotKey.SetHotKey(VK_DELETE,8);
		break;
	case ID_HOTKEY_BACK:
		m_wVirtualKeyCode = VK_BACK;
		m_wModifiers = 0;
		m_hotKey.SetHotKey(VK_BACK,0);
		break;
	case ID_HOTKEY_ENTER:
		m_wVirtualKeyCode = VK_RETURN;
		m_wModifiers = 0;
		m_hotKey.SetHotKey(VK_RETURN,0);
		break;
	default:
		break;
	}	
}

void CTCHotKeySetting::OnUpdateHotkeySys(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CTCHotKeySetting::OnHotkyeShift()
{
	// TODO: �ڴ���������������
	if(m_wModifiers & HOTKEYF_SHIFT)
		m_wModifiers ^= HOTKEYF_SHIFT;
	else
		m_wModifiers |= HOTKEYF_SHIFT;
	m_hotKey.SetHotKey(m_wVirtualKeyCode,m_wModifiers);
}


void CTCHotKeySetting::OnHotkeyCtrl()
{
	// TODO: �ڴ���������������
	if(m_wModifiers & HOTKEYF_CONTROL)
		m_wModifiers ^= HOTKEYF_CONTROL;
	else	
		m_wModifiers |= HOTKEYF_CONTROL;
	m_hotKey.SetHotKey(m_wVirtualKeyCode,m_wModifiers);
}


void CTCHotKeySetting::OnHotkeyAlt()
{
	// TODO: �ڴ���������������
	if(m_wModifiers & HOTKEYF_ALT)
		m_wModifiers ^= HOTKEYF_ALT;
	else
		m_wModifiers |= HOTKEYF_ALT;
	m_hotKey.SetHotKey(m_wVirtualKeyCode,m_wModifiers);
}


void CTCHotKeySetting::OnUpdateHotkyeShift(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������	
	if(m_wModifiers & HOTKEYF_SHIFT)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}


void CTCHotKeySetting::OnUpdateHotkeyCtrl(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_wModifiers & HOTKEYF_CONTROL)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}


void CTCHotKeySetting::OnUpdateHotkeyAlt(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_wModifiers & HOTKEYF_ALT)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}
