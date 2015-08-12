#pragma once
//#include "../resource.h"
#include "..\TC.h"    
#include "..\ControlRect.h"
#include "..\ControlTracker.h"
#include "..\TCRunTimeManager.h"
#include "..\RedoUndo\undo.h"

#include "..\TCCtlHeads.h"
#include "TCExplorer.h"
#include "TCHotKeyCtrl.h"
#include "GridCtrl_src/GridCtrl.h"

using namespace UNDO;

// CTCTabPage 

struct TCTABDLGCONFIG
{
	COLORREF    m_BkgrColor;
	CString     m_BkgrBitmap;
	CDialogEx::BackgroundLocation m_bmpLocation;
	TCTABDLGCONFIG()
	{
		m_BkgrColor=::GetSysColor(COLOR_BTNFACE);
		m_BkgrBitmap=_T("");
		m_bmpLocation=CDialogEx::BACKGR_TOPLEFT;
	}
	void Reset()
	{
		m_BkgrColor=::GetSysColor(COLOR_BTNFACE);//NULL;
		m_bmpLocation=CDialogEx::BACKGR_TOPLEFT;
		m_BkgrBitmap=_T("");
	}
};

class CTCTabPage : public CDialogEx
{
	DECLARE_DYNAMIC(CTCTabPage)

public:
	CTCTabPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTCTabPage();

//功能
public:
    CControlTracker m_ControlTracker;

private:
	TCTABDLGCONFIG m_TcDlgConfig;
	CImage  m_ImageLoader;

// 对话框数据
	enum { IDD = IDD_TCTABPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持



public:
	void TCSetBkgrColor(COLORREF color);
	COLORREF TCGetBkgrColor();
	void TCSetBackgroundImage(CString bmp,BackgroundLocation location = BACKGR_TOPLEFT);
	CString TCGetBackgroundImage();


	////////////////////////////////????///////////////////////////////////
	//??Button??
	void AddCtrl(CRect rc,TCTOOLTYPE type,TCPARENTTYPE parentType);
	//??????
	void OnSizeCtrl(CWnd* pCtrl,CRect rc,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType);
	//????
	void ClearCtrl(CWnd* pCtrl,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType);
	////////////////////////////////????///////////////////////////////////

	///////////////////???????TabPage????///////////////////////////
	void Redo();
	void Undo();

	//?????,???????
	void DeSelectAll();


	//////////////////////////??????????????////////////////////////////
	void SetTCButton(CTCButton *pButton,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCStatic(CTCStatic *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCEdit(CTCEdit *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCRadio(CTCRadio *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCCheck(CTCCheck *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCCombox(CTCComboBox *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCGroupBox(CTCGroupBox *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCListBox(CTCListBox *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCProgress(CTCProgress *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCPath(CTCBrowse *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCPicture(CTCPicture *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCSlider(CTCSlider *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCHotKey(CTCHotKeyCtrl *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCDate(CTCDate *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCExplorer(CTCExplorer *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	void SetTCGridCtrl(CGridCtrl *pCtrl,CRect rc,BOOL show = TRUE,CString ctrlid = _T(""),bool isPaste = false);
	//////////////////////////设置创建控件相关信息////////////////////////////
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
