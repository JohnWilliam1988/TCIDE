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
// 构造
public:
	CFileView();
	void AdjustLayout();
	void OnChangeVisualStyle();

// 特性
protected:

	CxSplitterWnd m_wndSplitter;				//左侧分割项目管理树跟文件内容明细树对象

	//CViewTree m_wndProjView;                   //项目管理树
    //CViewTree m_wndFileView;                   //文件内容明细树
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

    HTREEITEM m_hCurTreeItem;

    MySourceInfor m_FunTreeInfor;
    CString fFunTreePath;
    CString fFunTreeName;

    bool m_isNewFolder;                         //标识是否是新建目录

protected:
    bool DeleteFolder(CString lpPath);
// 实现
public:
	virtual ~CFileView();
    //加载项目
    void ReloadProj();
	//最后的参数用来控制是重命名文件节点，还是资源节点
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

