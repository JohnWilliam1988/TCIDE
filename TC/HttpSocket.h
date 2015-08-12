
#pragma once
#include "winsock2.h"
#include <afxinet.h>

class  CHttpSocket  
{
public:
	int				GetServerState();						//���ط�����״̬�� -1��ʾ���ɹ�
	int				GetField(const char* szSession,char *szValue,int nMaxLength);	//����ĳ����ֵ,-1��ʾ���ɹ�
	int				GetResponseLine(char *pLine,int nMaxLength);				//��ȡ����ͷ��һ��
	const char*		GetResponseHeader(int &Length);								//��ȡ�����ķ���ͷ
	const char *	FormatRequestHeader(char *pServer,char *pObject,long &Length,
						char* pCookie=NULL,char *pReferer=NULL,
						long nFrom=0,long nTo=0,
						int nServerType=0);									//��ʽ��HTTP����ͷ
	int				GetRequestHeader(char *pHeader,int nMaxLength) const;
	BOOL			SendRequest(const char* pRequestHeader = NULL,long Length = 0);
	
	CHttpSocket();
	virtual ~CHttpSocket();

	BOOL			SetTimeout(int nTime,int nType=0);
	long			Receive(char* pBuffer,long nMaxLength);
	BOOL			Connect(char* szHostName,int nPort=80);
	BOOL			Socket();
	BOOL			CloseSocket();
	BOOL			m_bResponsed;				//�Ƿ��Ѿ�ȡ���˷���ͷ
protected:	
	char m_requestheader[1024];		//����ͷ
	char m_ResponseHeader[1024];	//��Ӧͷ
	int m_port;						//�˿�
	char m_ipaddr[256];				//IP��ַ
	BOOL m_bConnected;
	SOCKET m_s;
	hostent *m_phostent;

	int m_nCurIndex;				//GetResponsLine()�������α��¼
	int m_nResponseHeaderSize;		//��Ӧͷ�Ĵ�С
};

