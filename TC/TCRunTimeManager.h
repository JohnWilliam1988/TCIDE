#pragma once

#include "matchLib.h"
#include "TCExeVersionManager.h"
#include "MyApiManager.h"
#include "EditorSettingPage.h"
class CTCRunTimeManager
{
public:
	static CString g_TCVersion;
    static CString g_runtimePath;     //这个是记录当前应用程序的路径
    static CString g_MainConfigPath;  //这个是记录当前应用程序的主配置文件的路径.
    static CString g_APIPath;         //这个是记录API的路径
    static CString g_HelpFilePath;    //这个是记录帮助文件的名称
    static CString g_ExeBackFile;    //这个是程序包

	static CString g_ExeBackFileVIP;    //VIP程序包1

    //static MyApiFrame *g_ApiFrame;    //这个是XML的对象
	
    static CString g_ProjPath;         //这个是记录当前项目的项目的路径
    static CString g_ProjName;         //这个是记录当前项目的项目的名称
    static CString g_ProjFilePath;     //这个是记录当前项目的项目管理tcproj文件的路径
    static CString g_ProjSourcePath;   //这个是记录当前源文件项目的路径
    static CString g_projEnterFile;    //这个是记录启动文件
    static CString g_projEnterFileName;//这个是记录启动文件文件名
    static CString g_projRcFile;       //这个是记录资源文件的
    static CString g_ProjBuildPath;    //这个是生成的路径
	static CString g_ProjCompilePath;  //编译使用的路径
    static int     g_LangType;         //这个是记录语言色类型
    static bool    g_IsLoadProj;       //标识是否加载了项目
	static bool	   g_ShowRunConfim;	   //标识是否确定运行项目
	static CMyApiManager g_TCAip;	   //API管理对象

	///////////编辑器全局变量//////////////////////
	static CString g_EditorFontName;   //编辑器字体
	static int	   g_EditorFontSize;   //编辑器字体大小
	static bool	   g_EditorFontBold;   //编辑器字体是否加粗
	static int     g_EditorTabWidth;   //编辑器Tab键宽度
	static bool	   g_EditorSignReplace;//编辑器自动替换中文字符
	///////////编辑器全局变量//////////////////////

	//////////自动更新全局变量////////////////////
	static bool    g_UpdateCheck;	  //启动检测版本
	//////////自动更新全局变量////////////////////

	/////////启动页面全局变量//////////////////////
	static bool g_StartPageShow;	//启动TC时显示启动页面			
	static bool g_StartPageClose;	//打开项目时关闭启动页面
	/////////启动页面全局变量//////////////////////

//操作
public:
	static BOOL CTCRunTimeManager::EnablePrivilege(LPCTSTR pszPrivName, BOOL fEnable /*= TRUE*/);
    static void commonInit();
    static void setProjInfor(CString path,CString name,CString ProjFilePath,CString sourcePath,CString enterFile,CString enterFileName,CString rcFile,CString buildPath,CString compilePath,int langType);
    static bool setMainSettingIni(CString appName,CString keyName,CString value);
    static CString getMainSettingIni(CString appName,CString keyName);
    static bool setTCProjFile(CString appName,CString keyName,CString value);
    static CString getTCProjFile(CString appName,CString keyName);
    static int getTCProjFileInt(CString appName,CString keyName);
    static bool isSameFile(CString file1,CString file2);
    static void getRCPathBmpList(CStringArray &bmpArray);
	static bool isMatchStatement(CString statement);
	static bool isMatchStatementEx(CString statement);
	static bool isBraceStatement(CString statement);

	//从配置文件中读取编辑器字体信息
	static void InitEditorInfo();
	//从配置文件中读取TC调试以及其他相关配置
	static void InitOtherSetting();
	//设置编辑器字体信息
	static void SetEditorInfo(EditorSetInfo editorInfo);
	//设置自动更新信息
	static void SetUpdateInfo(bool check);

private:
    CTCRunTimeManager(void);
};

//设置树形控件的节点附加信息的枚举类型
enum FCTREENODETYPE{
    FCTREE_PROJMANAGER=1024,
    FCTREE_FOLDER,
    FCTREE_RCFILE,
    FCTREE_RCWIN,
    FCTREE_FILE,
    FCTREE_NAMESPACE,
    FCTREE_FUNCTION
};

//属性的类型信息
enum TCPROPERTYTYPE
{
    TCPROPERTY_PROJ=256,
    TCPROPERTY_TWIN,
    TCPROPERTY_CTRL_STATIC,
    TCPROPERTY_CTRL_GROUP,
    TCPROPERTY_CTRL_BUTTON,
    TCPROPERTY_CTRL_EDIT,
    TCPROPERTY_CTRL_RADIO,
    TCPROPERTY_CTRL_CHECK,
    TCPROPERTY_CTRL_LIST,
    TCPROPERTY_CTRL_COMBOX,
    TCPROPERTY_CTRL_PIC,
    TCPROPERTY_CTRL_PROGRESS,
    TCPROPERTY_CTRL_PATH,
    TCPROPERTY_CTRL_SLIDER,
    TCPROPERTY_CTRL_DATE,
	TCPROPERTY_CTRL_TABLE,
	TCPROPERTY_CTRL_TIMER,
	TCPROPERTY_CTRL_HOTKEY,
	TCPROPERTY_CTRL_EXPLORER,
	TCPROPERTY_CTRL_GRIDCTRL,
    TCPROPERTY_CTRL,
	TCPROPERTY_TWIN_NODE,
    TCPROPERTY_MAX
};

// 控件的类型
enum TCTOOLTYPE
{
    TCTT_STATIC,
    TCTT_BUTTON,
    TCTT_EDIT,
    TCTT_RADIO,
    TCTT_CHECK,
    TCTT_COMBOBOX,
	TCTT_GROUP,
	TCTT_LISTBOX,
	TCTT_PROGRESS,
	TCTT_DATE,
	TCTT_PICTURE,
    TCTT_SLIDER,
    TCTT_PATH,
    TCTT_TAB,
	TCTT_TIMER,
	TCTT_HOTKEY,
	TCTT_EXPLORER,
	TCTT_GRIDCTRL,
	TCTT_TWIN,
    TCTT_MAX
};
//用于控件编辑功能,父窗口的类型
enum TCPARENTTYPE
{
	TCPARENT_DESDIN,
	TCPARENT_TABPAGE,
	TCPARENT_MAX
};

enum TCCURRENTVIEW
{
	TCVIEW_DESIGN,
	TCVIEW_FILE,
	TCVIEW_MAX
};

//报表控件类型枚举
enum TCGRIDCTRLTYPE
{
	TCGRID_CTRL_NOMAL,
	TCGRID_CTRL_READONLY,
	TCGRID_CTRL_COMBOX,
	TCGRID_CTRL_CHECK,
	TCGRID_CTRL_DATE,
	//TCGRID_CTRL_URL,
	TCGRID_CTRL_NUM,
	TCGRID_CTRL_MAX
};

//输出窗口控件类型枚举
enum TCOUTPUTCTRL
{
	//TCOUTPUT_OUTWND,
	TCOUTPUT_PROGRESS,
	TCOUTPUT_MAX
};

enum TC_PROGRESS_TYPE
{
	TC_PROGRESS_SHOW=-1,
	TC_PROGRESS_HIDE=0,
};

//TC添加事件的类型
enum TC_EVENT_TYPE
{
	TC_EVENT_CLOSEWINDOW,
	TC_EVENT_PRETRANSMSG,
	TC_EVENT_WINDOWPROC,
	TC_EVENT_MAX,
};

const unsigned int MAXCTRLNAME=64;
const unsigned int MAXCTRLLENGTH=1024;


struct wndNode
{
	TCHAR id[MAXCTRLNAME];
	CString winInfo;
	wndNode()
	{
		::wmemset(id,0,MAXCTRLNAME);
	}
};

class CTCDlgManager
{
public:
	CTCDlgManager();
	~CTCDlgManager();

	void load(CString rcPath = CTCRunTimeManager::g_projRcFile);
	void write(CString rcPath = CTCRunTimeManager::g_projRcFile);
	unsigned int getCount() const;
	bool add(wndNode *node);
	bool add(TCHAR * const id,CString winInfo);
	bool isGoodID(const TCHAR * const id);
	const wndNode *getat(unsigned int index) const;
	wndNode *getat(unsigned int index);
	//int isContainWnd(CWnd *hwnd) const;
	//bool wndChangeID(CWnd *hwnd,const TCHAR * const id);
	int getIndexByID(const TCHAR * const id);
	bool removeWndInfo(const TCHAR * const id);
	void reset();
	int GetLength();
	
private:
	wndNode m_list[MAXCTRLLENGTH];
	unsigned int m_length;
};


typedef struct _GETNOWNEWS_ONPUT{

	int  newsID;					  //新闻ID判断是否有更新
	wchar_t newsTitle[64];               //新闻标题
	wchar_t newsBuff[256];               //新闻内容
	wchar_t urlBuff[MAX_PATH];           //链接地址

	_GETNOWNEWS_ONPUT()//初始化
	{
		memset(this,0,sizeof(_GETNOWNEWS_ONPUT)); 
	}

}GETNOWNEWS_ONPUT ,*PGETNOWNEWS_ONPUT;