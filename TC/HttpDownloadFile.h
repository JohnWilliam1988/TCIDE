#pragma once
#include "HttpSocket.h"
class CHttpDownloadFile
{
public:
	CHttpDownloadFile(void);
	~CHttpDownloadFile(void);

	// 下载文件
	bool downloadFile(char* fileUrl);
	// 获取下载文件保存的路径
	CString GetDownfilePath();
	// 获取下载的文件名
	CString GetDownFileName();
	// 设置当前路径
	void  SetCurrentPath(CString path);	
	// 判断是否需要更新 
	// CString verStr 表示当前软件版本号
	// 返回值0: 网络连接失败 1:表示不需要更新 2:表示需要更新
	int	isNeedUpdata(CString verStr);  	
	// 获取下载连接
	CString getDownloadFileURL();
	// 执行下载程序
	void	InstallUp();
private:
	// 截取下载的文件名
	CString subFileName(char* fileUrl);
	// 判断文件是否存在
	bool isFileExist(char* filename);
	// 获取文件大小
	int	 getFileSize(char* filename);
	// 断点下载
	bool downloadFileEx(CString strServer,CString strObject,int startPos,CString oldName,CString newName);
	// 重命名文件名
	void reNameFile(char* oldfileName,char* newfileName);
	// 判断是否有打开的TC
	DWORD	FindAppProcessID();  
	
	CHttpSocket m_fHttpSocket;
	CString		m_currentPath;     // 文件保存的路径
	CString		m_fileName;        // 文件名
	CString		m_DownFileUrl;		// 下载的文件地址
};

