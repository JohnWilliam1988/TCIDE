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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "TCFunDecWnd.h"

////////�Զ���////////
#include "DesignToolBox.h"
#include "matchLib.h"
#include "TCCtlHeads.h"
#include "DiaTViewFind.h"
#include "XInfoTip.h"
#include "XInfoTipDebug.h"
#include "TCSettingDia.h"
#include "TCEditorLineJumper.h"
////////�Զ���////////

#define WM_MYTCDEBUG_MESSAGE         (WM_USER+100)
#define WM_MYTCOUTMSG_MESSAGE		 (WM_USER+101)

#define WM_MYCLOSESTARTVIEW_MESSAGE  (WM_USER+102)
#define WM_MYDESKTOPALERT_MESSAGE	 (WM_USER+103)

//#define PROTOCOL_CLIENT
//
//#include "protocol.h"

//��������ƥ�����ݽṹ�� ���°�
struct FindInfo
{
	CString path;                 //�����ļ�
	CString lindStr;              //�����ݴ�
	int lineNumber;               //�к�
	int strBeginPos;              //���ҵ��Ĵ���ʼλ��
	int strEndPos;                //���ҵ��Ĵ�����λ��

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

// ����
public:
    //������Ŀ
    void ProjFileViewReload();    
    //��ʾ��Ŀ������,upDate���������Ƿ�ʱ����
    void SetProjProperty(CString name,CString path,BOOL upDate = FALSE);
	//���ô��ڵĽڵ���Ϣ
	void SetRcNodeDiaProperty(CString name);
    //��ʾ�����Ƶ�����,upDate���������Ƿ�ʱ����
    void SetTWinDiaProperty(CString name,const CDiaTCDesign * const dlg,BOOL upDate = FALSE);
    //��ʾstatic�ؼ�������
    void SetTWinStaticProperty(CString id,CTCStatic *  ctl,BOOL upDate = FALSE);
    //��ʾgroup�ؼ�������
    void SetTWinGroupProperty(CString id,CTCGroupBox *  ctl,BOOL upDate = FALSE);
    //��ʾbutton�ؼ�������
    void SetTWinButtonProperty(CString id,CTCButton *  ctl,BOOL upDate = FALSE);
    //��ʾedit�ؼ�������
	void SetTWinEditProperty(CString id,CTCEdit *  ctl,BOOL upDate = FALSE);
    //��ʾCheck�ؼ�������
    void SetTWinCheckProperty(CString id, CTCCheck *  ctl,BOOL upDate = FALSE);
    //��ʾRadio�ؼ�������
    void SetTWinRadioProperty(CString id, CTCRadio *  ctl,BOOL upDate = FALSE);
    //��ʾCombox�ؼ�������
    void SetTWinComboxProperty(CString id, CTCComboBox *  ctl,BOOL upDate = FALSE);
    //��ʾPicture�ؼ�������
    void SetTWinPictureProperty(CString id, CTCPicture *  ctl,BOOL upDate = FALSE);
    //��ʾprogress�ؼ�������
    void SetTWinProgressProperty(CString id, CTCProgress *  ctl,BOOL upDate = FALSE);
    //��ʾ����������
    void SetTWinPathProperty(CString id, CTCBrowse *  ctl,BOOL upDate = FALSE);
    //��ʾSlider������
    void SetTWinSliderProperty(CString id, CTCSlider *  ctl,BOOL upDate = FALSE);
    //��ʾDate������
    void SetTWinDateProperty(CString id, CTCDate *  ctl,BOOL upDate = FALSE);
    //��ʾList������
    void SetTWinListProperty(CString id, CTCListBox *  ctl,BOOL upDate = FALSE);
    //��ʾTab������
	void SetTWinTableProperty(CString id, CTCTab*  ctl,BOOL upDate = FALSE);
	//��ʾTimer������
	void SetTWinTimerProperty(CString id, CTCTimer*  ctl,BOOL upDate = FALSE);
	//��ʾHotKey������
	void SetTWinHotKeyProperty(CString id, CTCHotKeyCtrl *  ctl,BOOL upDate = FALSE);
	//��ʾExplorer������
	void SetTWinExplorerProperty(CString id, CTCExplorer *  ctl,BOOL upDate = FALSE);
		//��ʾGridCtrl������
	void SetTWinGridCtrlProperty(CString id, CGridCtrl *  ctl,BOOL upDate = FALSE);
    //���޸����Կ�ʱ,�޸���Ŀ�����
	//���Ĳ��������������������ļ��ڵ㣬������Դ�ڵ�
    bool FileReNameByPropWnd(CString oldPatn,CString newName,BOOL FileOrWnd = TRUE);
	//�޸��ض�������Ա�������ػ���˸
	void SetTheProperty(TCPROPERTYTYPE protype,CString name,DWORD_PTR title,CString newValue);
	//////////////////////////////��ӿؼ����Ƶ����Դ���������//////////////////////////////
	void InsertCtrlToProCom(CString ctrlName);

	void TCUpdateProperty(TCTOOLTYPE type,CString ctrlID,CWnd *pCtrl,BOOL update);

	void TCActiveFindWnd();

	//������������
	void SetDecText(CString csNS,CString csFunName,CString csFunProto,CString csFunDes,CString csExample);

    //�༭��Ŀ
    bool TCCompileProj();
    //������Ŀ
    bool TCBuildProj();
    //������Ŀ
    bool TCClearProj();
    //�������е��ĵ�
    void TCSaveAllDoc();
    //���������Ϣ
    void TCClearOutMsg();
	//���������Ϣ
	void TCClearAllContentMsg();
    //���һ�������Ϣ
    void TCOutMsgAddString(CString add);
    //���һ���ϵ�
    bool TCDebugAddBP(CString path,int line);
    //ɾ��һ���ϵ�
    bool TCDebugDeleteBP(CString path,int line);

	void TCFindAllContent();
	void TCAddAllContentString();

	//////////////////////9-4���ɾ�����жϵ����/////////////
	void TCDebugDeleteAllBP();

    //���Ե�ʱ����ؿɼ��ı����б�
    void TCDebugLoadVarList();  
    //����
    void TCDebug();
    //����ִ�з���
    void TCDebugRun();
    //��������Ŀ
    bool TCOpenExistProject();

	//(11-2)����Ŀ�ɹ���,���ݵ�ǰ��Ŀ
	bool TCBackUpProject();

    //TC��ʱ��,����Ĳ���
    void TCStartPageSetting();
    //�õ���ǰ����������ѡ��
    TCTOOLTYPE GetSelectToolType(); 
    //���ù��������Ŀؼ���ѡ��
    void ResetSelectToolType(); 
    //ѹ�������Ľӿ�
    bool compress7z(CString compath);
    //����¼�����
    bool TCAddEventCode(CString namesp,CString fun,TC_EVENT_TYPE type=TC_EVENT_MAX);
    //����¼�����
    bool TCAddEventCode(CString csEvent,TC_EVENT_TYPE type=TC_EVENT_MAX);
	//�ڵ�ǰ��Դ����t�ļ�ҳ��������
    bool TCInsertCode(CString csCode,int pos);
	//����Ŀ�����ļ�����ļ�ͷ
	 bool TCInsertCodeToMainFile(CString csCode,int docPos);
	//�������������
	void ActiveDecWnd();

	void ActivePerprotyWnd();

	void ActiveOutPutWnd();

	void ResetPropertyCombox();
	/////////////////////////״̬����ʾ��غ���///////////////////////////////

	//���õ�ǰ�������������Ϣ��״̬��
	void TCSetStatusRowAndCol(int row,int col);

	//״̬��ͷ����Ϣ�ӿ�   ����1����ʾ������  ����2�������ʾ������ 
	void TCSetHeadPanelText(CString text,CString tiptext);

	//��ʾ������
	void ShowProgress(BOOL bShow = TRUE);

	//���ý���ֵ
	void TCSetProgressPos(int pos);

	//�����ļ���С
	void TCSetFileInfo(const UINT size);

	//�����ļ���С�ɼ�
	void TCSetFileInfoVisible(BOOL isViewable);
	/////////////////////////״̬����ʾ��غ���///////////////////////////////

	//TC�Զ�����Ϣ
	void TCSendOutWndMsg(WPARAM wParam,LPARAM lParam);

	//�����ʾ
    void GiveToolTip(CWnd *pWnd, LPCTSTR szTooltipText, HICON hIcon=NULL);

    //�Ƴ������ʾ
    void RemoveToolTip(CWnd *pWnd) ;
    
    //����

	bool TCIsDebugValue(const TCHAR *name,CString &csValue);
public:
	bool m_isVipBuild;                      //�Ƿ���VIP����

	//const MyDebugValueList m_TempValueList; //   �ϵ���Ե�ʱ��,��ʱ�ı���ָ��
	CWinThread *m_LoginHeartThread;

    ////�����Ǵ��������õ��ں˶���͹ܵ�ͨѶ����////////
    HANDLE m_LibEventHandle;     //֪ͨ���������ں˶���
    HANDLE m_WinEventHandle;     //֪ͨ������ں˶���
    bool   m_isInBP;             //���������ʶ�Ƿ��ڶϵ�״̬
	HANDLE m_hFileMap;
    CMyDebugInfor  *m_TCDebugInfor;                     //
    HANDLE m_TCDebugProcessHandle;                      //

    ////���������õ��ں˶���͹ܵ�ͨѶ����//////////////

    bool m_IsInCompile;				//��ʶ�Ƿ����
	bool m_IsDebuging;				//��ʶ�Ƿ����״̬
    //CTCDlgManager m_TWinManager;  //�����������¼������ƵĹ������

	CDiaTViewFind m_wndFind;

	CXInfoTip 		m_TipEx;                  //���������ʾʹ��

	CXInfoTipDebug 		m_TipExDebug;             //���������ʾ��������ʹ��

	//��ƴ���ʱ�Ĺ�����
	CTCDlgManager m_DlgManager;

	CString m_pCurrentWndID;					//ָ��ǰ�򿪵�������

	//���ļ�����������س�Ա ���°� 2013.1.21
	std::vector<int> m_markposL;
	std::vector<int> m_markposR;
	std::vector<int> m_markposLpos;
	std::vector<int> m_markposRpos;
	std::vector<int> m_markLineLenthR;
	std::vector<int> m_markLineLenthL;
	std::vector<wstring> m_markLpath;
	std::vector<wstring> m_markRpath;
	int m_markLorR;  //0:��ʼֵ,1:�󵼺�,2:�ҵ���
	//���ļ�����������س�Ա ���°� 2013.1.21
	//��������ƥ�����ݳ�Ա ���°�
	std::vector<FindInfo> m_findInfo;
	CString m_curFoundTFile;					//��ǰ�������ݵ�T�ļ�·��
	//��������ƥ�����ݳ�Ա ���°�
	//ѡ�����ÿ�
	//CTCSettingDia m_wndSettingDia;
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
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

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	//afx_msg void OnViewCaptionBar();
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    /////////��������,һ�����ڵ��Ե��Զ������Ϣ////
    afx_msg LRESULT OnTCMyDebugPTMsg(WPARAM, LPARAM); 
    /////////��������,һ�����ڵ��Ե��Զ������Ϣ////
	/////////������ڵ��Ե���Ϣ///////////////
	afx_msg LRESULT OnTCOutMsg(WPARAM, LPARAM); 
	/////////������ڵ��Ե���Ϣ///////////////


	/////////������һ���Զ���Ĳ�����Ϣ,�������Ե��Դ����ʱ���쳣������Ϣ��/////////////////////
    afx_msg LRESULT OnTCMyDebugInforMsg(WPARAM, LPARAM); 
    /////////������һ���Զ���Ĳ�����Ϣ/////////////////////

	/////////�Զ���ر�������ͼ��Ϣ///////////////////
	afx_msg LRESULT OnTCMyCloseStartViewMsg(WPARAM, LPARAM);
	/////////�Զ���ر�������ͼ��Ϣ///////////////////
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


