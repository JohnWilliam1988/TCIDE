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

// TC.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TC.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "TCDoc.h"
#include "TCView.h"
#include "StartDoc.h"
#include "StartView.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"
#include "VipCenterDoc.h"
#include "VipCenterView.h"

#include "SplashWnd.h"


//项目新建头文件
#include "DiaNewProj.h"

#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LONG WINAPI ProcessAAAException(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	CString wsDumpFilePath = _T("C:\\DumpFile.dmp");

	HANDLE hDumpFile = CreateFileW(wsDumpFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL ,NULL);
	if (hDumpFile)
	{
		MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
		loExceptionInfo.ExceptionPointers = pExceptionInfo;
		loExceptionInfo.ThreadId = GetCurrentThreadId();
		loExceptionInfo.ClientPointers = TRUE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
		CloseHandle(hDumpFile);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

// CTCApp
BEGIN_MESSAGE_MAP(CTCApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTCApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CTCApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CTCApp::OnFileOpen)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1,ID_FILE_MRU_FILE5,&CTCApp::OnRecentFile)
END_MESSAGE_MAP()

// CTCApp 构造
CTCApp::CTCApp()
{
	EnableHtmlHelp();

	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TC.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CTCApp 对象

CTCApp theApp;

// CTCApp 初始化

BOOL CTCApp::InitInstance()
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////下面这段代码据说可以改变VS的默认生成的字体,的确没有错,但是在改变窗口的显示样式以后会报错//////////////
    afxGlobalData.fontRegular.DeleteObject();
    afxGlobalData.fontRegular.CreateFont(
        12,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                 // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        ANSI_CHARSET,              // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        _T("宋体"));
    //////////////////////上面这段代码据说可以改变VS的默认生成的字体,的确没有错,但是在改变窗口的显示样式以后会报错//////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	SetUnhandledExceptionFilter(ProcessAAAException); // 用于自动产生程序异常跟踪定位文件（.dump）

    //这里添加TC自己的初始化代码
	//this->m_bSaveState=false;
    CTCRunTimeManager::commonInit();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;

	ParseCommandLine(cmdInfo);

	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	//CMultiDocTemplate* pDocTemplate;
	m_pDocTemplateTc = new CMultiDocTemplate(IDR_TCTYPE,
		RUNTIME_CLASS(CTCDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CTCView));
	if (!m_pDocTemplateTc)
		return FALSE;
	AddDocTemplate(m_pDocTemplateTc);

	//如果不需要显示启动页面则不创建,注意用到模板对象的地方

		//添加一个首页的模板
	m_pStartDocTemplate = new CMultiDocTemplate(IDR_TCSTARTTYPE,
		RUNTIME_CLASS(CStartDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CStartView));
	if (!m_pStartDocTemplate)
		return FALSE;
	AddDocTemplate(m_pStartDocTemplate);

	//添加一个VIP管理模板
	m_pVipDocTemplate = new CMultiDocTemplate(IDI_ICON_VIP,
		RUNTIME_CLASS(CVipCenterDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CVipCenterView));
	if (!m_pVipDocTemplate)
		return FALSE;
	AddDocTemplate(m_pVipDocTemplate);

    //添加一个面板设计的模板
    m_pDocTemplateTWin = new CMultiDocTemplate(IDR_TCSTARTTYPE,
        RUNTIME_CLASS(CTCDesignDoc),
        RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
        RUNTIME_CLASS(CTCDesignView));
    if (!m_pDocTemplateTWin)
        return FALSE;
    AddDocTemplate(m_pDocTemplateTWin);



	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
//	CCommandLineInfo cmdInfo;

//	ParseCommandLine(cmdInfo);

	/*if( FALSE == CTCRunTimeManager::EnablePrivilege(_T("SeDebugPrivilege"),true) )
	{
		AfxMessageBox(_T("权限提升失败"));
	}*/

	//开始文件关联
	ProjFileRelated();
	//处理命令行参数
	CString cmdline;
	cmdline = ::AfxGetApp()->m_lpCmdLine;
	//如果有参数,则设置文档打开信息

    //处理双吉打开tcproj文件的,命令行处理的参数
	if (cmdline.GetLength() > 0)
	{
		//这儿要FileNothing,要不然会打开.tcproj文件
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
		cmdInfo.m_strFileName = cmdline;

		if(cmdline.Right(7)==_T(".tcproj"))
		{
			//这儿直接调用最近打开文件的函数就可以了,参数是传进来的.tcproj的路径
			if(OpenRecentProject(cmdline))
			{
				//修改标题
				 CString csMainTitle;
				 csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
				((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
			}
		}
		else if( cmdline.Right(2) == _T(".t"))
		{
			m_pStartDocTemplate->OpenDocumentFile(NULL);
			m_pDocTemplateTc->OpenDocumentFile(cmdline);
		}
	}
	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
    {
		//4.01开始页面改动
		if( m_pStartDocTemplate != NULL && CTCRunTimeManager::g_StartPageShow )
			m_pStartDocTemplate->OpenDocumentFile(NULL);
    }
    else
    {
        if (!ProcessShellCommand(cmdInfo))
            return FALSE;
    }
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CTCApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CTCApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

//定义TC自己的项目新建
void CTCApp::OnFileNew()
{
    CDiaNewProj dia;
    int nResponse=dia.DoModal();
    if(nResponse == IDOK)  // OK
    {
		CTCExeVersionManager::TCResetVersionInfo();
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.reset();

        //打开项目默认文件,是否只需要这样打开就可以了? -match
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.load(CTCRunTimeManager::g_projRcFile);
        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->ProjFileViewReload();
        ((CTCApp*)AfxGetApp())->TCOpenTWin();

		 //设置主标题
		CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
        ((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
		//创建备份项目
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
    }
}

//定义TC自己的项目打开
void CTCApp::OnFileOpen()
{
    if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCOpenExistProject())
    {
        //设置主标题
        CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
        ((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
		//备份项目
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
    }
}

// 用于运行对话框的应用程序命令
void CTCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTCApp 自定义加载/保存方法
void CTCApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
    bNameValid = strName.LoadString(IDS_PROJ_LOADPROJ);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_PROJ_LOADPROJ);
    bNameValid = strName.LoadString(IDS_PROJ_FOLDER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_PROJ_FOLDER);
    bNameValid = strName.LoadString(IDS_PROJ_RC);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_PROJ_RC);
    bNameValid = strName.LoadString(IDS_PROJ_FILE);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_PROJ_FILE);
    bNameValid = strName.LoadString(IDS_PROJ_RCFILE);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_PROJ_RCFILE);
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_OUTPUT_POPUP);
	bNameValid = strName.LoadString(IDS_HOTKEY_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName,IDR_HOTKEY_POPUP);
}

void CTCApp::ProjFileRelated()
{
	CString AppPath;
	AppPath.GetBuffer(256);
	AppPath.ReleaseBuffer();
	::GetModuleFileName(this->m_hInstance, AppPath.GetBuffer(), 256);

	CString tIconPath;
	tIconPath.Format(_T("%s\\ico\\%s"),CTCRunTimeManager::g_runtimePath,_T("TCDoc.ico"));

	CString TCProjIconPath;
	TCProjIconPath.Format(_T("%s\\ico\\%s"),CTCRunTimeManager::g_runtimePath,_T("TCproj.ico"));

	CString tllIconPath;
	tllIconPath.Format(_T("%s\\ico\\%s"),CTCRunTimeManager::g_runtimePath,_T("tll.ico"));

	HKEY hKey;
	if(ERROR_SUCCESS == RegOpenKeyExW(HKEY_CLASSES_ROOT,_T("tcproj"),0,KEY_ALL_ACCESS,&hKey))
	{
		RegDeleteKeyW(hKey,_T("DefaultIcon"));
		RegDeleteKeyW(hKey,_T("Shell\\Open\\Command"));
		RegDeleteKeyW(hKey,_T("Shell\\Open"));
		RegDeleteKeyW(hKey,_T("Shell"));
		RegDeleteKeyW(HKEY_CLASSES_ROOT,_T("tcproj"));
		//RegCloseKey(hKey);
	}
	
	if(ERROR_SUCCESS == RegOpenKeyExW(HKEY_CLASSES_ROOT,_T("t"),0,KEY_ALL_ACCESS,&hKey))
	{
		RegDeleteKeyW(hKey,_T("DefaultIcon"));
		RegDeleteKeyW(hKey,_T("Shell\\Open\\Command"));
		RegDeleteKeyW(hKey,_T("Shell\\Open"));
		RegDeleteKeyW(hKey,_T("Shell"));
		RegDeleteKeyW(HKEY_CLASSES_ROOT,_T("t"));
		//RegCloseKey(hKey);
	}
	if(ERROR_SUCCESS == RegOpenKeyExW(HKEY_CLASSES_ROOT,_T("tll"),0,KEY_ALL_ACCESS,&hKey))
	{
		RegDeleteKeyW(hKey,_T("DefaultIcon"));
		RegDeleteKeyW(HKEY_CLASSES_ROOT,_T("tll"));
		//RegCloseKey(hKey);
	}

	int ret = TCprojFileRelated(_T("tcproj"),_T("TC项目文件"), TCProjIconPath, AppPath);
	int reg = TFileRelated(_T("t"),_T("t文件"),tIconPath , AppPath);
	int res = TLLFileRelated(_T("tll"),_T("tll文件"),tllIconPath,AppPath);

	if (ret!= NULL||reg!= NULL||res!=NULL)
		MessageBox(::AfxGetMainWnd()->m_hWnd,_T("文件关联失败"), _T("文件关联"), NULL);

	//刷新系统图标
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	return;
}
int CTCApp::TCprojFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath)
{
	int ret;
	HKEY hKey;
	//扩展名的注册表键
	CString ExtKeyName;
	ExtKeyName=FileNameExt;

	//添加文件扩展名键
	FileNameExt = _T(".") + FileNameExt;
	ret = RegCreateKey(HKEY_CLASSES_ROOT, FileNameExt, &hKey);
	//设置扩展名对应的键
	ret = RegSetValue(hKey, _T(""), REG_SZ, ExtKeyName, sizeof(FileNameExt));

	//添加扩展名对应的键
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	//设置扩展名的说明
	ret = RegSetValue(hKey,_T( ""), REG_SZ, ExtNote, sizeof(ExtNote));

	//添加扩展名的图标键
	ret = RegCreateKey(hKey, _T("DefaultIcon"), &hKey);
	//设置图标地址
	ret = RegSetValue(hKey, _T(""), REG_SZ, IconPath, sizeof(IconPath));

	//添加关联程序启动键
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	ret = RegCreateKey(hKey, _T("Shell\\Open\\Command"), &hKey);
	AppPath.Format(_T("%s %%1"), AppPath);

	ret =RegSetValue(hKey, _T(""), REG_SZ, AppPath, sizeof(AppPath));

	return ret;
}

int CTCApp::TFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath)
{
	int ret;
	HKEY hKey;
	//扩展名的注册表键
	CString ExtKeyName;
	ExtKeyName=FileNameExt;

	//添加文件扩展名键
	FileNameExt = _T(".") + FileNameExt;
	ret = RegCreateKeyW(HKEY_CLASSES_ROOT, FileNameExt, &hKey);
	//设置扩展名对应的键
	ret = RegSetValue(hKey, _T(""), REG_SZ, ExtKeyName, sizeof(FileNameExt));

	//添加扩展名对应的键
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	//设置扩展名的说明
	ret = RegSetValue(hKey,_T(""), REG_SZ, ExtNote, sizeof(ExtNote));

	//添加扩展名的图标键
	ret = RegCreateKey(hKey, _T("DefaultIcon"), &hKey);
	//设置图标地址
	ret = RegSetValue(hKey, _T(""), REG_SZ, IconPath, sizeof(IconPath));

	////添加关联程序启动键
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	ret = RegCreateKey(hKey, _T("Shell\\Open\\Command"), &hKey);
	AppPath.Format(_T("%s %%1"), AppPath);

	ret =RegSetValue(hKey, _T(""), REG_SZ, AppPath, sizeof(AppPath));

	return ret;
}

int CTCApp::TLLFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath)
{
	int ret;
	HKEY hKey;
	//扩展名的注册表键
	CString ExtKeyName;
	ExtKeyName=FileNameExt;

	//添加文件扩展名键
	FileNameExt = _T(".") + FileNameExt;
	ret = RegCreateKey(HKEY_CLASSES_ROOT, FileNameExt, &hKey);
	//设置扩展名对应的键
	ret = RegSetValue(hKey, _T(""), REG_SZ, ExtKeyName, sizeof(FileNameExt));

	//添加扩展名对应的键
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	//设置扩展名的说明
	ret = RegSetValue(hKey,_T(""), REG_SZ, ExtNote, sizeof(ExtNote));

	//添加扩展名的图标键
	ret = RegCreateKey(hKey, _T("DefaultIcon"), &hKey);
	//设置图标地址
	ret = RegSetValue(hKey, _T(""), REG_SZ, IconPath, sizeof(IconPath));

	return ret;
}


void CTCApp::LoadCustomState()
{


}

void CTCApp::SaveCustomState()
{

}

// CTCApp 消息处理程序

void CTCApp::TCCloseAllView()
{
    //获得文档模板的位置 
	if( m_pDocTemplateTc != NULL )
		m_pDocTemplateTc->CloseAllDocuments(0);
	if( m_pDocTemplateTWin != NULL )
		m_pDocTemplateTWin->CloseAllDocuments(0);
	if( m_pStartDocTemplate != NULL )
		m_pStartDocTemplate->CloseAllDocuments(0);
}

void CTCApp::TCCloseWithoutStartView()
{
	m_pDocTemplateTc->CloseAllDocuments(0);
	m_pDocTemplateTWin->CloseAllDocuments(0);
}

void CTCApp::TCOpenTFile(CString file)
{
    POSITION pos=m_pDocTemplateTc->GetFirstDocPosition();
	CTCView *pView=NULL;
    CTCDoc* pDoc = NULL;
    if(pos==NULL)
    {
		//如果先打开Twind就不报错，
		//TCOpenTWin();
		//9-14修改在这儿不用添加最近打开文档
		m_pDocTemplateTc->OpenDocumentFile(file,FALSE,TRUE);
		//m_pDocTemplateTc->InitialUpdateFrame(
		/*pDoc = new CTCDoc;
		((CTCApp*)AfxGetApp())->m_pDocTemplateTc->OpenDocumentFile(file);*/
		return;
    }
    while (NULL != pos)
    {
        pDoc = (CTCDoc*)(m_pDocTemplateTc->GetNextDoc(pos));
        if(pDoc==NULL)
            continue;
        POSITION posView = pDoc->GetFirstViewPosition();
        while (NULL != posView)
        {
            pView = (CTCView *)(pDoc->GetNextView(posView));
            if (NULL == pView )
                continue;
            if(CTCRunTimeManager::isSameFile(file,pView->m_ScintillaWnd.GetFileName()))
            {
                CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
                pFWnd->MDIActivate();
                return; // 文档已经被打开
            }
        }
    }

    //这儿打开文档,激活怎么搞了
    m_pDocTemplateTc->OpenDocumentFile(file,FALSE,TRUE);
}

CView * CTCApp::TCOpenTFileReturnView(CString file)
{
   POSITION pos=m_pDocTemplateTc->GetFirstDocPosition();

    CTCView *pView=NULL;
    CTCDoc* pDoc = NULL;
    while (NULL != pos)
    {
        pDoc = (CTCDoc*)(m_pDocTemplateTc->GetNextDoc(pos));
        if(pDoc==NULL)
            continue;
        POSITION posView = pDoc->GetFirstViewPosition();
        while (NULL != posView)
        {
            pView = (CTCView *)(pDoc->GetNextView(posView));
            if (NULL == pView )
                continue;
            if(CTCRunTimeManager::isSameFile(file,pView->m_ScintillaWnd.GetFileName()))
            {
                CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
                pFWnd->MDIActivate();
                return pView; // 文档已经被打开
            }
        }
    }

    //9-14修改，在这儿不用添加最近打开列表,激活怎么搞了
    pDoc=(CTCDoc*)m_pDocTemplateTc->OpenDocumentFile(file,FALSE,TRUE);
    POSITION posView=pDoc->GetFirstViewPosition();
    return pDoc->GetNextView(posView);
}

CView *CTCApp::TCGetTCDesignView(CString winName)
{
	POSITION pos=m_pDocTemplateTWin->GetFirstDocPosition();
	CTCDesignView *pView=NULL;
	CTCDesignDoc* pDoc = NULL;
	while (NULL != pos)
	{
		pDoc = (CTCDesignDoc*)m_pDocTemplateTWin->GetNextDoc(pos);
		if(pDoc==NULL)
			continue;
		POSITION posView = pDoc->GetFirstViewPosition();
		while (NULL != posView)
		{
			pView = (CTCDesignView *)pDoc->GetNextView(posView);
			if (NULL == pView )
				continue;
			if(0 == pView->m_TWinID.CompareNoCase(winName))
			{
				return pView;
			}
		}
	}
	return NULL;
}

void CTCApp::TCDeleteTFileView(CString file)
{
    CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTc;
    POSITION pos=pDocTmp->GetFirstDocPosition();
    CTCView *pView=NULL;
    CTCDoc* pDoc = NULL;
    while (NULL != pos)
    {
        pDoc = (CTCDoc*)pDocTmp->GetNextDoc(pos);
        if(pDoc==NULL)
            continue;
        POSITION posView = pDoc->GetFirstViewPosition();
        while (NULL != posView)
        {
            pView = (CTCView *)pDoc->GetNextView(posView);
            if (NULL == pView )
                continue;
            if(CTCRunTimeManager::isSameFile(file,pView->m_ScintillaWnd.GetFileName()))
            {
                CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
                pFWnd->MDIDestroy();
                return; // 文档已经被打开
            }
        }
    }
}

void CTCApp::TCDeleteDesignView(CString winID)
{
	CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTWin;
	POSITION pos=pDocTmp->GetFirstDocPosition();
	CTCDesignView *pView=NULL;
	CTCDesignDoc* pDoc = NULL;
	while (NULL != pos)
	{
		pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
		if(pDoc==NULL)
			continue;
		POSITION posView = pDoc->GetFirstViewPosition();
		while (NULL != posView)
		{
			pView = (CTCDesignView *)pDoc->GetNextView(posView);
			if (NULL == pView )
				continue;
			//if(CTCRunTimeManager::isSameFile(CTCRunTimeManager::g_ProjName,pView->m_TWinID))
			if(winID == pView->m_TWinID)
			{		
				CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
				pFWnd->MDIDestroy();
				return; // 文档已经被打开			
			}
		}
	}
}

void CTCApp::TCRenameTFile(CString oldPath,CString newPath)
{
	CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTc;
	POSITION pos=pDocTmp->GetFirstDocPosition();

	CTCView *pView=NULL;
	CTCDoc* pDoc = NULL;
	while (NULL != pos)
	{
		pDoc = (CTCDoc*)pDocTmp->GetNextDoc(pos);
		if(pDoc==NULL)
			continue;
		POSITION posView = pDoc->GetFirstViewPosition();
		while (NULL != posView)
		{
			pView = (CTCView *)pDoc->GetNextView(posView);
			if (NULL == pView )
				continue;
			if(CTCRunTimeManager::isSameFile(oldPath,pView->m_ScintillaWnd.GetFileName()))
			{
				//文档已经被打开,修改名称
				pDoc->SetPathName(newPath);
				pView->m_ScintillaWnd.m_strFileName=newPath;
				return; 
			}
		}
	}
}

void CTCApp::TCRenameTWin(CString oldStr,CString newStr)
{
	CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTWin;
	POSITION pos=pDocTmp->GetFirstDocPosition();
	CTCDesignView *pView=NULL;
	CTCDesignDoc* pDoc = NULL;
	while (NULL != pos)
	{
		pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
		if(pDoc==NULL)
			continue;
		POSITION posView = pDoc->GetFirstViewPosition();
		while (NULL != posView)
		{
			pView = (CTCDesignView *)pDoc->GetNextView(posView);
			if (NULL == pView )
				continue;
			//if(CTCRunTimeManager::isSameFile(CTCRunTimeManager::g_ProjName,pView->m_TWinID))
			if(oldStr == pView->m_TWinID)
			{		
				pDoc->SetTitle(newStr);
				pView->m_TWinID = newStr;
				pView->m_DiaDesign.m_TcDlgConfig.m_dlgID = newStr;			
			}
		}
	}
}

void CTCApp::TCOpenTWin(CString winID)
{
	CTCDesignView *pCurrentView = (CTCDesignView*)TCGetTCDesignView(winID);
	if(pCurrentView != NULL)
	{ 
		CMDIChildWnd *pChild = (CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
		CView *pView = pChild->GetActiveView();
		if(pView->m_hWnd != pCurrentView->m_hWnd)
		{
			CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pCurrentView->GetParent());
			//如果当前视图不是活动视图则激活
			pFWnd->MDIActivate();
			return;
		}
		return;
	}
   //CMultiDocTemplate *pDocTmp=m_pDocTemplateTWin;
   // POSITION pos=pDocTmp->GetFirstDocPosition();
   // CTCDesignView *pView=NULL;
   // CTCDesignDoc* pDoc = NULL;
   // while (NULL != pos)
   // {
   //     pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
   //     if(pDoc==NULL)
   //         continue;
   //     POSITION posView = pDoc->GetFirstViewPosition();
   //     while (NULL != posView)
   //     {
   //         pView = (CTCDesignView *)pDoc->GetNextView(posView);
   //         if (NULL == pView )
   //             continue;
   //        // if(CTCRunTimeManager::isSameFile(startDia/*CTCRunTimeManager::g_ProjName*/,pView->m_TWinID))
			//if(winID== pView->m_TWinID)
   //         {
			//	CMyView* pView;
			//	pView=(CMyView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView();
   //             CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
			//		//如果当前视图不是活动视图则激活
			//	pFWnd->MDIActivate();			
   //             return; // 文档已经被打开
   //         }
   //     }
   // }
    //这儿打开文档
	m_pDocTemplateTWin->OpenDocumentFile(winID/*CTCRunTimeManager::g_ProjName*/,FALSE,TRUE);
   // pDocTmp->OpenDocumentFile(CTCRunTimeManager::g_ProjName,FALSE,TRUE);
}

void CTCApp::TCOpenStartView()
{
	POSITION pos=m_pStartDocTemplate->GetFirstDocPosition();
	CStartView *pView=NULL;
	CStartDoc* pDoc = NULL;
	while (NULL != pos)
	{
		pDoc = (CStartDoc*)m_pStartDocTemplate->GetNextDoc(pos);
		if(pDoc==NULL)
			continue;
		POSITION posView = pDoc->GetFirstViewPosition();
		while (NULL != posView)
		{
			pView = (CStartView *)pDoc->GetNextView(posView);
			if ( pView  != NULL )
			{
				CMDIChildWnd *pChild = (CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
				CView *pCurrentView = pChild->GetActiveView();
				if(pView->m_hWnd != pCurrentView->m_hWnd)
				{
					CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
					//如果当前视图不是活动视图则激活
					pFWnd->MDIActivate();
					return;
				}
				return;
			}	
		}
	}

	//CMultiDocTemplate *pDocTmp=m_pDocTemplateTWin;
	// POSITION pos=pDocTmp->GetFirstDocPosition();
	// CTCDesignView *pView=NULL;
	// CTCDesignDoc* pDoc = NULL;
	// while (NULL != pos)
	// {
	//     pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
	//     if(pDoc==NULL)
	//         continue;
	//     POSITION posView = pDoc->GetFirstViewPosition();
	//     while (NULL != posView)
	//     {
	//         pView = (CTCDesignView *)pDoc->GetNextView(posView);
	//         if (NULL == pView )
	//             continue;
	//        // if(CTCRunTimeManager::isSameFile(startDia/*CTCRunTimeManager::g_ProjName*/,pView->m_TWinID))
	//if(winID== pView->m_TWinID)
	//         {
	//	CMyView* pView;
	//	pView=(CMyView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView();
	//             CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
	//		//如果当前视图不是活动视图则激活
	//	pFWnd->MDIActivate();			
	//             return; // 文档已经被打开
	//         }
	//     }
	// }
	//这儿打开文档
	m_pStartDocTemplate->OpenDocumentFile(_T("开始")/*CTCRunTimeManager::g_ProjName*/,FALSE,TRUE);
	// pDocTmp->OpenDocumentFile(CTCRunTimeManager::g_ProjName,FALSE,TRUE);
}


void CTCApp::TCOpenVipView()
{
	POSITION pos=m_pVipDocTemplate->GetFirstDocPosition();
	CVipCenterView* pView=NULL;
	CVipCenterDoc* pDoc = NULL;
	while (NULL != pos)
	{
		pDoc = (CVipCenterDoc*)m_pVipDocTemplate->GetNextDoc(pos);
		if(pDoc==NULL)
			continue;
		POSITION posView = pDoc->GetFirstViewPosition();
		while (NULL != posView)
		{
			pView = (CVipCenterView *)pDoc->GetNextView(posView);
			if ( pView  != NULL )
			{
				CMDIChildWnd *pChild = (CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
				CView *pCurrentView = pChild->GetActiveView();
				if(pView->m_hWnd != pCurrentView->m_hWnd)
				{
					CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
					//如果当前视图不是活动视图则激活
					pFWnd->MDIActivate();
					return;
				}
				return;
			}	
		}
	}

	m_pVipDocTemplate->OpenDocumentFile(_T("会员中心")/*CTCRunTimeManager::g_ProjName*/,FALSE,TRUE);
}



void CTCApp::TCDeleteTWinView()
{
    CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTWin;
    POSITION pos=pDocTmp->GetFirstDocPosition();
    CTCDesignView *pView=NULL;
    CTCDesignDoc* pDoc = NULL;
    while (NULL != pos)
    {
        pDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(pos);
        if(pDoc==NULL)
            continue;
        POSITION posView = pDoc->GetFirstViewPosition();
        while (NULL != posView)
        {
            pView = (CTCDesignView *)pDoc->GetNextView(posView);
            if (NULL == pView )
                continue;
            if(CTCRunTimeManager::isSameFile(CTCRunTimeManager::g_ProjName,pView->m_TWinID))
            {
                CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
                pFWnd->MDIDestroy();
                return; // 文档已经被打开
            }
        }
    }
}

/***********************************************************************/
/*  \函数名称：TCGetTCView
/*	\函数功能：判断是否存在TCView视类,用来设置编辑器字体相关
/*	\函数返回：BOOL 存在TCView则返回TRUE,否则返回FALSE
/*	\参数说明：@NULL
/***********************************************************************/

CView * CTCApp::TCGetTCView()
{
	POSITION pos=m_pDocTemplateTc->GetFirstDocPosition();

	CTCView *pView=NULL;
	CTCDoc* pDoc = NULL;
	while (NULL != pos)
	{
		pDoc = (CTCDoc*)(m_pDocTemplateTc->GetNextDoc(pos));
		if(pDoc==NULL)
			continue;
		POSITION posView = pDoc->GetFirstViewPosition();
		while (NULL != posView)
		{
			pView = (CTCView *)(pDoc->GetNextView(posView));
			if (NULL == pView )
				continue;
			if( pView != NULL )
			{
				return pView;
			}
		}
	}
	return NULL;
}

void CTCApp::AddToRecentFileList(LPCTSTR lpszPathName)
{
	// lpszPathName will be added to the top of the MRU list. 
	// If lpszPathName already exists in the MRU list, it will be moved to the top
	if (m_pRecentFileList != NULL)	{
		CString help = lpszPathName;
		//添加时剔除掉添加的t文件
		if( help.Right(2) == _T(".t"))
			return;
		m_pRecentFileList->Add(lpszPathName);
	}
}

void CTCApp::TCGetRecentFileList(CStringArray &recentList)
{

	recentList.RemoveAll();
	if( m_pRecentFileList != NULL)
	{
		int size = m_pRecentFileList->GetSize();
		for(int i = 0;i < size; i++)
		{
			if(!m_pRecentFileList->m_arrNames[i].IsEmpty())
				recentList.Add(m_pRecentFileList->m_arrNames[i]);
		}
	}
}

CString CTCApp::GetRecentFileListDisplayName(int index)
{
	if( m_pRecentFileList != NULL)	{
		return m_pRecentFileList->m_arrNames[index];
	}
}

void CTCApp::RemoveDirtyProFromRecentFileList(LPCTSTR lpszPathName)
{
	CString tempPath = lpszPathName;
	int size = m_pRecentFileList->GetSize();
	for(int i = 0;i < size; i++)
	{
		if( tempPath == m_pRecentFileList->m_arrNames[i])
			m_pRecentFileList->Remove(i);
	}
}
BOOL CTCApp::OnRecentFile(UINT uID) 
{
	//通过菜单ID算出在最近打开列表中的序号
	int index=uID-57616;
	CString strName;
	strName=((CTCApp*)AfxGetApp())->GetRecentFileListDisplayName(index);
	if(!OpenRecentProject(strName))
	{
		RemoveDirtyProFromRecentFileList(strName);
			return TRUE;
	}	
	AddToRecentFileList(strName);
	//修改标题
	CString csMainTitle;
	csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
	((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
	//11-2创建备份项目
	//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
	return TRUE;
}
 
//如果是从开始页面打开,则要在加载完成后再关闭视图
BOOL CTCApp::OpenRecentProject(LPCTSTR lpszPathName)
{
	if(((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->m_IsInCompile)
			return TRUE;

		CString isRecent=lpszPathName;		
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCSaveAllDoc();

		//判断是否需要关闭开始页面
		if(	CTCRunTimeManager::g_StartPageClose)
			TCCloseAllView();
		else
		    TCCloseWithoutStartView();

        m_pDocTemplateTc->CloseAllDocuments(TRUE);
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.reset();

		CString csProjFilePath,csProjName,csProjPath,csProjEnterFileName,csProjSourcePath,csRCFile,csBuildPath,csCompilePath;
		int langType=-1;
		CString csHelp=isRecent;
		int i=0, j=0;
		while( i<csHelp.GetLength() )
		{
			if( csHelp[i]=='\\' )
				j = i;
			i++;
		}
		//分离文件名
		CString csProjFullName = csHelp.Mid( j+1 );	
		csProjName = csProjFullName.Left(csProjFullName.GetLength()-7);
		csProjFilePath=csHelp;

		//从配置文件上面读取的内容的最大长度是1024
		::GetPrivateProfileString(_T("ProjInfo"),_T("EnterFile"),_T(""),csProjEnterFileName.GetBuffer(MAX_PATH),MAX_PATH,csProjFilePath);   //读主取配置文件
		csProjEnterFileName.ReleaseBuffer();
		if(csProjEnterFileName.IsEmpty())
		{
			::AfxMessageBox(_T("项目配置文件出错!"));
			return FALSE;
		}
		langType=::GetPrivateProfileInt(_T("ProjInfo"),_T("Language"),-1,csProjFilePath);
		//langType=CTCRunTimeManager::getTCProjFileInt(_T("ProjInfo"),_T("Language"));
		if(!(langType==0||langType==1))
		{
			::AfxMessageBox(_T("项目配置文件出错!"));
			return FALSE;
		}
		CString strCurFileName;
		int iLength = csHelp.GetLength();
		int iIndex = csHelp.ReverseFind(L'\\');
		csHelp.Delete(iIndex,iLength-iIndex);
		csProjPath=csHelp;
		if (!::PathFileExists(csProjPath))
		{
			::AfxMessageBox(_T("项目配置文件出错,不能定位项目目录!"));
			return FALSE;
		}       
		csProjSourcePath.Format(_T("%s\\%s"),csProjPath,csProjName);
		if (!::PathFileExists(csProjSourcePath))
		{
			::AfxMessageBox(_T("项目配置文件出错,不能定位项目源文件目录!"));
			return FALSE;
		}   
		csRCFile.Format(_T("%s\\资源\\main.twin"),csProjSourcePath);
		if (!::PathFileExists(csRCFile))
		{
			csRCFile.Format(_T("%s\\资源\\WinDesign.twin"),csProjSourcePath);
			if(!::PathFileExists(csRCFile))
			{
				::AfxMessageBox(_T("项目配置文件出错,不能定位资源文件!"));
				return false;
			}
		}   
		
		//将资源文件路径设置回来
		csRCFile.Format(_T("%s\\资源\\main.twin"),csProjSourcePath);
		csBuildPath.Format(_T("%s\\生成"),csProjPath);
		if (!::PathFileExists(csBuildPath))
		{
			/*::AfxMessageBox(_T("项目配置文件出错,不能定位生成目录!"));
			return FALSE;*/
			if(0==CreateDirectory(csBuildPath,NULL))   
			{
				AfxMessageBox(_T("创建项目生成文件夹失败,请重试!")); 
				return false;
			}
		} 

		csCompilePath.Format(_T("%s\\编译"),csProjPath);
		if (!::PathFileExists(csCompilePath))
		{
			/*::AfxMessageBox(_T("项目配置文件出错,不能定位生成目录!"));
			return FALSE;*/
			if(0==CreateDirectory(csCompilePath,NULL))   
			{
				AfxMessageBox(_T("创建项目编译文件夹失败,请重试!")); 
				return false;
			}
		}

		CString csProjEnterFile;
		csProjEnterFile.Format(_T("%s\\%s"),csProjSourcePath,csProjEnterFileName);
		CTCRunTimeManager::setProjInfor(csProjPath,csProjName,csProjFilePath,csProjSourcePath,csProjEnterFile,csProjEnterFileName,csRCFile,csBuildPath,csCompilePath,langType);

		//this->m_TWinManager.load(CTCRunTimeManager::g_projRcFile);
		//打开项目默认文件,是否只需要这样打开就可以了 -match

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.load();
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ProjFileViewReload();

		/////////////////////////读取版本信息///////////////////
		CTCExeVersionManager::TCResetVersionInfo();
		CTCExeVersionManager::TCReadVersionInfo(csProjFilePath);
		/////////////////////////读取版本信息///////////////////

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->OnTcOpendesignwin();

	//清空上个项目的输出信息
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCClearOutMsg();
	//清空所有引用信息
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCClearAllContentMsg();
	//设置属性框
	//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(CTCRunTimeManager::g_projEnterFileName,CTCRunTimeManager::g_projEnterFile);
	return TRUE;
}




BOOL CTCApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (CSplashWnd::PreTranslateAppMessage(pMsg)) {
		return TRUE;
	}
	return CWinAppEx::PreTranslateMessage(pMsg);
}
