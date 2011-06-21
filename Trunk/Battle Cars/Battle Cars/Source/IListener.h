/////////////////////////////////////////////////
//	File	:	"IListener.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the function for derived classes
//				to handle events that are sent to them
//				
/////////////////////////////////////////////////

#ifndef _ILISTENER_H_
#define _ILISTENER_H_

#include "CEvent.h"

class IListener
{
public:
	IListener(void) {}
	virtual ~IListener(void) {}

	// This function is used whenever the object receives an event 
	virtual void HandleEvent(CEvent* pEvent) = 0;
};

#endif