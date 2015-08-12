#pragma once
#include"TCRunTimeManager.h"
class TCLoadWinRCFile
{
public:
	TCLoadWinRCFile(CString winInfo/*csFilePath/*,BOOL readFile = TRUE*/);	
	~TCLoadWinRCFile();
	//获取下一行的数据
	 bool GetLineString(CString &lineBuf);
	 //控件类型
	 CString GetCtrlType(CString &lineBuf);
	 //控件ID
	 CString GetCtrlID(CString &lineBuf);
     //找出字符常量并处理转义字符
	 CString GetString(CString &lineBuf);

	 //单独处理tab的字符串
	 CString GetTabText(CString &lineBuf);

	 //得到控件的大小
	 bool GetCtrlRECT(CString &lineBuf,LPRECT ret);
	 //得到控件的属性
	 void GetCtrlProporty(TCTOOLTYPE type,CWnd *CWnd,CString &lineBuf);
	 
	 bool GetNextProporty(CString &lineBuf,CString &name,CString &value);

	 //得到控件的事件
	 bool TCLoadWinRCFile::GetCtrlEvent(TCTOOLTYPE type,CWnd *CWnd,CString &lineBuf);
	 //
	 bool TCLoadWinRCFile::GetNextEvent(CString &lineBuf,CString &name,CString &value);

	 bool GetStrValue(CString &lineBuf,CStringArray &strarry,bool addEmpty = false);//处理下拉框和列表框用户自定义的字符串数据
	 //
	 bool isEnd();
	 TCHAR *TCGetBuffer();
private:
	//报表属性专用方法
	bool GetGridRowProperty(CString &lineBuf,CString &rowPro);
	//报表一个属性
	bool GetGridRowNextProperty(CString &lineBuf,CString &name,CString &value);
	bool   m_isEnd;
	TCHAR *m_pBuffer;
	int    m_length;
	int    m_index;
};

