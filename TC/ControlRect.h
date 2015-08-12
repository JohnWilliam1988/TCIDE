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
	//�õ���׼ѡ�� 
	bool TCGetHighSelected();
private:
    //virtual function called to get handle masks
	UINT GetHandleMask() const;
   //Sets default attributes for CRectTracker
	void Initialize();

	BOOL m_bSelected;                         //TRUE if selected
	BOOL m_highLight;                         //��¼��ǰ�ؼ��Ƿ��ǻ�׼�ؼ�
	BOOL m_bLockedUp;						  //��ǰ�ؼ��Ƿ�����
    CWnd *m_ctlWnd;                           //��¼�ؼ��ľ��
    TCTOOLTYPE m_ToolType;                    //��¼�ؼ�������
    CString m_id;
	
	CWnd *m_ctrlParent;
public:
	TCPARENTTYPE m_parentWndType;			  //�����ڵ�����,��TabPage����DiaTCDesign
};

#endif 
