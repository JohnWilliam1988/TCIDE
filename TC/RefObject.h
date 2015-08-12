
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*

 CRefObject.h : 
	
	  CRefObject
	  CRefObList
	  CTypedRefObList

 Revision: 0

 Copyright (c) 1997, 1998
 Douglas Peterson - Praetor Software

*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __REFOBJECTH__
#define __REFOBJECTH__


/////////////////////////////////////////////////////////////////////////////
// CRefObject

//
// CRefObject provides for maintaining an object's lifetime through reference counting, much like COM objects do.
//
class CRefObject : public CObject
{
	DECLARE_DYNAMIC(CRefObject)

// Construction/destruction
public:
	CRefObject();

protected:
// Protected dtor so that one cannot be constructed on the stack and can only be deleted by Release member.
	virtual ~CRefObject();

// Operations
public:
	DWORD AddRef();
	DWORD Release();

	DWORD GetRefCount() const;

// Attributes
private:
	DWORD m_dwRefCount;

	friend CArchive& operator << (CArchive& ar, const CRefObject* pOb)
		{ ar.WriteObject(pOb); return ar; }
	friend CArchive& operator >> (CArchive& ar, CRefObject*& pOb)
		{ pOb = STATIC_DOWNCAST(CRefObject, ar.ReadObject(NULL)); return ar; }
	friend CArchive& operator >> (CArchive& ar, const CRefObject*& pOb)
		{ pOb = STATIC_DOWNCAST(CRefObject, ar.ReadObject(NULL)); return ar; }
};


////////////////////////////////////////////////////////////////////////////

// AddRef must be called after construction (unless you pass it off to a list).
inline CRefObject::CRefObject()
	{ m_dwRefCount = 0; }
inline CRefObject::~CRefObject()
	{ /* No implementation */ }
// Increment reference count.
inline DWORD CRefObject::AddRef()
	{ return ++m_dwRefCount; }
// Returns the current reference count.
inline DWORD CRefObject::GetRefCount() const
	{ return m_dwRefCount; }



/////////////////////////////////////////////////////////////////////////////
// CRefObList

//
// CRefObList is identical to CObList except that it provides specialized functionality in conjuction with
// CRefObject.  CRefObject's lifetimes are maintained by reference count.  This list (which can only be used
// with CRefObject derived objects) will automatically maintain object reference counts.
//
// CRefObList provides the sort capabilities found in CSortableObList.
//
class CRefObList : public CObject
{
	DECLARE_SERIAL(CRefObList)

protected:
	struct CNode
	{
		CNode* pNext;
		CNode* pPrev;
		CRefObject* data;
	};

// Construction
public:
	CRefObList(int nBlockSize = 10);
	~CRefObList();

// Operations
public:
	int GetCount() const;
	BOOL IsEmpty() const;

	CRefObject*& GetHead();
	CRefObject* GetHead() const;
	CRefObject*& GetTail();
	CRefObject* GetTail() const;

	// Get head (and remove it) - don't call on empty list!
	CRefObject* RemoveHead();
	// Get tail (and remove it) - don't call on empty list!
	CRefObject* RemoveTail();

	// Add before head
	POSITION AddHead(CRefObject* pNewOb);
	// Add before after tail
	POSITION AddTail(CRefObject* pNewOb);

	// Add another list of elements before head
	void AddHead(CRefObList* pNewList);
	// Add another list of elements after tail
	void AddTail(CRefObList* pNewList);

	void RemoveAll();

	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	CRefObject*& GetNext(POSITION& rPosition); // return *Position++
	CRefObject* GetNext(POSITION& rPosition) const; // return *Position++
	CRefObject*& GetPrev(POSITION& rPosition); // return *Position--
	CRefObject* GetPrev(POSITION& rPosition) const; // return *Position--

	CRefObject*& GetAt(POSITION position);
	CRefObject* GetAt(POSITION position) const;
	void SetAt(POSITION pos, CRefObject* pNewOb);
	void RemoveAt(POSITION position);

	POSITION InsertBefore(POSITION position, CRefObject* pNewOb);
	POSITION InsertAfter(POSITION position, CRefObject* pNewOb);

	// Get POSITION of object, return NULL if not found
	POSITION Find(CRefObject* pFindComp, POSITION startAfter = NULL) const;
	// Get the 'nIndex'th element (may return NULL)
	POSITION FindIndex(int nIndex) const;

	void Sort(int(*CompareFunc)(CRefObject* pFirstComp, CRefObject*pSecondComp));
	void Sort(POSITION posStart, int nElements, int (*CompareFunc)(CRefObject* pFirstComp, CRefObject* pSecondComp));

// Overrides
public:
	virtual void Serialize(CArchive&);
#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif

// Attributes
protected:
	CNode* m_pNodeHead;
	CNode* m_pNodeTail;
	int m_nCount;
	CNode* m_pNodeFree;
	struct CPlex* m_pBlocks;
	int m_nBlockSize;

	CNode* NewNode(CNode*, CNode*);
	void FreeNode(CNode*);
};


////////////////////////////////////////////////////////////////////////////

inline int CRefObList::GetCount() const
	{ return m_nCount; }
inline BOOL CRefObList::IsEmpty() const
	{ return m_nCount == 0; }
inline CRefObject*& CRefObList::GetHead()
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
inline CRefObject* CRefObList::GetHead() const
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
inline CRefObject*& CRefObList::GetTail()
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
inline CRefObject* CRefObList::GetTail() const
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
inline POSITION CRefObList::GetHeadPosition() const
	{ return (POSITION) m_pNodeHead; }
inline POSITION CRefObList::GetTailPosition() const
	{ return (POSITION) m_pNodeTail; }
inline CRefObject*& CRefObList::GetNext(POSITION& rPosition) // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
inline CRefObject* CRefObList::GetNext(POSITION& rPosition) const // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
inline CRefObject*& CRefObList::GetPrev(POSITION& rPosition) // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
inline CRefObject* CRefObList::GetPrev(POSITION& rPosition) const // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
inline CRefObject*& CRefObList::GetAt(POSITION position)
	{ CNode* pNode = (CNode*) position;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		return pNode->data; }
inline CRefObject* CRefObList::GetAt(POSITION position) const
	{ CNode* pNode = (CNode*) position;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CNode)));
		return pNode->data; }


/////////////////////////////////////////////////////////////////////////////
// CTypedRefObList< TYPE >

//
// CTypedRefObList provides a type safe mechanism for using CRefObList's.
//
template< class TYPE >
class CTypedRefObList : public CRefObList
{
// Construction
public:
	CTypedRefObList(int nBlockSize = 10) : CRefObList(nBlockSize)
		{}

	// peek at head or tail
	TYPE& GetHead()
		{ return (TYPE&)CRefObList::GetHead(); }
	TYPE GetHead() const
		{ return (TYPE)CRefObList::GetHead(); }
	TYPE& GetTail()
		{ return (TYPE&)CRefObList::GetTail(); }
	TYPE GetTail() const
		{ return (TYPE)CRefObList::GetTail(); }

	// get head or tail (and remove it) - don't call on empty list!
	TYPE RemoveHead()
		{ return (TYPE)CRefObList::RemoveHead(); }
	TYPE RemoveTail()
		{ return (TYPE)CRefObList::RemoveTail(); }

	// add before head or after tail
	POSITION AddHead(TYPE pNewOb)
		{ return CRefObList::AddHead(pNewOb); }
	POSITION AddTail(TYPE pNewOb)
		{ return CRefObList::AddTail(pNewOb); }

	// add another list of elements before head or after tail
	void AddHead(CTypedRefObList< TYPE >* pNewList)
		{ CRefObList::AddHead(pNewList); }
	void AddTail(CTypedRefObList< TYPE >* pNewList)
		{ CRefObList::AddTail(pNewList); }

	// iteration
	TYPE& GetNext(POSITION& rPosition)
		{ return (TYPE&)CRefObList::GetNext(rPosition); }
	TYPE GetNext(POSITION& rPosition) const
		{ return (TYPE)CRefObList::GetNext(rPosition); }
	TYPE& GetPrev(POSITION& rPosition)
		{ return (TYPE&)CRefObList::GetPrev(rPosition); }
	TYPE GetPrev(POSITION& rPosition) const
		{ return (TYPE)CRefObList::GetPrev(rPosition); }

	// getting/modifying an element at a given position
	TYPE& GetAt(POSITION position)
		{ return (TYPE&)CRefObList::GetAt(position); }
	TYPE GetAt(POSITION position) const
		{ return (TYPE)CRefObList::GetAt(position); }
	void SetAt(POSITION pos, TYPE pNewOb)
		{ CRefObList::SetAt(pos, pNewOb); }

	void Sort(int(*CompareFunc)(TYPE pFirstComp, TYPE pSecondComp) )
		{ CRefObList::Sort((int(*)(CRefObject*, CRefObject*))CompareFunc); }
	void Sort(POSITION posStart, int nElements, int(*CompareFunc)(TYPE pFirstComp, TYPE pSecondComp) )
		{ CRefObList::Sort(posStart, nElements, (int(*)(CRefObject*, CRefObject*))CompareFunc); }
};


#endif // !defined(__REFOBJECTH__)
