/////////////////////////////////////////////////
//	File	:	"CEventSystem.cpp"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the functions for processing
//				messages with the correct messsage proc
//				
/////////////////////////////////////////////////

#ifndef _CMESSAGESYSTEM_H_
#define _CMESSAGESYSTEM_H_

#include "CMessage.h"
#include <list>

typedef void (*MessageProc)(CBaseMessage*);


class CMessageSystem
{
private:
	std::list<CBaseMessage*>		m_MessageList;				//	Stores my messages.
	MessageProc						m_pfMessageProc;			//	Points to user defined function.

	CMessageSystem() { m_pfMessageProc = NULL;	}
	CMessageSystem(const CMessageSystem&);
	CMessageSystem& operator=(const CMessageSystem&);

	~CMessageSystem() {}

public:
	static CMessageSystem* GetInstance(void);

	int GetNumMessages(void) { return (int)m_MessageList.size(); }
	void SendMsg(CBaseMessage* pMsg);
	void ProcessMessages(void);
	void ClearMessages(void);

	void InitMessageSystem(MessageProc pfMessageProc);
	void ShutdownMessageSystem(void);
};

#endif