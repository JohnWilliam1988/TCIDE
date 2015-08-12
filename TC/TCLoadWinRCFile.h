#pragma once
#include"TCRunTimeManager.h"
class TCLoadWinRCFile
{
public:
	TCLoadWinRCFile(CString winInfo/*csFilePath/*,BOOL readFile = TRUE*/);	
	~TCLoadWinRCFile();
	//��ȡ��һ�е�����
	 bool GetLineString(CString &lineBuf);
	 //�ؼ�����
	 CString GetCtrlType(CString &lineBuf);
	 //�ؼ�ID
	 CString GetCtrlID(CString &lineBuf);
     //�ҳ��ַ�����������ת���ַ�
	 CString GetString(CString &lineBuf);

	 //��������tab���ַ���
	 CString GetTabText(CString &lineBuf);

	 //�õ��ؼ��Ĵ�С
	 bool GetCtrlRECT(CString &lineBuf,LPRECT ret);
	 //�õ��ؼ�������
	 void GetCtrlProporty(TCTOOLTYPE type,CWnd *CWnd,CString &lineBuf);
	 
	 bool GetNextProporty(CString &lineBuf,CString &name,CString &value);

	 //�õ��ؼ����¼�
	 bool TCLoadWinRCFile::GetCtrlEvent(TCTOOLTYPE type,CWnd *CWnd,CString &lineBuf);
	 //
	 bool TCLoadWinRCFile::GetNextEvent(CString &lineBuf,CString &name,CString &value);

	 bool GetStrValue(CString &lineBuf,CStringArray &strarry,bool addEmpty = false);//������������б���û��Զ�����ַ�������
	 //
	 bool isEnd();
	 TCHAR *TCGetBuffer();
private:
	//��������ר�÷���
	bool GetGridRowProperty(CString &lineBuf,CString &rowPro);
	//����һ������
	bool GetGridRowNextProperty(CString &lineBuf,CString &name,CString &value);
	bool   m_isEnd;
	TCHAR *m_pBuffer;
	int    m_length;
	int    m_index;
};

