// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// TC.h : TC Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

//#include "TCCtlHeads.h"

#include "resource.h"       // ������

#include "RedoUndo\undo.h"

#include "TCRunTimeManager.h"

//#include "TCTab.h"

// CTCApp:
// �йش����ʵ�֣������ TC.cpp
//

class CTCApp : public CWinAppEx
{
public:
	CTCApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

//�����ĵ�ģ��
    CMultiDocTemplate* m_pDocTemplateTc;
    CMultiDocTemplate* m_pDocTemplateTWin;
	CMultiDocTemplate* m_pStartDocTemplate;
	CMultiDocTemplate* m_pVipDocTemplate;

//����
    void TCCloseAllView();
	//����ڿ�ʼҳ������ѡ���˲��ر�����ҳ���ʱ�����
	void TCCloseWithoutStartView();			
    //tԴ�ļ��Ĵ�,ɾ��,����������
    void TCOpenTFile(CString file);

    CView *TCOpenTFileReturnView(CString file);
	//���ݴ���ID������ǰ���࣬������
	CView *TCGetTCDesignView(CString winName);

    void TCDeleteTFileView(CString file);
	void TCDeleteDesignView(CString winID);
    void TCRenameTFile(CString oldPath,CString newPath);
	void TCRenameTWin(CString oldStr,CString newStr);
    //twin�����ļ��Ĵ�,ɾ��,����������

    void TCOpenTWin(CString winID=CTCRunTimeManager::g_ProjName);
	void TCOpenStartView();
    void TCDeleteTWinView();

	void TCOpenVipView();

	//12-11-13,�ܿ���,�����Ƿ����TCView����
	CView *TCGetTCView();

public:



// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	//9-14����������Ŀ
	afx_msg BOOL OnRecentFile(UINT uID) ;
	void AddToRecentFileList(LPCTSTR lpszPathName);
	CString GetRecentFileListDisplayName(int index);
	void RemoveDirtyProFromRecentFileList(LPCTSTR lpszPathName);
	void TCGetRecentFileList(CStringArray &recentList);
	BOOL OpenRecentProject(LPCTSTR lpszPathName);
    //9-14����������Ŀ
    afx_msg void OnAppAbout();

    //����TC�Լ�����Ŀ�½�
    afx_msg void OnFileNew();
    //����TC�Լ�����Ŀ��
    afx_msg void OnFileOpen();
    //�����ļ�
	void ProjFileRelated();
	int TCprojFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath);
	int TFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath);
	int TLLFileRelated(CString FileNameExt, CString ExtNote, CString IconPath, CString AppPath);
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CTCApp theApp;
