// ListFarther.cpp : 实现文件
//
#include "stdafx.h"
#include "TC.h"
#include "TCView.h"
#include "afxdialogex.h"
#include "ListFarther.h"

// ListFarther 对话框
//
//IMPLEMENT_DYNAMIC(ListFarther, CDialogEx)

ListFarther::ListFarther(CWnd* pParent /*=NULL*/)
	: CDialogEx(ListFarther::IDD, pParent)
{

}

ListFarther::~ListFarther()
{
}

void ListFarther::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICONLISTBOX, m_listBox);
}


BEGIN_MESSAGE_MAP(ListFarther, CDialogEx)
END_MESSAGE_MAP()


// ListFarther 消息处理程序


BOOL ListFarther::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	::SetWindowPos(this->m_hWnd,HWND_TOP,0,0,180,180,SWP_NOMOVE|SWP_NOACTIVATE);
	
	m_listBox.m_ParentHandle = this->m_hWnd;
	m_listBox.SetWindowPos(NULL,0,0,180,180,SWP_NOMOVE|SWP_NOACTIVATE);
	//m_listView.Create(this
		//,5325388,CRect(0,0,180,180),WS_CHILD|WS_CLIPCHILDREN|WS_BORDER|LVS_SMALLICON|/*LVS_EDITLABELS*/WS_VSCROLL|LVS_NOLABELWRAP|LVS_SINGLESEL);
	//m_listView.setImageList();
	//m_listView.m_ParentHandle = this->m_hWnd;

	CBitmap bmp;	
	m_imgNormal.Create(16,
		16,
		ILC_COLOR32|ILC_MASK,
		10,	// 初始化图片数量
		10);

	ASSERT(m_imgNormal.m_hImageList);

	bmp.LoadBitmap(IDB_LB_IMG);
	m_imgNormal.Add( &bmp, -1);	
	bmp.DeleteObject();
	m_listBox.SetImageList(&m_imgNormal);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL ListFarther::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->wParam == VK_RETURN) && this->IsWindowVisible())
	{
		//m_listView.SendMessage(WM_KEYDOWN,VK_RETURN);
		m_listBox.SendMessage(WM_KEYDOWN,VK_RETURN);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
