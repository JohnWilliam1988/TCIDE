#pragma once//用于项目管理中文件明细树分割视图
#include "ViewTree.h"


// CFileTreeView 视图

class CFileTreeView : public CTreeView
{
	friend class CProjTreeView;
	DECLARE_DYNCREATE(CFileTreeView)

protected:
	CFileTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CFileTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
protected:
	CImageList m_FileViewImages;
	HTREEITEM m_hCurTreeItem;
public:
	CViewTree m_wndFileView;   //文件内容明细树
	void OnChangeVisualStyle();
	void AdjustLayout();
	void FillFileView();
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkFileView(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


