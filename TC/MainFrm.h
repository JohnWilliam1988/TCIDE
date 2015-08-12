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

// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "TCFunDecWnd.h"

////////自定义////////
#include "DesignToolBox.h"
#include "matchLib.h"
#include "TCCtlHeads.h"
#include "DiaTViewFind.h"
#include "XInfoTip.h"
#include "XInfoTipDebug.h"
#include "TCSettingDia.h"
#include "TCEditorLineJumper.h"
////////自定义////////

#define WM_MYTCDEBUG_MESSAGE         (WM_USER+100)
#define WM_MYTCOUTMSG_MESSAGE		 (WM_USER+101)

#define WM_MYCLOSESTARTVIEW_MESSAGE  (WM_USER+102)
#define WM_MYDESKTOPALERT_MESSAGE	 (WM_USER+103)

//#define PROTOCOL_CLIENT
//
//#include "protocol.h"

//查找所有匹配内容结构体 王崧百
struct FindInfo
{
	CString path;                 //所属文件
	CString lindStr;              //行内容串
	int lineNumber;               //行号
	int strBeginPos;              //查找到的串开始位置
	int strEndPos;                //查找到的串结束位置

	FindInfo()
	{
		path = L"";
		lindStr = L"";
		lineNumber = -1;
		strBeginPos = -1;
		strEndPos = -1;
	}
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 操作
public:
    //加载项目
    void ProjFileViewReload();    
    //显示项目的属性,upDate用来控制是否及时更新
    void SetProjProperty(CString name,CString path,BOOL upDate = FALSE);
	//设置窗口的节点信息
	void SetRcNodeDiaProperty(CString name);
    //显示面板设计的属性,upDate用来控制是否及时更新
    void SetTWinDiaProperty(CString name,const CDiaTCDesign * const dlg,BOOL upDate = FALSE);
    //显示static控件的属性
    void SetTWinStaticProperty(CString id,CTCStatic *  ctl,BOOL upDate = FALSE);
    //显示group控件的属性
    void SetTWinGroupProperty(CString id,CTCGroupBox *  ctl,BOOL upDate = FALSE);
    //显示button控件的属性
    void SetTWinButtonProperty(CString id,CTCButton *  ctl,BOOL upDate = FALSE);
    //显示edit控件的属性
	void SetTWinEditProperty(CString id,CTCEdit *  ctl,BOOL upDate = FALSE);
    //显示Check控件的属性
    void SetTWinCheckProperty(CString id, CTCCheck *  ctl,BOOL upDate = FALSE);
    //显示Radio控件的属性
    void SetTWinRadioProperty(CString id, CTCRadio *  ctl,BOOL upDate = FALSE);
    //显示Combox控件的属性
    void SetTWinComboxProperty(CString id, CTCComboBox *  ctl,BOOL upDate = FALSE);
    //显示Picture控件的属性
    void SetTWinPictureProperty(CString id, CTCPicture *  ctl,BOOL upDate = FALSE);
    //显示progress控件的属性
    void SetTWinProgressProperty(CString id, CTCProgress *  ctl,BOOL upDate = FALSE);
    //显示浏览框的属性
    void SetTWinPathProperty(CString id, CTCBrowse *  ctl,BOOL upDate = FALSE);
    //显示Slider的属性
    void SetTWinSliderProperty(CString id, CTCSlider *  ctl,BOOL upDate = FALSE);
    //显示Date的属性
    void SetTWinDateProperty(CString id, CTCDate *  ctl,BOOL upDate = FALSE);
    //显示List的属性
    void SetTWinListProperty(CString id, CTCListBox *  ctl,BOOL upDate = FALSE);
    //显示Tab的属性
	void SetTWinTableProperty(CString id, CTCTab*  ctl,BOOL upDate = FALSE);
	//显示Timer的属性
	void SetTWinTimerProperty(CString id, CTCTimer*  ctl,BOOL upDate = FALSE);
	//显示HotKey的属性
	void SetTWinHotKeyProperty(CString id, CTCHotKeyCtrl *  ctl,BOOL upDate = FALSE);
	//显示Explorer的属性
	void SetTWinExplorerProperty(CString id, CTCExplorer *  ctl,BOOL upDate = FALSE);
		//显示GridCtrl的属性
	void SetTWinGridCtrlProperty(CString id, CGridCtrl *  ctl,BOOL upDate = FALSE);
    //当修改属性框时,修改项目管理框
	//最后的参数用来控制是重命名文件节点，还是资源节点
    bool FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd = TRUE);
	//修改特定项的属性避免界面重绘闪烁
	void SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue);
	//////////////////////////////添加控件名称到属性窗口下拉框//////////////////////////////
	void InsertCtrlToProCom(CString ctrlName);

	void TCUpdateProperty(TCTOOLTYPE type,CString ctrlID,CWnd *pCtrl,BOOL update);

	void TCActiveFindWnd();

	//功能描述函数
	void SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample);

    //编辑项目
    bool TCCompileProj();
    //生成项目
    bool TCBuildProj();
    //清理项目
    bool TCClearProj();
    //保存所有的文档
    void TCSaveAllDoc();
    //清理输出信息
    void TCClearOutMsg();
	//清理查找信息
	void TCClearAllContentMsg();
    //添加一个输出信息
    void TCOutMsgAddString(CString add);
    //添加一个断点
    bool TCDebugAddBP(CString path,int line);
    //删除一个断点
    bool TCDebugDeleteBP(CString path,int line);

	void TCFindAllContent();
	void TCAddAllContentString();

	//////////////////////9-4添加删除所有断点操作/////////////
	void TCDebugDeleteAllBP();

    //调试的时候加载可见的变量列表
    void TCDebugLoadVarList();  
    //调试
    void TCDebug();
    //调试执行方法
    void TCDebugRun();
    //打开现有项目
    bool TCOpenExistProject();

	//(11-2)打开项目成功后,备份当前项目
	bool TCBackUpProject();

    //TC打开时候,界面的布局
    void TCStartPageSetting();
    //得到当前工具箱面板的选择
    TCTOOLTYPE GetSelectToolType(); 
    //重置工具箱面板的控件的选择
    void ResetSelectToolType(); 
    //压缩附件的接口
    bool compress7z(CString compath);
    //添加事件代码
    bool TCAddEventCode(CString namesp,CString fun,TC_EVENT_TYPE type=TC_EVENT_MAX);
    //添加事件代码
    bool TCAddEventCode(CString csEvent,TC_EVENT_TYPE type=TC_EVENT_MAX);
	//在当前的源代码t文件页面插入代码
    bool TCInsertCode(CString csCode,int pos);
	//向项目启动文件添加文件头
	 bool TCInsertCodeToMainFile(CString csCode,int docPos);
	//激活功能描述窗口
	void ActiveDecWnd();

	void ActivePerprotyWnd();

	void ActiveOutPutWnd();

	void ResetPropertyCombox();
	/////////////////////////状态栏显示相关函数///////////////////////////////

	//设置当前光标所在行列信息到状态栏
	void TCSetStatusRowAndCol(int row,int col);

	//状态栏头部信息接口   参数1：显示的内容  参数2：鼠标提示的内容 
	void TCSetHeadPanelText(CString text,CString tiptext);

	//显示进度条
	void ShowProgress(BOOL bShow = TRUE);

	//设置进度值
	void TCSetProgressPos(int pos);

	//设置文件大小
	void TCSetFileInfo(const UINT size);

	//设置文件大小可见
	void TCSetFileInfoVisible(BOOL isViewable);
	/////////////////////////状态栏显示相关函数///////////////////////////////

	//TC自定义消息
	void TCSendOutWndMsg(WPARAM wParam,LPARAM lParam);

	//鼠标提示
    void GiveToolTip(CWnd *pWnd, LPCTSTR szTooltipText, HICON hIcon=NULL);

    //移除鼠标提示
    void RemoveToolTip(CWnd *pWnd) ;
    
    //属性

	bool TCIsDebugValue(const TCHAR *name,CString &csValue);
public:
	bool m_isVipBuild;                      //是否是VIP生成

	//const MyDebugValueList m_TempValueList; //   断点调试的时候,临时的变量指针
	CWinThread *m_LoginHeartThread;

    ////下面是创建调试用的内核对象和管道通讯对象////////
    HANDLE m_LibEventHandle;     //通知解析器的内核对象
    HANDLE m_WinEventHandle;     //通知界面的内核对象
    bool   m_isInBP;             //这个变量标识是否处在断点状态
	HANDLE m_hFileMap;
    CMyDebugInfor  *m_TCDebugInfor;                     //
    HANDLE m_TCDebugProcessHandle;                      //

    ////创建调试用的内核对象和管道通讯对象//////////////

    bool m_IsInCompile;				//标识是否编译
	bool m_IsDebuging;				//标识是否调试状态
    //CTCDlgManager m_TWinManager;  //这个是用来记录界面设计的管理对象

	CDiaTViewFind m_wndFind;

	CXInfoTip 		m_TipEx;                  //用于鼠标提示使用

	CXInfoTipDebug 		m_TipExDebug;             //用于鼠标提示变量调试使用

	//设计窗口时的管理类
	CTCDlgManager m_DlgManager;

	CString m_pCurrentWndID;					//指向当前打开的设计面板

	//多文件导航功能相关成员 王崧百 2013.1.21
	std::vector<int> m_markposL;
	std::vector<int> m_markposR;
	std::vector<int> m_markposLpos;
	std::vector<int> m_markposRpos;
	std::vector<int> m_markLineLenthR;
	std::vector<int> m_markLineLenthL;
	std::vector<wstring> m_markLpath;
	std::vector<wstring> m_markRpath;
	int m_markLorR;  //0:初始值,1:左导航,2:右导航
	//多文件导航功能相关成员 王崧百 2013.1.21
	//查找所有匹配内容成员 王崧百
	std::vector<FindInfo> m_findInfo;
	CString m_curFoundTFile;					//当前所找内容的T文件路径
	//查找所有匹配内容成员 王崧百
	//选项配置框
	//CTCSettingDia m_wndSettingDia;
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar					m_wndRibbonBar;
	CMFCRibbonApplicationButton		m_MainButton;
	CMFCToolBarImages				m_PanelImages;
	//CMFCToolBarImages				m_Icons;
	CMFCRibbonStatusBar				m_wndStatusBar;
	CFileView						m_wndFileView;
	CClassView						m_wndClassView;
	COutputWnd						m_wndOutput;
	CPropertiesWnd					m_wndProperties;
	public:
    CDesignToolBox					m_wndDesignToolBox;
	//CTCCaptionBar					m_wndCaptionBar;
	CTCFunDecWnd					m_wndFunDec;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	//afx_msg void OnViewCaptionBar();
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    /////////复制这里,一个用于调试的自定义的消息////
    afx_msg LRESULT OnTCMyDebugPTMsg(WPARAM, LPARAM); 
    /////////复制这里,一个用于调试的自定义的消息////
	/////////输出窗口调试的消息///////////////
	afx_msg LRESULT OnTCOutMsg(WPARAM, LPARAM); 
	/////////输出窗口调试的消息///////////////


	/////////这里是一个自定义的测试消息,用来测试调试代码的时候异常返回信息的/////////////////////
    afx_msg LRESULT OnTCMyDebugInforMsg(WPARAM, LPARAM); 
    /////////这里是一个自定义的测试消息/////////////////////

	/////////自定义关闭启动视图消息///////////////////
	afx_msg LRESULT OnTCMyCloseStartViewMsg(WPARAM, LPARAM);
	/////////自定义关闭启动视图消息///////////////////
	afx_msg LRESULT OnTCDeskTopAlertWnd(WPARAM, LPARAM);


	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	//BOOL CreateCaptionBar();
	BOOL CreateStatusBar();
public:
	TCCURRENTVIEW m_currentViewType;
    afx_msg void OnTcDebugpro();
    afx_msg void OnTcDebugstep();
    afx_msg void OnTcDebugstop();
    afx_msg void OnTcDebugrun();
    afx_msg void OnViewMore();
    afx_msg void OnUpdateViewMore(CCmdUI *pCmdUI);
    afx_msg void OnViewFileView();
    afx_msg void OnUpdateViewFileView(CCmdUI *pCmdUI);
    afx_msg void OnViewPropertiesWnd();
    afx_msg void OnViewToolboxWnd();
    afx_msg void OnUpdateViewPropertiesWnd(CCmdUI *pCmdUI);
    afx_msg void OnUpdateViewDecfunWnds(CCmdUI *pCmdUI);
    afx_msg void OnViewDecfunWnds();
    afx_msg void OnUpdateViewToolboxWnd(CCmdUI *pCmdUI);
    afx_msg void OnViewOutputWnd();
    afx_msg void OnUpdateViewOutputWnd(CCmdUI *pCmdUI);
	  
	afx_msg void OnViewClassView();
    afx_msg void OnUpdateViewViewClassView(CCmdUI *pCmdUI);

    afx_msg void OnUpdateTcDebugrun(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcDebugstep(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcDebugpro(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcDebugstop(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcCompile(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcBuild(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcClear(CCmdUI *pCmdUI);
    afx_msg void OnTcCompile();
    afx_msg void OnTcBuild();
    afx_msg void OnTcClear();
    afx_msg void OnTcOpenbuild();
    afx_msg void OnUpdateTcOpenbuild(CCmdUI *pCmdUI);
    afx_msg void OnUpdateTcOpendesignwin(CCmdUI *pCmdUI);
    afx_msg void OnTcOpendesignwin();
	//OnUpdateViewMore
	//afx_msg void OnTcNewDelBp();
	//afx_msg void OnUpdateTcDeleteallbp(CCmdUI *pCmdUI);
	//afx_msg void OnTcDeleteallbp();
	afx_msg void OnTcAbzhuazhua();
	afx_msg void OnDestroy();
	afx_msg void OnViewStatusBar();
	afx_msg void OnUpdateViewStatusBar(CCmdUI *pCmdUI);
	afx_msg void OnTcComlook();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnOpenTCHelp();
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileMruFile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileClose(CCmdUI *pCmdUI);
	afx_msg void OnButtonTcsetting();

	afx_msg void OnLink();
	afx_msg void OnLineJumper();
	afx_msg void OnUpdatePane1(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePane2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSize(CCmdUI* pCmdUI);
	afx_msg void OnButtonOpenstartview();
	afx_msg void OnTcOpenvipview();
	afx_msg void OnUpdate();
};


