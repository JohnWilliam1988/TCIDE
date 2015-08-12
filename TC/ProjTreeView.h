#pragma once
//������Ŀ��������Ŀ�������ָ���ͼ
#include "ViewTree.h"
#include "matchLib.h"
// CProjTreeView ��ͼ

class CProjTreeView : public CTreeView
{
	friend class CFileTreeView;
	DECLARE_DYNCREATE(CProjTreeView)

protected:
	CProjTreeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

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
	CViewTree m_wndProjView;                  //��Ŀ������
	MySourceInfor m_FunTreeInfor;
	CString fFunTreePath;
	CString fFunTreeName;
	bool	 m_isNewFolder;                   //��ʶ�Ƿ����½�Ŀ¼

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
	////////////////�ļ��нڵ�˵�����//////////////////
	afx_msg void OnProjFolderrename();
	afx_msg void OnProjDelfolder();
	////////////////��Ŀ����ڵ�˵�����////////////////
	afx_msg void OnTcBuild();
	afx_msg void OnTcCompile();
	afx_msg void OnTcClear();
	afx_msg void OnTcDebug();
	afx_msg void OnProjNewfile();
	afx_msg void OnProjExistfile();
	afx_msg void OnProjNewfolder();
	////////////////�ļ��ڵ�˵����ܺ���////////////////
	afx_msg void OnProjImport();
	afx_msg void OnProjOpenfile();
	afx_msg void OnProjDeletefile();
	afx_msg void OnProjFilerename();
	////////////////��Դ�ڵ�˵����ܺ���////////////////
	afx_msg void OnProjInsertrc();
	afx_msg void OnProjOpenrcfolder();
	afx_msg void OnProjRcreload();
	afx_msg void OnProjNewdialog();
	///////////////��Դ�ļ��ڵ�˵����ܺ���/////////////
	afx_msg void OnProjRcfilerename();
	afx_msg void OnProjDeletercfile();

public:
	void FillFileView();
	void AdjustLayout();
	void SetImageList(CImageList imgageList);
	//�����Ǽ��ز���ʾ��Ŀ��������� �ļ��������ռ�ͺ���
	void ReloadSPandFunList();
	bool DeleteFolder(CString lpPath);
	//����Դ�ļ�Ŀ¼,���ص����οؼ�
	void filterSourcePath(CString lpPath,HTREEITEM item);
	//������ԴĿ¼,���ص����οؼ�
	void filterRCPath(CString lpPath,HTREEITEM item);
	//�����ļ���,���ص����οؼ�
	void filter(CString lpPath,HTREEITEM item);
	BOOL SortTreeList(HTREEITEM treeItem,CTreeCtrl *pmyTreeCtrl);
	static int CALLBACK SortTreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//��������ǵõ�һ�����οؼ��ڵ���������ļ���·��,�����·��.
	void GetItemRelativePatchFromFileClassTree(HTREEITEM item,CString &path);
	//��������ǵõ�һ�����οؼ��ڵ��������ļ���·��,�Ǿ���·��.
	void GetItemPatchFromFileClassTree(HTREEITEM item,CString &path);
	//������Ŀ
	void ReloadProj();
	//���Ĳ��������������������ļ��ڵ㣬������Դ�ڵ�
	bool FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd);
	//���Ĳ��������������������ļ��ڵ㣬������Դ�ڵ�
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


