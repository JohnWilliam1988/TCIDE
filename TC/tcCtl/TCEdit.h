/*===========================================================================
====                                                                     ====
====    File name           :  EditTrans.h                               ====
====    Creation date       :  7/10/2001                                 ====
====    Author(s)           :  Dany Cantin                               ====
====                                                                     ====
===========================================================================*/

#ifndef EDITTRANS_H
#define EDITTRANS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CTCEdit window

#define TRANS_BACK -1

class CTCEdit : public CEdit
{
// Construction
public:
	CTCEdit();

// Attributes
private:
    COLORREF m_TextColor;
    COLORREF m_BackColor;
    CBrush   m_Brush;

    //这里是记录两个事件值
    CString  m_id;
    CString  m_eEnChange;
	CString	 m_eSetFocus;
    CString  m_eKillfocus;

    bool     m_isPassword;	//密文
	bool	 m_isAbled;		//有效
	bool	 m_isReadOnly;	//只读
	bool	 m_isNumber;	//数字
	bool	 m_isMulityLine;//多行
	bool	 m_Verti;      //垂直滚动条
	bool     m_Horiz;      //水平滚动条

// Operations
public:
    void SetTextColor(COLORREF col) { m_TextColor = col;
                                      UpdateCtrl();      }
    COLORREF GetTextColor()			{ return m_TextColor; }
    void SetBackColor(COLORREF col) { m_BackColor = col;
                                      UpdateCtrl();      }
    COLORREF GetBackColor()			{ return m_BackColor; }
    void SetCtlID(CString id)		{ m_id = id; }                              
     
	void TCSetAbled(bool set);
	void TCSetMulityLine(bool set);
    void TCSetPassWord(bool password);
	void TCSetReadOnly(bool set);
	void TCSetNumber(bool set);
	void TCSetVertiScroll(bool set);
	void TCSetHorizScroll(bool set);

	void TCSetEnChangeEvent(CString csEvent);
	void TCSetFocusEvent(CString csEvent);
	void TCSetKillfocusEvent(CString csEvent); 

	CString TCGetEnChangeEvent();
	CString TCGetSetFocusEvent();
	CString TCGetKillfocusEvent();

	CString GetCtlID()		{	return m_id;			}
	bool TCGetMulityLine()	{	return m_isMulityLine;	}
	bool TCGetReadOnly()	{	return m_isReadOnly;	}
	bool TCGetAbled()		{	return m_isAbled;		}
	bool TCGetVerti()		{	return m_Verti;			}
	bool TCGetHoriz()		{	return m_Horiz;			}
	bool TCGetPassWord()	{	return m_isPassword;	}
	bool TCGetNumber()		{	return m_isNumber;		}
private:
    void UpdateCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTCEdit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions
protected:
	//{{AFX_MSG(CTCEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillfocus();
    afx_msg void OnEnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif // EDITTRANS_H
