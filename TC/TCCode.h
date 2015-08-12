// TCCode.h : 定义 DLL 应用程序的导出函数。
#ifndef TCCODE_H
#define TCCODE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//用来和网站交易平台做交互的类
enum    ENCRYPT_DECRYPT{Encrypt,Decrypt};
#define  MAXFILESIZE 8
// 唯一的应用程序对象

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