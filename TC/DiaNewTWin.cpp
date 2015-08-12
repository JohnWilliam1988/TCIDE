// DiaNewTWin.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "DiaNewTWin.h"
#include "afxdialogex.h"


// CDiaNewTWin 对话框

IMPLEMENT_DYNAMIC(CDiaNewTWin, CDialogEx)

CDiaNewTWin::CDiaNewTWin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaNewTWin::IDD, pParent)
	, m_newTwinName(_T("<输入名称>"))
{
	m_needNewTFile = true;
}

CDiaNewTWin::~CDiaNewTWin()
{
}

void CDiaNewTWin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTWINID, m_newTwinName);
}


BEGIN_MESSAGE_MAP(CDiaNewTWin, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDiaNewTWin::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiaNewTWin 消息处理程序


BOOL CDiaNewTWin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetBackgroundColor(0xf5e7dd);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDiaNewTWin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if(!MyValidator::validateNameNoException(m_newTwinName))
	{
		AfxMessageBox(_T("请输入正确的窗口名称,以中文或者字母或者下划线开头,名称中只能出现中文,字母,下划线,数字!"));
		return;
	}

	if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.isGoodID(m_newTwinName))
	{
		AfxMessageBox(_T("项目中已经存在当前名称的窗口,请重试!"));
		return;
	}

	CString newTFileName;
	newTFileName.Format(_T("%s.t"),m_newTwinName);
	CString newFilePath;
	newFilePath.Format(_T("%s\\%s.t"),CTCRunTimeManager::g_ProjSourcePath,m_newTwinName);
	if(PathFileExists(newFilePath))
	{
		if(IDYES == MessageBox(_T("当前路径下已经存在同名T文件,继续新建窗口？"),_T("TC"), MB_YESNO | MB_ICONINFORMATION ))
		{
			m_needNewTFile = false;
		}
		else
			return;
	}

	TCHAR winID[MAXCTRLNAME]={0};
	CString Twin;
	if( CTCRunTimeManager::g_LangType == 0 )
	{
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),_T("新建窗口"),_T("\""));
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("窗口"),m_newTwinName,diaTitle,0,0,482,306);
		Twin+=_T("\n");
		CString csID=_T("开始");   
		CString csText=_T("开始"); 
		CString title;             
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		RECT rc;
		rc.left=315;
		rc.top=255;
		rc.right=315+65;
		rc.bottom=255+28;
		CString csClick;
		csClick.Format(_T("%s.开始_点击"),m_newTwinName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");	//换行
		csID=_T("退出");
		csText=_T("退出");
		rc.left=396;
		rc.top=255;
		rc.right=396+65;
		rc.bottom=255+28;
		csClick.Format(_T("%s.退出_点击"),m_newTwinName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");//换行
		CString diaInfo;           //写入面板信息
		diaInfo.Format(_T("%s"),_T("窗口结束"));
		Twin+=diaInfo;
	}
	else if(CTCRunTimeManager::g_LangType == 1)
	{
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),_T("NewDialog"),_T("\""));
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("窗口"),m_newTwinName,diaTitle,0,0,482,306);
		Twin+=_T("\n"); 
		CString csID=_T("start");
		CString csText=_T("开始");
		CString title;
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		RECT rc;
		rc.left=315;
		rc.top=255;
		rc.right=315+65;
		rc.bottom=255+28;
		CString csClick;
		csClick.Format(_T("%s.start_click"),m_newTwinName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");
		csID=_T("exit");
		csText=_T("退出");
		rc.left=396;
		rc.top=255;
		rc.right=396+65;
		rc.bottom=255+28;
		csClick.Format(_T("%s.exit_click"),m_newTwinName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");
		CString diaInfo;
		diaInfo.Format(_T("%s"),_T("窗口结束"));
		Twin+=diaInfo;
	}
	wcscpy_s(winID,m_newTwinName);		
	//向窗口管理类中添加成功然后添加界面节点
	if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.add(winID,Twin))
	{
		if(m_needNewTFile)
		{
			//中文
			if(CTCRunTimeManager::g_LangType == 0)
			{
				byte bytes[2];
				bytes[0] = 0xFF;
				bytes[1] = 0xFE;
				wchar_t csDefCode[256]={0};
				::wsprintfW(csDefCode,_T("空间 %s\n//定义默认空间\n\n功能 开始_点击()\n//这里添加你要执行的代码\n\n返回 真\n//函数返回语句\n功能结束\n\n功能 退出_点击()\n//这里添加你要执行的代码\n\n返回 真\n//函数返回语句\n功能结束\n\n空间结束\n//空间结束语句"),winID);
				int nLength=::wcslen(csDefCode);
				int bRet;
				HANDLE handle=0;
				DWORD dwWritenSize = 0;
				handle=CreateFile(newFilePath,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
				::WriteFile(handle,bytes,2,&dwWritenSize,0);
				bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
				CloseHandle(handle);
			}
			//英文
			else if(CTCRunTimeManager::g_LangType == 1)
			{
				byte bytes[2];
				bytes[0] = 0xFF;
				bytes[1] = 0xFE;
				wchar_t csDefCode[256]={0};
				::wsprintfW(csDefCode,_T("namespace %s\n//定义默认空间\n\nfunction start_click()\n//这里添加你要执行的代码\n\nreturn true\n//函数返回语句\nendfunction\n\nfunction exit_click()\n//这里添加你要执行的代码\n\nreturn true\n//函数返回语句\nendfunction\n\nendnamespace\n//空间结束语句"),winID);
				int nLength=::wcslen(csDefCode);

				int bRet;
				HANDLE handle=0;
				DWORD dwWritenSize = 0;
				handle=CreateFile(newFilePath,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
				::WriteFile(handle,bytes,2,&dwWritenSize,0);
				bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
				CloseHandle(handle);
			}
			((CTCApp*)AfxGetApp())->m_pDocTemplateTWin->OpenDocumentFile(m_newTwinName,FALSE,TRUE);

			//向项目主文件添加头
			CString csInclude;
			csInclude.Format(_T("#include\"%s.t\"\n"),winID);
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCInsertCodeToMainFile(csInclude,0);
		}		
	}
	CDialogEx::OnOK();
}
