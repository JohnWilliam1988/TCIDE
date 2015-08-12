#pragma once
#include "HttpSocket.h"
class CHttpDownloadFile
{
public:
	CHttpDownloadFile(void);
	~CHttpDownloadFile(void);

	// �����ļ�
	bool downloadFile(char* fileUrl);
	// ��ȡ�����ļ������·��
	CString GetDownfilePath();
	// ��ȡ���ص��ļ���
	CString GetDownFileName();
	// ���õ�ǰ·��
	void  SetCurrentPath(CString path);	
	// �ж��Ƿ���Ҫ���� 
	// CString verStr ��ʾ��ǰ����汾��
	// ����ֵ0: ��������ʧ�� 1:��ʾ����Ҫ���� 2:��ʾ��Ҫ����
	int	isNeedUpdata(CString verStr);  	
	// ��ȡ��������
	CString getDownloadFileURL();
	// ִ�����س���
	void	InstallUp();
private:
	// ��ȡ���ص��ļ���
	CString subFileName(char* fileUrl);
	// �ж��ļ��Ƿ����
	bool isFileExist(char* filename);
	// ��ȡ�ļ���С
	int	 getFileSize(char* filename);
	// �ϵ�����
	bool downloadFileEx(CString strServer,CString strObject,int startPos,CString oldName,CString newName);
	// �������ļ���
	void reNameFile(char* oldfileName,char* newfileName);
	// �ж��Ƿ��д򿪵�TC
	DWORD	FindAppProcessID();  
	
	CHttpSocket m_fHttpSocket;
	CString		m_currentPath;     // �ļ������·��
	CString		m_fileName;        // �ļ���
	CString		m_DownFileUrl;		// ���ص��ļ���ַ
};

