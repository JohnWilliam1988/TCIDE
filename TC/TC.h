// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// TC.h : TC 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

//#include "TCCtlHeads.h"

#include "resource.h"       // 主符号

#include "RedoUndo\undo.h"

#include "TCRunTimeManager.h"

//#include "TCTab.h"

// CTCApp:
// 有关此类的实现，请参阅 TC.cpp
//

class CTCApp : public CWinAppEx
{
public:
	CTCApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

//三个文档模板
    CMultiDocTemplate* m_pDocTemplateTc;
    CMultiDocTemplate* m_pDocTemplateTWin;
	CMultiDocTemplate* m_pStartDocTemplate;
	CMultiDocTemplate* m_pVipDocTemplate;

//功能
    void TCCloseAllView();
	//如果在开始页面里面选择了不关闭启动页面的时候调用
	void TCCloseWithoutStartView();			
    //t源文件的打开,删除,重命名操作
    void TCOpenTFile(CString file);

    CView *TCOpenTFileReturnView(CString file);
	//根据窗口ID遍历当前视类，并返回
	CView *TCGetTCDesignView(CString winName);

    void TCDeleteTFileView(CString file);
	void TCDeleteDesignView(CString winID);
    void TCRenameTFile(CString oldPath,CString newPath);
	void TCRenameTWin(CString oldStr,CString newStr);
    //twin界面文件的打开,删除,重命名操作

    void TCOpenTWin(CString winID=CTCRunTimeManager::g_ProjName);
	void TCOpenStartView();
    void TCDeleteTWinView();

	void TCOpenVipView();

	//12-11-13,周俊亮,遍历是否存在TCView视类
	CView *TCGetTCView();

public:



// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	//9-14添加最近打开项目
	afx_msg BOOL OnRecentFile(UINT uID) ;
	void AddToRecentFileList(LPCTSTR lpszPathName);
	CString GetRecentFileListDisplayName(int index);
	void RemoveDirtyProFromRecentFileList(LPCTSTR lpszPathName);
	void TCGetRecentFileList(CStringArray &recentList);
	BOOL OpenRecentProject(LPCTSTR lpszPathName);
    //9-14添加最近打开项目
    afx_msg void OnAppAbout();

    //定义TC自己的项目新建
    afx_msg void OnFileNew();
    //定义TC自己的项目打开
    afx_msg void OnFileOpen();
    //关联文件
	void ProjFileRelated();
	int TCprojFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath);
	int TFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath);
	int TLLFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath);
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CTCApp theApp;
