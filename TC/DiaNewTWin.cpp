// DiaNewTWin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "MainFrm.h"
#include "DiaNewTWin.h"
#include "afxdialogex.h"


// CDiaNewTWin �Ի���

IMPLEMENT_DYNAMIC(CDiaNewTWin, CDialogEx)

CDiaNewTWin::CDiaNewTWin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaNewTWin::IDD, pParent)
	, m_newTwinName(_T("<��������>"))
{
	m_needNewTFile = true;
}

CDiaNewTWin::~CDiaNewTWin()
{
}

void CDiaNewTWin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTWINID, m_newTwinName);
}


BEGIN_MESSAGE_MAP(CDiaNewTWin, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDiaNewTWin::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiaNewTWin ��Ϣ�������


BOOL CDiaNewTWin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetBackgroundColor(0xf5e7dd);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDiaNewTWin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if(!MyValidator::validateNameNoException(m_newTwinName))
	{
		AfxMessageBox(_T("��������ȷ�Ĵ�������,�����Ļ�����ĸ�����»��߿�ͷ,������ֻ�ܳ�������,��ĸ,�»���,����!"));
		return;
	}

	if(!((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.isGoodID(m_newTwinName))
	{
		AfxMessageBox(_T("��Ŀ���Ѿ����ڵ�ǰ���ƵĴ���,������!"));
		return;
	}

	CString newTFileName;
	newTFileName.Format(_T("%s.t"),m_newTwinName);
	CString newFilePath;
	newFilePath.Format(_T("%s\\%s.t"),CTCRunTimeManager::g_ProjSourcePath,m_newTwinName);
	if(PathFileExists(newFilePath))
	{
		if(IDYES == MessageBox(_T("��ǰ·�����Ѿ�����ͬ��T�ļ�,�����½����ڣ�"),_T("TC"), MB_YESNO | MB_ICONINFORMATION ))
		{
			m_needNewTFile = false;
		}
		else
			return;
	}

	TCHAR winID[MAXCTRLNAME]={0};
	CString Twin;
	if( CTCRunTimeManager::g_LangType == 0 )
	{
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),_T("�½�����"),_T("\""));
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("����"),m_newTwinName,diaTitle,0,0,482,306);
		Twin+=_T("\n");
		CString csID=_T("��ʼ");   
		CString csText=_T("��ʼ"); 
		CString title;             
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		RECT rc;
		rc.left=315;
		rc.top=255;
		rc.right=315+65;
		rc.bottom=255+28;
		CString csClick;
		csClick.Format(_T("%s.��ʼ_���"),m_newTwinName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");	//����
		csID=_T("�˳�");
		csText=_T("�˳�");
		rc.left=396;
		rc.top=255;
		rc.right=396+65;
		rc.bottom=255+28;
		csClick.Format(_T("%s.�˳�_���"),m_newTwinName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");//����
		CString diaInfo;           //д�������Ϣ
		diaInfo.Format(_T("%s"),_T("���ڽ���"));
		Twin+=diaInfo;
	}
	else if(CTCRunTimeManager::g_LangType == 1)
	{
		CString diaTitle;
		diaTitle.Format(_T("%s%s%s"),_T("\""),_T("NewDialog"),_T("\""));
		Twin.Format(_T("%s %s %s %d,%d,%d,%d | |"),_T("����"),m_newTwinName,diaTitle,0,0,482,306);
		Twin+=_T("\n"); 
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
		csClick.Format(_T("%s.start_click"),m_newTwinName);
		CString TCButton;
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");
		csID=_T("exit");
		csText=_T("�˳�");
		rc.left=396;
		rc.top=255;
		rc.right=396+65;
		rc.bottom=255+28;
		csClick.Format(_T("%s.exit_click"),m_newTwinName);
		title.Format(_T("%s%s%s"),_T("\""),csText,_T("\""));
		TCButton.Format(_T("%s %s %s %d,%d,%d,%d | ��굥��:%s|"),_T("��ť"),csID,title,rc.left,rc.top,rc.right,rc.bottom,csClick);
		Twin+=TCButton;
		Twin+=_T("\n");
		CString diaInfo;
		diaInfo.Format(_T("%s"),_T("���ڽ���"));
		Twin+=diaInfo;
	}
	wcscpy_s(winID,m_newTwinName);		
	//�򴰿ڹ���������ӳɹ�Ȼ����ӽ���ڵ�
	if(((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_DlgManager.add(winID,Twin))
	{
		if(m_needNewTFile)
		{
			//����
			if(CTCRunTimeManager::g_LangType == 0)
			{
				byte bytes[2];
				bytes[0] = 0xFF;
				bytes[1] = 0xFE;
				wchar_t csDefCode[256]={0};
				::wsprintfW(csDefCode,_T("�ռ� %s\n//����Ĭ�Ͽռ�\n\n���� ��ʼ_���()\n//���������Ҫִ�еĴ���\n\n���� ��\n//�����������\n���ܽ���\n\n���� �˳�_���()\n//���������Ҫִ�еĴ���\n\n���� ��\n//�����������\n���ܽ���\n\n�ռ����\n//�ռ�������"),winID);
				int nLength=::wcslen(csDefCode);
				int bRet;
				HANDLE handle=0;
				DWORD dwWritenSize = 0;
				handle=CreateFile(newFilePath,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
				::WriteFile(handle,bytes,2,&dwWritenSize,0);
				bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
				CloseHandle(handle);
			}
			//Ӣ��
			else if(CTCRunTimeManager::g_LangType == 1)
			{
				byte bytes[2];
				bytes[0] = 0xFF;
				bytes[1] = 0xFE;
				wchar_t csDefCode[256]={0};
				::wsprintfW(csDefCode,_T("namespace %s\n//����Ĭ�Ͽռ�\n\nfunction start_click()\n//���������Ҫִ�еĴ���\n\nreturn true\n//�����������\nendfunction\n\nfunction exit_click()\n//���������Ҫִ�еĴ���\n\nreturn true\n//�����������\nendfunction\n\nendnamespace\n//�ռ�������"),winID);
				int nLength=::wcslen(csDefCode);

				int bRet;
				HANDLE handle=0;
				DWORD dwWritenSize = 0;
				handle=CreateFile(newFilePath,GENERIC_WRITE|GENERIC_READ,0,NULL,CREATE_ALWAYS,NULL,NULL);
				::WriteFile(handle,bytes,2,&dwWritenSize,0);
				bRet = ::WriteFile(handle,csDefCode,nLength*sizeof(TCHAR),&dwWritenSize,0);
				CloseHandle(handle);
			}
			((CTCApp*)AfxGetApp())->m_pDocTemplateTWin->OpenDocumentFile(m_newTwinName,FALSE,TRUE);

			//����Ŀ���ļ����ͷ
			CString csInclude;
			csInclude.Format(_T("#include\"%s.t\"\n"),winID);
			((CMainFrame*)AfxGetApp()->m_pMainWnd)->TCInsertCodeToMainFile(csInclude,0);
		}		
	}
	CDialogEx::OnOK();
}
