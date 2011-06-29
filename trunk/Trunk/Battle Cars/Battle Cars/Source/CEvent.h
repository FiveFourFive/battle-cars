/////////////////////////////////////////////////
//	File	:	"CEvent.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the function for derived classes
//				to create an event
//				
/////////////////////////////////////////////////


#ifndef _CEVENT_H_
#define _CEVENT_H_

#include <string>

typedef std::string EVENTID;		//	the string id of the event

class CEvent
{
private:
	EVENTID			m_EventID;
	void*			m_pParam; // add more void* if you need to send more data
	void*			m_pParam2;

public:
	CEvent(EVENTID eventID, void* pParam = NULL, void* pParam2 = NULL)
	{
		m_EventID = eventID;
		m_pParam  = pParam;
		m_pParam2 = pParam2;
	}

	~CEvent() {}

	//	Accessors:
	EVENTID  GetEventID(void)	{return m_EventID; }
	void*	GetParam(void)		{return m_pParam; }
	void*	GetParam2(void)		{ return m_pParam2; }
};

#endif