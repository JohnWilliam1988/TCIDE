// VipCenterHtmlDia.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "mainfrm.h"
#include "VipCenterHtmlDia.h"
//#include "HttpDownloadFile.h"
#include "TCRunTimeManager.h"
// CVipCenterHtmlDia 对话框

IMPLEMENT_DYNCREATE(CVipCenterHtmlDia, CDHtmlDialog)

CVipCenterHtmlDia::CVipCenterHtmlDia(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CVipCenterHtmlDia::IDD, CVipCenterHtmlDia::IDH, pParent)
{

}

CVipCenterHtmlDia::~CVipCenterHtmlDia()
{
}

void CVipCenterHtmlDia::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CVipCenterHtmlDia::OnInitDialog()
{
	UpdateData(TRUE);
	SetHostFlags( DOCHOSTUIFLAG_FLAT_SCROLLBAR | DOCHOSTUIFLAG_NO3DBORDER );
	CDHtmlDialog::OnInitDialog();
	this->m_pBrowserApp->put_Silent(VARIANT_TRUE);
	this->m_pBrowserApp->put_Offline(VARIANT_TRUE);
	this->m_pBrowserApp->Navigate(L"http://pt.tyuyan.com/pagesoft/soft_features.aspx",0,0,0,0);
	this->m_pBrowserApp->put_Offline(VARIANT_FALSE);
	//this->m_pBrowserApp->Navigate(L"http://pt.tyuyan.com",0,0,0,0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CVipCenterHtmlDia, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CVipCenterHtmlDia)
	/*DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)*/
	DHTML_EVENT_ONCLICK(_T("peel01"), OnSkin1)
	DHTML_EVENT_ONCLICK(_T("peel02"), OnSkin2)
	DHTML_EVENT_ONCLICK(_T("peel03"), OnSkin3)
	DHTML_EVENT_ONCLICK(_T("peel04"), OnSkin4)
	DHTML_EVENT_ONCLICK(_T("plug_import"), OnPlugImport)
END_DHTML_EVENT_MAP()



// CVipCenterHtmlDia 消息处理程序

//HRESULT CVipCenterHtmlDia::OnButtonOK(IHTMLElement* /*pElement*/)
//{
//	OnOK();
//	return S_OK;
//}
//
//HRESULT CVipCenterHtmlDia::OnButtonCancel(IHTMLElement* /*pElement*/)
//{
//	OnCancel();
//	return S_OK;
//}

HRESULT CVipCenterHtmlDia::OnSkin1(IHTMLElement* /*pElement*/)
{
	if(!CTCRunTimeManager::g_IsLoadProj)
		return false;
	CTCRunTimeManager::g_ExeBackFileVIP.Format(_T("%s\\bakafile.bak"),CTCRunTimeManager::g_runtimePath);

	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;	
	if(pFrame->m_IsInCompile)
		return S_OK;
	//pFrame->m_IsInCompile=TRUE;
	pFrame->m_isVipBuild=true;
	//这儿先激活OutPut窗口
	pFrame->OnTcBuild();	
	return S_OK;
}

HRESULT CVipCenterHtmlDia::OnSkin2(IHTMLElement* /*pElement*/)
{
	if(!CTCRunTimeManager::g_IsLoadProj)
		return false;
	CTCRunTimeManager::g_ExeBackFileVIP.Format(_T("%s\\bakbfile.bak"),CTCRunTimeManager::g_runtimePath);

	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;	
	if(pFrame->m_IsInCompile)
		return S_OK;
	//pFrame->m_IsInCompile=TRUE;
	pFrame->m_isVipBuild=true;
	//这儿先激活OutPut窗口
	pFrame->OnTcBuild();	
	return S_OK;
}

HRESULT CVipCenterHtmlDia::OnSkin3(IHTMLElement* /*pElement*/)
{
	if(!CTCRunTimeManager::g_IsLoadProj)
		return false;
	CTCRunTimeManager::g_ExeBackFileVIP.Format(_T("%s\\bakcfile.bak"),CTCRunTimeManager::g_runtimePath);

	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;	
	if(pFrame->m_IsInCompile)
		return S_OK;
	//pFrame->m_IsInCompile=TRUE;
	pFrame->m_isVipBuild=true;
	//这儿先激活OutPut窗口
	pFrame->OnTcBuild();	
	return S_OK;
}


HRESULT CVipCenterHtmlDia::OnSkin4(IHTMLElement* /*pElement*/)
{
	if(!CTCRunTimeManager::g_IsLoadProj)
		return false;
	CTCRunTimeManager::g_ExeBackFileVIP.Format(_T("%s\\bakdfile.bak"),CTCRunTimeManager::g_runtimePath);

	CMainFrame *pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;	
	if(pFrame->m_IsInCompile)
		return S_OK;
	//pFrame->m_IsInCompile=TRUE;
	pFrame->m_isVipBuild=true;
	//这儿先激活OutPut窗口
	pFrame->OnTcBuild();
	return S_OK;
}

HRESULT CVipCenterHtmlDia::OnPlugImport(IHTMLElement *pElement)
{
	//CHttpDownloadFile downfile;
	//downfile.SetCurrentPath(CTCRunTimeManager::g_ProjBuildPath+_T("/"));
	//if(downfile.downloadFile("http://pt.tyuyan.com/tc_gfpluglist/TSPlug.DLL"))
	//	::AfxMessageBox(L"导入成功");
	//else
	//	::AfxMessageBox(L"导入失败");
	//ShellExecute(NULL, _T("open"), L"http://pt.tyuyan.com/tc_gfpluglist/TSPlug.DLL", NULL, NULL, SW_SHOWNORMAL); 
	return S_OK;
}

BOOL CVipCenterHtmlDia::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == 116) || pMsg->message == WM_RBUTTONDOWN  || pMsg->message == WM_RBUTTONDBLCLK )
		return TRUE;
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}
