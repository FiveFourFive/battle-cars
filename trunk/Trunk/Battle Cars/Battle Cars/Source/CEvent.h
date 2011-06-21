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

public:
	CEvent(EVENTID eventID, void* pParam = NULL)
	{
		m_EventID = eventID;
		m_pParam  = pParam;
	}

	~CEvent() {}

	//	Accessors:
	EVENTID  GetEventID(void)	{return m_EventID; }
	void*	GetParam(void)		{return m_pParam; }
};

#endif