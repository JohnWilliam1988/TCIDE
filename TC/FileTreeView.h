#pragma once//������Ŀ�������ļ���ϸ���ָ���ͼ
#include "ViewTree.h"


// CFileTreeView ��ͼ

class CFileTreeView : public CTreeView
{
	friend class CProjTreeView;
	DECLARE_DYNCREATE(CFileTreeView)

protected:
	CFileTreeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	CViewTree m_wndFileView;   //�ļ�������ϸ��
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


