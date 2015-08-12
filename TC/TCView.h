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

// TCView.h : CTCView ��Ľӿ�
//

#pragma once

#include "ScintillaWnd.h"
//#include "MyIntellisenseList.h"
#include "TCDoc.h"
#include <vector>
#include "ListFarther.h"
#include "../ToolTip/XSuperTooltip.h"

class CTCView : public CView
{
protected: // �������л�����
	CTCView();
	DECLARE_DYNCREATE(CTCView)

// ����
public:
	CTCDoc* GetDocument() const;

// ����
public:
	//bool m_isFirst;

    //��ʼ���༭����
     void InitialiseEditor(void);     

     //�����к�
     void UpdateLineNumberWidth();

     void TCSetFocus();

     //��ת��ָ��λ��
     void TCGotoPos(int pos);

     //��ת��ָ����
     void TCGotoLine(int line);

     //�õ�ָ���е�λ��
     int TCLineIndex(int line);

	 //ָ��λ�ò�������
	 void GetUserEidtText(CString &rstr);
     
     //ָ��λ�ò�������
     void TCInserText(int startPos,int endPos,const TCHAR* text);

     //�õ��༭���ݵĳ���
     int TCGetLength();

	 void TCAddComment();

	 void TCActiveEditView();

	 //��״̬������ʾ�к���
	 void StatubarsShowRowAndCul();

     //�ڱ༭�����в�������
     
     //������Ϣ
     LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) {
         return ::SendMessage(m_ScintillaWnd, Msg, wParam, lParam);
     }

     //���Ƶ�ǰ�ϵ����ڵ�λ��
     void TCDrawCurrentBP(int line);

     bool TCGetFunList(CString csNameSPDot,CString &csList);

     bool TCGetFunParam(CString csNameSPDot,CString csFun,CString &csList);

	 int TCGetCurrentPos();

	 void TCDeleteDebugBP();

	 void TCFindText(CString text,int flag,int start,int end);

	 void TCContentText(int beginPos,int endPos);

	 void TCReplaceSelText(CString old,CString toReplace,int flag,bool fromBegin = false);

	 void showIntellisenseWindow();

	 void hideIntellisenseWindow(); 

	 void ShowIntelisenseFunExample();
	 void ShowIntelisenseFunExampleWithoutPos(CString nspace);

	 //�۵�/�� ������
	 void DisplayLine(int line);
	 bool m_isFold;
	 bool isFuntionStatement(CString statement);
	 void FoldToDefine(bool isFold);//0�۵�,1��
	 //�۵�/�� ������

	 void TCToggleFoldSave();

	 void TCToggleFoldLoad();

    bool LfetIfNote(int nowpos);
 
	bool isGoToStr(TCHAR* soucestr);
	 //���Ÿ��� ���°� 2012.12.22
	 void braceMatchHighlight(int Dvalue=0);
	 void TCTypesetting();

	  //�Ƿ�ע�ͻ��ַ����� ���°� 2013.2.7
	 bool IsNoteOrStr(TCHAR *str,int pos);

     //void CheckMenus(void);
     //void EnableAMenuItem(int id, bool enable);

	 void NavigationPos();
	 //��������ƥ������ ���°�
	 bool FindAllContent(TCHAR *rFindStr,int type,int flags = SCFIND_MATCHCASE | SCFIND_WHOLEWORD);//����1:���ض�̬��ȡ���û�Ҫ���ҵĴ�
	 //��������ƥ�����ݶ��ļ��� ���°� 2013.1.25
	 bool FindAllFileContent(std::vector<wstring> inFilePath,TCHAR *inFindStr); //����1:�ļ�·��,����2:Ҫ���ҵ��ִ�

	 //��������
	 void SetEditorInfo(EditorSetInfo editSetInfo);
	 //��ȡ�༭�������Ϣ
	 void GetEditInfo(EditorSetInfo &editSetInfo);

	 void AddToolTip(CString tip);

	 //void AddToolTipDebugVaule(CString tip);

	 //���ı���滻Ӣ�ı�� ���°� 2013.1.18
	 //bool isSign(TCHAR ch,int pos);
	 int replaceSign(TCHAR ch);
public:
	//����������س�Ա
	bool notFirst;
	int m_rowCount;

   //����������س�Ա

    //�༭�������
     CScintillaWnd m_ScintillaWnd;

	 CXSuperTooltip m_ToolTip;
	 //������ʾ���ڶ��� 
	 //CMyIntellisenseList m_listView;
	 //�б�����ĶԻ���
	 ListFarther m_listFaterView;
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CTCView();
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
    afx_msg void OnUpdateTcNewDelBp(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcDeleteallbp(CCmdUI *pCmdUI);
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
    afx_msg void OnEditUndo();
    afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
    afx_msg void OnEditRedo();
    afx_msg void OnEditCut();
    afx_msg void OnEditCopy();
    afx_msg void OnEditPaste();
    afx_msg void OnEditDelete();
    afx_msg void OnEditSelectAll();
	afx_msg void OnTcNewDelBp();
	afx_msg void OnTcDeleteallbp();
	afx_msg void OnEditAddcomment();
	afx_msg void OnUpdateEditAddcomment(CCmdUI *pCmdUI);
	afx_msg void OnEditRemovecomment();
	afx_msg void OnUpdateEditRemovecomment(CCmdUI *pCmdUI);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnEditFind();
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcTypesetting();
	afx_msg void OnEditGoback();
	afx_msg void OnUpdateEditGoback(CCmdUI *pCmdUI);
	afx_msg void OnEditGoforward();
	afx_msg void OnUpdateEditGoforward(CCmdUI *pCmdUI);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnUpdateTcTypesetting(CCmdUI *pCmdUI);
	afx_msg void OnEditTogglefoldall();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // TCView.cpp �еĵ��԰汾
inline CTCDoc* CTCView::GetDocument() const
   { return reinterpret_cast<CTCDoc*>(m_pDocument); }
#endif

