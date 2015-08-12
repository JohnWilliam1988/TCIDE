// DiaNewFile.cpp : 实现文件
//

#include "stdafx.h"
#include "TC.h"
#include "DiaNewFile.h"
#include "afxdialogex.h"


// CDiaNewFile 对话框

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

// CDiaNewFile 消息处理程序

void CDiaNewFile::SetFilePath(CString path)
{
    m_FilePath=path;
}

void CDiaNewFile::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码

    UpdateData(TRUE);

    CString csHelpFile;
    csHelpFile.Format(_T("%s\\%s.t"),m_FilePath,m_FileName);

    if (::PathFileExists(csHelpFile))
    {
        AfxMessageBox(_T("文件已存在，创建失败!"));
        return;
    }

    //新建文件
    _TCHAR bom = (_TCHAR)0xFEFF;
    CFile* pFile = new CFile();

    //2010-1-6 号,邓,修理DeBug 
    bool b_Open=false;
    b_Open=pFile->Open(csHelpFile,CFile::modeCreate | CFile::modeWrite );
    if(b_Open==false)
    {
        AfxMessageBox(_T("文件创建失败!"));
        return;
    }

    pFile->Write(&bom,sizeof(_TCHAR));
    pFile->Close();

	if(pFile != NULL)
		delete pFile;

    if (!::PathFileExists(csHelpFile))
    {
        AfxMessageBox(_T("创建文件失败!"));
        return;
    }

	//9-14只要是打开t文件的地方都不用添加到最近打开的链表,参数2是是否添加到最近打开链表
    ((CTCApp*)AfxGetApp())->m_pDocTemplateTc->OpenDocumentFile(csHelpFile,FALSE,TRUE);

    CDialogEx::OnOK();
}

BOOL CDiaNewFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetBackgroundColor(0xf5e7dd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
