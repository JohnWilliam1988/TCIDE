#ifndef _CONTROLRECT_H
#define _CONTROLRECT_H

#include "TCRunTimeManager.h"

class CControlRect : public CRectTracker
{
	friend class CControlTracker;
public:
    //Contructors
    CControlRect( int x1, int y1, int x2, int y2 ,CWnd *cwnd,TCTOOLTYPE type,CString id);
    CControlRect( const CRect & rect,CWnd *cwnd,TCTOOLTYPE type,CString id,TCPARENTTYPE parentwndType,CWnd *parent);
    virtual ~CControlRect();
    // Tracks movement rectangle for control
	void Track( CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE, CWnd* pWndClipTo = NULL );

    // Sets bounding rectangle for control
	void SetRect( int x1, int y1, int x2, int y2 , bool isupdate = true);
	void SetRect( const CRect & rect , bool isupdate = true);
	void MyDraw(CDC* pDC,CPen* pPen,BOOL locked = FALSE) ;
	void DrawLocked(CDC* pDC);
    CWnd *GetCWnd() const;
    CString GetCtlID();
    void SetCtlID(CString id);
    void TCMoveWindos( const CRect & rect );
    void TCMoveWindos(int x, int y, int nWidth, int nHeight);

	void TCOnSizeCtrlCommand();

	void TCUpdateProperty();
    TCTOOLTYPE GetCtlType();
	TCPARENTTYPE GetParentType();
	//得到基准选框 
	bool TCGetHighSelected();
private:
    //virtual function called to get handle masks
	UINT GetHandleMask() const;
   //Sets default attributes for CRectTracker
	void Initialize();

	BOOL m_bSelected;                         //TRUE if selected
	BOOL m_highLight;                         //记录当前控件是否是基准控件
	BOOL m_bLockedUp;						  //当前控件是否被锁定
    CWnd *m_ctlWnd;                           //记录控件的句柄
    TCTOOLTYPE m_ToolType;                    //记录控件的类型
    CString m_id;
	
	CWnd *m_ctrlParent;
public:
	TCPARENTTYPE m_parentWndType;			  //父窗口的类型,即TabPage或者DiaTCDesign
};

#endif 
