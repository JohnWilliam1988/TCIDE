// TCPicture.cpp : 实现文件
//

#include "stdafx.h"
#include "../TC.h"
#include "TCPicture.h"
#include "../TCRunTimeManager.h"

// CTCPicture

IMPLEMENT_DYNAMIC(CTCPicture, CStatic)

CTCPicture::CTCPicture()
{
	m_strPicName = _T("");
	m_clicked = _T("");
}

CTCPicture::~CTCPicture()
{
}


BEGIN_MESSAGE_MAP(CTCPicture, CStatic)
END_MESSAGE_MAP()



// CTCPicture 消息处理程序

void CTCPicture::SetRCPicture(CString name)
{
	if( name != _T(""))
	{
		CString bmpPath;
		bmpPath.Format(_T("%s\\资源\\%s"),CTCRunTimeManager::g_ProjSourcePath,name);

		CRect rect;
		this->GetWindowRect(&rect);
		this->GetParent()->ScreenToClient(&rect);
		HRESULT ret = img.Load(bmpPath);
		if(!img.IsNull())
		{
			HBITMAP bitmap = img.Detach();
			this->SetBitmap(bitmap);
			m_strPicName = name;
			this->MoveWindow(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,TRUE);
			this->Invalidate();
		}
	}
}

void CTCPicture::SetPicture(CString path)
{
	CRect rect;
    this->GetWindowRect(&rect);
    this->GetParent()->ScreenToClient(&rect);
	HRESULT ret = img.Load(path);
    if(!img.IsNull())
    {
        HBITMAP bitmap = img.Detach();
        this->SetBitmap(bitmap);
		this->MoveWindow(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,TRUE);
		this->Invalidate();
	}
}

void CTCPicture::TCSetClickEvent(CString str)
{
	m_clicked = str;
}

