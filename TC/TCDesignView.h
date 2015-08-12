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

// TCDesignView.h : CTCDesignView ��Ľӿ�
//

#pragma once

#include "DiaTCDesign.h"
#include "TCDesignDoc.h"

class CTCDesignView : public CView
{
protected: // �������л�����
	CTCDesignView();
	DECLARE_DYNCREATE(CTCDesignView)

// ����
public:
	//ɾ��ѡ�еĿؼ�
	void DeleteCtrl();

	//���ƿؼ�
	void CopyCtrl();
	///////////////////����TabCtrl�õ�ѡ�еĸ�����>1 ����>2////////
	BOOL TabCtrlSelectMoreOne();
	BOOL TabCtrlSelectMoreTwo();
	BOOL TabCtrlSelectMoreThree();

	BOOL TabCtrlCopyMoreOne();

	void RemoveCopyRetangles();

	CTCDesignDoc* GetDocument() const;
    CString m_TWinID;
	////////////////ɾ��TabʱʹTabPage���ӿؼ�����Clear���ʹ�ɳ���
	void DeleteCtrlsInTab(CTCTabPage *pTabPage,CWnd *pCtrl,TCTOOLTYPE ctrlType);
// ����
public:
    CDiaTCDesign m_DiaDesign; 

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CTCDesignView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // TCDesignView.cpp �еĵ��԰汾
inline CTCDesignDoc* CTCDesignView::GetDocument() const
   { return reinterpret_cast<CTCDesignDoc*>(m_pDocument); }
#endif

