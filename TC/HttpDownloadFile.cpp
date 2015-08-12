#include "StdAfx.h"
#include "HttpDownloadFile.h"
#include <sys/stat.h>   // ״̬��ʾͷ�ļ�.
#include <atlconv.h>

#include <tlhelp32.h>
////////////�����API��//////////////
#include<wininet.h>
#pragma comment(lib,"wininet.lib")

//#define _TEST_     // ���ڲ������صĺ꣬�������Ҫ������ע�ʹ˺�

int g_FileSize = 0;        // �ļ���С
int g_DownloadSize = 0;    // �Ѿ������˶���
double g_DownloadSpeed = 0;   // ���ص��ٶ��Ƕ���

CHttpDownloadFile::CHttpDownloadFile(void)
{
	int err;
	WORD wVersion;
	WSADATA WSAData;
	wVersion=MAKEWORD(2,0);
	err=WSAStartup(wVersion,&WSAData);
	if(err!=0)
	{
		//AfxMessageBox("�޷�װ��Socket��.");
	}
	if(LOBYTE( WSAData.wVersion ) != 2)
	{
		//AfxMessageBox("�޷��ҵ����ʵ�Socket��.");
		WSACleanup();
	}

	m_currentPath = _T("");
	m_DownFileUrl = _T("");
}


CHttpDownloadFile::~CHttpDownloadFile(void)
{
	WSACleanup();
}

// ��ȡ�����ļ������·��
CString CHttpDownloadFile::GetDownfilePath()
{
	return m_currentPath;
}
// ��ȡ���ص��ļ���
CString CHttpDownloadFile::GetDownFileName()
{
	return m_fileName;
}
// ���õ�ǰ·��
void  CHttpDownloadFile::SetCurrentPath(CString path)
{
	m_currentPath = path;
}

// �ж��Ƿ���Ҫ����
// CString verStr ��ʾ��ǰ����汾��
// ����ֵ0: ��������ʧ�� 1:��ʾ����Ҫ���� 2:��ʾ��Ҫ����
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
// �ж��Ƿ��д򿪵�TC
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

/// ��ȡ�ļ���
CString CHttpDownloadFile::subFileName(char* fileUrl)
{
	CString filename(fileUrl);
	int pos = filename.ReverseFind('/');
	filename = filename.Right(filename.GetLength() - pos -1);
	m_fileName = filename;
	return filename;
}
// ��ȡ��������
CString CHttpDownloadFile::getDownloadFileURL()
{
	return m_DownFileUrl;
}

// �ж��ļ��Ƿ����
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

// ��ȡ�ļ���С
int	 CHttpDownloadFile::getFileSize(char* filename)
{
	if(!isFileExist(filename)) return 0;
	struct _stat ST; 
	// ��ȡ�ļ�����.
	_stat(filename, &ST);
	UINT nFilesize=ST.st_size;
	return nFilesize; 
}
// ���������ļ�
bool CHttpDownloadFile::downloadFile(char* fileUrl)
{
	g_DownloadSize = 0;
	CString strFileName = subFileName(fileUrl);  // �õ������
	strFileName = m_currentPath + strFileName;
	CString strServer,strObject;
	unsigned short nPort;
	DWORD dwServiceType;
	long nLength;
	const char *pRequestHeader = NULL;
	CString helpUrl = CString(fileUrl);
	AfxParseURL(helpUrl,dwServiceType,strServer,strObject,nPort);  // ������������

	USES_CONVERSION;
	// ����HTTPͷ
	pRequestHeader = m_fHttpSocket.FormatRequestHeader(W2A(strServer),W2A(strObject),nLength);	
	m_fHttpSocket.Socket();    // ����Socket�׽���
	m_fHttpSocket.Connect(W2A(strServer));  // ���ӷ�����
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
	if(isFileExist(W2A(tempFileName)))   // ������ص��м��ļ����ھͻ�ȡ�ļ��Ĵ�С
	{
		/// �м��ļ�����������е��ļ�
		int pReadFileSize = getFileSize(W2A(tempFileName));			
		// �ж��м��ļ���С�Ƿ��Ŀ���ļ���Сһ��,��ͬ��С������,���������ļ�ָ�뵽��Ӧλ��
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
			// �ļ���С�쳣,�����������
			DownloadFile.Open(tempFileName,CFile::modeCreate | CFile::modeReadWrite);
			//DownloadFile.SeekToBegin();
		}
		else
		{
			//// �ϴ�����δ���,���ϴε�λ�ü�������
			g_DownloadSize += pReadFileSize;
			m_fHttpSocket.CloseSocket();    // �ر�ԭ��������,�½��ϵ���������
			return downloadFileEx(strServer,strObject,pReadFileSize,tempFileName,strFileName);
		}
	}
	else
	{
		/// �����µ��ļ�
		DownloadFile.Open(tempFileName,CFile::modeCreate | CFile::modeReadWrite);
	}
	nCompletedSize = g_DownloadSize;     // ֻ�жϵ����زŻ���ֵ
	char pData[1024] ={0};
	int nReceSize = 0;
	DWORD dwStartTime,dwEndTime;
	// �����ļ��Ĳ���
	while(nCompletedSize < nFileSize)
	{
		dwStartTime = GetTickCount();
		nReceSize = m_fHttpSocket.Receive(pData,1024);
		if(nReceSize == 0)
		{
			//�������Ѿ��ر�����.
			break;
		}
		if(nReceSize == -1)
		{
			//�������ݳ�ʱ
			break;
		}
		dwEndTime = GetTickCount();
		DownloadFile.Write(pData,nReceSize);
		nCompletedSize += nReceSize;
		g_DownloadSize += nReceSize;    // ͳ�����ص��ܴ�С

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

/////////////////////�ϵ�����///////////////////////////////////
bool CHttpDownloadFile::downloadFileEx(CString strServer,CString strObject,int startPos,CString oldName,CString newName)
{
	long nLength;
	const char *pRequestHeader = NULL;
	m_fHttpSocket.m_bResponsed = FALSE;
	USES_CONVERSION;    // ����ת����
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

	// �ϴ�����δ���,���ϴε�λ�ü�������
	CFile DownloadFile;
	DownloadFile.Open(oldName,CFile::modeWrite);
	DownloadFile.Seek(startPos,CFile::begin);

	char pData[1024] ={0};
	int nReceSize = 0;
	DWORD dwStartTime,dwEndTime;
	int nCompletedSize =0;
	// �����ļ��Ĳ���
	while(nCompletedSize < nFileSize)
	{
		dwStartTime = GetTickCount();
		nReceSize = m_fHttpSocket.Receive(pData,1024);
		if(nReceSize == 0)
		{
			//�������Ѿ��ر�����.
			break;
		}
		if(nReceSize == -1)
		{
			//�������ݳ�ʱ
			break;
		}
		dwEndTime = GetTickCount();
		DownloadFile.Write(pData,nReceSize);
		nCompletedSize += nReceSize;
		g_DownloadSize += nReceSize;    // ͳ�����ص��ܴ�С
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
// �������ļ���
void CHttpDownloadFile::reNameFile(char* oldfileName,char* newfileName)
{
	USES_CONVERSION;
	CFile oldNameFile(A2W(oldfileName),CFile::modeReadWrite);   // Դ�ļ�ֻ��
	ULONGLONG fileSize = oldNameFile.GetLength();
	CFile newNameFile(A2W(newfileName),CFile::modeCreate|CFile::modeReadWrite);  // ���ļ����Ǵ����Ͷ�д
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
		AfxMessageBox(_T("TC �������У����ȹرճ�����ܽ��и��£�\n�ر�TC֮���ٵ��ȷ�ϰ�ť��"),MB_OK);
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
	WaitForSingleObject( pi.hProcess, INFINITE );  // �ȴ���װ������ɰ�װ.
#else
		WaitForSingleObject( pi.hProcess, 10*1000 );  // �ȴ�10�밲װ������ɰ�װ.
		::TerminateProcess(pi.hProcess,-1);
#endif
	DWORD pExitCode;
	GetExitCodeProcess(pi.hProcess , &pExitCode);   // ��ȡ�����˳���
	// Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	::DeleteFileW(m_fileName);  // ɾ�����ص��ļ�
}