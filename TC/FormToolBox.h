#pragma once
#include "Resource.h"
#include "ControlRect.h"
#include "afxwin.h"

//
#include "ButtonSSL.h"

// CFormToolBox 对话框

class CFormToolBox : public CDialogEx
{
	DECLARE_DYNAMIC(CFormToolBox)

public:
	CFormToolBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFormToolBox();

// 对话框数据
	enum { IDD = IDD_FORMTOOLBOX };

public:
    TCTOOLTYPE m_SelectToolType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
    CButtonSSL m_btnDefault;
    CButtonSSL m_btnStatic;
    CButtonSSL m_btnBtn;
    CButtonSSL m_btnRadio;
    CButtonSSL m_btnCheck;
    CButtonSSL m_btnEdit;
    CButtonSSL m_btnGroup;
    CButtonSSL m_btnDate;
    CButtonSSL m_btnProgress;
    CButtonSSL m_btnPicture;
    CButtonSSL m_btnComboBox;
    CButtonSSL m_btnListbox;
    CButtonSSL m_btnSlider;
    CButtonSSL m_btnPath;
	CButtonSSL m_btnTab;
	CButtonSSL m_btnTimer;
	CButtonSSL m_btnHotkey;
	CButtonSSL m_btnExplorer;
	CButtonSSL m_btnGrid;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnBnClickedButtonstatic();
    afx_msg void OnBnClickedButtondefault();
    afx_msg void OnBnClickedButtonbtn();
    afx_msg void OnBnClickedButtonradio();
    afx_msg void OnBnClickedButtoncheck();
    afx_msg void OnBnClickedButtonedti();
    afx_msg void OnBnClickedButtongroup();
    afx_msg void OnBnClickedButtondate();
    afx_msg void OnBnClickedButtonprogress();
    afx_msg void OnBnClickedButtonpicture();
    afx_msg void OnBnClickedButtoncombobox();
    afx_msg void OnBnClickedButtonlistbox();
    afx_msg void OnBnClickedButtonslider();
    afx_msg void OnBnClickedButtonpath();
	afx_msg void OnBnClickedButtontab();
	afx_msg void OnBnClickedButtontimer();
	afx_msg void OnBnClickedButtonhotkey();
	afx_msg void OnBnClickedButtonexplorer();
	afx_msg void OnBnClickedButtongridctrl();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
