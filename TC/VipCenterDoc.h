#pragma once

// CVipCenterDoc 文档

class CVipCenterDoc : public CDocument
{
	DECLARE_DYNCREATE(CVipCenterDoc)

public:
	CVipCenterDoc();
	virtual ~CVipCenterDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
