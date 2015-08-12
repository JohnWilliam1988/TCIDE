// CommandHistory.h: interface for the ICommandHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDHISTORY_H__6E2D3094_1B39_11D2_B94F_8404FB2BC358__INCLUDED_)
#define AFX_COMMANDHISTORY_H__6E2D3094_1B39_11D2_B94F_8404FB2BC358__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define COMMAND_HISTORY_DEPTH 256

class CCommand;


//////////////////////////////////////////////////////////////////////
// ICommandHistory

class ICommandHistory
{
// Construction
public:
	ICommandHistory();
	virtual ~ICommandHistory();

// Operations
protected:
	int NextPos(int iPos) const;
	int PrevPos(int iPos) const;
	void IncrementCount(int& rCount);
	void DecrementCount(int& rCount);
	void IncrementClean();
	void DecrementClean();

	void DeleteHistory();								// Protected, use ResetHistory instead

public:
	virtual BOOL Do(CCommand* pCmd, BOOL bLog = TRUE);	// Execute and log the command
	virtual void LogCommand(CCommand* pCmd);					// Put the command in the ring buffer

	void ResetHistory();									// Reset the command history
	void SetHistorySize(int nHistorySize);			// Set the size of the ring buffer

	virtual void Undo();											// Undo the previous command
	virtual void Redo();											// Redo the previously undone command
	virtual BOOL CanUndo() const;								// Is a command available to be undone?
	virtual BOOL CanRedo() const;								// Is a command available to be redone?
	const CCommand* GetUndoCommand() const;		// Returns the available undo command, if any
	const CCommand* GetRedoCommand() const;		// Returns the available redo command, if any

	void SetClean();
	BOOL IsClean() const;
	BOOL IsDirty() const;

	virtual void OnStatusChange();
	virtual void OnCommandExecute(const CCommand* pCmd);

// Attributes
protected:
	int m_iCommandHistoryUndoPos;

	int m_iCommandHistoryUndoCount;
	int m_iCommandHistoryRedoCount;
	int m_iCommandHistoryCleanCount;

	CTypedPtrArray<CPtrArray, CCommand*> m_commandHistory;
};


inline int ICommandHistory::NextPos(int iPos) const
	{ return ( (iPos+1) % m_commandHistory.GetSize() ); }
inline int ICommandHistory::PrevPos(int iPos) const
	{ return ( (iPos + m_commandHistory.GetSize() - 1) % m_commandHistory.GetSize() ); }

inline void ICommandHistory::IncrementCount(int& rCount)
	{ rCount = min(rCount+1, m_commandHistory.GetSize()); }
inline void ICommandHistory::DecrementCount(int& rCount)
	{ rCount = max(rCount-1, -m_commandHistory.GetSize()); }

inline void ICommandHistory::IncrementClean()
	{ m_iCommandHistoryCleanCount = min(m_iCommandHistoryCleanCount+1, m_commandHistory.GetSize()+1); }
inline void ICommandHistory::DecrementClean()
	{ m_iCommandHistoryCleanCount = max(m_iCommandHistoryCleanCount-1, -(m_commandHistory.GetSize()+1)); }

inline void ICommandHistory::SetHistorySize(int nHistorySize)
	{ m_commandHistory.SetSize(nHistorySize); }

inline BOOL ICommandHistory::CanUndo() const
	{ return m_iCommandHistoryUndoCount != 0; }
inline BOOL ICommandHistory::CanRedo() const
	{ return m_iCommandHistoryRedoCount != 0; }

inline void ICommandHistory::SetClean()
	{ m_iCommandHistoryCleanCount = 0; OnStatusChange(); }
inline BOOL ICommandHistory::IsClean() const
	{ return m_iCommandHistoryCleanCount == 0; }
inline BOOL ICommandHistory::IsDirty() const
	{ return m_iCommandHistoryCleanCount != 0; }

inline void ICommandHistory::OnStatusChange()
	{ /* no implementation */ }
inline void ICommandHistory::OnCommandExecute(const CCommand*)
	{ /* no implementation */ }

#endif // !defined(AFX_COMMANDHISTORY_H__6E2D3094_1B39_11D2_B94F_8404FB2BC358__INCLUDED_)
