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

#pragma once

#include "ViewTree.h"

#include "matchLib.h"

#include "CxSplitterWnd.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// ����
public:
	CFileView();
	void AdjustLayout();
	void OnChangeVisualStyle();

// ����
protected:

	CxSplitterWnd m_wndSplitter;				//���ָ���Ŀ���������ļ�������ϸ������

	//CViewTree m_wndProjView;                   //��Ŀ������
    //CViewTree m_wndFileView;                   //�ļ�������ϸ��
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

    HTREEITEM m_hCurTreeItem;

    MySourceInfor m_FunTreeInfor;
    CString fFunTreePath;
    CString fFunTreeName;

    bool m_isNewFolder;                         //��ʶ�Ƿ����½�Ŀ¼

protected:
    bool DeleteFolder(CString lpPath);
// ʵ��
public:
	virtual ~CFileView();
    //������Ŀ
    void ReloadProj();
	//���Ĳ��������������������ļ��ڵ㣬������Դ�ڵ�
    bool FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
    //afx_msg void OnTcCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnNMDblclkFileView(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
 };

