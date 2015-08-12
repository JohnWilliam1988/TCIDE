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

// TCView.h : CTCView 类的接口
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
protected: // 仅从序列化创建
	CTCView();
	DECLARE_DYNCREATE(CTCView)

// 特性
public:
	CTCDoc* GetDocument() const;

// 操作
public:
	//bool m_isFirst;

    //初始化编辑对象
     void InitialiseEditor(void);     

     //更行行号
     void UpdateLineNumberWidth();

     void TCSetFocus();

     //跳转到指定位置
     void TCGotoPos(int pos);

     //跳转到指定行
     void TCGotoLine(int line);

     //得到指定行的位置
     int TCLineIndex(int line);

	 //指定位置插入内容
	 void GetUserEidtText(CString &rstr);
     
     //指定位置插入内容
     void TCInserText(int startPos,int endPos,const TCHAR* text);

     //得到编辑内容的长度
     int TCGetLength();

	 void TCAddComment();

	 void TCActiveEditView();

	 //在状态栏上显示行和列
	 void StatubarsShowRowAndCul();

     //在编辑界面中插入内容
     
     //发送消息
     LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) {
         return ::SendMessage(m_ScintillaWnd, Msg, wParam, lParam);
     }

     //绘制当前断点所在的位置
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

	 //折叠/打开 到定义
	 void DisplayLine(int line);
	 bool m_isFold;
	 bool isFuntionStatement(CString statement);
	 void FoldToDefine(bool isFold);//0折叠,1打开
	 //折叠/打开 到定义

	 void TCToggleFoldSave();

	 void TCToggleFoldLoad();

    bool LfetIfNote(int nowpos);
 
	bool isGoToStr(TCHAR* soucestr);
	 //括号高亮 王崧百 2012.12.22
	 void braceMatchHighlight(int Dvalue=0);
	 void TCTypesetting();

	  //是否注释或字符串中 王崧百 2013.2.7
	 bool IsNoteOrStr(TCHAR *str,int pos);

     //void CheckMenus(void);
     //void EnableAMenuItem(int id, bool enable);

	 void NavigationPos();
	 //查找所有匹配内容 王崧百
	 bool FindAllContent(TCHAR *rFindStr,int type,int flags = SCFIND_MATCHCASE | SCFIND_WHOLEWORD);//参数1:返回动态获取的用户要查找的串
	 //查找所有匹配内容多文件版 王崧百 2013.1.25
	 bool FindAllFileContent(std::vector<wstring> inFilePath,TCHAR *inFindStr); //参数1:文件路径,参数2:要查找的字串

	 //设置字体
	 void SetEditorInfo(EditorSetInfo editSetInfo);
	 //获取编辑器相关信息
	 void GetEditInfo(EditorSetInfo &editSetInfo);

	 void AddToolTip(CString tip);

	 //void AddToolTipDebugVaule(CString tip);

	 //中文标点替换英文标点 王崧百 2013.1.18
	 //bool isSign(TCHAR ch,int pos);
	 int replaceSign(TCHAR ch);
public:
	//导航功能相关成员
	bool notFirst;
	int m_rowCount;

   //导航功能相关成员

    //编辑界面对象
     CScintillaWnd m_ScintillaWnd;

	 CXSuperTooltip m_ToolTip;
	 //智能提示窗口对象 
	 //CMyIntellisenseList m_listView;
	 //列表上面的对话框
	 ListFarther m_listFaterView;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CTCView();
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

#ifndef _DEBUG  // TCView.cpp 中的调试版本
inline CTCDoc* CTCView::GetDocument() const
   { return reinterpret_cast<CTCDoc*>(m_pDocument); }
#endif

