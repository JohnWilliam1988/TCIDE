#pragma once
#include "atlimage.h"

// CTCPicture

class CTCPicture : public CStatic
{
	DECLARE_DYNAMIC(CTCPicture)

public:
	CTCPicture();
	virtual ~CTCPicture();

private:
	CImage img;	
	CString m_strPicName;
	CString m_clicked;	//µã»÷ÊÂ¼þ
public:    
	CString GetName(){ return m_strPicName; };
    void SetRCPicture(CString name);
    void SetPicture(CString path);
	void TCSetClickEvent(CString str);
	CString TCGetClickEvent(){ return m_clicked; }
protected:
	DECLARE_MESSAGE_MAP()
};


