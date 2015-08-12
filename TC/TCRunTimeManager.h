#pragma once

#include "matchLib.h"
#include "TCExeVersionManager.h"
#include "MyApiManager.h"
#include "EditorSettingPage.h"
class CTCRunTimeManager
{
public:
	static CString g_TCVersion;
    static CString g_runtimePath;     //����Ǽ�¼��ǰӦ�ó����·��
    static CString g_MainConfigPath;  //����Ǽ�¼��ǰӦ�ó�����������ļ���·��.
    static CString g_APIPath;         //����Ǽ�¼API��·��
    static CString g_HelpFilePath;    //����Ǽ�¼�����ļ�������
    static CString g_ExeBackFile;    //����ǳ����

	static CString g_ExeBackFileVIP;    //VIP�����1

    //static MyApiFrame *g_ApiFrame;    //�����XML�Ķ���
	
    static CString g_ProjPath;         //����Ǽ�¼��ǰ��Ŀ����Ŀ��·��
    static CString g_ProjName;         //����Ǽ�¼��ǰ��Ŀ����Ŀ������
    static CString g_ProjFilePath;     //����Ǽ�¼��ǰ��Ŀ����Ŀ����tcproj�ļ���·��
    static CString g_ProjSourcePath;   //����Ǽ�¼��ǰԴ�ļ���Ŀ��·��
    static CString g_projEnterFile;    //����Ǽ�¼�����ļ�
    static CString g_projEnterFileName;//����Ǽ�¼�����ļ��ļ���
    static CString g_projRcFile;       //����Ǽ�¼��Դ�ļ���
    static CString g_ProjBuildPath;    //��������ɵ�·��
	static CString g_ProjCompilePath;  //����ʹ�õ�·��
    static int     g_LangType;         //����Ǽ�¼����ɫ����
    static bool    g_IsLoadProj;       //��ʶ�Ƿ��������Ŀ
	static bool	   g_ShowRunConfim;	   //��ʶ�Ƿ�ȷ��������Ŀ
	static CMyApiManager g_TCAip;	   //API�������

	///////////�༭��ȫ�ֱ���//////////////////////
	static CString g_EditorFontName;   //�༭������
	static int	   g_EditorFontSize;   //�༭�������С
	static bool	   g_EditorFontBold;   //�༭�������Ƿ�Ӵ�
	static int     g_EditorTabWidth;   //�༭��Tab�����
	static bool	   g_EditorSignReplace;//�༭���Զ��滻�����ַ�
	///////////�༭��ȫ�ֱ���//////////////////////

	//////////�Զ�����ȫ�ֱ���////////////////////
	static bool    g_UpdateCheck;	  //�������汾
	//////////�Զ�����ȫ�ֱ���////////////////////

	/////////����ҳ��ȫ�ֱ���//////////////////////
	static bool g_StartPageShow;	//����TCʱ��ʾ����ҳ��			
	static bool g_StartPageClose;	//����Ŀʱ�ر�����ҳ��
	/////////����ҳ��ȫ�ֱ���//////////////////////

//����
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

	//�������ļ��ж�ȡ�༭��������Ϣ
	static void InitEditorInfo();
	//�������ļ��ж�ȡTC�����Լ������������
	static void InitOtherSetting();
	//���ñ༭��������Ϣ
	static void SetEditorInfo(EditorSetInfo editorInfo);
	//�����Զ�������Ϣ
	static void SetUpdateInfo(bool check);

private:
    CTCRunTimeManager(void);
};

//�������οؼ��Ľڵ㸽����Ϣ��ö������
enum FCTREENODETYPE{
    FCTREE_PROJMANAGER=1024,
    FCTREE_FOLDER,
    FCTREE_RCFILE,
    FCTREE_RCWIN,
    FCTREE_FILE,
    FCTREE_NAMESPACE,
    FCTREE_FUNCTION
};

//���Ե�������Ϣ
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

// �ؼ�������
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
//���ڿؼ��༭����,�����ڵ�����
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

//����ؼ�����ö��
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

//������ڿؼ�����ö��
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

//TC����¼�������
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

	int  newsID;					  //����ID�ж��Ƿ��и���
	wchar_t newsTitle[64];               //���ű���
	wchar_t newsBuff[256];               //��������
	wchar_t urlBuff[MAX_PATH];           //���ӵ�ַ

	_GETNOWNEWS_ONPUT()//��ʼ��
	{
		memset(this,0,sizeof(_GETNOWNEWS_ONPUT)); 
	}

}GETNOWNEWS_ONPUT ,*PGETNOWNEWS_ONPUT;