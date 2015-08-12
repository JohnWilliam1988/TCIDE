#ifndef _CONTROLTRACKER_H
#define _CONTROLTRACKER_H

#include "RefObject.h"
#include "TCRunTimeManager.h"

class CControlRect;

class CControlTracker : public CRectTracker
{
public:
    CControlTracker();
    virtual ~CControlTracker();

    //Sets the cursor on all the controls
    BOOL SetCursor(UINT nHitTest, UINT message );

    //Adds the Control to the Tracker
    //���ؼ���ӵ�Tracker����
    void Add(CControlRect* pObject );

	void JustAdd( CControlRect* pObject );

	//9-16���  
	//�ڴ����ؼ���ʱ��ͽ���ǰ�ؼ���Ϊѡ��״̬
	void SetCreateCtrlSelected(CControlRect* pObject );
    //Tells weather the specified control is selected or not
    BOOL IsSelected( CControlRect* pObject ) const;

    //�ж��Ƿ��Ǹ����Ļ�׼�ؼ�
    BOOL IsBasicElement( CControlRect* pObject ) const;

    //Selects the specified control. Returns TRUE if it was initially unselected else FALSE
    BOOL Select( CControlRect* pObject );

    //DeSelects the specified control. Returns TRUE if it was initially selected else FALSE
    BOOL DeSelect( CControlRect* pObject );

	//����
	void Lock( CControlRect* pObject );

	//����
	void UnLock( CControlRect* pObject );

	//���б����Ƴ�ɾ�����Ŀؼ�
	BOOL RemoveFromAll( CWnd* ctl);

    //Toggles the state of the object, TRUE: control was selected, else FALSE
    BOOL Toggle( CControlRect* pObject );

    //����״̬
    BOOL IsCotainHighLight();

    //Draws all the controls
    void Draw( CDC* pDC ) const;

    //Starts tracking the controls
    BOOL Track( const CPoint & point, UINT nFlags = 0, BOOL bTrackRubberBand = TRUE );

    //ѡ�пؼ�����һ��λ��
    BOOL MoveLeftOne();

    //ѡ�пؼ�����һ��λ��
    BOOL MoveRightOne();

    //ѡ�пؼ�����һ��λ��
    BOOL MoveTopOne();

    //ѡ�пؼ�����һ��λ��
    BOOL MoveBottomOne();

    //DeSelects all the controls, returns the number of controls that were selected
    int DeSelectAll();
   
    //Creates the Control Tracker Object
    void Create( CWnd* pParentWnd );

	//������������ؼ�
	void Insert(CControlRect* pObject);
    
    //Array of all items
    CArray< CControlRect*, CControlRect* > m_arrRectanglesAll;
    
    //Array of currently selected items
    CArray< CControlRect*, CControlRect* > m_arrRectanglesSelected;

	//������¼���ƿؼ���Ϣ
	CArray< CControlRect*, CControlRect* > m_arrRectanglesCopy;
   
    //�����������¼�ĸ��ǻ�׼�ؼ�
    CControlRect* m_BasicElement;

    //������ж��Ƿ�����˴˿ؼ�,��������λ��
    int IsIncludeControl(CWnd* ctl);

    //������ж���ѡ�еĿؼ����Ƿ�����˴˿ؼ�,��������λ��
    int IsInSelectControl(CWnd* ctl);

    //Gets Bounding rect for multiply selected objects
    CRect GetBoundsRect() const;

    //Gets Bounding rect for multiply selected objects
    void GetBoundsPointCur(POINT clientPoint,LPPOINT lpPoint);

    //�ж�ID�Ƿ����
    bool IsExistID(CString id);

    //����һ����Ч��ID��
    CString BuildValidCtlID(TCTOOLTYPE type);

    //����ID�õ��ؼ����
    CWnd* GetCtlByID(CString id);

    //����ID�õ��ؼ���Ƥ��
    CControlRect* GetRectByID(CString id);

    CControlRect * GetDisplayPropCtl();

    //�ı�ؼ�ID
    bool ChangeCtlID(CString oldID,CString newID);

    void Update();
//����
public:             
    //�Ի���ֱ����
    void CenterWindowV();
    //�Ի���ˮƽ����
    void CenterWindowH();
    //�����
    void AlignLeft();
    //�Ҷ���
    void AlignRight();
    //�Ͷ˶���
    void AlignBottom();
    //���˶���
    void AlignTop();
    //����ͬ���ĸߺͿ�
    void SetSameSize();
    //����ͬ���ĸ�
    void SetSameHeight();
    //����ͬ���Ŀ�
    void SetSameWidth();
    //ˮƽ���ж���
    void CenterAlignH();
    //��ֱ���ж���
    void CenterAlignV();
    //����ˮƽ���
    void UniformSpaceH();
    //���ȴ�ֱ���
    void UniformSpaceV();

	//������ѡ��ʱĬ�ϻ�׼�ؼ�
    BOOL CreateDefaultBasicCtrl();

	//����Z��ӿ� 
	void TCSetCurrentCtrlZorder(int pos,CWnd* pCtrl);
	int TCGetControlTrackerSize(CWnd* pCtrl);

private:
    //Corrects the z-orders of all the controls
    void CorrectZOrders( CWnd* pWndInsertAfter );

	 //TC�Զ���Z������
    void TCCorrectZOrders();

    //Selects a control unconditionally
    void SelectUnConditional( CControlRect* pObject );	
    //����ѡ�в�������
    //void SelectWithHighLight( CControlRect* pObject );
    
    //Parent window
    CWnd* m_pwndParent;

	TCPARENTTYPE m_parentWndType;

public:
	void SetParentWndType(TCPARENTTYPE wndType){
			m_parentWndType = wndType;
	}

};

#endif
