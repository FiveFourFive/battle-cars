/////////////////////////////////////////////////
//	File	:	"CMessageSystem.cpp"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the functions for using events
//				
/////////////////////////////////////////////////

#include "CMessageSystem.h"

CMessageSystem* CMessageSystem::GetInstance(void)
{
	static CMessageSystem instance;
	return &instance;
}

void CMessageSystem::InitMessageSystem(MessageProc pfMessageProc)
{
	m_pfMessageProc = pfMessageProc;
}

void CMessageSystem::SendMsg(CBaseMessage* pMsg)
{

	m_MessageList.push_back(pMsg);
}

void CMessageSystem::ProcessMessages(void)
{
	if(!m_pfMessageProc)
		return;

	while(!m_MessageList.empty())
	{
		m_pfMessageProc(m_MessageList.front());

		delete m_MessageList.front();

		m_MessageList.pop_front();					
	}
}

void CMessageSystem::ClearMessages(void)
{
	while(!m_MessageList.empty())
	{
		delete m_MessageList.front();
		m_MessageList.pop_front();
	}
}

void CMessageSystem::ShutdownMessageSystem(void)
{
	ClearMessages();

	m_pfMessageProc = NULL;
}