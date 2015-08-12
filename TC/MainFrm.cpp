// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//`
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����
// MainFrm.cpp : CMainFrame ���ʵ��
//
#include "stdafx.h"
#include "TC.h"

#include "MainFrm.h"

///ѹ���õ���ͷ�ļ�////////////
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
///ѹ���õ���ͷ�ļ�////////////
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

//////////////////////////////////////////ѹ���õ��������ռ�///////////////////////////////////////
using namespace NWindows;
using namespace NFile;
using namespace NCommandLineParser;

//////////////////////////////////////////ѹ���õ�ȫ�ֱ���////////////////////////////////////////
BOOL first;
HINSTANCE g_hInstance = 0;
CStdOutStream *g_StdStream=0;

#define NT_CHECK_FAIL_ACTION (*g_StdStream) << "Unsupported Windows version"; return NExitCode::kFatalError;

static const char *kNoFormats = "7-Zip cannot find the code that works with archives.";
static const char *kUnsupportedArcTypeMessage = "Unsupported archive type";
static const wchar_t *kDefaultSfxModule = L"7zCon.sfx";
////////////////////////////////////////ѹ���õ�ȫ�ֱ���////////////////////////////////////////

//�ж��ǲ���һ���ļ�
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
		//��Դ�ļ�����ֻ���ļ�,û���ļ���
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

/////////////////////�����õ��߳�/////////////////////////////
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

//TC��������Ϣ�߳�
UINT TCAlertInfoThread(LPVOID pParam)
{	
	//�������¼���߳�
	CMainFrame *pMain = (CMainFrame*)pParam;
	//ͳ�ƹ���
	{
		{
			HINTERNET internetopen = InternetOpen(L"TC",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
			if (internetopen==NULL)
			{
				//::MessageBox(pMain->m_hWnd,L"���ڲ�汾��Ҫ�����ſ���ʹ��",L"TC",0);
				//::exit(0);
				return 0;
			}
			CString csHelp;
			csHelp.Format(_T("http://www.tyuyan.com/tongjiversion.aspx?version=%s"),CTCRunTimeManager::g_TCVersion);
			HINTERNET internetopenurl = InternetOpenUrl(internetopen,csHelp,NULL,0,INTERNET_FLAG_RELOAD,0);
			if (internetopenurl==NULL)
			{    
				InternetCloseHandle(internetopen);
				//::MessageBox(pMain->m_hWnd,L"���ڲ�汾��Ҫ�����ſ���ʹ��",L"TC",0);
				//::exit(0);
				return 0;
			}
			InternetCloseHandle(internetopenurl);
			InternetCloseHandle(internetopen);
		}
	}
	//����������ʾ��֤
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
			::wcscpy(outData.newsTitle,L"TC������ʾ");
			::wcscpy(outData.newsBuff,L"TC�������°汾,����������°汾!");
			USES_CONVERSION;
			::wcscpy(outData.urlBuff,A2W(retSite));
			pMain->SendMessage(WM_MYDESKTOPALERT_MESSAGE,(WPARAM)&outData,0);
			goto news;
			/*CTCRunTimeManager::setMainSettingIni(L"setting",L"ver",CTCRunTimeManager::g_TCVersion);
			HANDLE Hhelp=FindWindow(_T("updata"),_T("TC����"));
			if(Hhelp)
			{
				goto news;
			}
			int msgRet=::MessageBox(pMain->m_hWnd,_T("����TC���°汾,�Ƿ����ϸ���"),_T("TC������ʾ"),MB_OKCANCEL);
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
		//��ȡ���ؼ�¼�����ű��
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

			BOOL queryed = HttpQueryInfo(internetopenurl,HTTP_QUERY_CONTENT_LENGTH,size,&numlen,&dwIndex); //��ȡ��ҳ��С

			if(queryed == FALSE || wcslen(size) == 0)
			{
				InternetCloseHandle(internetopenurl);
				InternetCloseHandle(internetopen);
				continue;
			}
			int length=_wtoi(size);//��ȡ����
			if(length==0)     //����Ϊ0 ����Ĭ�ϳ���
			{
				length=1024;
			}
			wchar_t buffer[1024]={0}; //��̬������С
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
			//�Ե�Unicodeͷ
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
				if(line!=_T("TC����"))
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
			if(temp!=_T("TC���Ž���"))
				continue;
		}
		//��������ڱ��ؼ�¼�����ű��,�򵯳���ʾ����
		pMain->SendMessage(WM_MYDESKTOPALERT_MESSAGE,(WPARAM)&outData,0);
		Sleep(2*60*1000);
	}
	return 0;
}

//ִ���߳�,����һ���ص��ַ�,Ϊ�������治����,���߳�ִ��
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

//��ֹ������ڿ������߳�
UINT TCDebugThread(LPVOID pParam)
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	mainF->TCDebug();
	return 0;
}

//����
UINT TCBuildProjThread(LPVOID pParam)
{
	CMainFrame *mainF=(CMainFrame *)pParam;
	mainF->TCBuildProj();
	mainF->m_IsInCompile=false;
	mainF->m_isVipBuild=false;
	mainF->DrawMenuBar();
	return 0;
}

//����
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

//�����������Ӳ��ҵ������������߳�
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
		// ȫ�ְ�������
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
		ON_MESSAGE(WM_MYTCDEBUG_MESSAGE, OnTCMyDebugPTMsg) //һ���Զ������Ϣ
		ON_MESSAGE(WM_MYTCOUTMSG_MESSAGE,OnTCOutMsg) //һ���Զ������Ϣ
		ON_MESSAGE(WM_MYCLOSESTARTVIEW_MESSAGE,OnTCMyCloseStartViewMsg) //һ���Զ������Ϣ
		ON_MESSAGE(WM_MYDESKTOPALERT_MESSAGE,OnTCDeskTopAlertWnd) //һ���Զ������Ϣ



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

	// CMainFrame ����/����
	CMainFrame::CMainFrame()
	{
		// TODO: �ڴ���ӳ�Ա��ʼ������
		theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
		//theApp.m_nAppLook=ID_VIEW_APPLOOK_WINDOWS_7;

		m_IsInCompile=false;
		m_IsDebuging=false;
		/*  m_TCBPList.removeAllNode();*/
		////�����Ǵ��������õ��ں˶���͹ܵ�ͨѶ����//
		m_LibEventHandle=NULL;  //֪ͨ���������ں˶���
		m_WinEventHandle=NULL;   //֪ͨ������ں˶���
		////�����Ǵ��������õ��ں˶���͹ܵ�ͨѶ����//

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
		// ���ڳ־�ֵ�����Ӿ�����������ʽ
		OnApplicationLook(theApp.m_nAppLook);

		CMDITabInfo mdiTabParams;
		mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_SCROLLED; // ����������ʽ...
		mdiTabParams.m_bActiveTabCloseButton = TRUE;          // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
		mdiTabParams.m_bTabIcons = TRUE;                       // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
		mdiTabParams.m_bAutoColor = FALSE;                     // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
		mdiTabParams.m_bDocumentMenu = TRUE;                   // ��ѡ�������ұ�Ե�����ĵ��˵�
		mdiTabParams.m_bTabCustomTooltips=TRUE;
		mdiTabParams.m_nTabBorderSize=2;

		EnableMDITabbedGroups(TRUE, mdiTabParams);

		/*	GetMDITabs().SetActiveTabColor(0xffff);
		GetMDITabs().SetActiveTabTextColor(0xff);*/

		m_wndRibbonBar.Create(this);
		m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

		//if (!m_wndStatusBar.Create(this))
		//{
		//    TRACE0("δ�ܴ���״̬��\n");
		//    return -1;      // δ�ܴ���
		//}

		//CString strTitlePane1;
		//CString strTitlePane2;
		//bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
		//ASSERT(bNameValid);
		//bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
		//ASSERT(bNameValid);
		//m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
		//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

		//���� Visual Studio 2005 ��ʽͣ������oh��Ϊ
		CDockingManager::SetDockingMode(DT_SMART);
		//���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
		EnableAutoHidePanes(CBRS_ALIGN_ANY);

		// ����������:
		//if (!CreateCaptionBar())
		//{
		//    TRACE0("δ�ܴ���������\n");
		//    return -1;      // δ�ܴ���
		//}

		// ���ز˵���ͼ��(�����κα�׼��������):
		CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

		// ����ͣ������
		if (!CreateDockingWindows())
		{
			TRACE0("δ�ܴ���ͣ������\n");
			return -1;
		}

		// ����״̬��:
		if (!CreateStatusBar())
		{
			TRACE0("δ�ܴ���״̬��\n");
			return -1;      // δ�ܴ���״̬��
		}

		m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
		m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndFileView);
		CDockablePane* pTabbedBar = NULL;
		m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);

		m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
		//���Դ���
		m_wndDesignToolBox.EnableDocking(CBRS_ALIGN_ANY);
		//���Դ���
		DockPane(&m_wndProperties);

		//���Դ���
		CDockablePane* pTabbedBarRight = NULL;
		m_wndDesignToolBox.AttachToTabWnd(&m_wndProperties, DM_SHOW, FALSE, &pTabbedBarRight);
		//���Դ���


		m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndOutput);

		//������ͣ��,��������ͣ����
		m_wndFunDec.EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndFunDec);

		CDockablePane* pTabbedBarBottom = NULL;
		m_wndFunDec.AttachToTabWnd(&m_wndOutput, DM_SHOW, FALSE, &pTabbedBarBottom);
		//������ͣ��,��������ͣ����

		//m_wndFunDec.ena

		// ������ǿ�Ĵ��ڹ���Ի���
		EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

		// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
		// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
		ModifyStyle(0, FWS_PREFIXTITLE);
		/////////////////////////�����ǳ�ʼ�����Ե��ں˶���////////////////////////
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
			// ӳ�������ڴ棬ȡ�ù����ڴ���׵�ַ
			m_TCDebugInfor = (CMyDebugInfor  *)MapViewOfFile(m_hFileMap,FILE_MAP_ALL_ACCESS,0,0,0);
		}
		else
			::MessageBox(0,L"����ӳ���ļ�ʧ��",L"TC",0);
		/////////////////////////�����ǳ�ʼ�����Ե��ں˶���////////////////////////
		////��ʼ��������ƵĹ������
		//m_TWinManager.reset();
		////��ʼ��������ƵĹ������
		//����̻߳�һֱ����,��Ҫ�������ȴ����Թ��̵�
		AfxBeginThread(MyDebugInforThread,this);
		m_TipEx.Create(this);
		m_TipExDebug.Create(this);
		CString csTitile;
		csTitile.Format(_T("TC%s"),CTCRunTimeManager::g_TCVersion);
		this->SetWindowText(csTitile);
		//m_wndSettingDia.Create(IDD_TCSETTINGDIA,this);
		CSplashWnd::HideSplashScreen();

		//���浯���Ի�����Ϣ��������

		AfxBeginThread(TCAlertInfoThread,this);
		//���浯���Ի�����Ϣ��������

		//�����߳�
		//AfxBeginThread(UpdataCheckThread,this);

		return 0;
	}

	BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CMDIFrameWndEx::PreCreateWindow(cs) )
			return FALSE;
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ
		cs.style &=~ (LONG) FWS_ADDTOTITLE;
		return TRUE;
	}

	BOOL CMainFrame::CreateDockingWindows()
	{
		BOOL bNameValid;

		// ��������ͼ
		CString strClassView;
		bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
		ASSERT(bNameValid);
		if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ���������ͼ������\n");
			return FALSE; // δ�ܴ���
		}

		// �����ļ���ͼ
		CString strFileView;
		bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
		ASSERT(bNameValid);
		if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ������ļ���ͼ������\n");
			return FALSE; // δ�ܴ���
		}

		// �����������
		CString strOutputWnd;
		bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
		ASSERT(bNameValid);
		if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ����������\n");
			return FALSE; // δ�ܴ���
		}

		//����������������
		CString strFunDecWnd;
		bNameValid = strFunDecWnd.LoadString(IDS_FUNDEC_WND);
		ASSERT(bNameValid);
		////////////////////////////////////////�����������ڵĴ���//////////////////////////////////
		if (!m_wndFunDec.Create(strFunDecWnd,this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_DECFUNWNDS, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ�������������������\n");
			return FALSE; // δ�ܴ���
		}
		////////////////////////////////////////�����������ڵĴ���////////////////////////////////////

		// �������Դ���
		CString strPropertiesWnd;
		bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
		ASSERT(bNameValid);
		if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ��������ԡ�����\n");
			return FALSE; // δ�ܴ���
		}

		CString strToolBoxWnd;
		bNameValid = strToolBoxWnd.LoadString(IDS_TOOLBOX_WND);
		ASSERT(bNameValid);
		////////////////////////////////////////������Ĵ���//////////////////////////////////
		if (!m_wndDesignToolBox.Create(strToolBoxWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_TOOLBOXWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ�����������������\n");
			return FALSE; // δ�ܴ���
		}
		////////////////////////////////////////������Ĵ���////////////////////////////////////

		//����ǲ���/�滻���ڵĴ���
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
			TRACE0("δ�ܴ���״̬��\n");
			return -1;      // δ�ܴ���
		}

		//m_wndStatusBar.SetInformation(_T("����"));
		//m_wndStatusBar.ShowWindow( SW_SHOWNORMAL );
		CString strTitlePane1;
		CString strTitlePane2;
		bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
		ASSERT(bNameValid);
		bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
		ASSERT(bNameValid);

		m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, _T(""),TRUE), strTitlePane1);
		//��ӷָ���
		//m_wndStatusBar.AddSeparator();
		//����ұߵ�״̬��ʾ
		m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2,_T(""),FALSE), strTitlePane2);
		//��ӷָ���
		//m_wndStatusBar.AddSeparator();
		m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_FILESIZE, _T(""),TRUE), _T("�ļ���С:"));
		//��ӷָ���
		//m_wndStatusBar.AddSeparator();
		//TC��������
		m_wndStatusBar.AddExtendedElement(new CMFCRibbonLinkCtrl(ID_STATUSBAR_LINK, _T("www.tyuyan.com"), _T("http://www.tyuyan.com")), _T("TC����"));
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
			pProgressBar->SetText(_T("����: "));
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
		//strSize.Format(_T("�ļ���С: %d"),size);
		wchar_t strSize[32]={0};
		::wsprintf(strSize,_T("�ļ���С: %d"),size);
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
			::wsprintf(str,_T("��: %d   ��: %d"),row,col);
		//CString str;
		//str.Format(_T("��: %d   ��: %d"),row,col);
		pPane->SetVisible(TRUE);
		pPane->SetText(str);
		pPane->SetToolTipText(_T("�������λ��"));
		pPane->Redraw();
		m_wndStatusBar.RecalcLayout();
		m_wndStatusBar.RedrawWindow();
		}
	}

	//BOOL CMainFrame::CreateCaptionBar()
	//{
	//    if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	//     {
	//         TRACE0("δ�ܴ���������\n");
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

	// CMainFrame ���

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


	// CMainFrame ��Ϣ�������

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
		//�õ��������
		int line=(int)wParam;
		//�õ���ǰ�ļ�
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
					outMsg.Format(_T("������ͣ�� %d �� \t�ļ�:%s"),line,curfilename);
					this->TCOutMsgAddString(outMsg);
					return TRUE; // �ĵ��Ѿ�����
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
		outMsg.Format(_T("������ͣ�� %d �� \t�ļ�:%s"),line,curfilename);
		this->TCOutMsgAddString(outMsg);
		return 0;
	}

	void CMainFrame::ResetPropertyCombox()
	{
		m_wndProperties.ResetPropertiesCombox();
	}

	//���¼�����Ŀ�ļ�
	void CMainFrame::ProjFileViewReload()
	{
		m_wndFileView.ReloadProj();
	}

	//������Ŀ�ļ�������
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

	//��ʾstatic�ؼ�������
	void CMainFrame::SetTWinStaticProperty(CString id, CTCStatic *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinStaticProperty(id,ctl,upDate);
	}
	//��ʾgroup�ؼ�������
	void CMainFrame::SetTWinGroupProperty(CString id, CTCGroupBox *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinGroupProperty(id,ctl,upDate);
	}
	//��ʾ��ť�ؼ�������
	void CMainFrame::SetTWinButtonProperty(CString id, CTCButton *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinButtonProperty(id,ctl,upDate);
	}
	//��ʾedit�ؼ�������
	void CMainFrame::SetTWinEditProperty(CString id, CTCEdit *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinEditProperty(id,ctl,upDate);
	}
	//��ʾCheck�ؼ�������   
	void CMainFrame::SetTWinCheckProperty(CString id, CTCCheck *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinCheckProperty(id,ctl,upDate);
	}
	//��ʾRadio�ؼ�������
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
	//��ʾprogress�ؼ�������
	void CMainFrame::SetTWinProgressProperty(CString id, CTCProgress *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinProgressProperty(id,ctl,upDate);
	}

	void CMainFrame::SetTWinPathProperty(CString id, CTCBrowse *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinPathProperty(id,ctl,upDate);
	}
	//��ʾSlider������
	void CMainFrame::SetTWinSliderProperty(CString id, CTCSlider *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinSliderProperty(id,ctl,upDate);
	}
	//��ʾSlider������
	void CMainFrame::SetTWinDateProperty(CString id, CTCDate *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinDateProperty(id,ctl,upDate);
	}
	//��ʾList������
	void CMainFrame::SetTWinListProperty(CString id, CTCListBox *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinListProperty(id,ctl,upDate);
	}
	//��ʾTab������
	void CMainFrame::SetTWinTableProperty(CString id, CTCTab*  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinTableProperty(id,ctl,upDate);
	}
	//��ʾTimer������
	void CMainFrame::SetTWinTimerProperty(CString id, CTCTimer*  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinTimerProperty(id,ctl,upDate);
	}

	//��ʾHotKey������
	void CMainFrame::SetTWinHotKeyProperty(CString id, CTCHotKeyCtrl *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinHotKeyProperty(id,ctl,upDate);
	}

	//��ʾExplorer������
	void CMainFrame::SetTWinExplorerProperty(CString id, CTCExplorer *  ctl,BOOL upDate)
	{
		m_wndProperties.SetTWinExplorerProperty(id,ctl,upDate);
	}

	//��ʾG�յ�Ctrl������
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

	//���޸����Կ�ʱ,�޸���Ŀ�����
	bool CMainFrame::FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd)
	{
		return m_wndFileView.FileReNameByPropWnd(oldPatn,newName,FileOrWnd);
	}

	//�޸�ָ���������ֵ�������ػ�
	void CMainFrame::SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue)
	{
		m_wndProperties.SetTheProperty(protype,name,title,newValue);
	}

	///////////////////////////��ӿؼ����Ƶ����Դ���������///////////////
	void CMainFrame::InsertCtrlToProCom(CString ctrlName)
	{
		m_wndProperties.InsertCtrlToProCom(ctrlName);
	}

	void CMainFrame::TCActiveFindWnd()
	{
		m_wndFind.ShowWindow(TRUE);
		m_wndFind.CenterWindow();
	}

	//��������
	void CMainFrame::SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample)
	{
		m_wndFunDec.SetDecText(csNS,csFunName,csFunProto,csFunDes,csExample);
	}

	//������Ŀ
	bool CMainFrame::TCCompileProj()
	{
		TCClearOutMsg();
		TCClearAllContentMsg();
		TCClearProj();
		CString csComplie;
		csComplie.Format(_T("========== ���뿪ʼ: ��Ŀ����: %s =========="),CTCRunTimeManager::g_ProjName);
		m_wndOutput.TCOutMsgAddString(csComplie);

		//�Ƿ��������Ŀ
		if(!CTCRunTimeManager::g_IsLoadProj)
			return false;

		::SetCurrentDirectory(CTCRunTimeManager::g_ProjSourcePath);

		//��δ����Ǳ������е��ĵ��ļ��������,��Ҫ��������ܱ���Ի����,���浱ǰ�ĵ�
		m_wndOutput.TCOutMsgAddString(_T(">���������ĵ�"));

		// TCSaveAllDoc();

		CString csEnterFile=CTCRunTimeManager::g_projEnterFile;

		if (!::PathFileExists(csEnterFile))
		{
			m_wndOutput.TCOutMsgAddString(_T(">û�л����Ŀ�����ļ�"),true);
			return FALSE;
		}
		//������
		MyParser parser;   
		//m_wndOutput.TCOutMsgAddString(_T("���ɽ�����"));

		//���󱨸����
		DomTreeErrorReporter errReporter; 
		//Set Error ob
		parser.setErrorHandler(&errReporter);


		bool errorsOccured = false;
		try
		{
			m_wndOutput.TCOutMsgAddString(_T(">��ʼ����..."));
			m_wndOutput.TCOutMsgAddString(_T(">������Դ�ļ�..."));
			if(CopyFolderAllFiles(CTCRunTimeManager::g_ProjSourcePath+L"\\��Դ",CTCRunTimeManager::g_ProjCompilePath))
			{
				//���߽�����,��ص�·��
				MySerializeBlockList::setCompilePath(CTCRunTimeManager::g_ProjCompilePath,CTCRunTimeManager::g_ProjSourcePath);
				parser.parse(csEnterFile);
			}
			else
			{
				errorsOccured=true;
				m_wndOutput.TCOutMsgAddString(_T(">������Դ�ļ�����,�����Ƿ����ļ�����ʹ��"),true);
			}
		}
		catch (const MyXMLExpError& e)  // Add Back 
		{
			CString csHelp;
			csHelp.Format(_T(">����:%s �к�:%d ������Ϣ:%s"),e.fSrcFile,e.fSrcLine,e.fMsg);
			m_wndOutput.TCOutMsgAddString(csHelp,true,e.fSrcFile,e.fSrcLine);
			errorsOccured = true;
		}
		catch (const OutOfMemoryException& )
		{
			//����
			////////////////////////////////////////////////////////////
			m_wndOutput.TCOutMsgAddString(_T(">����:OutOfMemoryException"),true);
			errorsOccured = true;
		}
		catch (const XMLException& e)
		{
			//����
			//////////////////////////////////////////////////////////////////////////////////
			CString csHelp;
			csHelp.Format(_T(">An error occurred during parsing   Message: %s"),XMLString::transcode(e.getMessage()));
			m_wndOutput.TCOutMsgAddString(csHelp,true);
			errorsOccured = true;
		}
		catch (const DOMException& e)
		{
			//����
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
			csHelp.Format(_T(">���������з�����δ֪�Ĵ���!"));
			m_wndOutput.TCOutMsgAddString(csHelp,true);
			errorsOccured = true;
		}	
		if(errorsOccured)
		{
			m_wndOutput.TCOutMsgAddString(_T(">���﷨����,���֤������!"));
		}
		else
		{
			m_wndOutput.TCOutMsgAddString(_T(">����༭�ɹ�"));
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

		//���û���
		::SetCurrentDirectory(CTCRunTimeManager::g_runtimePath);

		return !errorsOccured;
	}

	//������Ŀ
	bool CMainFrame::TCBuildProj()
	{
		//�ȱ���,��������ʱԴ�ļ�
		if(!TCCompileProj())
			return false;

		//����������Ŀ
		TCBackUpProject();

		//����EXE�ļ�
		CString csHelpExe;
		csHelpExe.Format(_T("%s\\%s.exe"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);

		m_wndOutput.TCOutMsgAddString(_T(">��ʼ����..."));

		///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_SHOW);
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,10);
		//ShowProgress(TRUE);
		//TCSetProgressPos(10);
		//���ڿ�ʼ��ʾ������

		
		if(m_isVipBuild)
		{
			if(!CopyFile(CTCRunTimeManager::g_ExeBackFileVIP,csHelpExe,false))
			{
				m_wndOutput.TCOutMsgAddString(_T(">����EXEʧ��,ԭ�������EXE�ļ������ڻ�����������"),true);
				//ʧ�������ٽ�����
				TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_HIDE);
				//ShowProgress(FALSE);
				return false;
			}
		}
		else
		{
			if(!CopyFile(CTCRunTimeManager::g_ExeBackFile,csHelpExe,false))
			{
				m_wndOutput.TCOutMsgAddString(_T(">����EXEʧ��,ԭ�������EXE�ļ������ڻ�����������"),true);
				//ʧ�������ٽ�����
				TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_HIDE);
				//ShowProgress(FALSE);
				return false;
			}
		}

		//ѹ������
		CString csCompressCmd=_T("u*"),csCompressFiles;

		//��������ɵ�ѹ���ļ���Ŀ¼
		CString csDestFiles;
		csDestFiles.Format(_T("%s\\TC.lc"),CTCRunTimeManager::g_ProjBuildPath);

		CString csDextTll;
		csDextTll.Format(_T("%s\\%s.tll"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);

		//��ʼ�������
		m_wndOutput.TCOutMsgAddString(_T(">��ʼ�������..."));

		///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,20);
		//���ڿ�ʼ��ʾ������

		//��ʼ�������
		m_wndOutput.TCOutMsgAddString(_T(">���Դ�ļ�"));

		//��ѹ��Դ�ļ�
		CString csTempFile;
		csTempFile.Format(_T("%s\\sourseTemp.bak"),CTCRunTimeManager::g_ProjBuildPath);

		//��ʼ�������
		m_wndOutput.TCOutMsgAddString(_T(">�����Դ�ļ�"));
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

					//Ȼ��ѹ����Դ�ļ�
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

		//����������
		m_wndOutput.TCOutMsgAddString(_T(">����������"));

		///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,40);
		//���ڿ�ʼ��ʾ������

		//////////////////////////////////////�������������LCX�ļ�,ƽ̨ʹ�õ��ļ�////////////////////
		CString csLcxCmd=csCompressCmd+csDextTll+lcxTempFile;
		compress7z(csLcxCmd);

		//������ѹ���ļ�ͷ��ʽ���ܴ���
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
		//////////////////////////////////////�������������LCX�ļ�,ƽ̨ʹ�õ��ļ�////////////////////
		compress7z(csCompressCmd+csDestFiles+csCompressFiles);

		DeleteFolder(CTCRunTimeManager::g_ProjCompilePath,false);
		///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,70);
		//���ڿ�ʼ��ʾ������

		CString csToBuildExe;
		csToBuildExe.Format(_T(">%s ���ɳɹ�"),csHelpExe);

		///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,80);
		//���ڿ�ʼ��ʾ������

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

			//�����Ǵ���ѹ���ļ�ͷ,��������ѹ���ļ�
			DWORD toTllHead1=::GetTickCount()*::GetTickCount();
			//wchar_t lcBeg=0XEEFF;ss
			f.Write(&toTllHead1,sizeof(toTllHead1));
			//wchar_t lcBeg=0XEEFF;
			//f.Write(&lcBeg,sizeof(wchar_t));
			f.SeekToBegin();
			//������ѹ���ļ�ͷ��ԭʼ��ʽ
			//string[0]=0x37;
			//string[1]=0x7a;
			//
			//string[0]=0xFEFF;
			f.Read(string,size);

			///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,95);
			//���ڿ�ʼ��ʾ������
			//�滻��Դ�ļ�
			HANDLE   hUpdate=BeginUpdateResource(csHelpExe,FALSE);   
			int rusult;
			rusult=UpdateResource(
				hUpdate,
				_T("TCLC"),   
				MAKEINTRESOURCE(139),   
				MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),   
				string, //д�������   
				size   //���ݵ��ֽ���
				);
			rusult=EndUpdateResource(hUpdate, FALSE);
			f.Close();
			///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,100);
			//TCSetProgressPos(100);
			//���ٽ�����
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,TC_PROGRESS_HIDE);
			//ShowProgress(FALSE);
			////////////////////////////////////
			delete string;

			if(rusult==0)
			{
				m_wndOutput.TCOutMsgAddString(_T(">����EXEʧ��,�޷����EXE"),true);
				return false;
			}
		}

		//EXE��ͼ���滻
		bool needToChangeExeIcon=false;
		//����ͼ��
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

		//���ɰ汾��Ϣ
		CTCExeVersionManager::changedExeVerInfo(csHelpExe);
		if (::PathFileExists(csTempFile))
		{
			DeleteFile(csTempFile);
		}
		return true;
	}

	//������Ŀ
	bool CMainFrame::TCClearProj()
	{
		//����EXE�ļ�
		if(CTCRunTimeManager::g_ProjCompilePath.IsEmpty()||(!::PathFileExists(CTCRunTimeManager::g_ProjCompilePath)))
			return false;
		TCClearOutMsg();
		TCClearAllContentMsg();
		m_wndOutput.TCOutMsgAddString(_T(">����ʼ..."));
		//��������ļ���
		DeleteFolder(CTCRunTimeManager::g_ProjCompilePath,false);
		//�������ɹ����е��м��ļ�
		CString csHelpExe;
		csHelpExe.Format(_T("%s\\%s.exe"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		if (::PathFileExists(csHelpExe))
		{
			DeleteFile(csHelpExe);  
			m_wndOutput.TCOutMsgAddString(_T(">�ɹ���������EXE!"));
		}

		CString csTempFile;
		csTempFile.Format(_T("%s\\sourseTemp.bak"),CTCRunTimeManager::g_ProjBuildPath);
		if (::PathFileExists(csTempFile))
		{
			DeleteFile(csTempFile);  
			m_wndOutput.TCOutMsgAddString(_T(">�ɹ�������ʱ�м��ļ�!"));
		}

		CString csTempRCFile;
		csTempRCFile.Format(_T("%s\\TC.lc"),CTCRunTimeManager::g_ProjBuildPath);
		if (::PathFileExists(csTempRCFile))
		{
			DeleteFile(csTempRCFile);  
			m_wndOutput.TCOutMsgAddString(_T(">�ɹ�������ʱ��Դ�ļ�!"));
		}

		CString csDextTll;
		csDextTll.Format(_T("%s\\%s.tll"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		if (::PathFileExists(csDextTll))
		{
			DeleteFile(csDextTll);  
			m_wndOutput.TCOutMsgAddString(_T(">�ɹ�����tll�ļ�!"));
		}

		CString csLcxFile;
		csLcxFile.Format(_T("%s\\%s.lcx"),CTCRunTimeManager::g_ProjBuildPath,CTCRunTimeManager::g_ProjName);
		if (::PathFileExists(csLcxFile))
		{
			DeleteFile(csLcxFile);  
			m_wndOutput.TCOutMsgAddString(_T(">�ɹ�����lcx�ļ�!"));
		}
		m_wndOutput.TCOutMsgAddString(_T(">�������"));
		return true;
	}




	//�Լ�д��һ�����������ĵ��ķ���,��Ҫ��Ϊ�˱��Ᵽ��Ի���ĵ���
	void CMainFrame::TCSaveAllDoc()
	{
		//����T�ļ�
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

		//����������
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
		//�������ô���
		m_wndOutput.TCSetAllContentWinActive();
		AfxBeginThread(AddFindAllMsgThread,this);
	}

	void CMainFrame::TCAddAllContentString()
	{ 
		for( int count = 0; count < m_findInfo.size(); count++ )
		{
			CString line;
			line.Format(_T("�к�:(%d)--%s"),m_findInfo[count].lineNumber,m_findInfo[count].lindStr);
			m_wndOutput.TCAddAllContentWindow(line);
		}
	}

	//���һ���ϵ�
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

	//ɾ��һ���ϵ�
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

	//ɾ�����жϵ�
	void CMainFrame::TCDebugDeleteAllBP()
	{
		m_TCDebugInfor->fLeng=0;
		::memset(m_TCDebugInfor->fList,0,sizeof(MyBreakPointInfo)*MATCH_MAXBREAKBPNUM); 
	}

	void CMainFrame::TCDebugLoadVarList()
	{
		m_wndOutput.TCDebugLoadVarList(m_TCDebugInfor->fVarInfo);
	}

	//MT 2012-9-18 �ϵ���Ե�ʱ�����ֵ��ʾ
	bool CMainFrame::TCIsDebugValue(const TCHAR *name,CString &csValue)
	{
		if(m_isInBP&&m_IsDebuging)
		{
			XMLCh *pData=m_TCDebugInfor->fVarInfo;
			while((!XMLString::startsWith(pData,L"null"))&&((*pData)))
			{
				//�ռ���
				pData+=XMLString::stringLen(pData)+1;
				if(XMLString::equals(pData,name))
				{
					//������
					pData+=XMLString::stringLen(pData)+1;
					//����
					bool isArray=false;
					bool isString=false;
					if(XMLString::equals(pData,L"����"))
						isArray=true;
					else if(XMLString::equals(pData,L"�ַ���"))
						isString=true;
					pData+=XMLString::stringLen(pData)+1;

					//����
					if(isArray)
					{
						//�ж������ͷ
						if(*pData==0XA5A6)
							pData+=2;
						CString csArray;
						//�ж������β��,�����ͽ���
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
					//������
					pData+=XMLString::stringLen(pData)+1;
					//����
					bool isArray=false;
					if(XMLString::equals(pData,L"����"))
						isArray=true;
					pData+=XMLString::stringLen(pData)+1;

					//����
					if(isArray)
					{
						//�ж������ͷ
						if(*pData==0XA5A6)
							pData+=2;
						CString csArray;
						//�ж������β��,�����ͽ���
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
		//������Ŀ
		TCClearProj();

		if(!TCBuildProj())
		{
			m_IsInCompile=FALSE;
			return;
		}

		//����Ϣ��������ö�
		if(CTCRunTimeManager::g_ShowRunConfim)
		{
			CTCRunConfim dlg;
			int popup = dlg.DoModal();
			if( IDCANCEL == popup )
			{
				m_IsInCompile=FALSE;
				/////���ٽ�����
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

		//���������д򿪵�CTcView����ͼ����һ����Ϣ,����ɾ������ʱ���,��ǰ�ϵ��������ͼ��
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
			_T("δ����.tcproj"),
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			// ע���������д��
			_T("TC Files (*.tcproj)|*.tcproj||"),this);

		// ����filedlg.m_ofn.lpstrTitle ָ�����ı���Ϊ����
		filedlg.m_ofn.lpstrTitle = _T("������Ŀ�ļ�(.tcproj)");

		if(IDOK==filedlg.DoModal())
		{
			TCSaveAllDoc();

			if( CTCRunTimeManager::g_StartPageClose )
				((CTCApp*)AfxGetApp())->TCCloseAllView();
			else
				((CTCApp*)AfxGetApp())->TCCloseWithoutStartView();

			((CTCApp*)AfxGetApp())->m_pDocTemplateTc->CloseAllDocuments(TRUE);

			m_DlgManager.reset();

			//��һ������Ŀʱ����ϴ���Ŀ�������Ϣ
			TCClearOutMsg();
			//����ϸ���Ŀ��������Ϣ
			TCClearAllContentMsg();

			CString csProjFilePath,csProjName,csProjPath,csProjEnterFileName,csProjSourcePath,csRCFile,csBuildPath,csCompilePath;
			int langType=-1;
			CString csHelp=filedlg.GetPathName();
			csProjName=filedlg.GetFileTitle();
			csProjFilePath=csHelp;

			//�������ļ������ȡ�����ݵ���󳤶���1024
			::GetPrivateProfileString(_T("ProjInfo"),_T("EnterFile"),_T(""),csProjEnterFileName.GetBuffer(MAX_PATH),MAX_PATH,csProjFilePath);   //����ȡ�����ļ�
			csProjEnterFileName.ReleaseBuffer();
			if(csProjEnterFileName.IsEmpty())
			{
				::AfxMessageBox(_T("��Ŀ�����ļ�����!"));
				return false;
			}
			langType=::GetPrivateProfileInt(_T("ProjInfo"),_T("Language"),-1,csProjFilePath);

			if(!(langType==0||langType==1))
			{
				::AfxMessageBox(_T("��Ŀ�����ļ�����!"));
				return false;
			}
			CString strCurFileName;
			int iLength = csHelp.GetLength();
			int iIndex = csHelp.ReverseFind(L'\\');
			csHelp.Delete(iIndex,iLength-iIndex);
			csProjPath=csHelp;
			if (!::PathFileExists(csProjPath))
			{
				::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ��ĿĿ¼!"));
				return false;
			}       
			csProjSourcePath.Format(_T("%s\\%s"),csProjPath,csProjName);
			if (!::PathFileExists(csProjSourcePath))
			{
				::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ��ĿԴ�ļ�Ŀ¼!"));
				return false;
			}   
			csRCFile.Format(_T("%s\\��Դ\\main.twin"),csProjSourcePath);
			if (!::PathFileExists(csRCFile))
			{
				csRCFile.Format(_T("%s\\��Դ\\WinDesign.twin"),csProjSourcePath);
				if(!::PathFileExists(csRCFile))
				{
					::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ��Դ�ļ�!"));
					return false;
				}

				// csRCFile.Format(_T("%s\\��Դ\\main.twin"),csProjSourcePath);
			}   
			

			//����Դ�ļ�·�����û���
			csRCFile.Format(_T("%s\\��Դ\\main.twin"),csProjSourcePath);
			csBuildPath.Format(_T("%s\\����"),csProjPath);
			if (!::PathFileExists(csBuildPath))
			{
				//::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ����Ŀ¼!"));
				//return false;
				//���������ļ���
				if(0==CreateDirectory(csBuildPath,NULL))   
				{
					AfxMessageBox(_T("������Ŀ�����ļ���ʧ��,������!")); 
					return false;
				}
			} 

			csCompilePath.Format(_T("%s\\����"),csProjPath);
			if (!::PathFileExists(csCompilePath))
			{
				//::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ����Ŀ¼!"));
				//return false;
				//���������ļ���
				if(0==CreateDirectory(csCompilePath,NULL))   
				{
					AfxMessageBox(_T("������Ŀ�����ļ���ʧ��,������!")); 
					return false;
				}
			}



			CString csProjEnterFile;
			csProjEnterFile.Format(_T("%s\\%s"),csProjSourcePath,csProjEnterFileName);
			CTCRunTimeManager::setProjInfor(csProjPath,csProjName,csProjFilePath,csProjSourcePath,csProjEnterFile,csProjEnterFileName,csRCFile,csBuildPath,csCompilePath,langType);

			//this->m_TWinManager.load(CTCRunTimeManager::g_projRcFile);
			//����ĿĬ���ļ�,�Ƿ�ֻ��Ҫ�����򿪾Ϳ����� -match

			m_DlgManager.load(CTCRunTimeManager::g_projRcFile);

			this->ProjFileViewReload();

			((CTCApp*)AfxGetApp())->TCOpenTWin();

			/////////////////////////��ȡ�汾��Ϣ///////////////////
			CTCExeVersionManager::TCResetVersionInfo();
			CTCExeVersionManager::TCReadVersionInfo(csProjFilePath);
			/////////////////////////��ȡ�汾��Ϣ///////////////////

			//9-14�������ļ����
			((CTCApp*)AfxGetApp())->AddToRecentFileList(csProjFilePath);

			//�������Կ�
			//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(CTCRunTimeManager::g_projEnterFileName,CTCRunTimeManager::g_projEnterFile);

			return true;
		}

		return false;
	}
	/***********************************************************************/
	/*  \�������ƣ�CopyDir
	/*	\�������ܣ������ļ��м������ļ���
	/*	\�������أ�bool �����true�����Ƴɹ�������ʧ�ܡ�
	/*	\����˵����
	@param     CString strSrcPath Դ�ļ���
	@param     CString strDstPath Ŀ���ļ���
	/***********************************************************************/
	bool CopyDir(CString strSrcPath, CString strDstPath)
	{
		//���Ŀ���ļ��в������򴴽����ļ���
		if( !PathFileExists(strDstPath) )
		{
			// ����Ŀ���ļ���
			if( 0 == CreateDirectory(strDstPath,NULL) )   
			{
				AfxMessageBox(_T("������ĿԴ�ļ��ļ���ʧ��!")); 
				return false;
			}
		}

		CFileFind finder;
		// ��ָ�����ļ��н�������
		BOOL bWorking = finder.FindFile(strSrcPath + "\\" + "*.*"); 
		while(bWorking)
		{
			// �ӵ�ǰĿ¼�����ļ�
			bWorking = finder.FindNextFile();
			CString strFileName = finder.GetFileName();
			CString strSrc = strSrcPath + "\\" + strFileName;
			CString strDst = strDstPath + "\\" + strFileName;
			// �ж����������ǲ���"."��".."Ŀ¼
			if(!finder.IsDots())
			{
				// �ж���������Ŀ¼�Ƿ����ļ���
				if(finder.IsDirectory())
				{
					// ������ļ��еĻ������еݹ�
					if(!CopyDir(strSrc, strDst)) 
						return false;
				}
				else
				{
					// ������ļ������и���
					if(!CopyFile(strSrc, strDst, FALSE))
						return false;
				}
			}
		}       
		return true;
	}

	bool CMainFrame::TCBackUpProject()
	{
		//�ж��Ƿ���Դ���������Ŀ
		CString csBackUpProPath;
		csBackUpProPath.Format(_T("%s\\%s"),CTCRunTimeManager::g_ProjPath,_T("����"));

		//�����Ŀ�ļ������Ѿ����ڱ����ļ���
		if( PathFileExists(csBackUpProPath) )
		{
			//��ֱ�ӿ�����Ŀ����ļ�

			//��project�ļ�
			CString bkProjFilePath;
			bkProjFilePath.Format(_T("%s\\%s.tcproj"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			::CopyFile(CTCRunTimeManager::g_ProjFilePath,bkProjFilePath,FALSE);

			//����Ŀ�ļ��м�����Ŀ¼
			CString bkProFolderPath;
			bkProFolderPath.Format(_T("%s\\%s"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			CopyDir( CTCRunTimeManager::g_ProjSourcePath,bkProFolderPath );

			//�������ļ��м�����Ŀ¼,����Ŀ¼��û��Ҫ����??
			/*CString bkDebugFolderPath;
			bkDebugFolderPath.Format(_T("%s\\����"),csBackUpProPath);
			CopyDir( CTCRunTimeManager::g_ProjBuildPath,bkDebugFolderPath );*/

			return true;
		}
		//���򴴽������ļ���
		else
		{
			if( 0 == CreateDirectory(csBackUpProPath,NULL) )   
			{
				AfxMessageBox(_T("������ĿԴ�ļ��ļ���ʧ��,������!")); 
				return false;
			}
			//��project�ļ�
			CString bkProjFilePath;
			bkProjFilePath.Format(_T("%s\\%s.tcproj"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			::CopyFile(CTCRunTimeManager::g_ProjFilePath,bkProjFilePath,FALSE);

			//����Ŀ�ļ��м�����Ŀ¼
			CString bkProFolderPath;
			bkProFolderPath.Format(_T("%s\\%s"),csBackUpProPath,CTCRunTimeManager::g_ProjName);
			CopyDir( CTCRunTimeManager::g_ProjSourcePath,bkProFolderPath );

			//�������ļ��м�����Ŀ¼,����Ŀ¼��û��Ҫ����??
			/* CString bkDebugFolderPath;
			bkDebugFolderPath.Format(_T("%s\\����"),csBackUpProPath);
			CopyDir( CTCRunTimeManager::g_ProjBuildPath,bkDebugFolderPath );*/

			return true;
		}
		return true;
	}


	void CMainFrame::OnTcDebugpro()
	{
		// TODO: �ڴ���������������
		if(m_isInBP)
		{        
			m_isInBP=false;
			// TODO: �ڴ���������������
			//���������д򿪵�CTcView����ͼ����һ����Ϣ,����ɾ������ʱ���,��ǰ�ϵ��������ͼ��
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
			//��յ��Ա�����Ϣ
			m_wndOutput.ClearOutputVar();
			TCSetHeadPanelText(_T("����"),_T(""));
		}
	}

	void CMainFrame::OnTcDebugstep()
	{
		// TODO: �ڴ���������������
		if(m_isInBP)
		{

			// TODO: �ڴ���������������
			//���������д򿪵�CTcView����ͼ����һ����Ϣ,����ɾ������ʱ���,��ǰ�ϵ��������ͼ��
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

			TCSetHeadPanelText(_T("��������"),_T("����������Ŀ"));
			m_isInBP=false;
			m_TCDebugInfor->fDebugType=CMyDebugManager::MATCH_STEPBYSTEP;

			::SetEvent(m_LibEventHandle);
			//��յ��Ա�����Ϣ
			m_wndOutput.ClearOutputVar();
			TCSetHeadPanelText(_T("����"),_T(""));
		}
	}

	void CMainFrame::OnTcDebugstop()
	{
		// TODO: �ڴ���������������
		if(m_IsDebuging)
		{
			TCSetHeadPanelText(_T("ֹͣ����"),_T("ֹͣ������Ŀ"));
			::TerminateProcess(m_TCDebugProcessHandle,0);
			m_TCDebugProcessHandle=NULL;
			m_IsDebuging=false;
			m_IsInCompile=false;
			m_isInBP=false;
			TCSetHeadPanelText(_T("����"),_T("")); 
			m_wndOutput.ClearOutputVar();
		}
	}

	void CMainFrame::OnTcDebugrun()
	{
		// TODO: �ڴ���������������
		TCSetHeadPanelText(_T("����..."),_T("��ʼ������Ŀ"));
		if(m_IsInCompile == FALSE)
			TCSaveAllDoc();
		else
		{
			::AfxMessageBox(_T("���ڵ�����,�ȵȴ����Խ���"));
			return ;
		}
		ActiveOutPutWnd();
		m_IsInCompile=TRUE;
		AfxBeginThread(TCDebugThread,this);
		//TCDebug();
		TCSetHeadPanelText(_T("����"),_T(""));
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
		// TODO: �ڴ���������������

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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ���������������
		if(m_wndStatusBar.IsVisible())
		{
			m_wndStatusBar.ShowPane(FALSE,FALSE,FALSE);
		}
		else
			m_wndStatusBar.ShowPane(TRUE,FALSE,TRUE);
	}

	void CMainFrame::OnUpdateViewStatusBar(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ���������������
		TCSetHeadPanelText(_T("����---"),_T("���뵱ǰ��Ŀ"));
		//�������OutPut����
		if(m_IsInCompile == FALSE)
			TCSaveAllDoc();
		else
		{
			::AfxMessageBox(_T("���ڵ�����,�ȵȴ����Խ���"));
			return ;
		}
		m_IsInCompile=true;	 
		ActiveOutPutWnd();
		CWinThread *helpThread=AfxBeginThread(TCCompileProjThread,this);
		//::WaitForSingleObject(helpThread->m_hThread,INFINITE);
		// m_IsInCompile=false;
		//TCCompileProj();
		TCSetHeadPanelText(_T("����"),_T(""));
	}

	void CMainFrame::OnTcBuild()
	{
		// TODO: �ڴ���������������
		TCSetHeadPanelText(_T("����"),_T("���ɵ�ǰ��Ŀ"));
		//�������OutPut����
		if(m_IsInCompile == FALSE)
			TCSaveAllDoc();
		else
		{
			::AfxMessageBox(_T("���ڵ�����,�ȵȴ����Խ���"));
			return ;
		}
		m_IsInCompile=TRUE;
		ActiveOutPutWnd();
		CWinThread *helpThread=AfxBeginThread(TCBuildProjThread,this);
		//::WaitForSingleObject(helpThread->m_hThread ,INFINITE);

		//TCBuildProj();
		TCSetHeadPanelText(_T("����"),_T(""));
	}

	void CMainFrame::OnTcClear()
	{
		// TODO: �ڴ���������������
		TCSetHeadPanelText(_T("����"),_T("����ǰ��Ŀ"));
		TCClearProj();
		TCSetHeadPanelText(_T("����"),_T(""));
	}

	void CMainFrame::OnTcOpenbuild()
	{
		// TODO: �ڴ���������������
		//::AfxMessageBox(_T("�����ɵ�EXE���ڵ�Ŀ¼"));
		TCSetHeadPanelText(_T("��"),_T("������Ŀ¼"));
		ShellExecute(NULL, _T("open"), CTCRunTimeManager::g_ProjBuildPath, NULL, NULL, SW_SHOWNORMAL); 
		TCSetHeadPanelText(_T("����"),_T(""));
	}

	void CMainFrame::OnUpdateTcOpenbuild(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
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


	//ѹ���ĸ�ʽL"u*c:\\tc.lc*c:\\tc.exe"  �ļ���һ���ͻ����
	//��ѹ�ĸ�ʽL"x*c:\\tc.lc*c:\\topath"
	bool CMainFrame::compress7z(CString compath)
	{
		bool ret=false;
		g_StdStream = &g_StdOut;

		NConsoleClose::CCtrlHandlerSetter ctrlHandlerSetter;
		int res = 0;

		UStringVector commandStrings;

		NCommandLineParser::SplitCommandLine(compath.GetBuffer(), commandStrings);

		///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
		//TCSetProgressPos(50);
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,50);
		//���ڿ�ʼ��ʾ������

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

		///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
		TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,60);
		//TCSetProgressPos(60);
		//���ڿ�ʼ��ʾ������

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

			///////////////////////////////////���ڿ�ʼ��ʾ������/////////////////////////////////////
			TCSendOutWndMsg(TCOUTPUTCTRL::TCOUTPUT_PROGRESS,65);
			//TCSetProgressPos(65);
			//���ڿ�ʼ��ʾ������

			if(result==S_OK)
				ret=true;

			//�ռ�������Ϣ
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
			::AfxMessageBox(_T("���ܶ�λ��Ŀ�ļ�!"));
			return false;
		} 

		MyEventScanner scer;

		scer.scanDocument(addFile,namesp,fun);

		//�����Ǵ������ļ�����ͼ����
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
					//return; // �ĵ��Ѿ�����
				}
			}
		}
		if(!pEnterView)
		{
			//������ĵ�,������ô����
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
					//csHelp.Format(_T("\n\n���� �߼��� %s()\n//���������Ҫִ�еĴ���\n\n���� ��\n���ܽ���\n\n"),fun);
					////////////////////////////////////////6-11���޸Ĳ���/////////////////////////////////////////////
					if( CTCRunTimeManager::g_LangType == 0)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//����ر�_ִ�в���\n���� %s()\n\n����.�˳�()\n���ܽ���\n\n"),fun);
							else
								csHelp.Format(_T("\n//����ر�_ִ�в���\n���� %s()\n\n�ؼ�.�رմ���(\"%s\",0)\n���ܽ���\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//��Ϣ·�ɹ���\n���� %s(���,��Ϣ,w����,l����,ʱ��,x����,y����)\n\n���� ��\n���ܽ���\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//��Ϣ���̹���\n���� %s(��Ϣ,w����,l����)\n\n���ܽ���\n\n"),fun);
						}
						else
							csHelp.Format(_T("\n���� %s()\n//���������Ҫִ�еĴ���\n\n���ܽ���\n\n"),fun);
					}
					else if(CTCRunTimeManager::g_LangType==1)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//����ر�_ִ�в���\nfunction %s()\n\nhelp.exit()\nendfunction\n\n"),fun);
							else
								csHelp.Format(_T("\n//����ر�_ִ�в���\nfunction %s()\n\ncontrol.closewindow(\"%s\",0)\nendfunction\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//��Ϣ·�ɹ���\nfunction %s(hwnd,message,wParam,lParam,time,x,y)\n\nreturn false\nendfunction\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//��Ϣ���̹���\nfunction %s(message,wParam,lParam)\n\nendfunction\n\n"),fun);
						}
						else
							csHelp.Format(_T("\nfunction %s()\n//���������Ҫִ�еĴ���\n\nendfunction\n\n"),fun);
					}

					pEnterView->TCInserText(index,index,csHelp);
				}
				else
				{

					int index=pEnterView->TCGetLength();
					CString csHelp;
					//csHelp.Format(_T("\n\n���� �߼��� %s()\n//���������Ҫִ�еĴ���\n\n���� ��\n���ܽ���\n\n"),fun);
					////////////////////////////////////////6-11���޸Ĳ���////////////////////////////
					if( CTCRunTimeManager::g_LangType == 0)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//����ر�_ִ�в���\n���� %s()\n\n����.�˳�()\n���ܽ���\n\n"),fun);
							else
								csHelp.Format(_T("\n//����ر�_ִ�в���\n���� %s()\n\n�ؼ�.�رմ���(\"%s\",0)\n���ܽ���\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//��Ϣ·�ɹ���\n���� %s(���,��Ϣ,w����,l����,ʱ��,x����,y����)\n\n���� ��\n���ܽ���\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//��Ϣ���̹���\n���� %s(��Ϣ,w����,l����)\n\n���ܽ���\n\n"),fun);
						}
						else
							csHelp.Format(_T("\n�ռ� %s\n\n���� %s()\n//���������Ҫִ�еĴ���\n\n���ܽ���\n\n�ռ����\n\n"),m_pCurrentWndID/*CTCRunTimeManager::g_ProjName*/,fun);
					}
					else if(CTCRunTimeManager::g_LangType==1)
					{
						if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
						{
							if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
								csHelp.Format(_T("\n//����ر�_ִ�в���\nfunction %s()\n\nhelp.exit()\nendfunction\n\n"),fun);
							else
								csHelp.Format(_T("\n//����ر�_ִ�в���\nfunction %s()\n\ncontrol.closewindow(\"%s\",0)\nendfunction\n\n"),fun,m_pCurrentWndID);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
						{
							csHelp.Format(_T("\n//��Ϣ·�ɹ���\nfunction %s(hwnd,message,wParam,lParam,time,x,y)\n\nreturn false\nendfunction\n\n"),fun);
						}
						else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
						{
							csHelp.Format(_T("\n//��Ϣ���̹���\nfunction %s(message,wParam,lParam)\n\nendfunction\n\n"),fun);
						}
						else
							csHelp.Format(_T("\nnamespace %s\n\nfunction %s()\n//���������Ҫִ�еĴ���\n\nendfunction\n\nendnamespace\n\n"),m_pCurrentWndID/*CTCRunTimeManager::g_ProjName*/,fun);
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
						csHelp.Format(_T("\n//����ر�_ִ�в���\n���� %s()\n\n����.�˳�()\n���ܽ���\n\n"),fun);
					else
						csHelp.Format(_T("\n//����ر�_ִ�в���\n���� %s()\n\n�ؼ�.�رմ���(\"%s\",0)\n���ܽ���\n\n"),fun,m_pCurrentWndID);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
				{
					csHelp.Format(_T("\n//��Ϣ·�ɹ���\n���� %s(���,��Ϣ,w����,l����,ʱ��,x����,y����)\n//���������Ҫִ�еĴ���\n\n���� ��\n���ܽ���\n\n"),fun);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
				{
					csHelp.Format(_T("\n//��Ϣ���̹���\n���� %s(��Ϣ,w����,l����)\n\n���ܽ���\n\n"),fun);
				}
				else
					csHelp.Format(_T("\n�ռ� %s\n\n���� %s()\n//���������Ҫִ�еĴ���\n\n���ܽ���\n\n�ռ����\n\n"),CTCRunTimeManager::g_ProjName,fun);
			}
			else if(CTCRunTimeManager::g_LangType==1)
			{
				if(type == TC_EVENT_TYPE::TC_EVENT_CLOSEWINDOW)
				{
					if(0 == namesp.CollateNoCase(CTCRunTimeManager::g_ProjName))
						csHelp.Format(_T("\n//����ر�_ִ�в���\nfunction %s()\n\nhelp.exit()\nendfunction\n\n"),fun);
					else
						csHelp.Format(_T("\n//����ر�_ִ�в���\nfunction %s()\n\ncontrol.closewindow(\"%s\",0)\nendfunction\n\n"),fun,m_pCurrentWndID);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_PRETRANSMSG)
				{
					csHelp.Format(_T("\n//��Ϣ·�ɹ���\nfunction %s(hwnd,message,wParam,lParam,time,x,y)\n\nreturn false\nendfunction\n\n"),fun);
				}
				else if(type == TC_EVENT_TYPE::TC_EVENT_WINDOWPROC)
				{
					csHelp.Format(_T("\n//��Ϣ���̹���\nfunction %s(message,wParam,lParam)\n\nendfunction\n\n"),fun);
				}
				else
					csHelp.Format(_T("\nnamespace %s\n\nfunction %s()\n//���������Ҫִ�еĴ���\n\nendfunction\n\nendnamespace\n\n"),CTCRunTimeManager::g_ProjName,fun);
			}

			pEnterView->TCInserText(index,index,csHelp);
		}
		(pEnterView->GetDocument())->DoFileSave();

		pEnterView->TCActiveEditView();
		//pEnterView->TCSetFocus();
		//pEnterView->GetParent()->SetFocus();
	}


	//����¼�����
	bool CMainFrame::TCAddEventCode(CString csEvent,TC_EVENT_TYPE type)
	{
		int i=csEvent.Find(_T('.'));
		CString ns=csEvent.Left(i);
		CString fun=csEvent.Right(csEvent.GetLength()-i-1);
		return TCAddEventCode(ns,fun,type);
	}

	//�ڵ�ǰ��Դ����t�ļ�ҳ��������
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

	//������Ŀ�ļ����ͷ�ļ�
	bool CMainFrame::TCInsertCodeToMainFile(CString csCode,int docPos)
	{
		//�����Ǵ������ļ�����ͼ����
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
					return true; // �ĵ��Ѿ�����
				}
			}
		}
		//�����ǰ�ĵ�û�д򿪣���ͷ�ļ�д�������ļ�
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
				csError.Format(_T("����T�ļ�ʧ��: %s"),CTCRunTimeManager::g_projEnterFile);
				AfxMessageBox(csError); 
				return false;
			}

			DWORD dwFileSize = GetFileSize(hReadFile,  NULL);
			if(dwFileSize)
			{
				//���������ļ�����
				DWORD dwReadSize = dwFileSize/sizeof(TCHAR)+1;
				m_pBuffer=new TCHAR[dwReadSize];
				//m_length=dwReadSize;
				::memset(m_pBuffer,NULL,(dwReadSize)*sizeof(TCHAR));
				DWORD dword;
				ReadFile(hReadFile,m_pBuffer,dwFileSize,&dword,NULL);
				m_pBuffer++;//��unicodeͷ
				dwReadSize--;
				CloseHandle(hReadFile);
			}
			else
			{
				CString csError;
				csError.Format(_T("����T�ļ�ʧ��,�յ�T�ļ�: %s"),CTCRunTimeManager::g_projEnterFile);
				AfxMessageBox(csError); 
				return false;
			}

			//д�������ļ�
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
		//���ͣ��������ʾ�ӳ�Ч��
		if( m_wndOutput.IsDocked())
			m_wndOutput.ShowPane(TRUE,TRUE,TRUE);
		else
			m_wndOutput.ShowPane(TRUE,FALSE,TRUE);
		//����Tabҳ�е���Ϣ�������
		m_wndOutput.TCSetOutputMsgActive();
	}

	void CMainFrame::OnUpdateTcOpendesignwin(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
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
		// TODO: �ڴ���������������
		((CTCApp*)AfxGetApp())->TCOpenTWin();
	}

	void CMainFrame::OnButtonOpenstartview()
	{
		// TODO: �ڴ���������������
		((CTCApp*)AfxGetApp())->TCOpenStartView();
	}


	void CMainFrame::OnTcAbzhuazhua()
	{
		// TODO: �ڴ���������������
		CString abTool;
		abTool.Format(_T("%s\\ABץץ.exe"),CTCRunTimeManager::g_runtimePath);
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

		// TODO: �ڴ˴������Ϣ����������
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
		//�ر�ƽ̨����
		XMLPlatformUtils::Terminate();
	}


	void CMainFrame::OnTcComlook()
	{
		// TODO: �ڴ���������������
		CString abTool;
		abTool.Format(_T("%s\\TC�����������.exe"),CTCRunTimeManager::g_runtimePath);
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
		// TODO: �ڴ����ר�ô����/����û���
		m_TipExDebug.RelayEvent(pMsg);
		 //m_TipEx.RelayEvent(pMsg);
		return CMDIFrameWndEx::PreTranslateMessage(pMsg);
	}


	//���������ʾ
	void CMainFrame::GiveToolTip(CWnd *pWnd, LPCTSTR szTooltipText, HICON hIcon)
	{
		//���������ʾ
		m_TipEx.AddTool(pWnd, szTooltipText, NULL);
		//m_Tip.Activate(TRUE);
	}
	void CMainFrame::RemoveToolTip(CWnd *pWnd)
	{
		//�Ƴ������ʾ
		m_TipEx.RemoveTool(pWnd);
	}

	void CMainFrame::OnOpenTCHelp()
	{
		HANDLE Hhelp=FindWindow(_T("HH Parent"),_T("TC�����ĵ� v3.026 �ڲ��"));
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
		//�ؼ�����
		int type = (int)wParam;
		//�ؼ�������Ϣ
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

	/////////������һ���Զ���Ĳ�����Ϣ,�������Ե��Դ����ʱ���쳣������Ϣ��/////////////////////
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
	/////////������һ���Զ���Ĳ�����Ϣ//////////////////////////////////////////////////////////

	void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(!m_IsInCompile);
	}


	void CMainFrame::OnUpdateFileOpen(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(!m_IsInCompile);
	}




	void CMainFrame::OnUpdateFileMruFile(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(!m_IsInCompile);
	}


	void CMainFrame::OnUpdateFileClose(CCmdUI *pCmdUI)
	{
		// TODO: �ڴ������������û����洦��������
		pCmdUI->Enable(!m_IsInCompile);
	}


	void CMainFrame::OnButtonTcsetting()
	{
		// TODO: �ڴ���������������
		//m_wndSettingDia.ShowWindow(SW_SHOW);
		//m_wndSettingDia.CenterWindow();
		CTCSettingDia dlg;
		dlg.DoModal();

	}

void CMainFrame::OnTcOpenvipview()
{
	// TODO: �ڴ���������������
	((CTCApp*)AfxGetApp())->TCOpenVipView();
}


void CMainFrame::OnUpdate()
{
	// TODO: �ڴ���������������
	AfxMessageBox(_T("��⵽���°汾"));
}
