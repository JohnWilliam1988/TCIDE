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

// TCDesignView.h : CTCDesignView 类的接口
//

#pragma once

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"

class CTCDesignView : public CView
{
protected: // 仅从序列化创建
	CTCDesignView();
	DECLARE_DYNCREATE(CTCDesignView)

// 特性
public:
	//删除选中的控件
	void DeleteCtrl();

	//复制控件
	void CopyCtrl();
	///////////////////遍历TabCtrl得到选中的个数是>1 还是>2////////
	BOOL TabCtrlSelectMoreOne();
	BOOL TabCtrlSelectMoreTwo();
	BOOL TabCtrlSelectMoreThree();

	BOOL TabCtrlCopyMoreOne();

	void RemoveCopyRetangles();

	CTCDesignDoc* GetDocument() const;
    CString m_TWinID;
	////////////////删除Tab时使TabPage的子控件触发Clear命令，使可撤销
	void DeleteCtrlsInTab(CTCTabPage *pTabPage,CWnd *pCtrl,TCTOOLTYPE ctrlType);
// 操作
public:
    CDiaTCDesign m_DiaDesign; 

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTCDesignView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnTcAlignleft();
    afx_msg void OnTcCenteralignh();
    afx_msg void OnTcAlignright();
    afx_msg void OnTcAligntop();
    afx_msg void OnTcCenteralignv();
    afx_msg void OnTcAlignbottom();
    afx_msg void OnTcSameheight();
    afx_msg void OnTcSamewidth();
    afx_msg void OnTcSamesize();
    afx_msg void OnTcUniformspaceh();
    afx_msg void OnTcUniformspacev();
    afx_msg void OnTcCenterwinv();
    afx_msg void OnTcCenterwinh();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnUpdateTcAlignleft(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcCenteralignh(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcAlignright(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcAligntop(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcCenteralignv(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcAlignbottom(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcSameheight(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcSamewidth(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcSamesize(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcUniformspaceh(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcUniformspacev(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcCenterwinv(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcCenterwinh(CCmdUI *pCmdUI);
    afx_msg void OnDisableUpdate(CCmdUI* pCmdUI);
    afx_msg void OnFileSave();
	afx_msg void OnUpdateEditDelete(CCmdUI *pCmdUI);
	afx_msg void OnEditDelete();
	
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnCtlperties();
	afx_msg void OnDestroy();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI *pCmdUI);
	afx_msg void OnEditCtljumptofile();
	afx_msg void OnEditLock();
	afx_msg void OnUpdateEditLock(CCmdUI *pCmdUI);
	afx_msg void OnEditUnlock();
	afx_msg void OnUpdateEditUnlock(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // TCDesignView.cpp 中的调试版本
inline CTCDesignDoc* CTCDesignView::GetDocument() const
   { return reinterpret_cast<CTCDesignDoc*>(m_pDocument); }
#endif

