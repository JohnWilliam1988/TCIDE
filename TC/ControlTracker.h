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
    //将控件添加到Tracker处理
    void Add(CControlRect* pObject );

	void JustAdd( CControlRect* pObject );

	//9-16添加  
	//在创建控件的时候就将当前控件置为选中状态
	void SetCreateCtrlSelected(CControlRect* pObject );
    //Tells weather the specified control is selected or not
    BOOL IsSelected( CControlRect* pObject ) const;

    //判断是否是高亮的基准控件
    BOOL IsBasicElement( CControlRect* pObject ) const;

    //Selects the specified control. Returns TRUE if it was initially unselected else FALSE
    BOOL Select( CControlRect* pObject );

    //DeSelects the specified control. Returns TRUE if it was initially selected else FALSE
    BOOL DeSelect( CControlRect* pObject );

	//锁定
	void Lock( CControlRect* pObject );

	//解锁
	void UnLock( CControlRect* pObject );

	//从列表中移除删除掉的控件
	BOOL RemoveFromAll( CWnd* ctl);

    //Toggles the state of the object, TRUE: control was selected, else FALSE
    BOOL Toggle( CControlRect* pObject );

    //高亮状态
    BOOL IsCotainHighLight();

    //Draws all the controls
    void Draw( CDC* pDC ) const;

    //Starts tracking the controls
    BOOL Track( const CPoint & point, UINT nFlags = 0, BOOL bTrackRubberBand = TRUE );

    //选中控件左移一个位置
    BOOL MoveLeftOne();

    //选中控件左移一个位置
    BOOL MoveRightOne();

    //选中控件上移一个位置
    BOOL MoveTopOne();

    //选中控件上移一个位置
    BOOL MoveBottomOne();

    //DeSelects all the controls, returns the number of controls that were selected
    int DeSelectAll();
   
    //Creates the Control Tracker Object
    void Create( CWnd* pParentWnd );

	//按点击规则插入控件
	void Insert(CControlRect* pObject);
    
    //Array of all items
    CArray< CControlRect*, CControlRect* > m_arrRectanglesAll;
    
    //Array of currently selected items
    CArray< CControlRect*, CControlRect* > m_arrRectanglesSelected;

	//用来记录复制控件信息
	CArray< CControlRect*, CControlRect* > m_arrRectanglesCopy;
   
    //这个是用来记录哪个是基准控件
    CControlRect* m_BasicElement;

    //这个是判断是否包含了此控件,并返回其位置
    int IsIncludeControl(CWnd* ctl);

    //这个是判断在选中的控件中是否包含了此控件,并返回其位置
    int IsInSelectControl(CWnd* ctl);

    //Gets Bounding rect for multiply selected objects
    CRect GetBoundsRect() const;

    //Gets Bounding rect for multiply selected objects
    void GetBoundsPointCur(POINT clientPoint,LPPOINT lpPoint);

    //判断ID是否存在
    bool IsExistID(CString id);

    //生成一个有效的ID名
    CString BuildValidCtlID(TCTOOLTYPE type);

    //根据ID得到控件句柄
    CWnd* GetCtlByID(CString id);

    //根据ID得到控件橡皮筋
    CControlRect* GetRectByID(CString id);

    CControlRect * GetDisplayPropCtl();

    //改变控件ID
    bool ChangeCtlID(CString oldID,CString newID);

    void Update();
//功能
public:             
    //对话框垂直局中
    void CenterWindowV();
    //对话框水平居中
    void CenterWindowH();
    //左对齐
    void AlignLeft();
    //右对齐
    void AlignRight();
    //低端对齐
    void AlignBottom();
    //顶端对齐
    void AlignTop();
    //设置同样的高和宽
    void SetSameSize();
    //设置同样的高
    void SetSameHeight();
    //设置同样的宽
    void SetSameWidth();
    //水平居中对齐
    void CenterAlignH();
    //垂直居中对齐
    void CenterAlignV();
    //均匀水平间隔
    void UniformSpaceH();
    //均匀垂直间隔
    void UniformSpaceV();

	//创建花选框时默认基准控件
    BOOL CreateDefaultBasicCtrl();

	//设置Z序接口 
	void TCSetCurrentCtrlZorder(int pos,CWnd* pCtrl);
	int TCGetControlTrackerSize(CWnd* pCtrl);

private:
    //Corrects the z-orders of all the controls
    void CorrectZOrders( CWnd* pWndInsertAfter );

	 //TC自定义Z序排列
    void TCCorrectZOrders();

    //Selects a control unconditionally
    void SelectUnConditional( CControlRect* pObject );	
    //高亮选中操作方法
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
