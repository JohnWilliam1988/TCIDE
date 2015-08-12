#pragma once
//用于项目管理中项目管理树分割视图
#include "ViewTree.h"
#include "matchLib.h"
// CProjTreeView 视图

class CProjTreeView : public CTreeView
{
	friend class CFileTreeView;
	DECLARE_DYNCREATE(CProjTreeView)

protected:
	CProjTreeView();           // 动态创建所使用的受保护的构造函数

	void OnChangeVisualStyle();
	virtual ~CProjTreeView();

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
	CViewTree m_wndProjView;                  //项目管理树
	MySourceInfor m_FunTreeInfor;
	CString fFunTreePath;
	CString fFunTreeName;
	bool	 m_isNewFolder;                   //标识是否是新建目录

public:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//afx_msg void OnPaint();
	afx_msg void OnClickedProjView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkProjView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginlabeleditProjView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditProjView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	////////////////文件夹节点菜单功能//////////////////
	afx_msg void OnProjFolderrename();
	afx_msg void OnProjDelfolder();
	////////////////项目管理节点菜单功能////////////////
	afx_msg void OnTcBuild();
	afx_msg void OnTcCompile();
	afx_msg void OnTcClear();
	afx_msg void OnTcDebug();
	afx_msg void OnProjNewfile();
	afx_msg void OnProjExistfile();
	afx_msg void OnProjNewfolder();
	////////////////文件节点菜单功能函数////////////////
	afx_msg void OnProjImport();
	afx_msg void OnProjOpenfile();
	afx_msg void OnProjDeletefile();
	afx_msg void OnProjFilerename();
	////////////////资源节点菜单功能函数////////////////
	afx_msg void OnProjInsertrc();
	afx_msg void OnProjOpenrcfolder();
	afx_msg void OnProjRcreload();
	afx_msg void OnProjNewdialog();
	///////////////资源文件节点菜单功能函数/////////////
	afx_msg void OnProjRcfilerename();
	afx_msg void OnProjDeletercfile();

public:
	void FillFileView();
	void AdjustLayout();
	void SetImageList(CImageList imgageList);
	//这里是加载并显示项目管理下面的 文件的命名空间和函数
	void ReloadSPandFunList();
	bool DeleteFolder(CString lpPath);
	//过滤源文件目录,加载到树形控件
	void filterSourcePath(CString lpPath,HTREEITEM item);
	//过滤资源目录,加载到树形控件
	void filterRCPath(CString lpPath,HTREEITEM item);
	//过滤文件夹,加载到树形控件
	void filter(CString lpPath,HTREEITEM item);
	BOOL SortTreeList(HTREEITEM treeItem,CTreeCtrl *pmyTreeCtrl);
	static int CALLBACK SortTreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//这个方法是得到一个树形控件节点所代码的文件的路径,是相对路径.
	void GetItemRelativePatchFromFileClassTree(HTREEITEM item,CString &path);
	//这个方法是得到一个树形控件节点所代表文件的路径,是绝对路径.
	void GetItemPatchFromFileClassTree(HTREEITEM item,CString &path);
	//加载项目
	void ReloadProj();
	//最后的参数用来控制是重命名文件节点，还是资源节点
	bool FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd);
	//最后的参数用来控制是重命名文件节点，还是资源节点
	bool TravelFileTree(HTREEITEM hItem,CString oldPatn,CString newName,BOOL FileOrWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnProperties();
	afx_msg void OnUpdateTcBuild(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTcCompile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTcClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTcDebugrun(CCmdUI *pCmdUI);
};


