// Download by http://www.codefans.net
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "RefObject.h"

#ifndef __AFXPLEX_H__
#include <afxplex_.h>
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
IMPLEMENT_DYNAMIC(CRefObject, CObject)

// Decrement reference count.  Object will be deleted when the reference count reaches zero.
DWORD CRefObject::Release()
{
	ASSERT_VALID(this);
	m_dwRefCount--;

	DWORD dwRefCount = m_dwRefCount;
	if (dwRefCount == 0)
	  delete this;
  return dwRefCount;
}


/////////////////////////////////////////////////////////////////////////////
// CRefObList

//
IMPLEMENT_SERIAL(CRefObList, CObject, 0)

CRefObList::CRefObList(int nBlockSize)
{
	ASSERT(nBlockSize > 0);

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

void CRefObList::RemoveAll()
{
	ASSERT_VALID(this);

	// Release elements
	for (POSITION pos = (POSITION)m_pNodeHead; pos != NULL;)
		GetNext(pos)->Release();

	// destroy elements
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

CRefObList::~CRefObList()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
/*
 * Implementation note: CNode's are stored in CPlex blocks and
 *  chained together. Free blocks are maintained in a singly linked list
 *  using the 'pNext' member of CNode with 'm_pNodeFree' as the head.
 *  Used blocks are maintained in a doubly linked list using both 'pNext'
 *  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
 *   as the head/tail.
 *
 * We never free a CPlex block unless the List is destroyed or RemoveAll()
 *  is used - so the total number of CPlex blocks may grow large depending
 *  on the maximum past size of the list.
 */

CRefObList::CNode* CRefObList::NewNode(CRefObList::CNode* pPrev, CRefObList::CNode* pNext)
{
	if (m_pNodeFree == NULL)
	{
		// add another block
		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
				 sizeof(CNode));

		// chain them into free list
		CNode* pNode = (CNode*) pNewBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}
	ASSERT(m_pNodeFree != NULL);  // we must have something

	CRefObList::CNode* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow

	pNode->data = 0; // start with zero

	return pNode;
}

void CRefObList::FreeNode(CRefObList::CNode* pNode)
{
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////

POSITION CRefObList::AddHead(CRefObject* pNewOb)
{
	ASSERT_VALID(this);
	ASSERT_KINDOF(CRefObject, pNewOb);	// Must be derived from CRefObject
	ASSERT_VALID(pNewOb);

	CNode* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = pNewOb;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;

	pNewOb->AddRef();
	return (POSITION) pNewNode;
}

POSITION CRefObList::AddTail(CRefObject* pNewOb)
{
	ASSERT_VALID(this);
	ASSERT_KINDOF(CRefObject, pNewOb);	// Must be derived from CRefObject
	ASSERT_VALID(pNewOb);

	CNode* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = pNewOb;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;

	pNewOb->AddRef();
	return (POSITION) pNewNode;
}

void CRefObList::AddHead(CRefObList* pNewList)
{
	ASSERT_VALID(this);

	ASSERT(pNewList != NULL);
	ASSERT_KINDOF(CRefObList, pNewList);
	ASSERT_VALID(pNewList);

	// add a list of same elements to head (maintain order)
	for (POSITION pos = pNewList->GetTailPosition(); pos != NULL; )
		AddHead(pNewList->GetPrev(pos));
}

void CRefObList::AddTail(CRefObList* pNewList)
{
	ASSERT_VALID(this);
	ASSERT(pNewList != NULL);
	ASSERT_KINDOF(CRefObList, pNewList);
	ASSERT_VALID(pNewList);

	// add a list of same elements
	for (POSITION pos = pNewList->GetHeadPosition(); pos != NULL; )
		AddTail(pNewList->GetNext(pos));
}

CRefObject* CRefObList::RemoveHead()
{
	ASSERT_VALID(this);
	ASSERT(m_pNodeHead != NULL);  // don't call on empty list !!!
	ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));

	CNode* pOldNode = m_pNodeHead;
	CRefObject* pRemoveOb = pOldNode->data;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;
	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);

	pRemoveOb->Release();
	return pRemoveOb;
}

CRefObject* CRefObList::RemoveTail()
{
	ASSERT_VALID(this);
	ASSERT(m_pNodeTail != NULL);  // don't call on empty list !!!
	ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));

	CNode* pOldNode = m_pNodeTail;
	CRefObject* pRemoveOb = pOldNode->data;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);

	pRemoveOb->Release();
	return pRemoveOb;
}

POSITION CRefObList::InsertBefore(POSITION position, CRefObject* pNewOb)
{
	ASSERT_VALID(this);
	ASSERT_KINDOF(CRefObject, pNewOb);	// Must be derived from CRefObject
	ASSERT_VALID(pNewOb);

	if (position == NULL)
		return AddHead(pNewOb); // insert before nothing -> head of the list

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
	CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = pNewOb;

	if (pOldNode->pPrev != NULL)
	{
		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeHead);
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;

	pNewOb->AddRef();
	return (POSITION) pNewNode;
}

POSITION CRefObList::InsertAfter(POSITION position, CRefObject* pNewOb)
{
	ASSERT_VALID(this);
	ASSERT_KINDOF(CRefObject, pNewOb);	// Must be derived from CRefObject
	ASSERT_VALID(pNewOb);

	if (position == NULL)
		return AddTail(pNewOb); // insert after nothing -> tail of the list

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));
	CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = pNewOb;

	if (pOldNode->pNext != NULL)
	{
		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeTail);
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;

	pNewOb->AddRef();
	return (POSITION) pNewNode;
}

void CRefObList::RemoveAt(POSITION position)
{
	ASSERT_VALID(this);

	CNode* pOldNode = (CNode*) position;
	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CNode)));

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
	}
	else
	{
		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CNode)));
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CNode)));
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}

	CRefObject* pRemoveOb = pOldNode->data;
	FreeNode(pOldNode);
	pRemoveOb->Release();
}


/////////////////////////////////////////////////////////////////////////////
// slow operations

POSITION CRefObList::FindIndex(int nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);

	if (nIndex >= m_nCount)
		return NULL;  // went too far

	CNode* pNode = m_pNodeHead;
	while (nIndex--)
	{
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		pNode = pNode->pNext;
	}
	return (POSITION) pNode;
}

POSITION CRefObList::Find(CRefObject* pFindOb, POSITION startAfter) const
{
	ASSERT_VALID(this);
	ASSERT_KINDOF(CRefObject, pFindOb);	// Must be derived from CRefObject
	ASSERT_VALID(pFindOb);

	CNode* pNode = (CNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		pNode = pNode->pNext;  // start after the one specified
	}

	for (; pNode != NULL; pNode = pNode->pNext)
		if (pNode->data == pFindOb)
			return (POSITION) pNode;
	return NULL;
}

void CRefObList::SetAt(POSITION pos, CRefObject* pNewOb)
{
	ASSERT_VALID(this);
	ASSERT_KINDOF(CRefObject, pNewOb);	// Must be derived from CRefObject
	ASSERT_VALID(pNewOb);

	CNode* pNode = (CNode*) pos;
	ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
	pNode->data->Release();
	pNode->data = pNewOb;
	pNewOb->AddRef();
}

// CompareFunc is expected to return a positive integer if pFirstObj
// should follow pSecondObj (is greater than)
//
// Uses Insertion Sort
//
// The Shell Sort is much faster than a straight insertion sort, however, it cannot
//  be performed on a linked list (it COULD, but the resulting code would probably be
//  much slower as a Shell Sort jumps all around the reletive positions of elements).
//
// An Insertion Sort works by evaluating an item, if that item should
// precede the item in front of it, than it shifts all the items that
// should follow that item up one place until it finds the correct position
// for the item, whereby it then 'inserts' that item.
//
void CRefObList::Sort(int (*CompareFunc)(CRefObject* pFirstComp, CRefObject* pSecondComp))
{
	ASSERT_VALID(this);

	// If the list contains no items, the HEAD position will be NULL
	if (m_pNodeHead == NULL)
		return;

	CRefObject *pOtemp;
	CRefObList::CNode *pNi,*pNj;

	// Walk the list
	for (pNi = m_pNodeHead->pNext; pNi != NULL; pNi = pNi->pNext)
	{
		// Save data pointer
		pOtemp = pNi->data;

		// Walk the list backwards from pNi to the beginning of the list or until
		// the CompareFunc() determines that this item is in it's correct position
		// shifting all items upwards as it goes
		for (pNj = pNi; pNj->pPrev != NULL && CompareFunc(pNj->pPrev->data, pOtemp) > 0; pNj = pNj->pPrev)
			pNj->data = pNj->pPrev->data;

		// Insert data pointer into it's proper position
		pNj->data = pOtemp;
	}

}

// This variation allows you to sort only a portion of the list
//
// iElements can be larger than the number of remaining elements without harm
// iElements can be -1 which will always sort to the end of the list
//
void CRefObList::Sort(POSITION posStart, int iElements, int (*CompareFunc)(CRefObject* pFirstComp, CRefObject* pSecondComp))
{
	ASSERT_VALID(this);
	ASSERT( AfxIsValidAddress((CRefObList::CNode*)posStart, sizeof(CRefObList::CNode)) );

	// Make certain posStart is a position value obtained by a GetHeadPosition or Find member function call
	//  as there is no way to test whether or not posStart is a valid CNode pointer from this list.
	// Ok, there is one way, we could walk the entire list and verify that posStart is in the chain, but even
	//  for debug builds that's a bit much.

	// If the list contains no items, the HEAD position will be NULL
	if (m_pNodeHead == NULL)
		return;

	CRefObject *pOtemp;
	CRefObList::CNode *pNi,*pNj;

	// Walk the list
	for (pNi = (CRefObList::CNode*)posStart; pNi != NULL && iElements != 0; pNi = pNi->pNext, iElements--)
	{
		// Save data pointer
		pOtemp = pNi->data;

		// Walk the list backwards from pNi to the beginning of the sort or until
		// the CompareFunc() determines that this item is in it's correct position
		// shifting all items upwards as it goes
		for (pNj = pNi; pNj->pPrev != NULL && pNj->pPrev != ((CRefObList::CNode*)posStart)->pPrev && CompareFunc(pNj->pPrev->data, pOtemp) > 0; pNj = pNj->pPrev)
			pNj->data = pNj->pPrev->data;

		// Insert data pointer into it's proper position
		pNj->data = pOtemp;
	}

}


/////////////////////////////////////////////////////////////////////////////
// Serialization

//
void CRefObList::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar.WriteCount(m_nCount);
		for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		{
			ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
			ar << pNode->data;
		}
	}
	else
	{
		DWORD nNewCount = ar.ReadCount();
		CRefObject* pOb;
		while (nNewCount--)
		{
			ar >> pOb;
			AddTail(pOb);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Diagnostics

#ifdef _DEBUG

//
void CRefObList::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with " << m_nCount << " objects";
	if (dc.GetDepth() > 0)
	{
		for (POSITION pos = GetHeadPosition(); pos != NULL; )
			dc << "\n\t" << GetNext(pos);
	}

	dc << "\n";
}

//
void CRefObList::AssertValid() const
{
	CObject::AssertValid();

	if (m_nCount == 0)
	{
		// empty list
		ASSERT(m_pNodeHead == NULL);
		ASSERT(m_pNodeTail == NULL);
	}
	else
	{
		// non-empty list
		ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CNode)));
		ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CNode)));
	}
}
#endif //_DEBUG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif


/////////////////////////////////////////////////////////////////////////////
