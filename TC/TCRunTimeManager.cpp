#include "StdAfx.h"
#include "TCRunTimeManager.h"
#include "TC.h"
#include "TCLoadWinRCFile.h"

//初始化静态数据
CString CTCRunTimeManager::g_TCVersion=_T("4.045");     //这个是记录版本号
CString CTCRunTimeManager::g_runtimePath=_T("");        //这个是记录当前应用程序的路径
CString CTCRunTimeManager::g_MainConfigPath=_T("");     //这个是记录当前应用程序的主配置文件的路径.
CString CTCRunTimeManager::g_APIPath=_T("");            //这个是记录API的路径
CString CTCRunTimeManager::g_HelpFilePath=_T("");       //这个是记录帮助文件的名称
CString CTCRunTimeManager::g_ExeBackFile=_T("");        //这个是用来记录EXE的备份文件

CString CTCRunTimeManager::g_ExeBackFileVIP=_T("");        //这个是用来记录EXE的备份文件

//MyApiFrame * CTCRunTimeManager::g_ApiFrame;             //这个是XML的对象

CString CTCRunTimeManager::g_ProjFilePath=_T("");    //这个是记录当前项目的项目管理tcproj文件的路径
CString CTCRunTimeManager::g_ProjPath=_T("");        //这个是记录当前项目的项目的路径
CString CTCRunTimeManager::g_ProjName=_T("");        //这个是记录当前项目的项目的名称
CString CTCRunTimeManager::g_ProjSourcePath=_T("");  //这个是记录当前源文件项目的路径
CString CTCRunTimeManager::g_ProjBuildPath=_T("");   //这个是生成的路径
CString CTCRunTimeManager::g_ProjCompilePath=_T("");   //这个是编译使用的路径
CString CTCRunTimeManager::g_projEnterFile=_T("");   //这个是记录启动文件
CString CTCRunTimeManager::g_projEnterFileName=_T("");   //这个是记录启动文件名
CString CTCRunTimeManager::g_projRcFile=_T("");      //这个是记录资源文件
int     CTCRunTimeManager::g_LangType=-1;            //这个记录语言类型
bool CTCRunTimeManager::g_IsLoadProj=false;          //标识是否加载了项目
bool CTCRunTimeManager::g_ShowRunConfim=true;		//标识是否弹出运行确认对话框

CMyApiManager CTCRunTimeManager::g_TCAip;

CString CTCRunTimeManager::g_EditorFontName=_T("微软雅黑");		//编辑器字体
int	CTCRunTimeManager::g_EditorFontSize=11;						//编辑器字体大小
bool CTCRunTimeManager::g_EditorFontBold=false;					//编辑器字体加粗
int CTCRunTimeManager::g_EditorTabWidth=8;						//编辑器Tab键宽度
bool CTCRunTimeManager::g_EditorSignReplace=false;				//编辑器替换中文字符

bool CTCRunTimeManager::g_UpdateCheck=true;					//启动默认检测版本
bool CTCRunTimeManager::g_StartPageShow=true;				//启动TC时显示启动页面			
bool CTCRunTimeManager::g_StartPageClose=true;				//打开项目时关闭启动页面


CTCRunTimeManager::CTCRunTimeManager(void)
{

}

//提权操作
BOOL CTCRunTimeManager::EnablePrivilege(LPCTSTR pszPrivName, BOOL fEnable /*= TRUE*/) 
{    
    BOOL fOk = FALSE;    
    //Assume function fails    
    HANDLE hToken;    
    // Try to open this process's access token   
    if (OpenProcessToken(GetCurrentProcess(),   
        TOKEN_ADJUST_PRIVILEGES, &hToken))  
    {            
        TOKEN_PRIVILEGES tp = { 1 };       
        //如果成功查找到特权值
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

void CTCRunTimeManager::commonInit()
{
//设置当前路径,这里主要是处理双吉打开tcproj文件时,命令行执行的操作
	CString  path=((CTCApp*)AfxGetApp())->m_pszHelpFilePath;
	//当前应用程序名称 
	CString  str=((CTCApp*)AfxGetApp())->m_pszExeName;
	path=path.Left(path.GetLength()-str.GetLength()-4-1);	
	//::AfxMessageBox(path);
	::SetCurrentDirectoryW(path);
//设置当前路径,这里主要是处理双吉打开tcproj文件时,命令行执行的操作

    //这个是记录应用程序当前路径
    TCHAR pWChar[MAX_PATH]={0};
    ::GetCurrentDirectory(MAX_PATH,pWChar);
    CString strFile=pWChar;
    g_runtimePath=strFile;


    //前两个文件是必须存在的,所以一定要判断文件是否存在
    //记录api.xml的当前路径
    g_APIPath.Format(_T("%s\\api.xml"),g_runtimePath);

    if (!::PathFileExists(g_APIPath))
    {
        ::AfxMessageBox(_T("无法定位api.xml文件,可能文件被破坏,请重新安装,或者联系官方"));
        ::exit(0);
    }

	if( !g_TCAip.LoadApiXml(g_APIPath) )
	{
		AfxMessageBox(_T("初始化API管理对象失败!"));
		::exit(0);
	}

    //生成的EXE的备份文件
    g_ExeBackFile.Format(_T("%s\\exebak.bak"),g_runtimePath);
    if (!::PathFileExists(g_APIPath))
    {
        ::AfxMessageBox(_T("无法定位exebak.bak文件,可能文件被破坏,请重新安装,或者联系官方"));
        ::exit(0);
    }

    //主配置文件的当前路径
    g_MainConfigPath.Format(_T("%s\\MainSettings.ini"),g_runtimePath);

    //帮助文件的当前路径
    g_HelpFilePath.Format(_T("%s\\TC.CHM"),g_runtimePath);

    try
    {   
        //初始化平台
        XMLPlatformUtils::Initialize();
    }
    catch(...)
    {
        ::AfxMessageBox(_T("无法初始化TC解析平台!"));
        ::exit(0);
    }
	//TC4.0改动
    //MyRuntimeManager::setApiFrame(g_APIPath);
    //g_ApiFrame=MyRuntimeManager::fApiFrame;

	//初始化编辑器配置信息(字体等)
	InitEditorInfo();
	//初始化其他配置信息
	InitOtherSetting();
}

void CTCRunTimeManager::setProjInfor(CString path,CString name,CString ProjFilePath,CString sourcePath,CString enterFile,CString enterFileName,CString rcFile,CString buildPath,CString compilePath,int langType)
{
    g_ProjPath=path;
    g_ProjName=name;
    g_ProjFilePath=ProjFilePath;
    g_ProjSourcePath=sourcePath;
    g_projEnterFile=enterFile;
    g_projEnterFileName=enterFileName;
    g_projRcFile=rcFile;
    g_ProjBuildPath=buildPath;
	g_ProjCompilePath=compilePath;
    g_LangType=langType;
    g_IsLoadProj=true;
}

bool CTCRunTimeManager::setMainSettingIni(CString appName,CString keyName,CString value)
{
	if(!::PathFileExists(g_MainConfigPath))
	{
		//新建文件
		_TCHAR bom = (_TCHAR)0xFEFF;
		CFile pFile;

		bool b_Open=false;
		b_Open=pFile.Open(g_MainConfigPath,CFile::modeCreate | CFile::modeWrite );
		if(b_Open==false)
		{
			AfxMessageBox(_T("文件创建失败!"));
			return false;
		}

		pFile.Write(&bom,sizeof(_TCHAR));
		pFile.Close();
	}
   return ::WritePrivateProfileString(appName,keyName,value,g_MainConfigPath);   //写主取配置文件
}

CString CTCRunTimeManager::getMainSettingIni(CString appName,CString keyName)
{
    CString csReturn;
    //从配置文件上面读取的内容的最大长度是1024
    ::GetPrivateProfileString(appName,keyName,_T(""),csReturn.GetBuffer(1024),1024,g_MainConfigPath);   //读主取配置文件
    csReturn.ReleaseBuffer();
    return csReturn;
}

 bool CTCRunTimeManager::setTCProjFile(CString appName,CString keyName,CString value)
 {
	 if(!::PathFileExists(g_ProjFilePath))
	{
		//新建文件
		_TCHAR bom = (_TCHAR)0xFEFF;
		CFile pFile;

		bool b_Open=false;
		b_Open=pFile.Open(g_ProjFilePath,CFile::modeCreate | CFile::modeWrite );
		if(b_Open==false)
		{
			AfxMessageBox(_T("文件创建失败!"));
			return false;
		}

		pFile.Write(&bom,sizeof(_TCHAR));
		pFile.Close();
	}
    return ::WritePrivateProfileString(appName,keyName,value,g_ProjFilePath);   //写主取配置文件
 }

 CString CTCRunTimeManager::getTCProjFile(CString appName,CString keyName)
 {
     CString csReturn;
    //从配置文件上面读取的内容的最大长度是1024
    ::GetPrivateProfileString(appName,keyName,_T(""),csReturn.GetBuffer(1024),1024,g_ProjFilePath);   //读主取配置文件
    csReturn.ReleaseBuffer();
    return csReturn;
 }

 int CTCRunTimeManager::getTCProjFileInt(CString appName,CString keyName)
 {
     int langType=-1;
    ::GetPrivateProfileInt(appName,keyName,langType,g_ProjFilePath);   //读主取配置文件
    return langType;
 }
 
 bool CTCRunTimeManager::isSameFile(CString file1,CString file2)
 {
    file1.Trim();
    file1.MakeLower();
    file2.Trim();
    file2.MakeLower();
    return (file1==file2);
 }

void CTCRunTimeManager::getRCPathBmpList(CStringArray &bmpArray)
{
    bmpArray.RemoveAll();

    CString rcPath;
    rcPath.Format(_T("%s//资源//"),g_ProjSourcePath);

    TCHAR szFind[MAX_PATH]={0};
    TCHAR szFile[MAX_PATH]={0};
    WIN32_FIND_DATA FindFileData;
    wcscpy_s(szFind,rcPath);
    wcscat_s(szFind,_T("*.*"));
    HANDLE hFind=::FindFirstFile(szFind,&FindFileData);

    if(INVALID_HANDLE_VALUE == hFind)
        return;

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
            CString csFileName=FindFileData.cFileName;
            csFileName.Trim();
            csFileName.MakeLower();
            CString csPictEx=csFileName.Right(4);
            if(0==::wcscmp(csPictEx,_T(".jpg"))||
                0==::wcscmp(csPictEx,_T(".bmp"))||
                0==::wcscmp(csPictEx,_T(".png"))||
                0==::wcscmp(csPictEx,_T(".ico"))||
                0==::wcscmp(csPictEx,_T(".gif"))
                )
            {
                bmpArray.Add(FindFileData.cFileName);
            }

        }
        if(!FindNextFile(hFind,&FindFileData))
            break;
    }
    FindClose(hFind);
}

bool CTCRunTimeManager::isMatchStatement(CString statement)
{
	statement.Trim();
	if(statement==_T("如果结束"))
		return true;
	else if(statement==_T("循环结束"))
		return true;
	else if(statement==_T("遍历结束"))
		return true;
	else if(statement==_T("否则"))
		return true;
	else if(statement==_T("选择结束"))
		return true;
	else if(statement==_T("功能结束"))
		return true;
	else if(statement==_T("endif"))
		return true;
	else if(statement==_T("endwhile"))
		return true;
	else if(statement==_T("endfor"))
		return true;
	else if(statement==_T("else"))
		return true;
	else if(statement==_T("endselect"))
		return true;
	else if(statement==_T("endfunction"))
		return true;
	return false;
}

bool CTCRunTimeManager::isMatchStatementEx(CString statement)
{
	statement.Replace(L" ",L"");
	int cc=wcslen(_T("如果结束//"));
	int ss=wcslen(statement);
	if(::wcsncmp(statement,_T("如果结束//"),::wcslen(_T("如果结束//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("循环结束//"),::wcslen(_T("循环结束//")))==0)
	{
		return true;
	}		
	else if(::wcsncmp(statement,_T("遍历结束//"),::wcslen(_T("遍历结束//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("否则//"),::wcslen(_T("否则//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("选择结束//"),::wcslen(_T("选择结束//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("空间结束//"),::wcslen(_T("空间结束//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("功能结束//"),::wcslen(_T("功能结束//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("endif//"),::wcslen(_T("endif//")))==0)
	{
		return true;
	}		
	else if(::wcsncmp(statement,_T("endwhile//"),::wcslen(_T("endwhile//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("endfor//"),::wcslen(_T("endfor//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("else//"),::wcslen(_T("else//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("endselect//"),::wcslen(_T("endselect//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("endfunction//"),::wcslen(_T("endfunction//")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("endnamespace//"),::wcslen(_T("endnamespace//")))==0)
	{
		return true;
	}
	return false;


}


bool CTCRunTimeManager::isBraceStatement(CString statement)
{
	statement.Trim();

	if(::wcsncmp(statement,_T("如果"),::wcslen(_T("如果")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("如果")));
		if(ch==' '||ch=='(')
			return true;
	}
	else if(::wcsncmp(statement,_T("循环"),::wcslen(_T("循环")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("循环")));
		if(ch==' '||ch=='(')
			return true;
	}		
	else if(::wcsncmp(statement,_T("遍历"),::wcslen(_T("遍历")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("遍历")));
		if(ch==' '||ch=='(')
			return true;
	}
	else if(::wcsncmp(statement,_T("否则"),::wcslen(_T("否则")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("选择"),::wcslen(_T("选择")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("选择")));
		if(ch==' '||ch=='(')
			return true;
	}
	else if(::wcsncmp(statement,_T("功能"),::wcslen(_T("功能")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("功能")));
		if(ch==' ')
			return true;
	}
	else if(::wcsncmp(statement,_T("if"),::wcslen(_T("if")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("if")));
		if(ch==' '||ch=='(')
			return true;
	}
	else if(::wcsncmp(statement,_T("while"),::wcslen(_T("while")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("while")));
		if(ch==' '||ch=='(')
			return true;
	}		
	else if(::wcsncmp(statement,_T("for"),::wcslen(_T("for")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("for")));
		if(ch==' '||ch=='(')
			return true;
	}
	else if(::wcsncmp(statement,_T("else"),::wcslen(_T("else")))==0)
	{
		return true;
	}
	else if(::wcsncmp(statement,_T("select"),::wcslen(_T("select")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("select")));
		if(ch==' '||ch=='(')
			return true;
	}
	else if(::wcsncmp(statement,_T("function"),::wcslen(_T("function")))==0)
	{
		wchar_t ch=statement.GetAt(::wcslen(_T("function")));
		if(ch==' ')
			return true;
	}
	return false;
}

 void CTCRunTimeManager::InitEditorInfo()
 {
	 CString fontFace = getMainSettingIni(_T("Editor"),_T("FontFace"));
	 CString fontSize = getMainSettingIni(_T("Editor"),_T("FontSize"));
	 CString fontBold = getMainSettingIni(_T("Editor"),_T("FontBold"));
	 CString tabWidth = getMainSettingIni(_T("Editor"),_T("TabWidth"));
	 CString replaceSign = getMainSettingIni(_T("Editor"),_T("ReplaceSign"));

	 //字体
	 if( fontFace.GetLength() != 0 )
		 g_EditorFontName = fontFace;
	 //字体大小
	 if( fontSize.GetLength() != 0 )
		 g_EditorFontSize = _wtoi(fontSize);
	 //粗体
	 if( fontBold.GetLength() != 0 )
	 {
		 bool isFontBold = false;
		 fontBold == _T("1") ? g_EditorFontBold = true : g_EditorFontBold = false; 	 
	 }
	 //Tab宽度
	 if( tabWidth.GetLength() != 0 )
		 g_EditorTabWidth = _wtoi(tabWidth);

	 if( replaceSign.GetLength() != 0 )
		 replaceSign == _T("1") ? g_EditorSignReplace = true : g_EditorSignReplace = false;

	 //如果没有读到配置信息则使用默认的
 }

 void CTCRunTimeManager::InitOtherSetting()
 {
	 CString runconfirm = getMainSettingIni(_T("Debug"),_T("Runconfirm"));
	 if ( 0 == runconfirm.CompareNoCase(_T("False")) )
		 g_ShowRunConfim = false;
	 CString showStartPage = getMainSettingIni(_T("StartPage"),_T("Show"));
	 if ( 0 == showStartPage.CompareNoCase(_T("False")) )
		 g_StartPageShow = false;
	 CString closeStartPage =  getMainSettingIni(_T("StartPage"),_T("Close"));
	 if ( 0 == closeStartPage.CompareNoCase(_T("False")) )
		 g_StartPageClose = false;
	 CString updateCheck = getMainSettingIni(_T("Update"),_T("Check"));
	 if ( 0 == updateCheck.CompareNoCase(_T("0")) )
		 g_UpdateCheck = false;
 }

 void CTCRunTimeManager::SetEditorInfo(EditorSetInfo editorInof)
 {
	 g_EditorFontName = editorInof.fontFace;
	 g_EditorFontSize = editorInof.fontSize;
	 g_EditorFontBold = editorInof.fontBold;
	 g_EditorTabWidth = editorInof.tabWidth;
	 g_EditorSignReplace = editorInof.replaceSign;

	 //写入配置信息
	 CString fontsizeStr,boldStr,tabWidthStr,replaceSignStr;
	 fontsizeStr.Format(_T("%d"),g_EditorFontSize);
	 boldStr.Format(_T("%d"),g_EditorFontBold);
	 tabWidthStr.Format(_T("%d"),g_EditorTabWidth);
	 replaceSignStr.Format(_T("%d"),g_EditorSignReplace);
	 setMainSettingIni(_T("Editor"),_T("FontFace"),g_EditorFontName);
	 setMainSettingIni(_T("Editor"),_T("FontSize"),fontsizeStr);
	 setMainSettingIni(_T("Editor"),_T("FontBold"),boldStr);
	 setMainSettingIni(_T("Editor"),_T("TabWidth"),tabWidthStr);
	 setMainSettingIni(_T("Editor"),_T("ReplaceSign"),replaceSignStr);
 }

 void CTCRunTimeManager::SetUpdateInfo(bool check)
 {
	 g_UpdateCheck = check;
	 CString updateCheck;
	 updateCheck.Format(_T("%d"),g_UpdateCheck);
	 setMainSettingIni(_T("Update"),_T("Check"),updateCheck);
 }

 //面板管理
 CTCDlgManager::CTCDlgManager()
 {
	 m_length=0;
	 //memset(m_list,0,sizeof(wndNode)*MAXCTRLLENGTH);
 }

 CTCDlgManager::~CTCDlgManager()
 {
	 m_length=0;
	 //memset(m_list,0,sizeof(wndNode)*MAXCTRLLENGTH);
 }

 void CTCDlgManager::load(CString rcPath)
 {
	 TCHAR *m_pBuffer = NULL;
	 HANDLE hReadFile = CreateFileW(rcPath,      // name of the readfile
		 GENERIC_READ,           // open for writing
		 FILE_SHARE_READ,        // do not share
		 NULL,                   // default security
		 OPEN_EXISTING,          // overwrite existing
		 FILE_ATTRIBUTE_NORMAL,  // normal file
		 NULL);                  // no attr. template

	 if (hReadFile == INVALID_HANDLE_VALUE) 
	 { 
		 CString csError;
		 csError.Format(_T("加载界面配置文件失败: %s"),rcPath);
		 MessageBox(0,csError,_T("TC"),NULL); 
		 //exit(0) ;
	 }
	 DWORD dwFileSize = GetFileSize(hReadFile,  NULL);
	 if(dwFileSize)
	 {
		 //读出配置文件内容
		 SetFilePointer(hReadFile,2,0,FILE_BEGIN);
		 DWORD dwReadSize = dwFileSize/sizeof(TCHAR)+1;
		 m_pBuffer=new TCHAR[dwReadSize];
		 ::memset(m_pBuffer,NULL,(dwReadSize)*sizeof(TCHAR));
		 DWORD dword;
		 ReadFile(hReadFile,m_pBuffer,dwFileSize,&dword,NULL);
		 CloseHandle(hReadFile);
	 }
	 else
	 {
		 CString csError;
		 csError.Format(_T("加载界面配置文件失败,空的界面配置文件: %s"),rcPath);
		 MessageBox(0,csError,_T("TC"),NULL); 
	 }
	 //吃掉Unicode头
	 //m_pBuffer++;

	 TCLoadWinRCFile rcFile(m_pBuffer);
	 CString csLine;
	 TCHAR winID[MAXCTRLNAME]={0};
	 CString winInfo;
	 while(rcFile.GetLineString(csLine))
	 {
		 if(csLine.Left(3) == _T("窗口 "))
		 {
			 CString wndPerpro = csLine;
			 CString csType = rcFile.GetCtrlType(wndPerpro);
			 CString csID = rcFile.GetCtrlID(wndPerpro); 
			  //如果是以前默认的窗口0，则把它换成项目名
			  if( csID == _T("窗口0"))
			 	 csID = CTCRunTimeManager::g_ProjName;
			  wcscpy_s(winID,csID);
			  winInfo += csLine + _T("\n");
			  continue;
		 }
		 else if(csLine.Left(4) == _T("窗口结束"))
		 {
			 winInfo += csLine;
			 add(winID,winInfo);
			 winInfo.Empty();
			 continue;
		 }
		 winInfo += csLine + _T("\n");		 		 
	 }

	 if(m_pBuffer != NULL)
		 delete m_pBuffer;
 }

 //将所有节点信息写入文件
 void CTCDlgManager::write(CString rcPath)
 {
	 if(0==m_length)
		 return;

	 CString wndInfo;
	 for(int count=0;count < m_length;count++)
	 {
		wndInfo += m_list[count].winInfo + _T("\n");
 	 }

	 //将所有面板节点信息写入文件
	 CFile cFile;
	 cFile.Open(rcPath,CFile::modeCreate|CFile::modeWrite);
	 WORD unicode = 0xFEFF; //UNICODE编码文件头
	 cFile.Write(&unicode,2);
	 cFile.Write(wndInfo,wndInfo.GetLength()*sizeof(TCHAR));
	 cFile.Close();
 }

 unsigned int CTCDlgManager::getCount() const
 {
	 return m_length;
 }

 bool CTCDlgManager::add(wndNode *node)
 {
	 if(m_length>=MAXCTRLLENGTH)
		 return false;

	 ::wcscpy(m_list[m_length].id,node->id);
	 m_list[m_length].winInfo=node->winInfo;

	 m_length++;

	 return true;
 }

 bool CTCDlgManager::add(TCHAR * const id,CString winInfo)
 {
	 if(m_length>=MAXCTRLLENGTH)
		 return false;

	 if((::wcslen(id)>=MAXCTRLNAME))
		 return false;

	if(!isGoodID(id))
		 return false;

	::wcscpy(m_list[m_length].id,id);
	m_list[m_length].winInfo=winInfo;

	 m_length++;

	 return true;
 }


 bool CTCDlgManager::isGoodID(const TCHAR * const id)
 {
	 //TC4.0改动
	 if(!MyValidator::validateNameNoException(id))
		 return false;

	 if(!id||(::wcslen(id)>=MAXCTRLNAME))
		 return false;

	 for(int i=0;i<m_length;i++)
	 {
		 if(::wcsicmp(m_list[i].id,id)==0)
			 return false;
	 }

	 return true;
 }

 const wndNode *CTCDlgManager::getat(unsigned int index) const 
 {
	 if(index>=m_length)
		 return NULL;
	 return  &m_list[index];
 }

 wndNode *CTCDlgManager::getat(unsigned int index)
 {
	 if(index>=m_length)
		 return NULL;
	 return  &m_list[index];
 }

 //int CTCDlgManager::isContainWnd(CWnd *hwnd) const
 //{
	// if(!hwnd)
	//	 return -1;

	// for(int i=0;i<m_length;i++)
	// {
	//	 if(m_list[i].hwnd==hwnd)
	//		 return i;
	// }

	// return -1;
 //}

 void CTCDlgManager::reset()
 {
	 m_length=0;
	 //memset(m_list,0,sizeof(wndNode)*MAXCTRLLENGTH);
 }

 //bool CTCDlgManager::wndChangeID(CWnd *hwnd,const TCHAR * const id)
 //{
	// if((!hwnd)||(!id))
	//	 return false;

	// for(int i=0;i<m_length;i++)
	// {
	//	 if(m_list[i].hwnd==hwnd)
	//	 {
	//		 ::memset(m_list[i].id,0,MAXCTRLNAME*sizeof(TCHAR));
	//		 ::wcscpy(m_list[i].id,id);
	//		 return true;
	//	 }
	// }

	// return false;
 //}

 int CTCDlgManager::getIndexByID(const TCHAR * const id)
 {
	 if(!id)
		 return -1;

	 for(int i=0;i<m_length;i++)
	 {
		 if(::wcscmp(m_list[i].id,id)==0)
			 return i;
	 }

	 return -1;
 }

 bool CTCDlgManager::removeWndInfo(const TCHAR * const id)
 {
	 if(!id)
		 return false;

	 int index = getIndexByID(id);
	 if(-1 == index)
		 return false;

	 if(index==m_length-1)
	 {
		 ::memset(m_list[index].id,0,MAXCTRLNAME*sizeof(TCHAR));
		 m_list[index].winInfo.Empty();
	 }
	 else
	 {
		 for(int i=index;i<m_length-1;i++)
		 {
			 ::wcscpy(m_list[i].id,m_list[i+1].id);
			 m_list[i].winInfo=m_list[i+1].winInfo;
		 }
	 }
	 m_length--;
	  return true;
 }

 int CTCDlgManager::GetLength()
 {
	 return m_length;
 }