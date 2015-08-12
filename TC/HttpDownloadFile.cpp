#include "StdAfx.h"
#include "HttpDownloadFile.h"
#include <sys/stat.h>   // 状态显示头文件.
#include <atlconv.h>

#include <tlhelp32.h>
////////////网络的API库//////////////
#include<wininet.h>
#pragma comment(lib,"wininet.lib")

//#define _TEST_     // 用于测试下载的宏，如果不需要测试则注释此宏

int g_FileSize = 0;        // 文件大小
int g_DownloadSize = 0;    // 已经下载了多少
double g_DownloadSpeed = 0;   // 下载的速度是多少

CHttpDownloadFile::CHttpDownloadFile(void)
{
	int err;
	WORD wVersion;
	WSADATA WSAData;
	wVersion=MAKEWORD(2,0);
	err=WSAStartup(wVersion,&WSAData);
	if(err!=0)
	{
		//AfxMessageBox("无法装载Socket库.");
	}
	if(LOBYTE( WSAData.wVersion ) != 2)
	{
		//AfxMessageBox("无法找到合适的Socket库.");
		WSACleanup();
	}

	m_currentPath = _T("");
	m_DownFileUrl = _T("");
}


CHttpDownloadFile::~CHttpDownloadFile(void)
{
	WSACleanup();
}

// 获取下载文件保存的路径
CString CHttpDownloadFile::GetDownfilePath()
{
	return m_currentPath;
}
// 获取下载的文件名
CString CHttpDownloadFile::GetDownFileName()
{
	return m_fileName;
}
// 设置当前路径
void  CHttpDownloadFile::SetCurrentPath(CString path)
{
	m_currentPath = path;
}

// 判断是否需要更新
// CString verStr 表示当前软件版本号
// 返回值0: 网络连接失败 1:表示不需要更新 2:表示需要更新
int	CHttpDownloadFile::isNeedUpdata(CString verStr)
{
	HINTERNET internetopen = ::InternetOpen(L"",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if (internetopen==NULL)
	{
		return 0;
	}
	HINTERNET internetopenurl;
	CString downSite=_T("http://www.tyuyan.com/tcsoftversion.aspx?version=")+verStr;
	internetopenurl=InternetOpenUrl(internetopen,downSite,NULL,0,INTERNET_FLAG_RELOAD,0);
	if (internetopenurl==NULL)
	{
		InternetCloseHandle(internetopen);
		return 0;
	}

	BOOL internetreadfile;
	char buffer[1024]={0};
	DWORD byteread;
	internetreadfile=InternetReadFile(internetopenurl,buffer,1000,&byteread);
	if(::strlen(buffer)==0)
	{
		InternetCloseHandle(internetopenurl);
		InternetCloseHandle(internetopen);
		return 0;
	}

	InternetCloseHandle(internetopenurl);
	InternetCloseHandle(internetopen);

	CStringA retSite=buffer;
	retSite.Trim();
	if(retSite.Compare("null")==0)
	{
		return 1;
	}

	if(::strncmp(retSite,"http://www",::strlen("http://www"))==0)
	{
		m_DownFileUrl = retSite;
		return 2;
	}
	return 1;
}
// 判断是否有打开的TC
DWORD CHttpDownloadFile::FindAppProcessID()
{
	HANDLE handle=::CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);
	PROCESSENTRY32 Info;
	Info.dwSize = sizeof(PROCESSENTRY32);
	if(::Process32First(handle,&Info))
	{
		do{
			CString ss=Info.szExeFile;
			if(!ss.CompareNoCase(_T("TC.exe")))
			{
				::CloseHandle(handle);
				return Info.th32ProcessID;
			}
		}
		while(::Process32Next(handle,&Info));	
		::CloseHandle(handle);
	}
	return -1;
}

/// 截取文件名
CString CHttpDownloadFile::subFileName(char* fileUrl)
{
	CString filename(fileUrl);
	int pos = filename.ReverseFind('/');
	filename = filename.Right(filename.GetLength() - pos -1);
	m_fileName = filename;
	return filename;
}
// 获取下载连接
CString CHttpDownloadFile::getDownloadFileURL()
{
	return m_DownFileUrl;
}

// 判断文件是否存在
bool CHttpDownloadFile::isFileExist(char* filename)
{
	DWORD dwAttributes = GetFileAttributesA(filename);
    if (dwAttributes == 0xFFFFFFFF)
        return false;

	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		 ==	FILE_ATTRIBUTE_DIRECTORY)
	{
			return false;
	}
	else{
		return true;
	}
}

// 获取文件大小
int	 CHttpDownloadFile::getFileSize(char* filename)
{
	if(!isFileExist(filename)) return 0;
	struct _stat ST; 
	// 获取文件长度.
	_stat(filename, &ST);
	UINT nFilesize=ST.st_size;
	return nFilesize; 
}
// 下载网络文件
bool CHttpDownloadFile::downloadFile(char* fileUrl)
{
	g_DownloadSize = 0;
	CString strFileName = subFileName(fileUrl);  // 得到保存的
	strFileName = m_currentPath + strFileName;
	CString strServer,strObject;
	unsigned short nPort;
	DWORD dwServiceType;
	long nLength;
	const char *pRequestHeader = NULL;
	CString helpUrl = CString(fileUrl);
	AfxParseURL(helpUrl,dwServiceType,strServer,strObject,nPort);  // 解析下载链接

	USES_CONVERSION;
	// 构建HTTP头
	pRequestHeader = m_fHttpSocket.FormatRequestHeader(W2A(strServer),W2A(strObject),nLength);	
	m_fHttpSocket.Socket();    // 创建Socket套接字
	m_fHttpSocket.Connect(W2A(strServer));  // 连接服务器
	m_fHttpSocket.SendRequest();
	m_fHttpSocket.SetTimeout(10000,0);

	int nLineSize = 0;
	char szLine[256];
	while(nLineSize != -1)
	{
		nLineSize = m_fHttpSocket.GetResponseLine(szLine,256);
		if(nLineSize > -1)
		{
			szLine[nLineSize] = '\0';
		}
	}
	char szValue[30];
	m_fHttpSocket.GetField("Content-Length",szValue,30);
	int nSvrState = m_fHttpSocket.GetServerState();
	int nFileSize = atoi(szValue);
	g_FileSize = nFileSize;

	int nCompletedSize = 0;
	CFile DownloadFile;
	CString tempFileName = strFileName + ".temp";
	if(isFileExist(W2A(tempFileName)))   // 如果下载的中间文件存在就获取文件的大小
	{
		/// 中间文件存在则打开现有的文件
		int pReadFileSize = getFileSize(W2A(tempFileName));			
		// 判断中间文件大小是否和目标文件大小一致,相同大小则不下载,否则设置文件指针到相应位置
		if(pReadFileSize == nFileSize) 
		{
			reNameFile(W2A(tempFileName),W2A(strFileName));
			g_DownloadSize = pReadFileSize;
			DeleteFile(tempFileName);
			m_fHttpSocket.CloseSocket();
			return true;
		}
		else if(pReadFileSize > nFileSize)
		{
			// 文件大小异常,清楚所有内容
			DownloadFile.Open(tempFileName,CFile::modeCreate | CFile::modeReadWrite);
			//DownloadFile.SeekToBegin();
		}
		else
		{
			//// 上次下载未完成,从上次的位置继续下载
			g_DownloadSize += pReadFileSize;
			m_fHttpSocket.CloseSocket();    // 关闭原来的连接,新建断点下载连接
			return downloadFileEx(strServer,strObject,pReadFileSize,tempFileName,strFileName);
		}
	}
	else
	{
		/// 创建新的文件
		DownloadFile.Open(tempFileName,CFile::modeCreate | CFile::modeReadWrite);
	}
	nCompletedSize = g_DownloadSize;     // 只有断点下载才会有值
	char pData[1024] ={0};
	int nReceSize = 0;
	DWORD dwStartTime,dwEndTime;
	// 下载文件的操作
	while(nCompletedSize < nFileSize)
	{
		dwStartTime = GetTickCount();
		nReceSize = m_fHttpSocket.Receive(pData,1024);
		if(nReceSize == 0)
		{
			//服务器已经关闭连接.
			break;
		}
		if(nReceSize == -1)
		{
			//接收数据超时
			break;
		}
		dwEndTime = GetTickCount();
		DownloadFile.Write(pData,nReceSize);
		nCompletedSize += nReceSize;
		g_DownloadSize += nReceSize;    // 统计下载的总大小

	}
	DownloadFile.Close();
	m_fHttpSocket.CloseSocket();
	if(nCompletedSize == nFileSize)
	{
		reNameFile(W2A(tempFileName),W2A(strFileName));
		DeleteFile(tempFileName);
		return true;
	}
	else
		return false;
}

/////////////////////断点下载///////////////////////////////////
bool CHttpDownloadFile::downloadFileEx(CString strServer,CString strObject,int startPos,CString oldName,CString newName)
{
	long nLength;
	const char *pRequestHeader = NULL;
	m_fHttpSocket.m_bResponsed = FALSE;
	USES_CONVERSION;    // 编码转换宏
	pRequestHeader = m_fHttpSocket.FormatRequestHeader(W2A(strServer),W2A(strObject),nLength,
		0,0,startPos,0,0);	
	m_fHttpSocket.Socket();
	m_fHttpSocket.Connect(W2A(strServer));
	m_fHttpSocket.SendRequest();
	m_fHttpSocket.SetTimeout(10000,0);

	int nLineSize = 0;
	char szLine[256];
	while(nLineSize != -1)
	{
		nLineSize = m_fHttpSocket.GetResponseLine(szLine,256);
		if(nLineSize > -1)
		{
			szLine[nLineSize] = '\0';
		}
	}
	char szValue[30];
	m_fHttpSocket.GetField("Content-Length",szValue,30);
	int nSvrState = m_fHttpSocket.GetServerState();
	int nFileSize = atoi(szValue);

	// 上次下载未完成,从上次的位置继续下载
	CFile DownloadFile;
	DownloadFile.Open(oldName,CFile::modeWrite);
	DownloadFile.Seek(startPos,CFile::begin);

	char pData[1024] ={0};
	int nReceSize = 0;
	DWORD dwStartTime,dwEndTime;
	int nCompletedSize =0;
	// 下载文件的操作
	while(nCompletedSize < nFileSize)
	{
		dwStartTime = GetTickCount();
		nReceSize = m_fHttpSocket.Receive(pData,1024);
		if(nReceSize == 0)
		{
			//服务器已经关闭连接.
			break;
		}
		if(nReceSize == -1)
		{
			//接收数据超时
			break;
		}
		dwEndTime = GetTickCount();
		DownloadFile.Write(pData,nReceSize);
		nCompletedSize += nReceSize;
		g_DownloadSize += nReceSize;    // 统计下载的总大小
	}
	DownloadFile.Close();	
	m_fHttpSocket.CloseSocket();

	if(nCompletedSize == nFileSize)
	{
		reNameFile(W2A(oldName),W2A(newName));
		DeleteFile(oldName);
		return true;
	}
	else
		return false;
}
// 重命名文件名
void CHttpDownloadFile::reNameFile(char* oldfileName,char* newfileName)
{
	USES_CONVERSION;
	CFile oldNameFile(A2W(oldfileName),CFile::modeReadWrite);   // 源文件只读
	ULONGLONG fileSize = oldNameFile.GetLength();
	CFile newNameFile(A2W(newfileName),CFile::modeCreate|CFile::modeReadWrite);  // 新文件总是创建和读写
	char pData[1024] = {0};
	int num = 0,downAccont = 0;
	while(true)
	{
		//if(oldNameFile.Read(pData,1024) == 0)
		//	break;
		num = oldNameFile.Read(pData,1024);
		newNameFile.Write(pData,1024);
		downAccont += num;
		if(downAccont == fileSize)
			break;
	}
	newNameFile.Flush();
	oldNameFile.Close();
	newNameFile.Close();
}

void CHttpDownloadFile::InstallUp() 
{
	DWORD pId = FindAppProcessID();	
	while( pId != -1)
	{
		AfxMessageBox(_T("TC 正在运行！请先关闭程序才能进行更新！\n关闭TC之后再点击确认按钮！"),MB_OK);
		pId = FindAppProcessID();
	}

	::SetCurrentDirectoryW(m_currentPath);
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	::CreateProcess(m_fileName,NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
#if !defined _TEST_
	WaitForSingleObject( pi.hProcess, INFINITE );  // 等待安装进程完成安装.
#else
		WaitForSingleObject( pi.hProcess, 10*1000 );  // 等待10秒安装进程完成安装.
		::TerminateProcess(pi.hProcess,-1);
#endif
	DWORD pExitCode;
	GetExitCodeProcess(pi.hProcess , &pExitCode);   // 获取进程退出码
	// Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	::DeleteFileW(m_fileName);  // 删除下载的文件
}