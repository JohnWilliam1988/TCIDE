// TCCode.h : ���� DLL Ӧ�ó���ĵ���������
#ifndef TCCODE_H
#define TCCODE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//��������վ����ƽ̨����������
enum    ENCRYPT_DECRYPT{Encrypt,Decrypt};
#define  MAXFILESIZE 8
// Ψһ��Ӧ�ó������

class CTCCode
{
public:
	static int  EnCodeString(char* str,char* key,char* rValueStr);
	static int  DeCodeString(char* str,char* key,char* rValueStr);
	static void EnCodeFile(wchar_t *InFile,wchar_t* key,wchar_t* OutFile);
	static void DeCodeFile(wchar_t *InFile,wchar_t* key,wchar_t* OutFile);
private:
	CTCCode();
};

#endif