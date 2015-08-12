// DiaNewProj.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "DiaNewProj.h"
#include "afxdialogex.h"

#include "TCButton.h"
#include "matchLib.h"
#include "TCRunTimeManager.h"

// CDiaNewProj �Ի���

IMPLEMENT_DYNAMIC(CDiaNewProj, CDialogEx)

CDiaNewProj::CDiaNewProj(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaNewProj::IDD, pParent)
    , m_ProjName(_T("<��������>"))
    , m_ProjPath(_T("<ѡ����Ŀ·��>"))
    , m_LangType(0)
{

}

CDiaNewProj::~CDiaNewProj()
{
}

void CDiaNewProj::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITPROJNAME, m_ProjName);
    DDX_Text(pDX, IDC_EDITPROJPATH, m_ProjPath);
    DDX_Radio(pDX, IDC_RADIOCN, m_LangType);
}


BEGIN_MESSAGE_MAP(CDiaNewProj, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDiaNewProj::OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDITPROJPATH, &CDiaNewProj::OnEnChangeEditprojpath)
END_MESSAGE_MAP()

// CDiaNewProj ��Ϣ�������

void CDiaNewProj::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
	//�ж��Ƿ���Ҫ�رտ�ʼҳ��
	if( CTCRunTimeManager::g_StartPageClose )
		((CTCApp*)AfxGetApp())->TCCloseAllView();
	else
		((CTCApp*)AfxGetApp())->TCCloseWithoutStartView();

    int langType=m_LangType;

    if(m_ProjPath == _T("") ||m_ProjPath == _T("<ѡ����Ŀ·��>") )
    { 
        AfxMessageBox(_T("��ѡ����Ŀ·��!")); 
        return;
    }

    if(m_ProjName == _T("")||m_ProjName == _T("<��������>")) 
    { 
        AfxMessageBox(_T("��������Ŀ����!")); 
        return;
    }

    if(!MyValidator::validateNameNoException(m_ProjName))
    {
        AfxMessageBox(_T("��������ȷ����Ŀ����,�����Ļ�����ĸ�����»��߿�ͷ,������ֻ�ܳ�������,��ĸ,�»���,����!")); 
        return;
    }

    //�ж�·���Ƿ���Դ�����Ŀ
    CString csTempProjPath;
    csTempProjPath.Format(_T("%s\\%s"),m_ProjPath,m_ProjName);

    if(PathFileExists(csTempProjPath))
    {
        AfxMessageBox(_T("��·�����Ѿ�����һ��ͬ����Ŀ,�޷�����,������!")); 
        return;
    }
    //������Ŀ�ļ���
    if(0==CreateDirectory(csTempProjPath,NULL))
    {
        AfxMessageBox(_T("���Ϸ�����Ŀ·��,����ѡ����ȷ����Ŀ·��!")); 
		return;
    }
    //������Ŀ�����ļ�
    CString csTempProjFile;
    csTempProjFile.Format(_T("%s\\%s.tcproj"),csTempProjPath,m_ProjName);

    //����Դ�ļ������ļ���
    //�ж�·���Ƿ���Դ�����Ŀ
    CString csTempSourcePath;
    csTempSourcePath.Format(_T("%s\\%s"),csTempProjPath,m_ProjName);
    //�����������½���,�ǲ����ܴ��ڵ�,��Ϊ����Ŀ¼���½���
    if(0==CreateDirectory(csTempSourcePath,NULL))   
    {
        AfxMessageBox(_T("������ĿԴ�ļ��ļ���ʧ��,������!")); 
        return;
    }
    //����Ĭ�ϵ���Ŀ����Դ�ļ�
    //����Ǽ�¼Ĭ����������Ŀ�����ļ���·��
    CString cstFileName;
    cstFileName.Format(_T("%s.t"),m_ProjName);

    CString csDefaultFile;
    csDefaultFile.Format(_T("%s\\%s.t"),csTempSourcePath,m_ProjName);
    //����Ĭ�ϵ�t�ļ�
    CString csDefSource;
    //����
    if( langType == 0)
    {
        byte bytes[2];
        bytes[0] = 0xFF;
        bytes[1] = 0xFE;
        wchar_t csDefCode[256]={0};
        ::wsprintfW(csDefCode,_T("�ռ� %s\n//Ĭ�Ͽռ�\n\n//��ʼ��ť_�������\n���� ��ʼ_���()\n\n���ܽ���\n\n//�˳���ť_�������\n���� �˳�_���()\n\n����.�˳�()\n���ܽ���\n\n�ռ����\n//�ռ�������"),m_ProjName);
        int nLength=::wcslen(csDefCode);
        int bRet;
        HANDLE handle=0;
        DWORD dwWritenSize = 0;
        handle=CreateFile(csDefaultFile,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
        ::WriteFile(handle,bytes,2,&dwWritenSize,0);
        bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
        CloseHandle(handle);
    }
    //Ӣ��
    else if( langType == 1 )
    {
        byte bytes[2];
        bytes[0] = 0xFF;
        bytes[1] = 0xFE;
        wchar_t csDefCode[256]={0};
        ::wsprintfW(csDefCode,_T("namespace %s\n//Ĭ�Ͽռ�\n\n//��ʼ��ť_�������\nfunction start_click()\n\nendfunction\n\n//�˳���ť_�������\nfunction exit_click()\n\nhelp.exit()\nendfunction\n\nendnamespace\n//�ռ����"),m_ProjName);
        int nLength=::wcslen(csDefCode);

        int bRet;
        HANDLE handle=0;
        DWORD dwWritenSize = 0;
        handle=CreateFile(csDefaultFile,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
        ::WriteFile(handle,bytes,2,&dwWritenSize,0);
        bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
        CloseHandle(handle);
    }

    //������Դ�ļ���
    CString csTempRcPath;
    csTempRcPath.Format(_T("%s\\��Դ"),csTempSourcePath);
    if(0==CreateDirectory(csTempRcPath,NULL))
    {
        AfxMessageBox(_T("������Դ�ļ���ʧ�ܣ�������!"));
        return;
    }
        
    //����Ĭ�ϵ���Դ�ļ�
    CString csDefRcSourceFile;
    CString csLangType=_T("");
    //////////////////////////////////////////////6-11��������Ŀ���Ĭ�ϵ�������Դ�ļ�/////////////////////////
    CString csDefaultRcFile;
    csDefaultRcFile.Format(_T("%s\\main.twin"),csTempRcPath);
    //
    if( langType == 0)
	{
        CFile cFile;
        cFile.Open(csDefaultRcFile,CFile::modeCreate|CFile::modeWrite);
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),m_ProjName,_T("\""));
		CString Twin;
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("����"),m_ProjName,diaTitle,0,0,482,306);
		WORD unicode = 0xFEFF;     //UNICODE�����ļ�ͷ
		cFile.Write(&unicode,2);   //д�������Ϣ
		cFile.Write(Twin,Twin.GetLength()*sizeof(TCHAR)); 
		cFile.Write(_T("\n"),2); //
		CString csID=_T("��ʼ");   //
		CString csText=_T("��ʼ"); //
		CString title;             //
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		RECT rc;
		rc.left=315;
        rc.top=255;
        rc.right=315+65;
        rc.bottom=255+28;
		CString csClick;
        csClick.Format(_T("%s.��ʼ_���"),m_ProjName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d %s| ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,_T("Ĭ�ϰ�ť:1"),csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2); //����
		csID=_T("�˳�");
		csText=_T("�˳�");
		rc.left=396;
        rc.top=255;
        rc.right=396+65;
        rc.bottom=255+28;
		csClick.Format(_T("%s.�˳�_���"),m_ProjName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2); //����
		CString diaInfo;           //д�������Ϣ
		diaInfo.Format(_T("%s"),_T("���ڽ���"));
		cFile.Write(diaInfo,diaInfo.GetLength()*sizeof(TCHAR));
		cFile.Close();
	}
	else if( langType == 1)
	{
        CFile cFile;
        cFile.Open(csDefaultRcFile,CFile::modeCreate|CFile::modeWrite);
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),m_ProjName,_T("\""));
		CString Twin;
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("����"),m_ProjName,diaTitle,0,0,482,306);
		WORD unicode = 0xFEFF;                            //UNICODE����
		cFile.Write(&unicode,2);                          //����
		cFile.Write(Twin,Twin.GetLength()*sizeof(TCHAR)); //д�������Ϣ
		cFile.Write(_T("\n"),2); 
		CString csID=_T("start");
		CString csText=_T("��ʼ");
		CString title;
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		RECT rc;
		rc.left=315;
        rc.top=255;
        rc.right=315+65;
        rc.bottom=255+28;
		CString csClick;
        csClick.Format(_T("%s.start_click"),m_ProjName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d %s| ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,_T("Ĭ�ϰ�ť:1"),csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2);  //����
		csID=_T("exit");
		csText=_T("�˳�");
		rc.left=396;
        rc.top=255;
        rc.right=396+65;
        rc.bottom=255+28;
		csClick.Format(_T("%s.exit_click"),m_ProjName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		cFile.Write(TCButton,TCButton.GetLength()*sizeof(TCHAR));
		cFile.Write(_T("\n"),2);  //����
		CString diaInfo;
		diaInfo.Format(_T("%s"),_T("���ڽ���"));
		cFile.Write(diaInfo,diaInfo.GetLength()*sizeof(TCHAR));
		cFile.Close();
	}

    if( langType == 0)
        csLangType=_T("0");
    else if( langType == 1)
        csLangType=_T("1");

    //���������ļ���
    CString csTempBuild;
    csTempBuild.Format(_T("%s\\����"),csTempProjPath);

    if(0==CreateDirectory(csTempBuild,NULL))   
    {
        AfxMessageBox(_T("������Ŀ�����ļ���ʧ��,������!")); 
        return;
    }

	    //���������ļ���
    CString csTempCompile;
    csTempCompile.Format(_T("%s\\����"),csTempProjPath);

    if(0==CreateDirectory(csTempCompile,NULL))   
    {
        AfxMessageBox(_T("������Ŀ�����ļ���ʧ��,������!")); 
        return;
    }


    //������Ŀ��Ϣ
    CTCRunTimeManager::setProjInfor(csTempProjPath,m_ProjName,csTempProjFile,csTempSourcePath,csDefaultFile,cstFileName,csDefaultRcFile,csTempBuild,csTempCompile,langType);
    
    //д��Ŀ�����ļ�
    CTCRunTimeManager::setTCProjFile(_T("ProjInfo"),_T("EnterFile"),cstFileName);
    CTCRunTimeManager::setTCProjFile(_T("ProjInfo"),_T("Language"),csLangType);
	CTCRunTimeManager::setTCProjFile(_T("ProjInfo"),_T("Version"),CTCRunTimeManager::g_TCVersion);

	//csTempProjFile
	((CTCApp*)AfxGetApp())->AddToRecentFileList(csTempProjFile);

    CDialogEx::OnOK();
}


void CDiaNewProj::OnEnChangeEditprojpath()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    CTCRunTimeManager::setMainSettingIni(_T("����"),_T("��Ŀ�½�λ��"),m_ProjPath);
}


BOOL CDiaNewProj::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CEdit*)(GetDlgItem(IDC_EDITPROJNAME)))->SetLimitText(31);
    //��ȡ�ϴ�ѡ�����Ŀ·��
    CString csLastPath=CTCRunTimeManager::getMainSettingIni(_T("����"),_T("��Ŀ�½�λ��"));
    m_ProjPath=csLastPath;
	this->SetBackgroundColor(0xf5e7dd);
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
