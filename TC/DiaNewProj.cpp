// DiaNewProj.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "DiaNewProj.h"
#include "afxdialogex.h"

#include "TCButton.h"
#include "matchLib.h"
#include "TCRunTimeManager.h"

// CDiaNewProj 对话框

IMPLEMENT_DYNAMIC(CDiaNewProj, CDialogEx)

CDiaNewProj::CDiaNewProj(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaNewProj::IDD, pParent)
    , m_ProjName(_T("<输入名称>"))
    , m_ProjPath(_T("<选择项目路径>"))
    , m_LangType(0)
{

}

CDiaNewProj::~CDiaNewProj()
{
}

void CDiaNewProj::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITPROJNAME, m_ProjName);
    DDX_Text(pDX, IDC_EDITPROJPATH, m_ProjPath);
    DDX_Radio(pDX, IDC_RADIOCN, m_LangType);
}


BEGIN_MESSAGE_MAP(CDiaNewProj, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDiaNewProj::OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDITPROJPATH, &CDiaNewProj::OnEnChangeEditprojpath)
END_MESSAGE_MAP()

// CDiaNewProj 消息处理程序

void CDiaNewProj::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	//判断是否需要关闭开始页面
	if( CTCRunTimeManager::g_StartPageClose )
		((CTCApp*)AfxGetApp())->TCCloseAllView();
	else
		((CTCApp*)AfxGetApp())->TCCloseWithoutStartView();

    int langType=m_LangType;

    if(m_ProjPath == _T("") ||m_ProjPath == _T("<选择项目路径>") )
    { 
        AfxMessageBox(_T("请选择项目路径!")); 
        return;
    }

    if(m_ProjName == _T("")||m_ProjName == _T("<输入名称>")) 
    { 
        AfxMessageBox(_T("请输入项目名称!")); 
        return;
    }

    if(!MyValidator::validateNameNoException(m_ProjName))
    {
        AfxMessageBox(_T("请输入正确的项目名称,以中文或者字母或者下划线开头,名称中只能出现中文,字母,下划线,数字!")); 
        return;
    }

    //判断路径是否可以创建项目
    CString csTempProjPath;
    csTempProjPath.Format(_T("%s\\%s"),m_ProjPath,m_ProjName);

    if(PathFileExists(csTempProjPath))
    {
        AfxMessageBox(_T("此路径下已经存在一个同名项目,无法创建,请重试!")); 
        return;
    }
    //创建项目文件夹
    if(0==CreateDirectory(csTempProjPath,NULL))
    {
        AfxMessageBox(_T("不合法的项目路径,请先选择正确的项目路径!")); 
		return;
    }
    //创建项目管理文件
    CString csTempProjFile;
    csTempProjFile.Format(_T("%s\\%s.tcproj"),csTempProjPath,m_ProjName);

    //创建源文件管理文件夹
    //判断路径是否可以创建项目
    CString csTempSourcePath;
    csTempSourcePath.Format(_T("%s\\%s"),csTempProjPath,m_ProjName);
    //照理这里是新建的,是不可能存在的,因为整个目录是新建的
    if(0==CreateDirectory(csTempSourcePath,NULL))   
    {
        AfxMessageBox(_T("创建项目源文件文件夹失败,请重试!")); 
        return;
    }
    //生成默认的项目启动源文件
    //这个是记录默认生产的项目启动文件的路径
    CString cstFileName;
    cstFileName.Format(_T("%s.t"),m_ProjName);

    CString csDefaultFile;
    csDefaultFile.Format(_T("%s\\%s.t"),csTempSourcePath,m_ProjName);
    //拷贝默认的t文件
    CString csDefSource;
    //中文
    if( langType == 0)
    {
        byte bytes[2];
        bytes[0] = 0xFF;
        bytes[1] = 0xFE;
        wchar_t csDefCode[256]={0};
        ::wsprintfW(csDefCode,_T("空间 %s\n//默认空间\n\n//开始按钮_点击操作\n功能 开始_点击()\n\n功能结束\n\n//退出按钮_点击操作\n功能 退出_点击()\n\n辅助.退出()\n功能结束\n\n空间结束\n//空间结束语句"),m_ProjName);
        int nLength=::wcslen(csDefCode);
        int bRet;
        HANDLE handle=0;
        DWORD dwWritenSize = 0;
        handle=CreateFile(csDefaultFile,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
        ::WriteFile(handle,bytes,2,&dwWritenSize,0);
        bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
        CloseHandle(handle);
    }
    //英文
    else if( langType == 1 )
    {
        byte bytes[2];
        bytes[0] = 0xFF;
        bytes[1] = 0xFE;
        wchar_t csDefCode[256]={0};
        ::wsprintfW(csDefCode,_T("namespace %s\n//默认空间\n\n//开始按钮_点击操作\nfunction start_click()\n\nendfunction\n\n//退出按钮_点击操作\nfunction exit_click()\n\nhelp.exit()\nendfunction\n\nendnamespace\n//空间结束"),m_ProjName);
        int nLength=::wcslen(csDefCode);

        int bRet;
        HANDLE handle=0;
        DWORD dwWritenSize = 0;
        handle=CreateFile(csDefaultFile,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
        ::WriteFile(handle,bytes,2,&dwWritenSize,0);
        bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
        CloseHandle(handle);
    }

    //创建资源文件夹
    CString csTempRcPath;
    csTempRcPath.Format(_T("%s\\资源"),csTempSourcePath);
    if(0==CreateDirectory(csTempRcPath,NULL))
    {
        AfxMessageBox(_T("创建资源文件夹失败，请重试!"));
        return;
    }
        
    //拷贝默认的资源文件
    CString csDefRcSourceFile;
    CString csLangType=_T("");
    //////////////////////////////////////////////6-11如果是中文拷贝默认的中文资源文件/////////////////////////
    CString csDefaultRcFile;
    csDefaultRcFile.Format(_T("%s\\main.twin"),csTempRcPath);
    //
    if( langType == 0)
	{
        CFile cFile;
        cFile.Open(csDefaultRcFile,CFile::modeCreate|CFile::modeWrite);
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),m_ProjName,_T("\""));
		CString Twin;
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("窗口"),m_ProjName,diaTitle,0,0,482,306);
		WORD unicode = 0xFEFF;     //UNICODE编码文件头
		cFile.Write(&unicode,2);   //写入面板信息
		cFile.Write(Twin,Twin.GetLength()*sizeof(TCHAR)); 
		cFile.Write(_T("\n"),2); //
		CString csID=_T("开始");   //
		CString csText=_T("开始"); //
		CString title;             //
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		RECT rc;
		rc.left=315;
        rc.top=255;
        rc.right=315+65;
        rc.bottom=255+28;
		CString csClick;
        csClick.Format(_T("%s.开始_点击"),m_ProjName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d %s| 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,_T("默认按钮:1"),csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2); //换行
		csID=_T("退出");
		csText=_T("退出");
		rc.left=396;
        rc.top=255;
        rc.right=396+65;
        rc.bottom=255+28;
		csClick.Format(_T("%s.退出_点击"),m_ProjName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2); //换行
		CString diaInfo;           //写入面板信息
		diaInfo.Format(_T("%s"),_T("窗口结束"));
		cFile.Write(diaInfo,diaInfo.GetLength()*sizeof(TCHAR));
		cFile.Close();
	}
	else if( langType == 1)
	{
        CFile cFile;
        cFile.Open(csDefaultRcFile,CFile::modeCreate|CFile::modeWrite);
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),m_ProjName,_T("\""));
		CString Twin;
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("窗口"),m_ProjName,diaTitle,0,0,482,306);
		WORD unicode = 0xFEFF;                            //UNICODE编码
		cFile.Write(&unicode,2);                          //换行
		cFile.Write(Twin,Twin.GetLength()*sizeof(TCHAR)); //写入面板信息
		cFile.Write(_T("\n"),2); 
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
        csClick.Format(_T("%s.start_click"),m_ProjName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d %s| 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,_T("默认按钮:1"),csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2);  //换行
		csID=_T("exit");
		csText=_T("退出");
		rc.left=396;
        rc.top=255;
        rc.right=396+65;
        rc.bottom=255+28;
		csClick.Format(_T("%s.exit_click"),m_ProjName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | 鼠标单击:%s|"),_T("按钮"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2);  //换行
		CString diaInfo;
		diaInfo.Format(_T("%s"),_T("窗口结束"));
		cFile.Write(diaInfo,diaInfo.GetLength()*sizeof(TCHAR));
		cFile.Close();
	}

    if( langType == 0)
        csLangType=_T("0");
    else if( langType == 1)
        csLangType=_T("1");

    //创建生成文件夹
    CString csTempBuild;
    csTempBuild.Format(_T("%s\\生成"),csTempProjPath);

    if(0==CreateDirectory(csTempBuild,NULL))   
    {
        AfxMessageBox(_T("创建项目生成文件夹失败,请重试!")); 
        return;
    }

	    //创建生成文件夹
    CString csTempCompile;
    csTempCompile.Format(_T("%s\\编译"),csTempProjPath);

    if(0==CreateDirectory(csTempCompile,NULL))   
    {
        AfxMessageBox(_T("创建项目生成文件夹失败,请重试!")); 
        return;
    }


    //设置项目信息
    CTCRunTimeManager::setProjInfor(csTempProjPath,m_ProjName,csTempProjFile,csTempSourcePath,csDefaultFile,cstFileName,csDefaultRcFile,csTempBuild,csTempCompile,langType);
    
    //写项目配置文件
    CTCRunTimeManager::setTCProjFile(_T("ProjInfo"),_T("EnterFile"),cstFileName);
    CTCRunTimeManager::setTCProjFile(_T("ProjInfo"),_T("Language"),csLangType);
	CTCRunTimeManager::setTCProjFile(_T("ProjInfo"),_T("Version"),CTCRunTimeManager::g_TCVersion);

	//csTempProjFile
	((CTCApp*)AfxGetApp())->AddToRecentFileList(csTempProjFile);

    CDialogEx::OnOK();
}


void CDiaNewProj::OnEnChangeEditprojpath()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CTCRunTimeManager::setMainSettingIni(_T("配置"),_T("项目新建位置"),m_ProjPath);
}


BOOL CDiaNewProj::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
	((CEdit*)(GetDlgItem(IDC_EDITPROJNAME)))->SetLimitText(31);
    //读取上次选择的项目路径
    CString csLastPath=CTCRunTimeManager::getMainSettingIni(_T("配置"),_T("项目新建位置"));
    m_ProjPath=csLastPath;
	this->SetBackgroundColor(0xf5e7dd);
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
