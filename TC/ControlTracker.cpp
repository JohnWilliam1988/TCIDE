#include "stdafx.h"
#include "ControlTracker.h"
#include "ControlRect.h"
#include "MainFrm.h"
#include "TCRunTimeManager.h"
#include "TCTabPage.h"
#include "Docommand.h"

CControlTracker::CControlTracker() : m_pwndParent( NULL )
{
    m_BasicElement = NULL;
}

CControlTracker::~CControlTracker()
{

}

void CControlTracker::Create(CWnd* pWnd)
{
	//������Ի���Tab����Redo��ʱ�򱨴�
    //ASSERT( m_pwndParent == NULL );
    m_pwndParent = pWnd;
}
void CControlTracker::Add( CControlRect* pObject )
{
	//����ؼ�����Ƥ����
	Insert(pObject);
	//�¿ؼ��ö�
	//CorrectZOrders(pObject->GetCWnd());
	TCCorrectZOrders();
}

void CControlTracker::JustAdd( CControlRect* pObject )
{
	m_arrRectanglesAll.Add(pObject);
}

void CControlTracker::Insert(CControlRect* pObject)
{
	//�޿ؼ�ʱ,����Ҫ����
	if (m_arrRectanglesAll.GetCount() == 0)
	{
		m_arrRectanglesAll.SetAtGrow(0, pObject);
		return;
	}

	for (int i = m_arrRectanglesAll.GetCount(); i > 0; i--)
	m_arrRectanglesAll.SetAtGrow(i, m_arrRectanglesAll[i - 1]);
	//�����¼ӵĿؼ�
	m_arrRectanglesAll.SetAtGrow(0, pObject);
}

/////////////////////////////////////////////////////////////////////////////////////////
//������ӵĿؼ��ö�
void CControlTracker::CorrectZOrders( CWnd* pWndInsertAfter )
{	
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )	
    {
        m_arrRectanglesAll.GetAt( i )->GetCWnd()->SetWindowPos( pWndInsertAfter , 0,0,0,0, SWP_NOMOVE   | SWP_NOSIZE   | SWP_SHOWWINDOW   );
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//TC�Զ���Z������
void CControlTracker::TCCorrectZOrders()
{	
	//�ؼ�����
	int count = m_arrRectanglesAll.GetCount();
	//�������ؼ�,����ǰ���ӵķ�ʽ,���뵽���һ���ؼ�z��ĺ���
	for (int i = count - 2; i >= 0; i--)
		m_arrRectanglesAll[i]->GetCWnd()->SetWindowPos(m_arrRectanglesAll[count - 1]->GetCWnd(),
			NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
	//�����һ���ؼ��ŵ�z�����.����,���пؼ�ͨ����ǰ���ӵķ�ʽ,z�����ź�
	if (count > 1)
		m_arrRectanglesAll[count - 1]->GetCWnd()->SetWindowPos(m_arrRectanglesAll[count - 2]->GetCWnd(),
			NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);
}

void CControlTracker::TCSetCurrentCtrlZorder(int pos,CWnd* pCtrl)
{

	//ԭ����ǰ�ؼ���λ��
	int oldPos = 0,newPos = 0;
	CControlRect* pTemp = NULL;
	for(int i=0;i<m_arrRectanglesAll.GetSize();i++)
	{
		if(m_arrRectanglesAll[i]->GetCtlType() == TCTT_TAB)
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==pCtrl->m_hWnd)
			{  
				oldPos = i;
				newPos = m_arrRectanglesAll.GetSize() - pos;
				pTemp = m_arrRectanglesAll[i];
				//��ǰ��λ��
				//���ƶ�Ҫ�������еĿؼ�
				if(oldPos < newPos)
				{
					for(int loop=0;loop<newPos-oldPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos+loop,m_arrRectanglesAll[oldPos+loop+1]);
					}
					//����ǰ�ؼ��ƶ�����λ��
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}
				else if(oldPos > newPos)
				{
					for(int loop=0;loop<oldPos-newPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos-loop,m_arrRectanglesAll[oldPos-loop-1]);
					}
					//����ǰ�ؼ��ƶ�����λ��
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}		
			}
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd()==pCtrl)
						{
							oldPos = j;
							newPos = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize() - pos;
							pTemp = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j];
							//��ǰ��λ��
							//���ƶ�Ҫ�������еĿؼ�
							if(oldPos < newPos)
							{
								for(int loop=0;loop<newPos-oldPos;loop++)
								{
									((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAt(oldPos+loop,((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[oldPos+loop+1]);
								}
								//����ǰ�ؼ��ƶ�����λ��
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.TCCorrectZOrders();
								return;
							}
							else if(oldPos > newPos)
							{
								for(int loop=0;loop<oldPos-newPos;loop++)
								{
									((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAt(oldPos-loop,((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[oldPos-loop-1]);
								}
								//����ǰ�ؼ��ƶ�����λ��
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
								((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.TCCorrectZOrders();
								return;
							}		
						}
					}
				}
			}
		}
		else
		{
			if(m_arrRectanglesAll[i]->GetCWnd() == pCtrl)
			{
				oldPos = i;
				newPos = m_arrRectanglesAll.GetSize() - pos;
				pTemp = m_arrRectanglesAll[i];
				//��ǰ��λ��
				//���ƶ�Ҫ�������еĿؼ�
				if(oldPos < newPos)
				{
					for(int loop=0;loop<newPos-oldPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos+loop,m_arrRectanglesAll[oldPos+loop+1]);
					}
					//����ǰ�ؼ��ƶ�����λ��
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}
				else if(oldPos > newPos)
				{
					for(int loop=0;loop<oldPos-newPos;loop++)
					{
						m_arrRectanglesAll.SetAt(oldPos-loop,m_arrRectanglesAll[oldPos-loop-1]);
					}
					//����ǰ�ؼ��ƶ�����λ��
					m_arrRectanglesAll.SetAtGrow(newPos,pTemp);
					TCCorrectZOrders();
					return;
				}		
			}
		}
	}
}

int CControlTracker::TCGetControlTrackerSize(CWnd* pCtrl)
{
	//���ж�Tab��
	for(int i=0;i<m_arrRectanglesAll.GetSize();i++)
	{
		if (m_arrRectanglesAll[i]->GetCtlType() == TCTT_TAB)
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==pCtrl->m_hWnd)
				return m_arrRectanglesAll.GetSize();
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd()==pCtrl)
							return ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					}
				}
			}
		}
		else
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==pCtrl->m_hWnd)
				return m_arrRectanglesAll.GetSize();
		}
	}
}

////////////////////////////��ǰ�����Ŀؼ���Ϊѡ��״̬/////////////////////////////
void CControlTracker::SetCreateCtrlSelected( CControlRect* pObject)
{
	//���Ƚ���ǰѡ�е�DeSelect��
	DeSelectAll();
	m_arrRectanglesSelected.Add( pObject );
	pObject->m_bSelected = TRUE;
	m_pwndParent->Invalidate();
}
////////////////////////////��ǰ�����Ŀؼ���Ϊѡ��״̬/////////////////////////////
BOOL CControlTracker::IsSelected( CControlRect* pObject ) const
{
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {
        if( m_arrRectanglesSelected[i] == pObject )
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CControlTracker::IsBasicElement( CControlRect* pObject ) const
{
    if( m_BasicElement != NULL && m_BasicElement == pObject)
    {
        return TRUE;
    }

    return FALSE;
}

void CControlTracker::SelectUnConditional( CControlRect* pObject )
{
    m_arrRectanglesSelected.Add( pObject );
    pObject->m_bSelected = TRUE;
    //////�����������ӻ���ѡ�к�Ļ�׼�ؼ���
}

//////////////////////������ʾ����ز���/////////////////////////////
//void CControlTracker::SelectWithHighLight( CControlRect* pObject )
//{
//    m_BasicElement = pObject;
//    pObject->m_highLight = TRUE;
//}

//////////////////////������ʾ����ز���/////////////////////////////
BOOL CControlTracker::IsCotainHighLight()
{
    for(int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {
        if(m_arrRectanglesSelected[i]->m_highLight && m_arrRectanglesSelected[i]->m_bSelected )
        { 	
            return TRUE;
        }
    }
    return FALSE;
}
///////////////////������ʾ����ز���//////////////////////////////


///////////////////����õ�Ĭ�ϻ�׼�ؼ�////////////////////////////
BOOL CControlTracker::CreateDefaultBasicCtrl()
{
    int size = m_arrRectanglesSelected.GetSize();
  
	if(size < 1)
		return FALSE;

	else if(size == 1)
		m_arrRectanglesSelected.GetAt( 0 )->TCUpdateProperty();

	else
	{
		m_arrRectanglesSelected.GetAt( 0 )->m_highLight = TRUE;
		m_BasicElement = m_arrRectanglesSelected.GetAt( 0 );
		m_BasicElement->TCUpdateProperty();
	}

    this->m_pwndParent->Invalidate();
    return TRUE;
}
///////////////////����õ�Ĭ�ϻ�׼�ؼ�////////////////////////////

BOOL CControlTracker::Select( CControlRect* pObject )	
{
    if( ! IsSelected( pObject ) )
    {
        SelectUnConditional( pObject );

        return TRUE;
    }

    return FALSE;
}


//����
void CControlTracker::Lock( CControlRect* pObject )
{
	if( pObject != NULL )
		pObject->m_bLockedUp = TRUE;
}

//����
void CControlTracker::UnLock( CControlRect* pObject )
{
	if( pObject != NULL )
		pObject->m_bLockedUp = FALSE;
}

BOOL CControlTracker::Toggle( CControlRect* pObject )
{
    if( IsSelected( pObject )&&IsCotainHighLight())
    {
        ///��ѡ
        //DeSelect( pObject );
        for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
        {
            m_arrRectanglesSelected[i]->m_highLight = FALSE;
        }
        pObject->m_bSelected = TRUE;
        pObject->m_highLight = TRUE;
        m_BasicElement = pObject;
		m_BasicElement->TCUpdateProperty();
        return TRUE;
    }
    else if( IsSelected( pObject )&&!IsCotainHighLight() )
    {
        pObject->m_bSelected = TRUE;
        pObject->m_highLight = TRUE;
        m_BasicElement = pObject;
		m_BasicElement->TCUpdateProperty();
        return TRUE;
    }
    else
    {
        //ѡ��
        Select(pObject);
        if(!IsCotainHighLight())
        {
            pObject->m_bSelected = TRUE;
            pObject->m_highLight = TRUE;
            m_BasicElement = pObject;
			m_BasicElement->TCUpdateProperty();
        }
        return TRUE;
    }

}

BOOL CControlTracker::DeSelect( CControlRect* pObject )
{
    //��ʱ����
    CArray< CControlRect*, CControlRect* > TempArray;

    if( IsSelected( pObject ) )
    {
        for(int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
        {
            if( m_arrRectanglesSelected[i] == pObject )
            { 				
                pObject->m_bSelected = FALSE;
                pObject->m_highLight = FALSE;
                if(m_BasicElement ==pObject)
                    m_BasicElement=NULL;
				//return TRUE;
            }
			//����ע�͵�,����Ϊѡ�е�״̬���л�����,��Ӱ�쵽ɾ���ؼ��Ĳ���
            else
            {
                TempArray.Add( m_arrRectanglesSelected[i] );
            }
			//����ע�͵�,����Ϊѡ�е�״̬���л�����,��Ӱ�쵽ɾ���ؼ��Ĳ���
        }

        m_arrRectanglesSelected.RemoveAll();

        for(int i=0; i < TempArray.GetSize(); i++ )
        {
            m_arrRectanglesSelected.Add( TempArray[i] );
        }

        return ( ! pObject->m_bSelected );
    }

    return FALSE;
}


//////////////////////////////////////////////////////////////
BOOL CControlTracker::RemoveFromAll(CWnd* ctl)
{
		int index = IsIncludeControl(ctl);
		if( index == -1 )
			return FALSE;
		else
		{
			m_arrRectanglesAll.RemoveAt(index);
			return TRUE;
		}
}

int CControlTracker::DeSelectAll()
{
    int UpperBound = m_arrRectanglesSelected.GetSize();

    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {
        m_arrRectanglesAll[i]->m_bSelected = FALSE;
        m_arrRectanglesAll[i]->m_highLight = FALSE;
    }

    m_arrRectanglesSelected.RemoveAll();
    m_BasicElement = NULL;
    return UpperBound;
}

BOOL CControlTracker::Track( const CPoint & point, UINT nFlags , BOOL bTrackRubberBand )
{
    ASSERT( m_pwndParent != NULL );

    int InitialCount = 0;
    CRect InitialBoundRect;
    int pX, pY;

    //��������Ԫ��Ȼ���ƶ�
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {				
        int nHit = m_arrRectanglesAll[i]->HitTest( point );
        //û���϶��ؼ���С
        if( nHit != CRectTracker::hitNothing  )
        {
            // See if the control key was down
            //�����ܼ��Ƿ���
            if( ( nFlags & MK_CONTROL ) != 0 )
            {
                //Toggle���л��ؼ���ѡ��״̬��
                Toggle( m_arrRectanglesAll[i] );
                m_pwndParent->Invalidate();
                return TRUE;						
            }						
            // See if the shift key was down
            if( ( nFlags & MK_SHIFT ) != 0 )
            {
                Toggle( m_arrRectanglesAll[i] );
                m_pwndParent->Invalidate();
                return TRUE;
            }

            if( ! IsSelected( m_arrRectanglesAll[i] ) ) 
                InitialCount = DeSelectAll();

            Select( m_arrRectanglesAll[i] );

            if( nHit == CRectTracker::hitMiddle )
            {		
						
                InitialBoundRect = GetBoundsRect();

				//��������
				for(int count = 0;count < m_arrRectanglesSelected.GetCount(); count++)
				{
					if( m_arrRectanglesSelected[count]->m_bLockedUp )
					{
						m_pwndParent->Invalidate();
						return TRUE;
					}
				}

                m_rect.CopyRect( & InitialBoundRect );
                CRectTracker::Track( m_pwndParent, point );

                pX = InitialBoundRect.left - m_rect.left;
                pY = InitialBoundRect.top - m_rect.top;
		
                // Offset each selected control

				//��������ÿ���ؼ���λ��
				//����û���ƶ�λ�þ���ӳ����ָ�����
				if( m_rect != InitialBoundRect )
				{
					SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
					pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());
					for( int j = 0;j < m_arrRectanglesSelected.GetSize();j++ )
					{
						CRect rect(m_arrRectanglesSelected[j]->m_rect.left - pX, 
							m_arrRectanglesSelected[j]->m_rect.top - pY,
							m_arrRectanglesSelected[j]->m_rect.right - pX,
							m_arrRectanglesSelected[j]->m_rect.bottom - pY);
						pCommand->m_TCCtrlSort[j].SetTCCtrl(m_arrRectanglesSelected.GetAt( j )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( j )->GetCtlType());
					}
					switch(m_parentWndType)
					{
					case TCPARENT_DESDIN:
						((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
						break;
					case TCPARENT_TABPAGE:
						((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
						break;
					default:
						break;
					}
				}
				else
				{
					int curselect = m_arrRectanglesSelected.GetCount();
					if(curselect>1)
					{
						for(int k=0;k<curselect;k++)
						{
							if(m_arrRectanglesSelected[k]->TCGetHighSelected())
								m_arrRectanglesSelected[k]->TCUpdateProperty();
						}
					}
					else if(curselect==1)
						m_arrRectanglesSelected[0]->TCUpdateProperty();
					m_pwndParent->Invalidate();
					return TRUE;
				}
			}
            else
            {
				//��������
				if( m_arrRectanglesAll[i]->m_bLockedUp )
					return TRUE;
                m_arrRectanglesAll[i]->Track(m_pwndParent,point);				
            }
            m_pwndParent->Invalidate();
            return TRUE;
        }
    }	
    // Exit if the shift key or the control key was down
    //���¹��ܼ����˳�
    if( ( ( nFlags & MK_SHIFT ) != 0 ) || ( ( nFlags & MK_CONTROL ) != 0 ) )
    {
        return FALSE;
    }

    // Deselect all the controls
    //ȥ��ѡ�е����пؼ�
    DeSelectAll();

    if( bTrackRubberBand == FALSE )
    {
        m_pwndParent->Invalidate( WM_PAINT );
        return FALSE;
    }

    // Start the selection rect
    //�����ʼ��ѡȡ��
    TrackRubberBand( m_pwndParent, point );
    //Rect�ĵ�ǰ��С����ÿһ���϶���Ҫ���滯
    m_rect.NormalizeRect();

    CRect temp;
    // Now select objects according to the resized rectangle
    /////���ݻ����ķ�Χ���Ƿ�ѡ�пؼ�::���ǽ���
    for(int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {
        temp.IntersectRect( & m_rect, & m_arrRectanglesAll[i]->m_rect );

        if( m_arrRectanglesAll[i]->m_rect.EqualRect( & temp ) )
        {
            Select(m_arrRectanglesAll[i] );	
			//����ǽ���Ĭ�ϻ�׼�ؼ��Ĳ���
			//CreateDefaultBasicCtrl(false);
        }
    }
	//����ǽ���Ĭ�ϻ�׼�ؼ��Ĳ���
	CreateDefaultBasicCtrl();
    //ˢ��
    m_pwndParent->Invalidate();
    return FALSE;
}

//ѡ�пؼ�����һ��λ��
BOOL CControlTracker::MoveLeftOne( )
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
    //m_pwndParent->ClientToScreen(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());

    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //��������ù��
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.left-1<parentRect.left)
        {
            ctlRect.left=parentRect.left;
            ctlRect.right=ctlRect.left+ctlRect.Width();
        }
        else
        {
            ctlRect.left=ctlRect.left-1;
            ctlRect.right=ctlRect.right-1;
        }
		//�ڶ�ѡ�����,ֻˢ�»�׼�ؼ�
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());

    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
    //m_pwndParent->Invalidate();
    return TRUE;
}

BOOL CControlTracker::MoveRightOne()
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
    //m_pwndParent->ClientToScreen(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());

    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //��������ù��
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.right+1>parentRect.right)
        {
            ctlRect.left=parentRect.right-ctlRect.Width();
            ctlRect.right=parentRect.right;
        }
        else
        {
            ctlRect.left=ctlRect.left+1;
            ctlRect.right=ctlRect.right+1;
        }
		//�ڶ�ѡ�����,ֻˢ�»�׼�ؼ�
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());

    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}

    //m_pwndParent->Invalidate();

    return TRUE;
}

BOOL CControlTracker::MoveTopOne()
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
    //m_pwndParent->ClientToScreen(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //��������ù��
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.top-1<parentRect.top)
        {
            ctlRect.top=parentRect.top;
            ctlRect.bottom=ctlRect.top+ctlRect.Height();
        }
        else
        {
            ctlRect.top=ctlRect.top-1;
            ctlRect.bottom=ctlRect.bottom-1;
        }
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());

    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
    return TRUE;
}

BOOL CControlTracker::MoveBottomOne()
{
    CRect parentRect;
    m_pwndParent->GetClientRect(&parentRect);
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(m_arrRectanglesSelected.GetSize());
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        CRect ctlRect;
        //��������ù��
        m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&ctlRect);
        m_pwndParent->ScreenToClient(&ctlRect);
        if(ctlRect.bottom+1>parentRect.bottom)
        {
            ctlRect.top=parentRect.bottom-ctlRect.Height();
            ctlRect.bottom=parentRect.bottom;
        }
        else
        {
            ctlRect.top=ctlRect.top+1;
            ctlRect.bottom=ctlRect.bottom+1;
        }
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),ctlRect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
		
    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
    return TRUE;
}

CRect CControlTracker::GetBoundsRect() const
{
    int Left(0), Right(0), Top(0), Bottom(0);
    int pLeft(0), pRight(0), pTop(0), pBottom(0);

    pLeft = m_arrRectanglesSelected[0]->m_rect.left;
    pTop = m_arrRectanglesSelected[0]->m_rect.top;
    pRight = m_arrRectanglesSelected[0]->m_rect.right;
    pBottom = m_arrRectanglesSelected[0]->m_rect.bottom;

    for( int i = 1; i < m_arrRectanglesSelected.GetSize(); i++ )
    {
        Left = m_arrRectanglesSelected[ i ]->m_rect.left;
        Top = m_arrRectanglesSelected[ i ]->m_rect.top;
        Right = m_arrRectanglesSelected[ i ]->m_rect.right;
        Bottom = m_arrRectanglesSelected[ i ]->m_rect.bottom;

        if( Left < pLeft )
            pLeft = Left;

        if( Top < pTop )
            pTop = Top;

        if( Bottom > pBottom )
            pBottom = Bottom;

        if( Right > pRight )
            pRight = Right;
    }

    return CRect( pLeft, pTop, pRight, pBottom );
}

void CControlTracker::GetBoundsPointCur(POINT clientPoint,LPPOINT lpPoint)
{
    CRect BoundRect=GetBoundsRect();
    lpPoint->x=clientPoint.x-BoundRect.left;
    lpPoint->y=clientPoint.y-BoundRect.top;
    return;
}

void CControlTracker::Draw( CDC* pDC ) const
{
    // Loop can iterate through all elements of the selected array
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {		
		if( m_arrRectanglesAll[i]->m_bSelected )
		{
			if(m_arrRectanglesAll[i]->m_highLight)
			{
				if(m_arrRectanglesAll[i]->m_bLockedUp)
				{
					//������ؼ���ѡ����Ϊ��׼�ؼ������Ƥ�� 
					CPen pen(PS_SOLID,1,RGB(255,0,0)); 
					m_arrRectanglesAll[i]->MyDraw(pDC,&pen,TRUE);
				}
				else
				{
					//������ؼ���ѡ����Ϊ��׼�ؼ������Ƥ�� 
					CPen pen(PS_SOLID,1,RGB(255,0,0)); 
					m_arrRectanglesAll[i]->MyDraw(pDC,&pen);
				}
			}
			else/* if(m_arrRectanglesAll[i]->m_highLight == FALSE)*/
			{
				if(m_arrRectanglesAll[i]->m_bLockedUp)
				{
					//������ؼ���ѡ����Ϊ��׼�ؼ������Ƥ�� 
					//CPen pen(PS_DOT,1,RGB(255,255,128)); 
 					//m_arrRectanglesAll[i]->MyDraw(pDC,&pen);
					m_arrRectanglesAll[i]->DrawLocked(pDC);
				}
				else
				{
					//���ֻ�ǻ�����ͨѡ�к�Ŀؼ�
					m_arrRectanglesAll[i]->Draw(pDC);
				}	
			}
		}    
    }	
}

BOOL CControlTracker::SetCursor( UINT nHitTest, UINT message )
{
    ASSERT( m_pwndParent != NULL );

    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
		//��������ù��
		if( m_arrRectanglesAll[i]->SetCursor(m_pwndParent, nHitTest))
			return TRUE; 
    }	

    return FALSE;
}


//������ж��Ƿ�����˴˿ؼ�,��������λ��
int CControlTracker::IsIncludeControl(CWnd* ctl)
{
    if(!ctl)
        return -1;
	/////////////////////////////3-28������Z�����/////////////////////////////
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
		if(m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			if(m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==ctl->m_hWnd)
				return i;
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd()==ctl)
							return j;
					}
				}
			}
		}
        /////////////////////////////3-28������Z�����/////////////////////////////
        if( m_arrRectanglesAll[i]->GetCWnd()->m_hWnd==ctl->m_hWnd)
            return i;
    }	
    return -1;
}

int CControlTracker::IsInSelectControl(CWnd* ctl)
{
    if(!ctl)
        return -1;
    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        //��������ù��
        if( m_arrRectanglesSelected[i]->GetCWnd()->m_hWnd==ctl->m_hWnd)
            return i;
    }	
    return -1;

}

//����жϿؼ�������ֻ�жϵ�ǰ���������ȫ�ֵĽ������config�е�ID����
bool CControlTracker::IsExistID(CString id)
{
	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{	
		//��������ù��
		id.Trim();
		//�����TAB�ؼ����ж�TAB�ؼ������Ƿ���ڴ�ID
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			if( m_arrRectanglesAll[i]->GetCtlID()==id )
				return true;
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());

				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID()==id)
							return true;
					}
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==id)
			return true;
	}	

	return false;
}


CWnd* CControlTracker::GetCtlByID(CString id)
{
	for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
	{	
		//��������ù��
		id.Trim();

		//�����Table�ؼ�Ҫ����Table���������
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB)
		{
			CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());
			for(int i=0;i<pTab->getTabPageCount();i++)
			{
				for(int j=0;j<((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();j++)
				{
					if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID()==id)
						return ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCWnd();
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==id)
			return m_arrRectanglesAll[i]->GetCWnd();
	}	
    return NULL;
}

CControlRect* CControlTracker::GetRectByID(CString id)
{
    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
        //��������ù��
        id.Trim();

		//�����Table�ؼ�Ҫ����Table���������
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB )
		{
			if(m_arrRectanglesAll[i]->GetCtlID() == id)
				return m_arrRectanglesAll[i];
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());
				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					int TabNum = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					for(int j=0;j<TabNum;j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID()==id)
							return ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j];
					}
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==id)
            return m_arrRectanglesAll[i];
    }	
    return NULL;
}

CControlRect * CControlTracker::GetDisplayPropCtl()
{
    CControlRect * rtn=NULL;
    int size=m_arrRectanglesSelected.GetSize();
    if(size==0)
        return NULL;

    rtn=m_arrRectanglesSelected[0];

    for( int i=0; i < m_arrRectanglesSelected.GetSize(); i++ )
    {	
        //��������ù��
        if( m_arrRectanglesSelected[i]->m_highLight)
            rtn=m_arrRectanglesSelected[i];
    }	
    return rtn;
}

bool CControlTracker::ChangeCtlID(CString oldID,CString newID)
{
	if(!MyValidator::validateNameNoException(newID))
		return false;

    if(IsExistID(newID))
        return false;

    for( int i=0; i < m_arrRectanglesAll.GetSize(); i++ )
    {	
        //��������ù��
        oldID.Trim();
		if( m_arrRectanglesAll[i]->GetCtlType()==TCTT_TAB )
		{
			if( m_arrRectanglesAll[i]->GetCtlID() == oldID )
			{
				m_arrRectanglesAll[i]->SetCtlID(newID);
				return true;
			}
			else
			{
				CTCTab* pTab = (CTCTab*)(m_arrRectanglesAll[i]->GetCWnd());
				for(int i=0;i<pTab->getTabPageCount();i++)
				{
					int TabNum = ((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll.GetSize();
					for(int j=0;j<TabNum;j++)
					{
						if(((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->GetCtlID() == oldID)
						{
							((CTCTabPage*)pTab->m_Pages[i])->m_ControlTracker.m_arrRectanglesAll[j]->SetCtlID(newID);
							return true;
						}
					}
				}
			}
		}
		else if( m_arrRectanglesAll[i]->GetCtlID()==oldID)
		{
			m_arrRectanglesAll[i]->SetCtlID(newID);
			return true;
		}        
    }		
    return false;

}

void CControlTracker::Update()
{

}

CString CControlTracker::BuildValidCtlID(TCTOOLTYPE type)
{
    if(CTCRunTimeManager::g_LangType==1)
    {
        if(type==TCTT_STATIC)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("Static"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_GROUP)
        {

            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("group"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }


            return _T("");
        }
        else if(type==TCTT_EDIT)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("edit"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_BUTTON)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("button"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_RADIO)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("radio"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_CHECK)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("check"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_COMBOBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("ComboBox"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_PICTURE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("picture"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }       
        else if(type==TCTT_PROGRESS)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("progress"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }     
        else if(type==TCTT_PATH)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("path"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_SLIDER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("slider"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_DATE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("date"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_LISTBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("list"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
		else if(type==TCTT_TAB)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("tab"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        } 	
		else if(type==TCTT_TIMER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("timer"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        } 	
		else if(type==TCTT_HOTKEY)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("hotkey"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
		else if(type==TCTT_EXPLORER)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("explorer"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
		else if(type==TCTT_GRIDCTRL)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("gridctrl"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
    }
    else if(CTCRunTimeManager::g_LangType == 0)
    {
        if(type==TCTT_STATIC)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("��ǩ"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else  if(type==TCTT_EDIT)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("�༭��"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_GROUP)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("�����"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_BUTTON)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("��ť"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_RADIO)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("��ѡ��"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_CHECK)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("��ѡ��"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_COMBOBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("������"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
        else if(type==TCTT_PICTURE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("ͼƬ��"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        } 
        else if(type==TCTT_PROGRESS)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("������"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_PATH)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("�����"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_SLIDER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("����"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_DATE)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("���ڿ�"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
        else if(type==TCTT_LISTBOX)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("�б��"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }  
	    else if(type==TCTT_TAB)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("ѡ�"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }

            return _T("");
        }
		else if(type==TCTT_TIMER)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("��ʱ��"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }
            return _T("");
        } 
		else if(type==TCTT_HOTKEY)
        {
            for(int i=0;i<MAXCTRLLENGTH;i++)
            {
                CString id;
                id.Format(_T("%s%d"),_T("�ȼ�"),i);
                if(!IsExistID(id))
                {
                    return id;
                }
            }
            return _T("");
        } 
		else if(type==TCTT_EXPLORER)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("�����"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
		else if(type==TCTT_GRIDCTRL)
		{
			for(int i=0;i<MAXCTRLLENGTH;i++)
			{
				CString id;
				id.Format(_T("%s%d"),_T("���"),i);
				if(!IsExistID(id))
				{
					return id;
				}
			}

			return _T("");
		}
    }
}
/////////////�������ѡ�еĿؼ�Ȼ��ֱ���в���///////////////////////////////////
void CControlTracker::CenterWindowV()
{
	int numOfCtrl = m_arrRectanglesSelected.GetSize();

	//ֻѡ��һ���ؼ���ʱ��
	if( 1 == numOfCtrl )
	{
		CRect rectPanel;
		CRect rectStart;
		this->m_pwndParent->GetClientRect(&rectPanel);

		m_arrRectanglesSelected.GetAt( 0 )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);

		if(rectPanel.Height()/2 == rectStart.top+rectStart.Height()/2)
			return;

		SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
		pCommand->SetTCCtrlNum(numOfCtrl);

		
        //m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectPanel.Height()/2-rectStart.Height()/2,rectStart.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left,rectPanel.Height()/2-rectStart.Height()/2,rectStart.left+rectStart.Width(),rectPanel.Height()/2-rectStart.Height()/2+rectStart.Height());
        //m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
        //this->m_pwndParent->ScreenToClient(&rect);	
        //m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected.GetAt( 0 )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( 0 )->GetCtlType());

		switch(m_parentWndType)
		{
		case TCPARENT_DESDIN:
			((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
			break;
		case TCPARENT_TABPAGE:
			((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
			break;
		default:
			break;
		}
		return;
	}
	

	CRect selectRect = GetBoundsRect();
	int selectHeight=selectRect.Height()/2;

	CRect diaRect;
	this->m_pwndParent->GetClientRect(&diaRect);
	int BasicHeight = diaRect.Height()/2;
	int avarage = BasicHeight - (selectRect.top + selectHeight);

	if(selectRect.top + selectHeight == BasicHeight)
		return;

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl);

	for(int i=0;i<numOfCtrl;i++)
	{
			CRect rectStart;
			m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
			this->m_pwndParent->ScreenToClient(&rectStart);		

			CRect rect(rectStart.left,rectStart.top+avarage,rectStart.right,rectStart.top+avarage+rectStart.Height());
			//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
			//this->m_pwndParent->ScreenToClient(&rect);	
			//m_arrRectanglesSelected[i]->SetRect(rect); 
			pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}    
}
/////////////�������ѡ�еĿؼ�Ȼ��ֱ���в���//////////////////////////////////////

/////////////�������ѡ�еĿؼ�Ȼ��ˮƽ���в���///////////////////////////////////
void CControlTracker::CenterWindowH()
{
    int numOfCtrl = m_arrRectanglesSelected.GetSize();

	if( 1 == numOfCtrl  )
	{
		CRect rectPanel;
		CRect rectStart;
		this->m_pwndParent->GetClientRect(&rectPanel);

		m_arrRectanglesSelected.GetAt( 0 )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);

		if(rectPanel.Width()/2 == rectStart.left+rectStart.Width()/2)
			return;

		SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
		pCommand->SetTCCtrlNum(numOfCtrl);
		
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectPanel.Width()/2-rectStart.Width()/2,rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(rectPanel.Width()/2-rectStart.Width()/2,rectStart.top,rectPanel.Width()/2-rectStart.Width()/2+rectStart.Width(),rectStart.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected.GetAt( 0 )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( 0 )->GetCtlType());
		
		switch(m_parentWndType)
		{
		case TCPARENT_DESDIN:
			((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
			break;
		case TCPARENT_TABPAGE:
			((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
			break;
		default:
			break;
		}
		return;
	}

	CRect selectRect = GetBoundsRect();
	int selectWidth=selectRect.Width()/2;

	CRect diaRect;
	this->m_pwndParent->GetClientRect(&diaRect);
	int BasicWidth = diaRect.Width()/2;
	int avarage = BasicWidth - (selectRect.left + selectWidth);

	if(selectRect.left + selectWidth == BasicWidth)
		return;

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl);

	for(int i=0;i<numOfCtrl;i++)
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left+avarage,rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left+avarage,rectStart.top,rectStart.left+avarage+rectStart.Width(),rectStart.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect); 
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
///////////�������ѡ�еĿؼ�Ȼ��ˮƽ���в���//////////////////////////////////////

////////////////////////ѡ�пؼ������////////////////////////////////////////
void CControlTracker::AlignLeft()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.left != ctrlRect.left)
			break;

		if(i == Index - 1)
			return;
	}
	
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);

    for(int i=0; i < Index; i++ )	
    {
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(m_Basicrect.left,rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(m_Basicrect.left,rectStart.top,m_Basicrect.left+rectStart.Width(),rectStart.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect); 
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
////////////////////////ѡ�пؼ������/////////////////////////////////////////

//////////////////////////////ѡ�пؼ��Ҷ���/////////////////////////////
void CControlTracker::AlignRight()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.right != ctrlRect.right)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
    for(int i=0; i < Index; i++ )	
    {
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(,m_Basicrect.right-rectStart.Width(),rectStart.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(m_Basicrect.right-rectStart.Width(),rectStart.top,m_Basicrect.right,rectStart.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		// this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////ѡ�пؼ��Ҷ���/////////////////////////////

//////////////////////////////ѡ�пؼ��Ͷ˶���/////////////////////////////
void CControlTracker::AlignBottom()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.bottom != ctrlRect.bottom)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,m_Basicrect.bottom-rectStart.Height(),rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(rectStart.left,m_Basicrect.bottom-rectStart.Height(),rectStart.right,m_Basicrect.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////ѡ�пؼ��Ͷ˶���/////////////////////////////

//////////////////////////////ѡ�пؼ����˶���/////////////////////////////
void CControlTracker::AlignTop()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.top != ctrlRect.top)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,m_Basicrect.top,rectStart.Width(),rectStart.Height(),TRUE);
		CRect rect(rectStart.left,m_Basicrect.top,rectStart.right,m_Basicrect.top+rectStart.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////ѡ�пؼ����˶���/////////////////////////////

/////////////////////////ʹ�ؼ��Ի�׼�ؼ�Ϊ��׼��Сһ��/////////////////////
void CControlTracker::SetSameSize()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int i = 0; i < Index; i++)
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if( m_Basicrect.Width() != ctrlRect.Width() || m_Basicrect.Height() != ctrlRect.Height() )
			break;

		if( i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	
    for(int i=0; i < Index; i++ )	
    {
			CRect rectStart;
			m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
			this->m_pwndParent->ScreenToClient(&rectStart);
			//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,m_Basicrect.Width(),m_Basicrect.Height(),TRUE);

			CRect rect(rectStart.left,rectStart.top,rectStart.left+m_Basicrect.Width(),rectStart.top+m_Basicrect.Height());
		   // m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
			//this->m_pwndParent->ScreenToClient(&rect);	
			//m_arrRectanglesSelected[i]->SetRect(rect);
			pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
    }
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////ʹѡ�пؼ��߶���ͬ////////////////////
void CControlTracker::SetSameHeight()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL)
        return;

    CRect m_Basicrect;
    m_Basicrect = m_BasicElement->m_rect;

	for(int count = 0;count < Index;count++)
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( count )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if(m_Basicrect.Height() != ctrlRect.Height())
			break;
		if(count == Index - 1)
			return;
	}
	
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);

	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,rectStart.Width(),m_Basicrect.Height(),TRUE);
		CRect rect(rectStart.left,rectStart.top,rectStart.right,rectStart.top+m_Basicrect.Height());
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////

/////////////////////ʹѡ�пؼ������ͬ /////////////////////////////////////
void CControlTracker::SetSameWidth()
{
    int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL )
        return;

	CRect m_Basicrect;
	m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect ctrlRect;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&ctrlRect);
		this->m_pwndParent->ScreenToClient(&ctrlRect);

		if( m_Basicrect.Width() != ctrlRect.Width() )
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,m_Basicrect.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left,rectStart.top,rectStart.left+m_Basicrect.Width(),rectStart.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////ѡ�пؼ���ֱ���ж���///////////////////////////////////////
void CControlTracker::CenterAlignV()
{
	int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL )
        return;

	CRect m_Basicrect;
	m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect rectCtrl;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectCtrl);
		this->m_pwndParent->ScreenToClient(&rectCtrl);

		if(m_Basicrect.top + m_Basicrect.Height()/2 != rectCtrl.top + rectCtrl.Height()/2)
			break;

		if(i == Index - 1)
			return;
	}

	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->MoveWindow(rectStart.left,rectStart.top,m_Basicrect.Width(),rectStart.Height(),TRUE);

		CRect rect(rectStart.left,m_Basicrect.top+m_Basicrect.Height()/2-rectStart.Height()/2,rectStart.right,m_Basicrect.bottom-m_Basicrect.Height()/2+rectStart.Height()/2);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}  
}
/////////////////////////////////////////////////////////////////////////

////////////////////////////ѡ�пؼ�ˮƽ���ж���///////////////////////////////////
void CControlTracker::CenterAlignH()
{
	int Index = m_arrRectanglesSelected.GetSize();
    if( Index <2 || m_BasicElement == NULL )
        return;

	CRect m_Basicrect;
	m_Basicrect = m_BasicElement->m_rect;

	for(int i=0; i < Index; i++ )	
	{
		CRect rectCtrl;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectCtrl);
		this->m_pwndParent->ScreenToClient(&rectCtrl);

		if(m_Basicrect.left + m_Basicrect.Width()/2 != rectCtrl.left + rectCtrl.Width()/2)
			break;

		if(i == Index - 1)
			return;
	}

	
	SortCtrlCommand *pCommand = new SortCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(Index);
	for(int i=0; i < Index; i++ )	
	{
		CRect rectStart;
		m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rectStart);
		this->m_pwndParent->ScreenToClient(&rectStart);

		CRect rect(m_Basicrect.left+m_Basicrect.Width()/2-rectStart.Width()/2,rectStart.top,m_Basicrect.right-m_Basicrect.Width()/2+rectStart.Width()/2,rectStart.bottom);
		//m_arrRectanglesSelected.GetAt( i )->GetCWnd()->GetWindowRect(&rect);
		//this->m_pwndParent->ScreenToClient(&rect);	
		//m_arrRectanglesSelected[i]->SetRect(rect);
		pCommand->m_TCCtrlSort[i].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rect,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}
	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	} 
}
////////////////////////////ѡ�пؼ�ˮƽ����///////////////////////////////////

///////////////////////////////////////����ˮƽ���////////////////////////////////////
void CControlTracker::UniformSpaceH()
{
	int numOfCtrl = m_arrRectanglesSelected.GetSize();

	UniformCtrlCommand *pCommand = new UniformCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl - 2);
	//�ؼ�����,ѡ��
	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		int k = i;
		for (int j = i + 1; j < numOfCtrl; j++)
		{
			//��ȡ�����ؼ�������
			CRect rectJ, rectK;
			m_arrRectanglesSelected[j]->GetCWnd()->GetWindowRect(&rectJ);
			m_arrRectanglesSelected[k]->GetCWnd()->GetWindowRect(&rectK);
			//��������
			if (rectJ.left < rectK.left)
				k = j;
		}
		if (k != i)
		{
			CControlRect *temp = m_arrRectanglesSelected[i];
			m_arrRectanglesSelected[i] = m_arrRectanglesSelected[k];
			m_arrRectanglesSelected[k] = temp;
		}
	}
	
	//����ؼ�����ܼ��//////////////////////////////////////////////
	int spacing = 0;
	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		CRect rect1, rect2;
		//��ȡ�����ؼ�������
		m_arrRectanglesSelected[i + 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);
		//����ؼ��ĺ�����
		spacing += rect1.left - rect2.right;
	}

	//��ƽ����಼�ֿؼ�///////////////////////////////////////////////
	//����ƽ���������µļ��
	int spacingAvg = abs(spacing) / (numOfCtrl - 1);
	int spacingRemain = abs(spacing) % (numOfCtrl - 1);

	//�ָ�ƽ����������
	if (spacing < 0)
		spacingAvg = -spacingAvg;

	/*CRect rect1,TempRect1;
	m_arrRectanglesSelected[0]->GetCWnd()->GetWindowRect(&rect1);
	TempRect1 = rect1;
	this->m_pwndParent->ScreenToClient(&rect1);
	pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected[0]->GetCWnd(),TempRect1,rect1,m_arrRectanglesSelected[0]->GetCtlType());*/

	//�ؼ�����
	for (int i = 1; i < numOfCtrl - 1 ; i++)
	{
		//���ǰ�������ؼ�������ֵ
		CRect rect1, rect2,rectTemp;
		m_arrRectanglesSelected[i - 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);

		rectTemp = rect2;

		//��ȡ�ؼ��Ŀ��
		int width = rect2.right - rect2.left;
		//����ǰһ���ؼ���ƽ�����,�ض�λ�ؼ�
		rect2.left = rect1.right + spacingAvg;
		//�������µļ��
		if (i <= spacingRemain)
			rect2.left += ((spacing > 0) ? 1 : -1);
		//�ָ��ؼ��Ŀ��
		rect2.right = rect2.left + width;

		//��λ�ؼ�,�ƶ��ؼ��������λ��
		this->m_pwndParent->ScreenToClient(&rect2);
		m_arrRectanglesSelected[i]->GetCWnd()->MoveWindow(&rect2);
		//m_arrRectanglesSelected[i]->SetRect(rect2);	
		pCommand->m_TCCtrlSort[i - 1].SetTCCtrl(m_arrRectanglesSelected.GetAt( i )->GetCWnd(),rectTemp,rect2,m_arrRectanglesSelected.GetAt( i )->GetCtlType());
	}

	/*CRect rect2,TempRect2;
	m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd()->GetWindowRect(&rect2);
	TempRect2 = rect2;
	this->m_pwndParent->ScreenToClient(&rect2);
	pCommand->m_TCCtrlSort[numOfCtrl - 1].SetTCCtrl(m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd(),TempRect2,rect2,m_arrRectanglesSelected[numOfCtrl - 1]->GetCtlType());*/

	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}

////////////////////////////////////���ȴ�ֱ���////////////////////////////////////
void CControlTracker::UniformSpaceV()
{
	int numOfCtrl = m_arrRectanglesSelected.GetSize();
	UniformCtrlCommand *pCommand = new UniformCtrlCommand(m_parentWndType,m_pwndParent);
	pCommand->SetTCCtrlNum(numOfCtrl - 2);

	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		int k = i;
		for (int j = i + 1; j < numOfCtrl; j++)
		{
			//��ȡ�����ؼ�������
			CRect rectJ, rectK;
			m_arrRectanglesSelected[j]->GetCWnd()->GetWindowRect(&rectJ);
			m_arrRectanglesSelected[k]->GetCWnd()->GetWindowRect(&rectK);
			//��������
			if (rectJ.top < rectK.top)
				k = j;
		}
		if (k != i)
		{
			CControlRect *temp = m_arrRectanglesSelected[i];
			m_arrRectanglesSelected[i] = m_arrRectanglesSelected[k];
			m_arrRectanglesSelected[k] = temp;
		}
	}

	//����ؼ�����ܼ��////////////////////////////////////////////////////
	int spacing = 0;
	for (int i = 0; i < numOfCtrl - 1; i++)
	{
		CRect rect1, rect2;
		//��ȡ�����ؼ�������
		m_arrRectanglesSelected[i + 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);
		//����ؼ���������
		spacing += rect1.top - rect2.bottom;
	}

	//��ƽ����಼�ֿؼ�/////////////////////////////////////////////////
	//����ƽ���������µļ��
	int spacingAvg = abs(spacing) / (numOfCtrl - 1);
	int spacingRemain = abs(spacing) % (numOfCtrl - 1);

	

		//�ָ�ƽ����������
		if (spacing < 0)
			spacingAvg = -spacingAvg;

		/*CRect rect1,TempRect1;
		m_arrRectanglesSelected[0]->GetCWnd()->GetWindowRect(&rect1);
		TempRect1 = rect1;
		this->m_pwndParent->ScreenToClient(&rect1);
		pCommand->m_TCCtrlSort[0].SetTCCtrl(m_arrRectanglesSelected[0]->GetCWnd(),TempRect1,rect1,m_arrRectanglesSelected[0]->GetCtlType());*/

	//�ؼ�����
	for (int i = 1; i < numOfCtrl - 1; i++)
	{
		//���ǰ�������ؼ�������ֵ
		CRect rect1, rect2,tempRect;
		m_arrRectanglesSelected[i - 1]->GetCWnd()->GetWindowRect(&rect1);
		m_arrRectanglesSelected[i]->GetCWnd()->GetWindowRect(&rect2);

		tempRect = rect2;
		
		//��ȡ�ؼ��ĸ߶�
		int height = rect2.bottom - rect2.top;
		//����ǰһ���ؼ���ƽ�����,�ض�λ�ؼ�
		rect2.top = rect1.bottom + spacingAvg;
		//�������µļ��
		if (i <= spacingRemain)
			rect2.top += ((spacing > 0) ? 1 : -1);
		//�ָ��ؼ��ĸ߶�
		rect2.bottom = rect2.top + height;

		//��λ�ؼ�,�ƶ��ؼ��������λ��
		this->m_pwndParent->ScreenToClient(&rect2);
		m_arrRectanglesSelected[i]->GetCWnd()->MoveWindow(&rect2);
		//m_arrRectanglesSelected[i]->SetRect(rect2);	
		pCommand->m_TCCtrlSort[i - 1].SetTCCtrl(m_arrRectanglesSelected[i]->GetCWnd(),tempRect,rect2,m_arrRectanglesSelected[i]->GetCtlType());
	}

	/*CRect rect2,TempRect2;
	m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd()->GetWindowRect(&rect2);
	TempRect2 = rect2;
	this->m_pwndParent->ScreenToClient(&rect2);
	pCommand->m_TCCtrlSort[numOfCtrl - 1].SetTCCtrl(m_arrRectanglesSelected[numOfCtrl - 1]->GetCWnd(),TempRect2,rect2,m_arrRectanglesSelected[numOfCtrl - 1]->GetCtlType());*/

	switch(m_parentWndType)
	{
	case TCPARENT_DESDIN:
		((CDiaTCDesign*)m_pwndParent)->m_undoManager.DoCommand(pCommand);
		break;
	case TCPARENT_TABPAGE:
		((CDiaTCDesign*)m_pwndParent->GetParent()->GetParent())->m_undoManager.DoCommand(pCommand);
		break;
	default:
		break;
	}
}