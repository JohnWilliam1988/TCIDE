// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// TC.cpp : ����Ӧ�ó��������Ϊ��
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


//��Ŀ�½�ͷ�ļ�
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CTCApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CTCApp::OnFileOpen)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1,ID_FILE_MRU_FILE5,&CTCApp::OnRecentFile)
END_MESSAGE_MAP()

// CTCApp ����
CTCApp::CTCApp()
{
	EnableHtmlHelp();

	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TC.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CTCApp ����

CTCApp theApp;

// CTCApp ��ʼ��

BOOL CTCApp::InitInstance()
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////������δ����˵���Ըı�VS��Ĭ�����ɵ�����,��ȷû�д�,�����ڸı䴰�ڵ���ʾ��ʽ�Ժ�ᱨ��//////////////
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
        _T("����"));
    //////////////////////������δ����˵���Ըı�VS��Ĭ�����ɵ�����,��ȷû�д�,�����ڸı䴰�ڵ���ʾ��ʽ�Ժ�ᱨ��//////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	SetUnhandledExceptionFilter(ProcessAAAException); // �����Զ����������쳣���ٶ�λ�ļ���.dump��

    //�������TC�Լ��ĳ�ʼ������
	//this->m_bSaveState=false;
    CTCRunTimeManager::commonInit();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;

	ParseCommandLine(cmdInfo);

	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	//CMultiDocTemplate* pDocTemplate;
	m_pDocTemplateTc = new CMultiDocTemplate(IDR_TCTYPE,
		RUNTIME_CLASS(CTCDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CTCView));
	if (!m_pDocTemplateTc)
		return FALSE;
	AddDocTemplate(m_pDocTemplateTc);

	//�������Ҫ��ʾ����ҳ���򲻴���,ע���õ�ģ�����ĵط�

		//���һ����ҳ��ģ��
	m_pStartDocTemplate = new CMultiDocTemplate(IDR_TCSTARTTYPE,
		RUNTIME_CLASS(CStartDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CStartView));
	if (!m_pStartDocTemplate)
		return FALSE;
	AddDocTemplate(m_pStartDocTemplate);

	//���һ��VIP����ģ��
	m_pVipDocTemplate = new CMultiDocTemplate(IDI_ICON_VIP,
		RUNTIME_CLASS(CVipCenterDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CVipCenterView));
	if (!m_pVipDocTemplate)
		return FALSE;
	AddDocTemplate(m_pVipDocTemplate);

    //���һ�������Ƶ�ģ��
    m_pDocTemplateTWin = new CMultiDocTemplate(IDR_TCSTARTTYPE,
        RUNTIME_CLASS(CTCDesignDoc),
        RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
        RUNTIME_CLASS(CTCDesignView));
    if (!m_pDocTemplateTWin)
        return FALSE;
    AddDocTemplate(m_pDocTemplateTWin);



	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������/��
	m_pMainWnd->DragAcceptFiles();

	// ������׼ shell ���DDE�����ļ�������������
//	CCommandLineInfo cmdInfo;

//	ParseCommandLine(cmdInfo);

	/*if( FALSE == CTCRunTimeManager::EnablePrivilege(_T("SeDebugPrivilege"),true) )
	{
		AfxMessageBox(_T("Ȩ������ʧ��"));
	}*/

	//��ʼ�ļ�����
	ProjFileRelated();
	//���������в���
	CString cmdline;
	cmdline = ::AfxGetApp()->m_lpCmdLine;
	//����в���,�������ĵ�����Ϣ

    //����˫����tcproj�ļ���,�����д���Ĳ���
	if (cmdline.GetLength() > 0)
	{
		//���ҪFileNothing,Ҫ��Ȼ���.tcproj�ļ�
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
		cmdInfo.m_strFileName = cmdline;

		if(cmdline.Right(7)==_T(".tcproj"))
		{
			//���ֱ�ӵ���������ļ��ĺ����Ϳ�����,�����Ǵ�������.tcproj��·��
			if(OpenRecentProject(cmdline))
			{
				//�޸ı���
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
	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
    {
		//4.01��ʼҳ��Ķ�
		if( m_pStartDocTemplate != NULL && CTCRunTimeManager::g_StartPageShow )
			m_pStartDocTemplate->OpenDocumentFile(NULL);
    }
    else
    {
        if (!ProcessShellCommand(cmdInfo))
            return FALSE;
    }
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CTCApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CTCApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

//����TC�Լ�����Ŀ�½�
void CTCApp::OnFileNew()
{
    CDiaNewProj dia;
    int nResponse=dia.DoModal();
    if(nResponse == IDOK)  // OK
    {
		CTCExeVersionManager::TCResetVersionInfo();
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.reset();

        //����ĿĬ���ļ�,�Ƿ�ֻ��Ҫ�����򿪾Ϳ�����? -match
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.load(CTCRunTimeManager::g_projRcFile);
        ((CMainFrame*)AfxGetApp()->m_pMainWnd)->ProjFileViewReload();
        ((CTCApp*)AfxGetApp())->TCOpenTWin();

		 //����������
		CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
        ((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
		//����������Ŀ
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
    }
}

//����TC�Լ�����Ŀ��
void CTCApp::OnFileOpen()
{
    if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCOpenExistProject())
    {
        //����������
        CString csMainTitle;
		csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
        ((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
		//������Ŀ
		//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
    }
}

// �������жԻ����Ӧ�ó�������
void CTCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTCApp �Զ������/���淽��
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

	int ret = TCprojFileRelated(_T("tcproj"),_T("TC��Ŀ�ļ�"), TCProjIconPath, AppPath);
	int reg = TFileRelated(_T("t"),_T("t�ļ�"),tIconPath , AppPath);
	int res = TLLFileRelated(_T("tll"),_T("tll�ļ�"),tllIconPath,AppPath);

	if (ret!= NULL||reg!= NULL||res!=NULL)
		MessageBox(::AfxGetMainWnd()->m_hWnd,_T("�ļ�����ʧ��"), _T("�ļ�����"), NULL);

	//ˢ��ϵͳͼ��
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	return;
}
int CTCApp::TCprojFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath)
{
	int ret;
	HKEY hKey;
	//��չ����ע����
	CString ExtKeyName;
	ExtKeyName=FileNameExt;

	//����ļ���չ����
	FileNameExt = _T(".") + FileNameExt;
	ret = RegCreateKey(HKEY_CLASSES_ROOT, FileNameExt, &hKey);
	//������չ����Ӧ�ļ�
	ret = RegSetValue(hKey, _T(""), REG_SZ, ExtKeyName, sizeof(FileNameExt));

	//�����չ����Ӧ�ļ�
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	//������չ����˵��
	ret = RegSetValue(hKey,_T( ""), REG_SZ, ExtNote, sizeof(ExtNote));

	//�����չ����ͼ���
	ret = RegCreateKey(hKey, _T("DefaultIcon"), &hKey);
	//����ͼ���ַ
	ret = RegSetValue(hKey, _T(""), REG_SZ, IconPath, sizeof(IconPath));

	//��ӹ�������������
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
	//��չ����ע����
	CString ExtKeyName;
	ExtKeyName=FileNameExt;

	//����ļ���չ����
	FileNameExt = _T(".") + FileNameExt;
	ret = RegCreateKeyW(HKEY_CLASSES_ROOT, FileNameExt, &hKey);
	//������չ����Ӧ�ļ�
	ret = RegSetValue(hKey, _T(""), REG_SZ, ExtKeyName, sizeof(FileNameExt));

	//�����չ����Ӧ�ļ�
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	//������չ����˵��
	ret = RegSetValue(hKey,_T(""), REG_SZ, ExtNote, sizeof(ExtNote));

	//�����չ����ͼ���
	ret = RegCreateKey(hKey, _T("DefaultIcon"), &hKey);
	//����ͼ���ַ
	ret = RegSetValue(hKey, _T(""), REG_SZ, IconPath, sizeof(IconPath));

	////��ӹ�������������
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
	//��չ����ע����
	CString ExtKeyName;
	ExtKeyName=FileNameExt;

	//����ļ���չ����
	FileNameExt = _T(".") + FileNameExt;
	ret = RegCreateKey(HKEY_CLASSES_ROOT, FileNameExt, &hKey);
	//������չ����Ӧ�ļ�
	ret = RegSetValue(hKey, _T(""), REG_SZ, ExtKeyName, sizeof(FileNameExt));

	//�����չ����Ӧ�ļ�
	ret = RegCreateKey(HKEY_CLASSES_ROOT, ExtKeyName, &hKey);
	//������չ����˵��
	ret = RegSetValue(hKey,_T(""), REG_SZ, ExtNote, sizeof(ExtNote));

	//�����չ����ͼ���
	ret = RegCreateKey(hKey, _T("DefaultIcon"), &hKey);
	//����ͼ���ַ
	ret = RegSetValue(hKey, _T(""), REG_SZ, IconPath, sizeof(IconPath));

	return ret;
}


void CTCApp::LoadCustomState()
{


}

void CTCApp::SaveCustomState()
{

}

// CTCApp ��Ϣ�������

void CTCApp::TCCloseAllView()
{
    //����ĵ�ģ���λ�� 
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
		//����ȴ�Twind�Ͳ�����
		//TCOpenTWin();
		//9-14�޸�������������������ĵ�
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
                return; // �ĵ��Ѿ�����
            }
        }
    }

    //������ĵ�,������ô����
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
                return pView; // �ĵ��Ѿ�����
            }
        }
    }

    //9-14�޸ģ�������������������б�,������ô����
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
                return; // �ĵ��Ѿ�����
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
				return; // �ĵ��Ѿ�����			
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
				//�ĵ��Ѿ�����,�޸�����
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
			//�����ǰ��ͼ���ǻ��ͼ�򼤻�
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
			//		//�����ǰ��ͼ���ǻ��ͼ�򼤻�
			//	pFWnd->MDIActivate();			
   //             return; // �ĵ��Ѿ�����
   //         }
   //     }
   // }
    //������ĵ�
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
					//�����ǰ��ͼ���ǻ��ͼ�򼤻�
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
	//		//�����ǰ��ͼ���ǻ��ͼ�򼤻�
	//	pFWnd->MDIActivate();			
	//             return; // �ĵ��Ѿ�����
	//         }
	//     }
	// }
	//������ĵ�
	m_pStartDocTemplate->OpenDocumentFile(_T("��ʼ")/*CTCRunTimeManager::g_ProjName*/,FALSE,TRUE);
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
					//�����ǰ��ͼ���ǻ��ͼ�򼤻�
					pFWnd->MDIActivate();
					return;
				}
				return;
			}	
		}
	}

	m_pVipDocTemplate->OpenDocumentFile(_T("��Ա����")/*CTCRunTimeManager::g_ProjName*/,FALSE,TRUE);
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
                return; // �ĵ��Ѿ�����
            }
        }
    }
}

/***********************************************************************/
/*  \�������ƣ�TCGetTCView
/*	\�������ܣ��ж��Ƿ����TCView����,�������ñ༭���������
/*	\�������أ�BOOL ����TCView�򷵻�TRUE,���򷵻�FALSE
/*	\����˵����@NULL
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
		//���ʱ�޳�����ӵ�t�ļ�
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
	//ͨ���˵�ID�����������б��е����
	int index=uID-57616;
	CString strName;
	strName=((CTCApp*)AfxGetApp())->GetRecentFileListDisplayName(index);
	if(!OpenRecentProject(strName))
	{
		RemoveDirtyProFromRecentFileList(strName);
			return TRUE;
	}	
	AddToRecentFileList(strName);
	//�޸ı���
	CString csMainTitle;
	csMainTitle.Format(_T("%s-TC%s"),CTCRunTimeManager::g_ProjName,CTCRunTimeManager::g_TCVersion);
	((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->SetWindowText(csMainTitle);
	//11-2����������Ŀ
	//((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->TCBackUpProject();
	return TRUE;
}
 
//����Ǵӿ�ʼҳ���,��Ҫ�ڼ�����ɺ��ٹر���ͼ
BOOL CTCApp::OpenRecentProject(LPCTSTR lpszPathName)
{
	if(((CMainFrame *)((CTCApp*)AfxGetApp()->m_pMainWnd))->m_IsInCompile)
			return TRUE;

		CString isRecent=lpszPathName;		
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCSaveAllDoc();

		//�ж��Ƿ���Ҫ�رտ�ʼҳ��
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
		//�����ļ���
		CString csProjFullName = csHelp.Mid( j+1 );	
		csProjName = csProjFullName.Left(csProjFullName.GetLength()-7);
		csProjFilePath=csHelp;

		//�������ļ������ȡ�����ݵ���󳤶���1024
		::GetPrivateProfileString(_T("ProjInfo"),_T("EnterFile"),_T(""),csProjEnterFileName.GetBuffer(MAX_PATH),MAX_PATH,csProjFilePath);   //����ȡ�����ļ�
		csProjEnterFileName.ReleaseBuffer();
		if(csProjEnterFileName.IsEmpty())
		{
			::AfxMessageBox(_T("��Ŀ�����ļ�����!"));
			return FALSE;
		}
		langType=::GetPrivateProfileInt(_T("ProjInfo"),_T("Language"),-1,csProjFilePath);
		//langType=CTCRunTimeManager::getTCProjFileInt(_T("ProjInfo"),_T("Language"));
		if(!(langType==0||langType==1))
		{
			::AfxMessageBox(_T("��Ŀ�����ļ�����!"));
			return FALSE;
		}
		CString strCurFileName;
		int iLength = csHelp.GetLength();
		int iIndex = csHelp.ReverseFind(L'\\');
		csHelp.Delete(iIndex,iLength-iIndex);
		csProjPath=csHelp;
		if (!::PathFileExists(csProjPath))
		{
			::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ��ĿĿ¼!"));
			return FALSE;
		}       
		csProjSourcePath.Format(_T("%s\\%s"),csProjPath,csProjName);
		if (!::PathFileExists(csProjSourcePath))
		{
			::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ��ĿԴ�ļ�Ŀ¼!"));
			return FALSE;
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
		}   
		
		//����Դ�ļ�·�����û���
		csRCFile.Format(_T("%s\\��Դ\\main.twin"),csProjSourcePath);
		csBuildPath.Format(_T("%s\\����"),csProjPath);
		if (!::PathFileExists(csBuildPath))
		{
			/*::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ����Ŀ¼!"));
			return FALSE;*/
			if(0==CreateDirectory(csBuildPath,NULL))   
			{
				AfxMessageBox(_T("������Ŀ�����ļ���ʧ��,������!")); 
				return false;
			}
		} 

		csCompilePath.Format(_T("%s\\����"),csProjPath);
		if (!::PathFileExists(csCompilePath))
		{
			/*::AfxMessageBox(_T("��Ŀ�����ļ�����,���ܶ�λ����Ŀ¼!"));
			return FALSE;*/
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

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.load();
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ProjFileViewReload();

		/////////////////////////��ȡ�汾��Ϣ///////////////////
		CTCExeVersionManager::TCResetVersionInfo();
		CTCExeVersionManager::TCReadVersionInfo(csProjFilePath);
		/////////////////////////��ȡ�汾��Ϣ///////////////////

		((CMainFrame*)AfxGetApp()->m_pMainWnd)->OnTcOpendesignwin();

	//����ϸ���Ŀ�������Ϣ
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCClearOutMsg();
	//�������������Ϣ
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCClearAllContentMsg();
	//�������Կ�
	//((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetProjProperty(CTCRunTimeManager::g_projEnterFileName,CTCRunTimeManager::g_projEnterFile);
	return TRUE;
}




BOOL CTCApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (CSplashWnd::PreTranslateAppMessage(pMsg)) {
		return TRUE;
	}
	return CWinAppEx::PreTranslateMessage(pMsg);
}
