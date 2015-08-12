// FormToolBox.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "FormToolBox.h"
#include "afxdialogex.h"

// CFormToolBox 对话框

IMPLEMENT_DYNAMIC(CFormToolBox, CDialogEx)

CFormToolBox::CFormToolBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFormToolBox::IDD, pParent)
{
    m_SelectToolType=TCTT_MAX;
}

CFormToolBox::~CFormToolBox()
{
}

void CFormToolBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTONDEFAULT, m_btnDefault);
	DDX_Control(pDX, IDC_BUTTONSTATIC, m_btnStatic);
	DDX_Control(pDX, IDC_BUTTONBTN, m_btnBtn);
	DDX_Control(pDX, IDC_BUTTONRADIO, m_btnRadio);
	DDX_Control(pDX, IDC_BUTTONCHECK, m_btnCheck);
	DDX_Control(pDX, IDC_BUTTONEDTI, m_btnEdit);
	DDX_Control(pDX, IDC_BUTTONGROUP, m_btnGroup);
	DDX_Control(pDX, IDC_BUTTONDATE, m_btnDate);
	DDX_Control(pDX, IDC_BUTTONPROGRESS, m_btnProgress);
	DDX_Control(pDX, IDC_BUTTONPICTURE, m_btnPicture);
	DDX_Control(pDX, IDC_BUTTONCOMBOBOX, m_btnComboBox);
	DDX_Control(pDX, IDC_BUTTONLISTBOX, m_btnListbox);
	DDX_Control(pDX, IDC_BUTTONSLIDER, m_btnSlider);
	DDX_Control(pDX, IDC_BUTTONPATH, m_btnPath);
	DDX_Control(pDX, IDC_BUTTONTAB, m_btnTab);
	DDX_Control(pDX, IDC_BUTTONTIMER, m_btnTimer);
	DDX_Control(pDX, IDC_BUTTONHOTKEY, m_btnHotkey);
	DDX_Control(pDX, IDC_BUTTONEXPLORER, m_btnExplorer);

	DDX_Control(pDX, IDC_BUTTONGRIDCTRL, m_btnGrid);
}


BEGIN_MESSAGE_MAP(CFormToolBox, CDialogEx)
    ON_WM_VSCROLL()
    ON_BN_CLICKED(IDC_BUTTONSTATIC, &CFormToolBox::OnBnClickedButtonstatic)
    ON_BN_CLICKED(IDC_BUTTONDEFAULT, &CFormToolBox::OnBnClickedButtondefault)
    ON_BN_CLICKED(IDC_BUTTONBTN, &CFormToolBox::OnBnClickedButtonbtn)
    ON_BN_CLICKED(IDC_BUTTONRADIO, &CFormToolBox::OnBnClickedButtonradio)
    ON_BN_CLICKED(IDC_BUTTONCHECK, &CFormToolBox::OnBnClickedButtoncheck)
    ON_BN_CLICKED(IDC_BUTTONEDTI, &CFormToolBox::OnBnClickedButtonedti)
    ON_BN_CLICKED(IDC_BUTTONGROUP, &CFormToolBox::OnBnClickedButtongroup)
    //ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTONDATE, &CFormToolBox::OnBnClickedButtondate)
    ON_BN_CLICKED(IDC_BUTTONPROGRESS, &CFormToolBox::OnBnClickedButtonprogress)
    ON_BN_CLICKED(IDC_BUTTONPICTURE, &CFormToolBox::OnBnClickedButtonpicture)
    ON_BN_CLICKED(IDC_BUTTONCOMBOBOX, &CFormToolBox::OnBnClickedButtoncombobox)
    ON_BN_CLICKED(IDC_BUTTONLISTBOX, &CFormToolBox::OnBnClickedButtonlistbox)
    ON_BN_CLICKED(IDC_BUTTONSLIDER, &CFormToolBox::OnBnClickedButtonslider)
    ON_BN_CLICKED(IDC_BUTTONPATH, &CFormToolBox::OnBnClickedButtonpath)
	ON_BN_CLICKED(IDC_BUTTONTAB, &CFormToolBox::OnBnClickedButtontab)
	ON_BN_CLICKED(IDC_BUTTONTIMER, &CFormToolBox::OnBnClickedButtontimer)
	ON_BN_CLICKED(IDC_BUTTONHOTKEY, &CFormToolBox::OnBnClickedButtonhotkey)
	ON_BN_CLICKED(IDC_BUTTONEXPLORER, &CFormToolBox::OnBnClickedButtonexplorer)
	ON_BN_CLICKED(IDC_BUTTONGRIDCTRL, &CFormToolBox::OnBnClickedButtongridctrl)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CFormToolBox 消息处理程序

BOOL CFormToolBox::OnInitDialog()
{
    CDialogEx::OnInitDialog();

   SetBackgroundColor(GetSysColor(COLOR_WINDOW));

   // TODO:在此添加额外的初始化

   m_btnDefault.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnStatic.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnBtn.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnRadio.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnCheck.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnEdit.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnGroup.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnDate.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnProgress.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnPicture.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnComboBox.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnListbox.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnSlider.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnPath.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnTab.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnTimer.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnHotkey.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnExplorer.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
   m_btnGrid.SetSSLColor(CButtonSSL::SSL_OVER_BK_COLOR,RGB(255,232,166));
 

   m_btnDefault.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnStatic.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnBtn.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnRadio.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnCheck.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnEdit.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnGroup.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnDate.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnProgress.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnPicture.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnComboBox.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnListbox.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnSlider.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnPath.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnTab.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnTimer.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnHotkey.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnExplorer.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
   m_btnGrid.SetSSLColor(CButtonSSL::SSL_DOWN_BK_COLOR,RGB(255,232,166));
 

   m_btnDefault.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnStatic.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnBtn.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnRadio.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnCheck.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnEdit.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnGroup.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnDate.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnProgress.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnPicture.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnComboBox.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnListbox.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnSlider.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnPath.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnTab.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnTimer.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnHotkey.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnExplorer.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));
   m_btnGrid.SetSSLColor(CButtonSSL::SSL_UP_BK_COLOR,GetSysColor(COLOR_WINDOW));

    m_btnStatic.SetSSLButtonToolTip(_T("标签"));
    m_btnEdit.SetSSLButtonToolTip(_T("编辑框"));
    m_btnRadio.SetSSLButtonToolTip(_T("单选框"));
    m_btnCheck.SetSSLButtonToolTip(_T("复选框"));
    m_btnDefault.SetSSLButtonToolTip(_T("默认"));
    m_btnGroup.SetSSLButtonToolTip(_T("分组框"));
    m_btnBtn.SetSSLButtonToolTip(_T("按钮"));
    m_btnDate.SetSSLButtonToolTip(_T("日期框"));
    m_btnProgress.SetSSLButtonToolTip(_T("进度条"));
    m_btnPicture.SetSSLButtonToolTip(_T("图片框"));
    m_btnComboBox.SetSSLButtonToolTip(_T("下拉框"));
    m_btnListbox.SetSSLButtonToolTip(_T("列表框"));
    m_btnSlider.SetSSLButtonToolTip(_T("滑块"));
    m_btnPath.SetSSLButtonToolTip(_T("浏览框"));
	m_btnTab.SetSSLButtonToolTip(_T("选项卡"));
	m_btnTimer.SetSSLButtonToolTip(_T("定时器"));
	m_btnHotkey.SetSSLButtonToolTip(_T("热键"));
	m_btnExplorer.SetSSLButtonToolTip(_T("浏览器"));
	m_btnGrid.SetSSLButtonToolTip(_T("表格"));

    m_btnStatic.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnEdit.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnRadio.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnCheck.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnDefault.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnGroup.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnBtn.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnDate.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnProgress.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnPicture.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnComboBox.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnListbox.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnSlider.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
    m_btnPath.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
	m_btnTab.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
	m_btnTimer.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
	m_btnHotkey.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
	m_btnExplorer.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);
	m_btnGrid.SetSSLImageAlign(SSL_IMAGE_LEFT|SSL_IMAGE_VCENTER);

    m_btnDefault.SetSSLButtonIcon(IDI_TOOL_DEFAULT);
    m_btnStatic.SetSSLButtonIcon(IDI_TOOL_STATIC);
    m_btnBtn.SetSSLButtonIcon(IDI_TOOL_BUTTON);
    m_btnRadio.SetSSLButtonIcon(IDI_TOOL_RADIO);
    m_btnCheck.SetSSLButtonIcon(IDI_TOOL_CHECK);
    m_btnEdit.SetSSLButtonIcon(IDI_TOOL_EDIT);
    m_btnGroup.SetSSLButtonIcon(IDI_TOOL_GROUP);
    m_btnDate.SetSSLButtonIcon(IDI_TOOL_DATE);
    m_btnProgress.SetSSLButtonIcon(IDI_TOOL_PROGRESS);
    m_btnPicture.SetSSLButtonIcon(IDI_TOOL_PICTURE);
    m_btnComboBox.SetSSLButtonIcon(IDI_TOOL_COMBOBOX);
    m_btnListbox.SetSSLButtonIcon(IDI_TOOL_LISTBOX);
    m_btnSlider.SetSSLButtonIcon(IDI_TOOL_SLIDER);
    m_btnPath.SetSSLButtonIcon(IDI_TOOL_PATH);
	m_btnTab.SetSSLButtonIcon(IDI_TOOL_TAB);
	m_btnTimer.SetSSLButtonIcon(IDI_TOOL_TIMER);
	m_btnHotkey.SetSSLButtonIcon(IDI_TOOL_HOTKEY);
	m_btnExplorer.SetSSLButtonIcon(IDI_TOOL_WEB);
	m_btnGrid.SetSSLButtonIcon(IDI_TOOL_GRID);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CFormToolBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    SCROLLINFO scrollinfo;  
    GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
    switch (nSBCode)  
    {  
    case SB_BOTTOM:  
        ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);  
        scrollinfo.nPos = scrollinfo.nMax;  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    case SB_TOP:  
        ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);  
        scrollinfo.nPos = scrollinfo.nMin;  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    case SB_LINEUP:  
        scrollinfo.nPos -= 1;  
        if(scrollinfo.nPos < scrollinfo.nMin)
        {  
            scrollinfo.nPos = scrollinfo.nMin;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,10);  
        break;  
    case SB_LINEDOWN:  
        scrollinfo.nPos += 1;  
        if (scrollinfo.nPos>scrollinfo.nMax)  
        {  
            scrollinfo.nPos = scrollinfo.nMax;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,-10);  
        break;  
    case SB_PAGEUP:  
        scrollinfo.nPos -= 5;  
        if (scrollinfo.nPos < scrollinfo.nMin)
        {  
            scrollinfo.nPos = scrollinfo.nMin;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,10*5);  
        break;  
    case SB_PAGEDOWN:  
        scrollinfo.nPos += 5;  
        if (scrollinfo.nPos>scrollinfo.nMax)  
        {  
            scrollinfo.nPos = scrollinfo.nMax;  
            break;  
        }  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        ScrollWindow(0,-10*5);  
        break;  
    case SB_ENDSCROLL:  
        // MessageBox("SB_ENDSCROLL");  
        break;  
    case SB_THUMBPOSITION:  
        // ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
        // scrollinfo.nPos = nPos;  
        // SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    case SB_THUMBTRACK:  
        ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
        scrollinfo.nPos = nPos;  
        SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
        break;  
    }
    CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFormToolBox::OnBnClickedButtonstatic()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_STATIC;
}


void CFormToolBox::OnBnClickedButtondefault()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_MAX;
}


void CFormToolBox::OnBnClickedButtonbtn()
{
    // TODO: 在此添加控件通知处理程序代码
     m_SelectToolType=TCTT_BUTTON;
}


void CFormToolBox::OnBnClickedButtonradio()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_RADIO;
}


void CFormToolBox::OnBnClickedButtoncheck()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_CHECK;
}


void CFormToolBox::OnBnClickedButtonedti()
{
    // TODO: 在此添加控件通知处理程序代码
      m_SelectToolType=TCTT_EDIT;
}

void CFormToolBox::OnBnClickedButtongroup()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_GROUP;
}

void CFormToolBox::OnBnClickedButtondate()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_DATE;
}

void CFormToolBox::OnBnClickedButtonprogress()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_PROGRESS;
}

void CFormToolBox::OnBnClickedButtonpicture()
{
    // TODO: 在此添加控件通知处理程序代码
      m_SelectToolType=TCTT_PICTURE;
}

void CFormToolBox::OnBnClickedButtoncombobox()
{
    // TODO: 在此添加控件通知处理程序代码
      m_SelectToolType=TCTT_COMBOBOX;
}

void CFormToolBox::OnBnClickedButtonlistbox()
{
    // TODO: 在此添加控件通知处理程序代码
      m_SelectToolType=TCTT_LISTBOX;
}


void CFormToolBox::OnBnClickedButtonslider()
{
    // TODO: 在此添加控件通知处理程序代码
     m_SelectToolType=TCTT_SLIDER;
}


void CFormToolBox::OnBnClickedButtonpath()
{
    // TODO: 在此添加控件通知处理程序代码
    m_SelectToolType=TCTT_PATH;
}


void CFormToolBox::OnBnClickedButtontab()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelectToolType=TCTT_TAB;
}


void CFormToolBox::OnBnClickedButtontimer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelectToolType=TCTT_TIMER;
}


void CFormToolBox::OnBnClickedButtonhotkey()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelectToolType=TCTT_HOTKEY;
}


void CFormToolBox::OnBnClickedButtonexplorer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SelectToolType=TCTT_EXPLORER;
}


void CFormToolBox::OnBnClickedButtongridctrl()
{
	// TODO: 在此添加控件通知处理程序代码
	//这儿要判断是不是VIP
	//if(CTcVipFunction::isVip())
		m_SelectToolType=TCTT_GRIDCTRL;
	//else 
	//	::AfxMessageBox(_T("此控件为VIP控件,请登录后,确认为VIP用户,才可以使用"));
}


BOOL CFormToolBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		SCROLLINFO scrollinfo;  
    GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
       
	//CPoint point = GetScrollPosition();
	if(zDelta==120)
	{
		scrollinfo.nPos -= 1;  
		if(scrollinfo.nPos < scrollinfo.nMin)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			//break;  
			return TRUE;
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL); 

		ScrollWindow(0,10);  

	}
	if(zDelta==-120)
	{
		scrollinfo.nPos += 1;  
		if (scrollinfo.nPos>scrollinfo.nMax)  
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			//break;  
			return TRUE;
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-10); 

	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
