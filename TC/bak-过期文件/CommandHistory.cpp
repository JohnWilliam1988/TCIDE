
// CommandHistory.cpp: implementation of the ICommandHistory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CommandHistory.h"
#include "Command.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// ICommandHistory

ICommandHistory::ICommandHistory()
{
	SetHistorySize(COMMAND_HISTORY_DEPTH);
	m_iCommandHistoryUndoCount = m_iCommandHistoryUndoPos = m_iCommandHistoryRedoCount = m_iCommandHistoryCleanCount = 0;
}

ICommandHistory::~ICommandHistory()
{
	DeleteHistory();
}

void ICommandHistory::DeleteHistory()
{
	for (int i = m_commandHistory.GetSize()-1; i >= 0; i--)
	{
		if (m_commandHistory[i])
		{
			delete m_commandHistory[i];
			m_commandHistory[i] = NULL;
		}
	}
}

void ICommandHistory::LogCommand(CCommand* pCmd)
{
	ASSERT_VALID(pCmd);

	if (m_commandHistory[m_iCommandHistoryUndoPos])
		delete m_commandHistory[m_iCommandHistoryUndoPos];

	m_commandHistory[m_iCommandHistoryUndoPos] = pCmd;
	m_iCommandHistoryUndoPos = NextPos(m_iCommandHistoryUndoPos);

	IncrementCount(m_iCommandHistoryUndoCount);
	if (m_iCommandHistoryCleanCount < 0 && m_iCommandHistoryRedoCount > 0)
		m_iCommandHistoryCleanCount = m_commandHistory.GetSize()+1;	// Can never get back to clean if saved after this position
	else
		IncrementClean();
	m_iCommandHistoryRedoCount = 0;

	OnStatusChange();
}

void ICommandHistory::Undo()
{
	ASSERT(CanUndo());

	IncrementCount(m_iCommandHistoryRedoCount);
	DecrementCount(m_iCommandHistoryUndoCount);
	DecrementClean();

	m_iCommandHistoryUndoPos = PrevPos(m_iCommandHistoryUndoPos);

	CCommand* pCmd = m_commandHistory[m_iCommandHistoryUndoPos];
	ASSERT_VALID(pCmd);

	CCommand* pCmdUndo = pCmd->GetUndoCommand();
	ASSERT_VALID(pCmdUndo);

	Do(pCmdUndo, FALSE);		// don't log
	OnStatusChange();
}

const CCommand* ICommandHistory::GetUndoCommand() const
{
	if (!CanUndo())
		return NULL;
	return m_commandHistory[PrevPos(m_iCommandHistoryUndoPos)];
}

void ICommandHistory::Redo()
{
	ASSERT(CanRedo());

	CCommand* pCmd = m_commandHistory[m_iCommandHistoryUndoPos];
	ASSERT_VALID(pCmd);

	m_iCommandHistoryUndoPos = NextPos(m_iCommandHistoryUndoPos);

	DecrementCount(m_iCommandHistoryRedoCount);
	IncrementCount(m_iCommandHistoryUndoCount);
	IncrementClean();

	CCommand* pCmdRedo = pCmd->GetRedoCommand();
	ASSERT_VALID(pCmdRedo);

	Do(pCmdRedo, FALSE);		// don't log
	OnStatusChange();
}

const CCommand* ICommandHistory::GetRedoCommand() const
{
	if (!CanRedo())
		return NULL;
	return m_commandHistory[m_iCommandHistoryUndoPos];
}

void ICommandHistory::ResetHistory()
{
	DeleteHistory();
	m_iCommandHistoryUndoCount = m_iCommandHistoryUndoPos = m_iCommandHistoryRedoCount = m_iCommandHistoryCleanCount = 0;
	OnStatusChange();
}

BOOL ICommandHistory::Do(CCommand* pCmd, BOOL bLog)
{
	ASSERT_VALID(pCmd);

	if (!pCmd->Execute())
	{
		// If the command returns false and was intended to be logged, delete it without logging it
		if (bLog)
			delete pCmd;
		return FALSE;
	}

	if (pCmd->IsKindOf(RUNTIME_CLASS(CCommandMacro)))
	{
		for (POSITION pos = STATIC_DOWNCAST(CCommandMacro, pCmd)->m_listCommands.GetHeadPosition(); pos != NULL; )
			OnCommandExecute(STATIC_DOWNCAST(CCommandMacro, pCmd)->m_listCommands.GetNext(pos));
	}
	else
		OnCommandExecute(pCmd);

	// Log the command,
	if (bLog)
		LogCommand(pCmd);
	// or delete it
	else
		delete pCmd;

	return TRUE;
}
