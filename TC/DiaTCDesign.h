#pragma once
//#include "Command.h"
#include "ControlRect.h"
#include "ControlTracker.h"
#include "TCRunTimeManager.h"
#include "TCCtlHeads.h"
#include "RedoUndo\undo.h"
using namespace UNDO;

// CDiaTCDesign 对话框

struct TCDLGCONFIG
{
    COLORREF    m_BkgrColor;
    CString     m_BkgrBitmap;
    CDialogEx::BackgroundLocation m_bmpLocation;
    CString     m_icon;
    CString     m_eDiaInit;
    CString     m_eDestory;
	CString		m_eClose;
	CString		m_ePreTrans;
	CString     m_eWinProc;
	bool		m_minBtn;
	bool		m_maxBtn;
	CString		m_dlgID;	//窗口ID
    TCDLGCONFIG()
    {
        m_BkgrColor=::GetSysColor(COLOR_BTNFACE);
        m_eDiaInit=_T("");
        m_eDestory=_T("");
		m_eClose=_T("");
		m_ePreTrans=_T("");
		m_eWinProc=_T("");
        m_BkgrBitmap=_T("");
        m_bmpLocation=CDialogEx::BACKGR_TOPLEFT;
        m_icon=_T("");
		m_minBtn=true;
		m_maxBtn=false;
		m_dlgID=_T("");
    }
    void Reset()
    {
		m_BkgrColor=::GetSysColor(COLOR_BTNFACE);//NULL;
        m_bmpLocation=CDialogEx::BACKGR_TOPLEFT;
        m_eDiaInit=_T("");
        m_eDestory=_T("");
		m_eClose=_T("");
		m_ePreTrans=_T("");
		m_eWinProc=_T("");
        m_BkgrBitmap=_T("");
        m_icon=_T("");
		m_minBtn=true;
		m_maxBtn=false;
		m_dlgID=_T("");
    }
};

class CDiaTCDesign : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaTCDesign)

public:
	CDiaTCDesign(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaTCDesign();

// 对话框数据
	enum { IDD = IDD_DIATCDESIGN };

public:
	//保存界面跟控件信息到窗口管理类中
	void SaveTWinRCFileToMem();
	//从窗口管理类中加载信息到窗口中
	void LoadTWinRCFileToMem(CString csWin,CString csCtrls);

    void TCSetBkgrColor(COLORREF color);
    void TCSetBackgroundImage(CString bmp,BackgroundLocation location = BACKGR_TOPLEFT);
	void TCSetIcon(CString ico);

    //void TCDeSelectAll();
    CControlTracker m_ControlTracker;
    TCDLGCONFIG m_TcDlgConfig;
	CommandManager m_undoManager;

private:
    CImage     m_ImageLoader;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    TCTOOLTYPE m_SelectToolType;

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);


/////////////////////////??Redo/Undo??///////////////////////////
public:

	/////////////////////////////RedoUndo//////////////////////////////////
	//添加控件命令
	void AddCtrl(CRect rc,TCTOOLTYPE type,TCPARENTTYPE parentType);
	//拖动大小命令
	void OnSizeCtrl(CWnd* pCtrl,CRect rc,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType);
	//????
	void ClearCtrl(CWnd* pCtrl,TCTOOLTYPE ctrlType,TCPARENTTYPE parentWndType);

	//?????????
	void DeSelectAll();
	void SetAllDefButtonFalse();

	//??Tab??
	void AddTabCtrl(CRect rc);
	//??Tab??
	void ClearTabCtrl(CTCTab *pCtrl,CRect rc);
	/////////////////////////////RedoUndo??//////////////////////////////////
	
		
	//////////////////////////??????????????////////////////////////////
	void SetTCButton(CTCButton *pButton,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCStatic(CTCStatic *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCEdit(CTCEdit *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCRadio(CTCRadio *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCCheck(CTCCheck *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCCombox(CTCComboBox *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCGroupBox(CTCGroupBox *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCListBox(CTCListBox *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCProgress(CTCProgress *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCPath(CTCBrowse *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCPicture(CTCPicture *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCSlider(CTCSlider *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCTimer(CTCTimer *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCHotKey(CTCHotKeyCtrl *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCDate(CTCDate *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCExplorer(CTCExplorer *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCTab(CTCTab *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	void SetTCGridCtrl(CGridCtrl *pCtrl,CRect rc,CString ctrlid = _T(""),bool isCopy = false);
	//////////////////////////设置控件属性相关////////////////////////////

	////////////////设置对话框的最大化，最小化按钮方法/////////////////////////
	void TCSetMinButton(bool set);
	void TCSetMaxButton(bool set);
	////////////////设置对话框的最大化，最小化按钮方法/////////////////////////


	/////////////////////////RedoUndo??////////////////////////
	void Undo();
	void Redo();
	BOOL CanUndo();
	BOOL CanRedo();
	/////////////////////////RedoUndo??////////////////////////

};
