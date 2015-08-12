// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//`
// 版权所有(C) Microsoft Corporation
// 保留所有权利。
// MainFrm.cpp : CMainFrame 类的实现
//
#include "stdafx.h"
#include "TC.h"

#include "MainFrm.h"

///压缩用到的头文件////////////
#include "Common/StdOutStream.h"
#include "Common/MyInitGuid.h"
#include "Common/LimitedStreams.h"
#include "Common/CopyCoder.h"
#include "Common/MyString.h"
#include "Common/FileStreams.h"
#include "Common/IntToString.h"
#include "Common/ArchiveCommandLine.h"
#include "Common/MyException.h"
#include "Common/ExitCode.h"

#include "C/ConsoleClose.h"
#include "C/BenchCon.h"
#include "C/ExtractCallbackConsole.h"
#include "C/List.h"
#include "C/OpenCallbackConsole.h"
#include "C/UpdateCallbackConsole.h"

#include "Windows/Error.h"
#include "Windows/NtCheck.h"
///压缩用到的头文件////////////
#include "TCDoc.h"
#include "TCView.h"
#include "TCDesignDoc.h"
#include "TCDesignView.h"
#include "TCRunTimeManager.h"
//
#include "TCRunConfim.h"
#include "TCExeVersionManager.h"

#include "SplashWnd.h"
#include "TCCode.h"
#include <direct.h>  
#include <wininet.h>
#pragma comment(lib,"wininet.lib")
#include "TCDesktopAlertDia.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////压缩用到的命名空间///////////////////////////////////////
using namespace NWindows;
using namespace NFile;
using namespace NCommandLineParser;

//////////////////////////////////////////压缩用的全局变量////////////////////////////////////////
BOOL first;
HINSTANCE g_hInstance = 0;
CStdOutStream *g_StdStream=0;

#define NT_CHECK_FAIL_ACTION (*g_StdStream) << "Unsupported Windows version"; return NExitCode::kFatalError;

static const char *kNoFormats = "7-Zip cannot find the code that works with archives.";
static const char *kUnsupportedArcTypeMessage = "Unsupported archive type";
static const wchar_t *kDefaultSfxModule = L"7zCon.sfx";
////////////////////////////////////////压缩用的全局变量////////////////////////////////////////

//判断是不是一个文件
static bool IsOneFile(CString file1,CString file2)
{
	file1.Trim();
	file1.MakeLower();
	file2.Trim();
	file2.MakeLower();
	return (file1==file2);
}

void   DeleteFolder(CString   sPath,bool deleteFolder=true) 
{ 
	CFileFind   ff; 
	BOOL   bFound; 
	bFound   =   ff.FindFile(sPath   +   "\\*.* "); 
	while(bFound) 
	{ 
		bFound   =   ff.FindNextFile(); 
		CString   sFilePath   =   ff.GetFilePath(); 

		if(ff.IsDirectory()) 
		{ 
			if(!ff.IsDots()) 
				DeleteFolder(sFilePath); 
		} 
		else 
		{ 
			if(ff.IsReadOnly()) 
			{ 
				SetFileAttributes(sFilePath,   FILE_ATTRIBUTE_NORMAL); 
			} 
			DeleteFile(sFilePath); 
		} 
	} 
	ff.Close(); 
	SetFileAttributes(sPath,   FILE_ATTRIBUTE_NORMAL); 
	if(deleteFolder)
		RemoveDirectory(sPath); 
} 
bool CopyFolderAllFiles(CString   csSourceFolder,   CString   csNewFolder) 
{ 
	CFileFind   f; 
	BOOL   bFind=f.FindFile(csSourceFolder+ L"\\*.* "); 
	while(bFind) 
	{ 
		bFind   =   f.FindNextFile(); 
		//TRACE(_T( "%s\r\n "),f.GetFileName()); 
		if(f.IsDots())   
			continue; 
		//资源文件里面只有文件,没有文件夹
		if(f.IsDirectory()) 
		{ 
			//              if(_wmkdir(csNewFolder+ L"\\"+f.GetFileName()))
			//return false;

			//              if(!CopyFolderAllFiles(csSourceFolder+ L"\\"+f.GetFileName(),csNewFolder+ L"\\"+f.GetFileName()))
			//return false;
		} 
		else
		{
			::SetFileAttributes(csSourceFolder+ L"\\"+f.GetFileName(),FILE_ATTRIBUTE_NORMAL); 
			if(!::CopyFile(csSourceFolder+ L"\\"+f.GetFileName(),csNewFolder+ L"\\"+f.GetFileName(),TRUE))
				return false;
		}
	} 
	return true;
}

/////////////////////调试用的线程/////////////////////////////
UINT MyDebugInforThread( LPVOID pParam )
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	while(true)
	{
		::WaitForSingleObject(mainF->m_WinEventHandle,INFINITE);
		mainF->SendMessage(WM_MYTCDEBUG_MESSAGE,(WPARAM)(mainF->m_TCDebugInfor->fCurrentBP.getDebugLineNum()),(LPARAM)(mainF->m_TCDebugInfor->fCurrentBP.getDebugFileName()));
		mainF->TCDebugLoadVarList();
		mainF->m_isInBP=true;
	}
	return 1;
}

//TC弹出框信息线程
UINT TCAlertInfoThread(LPVOID pParam)
{	
	//开启更新检测线程
	CMainFrame *pMain = (CMainFrame*)pParam;
	//统计功能
	{
		{
			HINTERNET internetopen = InternetOpen(L"TC",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
			if (internetopen==NULL)
			{
				//::MessageBox(pMain->m_hWnd,L"此内测版本需要联网才可以使用",L"TC",0);
				//::exit(0);
				return 0;
			}
			CString csHelp;
			csHelp.Format(_T("http://www.tyuyan.com/tongjiversion.aspx?version=%s"),CTCRunTimeManager::g_TCVersion);
			HINTERNET internetopenurl = InternetOpenUrl(internetopen,csHelp,NULL,0,INTERNET_FLAG_RELOAD,0);
			if (internetopenurl==NULL)
			{    
				InternetCloseHandle(internetopen);
				//::MessageBox(pMain->m_hWnd,L"此内测版本需要联网才可以使用",L"TC",0);
				//::exit(0);
				return 0;
			}
			InternetCloseHandle(internetopenurl);
			InternetCloseHandle(internetopen);
		}
	}
	//先做更新提示验证
	{
		::Sleep(3*1000);
		HINTERNET internetopen = ::InternetOpen(L"",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		if (internetopen==NULL)
		{			
			goto news;
		}
		HINTERNET internetopenurl;
		CString downSite=_T("http://www.tyuyan.com/tcsoftversion.aspx?version=");
		downSite=downSite+CTCRunTimeManager::g_TCVersion;
		internetopenurl=InternetOpenUrl(internetopen,downSite,NULL,0,INTERNET_FLAG_RELOAD,0);
		if (internetopenurl==NULL)
		{
			InternetCloseHandle(internetopen);
			goto news;
		}

		BOOL internetreadfile;
		char buffer[1024]={0};
		DWORD byteread;
		internetreadfile=InternetReadFile(internetopenurl,buffer,1000,&byteread);
		if(::strlen(buffer)==0)
		{
			InternetCloseHandle(internetopenurl);
			InternetCloseHandle(internetopen);
			goto news;
		}

		InternetCloseHandle(internetopenurl);
		InternetCloseHandle(internetopen);

		CStringA retSite=buffer;
		retSite.Trim();
		if(retSite.Compare("null")==0)
			goto news;

		if(::strncmp(retSite,"http://www",::strlen("http://www"))==0)
		{
			CString idBuffer = CTCRunTimeManager::getMainSettingIni(_T("News"),_T("ID"));
			_GETNOWNEWS_ONPUT outData;
			outData.newsID==_wtoi(idBuffer);
			::wcscpy(outData.newsTitle,L"TC更新提示");
			::wcscpy(outData.newsBuff,L"TC发布最新版本,点击下载最新版本!");
			USES_CONVERSION;
			::wcscpy(outData.urlBuff,A2W(retSite));
			pMain->SendMessage(WM_MYDESKTOPALERT_MESSAGE,(WPARAM)&outData,0);
			goto news;
			/*CTCRunTimeManager::setMainSettingIni(L"setting",L"ver",CTCRunTimeManager::g_TCVersion);
			HANDLE Hhelp=FindWindow(_T("updata"),_T("TC更新"));
			if(Hhelp)
			{
				goto news;
			}
			int msgRet=::MessageBox(pMain->m_hWnd,_T("发现TC最新版本,是否马上更新"),_T("TC更新提示"),MB_OKCANCEL);
			if(IDOK==msgRet)
			{
				CString abTool;
				abTool.Format(_T("%s\\updata.exe"),CTCRunTimeManager::g_runtimePath);
				ShellExecute(
					pMain->m_hWnd, 
					_T("open"), 
					abTool, 
					TEXT(""),
					NULL,
					SW_SHOW);
			}*/
			goto news;
		}
	}

news:

	while(TRUE)
	{
		Sleep(60000);
		//_GETNOWNEWS_INPUT inData;
		_GETNOWNEWS_ONPUT outData;
		//获取本地记录的新闻编号
		CString idBuffer = CTCRunTimeManager::getMainSettingIni(_T("News"),_T("ID"));
		int lastNewID=_wtoi(idBuffer);
		CString url = _T("http://www.tyuyan.com/tools/news.txt");
		{
			HINTERNET internetopen = InternetOpen(L"TCNEWS",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);

			if (internetopen==NULL)
			{			
				continue;
			}
			HINTERNET internetopenurl = InternetOpenUrl(internetopen,url,NULL,0,INTERNET_FLAG_RELOAD,0);   

			if (internetopenurl==NULL)
			{    
				InternetCloseHandle(internetopen);
				continue;
			}

			TCHAR size[32]={0};
			DWORD numlen=sizeof(size), byteread=0;
			DWORD dwIndex=0;

			BOOL queryed = HttpQueryInfo(internetopenurl,HTTP_QUERY_CONTENT_LENGTH,size,&numlen,&dwIndex); //获取网页大小

			if(queryed == FALSE || wcslen(size) == 0)
			{
				InternetCloseHandle(internetopenurl);
				InternetCloseHandle(internetopen);
				continue;
			}
			int length=_wtoi(size);//获取长度
			if(length==0)     //长度为0 分配默认长度
			{
				length=1024;
			}
			wchar_t buffer[1024]={0}; //动态创建大小
			queryed=InternetReadFile(internetopenurl,buffer,length+1,&byteread);
			if(queryed == FALSE || wcslen(buffer) == 0)
			{
				InternetCloseHandle(internetopenurl);
				InternetCloseHandle(internetopen);
				continue;
			}

			InternetCloseHandle(internetopenurl);
			InternetCloseHandle(internetopen);
			wchar_t* wszString=buffer;
			//吃掉Unicode头
			wszString++;
			CString temp = wszString;
			temp.Trim();
			if(temp.IsEmpty())
				continue;

			int pos = temp.Find(_T("\r\n"));
			if(-1 != pos)
			{
				CString line = temp.Left(pos);
				line.Trim();
				if(line!=_T("TC新闻"))
					continue;
				temp = temp.Mid(pos+1,temp.GetLength()-pos-1);
			}
			else
				continue;

			pos = temp.Find(_T("\r\n"));
			if(-1 != pos)
			{
				CString line = temp.Left(pos);
				line.Trim();
				outData.newsID=_wtoi(line);
				if(outData.newsID==lastNewID)
					continue;
				temp = temp.Mid(pos+1,temp.GetLength()-pos-1);
			}
			else
				continue;

			pos = temp.Find(_T("\r\n"));
			if(-1 != pos)
			{
				CString line = temp.Left(pos);
				line.Trim();
				::wcscpy(outData.newsTitle,line);
				temp = temp.Mid(pos+1,temp.GetLength()-pos-1);
			}
			else
				continue;

			pos = temp.Find(_T("\r\n"));
			if(-1 != pos)
			{
				CString line = temp.Left(pos);
				line.Trim();
				::wcscpy(outData.newsBuff,line);
				temp = temp.Mid(pos+1,temp.GetLength()-pos-1);
			}
			else
				continue;

			pos = temp.Find(_T("\r\n"));
			if(-1 != pos)
			{
				CString line = temp.Left(pos);
				line.Trim();
				::wcscpy(outData.urlBuff,line);
				temp = temp.Mid(pos+1,temp.GetLength()-pos-1);
			}
			else
				continue;

			temp.Trim();
			if(temp!=_T("TC新闻结束"))
				continue;
		}
		//如果不等于本地记录的新闻编号,则弹出提示窗口
		pMain->SendMessage(WM_MYDESKTOPALERT_MESSAGE,(WPARAM)&outData,0);
		Sleep(2*60*1000);
	}
	return 0;
}

//执行线程,用了一个回调手法,为了主界面不卡死,开线程执行
UINT MyDebugThread(LPVOID pParam)
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	mainF->TCDebugRun();
	mainF->m_IsDebuging=false;
	mainF->m_IsInCompile=false;
	mainF->m_isVipBuild=false;
	//mainF->DrawMenuBar();
	return 0;
}

//防止输出窗口卡死的线程
UINT TCDebugThread(LPVOID pParam)
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	mainF->TCDebug();
	return 0;
}

//生成
UINT TCBuildProjThread(LPVOID pParam)
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	mainF->TCBuildProj();
	mainF->m_IsInCompile=false;
	mainF->m_isVipBuild=false;
	mainF->DrawMenuBar();
	return 0;
}

//编译
UINT TCCompileProjThread(LPVOID pParam)
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	mainF->TCCompileProj();
	mainF->m_IsInCompile=false;
	mainF->m_isVipBuild=false;
	mainF->DrawMenuBar();
	DeleteFolder(CTCRunTimeManager::g_ProjCompilePath,false);
	return 0;
}

//向输出窗口添加查找到的所有引用线程
UINT AddFindAllMsgThread( LPVOID pParam )
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	mainF->TCAddAllContentString();
	return 0;
}

// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

	BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
		ON_WM_CREATE()
		// 全局帮助命令
		//ON_COMMAND(ID_HELP_FINDER, &CMDIFrameWndEx::OnHelpFinder)
		ON_COMMAND(ID_HELP_FINDER, &CMainFrame::OnOpenTCHelp)
		//ON_COMMAND(ID_HELP, &CMainFrame::OnOpenTCHelp)
		//ON_COMMAND(ID_CONTEXT_HELP, &CMainFrame::OnOpenTCHelp)
		//ON_COMMAND(ID_DEFAULT_HELP, &CMainFrame::OnOpenTCHelp)

		ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
		ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
		ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
		//ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
		//ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
		ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
		ON_WM_SETTINGCHANGE()
		ON_MESSAGE(WM_MYTCDEBUG_MESSAGE, OnTCMyDebugPTMsg) //一个自定义的消息
		ON_MESSAGE(WM_MYTCOUTMSG_MESSAGE,OnTCOutMsg) //一个自定义的消息
		ON_MESSAGE(WM_MYCLOSESTARTVIEW_MESSAGE,OnTCMyCloseStartViewMsg) //一个自定义的消息
		ON_MESSAGE(WM_MYDESKTOPALERT_MESSAGE,OnTCDeskTopAlertWnd) //一个自定义的消息



		ON_UPDATE_COMMAND_UI(ID_VIEW_DECFUNWNDS, &CMainFrame::OnUpdateViewDecfunWnds)
		ON_COMMAND(ID_VIEW_DECFUNWNDS, &CMainFrame::OnViewDecfunWnds)
		ON_UPDATE_COMMAND_UI(ID_VIEW_MORE, &CMainFrame::OnUpdateViewMore)
		ON_UPDATE_COMMAND_UI(ID_VIEW_FILEVIEW, &CMainFrame::OnUpdateViewFileView)
		ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIESWND, &CMainFrame::OnUpdateViewPropertiesWnd)
		ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBOXWND, &CMainFrame::OnUpdateViewToolboxWnd)
		ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUTWND, &CMainFrame::OnUpdateViewOutputWnd)
		ON_UPDATE_COMMAND_UI(ID_TC_DEBUGRUN, &CMainFrame::OnUpdateTcDebugrun)
		ON_UPDATE_COMMAND_UI(ID_TC_DEBUGSTEP, &CMainFrame::OnUpdateTcDebugstep)
		ON_UPDATE_COMMAND_UI(ID_TC_DEBUGPRO, &CMainFrame::OnUpdateTcDebugpro)
		ON_UPDATE_COMMAND_UI(ID_TC_DEBUGSTOP, &CMainFrame::OnUpdateTcDebugstop)
		ON_UPDATE_COMMAND_UI(ID_TC_COMPILE, &CMainFrame::OnUpdateTcCompile)
		ON_UPDATE_COMMAND_UI(ID_TC_BUILD, &CMainFrame::OnUpdateTcBuild)
		ON_UPDATE_COMMAND_UI(ID_TC_CLEAR, &CMainFrame::OnUpdateTcClear)
		ON_COMMAND(ID_VIEW_PROPERTIESWND, &CMainFrame::OnViewPropertiesWnd)
		ON_COMMAND(ID_VIEW_TOOLBOXWND, &CMainFrame::OnViewToolboxWnd)
		ON_COMMAND(ID_TC_DEBUGPRO, &CMainFrame::OnTcDebugpro)
		ON_COMMAND(ID_TC_DEBUGSTEP, &CMainFrame::OnTcDebugstep)
		ON_COMMAND(ID_TC_DEBUGSTOP, &CMainFrame::OnTcDebugstop)
		ON_COMMAND(ID_TC_DEBUGRUN, &CMainFrame::OnTcDebugrun)
		ON_COMMAND(ID_VIEW_MORE, &CMainFrame::OnViewMore)
		ON_COMMAND(ID_VIEW_FILEVIEW, &CMainFrame::OnViewFileView)
		ON_COMMAND(ID_VIEW_OUTPUTWND, &CMainFrame::OnViewOutputWnd)
		ON_COMMAND(ID_VIEW_CLASSVIEW, &CMainFrame::OnViewClassView)
		ON_UPDATE_COMMAND_UI(ID_VIEW_CLASSVIEW, &CMainFrame::OnUpdateViewViewClassView)
		ON_COMMAND(ID_TC_COMPILE, &CMainFrame::OnTcCompile)
		ON_COMMAND(ID_TC_BUILD, &CMainFrame::OnTcBuild)
		ON_COMMAND(ID_TC_CLEAR, &CMainFrame::OnTcClear)
		ON_COMMAND(ID_TC_OPENBUILD, &CMainFrame::OnTcOpenbuild)
		ON_UPDATE_COMMAND_UI(ID_TC_OPENBUILD, &CMainFrame::OnUpdateTcOpenbuild)
		ON_UPDATE_COMMAND_UI(ID_TC_OPENDESIGNWIN, &CMainFrame::OnUpdateTcOpendesignwin)
		ON_COMMAND(ID_TC_OPENDESIGNWIN, &CMainFrame::OnTcOpendesignwin)
		ON_COMMAND(ID_TC_ABZHUAZHUA, &CMainFrame::OnTcAbzhuazhua)
		ON_WM_DESTROY()
		ON_COMMAND(ID_VIEW_STATUS_BAR, &CMainFrame::OnViewStatusBar)
		ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, &CMainFrame::OnUpdateViewStatusBar)
		ON_COMMAND(ID_TC_COMLOOK, &CMainFrame::OnTcComlook)
		ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CMainFrame::OnUpdateFileNew)
		ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CMainFrame::OnUpdateFileOpen)
		ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FILE1,ID_FILE_MRU_FILE5, &CMainFrame::OnUpdateFileMruFile)
		ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, &CMainFrame::OnUpdateFileClose)
		ON_COMMAND(ID_BUTTON_TCSETTING, &CMainFrame::OnButtonTcsetting)

		ON_COMMAND(ID_STATUSBAR_LINK, OnLink)
		ON_UPDATE_COMMAND_UI(ID_STATUSBAR_PANE1, OnUpdatePane1)
		ON_UPDATE_COMMAND_UI(ID_STATUSBAR_PANE2, OnUpdatePane2)
		ON_UPDATE_COMMAND_UI(ID_STATUSBAR_FILESIZE, OnUpdateFileSize)
		ON_COMMAND(ID_STATUSBAR_PANE2, OnLineJumper)
		ON_COMMAND(ID_TC_OPENSTARTVIEW, &CMainFrame::OnButtonOpenstartview)
		ON_COMMAND(ID_TC_OPENVIPVIEW, &CMainFrame::OnTcOpenvipview)
		ON_COMMAND(ID_UPDATE, &CMainFrame::OnUpdate)
	END_MESSAGE_MAP()

	// CMainFrame 构造/析构
	CMainFrame::CMainFrame()
	{
		// TODO: 在此添加成员初始化代码
		theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
		//theApp.m_nAppLook=ID_VIEW_APPLOOK_WINDOWS_7;

		m_IsInCompile=false;
		m_IsDebuging=false;
		/*  m_TCBPList.removeAllNode();*/
		////下面是创建调试用的内核对象和管道通讯对象//
		m_LibEventHandle=NULL;  //通知解析器的内核对象
		m_WinEventHandle=NULL;   //通知界面的内核对象
		////下面是创建调试用的内核对象和管道通讯对象//

		m_LoginHeartThread=NULL;

		m_TCDebugProcessHandle=NULL;
		m_isInBP=false;
		m_isVipBuild=false;
		m_currentViewType = TCVIEW_MAX;

		m_pCurrentWndID.Empty();

		m_DlgManager.reset();

		m_findInfo.empty();
		m_curFoundTFile.Empty();
	}

	CMainFrame::~CMainFrame()
	{
	}

	int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
			return -1;
		 
		// Create and show the splash screen.
		CSplashWnd::ShowSplashScreen(/*1000,*/ IDB_SPLASH_PNG, IDB_TC_START_GIF,this);

		BOOL bNameValid;
		// 基于持久值设置视觉管理器和样式
		OnApplicationLook(theApp.m_nAppLook);

		CMDITabInfo mdiTabParams;
		mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_SCROLLED; // 其他可用样式...
		mdiTabParams.m_bActiveTabCloseButton = TRUE;          // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
		mdiTabParams.m_bTabIcons = TRUE;                       // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
		mdiTabParams.m_bAutoColor = FALSE;                     // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
		mdiTabParams.m_bDocumentMenu = TRUE;                   // 在选项卡区域的右边缘启用文档菜单
		mdiTabParams.m_bTabCustomTooltips=TRUE;
		mdiTabParams.m_nTabBorderSize=2;

		EnableMDITabbedGroups(TRUE, mdiTabParams);

		/*	GetMDITabs().SetActiveTabColor(0xffff);
		GetMDITabs().SetActiveTabTextColor(0xff);*/

		m_wndRibbonBar.Create(this);
		m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

		//if (!m_wndStatusBar.Create(this))
		//{
		//    TRACE0("未能创建状态栏\n");
		//    return -1;      // 未能创建
		//}

		//CString strTitlePane1;
		//CString strTitlePane2;
		//bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
		//ASSERT(bNameValid);
		//bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
		//ASSERT(bNameValid);
		//m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
		//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

		//启用 Visual Studio 2005 样式停靠窗口oh行为
		CDockingManager::SetDockingMode(DT_SMART);
		//启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
		EnableAutoHidePanes(CBRS_ALIGN_ANY);

		// 创建标题栏:
		//if (!CreateCaptionBar())
		//{
		//    TRACE0("未能创建标题栏\n");
		//    return -1;      // 未能创建
		//}

		// 加载菜单项图像(不在任何标准工具栏上):
		CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

		// 创建停靠窗口
		if (!CreateDockingWindows())
		{
			TRACE0("未能创建停靠窗口\n");
			return -1;
		}

		// 创建状态栏:
		if (!CreateStatusBar())
		{
			TRACE0("未能创建状态栏\n");
			return -1;      // 未能创建状态栏
		}

		m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
		m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndFileView);
		CDockablePane* pTabbedBar = NULL;
		m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);

		m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
		//测试代码
		m_wndDesignToolBox.EnableDocking(CBRS_ALIGN_ANY);
		//测试代码
		DockPane(&m_wndProperties);

		//测试代码
		CDockablePane* pTabbedBarRight = NULL;
		m_wndDesignToolBox.AttachToTabWnd(&m_wndProperties, DM_SHOW, FALSE, &pTabbedBarRight);
		//测试代码


		m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndOutput);

		//这里是停靠,函数描述停靠窗
		m_wndFunDec.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndFunDec);

		CDockablePane* pTabbedBarBottom = NULL;
		m_wndFunDec.AttachToTabWnd(&m_wndOutput, DM_SHOW, FALSE, &pTabbedBarBottom);
		//这里是停靠,函数描述停靠窗

		//m_wndFunDec.ena

		// 启用增强的窗口管理对话框
		EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

		// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
		// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
		ModifyStyle(0, FWS_PREFIXTITLE);
		/////////////////////////这里是初始化调试的内核对象////////////////////////
		if(!m_LibEventHandle)
		{
			m_LibEventHandle = ::CreateEvent(NULL,FALSE,FALSE,L"MyMatchDebugLibEvent3015");
		}
		if(!m_WinEventHandle)
		{
			m_WinEventHandle = ::CreateEvent(NULL,FALSE,FALSE,L"MyMatchDebugWinEvent3015");
		}
		m_hFileMap = (CMyDebugInfor  *)CreateFileMapping(INVALID_HANDLE_VALUE, 
			NULL, PAGE_READWRITE, 0, sizeof(CMyDebugInfor), L"MyMatchDebugFILEMAP4001");
		if(m_hFileMap)
		{
			// 映射它到内存，取得共享内存的首地址
			m_TCDebugInfor = (CMyDebugInfor  *)MapViewOfFile(m_hFileMap,FILE_MAP_ALL_ACCESS,0,0,0);
		}
		else
			::MessageBox(0,L"创建映射文件失败",L"TC",0);
		/////////////////////////这里是初始化调试的内核对象////////////////////////
		////初始化界面设计的管理对象
		//m_TWinManager.reset();
		////初始化界面设计的管理对象
		//这个线程会一直运行,主要是用来等待调试过程的
		AfxBeginThread(MyDebugInforThread,this);
		m_TipEx.Create(this);
		m_TipExDebug.Create(this);
		CString csTitile;
		csTitile.Format(_T("TC%s"),CTCRunTimeManager::g_TCVersion);
		this->SetWindowText(csTitile);
		//m_wndSettingDia.Create(IDD_TCSETTINGDIA,this);
		CSplashWnd::HideSplashScreen();

		//桌面弹出对话框消息启动代码

		AfxBeginThread(TCAlertInfoThread,this);
		//桌面弹出对话框消息启动代码

		//更新线程
		//AfxBeginThread(UpdataCheckThread,this);

		return 0;
	}

	BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CMDIFrameWndEx::PreCreateWindow(cs) )
			return FALSE;
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式
		cs.style &=~ (LONG) FWS_ADDTOTITLE;
		return TRUE;
	}

	BOOL CMainFrame::CreateDockingWindows()
	{
		BOOL bNameValid;

		// 创建类视图
		CString strClassView;
		bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
		ASSERT(bNameValid);
		if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“类视图”窗口\n");
			return FALSE; // 未能创建
		}

		// 创建文件视图
		CString strFileView;
		bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
		ASSERT(bNameValid);
		if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“文件视图”窗口\n");
			return FALSE; // 未能创建
		}

		// 创建输出窗口
		CString strOutputWnd;
		bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
		ASSERT(bNameValid);
		if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建输出窗口\n");
			return FALSE; // 未能创建
		}

		//创建功能描述窗口
		CString strFunDecWnd;
		bNameValid = strFunDecWnd.LoadString(IDS_FUNDEC_WND);
		ASSERT(bNameValid);
		////////////////////////////////////////功能描述窗口的创建//////////////////////////////////
		if (!m_wndFunDec.Create(strFunDecWnd,this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_DECFUNWNDS, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“功能描述”窗口\n");
			return FALSE; // 未能创建
		}
		////////////////////////////////////////功能描述窗口的创建////////////////////////////////////

		// 创建属性窗口
		CString strPropertiesWnd;
		bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
		ASSERT(bNameValid);
		if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“属性”窗口\n");
			return FALSE; // 未能创建
		}

		CString strToolBoxWnd;
		bNameValid = strToolBoxWnd.LoadString(IDS_TOOLBOX_WND);
		ASSERT(bNameValid);
		////////////////////////////////////////工具箱的创建//////////////////////////////////
		if (!m_wndDesignToolBox.Create(strToolBoxWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_TOOLBOXWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“工具栏”窗口\n");
			return FALSE; // 未能创建
		}
		////////////////////////////////////////工具箱的创建////////////////////////////////////

		//这个是查找/替换窗口的创建
		m_wndFind.Create(IDD_TCTVIEWFIND,this);

		SetDockingWindowIcons(theApp.m_bHiColorIcons);
		return TRUE;
	}

	void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
	{
		HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndFileView.SetIcon(hFileViewIcon, FALSE);

		HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndClassView.SetIcon(hClassViewIcon, FALSE);

		HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

		HICON hFunDecWndIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_TOOLBOX_WND_HC : IDI_TOOLBOX_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndFunDec.SetIcon(hFunDecWndIcon, FALSE);

		HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

		HICON hToolboxBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_TOOLBOX_WND_HC : IDI_TOOLBOX_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndDesignToolBox.SetIcon(hToolboxBarIcon, FALSE);

		UpdateMDITabbedBarsIcons();
	}

	BOOL CMainFrame::CreateStatusBar()
	{
		BOOL bNameValid;
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBRS_BOTTOM;
		if (!m_wndStatusBar.Create(this,dwStyle,AFX_IDW_STATUS_BAR))
		{
			TRACE0("未能创建状态栏\n");
			return -1;      // 未能创建
		}

		//m_wndStatusBar.SetInformation(_T("就绪"));
		//m_wndStatusBar.ShowWindow( SW_SHOWNORMAL );
		CString strTitlePane1;
		CString strTitlePane2;
		bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
		ASSERT(bNameValid);
		bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
		ASSERT(bNameValid);

		m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, _T(""),TRUE), strTitlePane1);
		//添加分割条
		//m_wndStatusBar.AddSeparator();
		//添加右边的状态显示
		m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2,_T(""),FALSE), strTitlePane2);
		//添加分割条
		//m_wndStatusBar.AddSeparator();
		m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_FILESIZE, _T(""),TRUE), _T("文件大小:"));
		//添加分割条
		//m_wndStatusBar.AddSeparator();
		//TC官网链接
		m_wndStatusBar.AddExtendedElement(new CMFCRibbonLinkCtrl(ID_STATUSBAR_LINK, _T("www.tyuyan.com"), _T("http://www.tyuyan.com")), _T("TC官网"));
		return TRUE;
	}

	void CMainFrame::OnLink()
	{
		CMFCRibbonLinkCtrl* pLink = (CMFCRibbonLinkCtrl*) m_wndStatusBar.FindByID(ID_STATUSBAR_LINK);

		if (pLink != NULL)
		{
			pLink->OpenLink();
		}
	}

	void CMainFrame::OnLineJumper()
	{
		CTCEditorLineJumper dlg;
		int ret = dlg.DoModal();
		if( ret == IDOK )
		{
			CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

			// Get the active MDI child window.
			CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();

			// or CMDIChildWnd *pChild = pFrame->MDIGetActive();
			// Get the active view attached to the active MDI child window.
			CView *pView = (CView *) pChild->GetActiveView();
			if(pView->IsKindOf(RUNTIME_CLASS(CTCView)))
			{
				CTCView *pTView = (CTCView*)pView;
				ASSERT( pTView != NULL );
				pTView->TCActiveEditView();
			}
		}
	}

	void CMainFrame::OnUpdatePane1(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(TRUE);
	}

	void CMainFrame::OnUpdatePane2(CCmdUI* pCmdUI)
	{
		CView *pView = ((CTCApp*)AfxGetApp())->TCGetTCView();
		if(pView != NULL)
		{
			if(pView->IsKindOf(RUNTIME_CLASS(CTCView)))
			{
				CTCView *pTView = (CTCView*)pView;
				ASSERT( pTView != NULL );
				pCmdUI->Enable(TRUE);
			}
			else
				pCmdUI->Enable(FALSE);
		}	
		else
			pCmdUI->Enable(FALSE);

	}

	void CMainFrame::OnUpdateFileSize(CCmdUI* pCmdUI)
	{
		pCmdUI->Enable(TRUE);
	}

	void CMainFrame::ShowProgress(BOOL bShow)
	{
		if (bShow)
		{
			CMFCRibbonProgressBar* pProgressBar = new CMFCRibbonProgressBar(ID_STATUSBAR_PROGRESS,160);
			pProgressBar->SetText(_T("进度: "));
			pProgressBar->SetRange(0,100);
			pProgressBar->SetInfiniteMode(false);

			m_wndStatusBar.AddDynamicElement(pProgressBar);

		}
		else
		{
			m_wndStatusBar.RemoveElement(ID_STATUSBAR_PROGRESS);
		}

		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();

		CMFCPopupMenu::UpdateAllShadows();
	}

	void CMainFrame::TCSetProgressPos(int pos)
	{
		CMFCRibbonProgressBar* pProgress = DYNAMIC_DOWNCAST(CMFCRibbonProgressBar, m_wndStatusBar.FindElement(ID_STATUSBAR_PROGRESS));
		ASSERT_VALID(pProgress);
		pProgress->SetPos(pos,TRUE);
	}

	void CMainFrame::TCSetHeadPanelText(CString text,CString tiptext)
	{
		CMFCRibbonStatusBarPane* pPane = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_PANE1));
		ASSERT_VALID(pPane);

		pPane->SetVisible(TRUE);
		pPane->SetText(text);
		pPane->SetToolTipText(tiptext);
		pPane->Redraw();
		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();

	}

	void CMainFrame::TCSetFileInfo(UINT size)
	{
		CMFCRibbonStatusBarPane* pPanesize = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_FILESIZE));
		ASSERT_VALID(pPanesize);

		pPanesize->SetVisible(TRUE);

		//CString strSize;
		//strSize.Format(_T("文件大小: %d"),size);
		wchar_t strSize[32]={0};
		::wsprintf(strSize,_T("文件大小: %d"),size);
		pPanesize->SetText(strSize);
		pPanesize->SetToolTipText(strSize);
		pPanesize->Redraw();
		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();
	}

	LRESULT CMainFrame::OnTCDeskTopAlertWnd(WPARAM w, LPARAM l)
	{
		UpdateData ();
		_GETNOWNEWS_ONPUT *outPut = (_GETNOWNEWS_ONPUT*)w;
		CString newsTitle(outPut->newsTitle);
		CString newsUrlText(outPut->newsBuff);
		CString newUrl(outPut->urlBuff);
		CTCDesktopAlertWnd* pPopup = new CTCDesktopAlertWnd;
		pPopup->SetAnimationType (CMFCPopupMenu::ANIMATION_TYPE::FADE);
		pPopup->SetAnimationSpeed (100);
		pPopup->SetTransparency ((BYTE)255);
		pPopup->SetSmallCaption (TRUE);
		pPopup->SetAutoCloseTime (TRUE ? 60 * 60 * 1000 : 0);
		pPopup->Create (this, IDD_TCDESKTOPALERT, NULL, CPoint(-1,-1), RUNTIME_CLASS (CTCDesktopAlertDlg));
		CTCDesktopAlertDlg* pDia = (CTCDesktopAlertDlg*)pPopup->GetAlertDialog();
		if( pDia != NULL )
		{
			pDia->m_wndFrom.SetWindowTextW(newsTitle);
			pDia->m_btnRL.SetWindowTextW(newsUrlText);
			pDia->SetAlertUrl(newUrl);
		}
		HICON hIcon = (HICON)::LoadImage (::AfxGetResourceHandle (),MAKEINTRESOURCE (IDR_MAINFRAME),IMAGE_ICON, ::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		pPopup->SetIcon (hIcon, FALSE);
		pPopup->SetWindowTextW(_T("Message"));
		CString idStr;
		idStr.Format(_T("%d"),outPut->newsID);
		CTCRunTimeManager::setMainSettingIni(_T("News"),_T("ID"),idStr);
		return 1;
	}

	void CMainFrame::TCSetFileInfoVisible(BOOL isViewable)
	{
		CMFCRibbonStatusBarPane* pPanesize = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_FILESIZE));
		ASSERT_VALID(pPanesize);
		CMFCRibbonStatusBarPane* pPane = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_PANE2));
		ASSERT_VALID(pPane);

		pPanesize->SetVisible(isViewable);
		pPane->SetVisible(isViewable);
		pPanesize->Redraw();
		pPane->Redraw();
		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();
	}

	void CMainFrame::TCSetStatusRowAndCol(int row,int col)
	{
		CMFCRibbonStatusBarPane* pPane = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_PANE2));
		ASSERT_VALID(pPane);
		if(pPane)
		{
			wchar_t str[32]={0};
			::wsprintf(str,_T("行: %d   列: %d"),row,col);
		//CString str;
		//str.Format(_T("行: %d   列: %d"),row,col);
		pPane->SetVisible(TRUE);
		pPane->SetText(str);
		pPane->SetToolTipText(_T("光标所在位置"));
		pPane->Redraw();
		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();
		}
	}

	//BOOL CMainFrame::CreateCaptionBar()
	//{
	//    if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	//     {
	//         TRACE0("未能创建标题栏\n");
	//         return FALSE;
	//     }
	//		m_wndCaptionBar.SetMargin(5.5);
	//BOOL bNameValid;

	//CString strTemp, strTemp2;
	//bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	//ASSERT(bNameValid);
	//m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);

	//bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	//ASSERT(bNameValid);
	//m_wndCaptionBar.SetButtonToolTip(strTemp);

	//bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	//ASSERT(bNameValid);
	//m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	//m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	//bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	//ASSERT(bNameValid);
	//bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	//ASSERT(bNameValid);
	//m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	//     return TRUE;
	// }

	// CMainFrame 诊断

#ifdef _DEBUG
	void CMainFrame::AssertValid() const
	{
		CMDIFrameWndEx::AssertValid();
	}

	void CMainFrame::Dump(CDumpContext& dc) const
	{
		CMDIFrameWndEx::Dump(dc);
	}
#endif //_DEBUG


	// CMainFrame 消息处理程序

	void CMainFrame::OnWindowManager()
	{
		ShowWindowsDialog();
	}

	void CMainFrame::OnApplicationLook(UINT id)
	{
		CWaitCursor wait;

		theApp.m_nAppLook = id;

		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_WIN_2000:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_OFF_XP:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_WIN_XP:
			CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_OFF_2003:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_VS_2005:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);

			break;

		case ID_VIEW_APPLOOK_VS_2008:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		case ID_VIEW_APPLOOK_WINDOWS_7:
			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
			//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
			//   CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
			break;

		default:
			switch (theApp.m_nAppLook)
			{
			case ID_VIEW_APPLOOK_OFF_2007_BLUE:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
				break;

			case ID_VIEW_APPLOOK_OFF_2007_BLACK:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
				break;

			case ID_VIEW_APPLOOK_OFF_2007_SILVER:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
				break;

			case ID_VIEW_APPLOOK_OFF_2007_AQUA:
				CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
				break;
			}

			CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
			CDockingManager::SetDockingMode(DT_SMART);
			m_wndRibbonBar.SetWindows7Look(FALSE);
		}

		RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

		theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
	}

	void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
	{
		pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
	}

	//void CMainFrame::OnViewCaptionBar()
	//{
	//    m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	//    RecalcLayout(FALSE);
	//}

	//void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
	//{
	//    pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
	// }

	void CMainFrame::OnOptions()
	{
		CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
		ASSERT(pOptionsDlg != NULL);
		pOptionsDlg->DoModal();
		delete pOptionsDlg;
	}

	void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
	{
		CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
		m_wndOutput.UpdateFonts();
	}

	LRESULT CMainFrame::OnTCMyDebugPTMsg(WPARAM wParam, LPARAM lParam)
	{
		//得到具体的行
		int line=(int)wParam;
		//得到当前文件
		wchar_t * curfilename=(wchar_t*)lParam;
		//((CTCApp*)AfxGetApp())->m_pDocTemplateTc->OpenDocumentFile(curfilename);
		//CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
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
				if(IsOneFile(curfilename,pView->m_ScintillaWnd.GetFileName()))
				{
					CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
					pFWnd->MDIActivate();
					//pView->ActivateTopParent();
					pView->TCDrawCurrentBP(line);
					int index=pView->TCLineIndex(line-1);
					pView->TCGotoPos(index);
					CString outMsg;
					outMsg.Format(_T("程序暂停在 %d 行 \t文件:%s"),line,curfilename);
					this->TCOutMsgAddString(outMsg);
					return TRUE; // 文档已经被打开
				}
			}
		}
		pDoc=(CTCDoc*)pDocTmp->OpenDocumentFile(curfilename,FALSE,TRUE);
		CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		// Get the active MDI child window.
		CMDIChildWnd *pChild =(CMDIChildWnd *)pFrame->GetActiveFrame();
		// Get the active view attached to the active MDI child window.
		pView = (CTCView *) pChild->GetActiveView();
		pView->TCDrawCurrentBP(line);
		int index=pView->TCLineIndex(line-1);
		pView->TCGotoPos(index);
		CString outMsg;
		outMsg.Format(_T("程序暂停在 %d 行 \t文件:%s"),line,curfilename);
		this->TCOutMsgAddString(outMsg);
		return 0;
	}

	void CMainFrame::ResetPropertyCombox()
	{
		m_wndProperties.ResetPropertiesCombox();
	}

	//重新加载项目文件
	void CMainFrame::ProjFileViewReload()
	{
		m_wndFileView.ReloadProj();
	}

	//设置项目文件的属性
	void CMainFrame::SetProjProperty(CString name,CString path,BOOL upDate)
	{
		m_wndProperties.SetProjProperty(name,path,upDate);
	}

	void CMainFrame::SetTWinDiaProperty(CString name,const CDiaTCDesign * const dlg,BOOL upDate)
	{
		m_wndProperties.SetTWinDiaProperty(name,dlg,upDate);
	}

	void CMainFrame::SetRcNodeDiaProperty(CString name)
	{
		m_wndProperties.SetRcNodeDiaProperty(name);
	}

	//显示static控件的属性
	void CMainFrame::SetTWinStaticProperty(CString id, CTCStatic *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinStaticProperty(id,ctl,upDate);
	}
	//显示group控件的属性
	void CMainFrame::SetTWinGroupProperty(CString id, CTCGroupBox *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinGroupProperty(id,ctl,upDate);
	}
	//显示按钮控件的属性
	void CMainFrame::SetTWinButtonProperty(CString id, CTCButton *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinButtonProperty(id,ctl,upDate);
	}
	//显示edit控件的属性
	void CMainFrame::SetTWinEditProperty(CString id, CTCEdit *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinEditProperty(id,ctl,upDate);
	}
	//显示Check控件的属性   
	void CMainFrame::SetTWinCheckProperty(CString id, CTCCheck *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinCheckProperty(id,ctl,upDate);
	}
	//显示Radio控件的属性
	void CMainFrame::SetTWinRadioProperty(CString id, CTCRadio *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinRadioProperty(id,ctl,upDate);
	}
	void CMainFrame::SetTWinComboxProperty(CString id, CTCComboBox *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinComboxProperty(id,ctl,upDate);
	}
	void CMainFrame::SetTWinPictureProperty(CString id, CTCPicture *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinPictureProperty(id,ctl,upDate);
	}
	//显示progress控件的属性
	void CMainFrame::SetTWinProgressProperty(CString id, CTCProgress *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinProgressProperty(id,ctl,upDate);
	}

	void CMainFrame::SetTWinPathProperty(CString id, CTCBrowse *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinPathProperty(id,ctl,upDate);
	}
	//显示Slider的属性
	void CMainFrame::SetTWinSliderProperty(CString id, CTCSlider *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinSliderProperty(id,ctl,upDate);
	}
	//显示Slider的属性
	void CMainFrame::SetTWinDateProperty(CString id, CTCDate *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinDateProperty(id,ctl,upDate);
	}
	//显示List的属性
	void CMainFrame::SetTWinListProperty(CString id, CTCListBox *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinListProperty(id,ctl,upDate);
	}
	//显示Tab的属性
	void CMainFrame::SetTWinTableProperty(CString id, CTCTab*  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinTableProperty(id,ctl,upDate);
	}
	//显示Timer的属性
	void CMainFrame::SetTWinTimerProperty(CString id, CTCTimer*  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinTimerProperty(id,ctl,upDate);
	}

	//显示HotKey的属性
	void CMainFrame::SetTWinHotKeyProperty(CString id, CTCHotKeyCtrl *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinHotKeyProperty(id,ctl,upDate);
	}

	//显示Explorer的属性
	void CMainFrame::SetTWinExplorerProperty(CString id, CTCExplorer *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinExplorerProperty(id,ctl,upDate);
	}

	//显示G日的Ctrl的属性
	void CMainFrame::SetTWinGridCtrlProperty(CString id, CGridCtrl *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinGridCtrlProperty(id,ctl,upDate);
	}

	void CMainFrame::TCUpdateProperty(TCTOOLTYPE type,CString ctrlID,CWnd *pCtrl,BOOL update)
	{
		switch(type)
		{
		case TCTT_BUTTON:
			((CTCButton*)pCtrl)->TCSetID(ctrlID);
			SetTWinButtonProperty(ctrlID,(CTCButton*)pCtrl,update);
			break;
		case TCTT_STATIC:
			SetTWinStaticProperty(ctrlID,(CTCStatic*)pCtrl,update);
			break;
		case TCTT_EDIT:
			SetTWinEditProperty(ctrlID,(CTCEdit*)pCtrl,update);
			break;
		case TCTT_RADIO:
			SetTWinRadioProperty(ctrlID,(CTCRadio*)pCtrl,update);
			break;
		case TCTT_CHECK:
			SetTWinCheckProperty(ctrlID,(CTCCheck*)pCtrl,update);
			break;
		case TCTT_COMBOBOX:
			SetTWinComboxProperty(ctrlID,(CTCComboBox*)pCtrl,update);
			break;
		case TCTT_GROUP:
			SetTWinGroupProperty(ctrlID,(CTCGroupBox*)pCtrl,update);
			break;
		case TCTT_LISTBOX:
			SetTWinListProperty(ctrlID,(CTCListBox*)pCtrl,update);
			break;
		case TCTT_PROGRESS:
			SetTWinProgressProperty(ctrlID,(CTCProgress*)pCtrl,update);
			break;
		case TCTT_DATE:
			SetTWinDateProperty(ctrlID,(CTCDate*)pCtrl,update);
			break;
		case TCTT_PICTURE:
			SetTWinPictureProperty(ctrlID,(CTCPicture*)pCtrl,update);
			break;
		case TCTT_SLIDER:
			SetTWinSliderProperty(ctrlID,(CTCSlider*)pCtrl,update);
			break;
		case TCTT_PATH:
			SetTWinPathProperty(ctrlID,(CTCBrowse*)pCtrl,update);
			break;
		case TCTT_HOTKEY:
			SetTWinHotKeyProperty(ctrlID,(CTCHotKeyCtrl*)pCtrl,update);
			break;
		case TCTT_EXPLORER:
			SetTWinExplorerProperty(ctrlID,(CTCExplorer*)pCtrl,update);
			break;
		case TCTT_GRIDCTRL:
			SetTWinGridCtrlProperty(ctrlID,(CGridCtrl*)pCtrl,update);
			break;
		default:
			break;
		}
	}

	//当修改属性框时,修改项目管理框
	bool CMainFrame::FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd)
	{
		return m_wndFileView.FileReNameByPropWnd(oldPatn,newName,FileOrWnd);
	}

	//修改指定项的属性值，避免重绘
	void CMainFrame::SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue)
	{
		m_wndProperties.SetTheProperty(protype,name,title,newValue);
	}

	///////////////////////////添加控件名称到属性窗口下拉框///////////////
	void CMainFrame::InsertCtrlToProCom(CString ctrlName)
	{
		m_wndProperties.InsertCtrlToProCom(ctrlName);
	}

	void CMainFrame::TCActiveFindWnd()
	{
		m_wndFind.ShowWindow(TRUE);
		m_wndFind.CenterWindow();
	}

	//功能描述
	void CMainFrame::SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample)
	{
		m_wndFunDec.SetDecText(csNS,csFunName,csFunProto,csFunDes,csExample);
	}

	//编译项目
	bool CMainFrame::TCCompileProj()
	{
		TCClearOutMsg();
		TCClearAllContentMsg();
		TCClearProj();
		CString csComplie;
		csComplie.Format(_T("========== 编译开始: 项目名称: %s =========="),CTCRunTimeManager::g_ProjName);
		m_wndOutput.TCOutMsgAddString(csComplie);

		//是否加载了项目
		if(!CTCRunTimeManager::g_IsLoadProj)
			return false;

		::SetCurrentDirectory(CTCRunTimeManager::g_ProjSourcePath);

		//这段代码是遍历所有的文档文件并保存的,主要是用来躲避保存对话框的,保存当前文档
		m_wndOutput.TCOutMsgAddString(_T(">保存所有文档"));

		// TCSaveAllDoc();

		CString csEnterFile=CTCRunTimeManager::g_projEnterFile;

		if (!::PathFileExists(csEnterFile))
		{
			m_wndOutput.TCOutMsgAddString(_T(">没有获得项目启动文件"),true);
			return FALSE;
		}
		//解析器
		MyParser parser;   
		//m_wndOutput.TCOutMsgAddString(_T("生成解析器"));

		//错误报告对象
		DomTreeErrorReporter errReporter; 
		//Set Error ob
		parser.setErrorHandler(&errReporter);


		bool errorsOccured = false;
		try
		{
			m_wndOutput.TCOutMsgAddString(_T(">开始编译..."));
			m_wndOutput.TCOutMsgAddString(_T(">编译资源文件..."));
			if(CopyFolderAllFiles(CTCRunTimeManager::g_ProjSourcePath+L"\\资源",CTCRunTimeManager::g_ProjCompilePath))
			{
				//告诉解析器,相关的路径
				MySerializeBlockList::setCompilePath(CTCRunTimeManager::g_ProjCompilePath,CTCRunTimeManager::g_ProjSourcePath);
				parser.parse(csEnterFile);
			}
			else
			{
				errorsOccured=true;
				m_wndOutput.TCOutMsgAddString(_T(">编译资源文件出错,请检查是否有文件正在使用"),true);
			}
		}
		catch (const MyXMLExpError& e)  // Add Back 
		{
			CString csHelp;
			csHelp.Format(_T(">错误:%s 行号:%d 错误信息:%s"),e.fSrcFile,e.fSrcLine,e.fMsg);
			m_wndOutput.TCOutMsgAddString(csHelp,true,e.fSrcFile,e.fSrcLine);
			errorsOccured = true;
		}
		catch (const OutOfMemoryException& )
		{
			//错误
			////////////////////////////////////////////////////////////
			m_wndOutput.TCOutMsgAddString(_T(">错误:OutOfMemoryException"),true);
			errorsOccured = true;
		}
		catch (const XMLException& e)
		{
			//错误
			//////////////////////////////////////////////////////////////////////////////////
			CString csHelp;
			csHelp.Format(_T(">An error occurred during parsing   Message: %s"),XMLString::transcode(e.getMessage()));
			m_wndOutput.TCOutMsgAddString(csHelp,true);
			errorsOccured = true;
		}
		catch (const DOMException& e)
		{
			//错误
			CString csHelp;
			csHelp.Format(_T("DOM Error during parsing"));
			m_wndOutput.TCOutMsgAddString(csHelp,true);
			//std::cerr << "\nDOM Error during parsing: '" << pDoc->GetFilePath() << "'\n"
			//	<< "DOMException code is:  " << e.code << std::endl;
			//m_wndEditOutPut.AddErrorMsg(_T("\nDOM Error during parsing: '"));
			//m_wndEditOutPut.AddErrorMsg(_T("'\n"));
			//m_wndEditOutPut.AddErrorMsg(_T("DOMException code is:  "));
			//m_wndEditOutPut.AddErrorMsgI(e.code);

			//if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
			//{
			//    //m_wndEditOutPut.AddErrorMsg(_T("Message is: "));
			//    //m_wndEditOutPut.AddErrorMsg(XMLString::transcode(e.getMessage()));
			//}
			errorsOccured = true;
		}
		catch (...)
		{
			CString csHelp;
			csHelp.Format(_T(">解析过程中发生了未知的错误!"));
			m_wndOutput.TCOutMsgAddString(csHelp,true);
			errorsOccured = true;
		}	
		if(errorsOccured)
		{
			m_wndOutput.TCOutMsgAddString(_T(">有语法错误,请查证后重试!"));
		}
		else
		{
			m_wndOutput.TCOutMsgAddString(_T(">代码编辑成功"));
			CString csTempFile;
			csTempFile.Format(_T("%s\\sourseTemp.bak"),CTCRunTimeManager::g_ProjCompilePath);

			if(m_isVipBuild)
			{
				unsigned char fixedkey[16]={71,122,122,81,49,53,85,56,74,72,81,53,55,81,122,76};

				MySerializeBlockList::serializeList(csTempFile,fixedkey);
			}
			else
				MySerializeBlockList::serializeList(csTempFile,0);
		}

		//设置回来
		::SetCurrentDirectory(CTCRunTimeManager::g_runtimePath);

		return !errorsOccured;
	}

	//生成项目
	bool CMainFrame::TCBuildProj()
	{
		//先编译,并产生临时源文件
		if(!TCCompileProj())
			return false;

		//创建备份项目
		TCBackUpProject();

		//生成EXE文件
		CString csHelpExe;
		csHelpExe.Format(_T("%s\\%s.exe"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);

		m_wndOutput.TCOutMsgAddString(_T(">开始生成..."));

		///////////////////////////////////现在开始显示进度条/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_SHOW);
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,10);
		//ShowProgress(TRUE);
		//TCSetProgressPos(10);
		//现在开始显示进度条

		
		if(m_isVipBuild)
		{
			if(!CopyFile(CTCRunTimeManager::g_ExeBackFileVIP,csHelpExe,false))
			{
				m_wndOutput.TCOutMsgAddString(_T(">生成EXE失败,原因可能是EXE文件不存在或者正在运行"),true);
				//失败则销毁进度条
				TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_HIDE);
				//ShowProgress(FALSE);
				return false;
			}
		}
		else
		{
			if(!CopyFile(CTCRunTimeManager::g_ExeBackFile,csHelpExe,false))
			{
				m_wndOutput.TCOutMsgAddString(_T(">生成EXE失败,原因可能是EXE文件不存在或者正在运行"),true);
				//失败则销毁进度条
				TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_HIDE);
				//ShowProgress(FALSE);
				return false;
			}
		}

		//压缩命令
		CString csCompressCmd=_T("u*"),csCompressFiles;

		//这个是生成的压缩文件的目录
		CString csDestFiles;
		csDestFiles.Format(_T("%s\\TC.lc"),CTCRunTimeManager::g_ProjBuildPath);

		CString csDextTll;
		csDextTll.Format(_T("%s\\%s.tll"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);

		//开始打包附件
		m_wndOutput.TCOutMsgAddString(_T(">开始打包附件..."));

		///////////////////////////////////现在开始显示进度条/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,20);
		//现在开始显示进度条

		//开始打包附件
		m_wndOutput.TCOutMsgAddString(_T(">打包源文件"));

		//先压缩源文件
		CString csTempFile;
		csTempFile.Format(_T("%s\\sourseTemp.bak"),CTCRunTimeManager::g_ProjBuildPath);

		//开始打包附件
		m_wndOutput.TCOutMsgAddString(_T(">打包资源文件"));
		CString lpPath;
		lpPath.Format(_T("%s\\"),CTCRunTimeManager::g_ProjCompilePath);
		{
			TCHAR szFind[MAX_PATH]={0};
			TCHAR szFile[MAX_PATH]={0};
			WIN32_FIND_DATA FindFileData;
			wcscpy_s(szFind,lpPath);
			wcscat_s(szFind,_T("*.*"));
			HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
			if(INVALID_HANDLE_VALUE == hFind)
				return TRUE;
			while(TRUE)
			{
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(FindFileData.cFileName[0]!=_T('.'))
					{

					}
				}
				else
				{
					CString csRCFiles;
					csRCFiles.Format(_T("%s%s"),lpPath,FindFileData.cFileName);

					//然后压缩资源文件
					csCompressFiles=csCompressFiles+_T("*");
					csCompressFiles=csCompressFiles+csRCFiles;
				}
				if(!FindNextFile(hFind,&FindFileData))
					break;
			}
			FindClose(hFind);
		}

		CString lcxTempFile=csCompressFiles;

		CString csLibFile;
		csLibFile.Format(_T("%s\\matchLib.Dll"),CTCRunTimeManager::g_runtimePath);
		csCompressFiles=csCompressFiles+_T("*");
		csCompressFiles=csCompressFiles+csLibFile;

		CString csDllFile;
		csDllFile.Format(_T("%s\\matchDll.Dll"),CTCRunTimeManager::g_runtimePath);
		csCompressFiles=csCompressFiles+_T("*");
		csCompressFiles=csCompressFiles+csDllFile;

		//打包附件完毕
		m_wndOutput.TCOutMsgAddString(_T(">附件打包完成"));

		///////////////////////////////////现在开始显示进度条/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,40);
		//现在开始显示进度条

		//////////////////////////////////////这里是用来打包LCX文件,平台使用的文件////////////////////
		CString csLcxCmd=csCompressCmd+csDextTll+lcxTempFile;
		compress7z(csLcxCmd);

		//这里是压缩文件头格式加密处理
		{
			CFile f;
			f.Open(csDextTll,CFile::modeReadWrite);
			DWORD toRand=f.GetLength()+99;
			DWORD toTllHead2=::rand();
			//wchar_t lcBeg=0XEEFF;
			f.Write(&toRand,sizeof(toRand));
			f.Write(&toTllHead2,sizeof(toTllHead2));
			f.Close();
		}
		CString csLcxFile;
		csLcxFile.Format(_T("%s\\%s.lcx"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		XMLCh lcxFile[MAX_PATH]={0};

		wchar_t lcxKey[18]={116,121,117,121,97,110,35,36,37,48,50,48,54,100,108,108,0};
		//"tyuyan#$%0206dll"
		CTCCode::EnCodeFile(csDextTll.GetBuffer(),lcxKey,csLcxFile.GetBuffer());
		//////////////////////////////////////这里是用来打包LCX文件,平台使用的文件////////////////////
		compress7z(csCompressCmd+csDestFiles+csCompressFiles);

		DeleteFolder(CTCRunTimeManager::g_ProjCompilePath,false);
		///////////////////////////////////现在开始显示进度条/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,70);
		//现在开始显示进度条

		CString csToBuildExe;
		csToBuildExe.Format(_T(">%s 生成成功"),csHelpExe);

		///////////////////////////////////现在开始显示进度条/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,80);
		//现在开始显示进度条

		m_wndOutput.TCOutMsgAddString(csToBuildExe);
		{
			CFile f;
			f.Open(csDestFiles,CFile::modeReadWrite);
			//f.Seek(2,CFile::begin);
			DWORD size;
			char *string;
			size=f.GetLength();
			string =new char[size];
			memset(string ,0,size*sizeof(char)); 

			//这里是处理压缩文件头,用来处理压缩文件
			DWORD toTllHead1=::GetTickCount()*::GetTickCount();
			//wchar_t lcBeg=0XEEFF;ss
			f.Write(&toTllHead1,sizeof(toTllHead1));
			//wchar_t lcBeg=0XEEFF;
			//f.Write(&lcBeg,sizeof(wchar_t));
			f.SeekToBegin();
			//这里是压缩文件头的原始格式
			//string[0]=0x37;
			//string[1]=0x7a;
			//
			//string[0]=0xFEFF;
			f.Read(string,size);

			///////////////////////////////////现在开始显示进度条/////////////////////////////////////
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,95);
			//现在开始显示进度条
			//替换资源文件
			HANDLE   hUpdate=BeginUpdateResource(csHelpExe,FALSE);   
			int rusult;
			rusult=UpdateResource(
				hUpdate,
				_T("TCLC"),   
				MAKEINTRESOURCE(139),   
				MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),   
				string, //写入的数据   
				size   //数据的字节数
				);
			rusult=EndUpdateResource(hUpdate, FALSE);
			f.Close();
			///////////////////////////////////现在开始显示进度条/////////////////////////////////////
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,100);
			//TCSetProgressPos(100);
			//销毁进度条
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_HIDE);
			//ShowProgress(FALSE);
			////////////////////////////////////
			delete string;

			if(rusult==0)
			{
				m_wndOutput.TCOutMsgAddString(_T(">生成EXE失败,无法打包EXE"),true);
				return false;
			}
		}

		//EXE的图标替换
		bool needToChangeExeIcon=false;
		//窗口图标
		CTCDesignView *pCurrent=(CTCDesignView*)((CTCApp*)AfxGetApp())->TCGetTCDesignView(CTCRunTimeManager::g_ProjName);
		if(pCurrent != NULL)
		{		
			needToChangeExeIcon = (pCurrent->m_DiaDesign.m_TcDlgConfig.m_icon.GetLength()==0);
			if(!needToChangeExeIcon)
			{
				CTCExeVersionManager::changedExeIcon(csHelpExe,pCurrent->m_DiaDesign.m_TcDlgConfig.m_icon);
				::WritePrivateProfileString(_T("ProjInfo"),_T("ExeIco"),pCurrent->m_DiaDesign.m_TcDlgConfig.m_icon,CTCRunTimeManager::g_ProjFilePath);
			}		
		}

		//生成版本信息
		CTCExeVersionManager::changedExeVerInfo(csHelpExe);
		if (::PathFileExists(csTempFile))
		{
			DeleteFile(csTempFile);
		}
		return true;
	}

	//清理项目
	bool CMainFrame::TCClearProj()
	{
		//生成EXE文件
		if(CTCRunTimeManager::g_ProjCompilePath.IsEmpty()||(!::PathFileExists(CTCRunTimeManager::g_ProjCompilePath)))
			return false;
		TCClearOutMsg();
		TCClearAllContentMsg();
		m_wndOutput.TCOutMsgAddString(_T(">清理开始..."));
		//清理编译文件夹
		DeleteFolder(CTCRunTimeManager::g_ProjCompilePath,false);
		//清理生成过程中的中间文件
		CString csHelpExe;
		csHelpExe.Format(_T("%s\\%s.exe"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		if (::PathFileExists(csHelpExe))
		{
			DeleteFile(csHelpExe);  
			m_wndOutput.TCOutMsgAddString(_T(">成功清理生成EXE!"));
		}

		CString csTempFile;
		csTempFile.Format(_T("%s\\sourseTemp.bak"),CTCRunTimeManager::g_ProjBuildPath);
		if (::PathFileExists(csTempFile))
		{
			DeleteFile(csTempFile);  
			m_wndOutput.TCOutMsgAddString(_T(">成功清理临时中间文件!"));
		}

		CString csTempRCFile;
		csTempRCFile.Format(_T("%s\\TC.lc"),CTCRunTimeManager::g_ProjBuildPath);
		if (::PathFileExists(csTempRCFile))
		{
			DeleteFile(csTempRCFile);  
			m_wndOutput.TCOutMsgAddString(_T(">成功清理临时资源文件!"));
		}

		CString csDextTll;
		csDextTll.Format(_T("%s\\%s.tll"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		if (::PathFileExists(csDextTll))
		{
			DeleteFile(csDextTll);  
			m_wndOutput.TCOutMsgAddString(_T(">成功清理tll文件!"));
		}

		CString csLcxFile;
		csLcxFile.Format(_T("%s\\%s.lcx"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		if (::PathFileExists(csLcxFile))
		{
			DeleteFile(csLcxFile);  
			m_wndOutput.TCOutMsgAddString(_T(">成功清理lcx文件!"));
		}
		m_wndOutput.TCOutMsgAddString(_T(">清理完毕"));
		return true;
	}




	//自己写了一个保存所有文档的方法,主要是为了避免保存对话框的弹出
	void CMainFrame::TCSaveAllDoc()
	{
		//保存T文件
		CTCDoc * pDoc;
		POSITION pos =((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetFirstDocPosition();
		if(pos!=NULL)
		{
			pDoc = (CTCDoc *)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
			if(pDoc->IsModified())
				pDoc->DoFileSave();
			while(pos!=NULL)
			{
				pDoc = (CTCDoc *)(((CTCApp*)AfxGetApp())->m_pDocTemplateTc->GetNextDoc(pos));
				if(pDoc->IsModified())
					pDoc->DoFileSave();
			}
		}

		//保存界面界面
		CMultiDocTemplate *pDocTmp=((CTCApp*)AfxGetApp())->m_pDocTemplateTWin;
		POSITION designPos=pDocTmp->GetFirstDocPosition();
		CTCDesignView *pDesignView=NULL;
		CTCDesignDoc* pDesignDoc = NULL;
		while (NULL != designPos)
		{
			pDesignDoc = (CTCDesignDoc*)pDocTmp->GetNextDoc(designPos);
			if(pDesignDoc==NULL)
				continue;
			POSITION posView = pDesignDoc->GetFirstViewPosition();
			while (NULL != posView)
			{
				pDesignView = (CTCDesignView *)pDesignDoc->GetNextView(posView);
				if (NULL == pDesignView )
					continue;
				pDesignView->OnFileSave();
			}
		}
	}

	void CMainFrame::TCClearOutMsg()
	{
		m_wndOutput.TCClearOutMsg();
	}

	void CMainFrame::TCClearAllContentMsg()
	{
		m_wndOutput.TCClearAllContentWindow();
	}

	void CMainFrame::TCOutMsgAddString(CString add)
	{
		m_wndOutput.TCOutMsgAddString(add);
	}

	void CMainFrame::TCFindAllContent()
	{
		m_wndOutput.TCClearAllContentWindow();
		//激活引用窗口
		m_wndOutput.TCSetAllContentWinActive();
		AfxBeginThread(AddFindAllMsgThread,this);
	}

	void CMainFrame::TCAddAllContentString()
	{ 
		for( int count = 0; count < m_findInfo.size(); count++ )
		{
			CString line;
			line.Format(_T("行号:(%d)--%s"),m_findInfo[count].lineNumber,m_findInfo[count].lindStr);
			m_wndOutput.TCAddAllContentWindow(line);
		}
	}

	//添加一个断点
	bool CMainFrame::TCDebugAddBP(CString path,int line)
	{
		if((!path)||(line<MATCH_STARTLINE))
		{
			throw;
			return false;
		}

		if(m_TCDebugInfor->fLeng>=MATCH_MAXBREAKBPNUM)
			return false;

		m_TCDebugInfor->fList[m_TCDebugInfor->fLeng].setFielName(path);
		m_TCDebugInfor->fList[m_TCDebugInfor->fLeng].setLineNum(line);

		m_TCDebugInfor->fLeng++;

		return true;
	}

	//删除一个断点
	bool CMainFrame::TCDebugDeleteBP(CString path,int line)
	{
		if(0==m_TCDebugInfor->fLeng)
			return false;

		int i=0;
		bool isFind=false;
		for(;i<m_TCDebugInfor->fLeng;i++)
		{
			if((0==::lstrcmp(path,m_TCDebugInfor->fList[i].getDebugFileName()))&&(line==m_TCDebugInfor->fList[i].getDebugLineNum()))
			{
				isFind=true;
				break;
			}
		}

		if(!isFind)
			return false;

		for(int j=i;j<m_TCDebugInfor->fLeng-1;j++)
		{
			m_TCDebugInfor->fList[j].setFielName(m_TCDebugInfor->fList[j+1].getDebugFileName());
			m_TCDebugInfor->fList[j].setLineNum(m_TCDebugInfor->fList[j+1].getDebugLineNum());
		}
		m_TCDebugInfor->fLeng--;
		return true;
	}

	//删除所有断点
	void CMainFrame::TCDebugDeleteAllBP()
	{
		m_TCDebugInfor->fLeng=0;
		::memset(m_TCDebugInfor->fList,0,sizeof(MyBreakPointInfo)*MATCH_MAXBREAKBPNUM); 
	}

	void CMainFrame::TCDebugLoadVarList()
	{
		m_wndOutput.TCDebugLoadVarList(m_TCDebugInfor->fVarInfo);
	}

	//MT 2012-9-18 断点调试的时候变量值显示
	bool CMainFrame::TCIsDebugValue(const TCHAR *name,CString &csValue)
	{
		if(m_isInBP&&m_IsDebuging)
		{
			XMLCh *pData=m_TCDebugInfor->fVarInfo;
			while((!XMLString::startsWith(pData,L"null"))&&((*pData)))
			{
				//空间名
				pData+=XMLString::stringLen(pData)+1;
				if(XMLString::equals(pData,name))
				{
					//变量名
					pData+=XMLString::stringLen(pData)+1;
					//类型
					bool isArray=false;
					bool isString=false;
					if(XMLString::equals(pData,L"数组"))
						isArray=true;
					else if(XMLString::equals(pData,L"字符型"))
						isString=true;
					pData+=XMLString::stringLen(pData)+1;

					//数据
					if(isArray)
					{
						//判读数组的头
						if(*pData==0XA5A6)
							pData+=2;
						CString csArray;
						//判读数组的尾部,遇到就结束
						while(*pData!=0XA6A5)
						{
							csArray=csArray+pData+L"\r\n";
							pData+=XMLString::stringLen(pData)+1;
						}
						csValue=csArray;
						pData+=2;
					}
					else
					{
						if(isString)
						{
							csValue.Format(L"%s = \"%s\"",name,pData);
							pData+=XMLString::stringLen(pData)+1;
						}
						else
						{
							csValue.Format(L"%s = %s",name,pData);
							pData+=XMLString::stringLen(pData)+1;
						}
					}

					return true;
				}
				else
				{
					//变量名
					pData+=XMLString::stringLen(pData)+1;
					//类型
					bool isArray=false;
					if(XMLString::equals(pData,L"数组"))
						isArray=true;
					pData+=XMLString::stringLen(pData)+1;

					//数据
					if(isArray)
					{
						//判读数组的头
						if(*pData==0XA5A6)
							pData+=2;
						CString csArray;
						//判读数组的尾部,遇到就结束
						while(*pData!=0XA6A5)
						{
							csArray=csArray+pData;
							pData+=XMLString::stringLen(pData)+1;
						}
						pData+=2;
					}
					else
					{
						pData+=XMLString::stringLen(pData)+1;

					}
				}
			}
		}
		return false;
	}

	void CMainFrame::TCDebug()
	{
		m_IsInCompile=TRUE;
		//清理项目
		TCClearProj();

		if(!TCBuildProj())
		{
			m_IsInCompile=FALSE;
			return;
		}

		//让信息输出窗口置顶
		if(CTCRunTimeManager::g_ShowRunConfim)
		{
			CTCRunConfim dlg;
			int popup = dlg.DoModal();
			if( IDCANCEL == popup )
			{
				m_IsInCompile=FALSE;
				/////销毁进度条
				TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_HIDE);
				return;
			}
		}
		AfxBeginThread(MyDebugThread,this);
	}

	BOOL EnablePrivilege(LPCTSTR pszPrivName, BOOL fEnable /*= TRUE*/) 
	{    
		BOOL fOk = FALSE;    
		// Assume function fails    
		HANDLE hToken;    
		// Try to open this process's access token    
		if (OpenProcessToken(GetCurrentProcess(),   
			TOKEN_ADJUST_PRIVILEGES, &hToken)) 
		{        
			// privilege        
			TOKEN_PRIVILEGES tp = { 1 };       

			if( LookupPrivilegeValue(NULL, pszPrivName, &tp.Privileges[0].Luid) )
			{
				tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;

				AdjustTokenPrivileges(hToken, FALSE, &tp,          
					sizeof(tp), NULL, NULL);

				fOk = (GetLastError() == ERROR_SUCCESS);  
			}
			CloseHandle(hToken); 
		} 
		return(fOk);
	}

	void CMainFrame::TCDebugRun()
	{
		EnablePrivilege(_T("SeDebugPrivilege"),true);

		STARTUPINFO si; 	 
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(STARTUPINFO); 
		PROCESS_INFORMATION pi; 
		GetStartupInfo(&si); 

		CString csHelpExe;
		csHelpExe.Format(_T("%s\\%s.exe"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		bool cpSuc=CreateProcess(csHelpExe,_T("tcdebug"),NULL,NULL,TRUE,CREATE_SUSPENDED,NULL,NULL,&si,&pi);
		if(cpSuc)
		{
			m_TCDebugProcessHandle=OpenProcess(PROCESS_ALL_ACCESS,false,pi.dwProcessId);
			ResumeThread(pi.hThread);
		}
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		m_IsDebuging=true;
		::WaitForSingleObject(m_TCDebugProcessHandle,INFINITE);

		m_TCDebugProcessHandle=NULL;
		m_IsDebuging=false;
		m_IsInCompile=false;
		m_isInBP=false;

		//这里向所有打开的CTcView的视图发送一个消息,用来删除调试时候的,当前断点的三角形图标
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
				pView->TCDeleteDebugBP();
			}
		}

		//SendEditor(SCI_MARKERDELETEALL,2,NULL);
	}

	bool CMainFrame::TCOpenExistProject()
	{
		CFileDialog filedlg(TRUE, 
			_T(".tcproj"),
			_T("未命名.tcproj"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			// 注意过滤器的写法
			_T("TC Files (*.tcproj)|*.tcproj||"),this);

		// 他将filedlg.m_ofn.lpstrTitle 指定的文本换为标题
		filedlg.m_ofn.lpstrTitle = _T("查找项目文件(.tcproj)");

		if(IDOK==filedlg.DoModal())
		{
			TCSaveAllDoc();

			if( CTCRunTimeManager::g_StartPageClose )
				((CTCApp*)AfxGetApp())->TCCloseAllView();
			else
				((CTCApp*)AfxGetApp())->TCCloseWithoutStartView();

			((CTCApp*)AfxGetApp())->m_pDocTemplateTc->CloseAllDocuments(TRUE);

			m_DlgManager.reset();

			//打开一个新项目时清空上次项目的输出信息
			TCClearOutMsg();
			//清空上个项目的引用信息
			TCClearAllContentMsg();

			CString csProjFilePath,csProjName,csProjPath,csProjEnterFileName,csProjSourcePath,csRCFile,csBuildPath,csCompilePath;
			int langType=-1;
			CString csHelp=filedlg.GetPathName();
			csProjName=filedlg.GetFileTitle();
			csProjFilePath=csHelp;

			//从配置文件上面读取的内容的最大长度是1024
			::GetPrivateProfileString(_T("ProjInfo"),_T("EnterFile"),_T(""),csProjEnterFileName.GetBuffer(MAX_PATH),MAX_PATH,csProjFilePath);   //读主取配置文件
			csProjEnterFileName.ReleaseBuffer();
			if(csProjEnterFileName.IsEmpty())
			{
				::AfxMessageBox(_T("项目配置文件出错!"));
				return false;
			}
			langType=::GetPrivateProfileInt(_T("ProjInfo"),_T("Language"),-1,csProjFilePath);

			if(!(langType==0||langType==1))
			{
				::AfxMessageBox(_T("项目配置文件出错!"));
				return false;
			}
			CString strCurFileName;
			int iLength = csHelp.GetLength();
			int iIndex = csHelp.ReverseFind(L'\\');
			csHelp.Delete(iIndex,iLength-iIndex);
			csProjPath=csHelp;
			if (!::PathFileExists(csProjPath))
			{
				::AfxMessageBox(_T("项目配置文件出错,不能定位项目目录!"));
				return false;
			}       
			csProjSourcePath.Format(_T("%s\\%s"),csProjPath,csProjName);
			if (!::PathFileExists(csProjSourcePath))
			{
				::AfxMessageBox(_T("项目配置文件出错,不能定位项目源文件目录!"));
				return false;
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

				// csRCFile.Format(_T("%s\\资源\\main.twin"),csProjSourcePath);
			}   
			

			//将资源文件路径设置回来
			csRCFile.Format(_T("%s\\资源\\main.twin"),csProjSourcePath);
			csBuildPath.Format(_T("%s\\生成"),csProjPath);
			if (!::PathFileExists(csBuildPath))
			{
				//::AfxMessageBox(_T("项目配置文件出错,不能定位生成目录!"));
				//return false;
				//创建生成文件夹
				if(0==CreateDirectory(csBuildPath,NULL))   
				{
					AfxMessageBox(_T("创建项目生成文件夹失败,请重试!")); 
					return false;
				}
			} 

			csCompilePath.Format(_T("%s\\编译"),csProjPath);
			if (!::PathFileExists(csCompilePath))
			{
				//::AfxMessageBox(_T("项目配置文件出错,不能定位生成目录!"));
				//return false;
				//创建生成文件夹
				if(0==CreateDirectory(csCompilePath,NULL))   
				{
					AfxMessageBox(_T("创建项目生成文件夹失败,请重试!")); 
					return false;
				}
			}



			CString csProjEnterFile;
			csProjEnterFile.Format(_T("%s\\%s"),csProjSourcePath,csProjEnterFileName);
			CTCRunTimeManager::setProjInfor(csProjPath,csProjName,csProjFilePath,csProjSourcePath,csProjEnterFile,csProjEnterFileName,csRCFile,csBuildPath,csCompilePath,langType);

			//this->m_TWinManager.load(CTCRunTimeManager::g_projRcFile);
			//打开项目默认文件,是否只需要这样打开就可以了 -match

			m_DlgManager.load(CTCRunTimeManager::g_projRcFile);

			this->ProjFileViewReload();

			((CTCApp*)AfxGetApp())->TCOpenTWin();

			/////////////////////////读取版本信息///////////////////
			CTCExeVersionManager::TCResetVersionInfo();
			CTCExeVersionManager::TCReadVersionInfo(csProjFilePath);
			/////////////////////////读取版本信息///////////////////

			//9-14添加添加文件相关
			((CTCApp*)AfxGetApp())->AddToRecentFileList(csProjFilePath);

			//设置属性框
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(CTCRunTimeManager::g_projEnterFileName,CTCRunTimeManager::g_projEnterFile);

			return true;
		}

		return false;
	}
	/***********************************************************************/
	/*  \函数名称：CopyDir
	/*	\函数功能：复制文件夹及其子文件夹
	/*	\函数返回：bool 如果是true，复制成功，否则失败。
	/*	\参数说明：
	@param     CString strSrcPath 源文件夹
	@param     CString strDstPath 目标文件夹
	/***********************************************************************/
	bool CopyDir(CString strSrcPath, CString strDstPath)
	{
		//如果目标文件夹不存在则创建该文件夹
		if( !PathFileExists(strDstPath) )
		{
			// 创建目标文件夹
			if( 0 == CreateDirectory(strDstPath,NULL) )   
			{
				AfxMessageBox(_T("创建项目源文件文件夹失败!")); 
				return false;
			}
		}

		CFileFind finder;
		// 打开指定的文件夹进行搜索
		BOOL bWorking = finder.FindFile(strSrcPath + "\\" + "*.*"); 
		while(bWorking)
		{
			// 从当前目录搜索文件
			bWorking = finder.FindNextFile();
			CString strFileName = finder.GetFileName();
			CString strSrc = strSrcPath + "\\" + strFileName;
			CString strDst = strDstPath + "\\" + strFileName;
			// 判断搜索到的是不是"."和".."目录
			if(!finder.IsDots())
			{
				// 判断搜索到的目录是否是文件夹
				if(finder.IsDirectory())
				{
					// 如果是文件夹的话，进行递归
					if(!CopyDir(strSrc, strDst)) 
						return false;
				}
				else
				{
					// 如果是文件，进行复制
					if(!CopyFile(strSrc, strDst, FALSE))
						return false;
				}
			}
		}       
		return true;
	}

	bool CMainFrame::TCBackUpProject()
	{
		//判断是否可以创建备份项目
		CString csBackUpProPath;
		csBackUpProPath.Format(_T("%s\\%s"),CTCRunTimeManager::g_ProjPath,_T("备份"));

		//如果项目文件夹中已经存在备份文件夹
		if( PathFileExists(csBackUpProPath) )
		{
			//则直接拷贝项目相关文件

			//①project文件
			CString bkProjFilePath;
			bkProjFilePath.Format(_T("%s\\%s.tcproj"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			::CopyFile(CTCRunTimeManager::g_ProjFilePath,bkProjFilePath,FALSE);

			//②项目文件夹及其子目录
			CString bkProFolderPath;
			bkProFolderPath.Format(_T("%s\\%s"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			CopyDir( CTCRunTimeManager::g_ProjSourcePath,bkProFolderPath );

			//③生成文件夹及其子目录,生成目录有没必要拷贝??
			/*CString bkDebugFolderPath;
			bkDebugFolderPath.Format(_T("%s\\生成"),csBackUpProPath);
			CopyDir( CTCRunTimeManager::g_ProjBuildPath,bkDebugFolderPath );*/

			return true;
		}
		//否则创建备份文件夹
		else
		{
			if( 0 == CreateDirectory(csBackUpProPath,NULL) )   
			{
				AfxMessageBox(_T("创建项目源文件文件夹失败,请重试!")); 
				return false;
			}
			//①project文件
			CString bkProjFilePath;
			bkProjFilePath.Format(_T("%s\\%s.tcproj"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			::CopyFile(CTCRunTimeManager::g_ProjFilePath,bkProjFilePath,FALSE);

			//②项目文件夹及其子目录
			CString bkProFolderPath;
			bkProFolderPath.Format(_T("%s\\%s"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			CopyDir( CTCRunTimeManager::g_ProjSourcePath,bkProFolderPath );

			//③生成文件夹及其子目录,生成目录有没必要拷贝??
			/* CString bkDebugFolderPath;
			bkDebugFolderPath.Format(_T("%s\\生成"),csBackUpProPath);
			CopyDir( CTCRunTimeManager::g_ProjBuildPath,bkDebugFolderPath );*/

			return true;
		}
		return true;
	}


	void CMainFrame::OnTcDebugpro()
	{
		// TODO: 在此添加命令处理程序代码
		if(m_isInBP)
		{        
			m_isInBP=false;
			// TODO: 在此添加命令处理程序代码
			//这里向所有打开的CTcView的视图发送一个消息,用来删除调试时候的,当前断点的三角形图标
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
					pView->TCDeleteDebugBP();
				}
			}	 
			m_TCDebugInfor->fDebugType=CMyDebugManager::MATCH_BREAKDEBUG;
			::SetEvent(m_LibEventHandle);
			//清空调试变量信息
			m_wndOutput.ClearOutputVar();
			TCSetHeadPanelText(_T("就绪"),_T(""));
		}
	}

	void CMainFrame::OnTcDebugstep()
	{
		// TODO: 在此添加命令处理程序代码
		if(m_isInBP)
		{

			// TODO: 在此添加命令处理程序代码
			//这里向所有打开的CTcView的视图发送一个消息,用来删除调试时候的,当前断点的三角形图标
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
					pView->TCDeleteDebugBP();
				}
			}	

			TCSetHeadPanelText(_T("单步运行"),_T("单步运行项目"));
			m_isInBP=false;
			m_TCDebugInfor->fDebugType=CMyDebugManager::MATCH_STEPBYSTEP;

			::SetEvent(m_LibEventHandle);
			//清空调试变量信息
			m_wndOutput.ClearOutputVar();
			TCSetHeadPanelText(_T("就绪"),_T(""));
		}
	}

	void CMainFrame::OnTcDebugstop()
	{
		// TODO: 在此添加命令处理程序代码
		if(m_IsDebuging)
		{
			TCSetHeadPanelText(_T("停止运行"),_T("停止运行项目"));
			::TerminateProcess(m_TCDebugProcessHandle,0);
			m_TCDebugProcessHandle=NULL;
			m_IsDebuging=false;
			m_IsInCompile=false;
			m_isInBP=false;
			TCSetHeadPanelText(_T("就绪"),_T("")); 
			m_wndOutput.ClearOutputVar();
		}
	}

	void CMainFrame::OnTcDebugrun()
	{
		// TODO: 在此添加命令处理程序代码
		TCSetHeadPanelText(_T("运行..."),_T("开始运行项目"));
		if(m_IsInCompile == FALSE)
			TCSaveAllDoc();
		else
		{
			::AfxMessageBox(_T("正在调试中,先等待调试结束"));
			return ;
		}
		ActiveOutPutWnd();
		m_IsInCompile=TRUE;
		AfxBeginThread(TCDebugThread,this);
		//TCDebug();
		TCSetHeadPanelText(_T("就绪"),_T(""));
	}

	void CMainFrame::TCStartPageSetting()
	{
		m_wndFileView.ShowPane(TRUE,FALSE,FALSE);
		m_wndClassView.ShowPane(TRUE,FALSE,FALSE);
		m_wndOutput.ShowPane(TRUE,FALSE,FALSE);
		m_wndProperties.ShowPane(TRUE,FALSE,FALSE);
		m_wndFunDec.ShowPane(TRUE,FALSE,FALSE);
	}

	void CMainFrame::OnViewMore()
	{
		// TODO: 在此添加命令处理程序代码

	}

	void CMainFrame::OnViewFileView()
	{
		if(m_wndFileView.IsVisible())
			m_wndFileView.ShowPane(FALSE,FALSE,FALSE);
		else
			m_wndFileView.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnUpdateViewMore(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
	}

	void CMainFrame::OnUpdateViewFileView(CCmdUI *pCmdUI)
	{
		if(m_wndFileView.IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}

	void CMainFrame::OnViewPropertiesWnd()
	{
		if(m_wndProperties.IsVisible())
			m_wndProperties.ShowPane(FALSE,FALSE,FALSE);
		else
			m_wndProperties.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnViewToolboxWnd()
	{
		if(m_wndDesignToolBox.IsVisible())
			m_wndDesignToolBox.ShowPane(FALSE,FALSE,FALSE);
		else
			m_wndDesignToolBox.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnUpdateViewPropertiesWnd(CCmdUI *pCmdUI)
	{
		if(m_wndProperties.IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}

	void CMainFrame::OnUpdateViewToolboxWnd(CCmdUI *pCmdUI)
	{
		if(m_wndDesignToolBox.IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}

	void CMainFrame::OnViewOutputWnd()
	{
		if(m_wndOutput.IsVisible())
			m_wndOutput.ShowPane(FALSE,FALSE,FALSE);
		else
			m_wndOutput.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnUpdateViewOutputWnd(CCmdUI *pCmdUI)
	{
		if(m_wndOutput.IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}

	void CMainFrame::OnViewClassView()
	{
		if(m_wndClassView.IsVisible())
			m_wndClassView.ShowPane(FALSE,FALSE,FALSE);
		else
			m_wndClassView.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnUpdateViewViewClassView(CCmdUI *pCmdUI)
	{
		if(m_wndClassView.IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}

	void CMainFrame::OnUpdateTcDebugrun(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			if(m_IsInCompile)
			{
				pCmdUI->Enable(FALSE);
			}
			else
			{
				pCmdUI->Enable();
			}
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CMainFrame::OnUpdateTcDebugstep(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(m_IsDebuging&&m_isInBP)
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CMainFrame::OnUpdateTcDebugpro(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(m_IsDebuging&&m_isInBP)
		{
			pCmdUI->Enable();

		}
		else
		{

			pCmdUI->Enable(FALSE);
		}
	}

	void CMainFrame::OnUpdateTcDebugstop(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(m_IsDebuging)
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CMainFrame::OnUpdateTcCompile(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			if(m_IsInCompile)
			{
				pCmdUI->Enable(FALSE);
			}
			else
			{
				pCmdUI->Enable();
			}
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CMainFrame::OnUpdateTcBuild(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			if(m_IsInCompile)
			{
				pCmdUI->Enable(FALSE);
			}
			else
			{
				pCmdUI->Enable();
			}
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CMainFrame::OnUpdateTcClear(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			if(m_IsInCompile)
			{
				pCmdUI->Enable(FALSE);
			}
			else
			{
				pCmdUI->Enable();
			}
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

	void CMainFrame::OnUpdateViewDecfunWnds(CCmdUI *pCmdUI)
	{
		if(m_wndFunDec.IsVisible())
			pCmdUI->SetCheck(TRUE);
		else
			pCmdUI->SetCheck(FALSE);
	}

	void CMainFrame::OnViewDecfunWnds()
	{
		if(m_wndFunDec.IsVisible())
			m_wndFunDec.ShowPane(FALSE,FALSE,FALSE);
		else
			m_wndFunDec.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnViewStatusBar()
	{
		// TODO: 在此添加命令处理程序代码
		if(m_wndStatusBar.IsVisible())
		{
			m_wndStatusBar.ShowPane(FALSE,FALSE,FALSE);
		}
		else
			m_wndStatusBar.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnUpdateViewStatusBar(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(m_wndStatusBar.IsVisible())
		{
			pCmdUI->SetCheck(TRUE);
			m_wndStatusBar.SendMessageToDescendants(WM_IDLEUPDATECMDUI,(WPARAM)FALSE, 0, FALSE, FALSE);
		}
		else
			pCmdUI->SetCheck(FALSE);
	}

	void CMainFrame::OnTcCompile()
	{
		// TODO: 在此添加命令处理程序代码
		TCSetHeadPanelText(_T("编译---"),_T("编译当前项目"));
		//这儿激活OutPut窗口
		if(m_IsInCompile == FALSE)
			TCSaveAllDoc();
		else
		{
			::AfxMessageBox(_T("正在调试中,先等待调试结束"));
			return ;
		}
		m_IsInCompile=true;	 
		ActiveOutPutWnd();
		CWinThread *helpThread=AfxBeginThread(TCCompileProjThread,this);
		//::WaitForSingleObject(helpThread->m_hThread,INFINITE);
		// m_IsInCompile=false;
		//TCCompileProj();
		TCSetHeadPanelText(_T("就绪"),_T(""));
	}

	void CMainFrame::OnTcBuild()
	{
		// TODO: 在此添加命令处理程序代码
		TCSetHeadPanelText(_T("生成"),_T("生成当前项目"));
		//这儿激活OutPut窗口
		if(m_IsInCompile == FALSE)
			TCSaveAllDoc();
		else
		{
			::AfxMessageBox(_T("正在调试中,先等待调试结束"));
			return ;
		}
		m_IsInCompile=TRUE;
		ActiveOutPutWnd();
		CWinThread *helpThread=AfxBeginThread(TCBuildProjThread,this);
		//::WaitForSingleObject(helpThread->m_hThread ,INFINITE);

		//TCBuildProj();
		TCSetHeadPanelText(_T("就绪"),_T(""));
	}

	void CMainFrame::OnTcClear()
	{
		// TODO: 在此添加命令处理程序代码
		TCSetHeadPanelText(_T("清理"),_T("清理当前项目"));
		TCClearProj();
		TCSetHeadPanelText(_T("就绪"),_T(""));
	}

	void CMainFrame::OnTcOpenbuild()
	{
		// TODO: 在此添加命令处理程序代码
		//::AfxMessageBox(_T("打开生成的EXE所在的目录"));
		TCSetHeadPanelText(_T("打开"),_T("打开生成目录"));
		ShellExecute(NULL, _T("open"), CTCRunTimeManager::g_ProjBuildPath, NULL, NULL, SW_SHOWNORMAL); 
		TCSetHeadPanelText(_T("就绪"),_T(""));
	}

	void CMainFrame::OnUpdateTcOpenbuild(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}


	TCTOOLTYPE CMainFrame::GetSelectToolType()
	{
		return m_wndDesignToolBox.GetSelectToolType();
	}

	void CMainFrame::ResetSelectToolType()
	{
		m_wndDesignToolBox.ResetSelectToolType();
	}


	//压缩的格式L"u*c:\\tc.lc*c:\\tc.exe"  文件名一样就会更新
	//解压的格式L"x*c:\\tc.lc*c:\\topath"
	bool CMainFrame::compress7z(CString compath)
	{
		bool ret=false;
		g_StdStream = &g_StdOut;

		NConsoleClose::CCtrlHandlerSetter ctrlHandlerSetter;
		int res = 0;

		UStringVector commandStrings;

		NCommandLineParser::SplitCommandLine(compath.GetBuffer(), commandStrings);

		///////////////////////////////////现在开始显示进度条/////////////////////////////////////
		//TCSetProgressPos(50);
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,50);
		//现在开始显示进度条

		//commandStrings.Delete(0);

		CArchiveCommandLineOptions options;

		CArchiveCommandLineParser parser;

		parser.Parse1(commandStrings, options);

		CStdOutStream &stdStream = options.StdOutMode ? g_StdErr : g_StdOut;
		g_StdStream = &stdStream;

		parser.Parse2(options);

		CCodecs *codecs = new CCodecs;
		CMyComPtr<
#ifdef EXTERNAL_CODECS
			ICompressCodecsInfo
#else
			IUnknown
#endif
		> compressCodecsInfo = codecs;

		HRESULT result = codecs->Load();

		bool isExtractGroupCommand = options.Command.IsFromExtractGroup();

		if (codecs->Formats.Size() == 0 &&
			(isExtractGroupCommand ||
			options.Command.CommandType == NCommandType::kList ||
			options.Command.IsFromUpdateGroup()))
			throw kNoFormats;

		///////////////////////////////////现在开始显示进度条/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,60);
		//TCSetProgressPos(60);
		//现在开始显示进度条

		CIntVector formatIndices;
		if (!codecs->FindFormatForArchiveType(options.ArcType, formatIndices))
			throw kUnsupportedArcTypeMessage;

		else if (options.Command.IsFromUpdateGroup())
		{
			CUpdateOptions &uo = options.UpdateOptions;
			if (uo.SfxMode && uo.SfxModule.IsEmpty())
				uo.SfxModule = kDefaultSfxModule;

			COpenCallbackConsole openCallback;
			openCallback.OutStream = &stdStream;

			CUpdateCallbackConsole callback;
			callback.EnablePercents = options.EnablePercents;

			callback.StdOutMode = uo.StdOutMode;
			callback.Init(&stdStream);

			CUpdateErrorInfo errorInfo;

			if (!uo.Init(codecs, formatIndices, options.ArchiveName))
				throw kUnsupportedArcTypeMessage;

			HRESULT result = UpdateArchive(codecs,
				options.WildcardCensor, uo,
				errorInfo, &openCallback, &callback);

			///////////////////////////////////现在开始显示进度条/////////////////////////////////////
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,65);
			//TCSetProgressPos(65);
			//现在开始显示进度条

			if(result==S_OK)
				ret=true;

			//收集错误信息
			//int exitCode = NExitCode::kSuccess;
			//if (callback.CantFindFiles.Size() > 0)
			// {
			// stdStream << endl;
			// stdStream << "WARNINGS for files:" << endl << endl;
			// int numErrors = callback.CantFindFiles.Size();
			// for (int i = 0; i < numErrors; i++)
			//	 {
			//	 stdStream << callback.CantFindFiles[i] << " : ";
			//	 stdStream << NError::MyFormatMessageW(callback.CantFindCodes[i]) << endl;
			//	 }
			// stdStream << "----------------" << endl;
			// stdStream << "WARNING: Cannot find " << numErrors << " file";
			// if (numErrors > 1)
			//	 stdStream << "s";
			// stdStream << endl;
			// exitCode = NExitCode::kWarning;
			// }

			//if (result != S_OK)
			// {
			// UString message;
			// if (!errorInfo.Message.IsEmpty())
			//	 {
			//	 message += errorInfo.Message;
			//	 message += L"\n";
			//	 }
			// if (!errorInfo.FileName.IsEmpty())
			//	 {
			//	 message += errorInfo.FileName;
			//	 message += L"\n";
			//	 }
			// if (!errorInfo.FileName2.IsEmpty())
			//	 {
			//	 message += errorInfo.FileName2;
			//	 message += L"\n";
			//	 }
			// if (errorInfo.SystemError != 0)
			//	 {
			//	 message += NError::MyFormatMessageW(errorInfo.SystemError);
			//	 message += L"\n";
			//	 }
			// if (!message.IsEmpty())
			//	 stdStream << L"\nError:\n" << message;
			// //throw CSystemException(result);
			// }
			//int numErrors = callback.FailedFiles.Size();
			//if (numErrors == 0)
			// {
			// if (callback.CantFindFiles.Size() == 0)
			//	 stdStream << L"\nError:\n"/*kEverythingIsOk*/ << endl;
			// }
			//else
			// {
			// stdStream << endl;
			// stdStream << "WARNINGS for files:" << endl << endl;
			// for (int i = 0; i < numErrors; i++)
			//	 {
			//	 stdStream << callback.FailedFiles[i] << " : ";
			//	 stdStream << NError::MyFormatMessageW(callback.FailedCodes[i]) << endl;
			//	 }
			// stdStream << "----------------" << endl;
			// stdStream << "WARNING: Cannot open " << numErrors << " file";
			// if (numErrors > 1)
			//	 stdStream << "s";
			// stdStream << endl;
			// exitCode = NExitCode::kWarning;
			// }
			//return exitCode;
		}

		return ret;

	}

	bool CMainFrame::TCAddEventCode(CString namesp,CString fun,TC_EVENT_TYPE type)
	{
		//CString enterFile=CTCRunTimeManager::g_projEnterFile;
		CString addFile;
		addFile.Format(_T("%s\\%s.t"),CTCRunTimeManager::g_ProjSourcePath,m_pCurrentWndID);
		if (!::PathFileExists(addFile))
		{
			::AfxMessageBox(_T("不能定位项目文件!"));
			return false;
		} 

		MyEventScanner scer;

		scer.scanDocument(addFile,namesp,fun);

		//这里是打开启动文件的视图对象
		CTCView *pEnterView=NULL;
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
				//if(CTCRunTimeManager::isSameFile(enterFile,pView->m_ScintillaWnd.GetFileName()))
				if(CTCRunTimeManager::isSameFile(addFile,pView->m_ScintillaWnd.GetFileName()))
				{
					CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
					pFWnd->MDIActivate();
					pEnterView=pView;
					//return; // 文档已经被打开
				}
			}
		}
		if(!pEnterView)
		{
			//这儿打开文档,激活怎么搞了
			CTCDoc* pDoc = NULL;
			pDoc=(CTCDoc*)pDocTmp->OpenDocumentFile(addFile,FALSE,TRUE);
			POSITION pos =pDoc->GetFirstViewPosition();
			pEnterView=(CTCView *)pDoc->GetNextView(pos);
		}
		if(scer.fFindNS)
		{
			if(scer.fFindFun)
			{    
				int index=pEnterView->TCLineIndex(scer.fFunLineNum-1);
				pEnterView->TCGotoPos(index);
			}
			else
			{
				if(scer.fFindNSEnd)
				{
					int index=pEnterView->TCLineIndex(scer.fNSEndLineNum-1);
					CString csHelp;
					//csHelp.Format(_T("\n\n功能 逻辑型 %s()\n//这里添加你要执行的代码\n\n返回 真\n功能结束\n\n"),fun);
					////////////////////////////////////////6-11号修改插入/////////////////////////////////////////////
					if( CTCRunTimeManager::g_LangType == 0)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//点击关闭_执行操作\n功能 %s()\n\n辅助.退出()\n功能结束\n\n"),fun);
							else
								csHelp.Format(_T("\n//点击关闭_执行操作\n功能 %s()\n\n控件.关闭窗口(\"%s\",0)\n功能结束\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//消息路由功能\n功能 %s(句柄,消息,w参数,l参数,时间,x坐标,y坐标)\n\n返回 假\n功能结束\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//消息过程功能\n功能 %s(消息,w参数,l参数)\n\n功能结束\n\n"),fun);
						}
						else
							csHelp.Format(_T("\n功能 %s()\n//这里添加你要执行的代码\n\n功能结束\n\n"),fun);
					}
					else if(CTCRunTimeManager::g_LangType==1)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//点击关闭_执行操作\nfunction %s()\n\nhelp.exit()\nendfunction\n\n"),fun);
							else
								csHelp.Format(_T("\n//点击关闭_执行操作\nfunction %s()\n\ncontrol.closewindow(\"%s\",0)\nendfunction\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//消息路由功能\nfunction %s(hwnd,message,wParam,lParam,time,x,y)\n\nreturn false\nendfunction\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//消息过程功能\nfunction %s(message,wParam,lParam)\n\nendfunction\n\n"),fun);
						}
						else
							csHelp.Format(_T("\nfunction %s()\n//这里添加你要执行的代码\n\nendfunction\n\n"),fun);
					}

					pEnterView->TCInserText(index,index,csHelp);
				}
				else
				{

					int index=pEnterView->TCGetLength();
					CString csHelp;
					//csHelp.Format(_T("\n\n功能 逻辑型 %s()\n//这里添加你要执行的代码\n\n返回 真\n功能结束\n\n"),fun);
					////////////////////////////////////////6-11号修改插入////////////////////////////
					if( CTCRunTimeManager::g_LangType == 0)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//点击关闭_执行操作\n功能 %s()\n\n辅助.退出()\n功能结束\n\n"),fun);
							else
								csHelp.Format(_T("\n//点击关闭_执行操作\n功能 %s()\n\n控件.关闭窗口(\"%s\",0)\n功能结束\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//消息路由功能\n功能 %s(句柄,消息,w参数,l参数,时间,x坐标,y坐标)\n\n返回 假\n功能结束\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//消息过程功能\n功能 %s(消息,w参数,l参数)\n\n功能结束\n\n"),fun);
						}
						else
							csHelp.Format(_T("\n空间 %s\n\n功能 %s()\n//这里添加你要执行的代码\n\n功能结束\n\n空间结束\n\n"),m_pCurrentWndID/*CTCRunTimeManager::g_ProjName*/,fun);
					}
					else if(CTCRunTimeManager::g_LangType==1)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//点击关闭_执行操作\nfunction %s()\n\nhelp.exit()\nendfunction\n\n"),fun);
							else
								csHelp.Format(_T("\n//点击关闭_执行操作\nfunction %s()\n\ncontrol.closewindow(\"%s\",0)\nendfunction\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//消息路由功能\nfunction %s(hwnd,message,wParam,lParam,time,x,y)\n\nreturn false\nendfunction\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//消息过程功能\nfunction %s(message,wParam,lParam)\n\nendfunction\n\n"),fun);
						}
						else
							csHelp.Format(_T("\nnamespace %s\n\nfunction %s()\n//这里添加你要执行的代码\n\nendfunction\n\nendnamespace\n\n"),m_pCurrentWndID/*CTCRunTimeManager::g_ProjName*/,fun);
					}
					pEnterView->TCInserText(index,index,csHelp);
				}
			}
		}
		else
		{
			int index=pEnterView->TCGetLength();
			CString csHelp;
			if( CTCRunTimeManager::g_LangType == 0)
			{
				if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
				{
					if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
						csHelp.Format(_T("\n//点击关闭_执行操作\n功能 %s()\n\n辅助.退出()\n功能结束\n\n"),fun);
					else
						csHelp.Format(_T("\n//点击关闭_执行操作\n功能 %s()\n\n控件.关闭窗口(\"%s\",0)\n功能结束\n\n"),fun,m_pCurrentWndID);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
				{
					csHelp.Format(_T("\n//消息路由功能\n功能 %s(句柄,消息,w参数,l参数,时间,x坐标,y坐标)\n//这里添加你要执行的代码\n\n返回 假\n功能结束\n\n"),fun);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
				{
					csHelp.Format(_T("\n//消息过程功能\n功能 %s(消息,w参数,l参数)\n\n功能结束\n\n"),fun);
				}
				else
					csHelp.Format(_T("\n空间 %s\n\n功能 %s()\n//这里添加你要执行的代码\n\n功能结束\n\n空间结束\n\n"),CTCRunTimeManager::g_ProjName,fun);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
				{
					if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
						csHelp.Format(_T("\n//点击关闭_执行操作\nfunction %s()\n\nhelp.exit()\nendfunction\n\n"),fun);
					else
						csHelp.Format(_T("\n//点击关闭_执行操作\nfunction %s()\n\ncontrol.closewindow(\"%s\",0)\nendfunction\n\n"),fun,m_pCurrentWndID);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
				{
					csHelp.Format(_T("\n//消息路由功能\nfunction %s(hwnd,message,wParam,lParam,time,x,y)\n\nreturn false\nendfunction\n\n"),fun);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
				{
					csHelp.Format(_T("\n//消息过程功能\nfunction %s(message,wParam,lParam)\n\nendfunction\n\n"),fun);
				}
				else
					csHelp.Format(_T("\nnamespace %s\n\nfunction %s()\n//这里添加你要执行的代码\n\nendfunction\n\nendnamespace\n\n"),CTCRunTimeManager::g_ProjName,fun);
			}

			pEnterView->TCInserText(index,index,csHelp);
		}
		(pEnterView->GetDocument())->DoFileSave();

		pEnterView->TCActiveEditView();
		//pEnterView->TCSetFocus();
		//pEnterView->GetParent()->SetFocus();
	}


	//添加事件代码
	bool CMainFrame::TCAddEventCode(CString csEvent,TC_EVENT_TYPE type)
	{
		int i=csEvent.Find(_T('.'));
		CString ns=csEvent.Left(i);
		CString fun=csEvent.Right(csEvent.GetLength()-i-1);
		return TCAddEventCode(ns,fun,type);
	}

	//在当前的源代码t文件页面插入代码
	bool CMainFrame::TCInsertCode(CString csCode,int docPos)
	{
		// Get the active MDI child window.
		CMDIFrameWnd *pFrame =  
			(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

		// Get the active MDI child window.
		CMDIChildWnd *pChild =  
			(CMDIChildWnd *) pFrame->GetActiveFrame();

		// or CMDIChildWnd *pChild = pFrame->MDIGetActive();

		// Get the active view attached to the active MDI child
		// window.
		CView *pView = (CView *) pChild->GetActiveView();
		if(pView->IsKindOf(RUNTIME_CLASS(CTCView)))
		{
			CTCView * pTView=(CTCView *)pView;
			pTView->TCInserText(docPos,docPos,csCode);
			return TRUE;
		}
		return FALSE;
	}

	//向主项目文件添加头文件
	bool CMainFrame::TCInsertCodeToMainFile(CString csCode,int docPos)
	{
		//这里是打开启动文件的视图对象
		CTCView *pEnterView=NULL;
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
				//if(CTCRunTimeManager::isSameFile(enterFile,pView->m_ScintillaWnd.GetFileName()))
				if(CTCRunTimeManager::isSameFile(CTCRunTimeManager::g_projEnterFile,pView->m_ScintillaWnd.GetFileName()))
				{
					//CMDIChildWnd *pFWnd = static_cast<CMDIChildWnd*>(pView->GetParent());
					//pFWnd->MDIActivate();
					pView->TCInserText(docPos,docPos,csCode);
					pEnterView=pView;
					return true; // 文档已经被打开
				}
			}
		}
		//如果当前文档没有打开，则将头文件写入启动文件
		if(!pEnterView)
		{
			TCHAR *m_pBuffer = NULL;
			HANDLE hReadFile = CreateFileW(CTCRunTimeManager::g_projEnterFile,      // name of the readfile
				GENERIC_READ,           // open for writing
				FILE_SHARE_READ,        // do not share
				NULL,                   // default security
				OPEN_EXISTING,          // overwrite existing
				FILE_ATTRIBUTE_NORMAL,  // normal file
				NULL);                  // no attr. template

			if (hReadFile == INVALID_HANDLE_VALUE) 
			{ 
				CString csError;
				csError.Format(_T("加载T文件失败: %s"),CTCRunTimeManager::g_projEnterFile);
				AfxMessageBox(csError); 
				return false;
			}

			DWORD dwFileSize = GetFileSize(hReadFile,  NULL);
			if(dwFileSize)
			{
				//读出配置文件内容
				DWORD dwReadSize = dwFileSize/sizeof(TCHAR)+1;
				m_pBuffer=new TCHAR[dwReadSize];
				//m_length=dwReadSize;
				::memset(m_pBuffer,NULL,(dwReadSize)*sizeof(TCHAR));
				DWORD dword;
				ReadFile(hReadFile,m_pBuffer,dwFileSize,&dword,NULL);
				m_pBuffer++;//吃unicode头
				dwReadSize--;
				CloseHandle(hReadFile);
			}
			else
			{
				CString csError;
				csError.Format(_T("加载T文件失败,空的T文件: %s"),CTCRunTimeManager::g_projEnterFile);
				AfxMessageBox(csError); 
				return false;
			}

			//写入启动文件
			byte bytes[2];
			bytes[0] = 0xFF;
			bytes[1] = 0xFE;
			wchar_t csHeadCode[MAX_PATH]={0};
			::wsprintfW(csHeadCode,csCode);
			//int nLength=::wcslen(csHeadCode);

			int bRet;
			HANDLE handle=0;
			DWORD dwWritenSize = 0;
			handle=CreateFile(CTCRunTimeManager::g_projEnterFile,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
			::WriteFile(handle,bytes,2,&dwWritenSize,0);
			::WriteFile(handle,csHeadCode,::wcslen(csHeadCode)*sizeof(TCHAR),&dwWritenSize,0);
			bRet = ::WriteFile(handle,m_pBuffer,::wcslen(m_pBuffer)*sizeof(TCHAR),&dwWritenSize,0);
			CloseHandle(handle);

			return true;
		}
		return false;
	}

	void CMainFrame::ActiveDecWnd()
	{
		m_wndFunDec.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::ActivePerprotyWnd()
	{
		m_wndProperties.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::ActiveOutPutWnd()
	{
		//如果停靠了则显示延迟效果
		if( m_wndOutput.IsDocked())
			m_wndOutput.ShowPane(TRUE,TRUE,TRUE);
		else
			m_wndOutput.ShowPane(TRUE,FALSE,TRUE);
		//激活Tab页中的信息输出窗口
		m_wndOutput.TCSetOutputMsgActive();
	}

	void CMainFrame::OnUpdateTcOpendesignwin(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		if(CTCRunTimeManager::g_IsLoadProj)
		{
			pCmdUI->Enable();
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}


	void CMainFrame::OnTcOpendesignwin()
	{
		// TODO: 在此添加命令处理程序代码
		((CTCApp*)AfxGetApp())->TCOpenTWin();
	}

	void CMainFrame::OnButtonOpenstartview()
	{
		// TODO: 在此添加命令处理程序代码
		((CTCApp*)AfxGetApp())->TCOpenStartView();
	}


	void CMainFrame::OnTcAbzhuazhua()
	{
		// TODO: 在此添加命令处理程序代码
		CString abTool;
		abTool.Format(_T("%s\\AB抓抓.exe"),CTCRunTimeManager::g_runtimePath);
		ShellExecute(
			this->m_hWnd, 
			_T("open"), 
			abTool, 
			TEXT(""),
			NULL,
			SW_SHOW);
	}


	void CMainFrame::OnDestroy()
	{
		CMDIFrameWndEx::OnDestroy();

		// TODO: 在此处添加消息处理程序代码
		UnmapViewOfFile(m_TCDebugInfor);
		CloseHandle(m_hFileMap);
		if(m_IsInCompile||m_IsDebuging)
		{
			if(m_TCDebugProcessHandle)
				::TerminateProcess(m_TCDebugProcessHandle,0);
			m_TCDebugProcessHandle=NULL;
			m_IsInCompile=false;
			m_IsDebuging=false;
			m_isInBP=false;
		}
		//关闭平台功能
		XMLPlatformUtils::Terminate();
	}


	void CMainFrame::OnTcComlook()
	{
		// TODO: 在此添加命令处理程序代码
		CString abTool;
		abTool.Format(_T("%s\\TC插件辅助工具.exe"),CTCRunTimeManager::g_runtimePath);
		ShellExecute(
			this->m_hWnd, 
			_T("open"), 
			abTool, 
			TEXT(""),
			NULL,
			SW_SHOW);
	}


	BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
	{
		// TODO: 在此添加专用代码和/或调用基类
		m_TipExDebug.RelayEvent(pMsg);
		 //m_TipEx.RelayEvent(pMsg);
		return CMDIFrameWndEx::PreTranslateMessage(pMsg);
	}


	//给出鼠标提示
	void CMainFrame::GiveToolTip(CWnd *pWnd, LPCTSTR szTooltipText, HICON hIcon)
	{
		//设置鼠标提示
		m_TipEx.AddTool(pWnd, szTooltipText, NULL);
		//m_Tip.Activate(TRUE);
	}
	void CMainFrame::RemoveToolTip(CWnd *pWnd)
	{
		//移除鼠标提示
		m_TipEx.RemoveTool(pWnd);
	}

	void CMainFrame::OnOpenTCHelp()
	{
		HANDLE Hhelp=FindWindow(_T("HH Parent"),_T("TC帮助文档 v3.026 内测版"));
		if(Hhelp)
		{
			return;
		}
		CString abTool;
		abTool.Format(_T("%s\\TC.chm"),CTCRunTimeManager::g_runtimePath);
		ShellExecute(
			this->m_hWnd, 
			_T("open"), 
			abTool, 
			TEXT(""),
			NULL,
			SW_SHOW);
	}

	void CMainFrame::TCSendOutWndMsg(WPARAM wParam,LPARAM lParam)
	{
		SendMessage(WM_MYTCOUTMSG_MESSAGE,wParam,lParam);
	}

	LRESULT CMainFrame::OnTCOutMsg(WPARAM wParam, LPARAM lParam)
	{
		//控件类型
		int type = (int)wParam;
		//控件附加信息
		int info = (int)lParam;
		switch(type)
		{
			//case TCOUTPUTCTRL::TCOUTPUT_OUTWND:
			//	if(0 == info)
			//		ActiveOutPutWnd();
			//	else if(1 == info)
			//		m_wndOutput.ShowPane(true,false,true);
			//	break;
		case TCOUTPUTCTRL::TCOUTPUT_PROGRESS:
			if(0 == info)
				ShowProgress(false);
			else if(-1 == info)
				ShowProgress(true);
			else
				TCSetProgressPos(info);
			break;
		default:
			break;
		}
		return 0;
	}

	/////////这里是一个自定义的测试消息,用来测试调试代码的时候异常返回信息的/////////////////////
	LRESULT CMainFrame::OnTCMyDebugInforMsg(WPARAM w, LPARAM l)
	{

		::MessageBox(0,L"test",L"tc",0);
		return 0;
	}

	LRESULT CMainFrame::OnTCMyCloseStartViewMsg(WPARAM w, LPARAM l)
	{
		BSTR projPath = (BSTR)l;
		CString projPathStr(projPath);
		SysFreeString(projPath);
		((CTCApp*)AfxGetApp())->OpenRecentProject(projPathStr);
		return LRESULT(TRUE);
	}
	/////////这里是一个自定义的测试消息//////////////////////////////////////////////////////////

	void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		pCmdUI->Enable(!m_IsInCompile);
	}


	void CMainFrame::OnUpdateFileOpen(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		pCmdUI->Enable(!m_IsInCompile);
	}




	void CMainFrame::OnUpdateFileMruFile(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		pCmdUI->Enable(!m_IsInCompile);
	}


	void CMainFrame::OnUpdateFileClose(CCmdUI *pCmdUI)
	{
		// TODO: 在此添加命令更新用户界面处理程序代码
		pCmdUI->Enable(!m_IsInCompile);
	}


	void CMainFrame::OnButtonTcsetting()
	{
		// TODO: 在此添加命令处理程序代码
		//m_wndSettingDia.ShowWindow(SW_SHOW);
		//m_wndSettingDia.CenterWindow();
		CTCSettingDia dlg;
		dlg.DoModal();

	}

void CMainFrame::OnTcOpenvipview()
{
	// TODO: 在此添加命令处理程序代码
	((CTCApp*)AfxGetApp())->TCOpenVipView();
}


void CMainFrame::OnUpdate()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("检测到最新版本"));
}
