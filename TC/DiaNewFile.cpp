// DiaNewFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TC.h"
#include "DiaNewFile.h"
#include "afxdialogex.h"


// CDiaNewFile �Ի���

IMPLEMENT_DYNAMIC(CDiaNewFile, CDialogEx)

CDiaNewFile::CDiaNewFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaNewFile::IDD, pParent)
    , m_FileName(_T(""))
    , m_FilePath(_T(""))
{

}

CDiaNewFile::~CDiaNewFile()
{

}

void CDiaNewFile::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDITFILENAME, m_FileName);
    DDX_Text(pDX, IDC_EDITFILEPATH, m_FilePath);
}

BEGIN_MESSAGE_MAP(CDiaNewFile, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDiaNewFile::OnBnClickedOk)
END_MESSAGE_MAP()

// CDiaNewFile ��Ϣ�������

void CDiaNewFile::SetFilePath(CString path)
{
    m_FilePath=path;
}

void CDiaNewFile::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    UpdateData(TRUE);

    CString csHelpFile;
    csHelpFile.Format(_T("%s\\%s.t"),m_FilePath,m_FileName);

    if (::PathFileExists(csHelpFile))
    {
        AfxMessageBox(_T("�ļ��Ѵ��ڣ�����ʧ��!"));
        return;
    }

    //�½��ļ�
    _TCHAR bom = (_TCHAR)0xFEFF;
    CFile* pFile = new CFile();

    //2010-1-6 ��,��,����DeBug 
    bool b_Open=false;
    b_Open=pFile->Open(csHelpFile,CFile::modeCreate | CFile::modeWrite );
    if(b_Open==false)
    {
        AfxMessageBox(_T("�ļ�����ʧ��!"));
        return;
    }

    pFile->Write(&bom,sizeof(_TCHAR));
    pFile->Close();

	if(pFile != NULL)
		delete pFile;

    if (!::PathFileExists(csHelpFile))
    {
        AfxMessageBox(_T("�����ļ�ʧ��!"));
        return;
    }

	//9-14ֻҪ�Ǵ�t�ļ��ĵط���������ӵ�����򿪵�����,����2���Ƿ���ӵ����������
    ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->OpenDocumentFile(csHelpFile,FALSE,TRUE);

    CDialogEx::OnOK();
}

BOOL CDiaNewFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->SetBackgroundColor(0xf5e7dd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
