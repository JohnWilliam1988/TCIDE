// DiaTCDesign.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mainfrm.h"
#include "TC.h"
#include "DiaTCDesign.h"
#include "afxdialogex.h"
#include "TCGridItems.h"
//
#include "TCCtlHeads.h"
#include "Docommand.h"
//
#include "TCLoadWinRCFile.h"

//LPOLESTR ControlNames[20] ={L"���ڽ���",L"��ǩ",L"�����",L"��ť",L"�༭��",L"��ѡ��",L"��ѡ��",L"������",L"�б��",
//	 					L"������",L"����",L"ͼƬ��",L"·��",L"ʱ��",L"�ȼ�",L"��ҳ��",L"\0"};

//CDiaTCDesign �Ի���
IMPLEMENT_DYNAMIC(CDiaTCDesign, CDialogEx)

	CDiaTCDesign::CDiaTCDesign(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaTCDesign::IDD, pParent)
{
	m_SelectToolType=TCTT_MAX;
	m_TcDlgConfig.Reset();
}

CDiaTCDesign::~CDiaTCDesign()
{

}

void CDiaTCDesign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDiaTCDesign, CDialogEx)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDOK, &CDiaTCDesign::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDiaTCDesign::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	//ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CDiaTCDesign ��Ϣ�������

LRESULT CDiaTCDesign::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CDialogEx::OnNcHitTest(point);
	UINT nHitTest = CDialogEx::OnNcHitTest(point);
	//������б�����,���Ͻǣ���߿��򷵻�
	if(nHitTest == HTCAPTION || nHitTest == HTLEFT || nHitTest == HTTOPLEFT||nHitTest ==HTTOP|| nHitTest ==HTBOTTOMLEFT
		||nHitTest ==HTTOPRIGHT)
	{
		return 0;
	}
	else 
	{
		return CDialogEx::OnNcHitTest(point);
	}
}

void CDiaTCDesign::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
}

void CDiaTCDesign::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
}

void CDiaTCDesign::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	m_ControlTracker.Draw(&dc);
}

int CDiaTCDesign::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_ControlTracker.Create(this);
	m_ControlTracker.SetParentWndType(TCPARENT_DESDIN);
	return 0;
}

BOOL CDiaTCDesign::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_ControlTracker.SetCursor(nHitTest, message ))
		return TRUE;

	if(nHitTest == HTCLIENT) 
	{
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

		TCTOOLTYPE type=pFrame->GetSelectToolType();

		if(type==TCTOOLTYPE::TCTT_STATIC)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSTATIC))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_EDIT)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CUREDIT))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_CHECK)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURCHECK))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_COMBOBOX)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURCOMBOBOX))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_RADIO)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURRADIO))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_PROGRESS)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURPROGRESS))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_GROUP)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURGROUPBOX))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_LISTBOX)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURLISTBOX))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_DATE)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURDATE))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_PICTURE)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURPICTURE))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_BUTTON)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURBUTTON))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_SLIDER)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSLIDER))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_PATH)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURPATH))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_TAB)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURTAB))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_TIMER)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURTIMER))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_HOTKEY)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURHOTKEY))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_EXPLORER)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURWEB))))
				return true;
		}
		else if(type==TCTOOLTYPE::TCTT_GRIDCTRL)
		{
			if(::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURGRID))))
				return true;
		}
		
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}



void CDiaTCDesign::AddCtrl(CRect rc,TCTOOLTYPE type,TCPARENTTYPE parentType)
{
	AddCtrlCommand* pCommand = new AddCtrlCommand(this,rc,type,parentType);
	m_undoManager.DoCommand(pCommand);
}

void CDiaTCDesign::OnSizeCtrl(CWnd* pCtrl,CRect rc,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType)
{
	OnSizeCtrlCommand *pCommand = new OnSizeCtrlCommand(this,pCtrl,rc,ctrlType,parentWndType);
	m_undoManager.DoCommand(pCommand);
}

void CDiaTCDesign::ClearCtrl(CWnd* pCtrl,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType)
{
	ClearCtrlCommand *pCommand = new ClearCtrlCommand(this,pCtrl,ctrlType,parentWndType);
	m_undoManager.DoCommand(pCommand);
}

void CDiaTCDesign::AddTabCtrl(CRect rc)
{
	AddTabCommand *pCommand = new AddTabCommand(this,rc);
	m_undoManager.DoCommand(pCommand);
}

void CDiaTCDesign::ClearTabCtrl(CTCTab *pCtrl,CRect rc)
{
	ClearTabCommand *pCommand = new ClearTabCommand(this,pCtrl,rc);
	for(int i=0;i<pCtrl->getTabPageCount();i++)
	{
		TCITEM item;
		wchar_t buffer[256] = {0};	
		item.pszText = buffer;
		item.cchTextMax = 100;
		item.mask = TCIF_TEXT;
		pCtrl->GetItem(i, &item);
		CString tabStr = buffer;
		pCommand->m_pageTitle.Add(tabStr);
		//pCommand->m_tcTabPage[i].m_pTabPage = pCtrl->m_Pages[i];
		//pCommand->m_pTabPage[i] = pCtrl->m_Pages[i];
	}
	m_undoManager.DoCommand(pCommand);
}

void CDiaTCDesign::SetTCButton(CTCButton* pButton,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pButton->SetFont(pFont);

	CString id = ctrlid.IsEmpty() ? m_ControlTracker.BuildValidCtlID(TCTT_BUTTON) : ctrlid;
	pButton->TCSetID(id); //��̬�ؼ�û���¼�,����û��Ҫ��¼
	CControlRect *rectButton =new CControlRect(rc,pButton,TCTT_BUTTON,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectButton);
	isCopy? m_ControlTracker.Select(rectButton) : m_ControlTracker.SetCreateCtrlSelected(rectButton);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(id,pButton);
}


void CDiaTCDesign::SetTCStatic(CTCStatic* pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty() ? m_ControlTracker.BuildValidCtlID(TCTT_STATIC) : ctrlid;
	
	//pStatic->TCInitail(id); //��̬�ؼ�û���¼�,����û��Ҫ��¼
	CControlRect *rectStatic =new CControlRect(rc,pCtrl,TCTT_STATIC,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectStatic);
	isCopy ? m_ControlTracker.Select(rectStatic) : m_ControlTracker.SetCreateCtrlSelected(rectStatic);
	if(!isCopy)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCEdit(CTCEdit *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	//pCtrl->SetBackColor(RGB(255,255,255));
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id = ctrlid.IsEmpty() ? m_ControlTracker.BuildValidCtlID(TCTT_EDIT) : ctrlid;
	
	CControlRect *rectEdit =new CControlRect(rc,pCtrl,TCTT_EDIT,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectEdit);
	isCopy ? m_ControlTracker.Select(rectEdit) : m_ControlTracker.SetCreateCtrlSelected(rectEdit);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCRadio(CTCRadio *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle());
	pCtrl->SetFont(pFont);

	CString id = ctrlid.IsEmpty() ? m_ControlTracker.BuildValidCtlID(TCTT_RADIO) : ctrlid;	
	CControlRect *rectRadio =new CControlRect(rc,pCtrl,TCTT_RADIO,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectRadio);

	isCopy ? m_ControlTracker.Select(rectRadio) : m_ControlTracker.SetCreateCtrlSelected(rectRadio);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCCheck(CTCCheck *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);

	CString id = ctrlid.IsEmpty() ? m_ControlTracker.BuildValidCtlID(TCTT_CHECK) : ctrlid;
	
	CControlRect *rectCheck =new CControlRect(rc,pCtrl,TCTT_CHECK,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectCheck);
	isCopy ? m_ControlTracker.Select(rectCheck) : m_ControlTracker.SetCreateCtrlSelected(rectCheck);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCCombox(CTCComboBox *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty() ? m_ControlTracker.BuildValidCtlID(TCTT_COMBOBOX) : ctrlid;
	
	CControlRect *rectCombox =new CControlRect(rc,pCtrl,TCTT_COMBOBOX,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectCombox);
	isCopy? m_ControlTracker.Select(rectCombox) : m_ControlTracker.SetCreateCtrlSelected(rectCombox);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCProgress(CTCProgress *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_PROGRESS) : ctrlid;
	CControlRect *rectProgress =new CControlRect(rc,pCtrl,TCTT_PROGRESS,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectProgress);
	isCopy? m_ControlTracker.Select(rectProgress) : m_ControlTracker.SetCreateCtrlSelected(rectProgress);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCPath(CTCBrowse *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_PATH) : ctrlid;
	
	CControlRect *rectEdit =new CControlRect(rc,pCtrl,TCTT_PATH,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectEdit);
	isCopy ? m_ControlTracker.Select(rectEdit) : m_ControlTracker.SetCreateCtrlSelected(rectEdit);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCGroupBox(CTCGroupBox *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {pFont = CFont::FromHandle(hFont);}
	}
	LOGFONT lg;
	pFont->GetLogFont(&lg);
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(lg);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_GROUP) : ctrlid;
	CControlRect *rectGroup =new CControlRect(rc,pCtrl,TCTT_GROUP,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectGroup);
	isCopy?  m_ControlTracker.Select(rectGroup) : m_ControlTracker.SetCreateCtrlSelected(rectGroup);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCListBox(CTCListBox *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_LISTBOX) : ctrlid;
	CControlRect *rectList =new CControlRect(rc,pCtrl,TCTT_LISTBOX,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectList);
	isCopy ? m_ControlTracker.Select(rectList) : m_ControlTracker.SetCreateCtrlSelected(rectList);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCPicture(CTCPicture *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_PICTURE) : ctrlid;
	CControlRect *rectPicture =new CControlRect(rc,pCtrl,TCTT_PICTURE,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectPicture);
	isCopy ? m_ControlTracker.Select(rectPicture) : m_ControlTracker.SetCreateCtrlSelected(rectPicture);
	if(!isCopy)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCSlider(CTCSlider *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_SLIDER) : ctrlid;
	CControlRect *rectSlider =new CControlRect(rc,pCtrl,TCTT_SLIDER,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectSlider);
	isCopy? m_ControlTracker.Select(rectSlider) : m_ControlTracker.SetCreateCtrlSelected(rectSlider);
	if(!isCopy)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCTimer(CTCTimer *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_TIMER) : ctrlid;
	CControlRect *rectTimer =new CControlRect(rc,pCtrl,TCTT_TIMER,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectTimer);
	isCopy? m_ControlTracker.Select(rectTimer) : m_ControlTracker.SetCreateCtrlSelected(rectTimer);
	if(!isCopy)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCHotKey(CTCHotKeyCtrl *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_HOTKEY) : ctrlid;
	CControlRect *rectHotKey =new CControlRect(rc,pCtrl,TCTT_HOTKEY,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectHotKey);
	isCopy ? m_ControlTracker.Select(rectHotKey) : m_ControlTracker.SetCreateCtrlSelected(rectHotKey);
	if(!isCopy)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCDate(CTCDate *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()?  m_ControlTracker.BuildValidCtlID(TCTT_DATE) : ctrlid;
	CControlRect *rectDate =new CControlRect(rc,pCtrl,TCTT_DATE,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectDate);
	isCopy? m_ControlTracker.Select(rectDate) : m_ControlTracker.SetCreateCtrlSelected(rectDate);
	if(!isCopy)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCTab(CTCTab *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()? m_ControlTracker.BuildValidCtlID(TCTT_TAB) : ctrlid;
	CControlRect* rectTab =new CControlRect(rc,pCtrl,TCTT_TAB,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectTab);
	isCopy? m_ControlTracker.Select(rectTab) : m_ControlTracker.SetCreateCtrlSelected(rectTab);
	if(!isCopy)
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(id,pCtrl);
	pCtrl->showIndex(0);
}

void CDiaTCDesign::SetTCExplorer(CTCExplorer *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()?  m_ControlTracker.BuildValidCtlID(TCTT_EXPLORER) : ctrlid;
	CControlRect *rectExplorer =new CControlRect(rc,pCtrl,TCTT_EXPLORER,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectExplorer);
	isCopy? m_ControlTracker.Select(rectExplorer) : m_ControlTracker.SetCreateCtrlSelected(rectExplorer);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinExplorerProperty(id,pCtrl);
}

void CDiaTCDesign::SetTCGridCtrl(CGridCtrl *pCtrl,CRect rc,CString ctrlid,bool isCopy)
{
	CFont* pFont = GetFont();
	if (!pFont) {
		HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
		if (NULL == hFont) {
			hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
		}
		if (hFont) {
			pFont = CFont::FromHandle (hFont);
		}
	}
	ASSERT (pFont->GetSafeHandle ());
	pCtrl->SetFont(pFont);
	CString id = ctrlid.IsEmpty()?  m_ControlTracker.BuildValidCtlID(TCTT_GRIDCTRL) : ctrlid;
	CControlRect *rectGridCtrl =new CControlRect(rc,pCtrl,TCTT_GRIDCTRL,id,TCPARENT_DESDIN,this);
	m_ControlTracker.Add(rectGridCtrl);
	isCopy? m_ControlTracker.Select(rectGridCtrl) : m_ControlTracker.SetCreateCtrlSelected(rectGridCtrl);
	if(!isCopy)
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGridCtrlProperty(id,pCtrl);
}

void CDiaTCDesign::DeSelectAll()
{
	//9-17
	//�����Ҫ������Tab�ؼ������ѡ���ÿ�
	//////////////////////////////////////////�����ʼ��Table�е�ѡ��״̬�����л�//////////////////////////////////
	for(int i=0;i<m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
	{
		if(m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab* pTab = (CTCTab*)m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
			for(int j=0;j<pTab->getTabPageCount();j++)
			{
				int pageIndex=j;
				int pageSize = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
				for(int k=0;k<pageSize;k++)
				{
					((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelectAll();
				}
			}
		}
	}
	m_ControlTracker.DeSelectAll();
	//////////////////////////////////////////�����ʼ��Table�е�ѡ��״̬�����л�//////////////////////////////////
}

//�����е�Ĭ�ϰ�ť��ΪFalse
void CDiaTCDesign::SetAllDefButtonFalse()
{
	for(int i=0;i<m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
	{
		if(m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType() == TCTT_TAB)
		{
			CTCTab* pTab = (CTCTab*)m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
			for(int j=0;j<pTab->getTabPageCount();j++)
			{
				int pageSize = ((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll.GetSize();
				for(int k=0;k<pageSize;k++)
				{
					if(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll[k]->GetCtlType() == TCTT_BUTTON)
					{
						CTCButton *pButton = (CTCButton*)(((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll[k]->GetCWnd());
						pButton->TCSetDefButton(false);
					}
				}
			}
		}
		else if(m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType() == TCTT_BUTTON)
		{
			CTCButton *pButton = (CTCButton*)(m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd());
			pButton->TCSetDefButton(false);
		}
	}
	//////////////////////////////////////////�����ʼ��Table�е�ѡ��״̬�����л�//////////////////////////////////
}

void CDiaTCDesign::Undo()
{
	m_undoManager.Undo();
	this->SetFocus();
}

void CDiaTCDesign::Redo()
{
	m_undoManager.Redo();
	this->SetFocus();
}

BOOL CDiaTCDesign::CanUndo()
{
	return m_undoManager.CanUndo();
}

BOOL CDiaTCDesign::CanRedo()
{
	return m_undoManager.CanRedo();
}


BOOL CDiaTCDesign::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	TCTOOLTYPE type=pFrame->GetSelectToolType();

	POINT point;
	pMsg->pt;
	point.x=pMsg->pt.x;
	point.y=pMsg->pt.y;
	this->ScreenToClient(&point);

	ClipCursor(NULL);
	if(pMsg->message==WM_CHAR)
	{
		//this->SetFocus();
		return TRUE;
	}
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		CMDIChildWnd* pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
		CTCDesignView* pView = (CTCDesignView *)pChild->GetActiveView();
		pView->SetFocus();

		//CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTWin;
		//POSITION pos=pDocTmp->GetFirstDocPosition();
		//CTCDesignView *pView=NULL;
		//CTCDesignDoc* pDoc = NULL;
		//while (NULL != pos)
		//{
		//	pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
		//	if(pDoc==NULL)
		//		continue;
		//	POSITION posView = pDoc->GetFirstViewPosition();
		//	while (NULL != posView)
		//	{
		//		pView = (CTCDesignView *)pDoc->GetNextView(posView);
		//		if (NULL == pView )
		//			continue;
		//		//if(CTCRunTimeManager::isSameFile(CTCRunTimeManager::g_ProjName,pView->m_TWinID))
		//		//�������ĵ�ǰ�����ǵ�ǰ��ͼ
		//		if(m_TcDlgConfig.m_dlgID==pView->m_TWinID)
		//		{
		//			CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
		//			pFWnd->MDIActivate();	
		//			pView->SetFocus();				
		//		}
		//	}
		//}
		
		if(type==TCTOOLTYPE::TCTT_MAX)
		{
			CWnd * hwnd;
			POINT pointGetCtl;
			GetCursorPos(&pointGetCtl);
			hwnd= (CWnd *)WindowFromPoint(pointGetCtl);


			//////////////////////////////////////////�����ʼ��Table�е�ѡ��״̬�����л�//////////////////////////////////
			for(int i=0;i<m_ControlTracker.m_arrRectanglesAll.GetSize();i++)
			{
				if(m_ControlTracker.m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
				{
					CTCTab* pTab = (CTCTab*)m_ControlTracker.m_arrRectanglesAll[i]->GetCWnd();
					for(int j=0;j<pTab->getTabPageCount();j++)
					{
						int pageIndex=j;
						int pageSize=((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize();
						for(int k=0;k<pageSize/*k<((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesSelected.GetSize()*/;k++)
						{
							((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.DeSelectAll();
						}
					}
				}
			}
			//////////////////////////////////////////�����ʼ��Table�е�ѡ��״̬�����л�//////////////////////////////////



			//////����ǵ���ĶԻ���,���öԻ�������
			//if(hwnd==this && m_ControlTracker.m_arrRectanglesSelected.GetSize()==0)
			//	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(m_TcDlgConfig.m_dlgID,this,TRUE);

			while (hwnd && hwnd->GetParent()!=this)
				hwnd = hwnd->GetParent();
			int index=m_ControlTracker.IsIncludeControl(hwnd);
			if(-1!=index)
			{
				//�����ǵõ������ƿͻ�����ķ�Χ
				CRect DesignRect;
				this->GetClientRect(&DesignRect);
				this->ClientToScreen(&DesignRect);

				//�����ǵõ�ѡ�пռ�ķ�Χ
				CRect CtlRect;
				POINT pointCur;

				//�ж��ǲ���ѡ���˶���ؼ�,����������ڵĿؼ�������ѡ�еĿؼ�
				if(m_ControlTracker.m_arrRectanglesSelected.GetSize()&&(-1!=m_ControlTracker.IsInSelectControl(hwnd)))
				{
					CtlRect=m_ControlTracker.GetBoundsRect();
					POINT pointHelp;
					GetCursorPos(&pointHelp);
					this->ScreenToClient(&pointHelp);
					m_ControlTracker.GetBoundsPointCur(pointHelp,&pointCur);
				}
				else
				{
					hwnd->GetWindowRect(&CtlRect);
					ScreenToClient(&CtlRect);
					//�����ǵõ�����ڿؼ��ϵ�λ��
					GetCursorPos(&pointCur);
					hwnd->ScreenToClient(&pointCur);
				}
				//�����������϶��ķ�Χ
				CRect ClipRect;
				ClipRect.left=DesignRect.left+pointCur.x;
				ClipRect.top=DesignRect.top+pointCur.y;
				ClipRect.right=DesignRect.right-(CtlRect.Width()-pointCur.x);
				ClipRect.bottom=DesignRect.bottom-(CtlRect.Height()-pointCur.y);

				bool hitSide = false;
				if (m_ControlTracker.m_arrRectanglesSelected.GetCount() > 0)
				{
					int hitPos = m_ControlTracker.m_arrRectanglesSelected[0]->HitTest(point);
					if (hitPos != CRectTracker::hitNothing && hitPos != CRectTracker::hitMiddle)
						hitSide = true;
				}

				//���û�õ��б�Ե��
                //�����������
				if (hitSide == false)
				ClipCursor(&ClipRect);

				////////////////////��ʾѡ�пؼ�������//////////////////////
				//int showIndex=m_ControlTracker.IsIncludeControl(hwnd);
				//if(showIndex!=-1)
				//{
				//	TCTOOLTYPE selectType;
				//	CWnd *     selectHwnd;
				//	CString    selectID;
				//	selectType=m_ControlTracker.m_arrRectanglesAll[showIndex]->GetCtlType();
				//	selectHwnd=m_ControlTracker.m_arrRectanglesAll[showIndex]->GetCWnd();
				//	selectID=m_ControlTracker.m_arrRectanglesAll[showIndex]->GetCtlID();
				//	//TCTOOLTYPE SelectType;
				//	if(selectType==TCTT_STATIC)
				//	{
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinStaticProperty(selectID,(CTCStatic *)selectHwnd);
				//	}
				//	else if( selectType == TCTT_BUTTON )
				//	{
				//		CTCButton *ctrlButton = (CTCButton*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinButtonProperty(selectID,ctrlButton);
				//	}
				//	else if( selectType == TCTT_GROUP )
				//	{
				//		CTCGroupBox *ctrlGroup = (CTCGroupBox*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinGroupProperty(selectID,ctrlGroup);
				//	}
				//	else if( selectType == TCTT_EDIT )
				//	{
				//		CTCEdit *ctrlEdit = (CTCEdit*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinEditProperty(selectID,ctrlEdit);
				//	}
				//	else if( selectType == TCTT_RADIO )
				//	{
				//		CTCRadio *ctrlRadio = (CTCRadio*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinRadioProperty(selectID,ctrlRadio);
				//	}
				//	else if( selectType == TCTT_CHECK )
				//	{
				//		CTCCheck *ctrlCheck = (CTCCheck*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinCheckProperty(selectID,ctrlCheck);
				//	}
				//	else if( selectType == TCTT_COMBOBOX )
				//	{
				//		CTCComboBox *ctrlCombox = (CTCComboBox*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinComboxProperty(selectID,ctrlCombox);
				//	}
				//	else if( selectType == TCTT_PICTURE )
				//	{
				//		CTCPicture *ctrlPic = (CTCPicture*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPictureProperty(selectID,ctrlPic);
				//	}
				//	else if( selectType ==TCTT_PROGRESS )
				//	{
				//		CTCProgress *ctrlProgress = (CTCProgress*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinProgressProperty(selectID,ctrlProgress);
				//	}
				//	else if( selectType == TCTT_PATH )
				//	{
				//		CTCBrowse *ctrlPath = (CTCBrowse*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinPathProperty(selectID,ctrlPath);
				//	}
				//	else if( selectType == TCTT_DATE )
				//	{
				//		CTCDate *ctrlDate = (CTCDate*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDateProperty(selectID,ctrlDate);
				//	}
				//	else if( selectType == TCTT_SLIDER )
				//	{
				//		CTCSlider *ctrlSlider = (CTCSlider*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinSliderProperty(selectID,ctrlSlider);
				//	}
				//	else if( selectType == TCTT_LISTBOX )
				//	{
				//		CTCListBox *ctrlList = (CTCListBox*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinListProperty(selectID,ctrlList);
				//	}
				//	else if( selectType == TCTT_TAB)
				//	{
				//		CTCTab *ctrlTab = (CTCTab*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTableProperty(selectID,ctrlTab);
				//	}
				//	else if( selectType == TCTT_TIMER )
				//	{
				//		CTCTimer *ctrlTimer = (CTCTimer*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinTimerProperty(selectID,ctrlTimer);
				//	}
				//	else if( selectType == TCTT_HOTKEY )
				//	{
				//		CTCHotKeyCtrl *ctrlHotKey = (CTCHotKeyCtrl*)selectHwnd;
				//		((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinHotKeyProperty(selectID,ctrlHotKey);
				//	}
				//}
			}
			m_ControlTracker.Track( point ,pMsg->wParam,true);
			////����ǵ���ĶԻ���,���öԻ�������
			if(m_ControlTracker.m_arrRectanglesSelected.GetSize() == 0)
				((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTWinDiaProperty(m_TcDlgConfig.m_dlgID,this,TRUE);
		}

		/////////////////////////////////��δ�������������,TAB�ؼ������ʱ���ҳ���л�///////////////////
		/////////////////////////////////��δ�������������,TAB�ؼ������ʱ���ҳ���л�///////////////////
		//�жϵ�ǰ���е��Ƿ�Ϊtab�ؼ�
		//�жϵ�ǰ���е��Ƿ�Ϊtab�ؼ���ͷ��
		CWnd * hwnd;
		POINT pointGetCtl;
		GetCursorPos(&pointGetCtl);
		hwnd= (CWnd *)WindowFromPoint(pointGetCtl);
		int index=m_ControlTracker.IsIncludeControl(hwnd);
		if(index!=-1&&index<m_ControlTracker.m_arrRectanglesAll.GetSize())
		{
			TCTOOLTYPE selectType;
			CWnd *     selectHwnd;
			CString    selectID;
			selectType=m_ControlTracker.m_arrRectanglesAll[index]->GetCtlType();
			selectHwnd=m_ControlTracker.m_arrRectanglesAll[index]->GetCWnd();
			selectID=m_ControlTracker.m_arrRectanglesAll[index]->GetCtlID();
			if (hwnd==selectHwnd&&selectType == TCTT_TAB &&
				m_ControlTracker.m_arrRectanglesAll[index]->HitTest(point) == CRectTracker::hitMiddle)
			{
				return FALSE;
			}

		}
		/////////////////////////////////��δ�������������,TAB�ؼ������ʱ���ҳ���л�///////////////////
		/////////////////////////////////��δ�������������,TAB�ؼ������ʱ���ҳ���л�///////////////////

		////////////////////2-23�Ž�������嵯���˵�����ʧ������//////////////////
		CMFCPopupMenu *pPopupMenu = CMFCPopupMenu::GetActiveMenu();
		if(pPopupMenu!=NULL)
			pPopupMenu->SendMessage(WM_CLOSE,NULL,NULL);
		return TRUE;
	}
	if(pMsg->message==WM_LBUTTONUP)
	{
		if(type!=TCTOOLTYPE::TCTT_MAX)
		{    
			//������ѡ��״̬�ÿ�
			//DeSelectAll();
			POINT point;
			GetCursorPos(&point);
			this->ScreenToClient(&point);
			///////////////////////��ǩ�ؼ�///////////////
			if(type==TCTT_STATIC)
				AddCtrl(CRect(point.x,point.y,point.x+30,point.y+15),TCTT_STATIC,TCPARENT_DESDIN);
			///////////////////////��ť�ؼ�///////////////
			else if(type==TCTT_BUTTON)
				AddCtrl(CRect(point.x,point.y,point.x+65,point.y+28),TCTT_BUTTON,TCPARENT_DESDIN);
			///////////////////////�༭��ؼ�///////////////
			else if(type==TCTT_EDIT)
				AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_EDIT,TCPARENT_DESDIN);
			///////////////////////��ѡ��ؼ�///////////////
			else if(type==TCTT_RADIO)
				AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_RADIO,TCPARENT_DESDIN);
			///////////////////////��ѡ��ؼ�///////////////
			else if(type==TCTT_CHECK)
				AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_CHECK,TCPARENT_DESDIN);
				///////////////////////�����///////////////////
			else if(type==TCTT_GROUP)
				AddCtrl(CRect(point.x,point.y,point.x+100,point.y+100),TCTT_GROUP,TCPARENT_DESDIN);
			///////////////////////ͼƬ��///////////////////
			else if(type==TCTT_PICTURE)
				AddCtrl(CRect(point.x,point.y,point.x+100,point.y+70),TCTT_PICTURE,TCPARENT_DESDIN);
			///////////////////////������///////////////////
			else if(type==TCTT_COMBOBOX)
				AddCtrl(CRect(point.x,point.y,point.x+100,point.y+20),TCTT_COMBOBOX,TCPARENT_DESDIN);
			///////////////////////�������ؼ�///////////////
			else if(type==TCTT_PROGRESS)
				AddCtrl(CRect(point.x,point.y,point.x+120,point.y+20),TCTT_PROGRESS,TCPARENT_DESDIN);
			////////////////////////�����ؼ�///////////////
			else if(type==TCTT_PATH)
				AddCtrl(CRect(point.x,point.y,point.x+120,point.y+25),TCTT_PATH,TCPARENT_DESDIN);
			////////////////////////�������ؼ�///////////////
			else if(type==TCTT_SLIDER)
				AddCtrl(CRect(point.x,point.y,point.x+100,point.y+22),TCTT_SLIDER,TCPARENT_DESDIN);
			////////////////////////���ڿ�ؼ�///////////////
			else if(type==TCTT_DATE)
				AddCtrl(CRect(point.x,point.y,point.x+95,point.y+22),TCTT_DATE,TCPARENT_DESDIN);
			////////////////////////�б��ؼ�///////////////
			else if(type==TCTT_LISTBOX)
				AddCtrl(CRect(point.x,point.y,point.x+95,point.y+50),TCTT_LISTBOX,TCPARENT_DESDIN);
			////////////////////////Table�ؼ�///////////////
			else if(type==TCTT_TAB)
				AddTabCtrl(CRect(point.x,point.y,point.x+200,point.y+180));
			////////////////////////ʱ�ӿؼ�////////////////////////
			else if(type==TCTT_TIMER)
				AddCtrl(CRect(point.x,point.y,point.x+20,point.y+20),TCTT_TIMER,TCPARENT_DESDIN);
			///////////////////////�ȼ��ؼ�
			else if(type==TCTT_HOTKEY)
				AddCtrl(CRect(point.x,point.y,point.x+60,point.y+20),TCTT_HOTKEY,TCPARENT_DESDIN);
			///////////////////////������ؼ�
			else if(type==TCTT_EXPLORER)
				AddCtrl(CRect(point.x,point.y,point.x+200,point.y+100),TCTT_EXPLORER,TCPARENT_DESDIN);
			///////////////////////����ؼ�////////////////////////
			else if(type==TCTT_GRIDCTRL)
				AddCtrl(CRect(point.x,point.y,point.x+200,point.y+100),TCTT_GRIDCTRL,TCPARENT_DESDIN);
		}
		pFrame->ResetSelectToolType();

		return TRUE;
	}
	//��ֻ֮Ҫ��Ϣ�������ִ��Undo����
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'Z' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
	{
		Undo();
		//Invalidate(TRUE);
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'Y' && (VK_LEFT && ::GetKeyState(VK_CONTROL)<0))
	{
		Redo();
		//Invalidate(TRUE);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDiaTCDesign::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::PreCreateWindow(cs);
}
static CString ColorToString(COLORREF bkColor)
{
	CString color;
	color.Format(_T("%u"),(DWORD)bkColor);
	return color;
}

void CDiaTCDesign::SaveTWinRCFileToMem()
{
	//���ע��ɾ����ǰ���ڹ���ڵ��е�һ���ڵ��
	int index = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getIndexByID(m_TcDlgConfig.m_dlgID);
	if( -1 == index )
		return;
	wndNode * node = ((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.getat(index);

	CString winInfo;
	//��������
	CString csCaption;
	this->GetWindowTextW(csCaption);
	//ת���ַ�
	csCaption.Replace(_T("\\"),_T("\\\\"));
	csCaption.Replace(_T("\""),_T("\\\""));
	CString title;
	title.Format(_T("%s%s%s"),_T("\""),csCaption,_T("\""));
	RECT rc;
	//this->GetWindowRect(&rc);
	this->GetClientRect(&rc);
	//this->ClientToScreen(&rc);

	//���洰��ID������
	CString csWin;
	csWin.Format(_T("%s %s %s %d,%d,%d,%d "),_T("����"),m_TcDlgConfig.m_dlgID,title,rc.left,rc.top,rc.right,rc.bottom);

	//��������
	CString csWinProp;
	if(m_TcDlgConfig.m_BkgrColor!=::GetSysColor(COLOR_BTNFACE))
	{
		csWinProp+=_T("������ɫ:")+ColorToString(m_TcDlgConfig.m_BkgrColor)+_T("|");
	}
	if(!m_TcDlgConfig.m_BkgrBitmap.IsEmpty())
	{
		csWinProp+=_T("����ͼƬ:")+m_TcDlgConfig.m_BkgrBitmap+_T("|");
	}
	if(m_TcDlgConfig.m_maxBtn)
	{
		csWinProp=csWinProp+_T("���:")+_T("1")+_T("|");
	}
	if(!m_TcDlgConfig.m_minBtn)
	{
		csWinProp=csWinProp+_T("��С��:")+_T("0")+_T("|");
	}
	if(csWinProp.IsEmpty())
		csWin+=_T("|");
	else
		csWin+=csWinProp;

	//�����¼�
	CString csWinEvent;
	if(!m_TcDlgConfig.m_eDiaInit.IsEmpty())
	{
		csWinEvent=csWinEvent+_T("���ڳ�ʼ��:")+m_TcDlgConfig.m_eDiaInit+_T("|");
	}
	if(!m_TcDlgConfig.m_eDestory.IsEmpty())
	{
		csWinEvent=csWinEvent+_T("��������:")+m_TcDlgConfig.m_eDestory+_T("|");
	}
	if(!m_TcDlgConfig.m_eClose.IsEmpty())
	{
		csWinEvent=csWinEvent+_T("���ڹر�:")+m_TcDlgConfig.m_eClose+_T("|");
	}
	if(!m_TcDlgConfig.m_ePreTrans.IsEmpty())
	{
		csWinEvent=csWinEvent+_T("��Ϣ·��:")+m_TcDlgConfig.m_ePreTrans+_T("|");
	}
	if(!m_TcDlgConfig.m_eWinProc.IsEmpty())
	{
		csWinEvent=csWinEvent+_T("��Ϣ����:")+m_TcDlgConfig.m_eWinProc+_T("|");
	}

	if(csWinEvent.IsEmpty())
		csWin+=_T(" |");
	else
		csWin=csWin+_T(" ")+csWinEvent;

	winInfo+=csWin;
	//cFile.Write(csWin,csWin.GetLength()*sizeof(TCHAR)); //д�������Ϣ	
	int count=m_ControlTracker.m_arrRectanglesAll.GetSize();;   //����пؼ�������
	
	//����Radio����GroupΪTrue
	//bool isFirstRadio = true;
	for(int i=0;i<count;i++)
	{
		CControlRect* ctl=m_ControlTracker.m_arrRectanglesAll.GetAt(i);
		TCTOOLTYPE type=ctl->GetCtlType();
		winInfo+=_T("\n");
		//cFile.Write(_T("\r\n"),2); //����
		if(type==TCTT_STATIC)
		{
			CTCStatic* pStatic=(CTCStatic *)ctl->GetCWnd();
			//��������
			CString text;
			pStatic->GetWindowTextW(text);
			///ת���ַ�
			text.Replace(_T("\\"),_T("\\\\"));
			text.Replace(_T("\""),_T("\\\""));
			CString title;
			title.Format(_T("\"%s\""),text);
			RECT rc;
			pStatic->GetWindowRect(&rc);
			this->ScreenToClient(&rc);
			CString csStatic;
			csStatic.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ǩ"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

			//����
			//����
			CString staticProp;
			CString szFont=pStatic->GetFontName();
			DWORD dwColor=pStatic->GetTextColor();
			DWORD dwBgColor=pStatic->GetBackground();
			int nSize=pStatic->GetFontSize();
			DWORD fontStyle=pStatic->GetFontStyle();

			if(dwBgColor!=::GetSysColor(COLOR_BTNFACE))
			{
				CString font;
				font.Format(_T("����:(%s,%d,%d,%d,%d)"),szFont,dwColor,dwBgColor,nSize,fontStyle);
				staticProp=staticProp+font+_T("|");
			}
			else
			{
				CString font;
				font.Format(_T("����:(%s,%d,FFFFFFFF,%d,%d)"),szFont,dwColor,nSize,fontStyle);
				if(font!=_T("����:(MS Sans Serif,0,FFFFFFFF,8,64)"))
					staticProp=staticProp+font+_T("|");
			}

			if(staticProp.IsEmpty())
				csStatic=csStatic+_T("|");
			else
				csStatic=csStatic+staticProp;

			//�¼�
			CString staticEvent;
			if(!pStatic->m_eClick.IsEmpty())
				staticEvent=staticEvent+_T("��굥��:")+pStatic->m_eClick+_T("|");

			if(staticEvent.IsEmpty())
				csStatic+=_T(" |");
			else
				csStatic=csStatic+_T(" ")+staticEvent;

			winInfo+=csStatic;
			//cFile.Write(csStatic,csStatic.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_GROUP)
		{
			CTCGroupBox* pGroup=(CTCGroupBox *)ctl->GetCWnd();

			//����
			CString text;
			pGroup->GetText(text);
			///ת���ַ�
			text.Replace(_T("\\"),_T("\\\\"));
			text.Replace(_T("\""),_T("\\\""));
			CString title;
			title.Format(_T("\"%s\""),text);
			RECT rc;
			pGroup->GetWindowRect(&rc);
			this->ScreenToClient(&rc);
			CString csGroup;
			csGroup.Format(_T("%s %s %s %d,%d,%d,%d "),_T("�����"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

			//����
			CString groupProp;

			LOGFONT lf = pGroup->GetGroupFont();
			

			int nHeight=lf.lfHeight;
			int nWidth=lf.lfWidth;
			int nEscap=lf.lfEscapement;
			int nOrient=lf.lfOrientation;
			int nWeight=lf.lfWeight;

			BYTE nItalic=lf.lfItalic;
			BYTE nUnderlint=lf.lfUnderline;
			BYTE nStrikeOut=lf.lfStrikeOut;
			BYTE nCharSet=lf.lfCharSet;
			BYTE nOutPreci=lf.lfOutPrecision;
			BYTE nClipPreci=lf.lfClipPrecision;
			BYTE nQuality=lf.lfQuality;
			BYTE nPitch=lf.lfPitchAndFamily;

			CString szFont=lf.lfFaceName;

			CString fontGroup;
			fontGroup.Format(_T("����:(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s)"),nHeight,nWidth,nEscap,nOrient,nWeight,
				nItalic,nUnderlint,nStrikeOut,nCharSet,nOutPreci,nClipPreci,nQuality,nPitch,szFont);

			
			if(fontGroup!=_T("����:(-11,0,0,0,400,0,0,0,134,0,0,0,0,MS Shell Dlg 2)"))
				groupProp+=fontGroup+_T("|");


			//����
			if(pGroup->m_dwAlignment!=SS_LEFT)
			{
				CString csHelp;
				csHelp.Format(_T("����:%d|"),pGroup->m_dwAlignment);
				groupProp=groupProp+csHelp;
			}

			//�߿���ɫ
			if(pGroup->GetBorderColor()!=::GetSysColor(COLOR_3DSHADOW))
			{
				groupProp=groupProp+_T("�߿���ɫ:")+ColorToString(pGroup->GetBorderColor())+_T("|");
			}

			//���ⱳ����ɫ
			if(pGroup->GetTitleBackgroundColor()!=::GetSysColor(COLOR_BTNFACE))
			{
				groupProp=groupProp+_T("���ⱳ����ɫ:")+ColorToString(pGroup->GetTitleBackgroundColor())+_T("|");
			}

			//�ͻ�����ɫ
			if(pGroup->GetClientBackgroundColor()!=::GetSysColor(COLOR_BTNFACE))
			{
				groupProp=groupProp+_T("�ͻ�����ɫ:")+ColorToString(pGroup->GetClientBackgroundColor())+_T("|");
			}

			//������ɫ
			if(pGroup->GetTitleTextColor()!=::GetSysColor(COLOR_WINDOWTEXT))
			{
				groupProp=groupProp+_T("������ɫ:")+ColorToString(pGroup->GetTitleTextColor())+_T("|");
			}

			//�����ģʽ
			if(pGroup->GetGroupBoxStyle()!=CTCGroupBox::TCGB_FRAME)
			{
				groupProp=groupProp+_T("�����ģʽ:")+_T("1")+_T("|");
			}

			if(groupProp.IsEmpty())
				csGroup=csGroup+_T("|");
			else
				csGroup=csGroup+groupProp;

			csGroup=csGroup+_T(" |");

			winInfo+=csGroup;
			//cFile.Write(csGroup,csGroup.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_BUTTON)
		{
			CTCButton* pButton=(CTCButton *)ctl->GetCWnd();

			CString text;
			pButton->GetWindowTextW(text);

			///ת���ַ�
			text.Replace(_T("\\"),_T("\\\\"));
			text.Replace(_T("\""),_T("\\\""));


			CString title;
			title.Format(_T("\"%s\""),text);

			RECT rc;
			pButton->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csButton;
			csButton.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ť"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

			//����
			CString buttonProp;

			if(pButton->TCGetDefButton())
			{
				CString csDefButton;
				csDefButton.Format(_T("Ĭ�ϰ�ť:%d|"),(int)pButton->TCGetDefButton());
				buttonProp+=csDefButton;
			}

			if(!pButton->TCGetAbled())
			{
				CString csAbled;
				csAbled.Format(_T("��Ч:%d|"),pButton->TCGetAbled());
				buttonProp+=csAbled;
			}

			if(pButton->TCGetMulityLine())
			{
				CString csMulityLine;
				csMulityLine.Format(_T("����:%d|"),(int)pButton->TCGetMulityLine());
				buttonProp+=csMulityLine;
			}

			if(buttonProp.IsEmpty())
				csButton+=_T("|");
			else
				csButton+=buttonProp;



			/*BOOL isMulityLine=pButton->TCGetMulityLine();
			BOOL isAble=pButton->TCGetAbled();*/


			CString clickEvent;
			if(!pButton->TCGetClickEvent().IsEmpty())
				clickEvent.Format(_T("��굥��:%s"),pButton->TCGetClickEvent());

			if(clickEvent.IsEmpty())
				csButton+=_T(" |");
			else
				csButton+=_T(" ")+clickEvent+_T("|");


			/*CString TCButton;
			TCButton.Format(_T("%s %s %s %d,%d,%d,%d ��Ч:%d|����:%d| %s|"),_T("��ť"),ctl->GetCtlID(),title,rc.left,rc.top,
			rc.right,rc.bottom,(int)isAble,(int)isMulityLine,clickEvent);*/

			winInfo+=csButton;
			//cFile.Write(csButton,csButton.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_EDIT)
		{
			CTCEdit* pEdit=(CTCEdit *)ctl->GetCWnd();

			CString text;
			pEdit->GetWindowTextW(text);

			///ת���ַ�
			text.Replace(_T("\\"),_T("\\\\"));
			text.Replace(_T("\""),_T("\\\""));

			CString title;
			title.Format(_T("\"%s\""),text);

			RECT rc;
			pEdit->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csEdit;
			csEdit.Format(_T("%s %s %s %d,%d,%d,%d "),_T("�༭��"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

			CString csEditProp;
			bool isAble = pEdit->TCGetAbled();
			bool isMulity = pEdit->TCGetMulityLine();
			bool isPassWord = pEdit->TCGetPassWord();
			bool isReadOnly = pEdit->TCGetReadOnly();
			bool isNum = pEdit->TCGetNumber();
			bool isVerty = pEdit->TCGetVerti();
			bool isHoriz = pEdit->TCGetHoriz();

			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				csEditProp+=able;
			}

			if(isMulity)
			{
				CString mulityLine;
				mulityLine.Format(_T("����:%d|"),isMulity);
				csEditProp+=mulityLine;
			}

			if(isPassWord)
			{
				CString password;
				password.Format(_T("����:%d|"),isPassWord);
				csEditProp+=password;
			}

			if(isNum)
			{
				CString num;
				num.Format(_T("����:%d|"),isNum);
				csEditProp+=num;
			}

			if(isReadOnly)
			{
				CString readonly;
				readonly.Format(_T("ֻ��:%d|"),isReadOnly);
				csEditProp+=readonly;
			}

			if(isVerty)
			{
				CString verty;
				verty.Format(_T("��ֱ:%d|"),isVerty);
				csEditProp+=verty;
			}

			if(isHoriz)
			{
				CString horiz;
				horiz.Format(_T("ˮƽ:%d|"),isHoriz);
				csEditProp+=horiz;
			}

			if(pEdit->GetTextColor()!=::GetSysColor(COLOR_WINDOWTEXT))
			{
				csEditProp+=_T("�ı���ɫ:")+ColorToString(pEdit->GetTextColor())+_T("|");
			}

			if(pEdit->GetBackColor()!=RGB(255,255,255))
			{
				csEditProp+=_T("������ɫ:")+ColorToString(pEdit->GetBackColor())+_T("|");
			}

			if(csEditProp.IsEmpty())
				csEdit+=_T("|");
			else
				csEdit+=csEditProp;

			CString editEvent;
			if(!pEdit->TCGetEnChangeEvent().IsEmpty())
			{
				CString enChange;
				enChange.Format(_T("���ݸı�:%s|"),pEdit->TCGetEnChangeEvent());
				editEvent+=enChange;
			}

			if(!pEdit->TCGetSetFocusEvent().IsEmpty())
			{
				CString setFocus;
				setFocus.Format(_T("��ý���:%s|"),pEdit->TCGetSetFocusEvent());
				editEvent+=setFocus;
			}
			
			if(!pEdit->TCGetKillfocusEvent().IsEmpty())
			{
				CString killFocus;
				killFocus.Format(_T("ʧȥ����:%s|"),pEdit->TCGetKillfocusEvent());
				editEvent+=killFocus;
			}

			if(editEvent.IsEmpty())
				csEdit+=_T(" |");
			else
				csEdit+=_T(" ")+editEvent;

			winInfo+=csEdit;
			//cFile.Write(csEdit,csEdit.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_RADIO)
		{
			CTCRadio* pRadio=(CTCRadio *)ctl->GetCWnd();

			CString text;
			pRadio->GetWindowTextW(text);

			///ת���ַ�
			text.Replace(_T("\\"),_T("\\\\"));
			text.Replace(_T("\""),_T("\\\""));

			CString title;
			title.Format(_T("\"%s\""),text);
			RECT rc;
			pRadio->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csRadio;
			csRadio.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ѡ��"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

			BOOL isCheck=pRadio->TCGetCheck();
			BOOL isTextLeft=pRadio->TCGetTextLeft();
			BOOL isAble=pRadio->TCGetAbled();
			BOOL isGroup = pRadio->TCGetGroup();

			CString radioProp;
			if(isCheck)
			{
				CString check;
				check.Format(_T("�Ƿ�ѡ��:%d|"),(int)isCheck);
				radioProp+=check;
			}
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),(int)isAble);
				radioProp+=able;
			}
			if(isGroup)
			{
				CString group;
				group.Format(_T("����:%d|"),(int)isGroup);
				radioProp+=group;
			}
			if(isTextLeft)
			{
				CString textLeft;
				textLeft.Format(_T("�������:%d|"),(int)isTextLeft);
				radioProp+=textLeft;
			}

			if(radioProp.IsEmpty())
				csRadio+=_T("|");
			else
				csRadio+=radioProp;

			CString clickEvent;
			if(!pRadio->TCGetRadioEvent().IsEmpty())
			{
				clickEvent.Format(_T("�������:%s|"),pRadio->TCGetRadioEvent());
			}

			if(clickEvent.IsEmpty())
				csRadio+=_T(" |");
			else 
				csRadio+=_T(" ")+clickEvent;

			winInfo+=csRadio;
			//cFile.Write(csRadio,csRadio.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_CHECK)
		{
			CTCCheck * pCheck=(CTCCheck *)ctl->GetCWnd();

			CString text;
			pCheck->GetWindowTextW(text);

			///ת���ַ�
			text.Replace(_T("\\"),_T("\\\\"));
			text.Replace(_T("\""),_T("\\\""));

			CString title;
			title.Format(_T("\"%s\""),text);

			RECT rc;
			pCheck->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csCheck;
			csCheck.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ѡ��"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

			BOOL isCheck=pCheck->TCGetCheck();
			BOOL isTextLeft=pCheck->TCGetTextLeft();
			BOOL isAble=pCheck->TCGetAbled();

			CString checkProp;
			if(isCheck)
			{
				CString check;
				check.Format(_T("�Ƿ�ѡ��:%d|"),(int)isCheck);
				checkProp+=check;
			}
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),(int)isAble);
				checkProp+=able;
			}
			if(isTextLeft)
			{
				CString textLeft;
				textLeft.Format(_T("�������:%d|"),isTextLeft);
				checkProp+=textLeft;
			}

			if(checkProp.IsEmpty())
				csCheck+=_T("|");
			else 
				csCheck+=checkProp;

			CString clickEvent;
			if(!pCheck->TCGetCheckEvent().IsEmpty())
			{
				clickEvent.Format(_T("�������:%s|"),pCheck->TCGetCheckEvent());
			}

			if(clickEvent.IsEmpty())
				csCheck+=_T(" |");
			else
				csCheck+=_T(" ")+clickEvent;

			winInfo+=csCheck;
			//cFile.Write(csCheck,csCheck.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_COMBOBOX)
		{
			CTCComboBox* pCombox=(CTCComboBox *)ctl->GetCWnd();		
			CString text;
			for(int i=0;i<pCombox->GetCount();i++)
			{
				CString node;
				pCombox->GetLBText(i,node);

				///ת���ַ�
				node.Replace(_T("\\"),_T("\\\\"));
				node.Replace(_T("\""),_T("\\\""));
				node.Replace(_T("\|"),_T("\\\|"));
				CString add;
				add.Format(_T("%s%s"),node,_T("|"));
				//node=node+_T("\n");
				//add=add+_T(",");
				text+=add;
			}
			if(text.IsEmpty())
				text="\"\"";
			else
				text=_T("\"")+text+_T("\"");			

			RECT rc;
			pCombox->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csCombox;
			csCombox.Format(_T("%s %s %s %d,%d,%d,%d "),_T("������"),ctl->GetCtlID(),text,rc.left,rc.top,rc.right,rc.bottom);

			bool isAble = pCombox->TCGetAbled();
			CString comboxProp;
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				comboxProp+=able;
			}

			if(comboxProp.IsEmpty())
				csCombox+=_T("|");
			else
				csCombox+=comboxProp;

			CString comEvent;
			if(!pCombox->GetLeftDownEvent().IsEmpty())
			{
				CString click;
				click.Format(_T("�������:%s|"),pCombox->GetLeftDownEvent());
				comEvent+=click;
			}
			if(!pCombox->GetEnChangeEvent().IsEmpty())
			{
				CString enchange;
				enchange.Format(_T("ѡ��ı�:%s|"),pCombox->GetEnChangeEvent());
				comEvent+=enchange;
			}

			if(comEvent.IsEmpty())
				csCombox+=_T(" |");
			else
				csCombox+=_T(" ")+comEvent;

			winInfo+=csCombox;
			//cFile.Write(csCombox,csCombox.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_LISTBOX)
		{
			CTCListBox* pList=(CTCListBox *)ctl->GetCWnd();
			CString title;

			title.Format(_T("\"%s\""),pList->TCGetListBoxCaption());
			CString text;
			CString titletext=pList->TCGetListBoxCaption();

			if(!titletext.IsEmpty())
			{
				///ת���ַ�
				titletext.Replace(_T("\\"),_T("\\\\"));
				titletext.Replace(_T("\""),_T("\\\""));
			}

			title.Format(_T("\"%s\""),titletext);

			for(int i=0;i<pList->GetCount();i++)
			{
				CString node;
				node = pList->GetItemText(i);

				///ת���ַ�
				node.Replace(_T("\\"),_T("\\\\"));
				node.Replace(_T("\""),_T("\\\""));
				node.Replace(_T("\|"),_T("\\\|"));
				CString add;
				add.Format(_T("%s%s"),node,_T("|"));
				//node=node+_T("\n");
				//add=add+_T(",");
				text+=add;
			}
			if(text.IsEmpty())
				text="\"\"";
			else
				text=_T("\"")+text+_T("\"");

			RECT rc;
			pList->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csList;
			csList.Format(_T("%s %s %s %s %d,%d,%d,%d "),_T("�б��"),ctl->GetCtlID(),title,text,rc.left,rc.top,rc.right,rc.bottom);

			bool isAble=pList->TCGetAbled();
			bool isDefaultRool=pList->TCGetDefaultRool();

			CString listProp;
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				listProp+=able;
			}
			if(isDefaultRool)
			{
				CString rool;
				rool.Format(_T("Ĭ�Ϲ���:%d|"),isDefaultRool);
				listProp+=rool;
			}
			
			if(listProp.IsEmpty())
				csList+=_T("|");
			else
				csList+=listProp;

			CString listEvent;
			if(!pList->TCGetListBoxDoubleClick().IsEmpty())
			{
				CString clicked;
				clicked.Format(_T("���˫��:%s|"),pList->TCGetListBoxDoubleClick());
				listEvent+=clicked;
			}
			if(!pList->TCGetListBoxSelChange().IsEmpty())
			{
				CString selChange;
				selChange.Format(_T("ѡ��ı�:%s|"),pList->TCGetListBoxSelChange());
				listEvent+=selChange;
			}

			if(listEvent.IsEmpty())
				csList+=_T(" |");
			else
				csList+=_T(" ") + listEvent;

			winInfo+=csList;
			//cFile.Write(csList,csList.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_PROGRESS)
		{
			CTCProgress* pProgress=(CTCProgress *)ctl->GetCWnd();
			RECT rc;
			pProgress->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csProgress;
			csProgress.Format(_T("%s %s %d,%d,%d,%d "),_T("������"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			bool isAble=pProgress->TCGetAbled();
			BOOL isVertical=pProgress->TCGetVertical();

			CString progressProp;
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				progressProp+=able;
			}
			if(isVertical)
			{
				CString verti;
				verti.Format(_T("��ֱ:%d|"),(int)isVertical);
				progressProp+=verti;
			}

			if(progressProp.IsEmpty())
				csProgress+=_T("|");
			else
				csProgress+=progressProp;

			csProgress+=_T(" |");

			//CString TCProgress;
			//TCProgress.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d|��ֱ:%d| |"),_T("������")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)isAble,(int)isVertical);

			winInfo+=csProgress;
			//cFile.Write(csProgress,csProgress.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_DATE)
		{
			CTCDate* pDate=(CTCDate *)ctl->GetCWnd();
			RECT rc;
			pDate->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csDate;
			csDate.Format(_T("%s %s %d,%d,%d,%d "),_T("���ڿ�"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			bool isAble = pDate->TCGetAbled();

			CString dateProp;
			if(isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				dateProp+=able;
			}
			if(dateProp.IsEmpty())
				csDate+=_T("|");
			else
				csDate+=dateProp;

			csDate+=_T(" |");

			//CString TCDate;
			//TCDate.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d| |"),_T("���ڿ�")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)isAble);

			winInfo+=csDate;
			//cFile.Write(csDate,csDate.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_SLIDER)
		{
			CTCSlider* pSlider=(CTCSlider *)ctl->GetCWnd();
			RECT rc;
			pSlider->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csSlider;
			csSlider.Format(_T("%s %s %d,%d,%d,%d "),_T("����"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			BOOL vertical = pSlider->TCGetVertical();
			BOOL isAble = pSlider->TCGetAbled();

			CString sliderProp;
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				sliderProp+=able;
			}
			if(vertical)
			{
				CString verti;
				verti.Format(_T("��ֱ:%d|"),(int)vertical);
				sliderProp+=verti;
			}

			if(sliderProp.IsEmpty())
				csSlider+=_T("|");
			else
				csSlider+=sliderProp;

			CString sliderEvent;
			if(!pSlider->TCGetSliderChangeEvent().IsEmpty())
			{
				CString posChange;
				posChange.Format(_T("λ�øı�:%s|"),pSlider->TCGetSliderChangeEvent());
				sliderEvent+=posChange;
			}

			if(sliderEvent.IsEmpty())
				csSlider+=_T(" |");
			else
				csSlider+=_T(" ")+sliderEvent;

			//CString TCSlider;
			//TCSlider.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d|��ֱ:%d| %s|"),_T("����")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)isAble,(int)vertical,sliderEvent);

			winInfo+=csSlider;
			//cFile.Write(csSlider,csSlider.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_PICTURE)
		{
			CTCPicture* pPic=(CTCPicture *)ctl->GetCWnd();
			CString picName;
			picName=pPic->GetName();

			RECT rc;
			pPic->GetWindowRect(&rc);
			this->ScreenToClient(&rc);  

			CString csPicture;
			csPicture.Format(_T("%s %s %d,%d,%d,%d "),_T("ͼƬ��"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			CString pictureProp;
			if(!picName.IsEmpty())
			{
				pictureProp+=_T("ͼƬ����:")+picName+_T("|");
			}

			if(pictureProp.IsEmpty())
				csPicture+=_T("|");
			else
				csPicture+=pictureProp;

			CString picsEvent;
			if(!pPic->TCGetClickEvent().IsEmpty())
			{
				CString click;
				click.Format(_T("�������:%s|"),pPic->TCGetClickEvent());
				picsEvent+=click;
			}

			if(picsEvent.IsEmpty())
				csPicture+=_T(" |");
			else
				csPicture+=_T(" ")+picsEvent;

			//CString TCPicture;
			//TCPicture.Format(_T("%s %s %d,%d,%d,%d ͼƬ����:%s| |"),_T("ͼƬ��")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,picName);

			winInfo+=csPicture;
			//cFile.Write(csPicture,csPicture.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_PATH)
		{
			CTCBrowse* pPath=(CTCBrowse *)ctl->GetCWnd();
			RECT rc;
			pPath->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csPath;
			csPath.Format(_T("%s %s %d,%d,%d,%d "),_T("�����"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			int mode=(int)pPath->TCGetMode();
			bool isAble=pPath->TCGetAbled();

			CString pathProp;
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				pathProp+=able;
			}
			if(mode!=0)
			{
				CString browType;
				browType.Format(_T("���ģʽ:%d|"),mode);
				pathProp+=browType;
			}

			if(pathProp.IsEmpty())
				csPath+=_T("|");
			else
				csPath+=pathProp;

			CString pathEvent;
			if(!pPath->TCGetEnChangeEvent().IsEmpty())
			{
				CString selChange;
				selChange.Format(_T("ѡ��ı�:%s|"),pPath->TCGetEnChangeEvent());
				pathEvent+=selChange;
			}

			if(pathEvent.IsEmpty())
				csPath+=_T(" |");
			else
				csPath+=_T(" ")+pathEvent;		

			//CString TCPath;
			//TCPath.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d|���ģʽ:%d| %s|"),_T("�����")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)able,mode,pathEvent);

			winInfo+=csPath;
			//cFile.Write(csPath,csPath.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_TIMER)
		{
			CTCTimer* pTimer=(CTCTimer*)ctl->GetCWnd();
			UINT timer;
			timer=pTimer->GetTimer();

			RECT rc;
			pTimer->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csTimer;
			csTimer.Format(_T("%s %s %d,%d,%d,%d "),_T("ʱ��"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			CString timerProp;
			if(timer!=0)
			{
				CString time;
				time.Format(_T("Ƶ��:%d|"),timer);
				timerProp+=time;
			}

			if(timerProp.IsEmpty())
				csTimer+=_T("|");
			else
				csTimer+=timerProp;

			CString timerEvent;
			if(!pTimer->TCGetTimeEvent().IsEmpty())
			{
				CString strEvent;
				strEvent.Format(_T("ʱ��:%s|"),pTimer->TCGetTimeEvent());
				timerEvent+=strEvent;
			}

			if(timerEvent.IsEmpty())
				csTimer+=_T(" |");
			else
				csTimer+=_T(" ")+timerEvent;

			//CString TCTimer;
			//TCTimer.Format(_T("%s %s %d,%d,%d,%d Ƶ��:%d| %s|"),_T("ʱ��")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,timer,timerEvent);
			winInfo+=csTimer;
			//cFile.Write(csTimer,csTimer.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_HOTKEY)
		{
			CTCHotKeyCtrl* pHotKey=(CTCHotKeyCtrl *)ctl->GetCWnd();
			WORD wVirtualKey=0;
			WORD wMod=0;
			pHotKey->GetHotKeyValue(wVirtualKey,wMod);

			RECT rc;
			pHotKey->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csHotKey;
			csHotKey.Format(_T("%s %s %d,%d,%d,%d "),_T("�ȼ�"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			bool isAble=pHotKey->TCGetAbled();

			CString hotkeyProp;
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				hotkeyProp+=able;
			}

			if(wVirtualKey!=0)
			{
				CString virtualkey;
				virtualkey.Format(_T("�����ֵ:%d|"),wVirtualKey);
				hotkeyProp+=virtualkey;
			}

			if(wMod!=0)
			{
				CString mod;
				mod.Format(_T("ģֵ̬:%d|"),wMod);
				hotkeyProp+=mod;
			}

			if(hotkeyProp.IsEmpty())
				csHotKey+=_T("|");
			else
				csHotKey+=hotkeyProp;

			CString hotkeyEvent;
			if(!pHotKey->TCGetHotKeyEvent().IsEmpty())
			{
				CString hotkey;
				hotkey.Format(_T("�ȼ�:%s|"),pHotKey->TCGetHotKeyEvent());
				hotkeyEvent+=hotkey;
			}
			if(!pHotKey->TCGetKillFocusEvent().IsEmpty())
			{
				CString killfocus;
				killfocus.Format(_T("ʧȥ����:%s|"),pHotKey->TCGetKillFocusEvent());
				hotkeyEvent+=killfocus;
			}

			if(hotkeyEvent.IsEmpty())
				csHotKey+=_T(" |");
			else
				csHotKey+=_T(" ")+hotkeyEvent;

			winInfo+=csHotKey;
			//cFile.Write(csHotKey,csHotKey.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_EXPLORER)
		{
			CTCExplorer* pExplorer=(CTCExplorer *)ctl->GetCWnd();
			CString url = pExplorer->TCGetURL();
			CString loadComplete = pExplorer->TCGetloadComplete();

			RECT rc;
			pExplorer->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csExplorer;
			csExplorer.Format(_T("%s %s %d,%d,%d,%d "),_T("�����"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			CString explorerProp;
			if(!url.IsEmpty())
				explorerProp+=_T("URL:")+url+_T("|");

			if(explorerProp.IsEmpty())
				csExplorer+=_T("|");
			else
				csExplorer+=explorerProp;

			CString explorerEvet;
			if(!loadComplete.IsEmpty())
				explorerEvet+=_T("�������:")+loadComplete+_T("|");

				if(explorerEvet.IsEmpty())
					csExplorer+=_T(" |");
				else
					csExplorer+=_T(" ")+explorerEvet;

			//CString TCExplorer;
			//TCExplorer.Format(_T("%s %s %d,%d,%d,%d URL:%s| �������:%s|"),_T("�����")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,url,loadComplete);
			
			winInfo+=csExplorer;
			//cFile.Write(csExplorer,csExplorer.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_GRIDCTRL)
		{
			CGridCtrl* pGridCtrl=(CGridCtrl *)ctl->GetCWnd();

			RECT rc;
			pGridCtrl->GetWindowRect(&rc);
			this->ScreenToClient(&rc);

			CString csGrid;
			csGrid.Format(_T("%s %s %d,%d,%d,%d "),_T("���"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			CString gridProp;

			LOGFONT lf;
			CFont* font = pGridCtrl->GetFont();
			font->GetLogFont(&lf);

			int nHeight=lf.lfHeight;
			int nWidth=lf.lfWidth;
			int nEscap=lf.lfEscapement;
			int nOrient=lf.lfOrientation;
			int nWeight=lf.lfWeight;

			BYTE nItalic=lf.lfItalic;
			BYTE nUnderlint=lf.lfUnderline;
			BYTE nStrikeOut=lf.lfStrikeOut;
			BYTE nCharSet=lf.lfCharSet;
			BYTE nOutPreci=lf.lfOutPrecision;
			BYTE nClipPreci=lf.lfClipPrecision;
			BYTE nQuality=lf.lfQuality;
			BYTE nPitch=lf.lfPitchAndFamily;

			CString szFont=lf.lfFaceName;

			CString fontGrid;
			fontGrid.Format(_T("����:(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s)"),nHeight,nWidth,nEscap,nOrient,nWeight,
				nItalic,nUnderlint,nStrikeOut,nCharSet,nOutPreci,nClipPreci,nQuality,nPitch,szFont);

			if(fontGrid!=_T("����:(-11,0,0,0,400,0,0,0,134,0,0,0,0,MS Shell Dlg 2)"))
				gridProp+=fontGrid+_T("|");

			CString csRowCount,csColCount,csFixRowCount,csFixColCount;
			csRowCount.Format(_T("����:%d|"),pGridCtrl->GetRowCount());
			csColCount.Format(_T("����:%d|"),pGridCtrl->GetColumnCount());
			csFixRowCount.Format(_T("�̶�����:%d|"),pGridCtrl->GetFixedRowCount());
			csFixColCount.Format(_T("�̶�����:%d|"),pGridCtrl->GetFixedColumnCount());

			gridProp+=csRowCount+csColCount+csFixRowCount+csFixColCount;

			if(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetBackClr()!=RGB(0xFF, 0xFF, 0xE0))
				gridProp+=_T("������ɫ:")+ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetBackClr())+_T("|");
			if(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetTextClr()!=0)
				gridProp+=_T("�ı���ɫ:")+ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetTextClr())+_T("|");
			if(pGridCtrl->GetFixedBkColor()!=::GetSysColor(COLOR_BTNFACE))
				gridProp+=_T("�̶������ɫ:")+ColorToString(pGridCtrl->GetFixedBkColor())+_T("|");
			if(pGridCtrl->GetFixedTextColor()!=0)
				gridProp+=_T("�̶����ı���ɫ:")+ColorToString(pGridCtrl->GetFixedTextColor())+_T("|");

			if(!pGridCtrl->IsShowHorizLines())
			{
				CString horizLine;
				horizLine.Format(_T("%d"),(int)pGridCtrl->IsShowHorizLines());
				gridProp+=_T("ˮƽ����:")+horizLine+_T("|");
			}
			if(!pGridCtrl->IsShowVertiLines())
			{
				CString vertiLine;
				vertiLine.Format(_T("%d"),(int)pGridCtrl->IsShowVertiLines());
				gridProp+=_T("��ֱ����:")+vertiLine+_T("|");
			}
			if(!pGridCtrl->TCGetAllSellsEditAbled())
			{
				CString csEdit;
				csEdit.Format(_T("%d"),(int)pGridCtrl->TCGetAllSellsEditAbled());
				gridProp+=_T("�ɱ༭:")+csEdit+_T("|");
			}
			if(pGridCtrl->GetHeaderSort())
			{
				CString csHeadSort;
				csHeadSort.Format(_T("%d"),(int)pGridCtrl->GetHeaderSort());
				gridProp+=_T("����:")+csHeadSort+_T("|");
			}

			if(pGridCtrl->TCGetExpandToFit())
			{
				CString csExpand;
				csExpand.Format(_T("%d"),(int)pGridCtrl->TCGetExpandToFit());
				gridProp+=_T("���:")+csExpand+_T("|");
			}

			if(!pGridCtrl->IsSelectable())
			{
				CString csSelect;
				csSelect.Format(_T("%d"),(int)pGridCtrl->IsSelectable());
				gridProp+=_T("ѡ��:")+csSelect+_T("|");
			}
			if(!pGridCtrl->GetFixedRowSelection())
			{
				CString csFixRowSelect;
				csFixRowSelect.Format(_T("%d"),(int)pGridCtrl->GetFixedRowSelection());
				gridProp+=_T("�̶���ѡ��:")+csFixRowSelect+_T("|");
			}
			if(!pGridCtrl->GetFixedColumnSelection())
			{
				CString csFixColSelect;
				csFixColSelect.Format(_T("%d"),(int)pGridCtrl->GetFixedColumnSelection());
				gridProp+=_T("�̶���ѡ��:")+csFixColSelect+_T("|");
			}
			if(pGridCtrl->GetListMode())
			{
				CString csListMode;
				csListMode.Format(_T("%d"),(int)pGridCtrl->GetListMode());
				gridProp+=_T("�б�ģʽ:")+csListMode+_T("|");
			}

			CString colInfo;
			for(int col=0;col<pGridCtrl->GetColumnCount();col++)
			{
				CString node,width,type,readonly;
				node = pGridCtrl->GetItemText(0,col);
				width.Format(_T("%d"),pGridCtrl->GetColumnWidth(col));
				type.Format(_T("%d"),(int)pGridCtrl->GetItemData(0,col));
				readonly.Format(_T("%d"),(int)pGridCtrl->TCGetColReadInfo(0,col));
				///ת���ַ�
				node.Replace(_T("\\"),_T("\\\\"));
				node.Replace(_T("\""),_T("\\\""));
				if(node.IsEmpty())
					node="\"\"";
				else 
					node=_T("\"")+node+_T("\"");
				CString add;
				add.Format(_T("����:%s,ֻ��:%s,�п�:%s,����:%s%s"),node,readonly,width,type,_T(";"));
				colInfo+=add;
			}
			gridProp+=_T("������:")+colInfo+_T("|");

			//gridProp������Ϊ��
			csGrid+=gridProp;

			CString gridEvent;
			if(!pGridCtrl->TCGetClickEvent().IsEmpty())
				gridEvent+=_T("�������:")+pGridCtrl->TCGetClickEvent()+_T("|");
			if(!pGridCtrl->TCGetBeforeEditEvent().IsEmpty())
				gridEvent+=_T("�༭��ʼ:")+pGridCtrl->TCGetBeforeEditEvent()+_T("|");
			if(!pGridCtrl->TCGetAfterEditEvent().IsEmpty())
				gridEvent+=_T("�༭����:")+pGridCtrl->TCGetAfterEditEvent()+_T("|");
			if(!pGridCtrl->TCGetHeadSortEvent().IsEmpty())
				gridEvent+=_T("�������:")+pGridCtrl->TCGetHeadSortEvent()+_T("|");

			if(gridEvent.IsEmpty())
				csGrid+=_T(" |");
			else
				csGrid=csGrid+_T(" ")+gridEvent;
			//CString colInfo;
			//for(int col=0;col<pGridCtrl->GetColumnCount();col++)
			//{
			//	CString node,width,type;
			//	node = pGridCtrl->GetItemText(0,col);
			//	width.Format(_T("%d"),pGridCtrl->GetColumnWidth(col));
			//	type.Format(_T("%d"),(int)pGridCtrl->GetItemData(0,col));
			//	///ת���ַ�
			//	node.Replace(_T("\\"),_T("\\\\"));
			//	node.Replace(_T("\""),_T("\\\""));
			//	if(node.IsEmpty())
			//		node="\"\"";
			//	else 
			//		node=_T("\"")+node+_T("\"");
			//	CString add;
			//	add.Format(_T("����:%s,�п�:%s,����:%s%s"),node,width,type,_T(";"));
			//	colInfo+=add;
			//}			


			//LOGFONT lf;
			//CFont* font = pGridCtrl->GetFont();
			//font->GetLogFont(&lf);

			//int nHeight=lf.lfHeight;
			//int nWidth=lf.lfWidth;
			//int nEscap=lf.lfEscapement;
			//int nOrient=lf.lfOrientation;
			//int nWeight=lf.lfWeight;

			//BYTE nItalic=lf.lfItalic;
			//BYTE nUnderlint=lf.lfUnderline;
			//BYTE nStrikeOut=lf.lfStrikeOut;
			//BYTE nCharSet=lf.lfCharSet;
			//BYTE nOutPreci=lf.lfOutPrecision;
			//BYTE nClipPreci=lf.lfClipPrecision;
			//BYTE nQuality=lf.lfQuality;
			//BYTE nPitch=lf.lfPitchAndFamily;

			//CString szFont=lf.lfFaceName;


			//CString fontGrid;
			//fontGrid.Format(_T("����:(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s)"),nHeight,nWidth,nEscap,nOrient,nWeight,
			//	nItalic,nUnderlint,nStrikeOut,nCharSet,nOutPreci,nClipPreci,nQuality,nPitch,szFont);

			//CString TCGridCtrl;
			//TCGridCtrl.Format(_T("%s %s %d,%d,%d,%d %s|����:%d|����:%d|�̶�����:%d|�̶�����:%d|������ɫ:%s|�ı���ɫ:%s|�̶������ɫ:%s\
			//					 |�̶����ı���ɫ:%s|ˮƽ����:%d|��ֱ����:%d|�ɱ༭:%d|����:%d|������:%s|���:%d|ѡ��:%d|�̶���ѡ��:%d|�̶���ѡ��:%d|�б�ģʽ:%d| �������:%s|�༭��ʼ:%s|�༭����:%s|"),
			//					 _T("���")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,fontGrid,pGridCtrl->GetRowCount(),pGridCtrl->GetColumnCount()
			//					 ,pGridCtrl->GetFixedRowCount(),pGridCtrl->GetFixedColumnCount(),ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetBackClr())
			//					 ,ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetTextClr()),ColorToString(pGridCtrl->GetFixedBkColor())
			//					 ,ColorToString(pGridCtrl->GetFixedTextColor()),(int)pGridCtrl->IsShowHorizLines(),(int)pGridCtrl->IsShowVertiLines()
			//					 ,(int)pGridCtrl->TCGetAllSellsEditAbled(),(int)pGridCtrl->GetHeaderSort(),colInfo,(int)pGridCtrl->TCGetExpandToFit()
			//					 ,(int)pGridCtrl->IsSelectable(),(int)pGridCtrl->GetFixedRowSelection(),(int)pGridCtrl->GetFixedColumnSelection(),(int)pGridCtrl->GetListMode()
			//					 ,pGridCtrl->TCGetClickEvent(),pGridCtrl->TCGetBeforeEditEvent(),pGridCtrl->TCGetAfterEditEvent());

			winInfo+=csGrid;
			//cFile.Write(csGrid,csGrid.GetLength()*sizeof(TCHAR));
		}
		else if(type==TCTT_TAB)
		{
			CTCTab* pTab=(CTCTab *)ctl->GetCWnd();
			RECT rc;
			pTab->GetWindowRect(&rc);
			this->ScreenToClient(&rc); 

			CString csTab;
			csTab.Format(_T("%s %s %d,%d,%d,%d "),_T("ѡ�"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

			int pagecount=pTab->getTabPageCount();
			bool isAble=pTab->TCGetAbled();

			CString tabProp;
			if(!isAble)
			{
				CString able;
				able.Format(_T("��Ч:%d|"),isAble);
				tabProp+=able;
			}

			if(pagecount!=0)
			{
				CString pagenum;
				pagenum.Format(_T("ҳ��:%d|"),pagecount);
				tabProp+=pagenum;
			}


			CString tabEvent;
			if(!pTab->TCGetSelChange().IsEmpty())
			{
				CString selChangeEvent;
				selChangeEvent.Format(_T("ѡ��ı�:%s|"),pTab->TCGetSelChange());
				tabEvent+=selChangeEvent;
			}

			if(tabEvent.IsEmpty())
				csTab+=_T(" |");
			else
				csTab+=_T(" ")+tabEvent;

			////////////////////////�����Ǳ���ÿ��tabpage�Ŀؼ�////////////////////////////////


			//CString TCTab;
			//TCTab.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d|ҳ��:%d| %s|"),_T("ѡ�")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)isAble,pagecount,selChangeEvent);

			winInfo+=csTab;
			//cFile.Write(csTab,csTab.GetLength()*sizeof(TCHAR));
			//bool isFirstRadio=true;
			for(int j=0;j<pagecount;j++)
			{
				winInfo+=_T("\n");
				//cFile.Write(_T("\r\n"),2); //����
				TCITEM item;
				wchar_t buffer[256] = {0};
				item.pszText = buffer;
				item.cchTextMax = 256;
				item.mask = TCIF_TEXT;
				pTab->GetItem(j,&item);
				//�������Title
				CString csTitle=buffer;
				CString TabPageStart;
				TabPageStart.Format(_T("%s %s"),_T("ѡ�ҳ��"),csTitle);
				
				CString tabProperty;
				if(((CTCTabPage*)pTab->m_Pages[j])->TCGetBkgrColor()!=::GetSysColor(COLOR_BTNFACE))
					tabProperty = tabProperty+_T(" ")+_T("������ɫ:")+ColorToString(((CTCTabPage*)pTab->m_Pages[j])->TCGetBkgrColor())+_T("|");

				if(!((CTCTabPage*)pTab->m_Pages[j])->TCGetBackgroundImage().IsEmpty())
					tabProperty = tabProperty+_T(" ")+_T("����ͼƬ:")+((CTCTabPage*)pTab->m_Pages[j])->TCGetBackgroundImage()+_T("|");

				if(tabProperty.IsEmpty())
					TabPageStart+=_T(" |");
				else
					TabPageStart+=tabProperty;

				TabPageStart+=_T(" |");

				winInfo+=TabPageStart;
				//cFile.Write(TabPageStart,TabPageStart.GetLength()*sizeof(TCHAR)); //��ʾPAGESTART��ʼ
				//cFile.Write(_T("\r\n"),2); //����
				int ctlcount =((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll.GetSize();
				for(int k=0;k<ctlcount;k++)
				{
					CControlRect *ctl=((CTCTabPage*)pTab->m_Pages[j])->m_ControlTracker.m_arrRectanglesAll.GetAt(k);
					TCTOOLTYPE typeTab=ctl->GetCtlType();
					winInfo+=_T("\n");
					if(typeTab==TCTT_STATIC)
					{
						CTCStatic* pStatic=(CTCStatic *)ctl->GetCWnd();
						CString text;
						pStatic->GetWindowTextW(text);
						///ת���ַ�
						text.Replace(_T("\\"),_T("\\\\"));
						text.Replace(_T("\""),_T("\\\""));

						CString title;
						title.Format(_T("\"%s\""),text);

						RECT rc;
						pStatic->GetWindowRect(&rc);
						pStatic->GetParent()->ScreenToClient(&rc);

						CString csStatic;
						csStatic.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ǩ"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

						//����
						CString staticProp;
						CString szFont=pStatic->GetFontName();
						DWORD dwColor=pStatic->GetTextColor();
						DWORD dwBgColor=pStatic->GetBackground();
						int nSize=pStatic->GetFontSize();
						DWORD fontStyle=pStatic->GetFontStyle();

						if(dwBgColor!=::GetSysColor(COLOR_BTNFACE))
						{
							CString font;
							font.Format(_T("����:(%s,%d,%d,%d,%d)"),szFont,dwColor,dwBgColor,nSize,fontStyle);
							staticProp=staticProp+font+_T("|");
						}
						else
						{
							CString font;
							font.Format(_T("����:(%s,%d,FFFFFFFF,%d,%d)"),szFont,dwColor,nSize,fontStyle);
							if(font!=_T("����:(MS Sans Serif,0,FFFFFFFF,8,64)"))
								staticProp=staticProp+font+_T("|");
						}

						//CString font;
						//font.Format(_T("����:(%s,%d,%d,%d,%d)"),szFont,dwColor,dwBgColor,nSize,fontStyle);
						//CString csFont;
						//csFont.Format(_T("����:(MS Sans Serif,0,%d,8,64)"),::GetSysColor(COLOR_BTNFACE));
						//if(font!=csFont)
						//	staticProp=staticProp+font+_T("|");

						if(staticProp.IsEmpty())
							csStatic=csStatic+_T("|");
						else
							csStatic=csStatic+staticProp;

						//�¼�
						CString staticEvent;
						if(!pStatic->m_eClick.IsEmpty())
							staticEvent=staticEvent+_T("��굥��:")+pStatic->m_eClick+_T("|");

						if(staticEvent.IsEmpty())
							csStatic+=_T(" |");
						else
							csStatic=csStatic+_T(" ")+staticEvent;

						winInfo+=csStatic;
						//cFile.Write(csStatic,csStatic.GetLength()*sizeof(TCHAR));						
					}
					else if(typeTab==TCTT_GROUP)
					{
						CTCGroupBox* pGroup=(CTCGroupBox *)ctl->GetCWnd();

						CString text;
						pGroup->GetText(text);

						///ת���ַ�
						text.Replace(_T("\\"),_T("\\\\"));
						text.Replace(_T("\""),_T("\\\""));

						CString title;
						title.Format(_T("\"%s\""),text);

						RECT rc;
						pGroup->GetWindowRect(&rc);
						pGroup->GetParent()->ScreenToClient(&rc);
						CString csGroup;
						csGroup.Format(_T("%s %s %s %d,%d,%d,%d "),_T("�����"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

						//����
						CString groupProp;

						LOGFONT lf = pGroup->GetGroupFont();


						int nHeight=lf.lfHeight;
						int nWidth=lf.lfWidth;
						int nEscap=lf.lfEscapement;
						int nOrient=lf.lfOrientation;
						int nWeight=lf.lfWeight;

						BYTE nItalic=lf.lfItalic;
						BYTE nUnderlint=lf.lfUnderline;
						BYTE nStrikeOut=lf.lfStrikeOut;
						BYTE nCharSet=lf.lfCharSet;
						BYTE nOutPreci=lf.lfOutPrecision;
						BYTE nClipPreci=lf.lfClipPrecision;
						BYTE nQuality=lf.lfQuality;
						BYTE nPitch=lf.lfPitchAndFamily;

						CString szFont=lf.lfFaceName;

						CString fontGroup;
						fontGroup.Format(_T("����:(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s)"),nHeight,nWidth,nEscap,nOrient,nWeight,
							nItalic,nUnderlint,nStrikeOut,nCharSet,nOutPreci,nClipPreci,nQuality,nPitch,szFont);


						if(fontGroup!=_T("����:(-11,0,0,0,400,0,0,0,134,0,0,0,0,MS Shell Dlg 2)"))
							groupProp+=fontGroup+_T("|");

						//����
						if(pGroup->m_dwAlignment!=SS_LEFT)
						{
							CString csHelp;
							csHelp.Format(_T("����:%d|"),pGroup->m_dwAlignment);
							groupProp=groupProp+csHelp;
						}

						//�߿���ɫ
						if(pGroup->GetBorderColor()!=::GetSysColor(COLOR_3DSHADOW))
						{
							groupProp=groupProp+_T("�߿���ɫ:")+ColorToString(pGroup->GetBorderColor())+_T("|");
						}

						//���ⱳ����ɫ
						if(pGroup->GetTitleBackgroundColor()!=::GetSysColor(COLOR_BTNFACE))
						{
							groupProp=groupProp+_T("���ⱳ����ɫ:")+ColorToString(pGroup->GetTitleBackgroundColor())+_T("|");
						}

						//�ͻ�����ɫ
						if(pGroup->GetClientBackgroundColor()!=::GetSysColor(COLOR_BTNFACE))
						{
							groupProp=groupProp+_T("�ͻ�����ɫ:")+ColorToString(pGroup->GetClientBackgroundColor())+_T("|");
						}

						//������ɫ
						if(pGroup->GetTitleTextColor()!=::GetSysColor(COLOR_WINDOWTEXT))
						{
							groupProp=groupProp+_T("������ɫ:")+ColorToString(pGroup->GetTitleTextColor())+_T("|");
						}

						//�����ģʽ
						if(pGroup->GetGroupBoxStyle()!=CTCGroupBox::TCGB_FRAME)
						{
							groupProp=groupProp+_T("�����ģʽ:")+_T("1")+_T("|");
						}

						if(groupProp.IsEmpty())
							csGroup=csGroup+_T("|");
						else
							csGroup=csGroup+groupProp;

						csGroup=csGroup+_T(" |");
						winInfo+=csGroup;
						//cFile.Write(csGroup,csGroup.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_BUTTON)
					{
						CTCButton* pButton=(CTCButton *)ctl->GetCWnd();

						CString text;
						pButton->GetWindowTextW(text);

						///ת���ַ�
						text.Replace(_T("\\"),_T("\\\\"));
						text.Replace(_T("\""),_T("\\\""));

						CString title;
						title.Format(_T("\"%s\""),text);

						RECT rc;
						pButton->GetWindowRect(&rc);
						pButton->GetParent()->ScreenToClient(&rc);
						CString csButton;
						csButton.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ť"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

						//����
						CString buttonProp;

						if(!pButton->TCGetAbled())
						{
							CString csAbled;
							csAbled.Format(_T("��Ч:%d|"),pButton->TCGetAbled());
							buttonProp+=csAbled;
						}

						if(pButton->TCGetMulityLine())
						{
							CString csMulityLine;
							csMulityLine.Format(_T("����:%d|"),(int)pButton->TCGetMulityLine());
							buttonProp+=csMulityLine;
						}

						if(buttonProp.IsEmpty())
							csButton+=_T("|");
						else
							csButton+=buttonProp;



						/*BOOL isMulityLine=pButton->TCGetMulityLine();
						BOOL isAble=pButton->TCGetAbled();*/


						CString clickEvent;
						if(!pButton->TCGetClickEvent().IsEmpty())
							clickEvent.Format(_T("��굥��:%s"),pButton->TCGetClickEvent());

						if(clickEvent.IsEmpty())
							csButton+=_T(" |");
						else
							csButton+=_T(" ")+clickEvent+_T("|");


						/*CString TCButton;
						TCButton.Format(_T("%s %s %s %d,%d,%d,%d ��Ч:%d|����:%d| %s|"),_T("��ť"),ctl->GetCtlID(),title,rc.left,rc.top,
						rc.right,rc.bottom,(int)isAble,(int)isMulityLine,clickEvent);*/


						winInfo+=csButton;
						//cFile.Write(csButton,csButton.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_EDIT)
					{
						CTCEdit* pEdit=(CTCEdit *)ctl->GetCWnd();

						CString text;
						pEdit->GetWindowTextW(text);

						///ת���ַ�
						text.Replace(_T("\\"),_T("\\\\"));
						text.Replace(_T("\""),_T("\\\""));

						CString title;
						title.Format(_T("\"%s\""),text);

						RECT rc;
						pEdit->GetWindowRect(&rc);
						pEdit->GetParent()->ScreenToClient(&rc);

						CString csEdit;
						csEdit.Format(_T("%s %s %s %d,%d,%d,%d "),_T("�༭��"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

						CString csEditProp;
						bool isAble = pEdit->TCGetAbled();
						bool isMulityLine = pEdit->TCGetMulityLine();
						bool isPassWord = pEdit->TCGetPassWord();
						bool isReadOnly = pEdit->TCGetReadOnly();
						bool isNum = pEdit->TCGetNumber();
						bool isVerty = pEdit->TCGetVerti();
						bool isHoriz = pEdit->TCGetHoriz();

						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							csEditProp+=able;
						}

						if(isMulityLine)
						{
							CString mulityLine;
							mulityLine.Format(_T("����:%d|"),isMulityLine);
							csEditProp+=mulityLine;
						}

						if(isPassWord)
						{
							CString password;
							password.Format(_T("����:%d|"),isPassWord);
							csEditProp+=password;
						}

						if(isNum)
						{
							CString num;
							num.Format(_T("����:%d|"),isNum);
							csEditProp+=num;
						}

						if(isReadOnly)
						{
							CString readonly;
							readonly.Format(_T("ֻ��:%d|"),isReadOnly);
							csEditProp+=readonly;
						}

						if(isVerty)
						{
							CString verty;
							verty.Format(_T("��ֱ:%d|"),isVerty);
							csEditProp+=verty;
						}

						if(isHoriz)
						{
							CString horiz;
							horiz.Format(_T("ˮƽ:%d|"),isHoriz);
							csEditProp+=horiz;
						}

						if(pEdit->GetTextColor()!=::GetSysColor(COLOR_WINDOWTEXT))
						{
							csEditProp+=_T("�ı���ɫ:")+ColorToString(pEdit->GetTextColor())+_T("|");
						}

						if(pEdit->GetBackColor()!=RGB(255,255,255))
						{
							csEditProp+=_T("������ɫ:")+ColorToString(pEdit->GetBackColor())+_T("|");
						}

						if(csEditProp.IsEmpty())
							csEdit+=_T("|");
						else
							csEdit+=csEditProp;

						CString editEvent;
						if(!pEdit->TCGetEnChangeEvent().IsEmpty())
						{
							CString enChange;
							enChange.Format(_T("���ݸı�:%s|"),pEdit->TCGetEnChangeEvent());
							editEvent+=enChange;
						}

						if(!pEdit->TCGetSetFocusEvent().IsEmpty())
						{
							CString setFocus;
							setFocus.Format(_T("��ý���:%s|"),pEdit->TCGetSetFocusEvent());
							editEvent+=setFocus;
						}
						
						if(!pEdit->TCGetKillfocusEvent().IsEmpty())
						{
							CString killFocus;
							killFocus.Format(_T("ʧȥ����:%s|"),pEdit->TCGetKillfocusEvent());
							editEvent+=killFocus;
						}

						if(editEvent.IsEmpty())
							csEdit+=_T(" |");
						else
							csEdit+=_T(" ")+editEvent;


						winInfo+=csEdit;
						//cFile.Write(csEdit,csEdit.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_RADIO)
					{
						CTCRadio* pRadio=(CTCRadio *)ctl->GetCWnd();
						CString text;
						pRadio->GetWindowTextW(text);
						///ת���ַ�
						text.Replace(_T("\\"),_T("\\\\"));
						text.Replace(_T("\""),_T("\\\""));

						CString title;
						title.Format(_T("%s%s%s"),_T("\""),text,_T("\""));

						RECT rc;
						pRadio->GetWindowRect(&rc);
						pRadio->GetParent()->ScreenToClient(&rc);

						CString csRadio;
						csRadio.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ѡ��"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

						BOOL isCheck=pRadio->TCGetCheck();
						BOOL isTextLeft=pRadio->TCGetTextLeft();
						BOOL isAble=pRadio->TCGetAbled();
						BOOL isGroup=pRadio->TCGetGroup();	

						CString radioProp;
						if(isCheck)
						{
							CString check;
							check.Format(_T("�Ƿ�ѡ��:%d|"),(int)isCheck);
							radioProp+=check;
						}
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),(int)isAble);
							radioProp+=able;
						}
						if(isGroup)
						{
							CString group;
							group.Format(_T("����:%d|"),(int)isGroup);
							radioProp+=group;
						}
						if(isTextLeft)
						{
							CString textLeft;
							textLeft.Format(_T("�������:%d|"),(int)isTextLeft);
							radioProp+=textLeft;
						}

						if(radioProp.IsEmpty())
							csRadio+=_T("|");
						else
							csRadio+=radioProp;

						CString clickEvent;
						if(!pRadio->TCGetRadioEvent().IsEmpty())
						{
							clickEvent.Format(_T("�������:%s|"),pRadio->TCGetRadioEvent());
						}

						if(clickEvent.IsEmpty())
							csRadio+=_T(" |");
						else 
							csRadio+=_T(" ")+clickEvent;

						winInfo+=csRadio;
						//cFile.Write(csRadio,csRadio.GetLength()*sizeof(TCHAR));

					}
					else if(typeTab==TCTT_CHECK)
					{
						CTCCheck* pCheck=(CTCCheck *)ctl->GetCWnd();

						CString text;
						pCheck->GetWindowTextW(text);

						///ת���ַ�
						text.Replace(_T("\\"),_T("\\\\"));
						text.Replace(_T("\""),_T("\\\""));

						CString title;
						title.Format(_T("%s%s%s"),_T("\""),text,_T("\""));

						RECT rc;
						pCheck->GetWindowRect(&rc);
						pCheck->GetParent()->ScreenToClient(&rc);

						CString csCheck;
						csCheck.Format(_T("%s %s %s %d,%d,%d,%d "),_T("��ѡ��"),ctl->GetCtlID(),title,rc.left,rc.top,rc.right,rc.bottom);

						BOOL isCheck=pCheck->TCGetCheck();
						BOOL isTextLeft=pCheck->TCGetTextLeft();
						BOOL isAble=pCheck->TCGetAbled();

						CString checkProp;
						if(isCheck)
						{
							CString check;
							check.Format(_T("�Ƿ�ѡ��:%d|"),(int)isCheck);
							checkProp+=check;
						}
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),(int)isAble);
							checkProp+=able;
						}
						if(isTextLeft)
						{
							CString textLeft;
							textLeft.Format(_T("�������:%d|"),isTextLeft);
							checkProp+=textLeft;
						}

						if(checkProp.IsEmpty())
							csCheck+=_T("|");
						else 
							csCheck+=checkProp;

						CString clickEvent;
						if(!pCheck->TCGetCheckEvent().IsEmpty())
						{
							clickEvent.Format(_T("�������:%s|"),pCheck->TCGetCheckEvent());
						}

						if(clickEvent.IsEmpty())
							csCheck+=_T(" |");
						else
							csCheck+=_T(" ")+clickEvent;

						winInfo+=csCheck;
						//cFile.Write(csCheck,csCheck.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_COMBOBOX)
					{
						CTCComboBox* pCombox=(CTCComboBox *)ctl->GetCWnd();
						CString title;
						pCombox->GetWindowTextW(title);
						CString text;
						for(int i=0;i<pCombox->GetCount();i++)
						{
							CString node;
							pCombox->GetLBText(i,node);

							///ת���ַ�
							node.Replace(_T("\\"),_T("\\\\"));
							node.Replace(_T("\""),_T("\\\""));
							CString add;
							add.Format(_T("%s%s"),node,_T("|"));
							text+=add;
						}
						if(text.IsEmpty())
							text="\"\"";
						else
							text=_T("\"")+text+_T("\"");

						RECT rc;
						pCombox->GetWindowRect(&rc);
						pCombox->GetParent()->ScreenToClient(&rc);

						CString csCombox;
						csCombox.Format(_T("%s %s %s %d,%d,%d,%d "),_T("������"),ctl->GetCtlID(),text,rc.left,rc.top,rc.right,rc.bottom);

						bool isAble = pCombox->TCGetAbled();
						CString comboxProp;
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							comboxProp+=able;
						}

						if(comboxProp.IsEmpty())
							csCombox+=_T("|");
						else
							csCombox+=comboxProp;

						CString comEvent;
						if(!pCombox->GetLeftDownEvent().IsEmpty())
						{
							CString click;
							click.Format(_T("�������:%s|"),pCombox->GetLeftDownEvent());
							comEvent+=click;
						}
						if(!pCombox->GetEnChangeEvent().IsEmpty())
						{
							CString enchange;
							enchange.Format(_T("ѡ��ı�:%s|"),pCombox->GetEnChangeEvent());
							comEvent+=enchange;
						}

						if(comEvent.IsEmpty())
							csCombox+=_T(" |");
						else
							csCombox+=_T(" ")+comEvent;

						winInfo+=csCombox;
						//cFile.Write(csCombox,csCombox.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_LISTBOX)
					{
						CTCListBox* pList=(CTCListBox *)ctl->GetCWnd();
						CString title;
						CString titletext=pList->TCGetListBoxCaption();
						if(!titletext.IsEmpty())
						{
							///ת���ַ�
							titletext.Replace(_T("\\"),_T("\\\\"));
							titletext.Replace(_T("\""),_T("\\\""));
						}
						title.Format(_T("\"%s\""),titletext);
						CString text;
						for(int i=0;i<pList->GetCount();i++)
						{
							CString node;
							node = pList->GetItemText(i);

							///ת���ַ�
							node.Replace(_T("\\"),_T("\\\\"));
							node.Replace(_T("\""),_T("\\\""));
							CString add;
							add.Format(_T("%s%s"),node,_T("|"));
							//node=node+_T("\n");
							//add=add+_T(",");
							text+=add;
						}
						if(text.IsEmpty())
							text="\"\"";
						else
							text=_T("\"")+text+_T("\"");

						RECT rc;
						pList->GetWindowRect(&rc);
						pList->GetParent()->ScreenToClient(&rc);

						CString csList;
						csList.Format(_T("%s %s %s %s %d,%d,%d,%d "),_T("�б��"),ctl->GetCtlID(),title,text,rc.left,rc.top,rc.right,rc.bottom);

						bool isAble=pList->TCGetAbled();
						bool isDefaultRool=pList->TCGetDefaultRool();
						CString listProp;
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							listProp+=able;
						}
						if(isDefaultRool)
						{
							CString rool;
							rool.Format(_T("Ĭ�Ϲ���:%d|"),isDefaultRool);
							listProp+=rool;
						}

						if(listProp.IsEmpty())
							csList+=_T("|");
						else
							csList+=listProp;

						CString listEvent;
						if(!pList->TCGetListBoxDoubleClick().IsEmpty())
						{
							CString clicked;
							clicked.Format(_T("���˫��:%s|"),pList->TCGetListBoxDoubleClick());
							listEvent+=clicked;
						}
						if(!pList->TCGetListBoxSelChange().IsEmpty())
						{
							CString selChange;
							selChange.Format(_T("ѡ��ı�:%s|"),pList->TCGetListBoxSelChange());
							listEvent+=selChange;
						}

						if(listEvent.IsEmpty())
							csList+=_T(" |");
						else
							csList += _T(" ") + listEvent;

						winInfo+=csList;
						//cFile.Write(csList,csList.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_PROGRESS)
					{
						CTCProgress* pProgress=(CTCProgress *)ctl->GetCWnd();
						RECT rc;
						pProgress->GetWindowRect(&rc);
						pProgress->GetParent()->ScreenToClient(&rc);

						CString csProgress;
						csProgress.Format(_T("%s %s %d,%d,%d,%d "),_T("������"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

						bool isAble=pProgress->TCGetAbled();
						BOOL isVertical=pProgress->TCGetVertical();

						CString progressProp;
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							progressProp+=able;
						}
						if(isVertical)
						{
							CString verti;
							verti.Format(_T("��ֱ:%d|"),(int)isVertical);
							progressProp+=verti;
						}

						if(progressProp.IsEmpty())
							csProgress+=_T("|");
						else
							csProgress+=progressProp;

						csProgress+=_T(" |");

						//CString TCProgress;
						//TCProgress.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d|��ֱ:%d| |"),_T("������")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)isAble,(int)isVertical);

						winInfo+=csProgress;
						//cFile.Write(csProgress,csProgress.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_DATE)
					{
						CTCDate * pDate=(CTCDate *)ctl->GetCWnd();
						RECT rc;
						pDate->GetWindowRect(&rc);
						pDate->GetParent()->ScreenToClient(&rc);

						CString csDate;
						csDate.Format(_T("%s %s %d,%d,%d,%d "),_T("���ڿ�"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

						bool isAble = pDate->TCGetAbled();

						CString dateProp;
						if(isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							dateProp+=able;
						}
						if(dateProp.IsEmpty())
							csDate+=_T("|");
						else
							csDate+=dateProp;

						csDate+=_T(" |");

						//CString TCDate;
						//TCDate.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d| |"),_T("���ڿ�")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)isAble);

						winInfo+=csDate;
						//cFile.Write(csDate,csDate.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_SLIDER)
					{
						CTCSlider* pSlider=(CTCSlider *)ctl->GetCWnd();
						RECT rc;
						pSlider->GetWindowRect(&rc);
						pSlider->GetParent()->ScreenToClient(&rc);
						CString csSlider;
						csSlider.Format(_T("%s %s %d,%d,%d,%d "),_T("����"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

						BOOL vertical = pSlider->TCGetVertical();
						BOOL isAble = pSlider->TCGetAbled();

						CString sliderProp;
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							sliderProp+=able;
						}
						if(vertical)
						{
							CString verti;
							verti.Format(_T("��ֱ:%d|"),(int)vertical);
							sliderProp+=verti;
						}

						if(sliderProp.IsEmpty())
							csSlider+=_T("|");
						else
							csSlider+=sliderProp;

						CString sliderEvent;
						if(!pSlider->TCGetSliderChangeEvent().IsEmpty())
						{
							CString posChange;
							posChange.Format(_T("λ�øı�:%s|"),pSlider->TCGetSliderChangeEvent());
							sliderEvent+=posChange;
						}

						if(sliderEvent.IsEmpty())
							csSlider+=_T(" |");
						else
							csSlider+=_T(" ")+sliderEvent;

						//CString TCSlider;
						//TCSlider.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d|��ֱ:%d| %s|"),_T("����")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)isAble,(int)vertical,sliderEvent);

						winInfo+=csSlider;
						//cFile.Write(csSlider,csSlider.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_PICTURE)
					{
						CTCPicture* pPic=(CTCPicture *)ctl->GetCWnd();
						CString picName;
						picName=pPic->GetName();

						RECT rc;
						pPic->GetWindowRect(&rc);
						pPic->GetParent()->ScreenToClient(&rc);  

						CString csPicture;
						csPicture.Format(_T("%s %s %d,%d,%d,%d "),_T("ͼƬ��"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

						CString pictureProp;
						if(!picName.IsEmpty())
						{
							pictureProp+=_T("ͼƬ����:")+picName+_T("|");
						}

						if(pictureProp.IsEmpty())
							csPicture+=_T("|");
						else
							csPicture+=pictureProp;

						CString picsEvent;
						if(!pPic->TCGetClickEvent().IsEmpty())
						{
							CString click;
							click.Format(_T("�������:%s|"),pPic->TCGetClickEvent());
							picsEvent+=click;
						}

						if(picsEvent.IsEmpty())
							csPicture+=_T(" |");
						else
							csPicture+=_T(" ")+picsEvent;

						//CString TCPicture;
						//TCPicture.Format(_T("%s %s %d,%d,%d,%d ͼƬ����:%s| |"),_T("ͼƬ��")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,picName);

						winInfo+=csPicture;
						//cFile.Write(csPicture,csPicture.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_PATH)
					{
						CTCBrowse* pPath=(CTCBrowse *)ctl->GetCWnd();
						RECT rc;
						pPath->GetWindowRect(&rc);
						pPath->GetParent()->ScreenToClient(&rc);
						CString csPath;
						csPath.Format(_T("%s %s %d,%d,%d,%d "),_T("�����"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

						int mode=(int)pPath->TCGetMode();
						bool isAble=pPath->TCGetAbled();

						CString pathProp;
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							pathProp+=able;
						}
						if(mode!=0)
						{
							CString browType;
							browType.Format(_T("���ģʽ:%d|"),mode);
							pathProp+=browType;
						}

						if(pathProp.IsEmpty())
							csPath+=_T("|");
						else
							csPath+=pathProp;

						CString pathEvent;
						if(!pPath->TCGetEnChangeEvent().IsEmpty())
						{
							CString selChange;
							selChange.Format(_T("ѡ��ı�:%s|"),pPath->TCGetEnChangeEvent());
							pathEvent+=selChange;
						}

						if(pathEvent.IsEmpty())
							csPath+=_T(" |");
						else
							csPath+=_T(" ")+pathEvent;		

						//CString TCPath;
						//TCPath.Format(_T("%s %s %d,%d,%d,%d ��Ч:%d|���ģʽ:%d| %s|"),_T("�����")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,(int)able,mode,pathEvent);

						winInfo+=csPath;
						//cFile.Write(csPath,csPath.GetLength()*sizeof(TCHAR));
					}
					else if(typeTab==TCTT_HOTKEY)
					{
						CTCHotKeyCtrl* pHotKey=(CTCHotKeyCtrl *)ctl->GetCWnd();
						WORD wVirtualKey=0;
						WORD wMod=0;
						pHotKey->GetHotKeyValue(wVirtualKey,wMod);
						bool isAble = pHotKey->TCGetAbled();

						RECT rc;
						pHotKey->GetWindowRect(&rc);
						pHotKey->GetParent()->ScreenToClient(&rc);

						CString csHotKey;
						csHotKey.Format(_T("%s %s %d,%d,%d,%d "),_T("�ȼ�"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

						CString hotkeyProp;
						if(!isAble)
						{
							CString able;
							able.Format(_T("��Ч:%d|"),isAble);
							hotkeyProp+=able;
						}

						if(wVirtualKey!=0)
						{
							CString virtualkey;
							virtualkey.Format(_T("�����ֵ:%d|"),wVirtualKey);
							hotkeyProp+=virtualkey;
						}

						if(wMod!=0)
						{
							CString mod;
							mod.Format(_T("ģֵ̬:%d|"),wMod);
							hotkeyProp+=mod;
						}

						if(hotkeyProp.IsEmpty())
							csHotKey+=_T("|");
						else
							csHotKey+=hotkeyProp;

						CString hotkeyEvent;
						if(!pHotKey->TCGetHotKeyEvent().IsEmpty())
						{
							CString hotkey;
							hotkey.Format(_T("�ȼ�:%s|"),pHotKey->TCGetHotKeyEvent());
							hotkeyEvent+=hotkey;
						}
						if(!pHotKey->TCGetKillFocusEvent().IsEmpty())
						{
							CString killfocus;
							killfocus.Format(_T("ʧȥ����:%s|"),pHotKey->TCGetKillFocusEvent());
							hotkeyEvent+=killfocus;
						}

						if(hotkeyEvent.IsEmpty())
							csHotKey+=_T(" |");
						else
							csHotKey+=_T(" ")+hotkeyEvent;

						winInfo+=csHotKey;

						//cFile.Write(csHotKey,csHotKey.GetLength()*sizeof(TCHAR));
					}
					//else if(typeTab==TCTT_EXPLORER)
					//{
					//	CTCExplorer* pExplorer=(CTCExplorer *)ctl->GetCWnd();
					//	CString url = pExplorer->TCGetURL();
					//	CString loadComplete = pExplorer->TCGetloadComplete();

					//	RECT rc;
					//	pExplorer->GetWindowRect(&rc);
					//	pExplorer->GetParent()->ScreenToClient(&rc);

					//	CString csExplorer;
					//	csExplorer.Format(_T("%s %s %d,%d,%d,%d "),_T("�����"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

					//	CString explorerProp;
					//	if(!url.IsEmpty())
					//		explorerProp+=_T("URL:")+url+_T("|");

					//	if(explorerProp.IsEmpty())
					//		csExplorer+=_T("|");
					//	else
					//		csExplorer+=explorerProp;

					//	CString explorerEvet;
					//	if(!loadComplete.IsEmpty())
					//		explorerEvet+=_T("�������:")+loadComplete+_T("|");

					//	if(explorerEvet.IsEmpty())
					//		csExplorer+=_T(" |");
					//	else
					//		csExplorer+=_T(" ")+explorerEvet;

					//	//CString TCExplorer;
					//	//TCExplorer.Format(_T("%s %s %d,%d,%d,%d URL:%s| �������:%s|"),_T("�����")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,url,loadComplete);

					//	cFile.Write(csExplorer,csExplorer.GetLength()*sizeof(TCHAR));
					//}
					else if(typeTab==TCTT_GRIDCTRL)
					{
						CGridCtrl* pGridCtrl=(CGridCtrl *)ctl->GetCWnd();

						RECT rc;
						pGridCtrl->GetWindowRect(&rc);
						pGridCtrl->GetParent()->ScreenToClient(&rc);

						CString csGrid;
						csGrid.Format(_T("%s %s %d,%d,%d,%d "),_T("���"),ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom);

						CString gridProp;

						LOGFONT lf;
						CFont* font = pGridCtrl->GetFont();
						font->GetLogFont(&lf);

						int nHeight=lf.lfHeight;
						int nWidth=lf.lfWidth;
						int nEscap=lf.lfEscapement;
						int nOrient=lf.lfOrientation;
						int nWeight=lf.lfWeight;

						BYTE nItalic=lf.lfItalic;
						BYTE nUnderlint=lf.lfUnderline;
						BYTE nStrikeOut=lf.lfStrikeOut;
						BYTE nCharSet=lf.lfCharSet;
						BYTE nOutPreci=lf.lfOutPrecision;
						BYTE nClipPreci=lf.lfClipPrecision;
						BYTE nQuality=lf.lfQuality;
						BYTE nPitch=lf.lfPitchAndFamily;

						CString szFont=lf.lfFaceName;

						CString fontGrid;
						fontGrid.Format(_T("����:(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s)"),nHeight,nWidth,nEscap,nOrient,nWeight,
							nItalic,nUnderlint,nStrikeOut,nCharSet,nOutPreci,nClipPreci,nQuality,nPitch,szFont);

						if(fontGrid!=_T("����:(-11,0,0,0,400,0,0,0,134,0,0,0,0,MS Shell Dlg 2)"))
							gridProp+=fontGrid+_T("|");

						CString csRowCount,csColCount,csFixRowCount,csFixColCount;
						csRowCount.Format(_T("����:%d|"),pGridCtrl->GetRowCount());
						csColCount.Format(_T("����:%d|"),pGridCtrl->GetColumnCount());
						csFixRowCount.Format(_T("�̶�����:%d|"),pGridCtrl->GetFixedRowCount());
						csFixColCount.Format(_T("�̶�����:%d|"),pGridCtrl->GetFixedColumnCount());

						gridProp+=csRowCount+csColCount+csFixRowCount+csFixColCount;

						if(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetBackClr()!=RGB(0xFF, 0xFF, 0xE0))
							gridProp+=_T("������ɫ:")+ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetBackClr())+_T("|");
						if(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetTextClr()!=0)
							gridProp+=_T("�ı���ɫ:")+ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetTextClr())+_T("|");
						if(pGridCtrl->GetFixedBkColor()!=::GetSysColor(COLOR_BTNFACE))
							gridProp+=_T("�̶������ɫ:")+ColorToString(pGridCtrl->GetFixedBkColor())+_T("|");
						if(pGridCtrl->GetFixedTextColor()!=0)
							gridProp+=_T("�̶����ı���ɫ:")+ColorToString(pGridCtrl->GetFixedTextColor())+_T("|");
						
						if(!pGridCtrl->IsShowHorizLines())
						{
							CString horizLine;
							horizLine.Format(_T("%d"),(int)pGridCtrl->IsShowHorizLines());
							gridProp+=_T("ˮƽ����:")+horizLine+_T("|");
						}
						if(!pGridCtrl->IsShowVertiLines())
						{
							CString vertiLine;
							vertiLine.Format(_T("%d"),(int)pGridCtrl->IsShowVertiLines());
							gridProp+=_T("��ֱ����:")+vertiLine+_T("|");
						}
						if(!pGridCtrl->TCGetAllSellsEditAbled())
						{
							CString csEdit;
							csEdit.Format(_T("%d"),(int)pGridCtrl->TCGetAllSellsEditAbled());
							gridProp+=_T("�ɱ༭:")+csEdit+_T("|");
						}
						if(pGridCtrl->GetHeaderSort())
						{
							CString csHeadSort;
							csHeadSort.Format(_T("%d"),(int)pGridCtrl->GetHeaderSort());
							gridProp+=_T("����:")+csHeadSort+_T("|");
						}

						if(pGridCtrl->TCGetExpandToFit())
						{
							CString csExpand;
							csExpand.Format(_T("%d"),(int)pGridCtrl->TCGetExpandToFit());
							gridProp+=_T("���:")+csExpand+_T("|");
						}

						if(!pGridCtrl->IsSelectable())
						{
							CString csSelect;
							csSelect.Format(_T("%d"),(int)pGridCtrl->IsSelectable());
							gridProp+=_T("ѡ��:")+csSelect+_T("|");
						}
						if(!pGridCtrl->GetFixedRowSelection())
						{
							CString csFixRowSelect;
							csFixRowSelect.Format(_T("%d"),(int)pGridCtrl->GetFixedRowSelection());
							gridProp+=_T("�̶���ѡ��:")+csFixRowSelect+_T("|");
						}
						if(!pGridCtrl->GetFixedColumnSelection())
						{
							CString csFixColSelect;
							csFixColSelect.Format(_T("%d"),(int)pGridCtrl->GetFixedColumnSelection());
							gridProp+=_T("�̶���ѡ��:")+csFixColSelect+_T("|");
						}
						if(pGridCtrl->GetListMode())
						{
							CString csListMode;
							csListMode.Format(_T("%d"),(int)pGridCtrl->GetListMode());
							gridProp+=_T("�б�ģʽ:")+csListMode+_T("|");
						}

						CString colInfo;
						for(int col=0;col<pGridCtrl->GetColumnCount();col++)
						{
							CString node,width,type,readonly;
							node = pGridCtrl->GetItemText(0,col);
							width.Format(_T("%d"),pGridCtrl->GetColumnWidth(col));
							type.Format(_T("%d"),(int)pGridCtrl->GetItemData(0,col));
							readonly.Format(_T("%d"),(int)pGridCtrl->TCGetColReadInfo(0,col));
							///ת���ַ�
							node.Replace(_T("\\"),_T("\\\\"));
							node.Replace(_T("\""),_T("\\\""));
							if(node.IsEmpty())
								node="\"\"";
							else 
								node=_T("\"")+node+_T("\"");
							CString add;
							add.Format(_T("����:%s,ֻ��:%s,�п�:%s,����:%s%s"),node,readonly,width,type,_T(";"));
							colInfo+=add;
						}
						gridProp+=_T("������:")+colInfo+_T("|");

						//gridProp������Ϊ��
						csGrid+=gridProp;

						CString gridEvent;
						if(!pGridCtrl->TCGetClickEvent().IsEmpty())
							gridEvent+=_T("�������:")+pGridCtrl->TCGetClickEvent()+_T("|");
						if(!pGridCtrl->TCGetBeforeEditEvent().IsEmpty())
							gridEvent+=_T("�༭��ʼ:")+pGridCtrl->TCGetBeforeEditEvent()+_T("|");
						if(!pGridCtrl->TCGetAfterEditEvent().IsEmpty())
							gridEvent+=_T("�༭����:")+pGridCtrl->TCGetAfterEditEvent()+_T("|");
						if(!pGridCtrl->TCGetHeadSortEvent().IsEmpty())
							gridEvent+=_T("�������:")+pGridCtrl->TCGetHeadSortEvent()+_T("|");

						if(gridEvent.IsEmpty())
							csGrid+=_T(" |");
						else
							csGrid+=_T(" ")+gridEvent;

						//CString TCGridCtrl;
						//TCGridCtrl.Format(_T("%s %s %d,%d,%d,%d %s|����:%d|����:%d|�̶�����:%d|�̶�����:%d|������ɫ:%s|�ı���ɫ:%s|\
						//					 �̶������ɫ:%s|�̶����ı���ɫ:%s|ˮƽ����:%d|��ֱ����:%d|�ɱ༭:%d|����:%d|������:%s\
						//					 |���:%d|ѡ��:%d|�̶���ѡ��:%d|�̶���ѡ��:%d|�б�ģʽ:%d| �������:%s|�༭��ʼ:%s|�༭����:%s|"),
						//					 _T("���")/*(int)type*/,ctl->GetCtlID(),rc.left,rc.top,rc.right,rc.bottom,fontGrid,pGridCtrl->GetRowCount(),pGridCtrl->GetColumnCount()
						//					 ,pGridCtrl->GetFixedRowCount(),pGridCtrl->GetFixedColumnCount(),ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetBackClr())
						//					 ,ColorToString(pGridCtrl->GetDefaultCell(FALSE,FALSE)->GetTextClr()),ColorToString(pGridCtrl->GetFixedBkColor())
						//					 ,ColorToString(pGridCtrl->GetFixedTextColor()),(int)pGridCtrl->IsShowHorizLines(),(int)pGridCtrl->IsShowVertiLines()
						//					 ,(int)pGridCtrl->TCGetAllSellsEditAbled(),(int)pGridCtrl->GetHeaderSort(),colInfo,(int)pGridCtrl->TCGetExpandToFit()
						//					 ,(int)pGridCtrl->IsSelectable(),(int)pGridCtrl->GetFixedRowSelection(),(int)pGridCtrl->GetFixedColumnSelection(),(int)pGridCtrl->GetListMode()
						//					 ,pGridCtrl->TCGetClickEvent(),pGridCtrl->TCGetBeforeEditEvent(),pGridCtrl->TCGetAfterEditEvent());

						winInfo+=csGrid;
						//cFile.Write(csGrid,csGrid.GetLength()*sizeof(TCHAR));
					}
				}
				winInfo+=_T("\n");
				CString TabPageEnd;
				TabPageEnd.Format(_T("%s"),_T("ѡ�ҳ�����"));

				winInfo+=TabPageEnd;
				//cFile.Write(TabPageEnd,TabPageEnd.GetLength()*sizeof(TCHAR)); //��ʾPAGEEND�����ͻ���
			}
			CString tabInfo;
			tabInfo.Format(_T("%s"),_T("ѡ�����")/*,ctl->GetCtlID()*/);
			winInfo+=_T("\n");
			winInfo+=tabInfo;
			//cFile.Write(tabInfo,tabInfo.GetLength()*sizeof(TCHAR));
		}
	}
	winInfo+=_T("\n");
	winInfo+=_T("���ڽ���");

	node->winInfo = winInfo;

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.write();
}

void CDiaTCDesign::LoadTWinRCFileToMem(CString csWin,CString csCtrls)
{	
	TCLoadWinRCFile rcFile(csCtrls);

	bool ret=false;
	CString csLine;
	//�õ���һ��
	ret=rcFile.GetLineString(csLine);
	//�Ǵ�������
	CString csType=rcFile.GetCtrlType(csLine);

	//����ID
	CString csID=rcFile.GetCtrlID(csLine);

	//���ڱ���
	CString csCaption=rcFile.GetString(csLine);
	this->SetWindowTextW(csCaption);

	int xc = ::GetSystemMetrics(SM_CYCAPTION);
	int cx = ::GetSystemMetrics(SM_CXFRAME);
	int cy = ::GetSystemMetrics(SM_CYFRAME);

	//���ڴ�С
	RECT rt;
	rcFile.GetCtrlRECT(csLine,&rt);
	this->MoveWindow(0,0,rt.right-rt.left+2*cx,rt.bottom-rt.top+2*cy+xc);

	//�õ��ؼ�������
	rcFile.GetCtrlProporty(TCTT_TWIN,this,csLine);
	//�õ��¼�
	rcFile.GetCtrlEvent(TCTT_TWIN,this,csLine);
	this->TCSetBkgrColor(m_TcDlgConfig.m_BkgrColor);


	rcFile.GetLineString(csLine);
	csType=rcFile.GetCtrlType(csLine);

	while(csType!=_T("���ڽ���"))
	{
		if(csType==_T("��ǩ"))
		{
			csID=rcFile.GetCtrlID(csLine);
			csCaption=rcFile.GetString(csLine);
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCStatic* pStatic=new CTCStatic(); 
			pStatic->Create(csCaption, WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,rt,this);  
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pStatic->SetFont(pFont);
			rcFile.GetCtrlProporty(TCTT_STATIC,pStatic,csLine);
			rcFile.GetCtrlEvent(TCTT_STATIC,pStatic,csLine);

			CControlRect* rectStatic =new CControlRect(rt,pStatic,TCTT_STATIC,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectStatic);
		}
		else if(csType==_T("�����"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡ�������
			csCaption=rcFile.GetString(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			//��ȡ���������
			CTCGroupBox *pGroup=new CTCGroupBox(); 
			pGroup->Create(NULL,csCaption, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,this,65535,NULL);
			pGroup->SetText(csCaption);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			LOGFONT lg;
			pFont->GetLogFont(&lg);
			ASSERT (pFont->GetSafeHandle ());
			pGroup->SetFont(lg);

			rcFile.GetCtrlProporty(TCTT_GROUP,pGroup,csLine);//��ȡ���������
			//rcFile.GetCtrlEvent(TCTT_STATIC,pGroup,csLine); //��ȡ������¼���Ԥ����

			CControlRect *rectGroup =new CControlRect(rt,pGroup,TCTT_GROUP,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectGroup);
		}
		else if(csType==_T("��ť"))
		{
			bool isMulityLine,isAble;
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡ��ť��
			csCaption=rcFile.GetString(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCButton *pButton=new CTCButton(); 
			pButton->Create(csCaption,WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,rt,this,65535);
			//��ӵ�Button����������
			//this->DoAddButton(pButton);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pButton->SetFont(pFont);
			pButton->TCSetID(csID);

			rcFile.GetCtrlProporty(TCTT_BUTTON,pButton,csLine);//��ȡ��ť����
			rcFile.GetCtrlEvent(TCTT_BUTTON,pButton,csLine);//��ȡ��ť�¼�

			CControlRect *rectButton =new CControlRect(rt,pButton,TCTT_BUTTON,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectButton); 

		}
		else if(csType==_T("�༭��"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡ�༭����
			csCaption=rcFile.GetString(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCEdit *pEdit=new CTCEdit(); 
			//9-5����Զ���������
			pEdit->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,rt,this,65537);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pEdit->SetFont(pFont);
			pEdit->SetWindowTextW(csCaption);
			rcFile.GetCtrlProporty(TCTT_EDIT,pEdit,csLine);//��ȡ�༭������
			rcFile.GetCtrlEvent(TCTT_EDIT,pEdit,csLine); //��ȡ�༭���¼�

			CControlRect *rectEdit =new CControlRect(rt,pEdit,TCTT_EDIT,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectEdit);
		}
		else if(csType==_T("��ѡ��"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡ�������
			csCaption=rcFile.GetString(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCRadio *pRadio=new CTCRadio(); 
			pRadio->Create(csCaption,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS  ,rt,this,65532);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pRadio->SetFont(pFont);

			rcFile.GetCtrlProporty(TCTT_RADIO,pRadio,csLine);//��ȡ��ѡ������
			rcFile.GetCtrlEvent(TCTT_RADIO,pRadio,csLine); //��ȡ��ѡ���¼�

			CControlRect *rectRadio =new CControlRect(rt,pRadio,TCTT_RADIO,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectRadio);
		}
		else if(csType==_T("��ѡ��"))
		{
			bool isCheck=false,isTextLeft=false,isAble=false;
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡ��ѡ����
			csCaption=rcFile.GetString(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCCheck *pCheck=new CTCCheck(); 
			pCheck->Create(csCaption,WS_CHILD|WS_VISIBLE|BS_CHECKBOX|WS_CLIPSIBLINGS,rt,this,65533);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pCheck->SetFont(pFont);

			rcFile.GetCtrlProporty(TCTT_CHECK,pCheck,csLine);//��ȡ��ѡ������
			rcFile.GetCtrlEvent(TCTT_CHECK,pCheck,csLine); //��ȡ��ѡ���¼�

			CControlRect *rectCheck =new CControlRect(rt,pCheck,TCTT_CHECK,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectCheck);
		}
		else if(csType==_T("������"))
		{
			CStringArray Comboxstrarry;
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡ�û��Զ����ַ�
			rcFile.GetStrValue(csLine,Comboxstrarry);//������������б���û��Զ�����ַ�������
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCComboBox *pCombox=new CTCComboBox(); 
			pCombox->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,rt,this,65535);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pCombox->SetFont(pFont);

			for(int i=0;i<Comboxstrarry.GetCount();i++)
			{

				pCombox->AddString(Comboxstrarry[i]);
			}
			CControlRect *rectCombox =new CControlRect(rt,pCombox,TCTT_COMBOBOX,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectCombox);
			rcFile.GetCtrlProporty(TCTT_COMBOBOX,pCombox,csLine);//��ȡ����������
			rcFile.GetCtrlEvent(TCTT_COMBOBOX,pCombox,csLine); //��ȡ�������¼�

		}
		else if(csType==_T("�б��"))
		{
			CStringArray Liststrarry;
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡ�б����
			csCaption=rcFile.GetString(csLine);
			//Liststrarry
			rcFile.GetStrValue(csLine,Liststrarry);//������������б���û��Զ�����ַ�������
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCListBox *pListbox=new CTCListBox(); 
			pListbox->Create(csCaption,WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,rt,this,65535);
			pListbox->SetWindowTextW(csCaption);

			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pListbox->SetFont(pFont);

			CControlRect *rectList =new CControlRect(rt,pListbox,TCTT_LISTBOX,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectList);

			for(int i=0;i<Liststrarry.GetCount();i++)
			{
				pListbox->AddItem(Liststrarry[i]);
			}
			rcFile.GetCtrlProporty(TCTT_LISTBOX,pListbox,csLine);//��ȡ�б������
			rcFile.GetCtrlEvent(TCTT_LISTBOX,pListbox,csLine); //��ȡ�б���¼�
		}
		else if(csType==_T("������"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCProgress *pProgress=new CTCProgress(); 
			pProgress->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,this,65537);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pProgress->SetFont(pFont);

			rcFile.GetCtrlProporty(TCTT_PROGRESS,pProgress,csLine);//��ȡ����������
			pProgress->SetPos(70);

			CControlRect *rectProgress =new CControlRect(rt,pProgress,TCTT_PROGRESS,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectProgress);
		}
		else if(csType==_T("���ڿ�"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCDate* pDate=new CTCDate(); 
			pDate->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,this,65535);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pDate->SetFont(pFont);

			rcFile.GetCtrlProporty(TCTT_DATE,pDate,csLine);//��ȡ���ڿ�����

			CControlRect *rectDate =new CControlRect(rt,pDate,TCTT_DATE,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectDate);
		}
		else if(csType==_T("����"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCSlider *pSlider=new CTCSlider(); 
			pSlider->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,this,65539);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pSlider->SetFont(pFont);
			rcFile.GetCtrlProporty(TCTT_SLIDER,pSlider,csLine);//��ȡ��������
			rcFile.GetCtrlEvent(TCTT_SLIDER,pSlider,csLine); //��ȡ�����¼�
			CControlRect *rectSlider =new CControlRect(rt,pSlider,TCTT_SLIDER,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectSlider);
		}
		else if(csType==_T("ͼƬ��"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCPicture *pPic=new CTCPicture(); 
			pPic->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|WS_BORDER|SS_NOTIFY|WS_CLIPSIBLINGS,rt,this,65535);
			//���ؿؼ���ʱ���Ҫ����ͼƬ,���������ж�ͼƬ�����Ƿ�Ϊ��//
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pPic->SetFont(pFont);
			rcFile.GetCtrlProporty(TCTT_PICTURE,pPic,csLine);//��ȡͼƬ������
			rcFile.GetCtrlEvent(TCTT_PICTURE,pPic,csLine);
			CControlRect *rectPic =new CControlRect(rt,pPic,TCTT_PICTURE,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectPic);
		}
		else if(csType==_T("�����"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCBrowse *pPath=new CTCBrowse(); 
			pPath->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,rt,this,655311);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pPath->SetFont(pFont);
			rcFile.GetCtrlProporty(TCTT_PATH,pPath,csLine);//��ȡ���������
			rcFile.GetCtrlEvent(TCTT_PATH,pPath,csLine); //��ȡ������¼�
			CControlRect *rectPath =new CControlRect(rt,pPath,TCTT_PATH,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectPath);
		}
		else if(csType==_T("ʱ��"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCTimer *pTimer=new CTCTimer(); 
			pTimer->Create(_T("ʱ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_NOTIFY|SS_REALSIZEIMAGE|WS_CLIPSIBLINGS,rt,this,65535);
			//pTimer->m_bmp.LoadBitmapW(IDB_BITMAP_TIMER);
			pTimer->SetBitmap(pTimer->m_bmp);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pTimer->SetFont(pFont);

			rcFile.GetCtrlProporty(TCTT_TIMER,pTimer,csLine);//��ȡ���������
			rcFile.GetCtrlEvent(TCTT_TIMER,pTimer,csLine); //��ȡ������¼�

			CControlRect *rectTimer =new CControlRect(rt,pTimer,TCTT_TIMER,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectTimer);

		}
		else if(csType==_T("�ȼ�"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCHotKeyCtrl* pHotKey = new CTCHotKeyCtrl();
			pHotKey->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,this,65535);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}

			}
			ASSERT (pFont->GetSafeHandle ());
			pHotKey->SetFont(pFont);
			CControlRect *rectHotKey =new CControlRect(rt,pHotKey,TCTT_HOTKEY,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectHotKey);

			rcFile.GetCtrlProporty(TCTT_HOTKEY,pHotKey,csLine);//��ȡ�ȼ�����
			pHotKey->SetHotKey(pHotKey->m_wVirtualKeyCode,pHotKey->m_wModifiers);

			rcFile.GetCtrlEvent(TCTT_HOTKEY,pHotKey,csLine); //��ȡ�ȼ��¼�
		}
		else if(csType==_T("�����"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);

			CTCExplorer* pExplorer=new CTCExplorer(); 
			pExplorer->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,rt,this,65535);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pExplorer->SetFont(pFont);

			rcFile.GetCtrlProporty(TCTT_EXPLORER,pExplorer,csLine);//��ȡ���������
			rcFile.GetCtrlEvent(TCTT_EXPLORER,pExplorer,csLine); //��ȡ������¼�
			CControlRect *rectExploer =new CControlRect(rt,pExplorer,TCTT_EXPLORER,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectExploer);
		}
		else if(csType==_T("���"))
		{
			csID=rcFile.GetCtrlID(csLine);
			rcFile.GetCtrlRECT(csLine,&rt);
			CGridCtrl* pGridCtrl=new CGridCtrl(); 
			pGridCtrl->Create(rt,this,65535);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pGridCtrl->SetFont(pFont);
			//pGridCtrl->EnableDragAndDrop(TRUE);
			pGridCtrl->GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
			pGridCtrl->SetVirtualMode(FALSE);
			pGridCtrl->SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));
			rcFile.GetCtrlProporty(TCTT_GRIDCTRL,pGridCtrl,csLine);
			if(!pGridCtrl->TCGetExpandToFit())
			{
				for(int row = 0;row < pGridCtrl->GetRowCount(); row++)
				{
					pGridCtrl->SetRowHeight(row,25);
				}
			}	
			rcFile.GetCtrlEvent(TCTT_GRIDCTRL,pGridCtrl,csLine);

			CControlRect* rectGridCtrl =new CControlRect(rt,pGridCtrl,TCTT_GRIDCTRL,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectGridCtrl);
		}
		else if(csType==_T("ѡ�"))
		{
			//��ȡID
			csID=rcFile.GetCtrlID(csLine);
			//��ȡλ��
			rcFile.GetCtrlRECT(csLine,&rt);
			CTCTab *pTab=new CTCTab(); 
			pTab->Create(WS_VISIBLE|TCS_MULTILINE|TCS_TABS|WS_CLIPSIBLINGS,rt,this,65535);
			CFont* pFont = GetFont();
			if (!pFont) {
				HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
				if (NULL == hFont) {
					hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
				}
				if (hFont) {pFont = CFont::FromHandle(hFont);}
			}
			ASSERT (pFont->GetSafeHandle ());
			pTab->SetFont(pFont);
			//��ȡ����
			rcFile.GetCtrlProporty(TCTT_TAB,pTab,csLine);
			rcFile.GetCtrlEvent(TCTT_TAB,pTab,csLine);

			CControlRect *rectTab =new CControlRect(rt,pTab,TCTT_TAB,csID,TCPARENT_DESDIN,this);
			m_ControlTracker.JustAdd(rectTab);
			int i=0;
			while(csLine!=_T("ѡ�����"))
			{
				rcFile.GetLineString(csLine);
				if(csLine==_T("ѡ�����"))
					break;
				rcFile.GetCtrlType(csLine);
				CString pageTitle = rcFile.GetTabText(csLine);
				//CString pageTitle=csLine;
				pTab->addPage(pageTitle);
				CString name,value;
				while(rcFile.GetNextProporty(csLine,name,value))
				{
					if(name==_T("������ɫ"))
					{
						((CTCTabPage*)pTab->m_Pages[i])->TCSetBkgrColor(_wtoi(value));
					}
					else if(name==_T("����ͼƬ"))
					{
						((CTCTabPage*)pTab->m_Pages[i])->TCSetBackgroundImage(value);
					}
				}
				while(csLine!=_T("ѡ�ҳ�����"))
				{
					rcFile.GetLineString(csLine);
					if(csLine==_T("ѡ�ҳ�����"))
						break;
					//Table�пؼ�����
					CString TCCtlType=rcFile.GetCtrlType(csLine);

					if(TCCtlType==_T("��ǩ"))
					{
						csID=rcFile.GetCtrlID(csLine);
						csCaption=rcFile.GetString(csLine);
						rcFile.GetCtrlRECT(csLine,&rt);
						CTCStatic* pStatic=new CTCStatic(); 
						pStatic->Create(csCaption, WS_CHILD|WS_VISIBLE|SS_NOTIFY|WS_CLIPSIBLINGS ,rt,pTab->m_Pages[i]);  
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pStatic->SetFont(pFont);
						rcFile.GetCtrlProporty(TCTT_STATIC,pStatic,csLine);//��ȡ��ǩ����
						rcFile.GetCtrlEvent(TCTT_STATIC,pStatic,csLine); //��ȡ��ǩ�¼�
						CControlRect* rectStatic =new CControlRect(rt,pStatic,TCTT_STATIC,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectStatic);
					}
					else if(TCCtlType==_T("�����"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡ�������
						csCaption=rcFile.GetString(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						//��ȡ���������
						CTCGroupBox *pGroup=new CTCGroupBox(); 
						pGroup->Create(NULL,csCaption, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65535,NULL);
						pGroup->SetText(csCaption);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						LOGFONT lg;
						pFont->GetLogFont(&lg);
						ASSERT (pFont->GetSafeHandle ());
						pGroup->SetFont(lg);

						rcFile.GetCtrlProporty(TCTT_GROUP,pGroup,csLine);//��ȡ���������
						//rcFile.GetCtrlEvent(TCTT_STATIC,pGroup,csLine); //��ȡ������¼���Ԥ����

						CControlRect *rectGroup =new CControlRect(rt,pGroup,TCTT_GROUP,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectGroup);
					}
					else if(TCCtlType==_T("��ť"))
					{
						bool isMulityLine,isAble;
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡ��ť��
						csCaption=rcFile.GetString(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);
						CTCButton *pButton=new CTCButton(); 
						pButton->Create(csCaption,WS_CHILD|WS_VISIBLE|BS_NOTIFY|BS_CENTER|WS_CLIPSIBLINGS ,rt,pTab->m_Pages[i],65535);
						//��ӵ�Button����������
						//this->DoAddButton(pButton);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pButton->SetFont(pFont);
						pButton->TCSetID(csID);

						rcFile.GetCtrlProporty(TCTT_BUTTON,pButton,csLine);//��ȡ��ť����
						rcFile.GetCtrlEvent(TCTT_BUTTON,pButton,csLine);//��ȡ��ť�¼�

						CControlRect *rectButton =new CControlRect(rt,pButton,TCTT_BUTTON,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectButton); 

					}
					else if(TCCtlType==_T("�༭��"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡ�༭����
						csCaption=rcFile.GetString(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);
						CTCEdit *pEdit=new CTCEdit(); 
						//9-5����Զ���������
						pEdit->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_WANTRETURN |WS_CLIPSIBLINGS ,rt,pTab->m_Pages[i],65537);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pEdit->SetFont(pFont);
						pEdit->SetWindowTextW(csCaption);
						rcFile.GetCtrlProporty(TCTT_EDIT,pEdit,csLine);//��ȡ�༭������
						rcFile.GetCtrlEvent(TCTT_EDIT,pEdit,csLine); //��ȡ�༭���¼�

						CControlRect *rectEdit =new CControlRect(rt,pEdit,TCTT_EDIT,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectEdit);
					}
					else if(TCCtlType==_T("��ѡ��"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡ�������
						csCaption=rcFile.GetString(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);
						CTCRadio *pRadio=new CTCRadio(); 
						pRadio->Create(csCaption,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|WS_CLIPSIBLINGS ,rt,pTab->m_Pages[i],65532);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pRadio->SetFont(pFont);

						rcFile.GetCtrlProporty(TCTT_RADIO,pRadio,csLine);//��ȡ��ѡ������
						rcFile.GetCtrlEvent(TCTT_RADIO,pRadio,csLine); //��ȡ��ѡ���¼�

						CControlRect *rectRadio =new CControlRect(rt,pRadio,TCTT_RADIO,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectRadio);
					}
					else if(TCCtlType==_T("��ѡ��"))
					{
						bool isCheck=false,isTextLeft=false,isAble=false;
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡ��ѡ����
						csCaption=rcFile.GetString(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);
						CTCCheck *pCheck=new CTCCheck(); 
						pCheck->Create(csCaption,WS_CHILD|WS_VISIBLE|BS_CHECKBOX|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65533);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pCheck->SetFont(pFont);

						rcFile.GetCtrlProporty(TCTT_CHECK,pCheck,csLine);//��ȡ��ѡ������
						rcFile.GetCtrlEvent(TCTT_CHECK,pCheck,csLine); //��ȡ��ѡ���¼�

						CControlRect *rectCheck =new CControlRect(rt,pCheck,TCTT_CHECK,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectCheck);
					}
					else if(TCCtlType==_T("������"))
					{
						CStringArray Comboxstrarry;
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡ�û��Զ����ַ�
						rcFile.GetStrValue(csLine,Comboxstrarry);//������������б���û��Զ�����ַ�������
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCComboBox *pCombox=new CTCComboBox(); 
						pCombox->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65535);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pCombox->SetFont(pFont);

						for(int i=0;i<Comboxstrarry.GetCount();i++)
						{

							pCombox->AddString(Comboxstrarry[i]);
						}
						CControlRect *rectCombox =new CControlRect(rt,pCombox,TCTT_COMBOBOX,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectCombox);

						rcFile.GetCtrlProporty(TCTT_COMBOBOX,pCombox,csLine);//��ȡ����������
						rcFile.GetCtrlEvent(TCTT_COMBOBOX,pCombox,csLine); //��ȡ�������¼�

					}
					else if(TCCtlType==_T("�б��"))
					{
						CStringArray Liststrarry;
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡ�б����
						csCaption=rcFile.GetString(csLine);
						//Liststrarry
						rcFile.GetStrValue(csLine,Liststrarry);//������������б���û��Զ�����ַ�������
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCListBox *pListbox=new CTCListBox(); 
						pListbox->Create(csCaption,WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_OWNERDRAWVARIABLE|SS_NOTIFY|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65535);
						pListbox->SetWindowTextW(csCaption);

						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pListbox->SetFont(pFont);

						CControlRect *rectList =new CControlRect(rt,pListbox,TCTT_LISTBOX,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectList);

						for(int i=0;i<Liststrarry.GetCount();i++)
						{
							pListbox->AddItem(Liststrarry[i]);
						}
						rcFile.GetCtrlProporty(TCTT_LISTBOX,pListbox,csLine);//��ȡ�б������
						rcFile.GetCtrlEvent(TCTT_LISTBOX,pListbox,csLine); //��ȡ�б���¼�
					}
					else if(TCCtlType==_T("������"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCProgress *pProgress=new CTCProgress(); 
						pProgress->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65537);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pProgress->SetFont(pFont);

						rcFile.GetCtrlProporty(TCTT_PROGRESS,pProgress,csLine);//��ȡ����������
						pProgress->SetPos(70);

						CControlRect *rectProgress =new CControlRect(rt,pProgress,TCTT_PROGRESS,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectProgress);
					}
					else if(TCCtlType==_T("���ڿ�"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);
						CTCDate* pDate=new CTCDate(); 
						pDate->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65535);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pDate->SetFont(pFont);

						rcFile.GetCtrlProporty(TCTT_DATE,pDate,csLine);//��ȡ���ڿ�����

						CControlRect *rectDate =new CControlRect(rt,pDate,TCTT_DATE,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectDate);
					}
					else if(TCCtlType==_T("����"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCSlider *pSlider=new CTCSlider(); 
						pSlider->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65539);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pSlider->SetFont(pFont);
						rcFile.GetCtrlProporty(TCTT_SLIDER,pSlider,csLine);//��ȡ��������
						rcFile.GetCtrlEvent(TCTT_SLIDER,pSlider,csLine); //��ȡ�����¼�
						CControlRect *rectSlider =new CControlRect(rt,pSlider,TCTT_SLIDER,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectSlider);
					}
					else if(TCCtlType==_T("ͼƬ��"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCPicture *pPic=new CTCPicture(); 
						pPic->Create(_T("ͼƬ��"),WS_CHILD|WS_VISIBLE|SS_BITMAP|WS_BORDER|SS_NOTIFY|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65535);
						//���ؿؼ���ʱ���Ҫ����ͼƬ,���������ж�ͼƬ�����Ƿ�Ϊ��//
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pPic->SetFont(pFont);
						rcFile.GetCtrlProporty(TCTT_PICTURE,pPic,csLine);//��ȡͼƬ������
						rcFile.GetCtrlEvent(TCTT_PICTURE,pPic,csLine); //��ȡͼƬ���¼�
						CControlRect *rectPic =new CControlRect(rt,pPic,TCTT_PICTURE,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectPic);
					}
					else if(TCCtlType==_T("�����"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCBrowse *pPath=new CTCBrowse(); 
						pPath->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|0x80|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],655311);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pPath->SetFont(pFont);
						rcFile.GetCtrlProporty(TCTT_PATH,pPath,csLine);//��ȡ���������
						rcFile.GetCtrlEvent(TCTT_PATH,pPath,csLine); //��ȡ������¼�
						CControlRect *rectPath =new CControlRect(rt,pPath,TCTT_PATH,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectPath);
					}
					else if(TCCtlType==_T("�ȼ�"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCHotKeyCtrl* pHotKey = new CTCHotKeyCtrl();
						pHotKey->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65535);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}

						}
						ASSERT (pFont->GetSafeHandle ());
						pHotKey->SetFont(pFont);
						CControlRect *rectHotKey =new CControlRect(rt,pHotKey,TCTT_HOTKEY,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectHotKey);

						rcFile.GetCtrlProporty(TCTT_HOTKEY,pHotKey,csLine);//��ȡ�ȼ�����
						pHotKey->SetHotKey(pHotKey->m_wVirtualKeyCode,pHotKey->m_wModifiers);

						rcFile.GetCtrlEvent(TCTT_HOTKEY,pHotKey,csLine); //��ȡ�ȼ��¼�
					}
					else if(TCCtlType==_T("�����"))
					{
						//��ȡID
						csID=rcFile.GetCtrlID(csLine);
						//��ȡλ��
						rcFile.GetCtrlRECT(csLine,&rt);

						CTCExplorer* pExplorer=new CTCExplorer(); 
						pExplorer->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|WS_CLIPSIBLINGS,rt,pTab->m_Pages[i],65535);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pExplorer->SetFont(pFont);

						rcFile.GetCtrlProporty(TCTT_EXPLORER,pExplorer,csLine);//��ȡ���������
						rcFile.GetCtrlEvent(TCTT_EXPLORER,pExplorer,csLine); //��ȡ������¼�
						CControlRect *rectExploer =new CControlRect(rt,pExplorer,TCTT_EXPLORER,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectExploer);
					}
					else if(TCCtlType==_T("���"))
					{
						csID=rcFile.GetCtrlID(csLine);
						rcFile.GetCtrlRECT(csLine,&rt);
						CGridCtrl* pGridCtrl=new CGridCtrl(); 
						pGridCtrl->Create(rt,pTab->m_Pages[i],65535);
						CFont* pFont = GetFont();
						if (!pFont) {
							HFONT hFont = (HFONT)GetStockObject (DEFAULT_GUI_FONT);
							if (NULL == hFont) {
								hFont = (HFONT) GetStockObject (ANSI_VAR_FONT);
							}
							if (hFont) {pFont = CFont::FromHandle(hFont);}
						}
						ASSERT (pFont->GetSafeHandle ());
						pGridCtrl->SetFont(pFont);
						//pGridCtrl->EnableDragAndDrop(TRUE);
						pGridCtrl->GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
						pGridCtrl->SetVirtualMode(FALSE);
						pGridCtrl->SetFixedBkColor(::GetSysColor(COLOR_BTNFACE));
						rcFile.GetCtrlProporty(TCTT_GRIDCTRL,pGridCtrl,csLine);
						if(!pGridCtrl->TCGetExpandToFit())
						{
							for(int row = 0;row < pGridCtrl->GetRowCount(); row++)
							{
								pGridCtrl->SetRowHeight(row,25);
							}
						}	
						rcFile.GetCtrlEvent(TCTT_GRIDCTRL,pGridCtrl,csLine);

						CControlRect* rectGridCtrl =new CControlRect(rt,pGridCtrl,TCTT_GRIDCTRL,csID,TCPARENT_TABPAGE,pTab->m_Pages[i]);
						((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.JustAdd(rectGridCtrl);
					}
				}
				i++;
			}
			//���������Ӵ������Ӧ���Ӵ���
			pTab->TCAdjustRect();
			pTab->SetCurSel(pTab->getTabPageCount()-1);
			pTab->showIndex(pTab->getTabPageCount()-1);
		}
		rcFile.GetLineString(csLine);
		csType=rcFile.GetCtrlType(csLine);
	}
}

void CDiaTCDesign::TCSetBkgrColor(COLORREF color)
{
	m_TcDlgConfig.m_BkgrColor=color;
	this->SetBackgroundColor(color);
}

void CDiaTCDesign::TCSetBackgroundImage(CString bmp,BackgroundLocation location)
{
	CString bmpPath;
	bmpPath.Format(_T("%s\\��Դ\\%s"),CTCRunTimeManager::g_ProjSourcePath,bmp);

	m_ImageLoader.Load(bmpPath);
	if(!m_ImageLoader.IsNull())
	{
		m_TcDlgConfig.m_BkgrBitmap=bmp;
		HBITMAP bitmap = m_ImageLoader.Detach();
		this->SetBackgroundImage(bitmap,location);
	}
	else
	{
		m_TcDlgConfig.m_BkgrBitmap=_T("");
		this->SetBackgroundImage((HBITMAP)NULL);
	}
}

/////////////////������ͼ������//////////////
void CDiaTCDesign::TCSetIcon(CString ico)
{
	ico.Trim();
	if(!ico.IsEmpty())
		m_TcDlgConfig.m_icon = ico;
	else
		m_TcDlgConfig.m_icon = _T("");
}

void CDiaTCDesign::TCSetMaxButton(bool set)
{
	m_TcDlgConfig.m_maxBtn = set;
}

void CDiaTCDesign::TCSetMinButton(bool set)
{
	m_TcDlgConfig.m_minBtn = set;
}

BOOL CDiaTCDesign::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString exeIcon;
	::GetPrivateProfileString(_T("ProjInfo"),_T("ExeIco"),_T(""),exeIcon.GetBuffer(MAX_PATH),MAX_PATH,CTCRunTimeManager::g_ProjFilePath);   //����ȡ�����ļ�
	exeIcon.ReleaseBuffer(MAX_PATH);
	m_TcDlgConfig.m_icon = exeIcon;

	//LoadTWinRCFile();
	m_undoManager.Clear();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



//9-3����Ӹı�������ʱָ��������ֵ���Ÿı�
void CDiaTCDesign::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//TODO: �ڴ˴������Ϣ����������
	CRect rect;
	this->GetWindowRect(&rect);
	int width = rect.Width();
	int height = rect.Height();
	CString strWidth = _T("");
	CString strHeight = _T("");
	strWidth.Format(_T("%d"),width);
	strHeight.Format(_T("%d"),height);
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTheProperty(TCPROPERTYTYPE::TCPROPERTY_TWIN,
		m_TcDlgConfig.m_dlgID,DWORD_PTR(_T("���ڿ��")),strWidth);
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetTheProperty(TCPROPERTYTYPE::TCPROPERTY_TWIN,
		m_TcDlgConfig.m_dlgID,DWORD_PTR(_T("���ڸ߶�")),strHeight);

}
