#include "stdafx.h"
#include "TCCode.h"
#include "TCAes.h"
#include "des.h"


int  CTCCode::EnCodeString(char* str,char* key,char* rValueStr)
{
	if(str&&key)
	{
		std::string sourcestr = str , sourcerstr;
		Aes aesS;
		aesS.My_Encrypt(str,key,sourcerstr);
		strcpy(rValueStr,(char*)sourcerstr.c_str());
		return 0; 
	}
}

int CTCCode::DeCodeString(char* str,char* key,char* rValueStr)
{
	std::string sourcestr = str , sourcerstr;
	Aes aesS;
	aesS.My_Decrypt(str,key,sourcerstr);
	strcpy(rValueStr,(char*)sourcerstr.c_str());
	return 0;  
}

void  CTCCode::EnCodeFile(wchar_t *InFile,wchar_t* key,wchar_t* OutFile)
{
	if(InFile&&key&&OutFile)
	{
		HANDLE infile=::CreateFile(InFile,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(infile!=INVALID_HANDLE_VALUE)
		{
			HANDLE outfile=::CreateFile(OutFile,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if(outfile!=INVALID_HANDLE_VALUE)
			{
				DES des;
				//unicode 转 anscii
				int num=::WideCharToMultiByte (CP_ACP,0,key,-1,NULL,0,NULL,NULL);
				char* key_l = new char[num];
				memset(key_l, 0, num); //初始化动作
				WideCharToMultiByte (CP_ACP,0,key, num ,key_l, num ,NULL,NULL);
				long lFileLen=::GetFileSize(infile,NULL);
				long count=lFileLen/MAXFILESIZE;	//整除,知道整个文件的块数
				long d=lFileLen%MAXFILESIZE;	//取余数
				long outLen = 0;
				char inbuff[MAXFILESIZE]={0};
				char oubuff[MAXFILESIZE]={0};
				DWORD ReadFileSize=0;
				DWORD DwWitten=0;
				for(long i=0;i<count;i++)
				{
					memset(inbuff,0,sizeof(inbuff));
					memset(oubuff,0,sizeof(oubuff));
					::ReadFile(infile,inbuff,MAXFILESIZE,&ReadFileSize ,NULL);
					des.Des_Go(oubuff, inbuff, sizeof(inbuff), key_l,sizeof(key_l), ENCRYPT1);
					::WriteFile(outfile,oubuff,MAXFILESIZE,&DwWitten,NULL);
				}
				if(d>0)
				{
					memset(inbuff,0,sizeof(inbuff));
					memset(oubuff,0,sizeof(oubuff));
					::ReadFile(infile,inbuff,d,&ReadFileSize ,NULL);
					des.Des_Go(oubuff, inbuff, MAXFILESIZE, key_l,sizeof(key_l), ENCRYPT1);
					::WriteFile(outfile,oubuff,MAXFILESIZE,&DwWitten,NULL);
				}
				delete []	key_l;
				//写入文件长度及结束符
				char endmark[2]={0xA3,0xA4};
				::WriteFile(outfile,endmark,2,&DwWitten,NULL);
				char dsize[2]={0};
				itoa(d,dsize,10);
				::WriteFile(outfile,dsize,2,&DwWitten,NULL);
			}
			CloseHandle(outfile);
		}
		CloseHandle(infile);
	}

}

void CTCCode::DeCodeFile(wchar_t *InFile,wchar_t* key,wchar_t* OutFile)
{
	if(InFile&&key&&OutFile)
	{
		HANDLE infile=::CreateFile(InFile,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(infile!=INVALID_HANDLE_VALUE)
		{
			HANDLE outfile=::CreateFile(OutFile,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if(outfile!=INVALID_HANDLE_VALUE)
			{
				DES des;
				//unicode 转 anscii
				int num=::WideCharToMultiByte (CP_ACP,0,key,-1,NULL,0,NULL,NULL);
				char* key_l = new char[num];
				memset(key_l, 0, num); //初始化动作
				WideCharToMultiByte (CP_ACP,0,key, num ,key_l, num ,NULL,NULL);
				long lFileLen=::GetFileSize(infile,NULL);
				char inbuff[MAXFILESIZE]={0};
				char oubuff[MAXFILESIZE]={0};
				DWORD ReadFileSize=0;
				DWORD DwWitten=0;
				int writSize = MAXFILESIZE;

				////////////////////////读取长度///////////////////////////
				lFileLen -= 4;//指定到自己写的结束符位置
				SetFilePointer(infile,lFileLen,0,FILE_CURRENT);//设置位置
				char inbufftem[4]={0};
				char *p=inbufftem;
				char endmark[2]={0xA3,0xA4};
				::ReadFile(infile,inbufftem,4,&ReadFileSize,NULL);
				int dd=0;

				if(inbufftem[0]==endmark[0] && inbufftem[1]==endmark[1])
				{
					p+=2;
					dd=atoi(p);
				}

				SetFilePointer(infile,0,0,FILE_BEGIN);//指针复位
				long count=lFileLen/MAXFILESIZE;	//整除,知道整个文件的块数

				for(long i=0;i<count;i++)
				{
					memset(inbuff,0,sizeof(inbuff));
					memset(oubuff,0,sizeof(oubuff));
					::ReadFile(infile,inbuff,MAXFILESIZE,&ReadFileSize ,NULL);
					des.Des_Go(oubuff, inbuff, sizeof(inbuff), key_l,sizeof(key_l), DECRYPT);
					if(i == count-1)//最后一次进来,处理oubuff值
					{
						if(dd==0)
							dd=MAXFILESIZE;
						::WriteFile(outfile,oubuff,dd,&DwWitten,NULL);//最后一次写正确的长度
						break;
					}
					::WriteFile(outfile,oubuff,MAXFILESIZE,&DwWitten,NULL);
				}
				delete []	key_l;
			}
			CloseHandle(outfile);
		}
		CloseHandle(infile);
	}

}