/////////////////////////////////////////////////
//	File	:	"CEventSystem.cpp"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the functions for using events
//				
/////////////////////////////////////////////////

#include "CEventSystem.h"

CEventSystem* CEventSystem::GetInstance ()
{
		static CEventSystem instance;
		return &instance;
}

void CEventSystem::RegisterClient(EVENTID eventID, IListener* pClient)
{
	if (!pClient || AlreadyRegistered(eventID, pClient))	
		return;

	m_mmClientsData.insert( make_pair(eventID, pClient) );
}

void CEventSystem::UnregisterClient(EVENTID eventID, IListener *pClient)
{
	pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	range = m_mmClientsData.equal_range(eventID);

	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_mmClientsData.erase(mmIter);
			break;
		}
	}
}


void CEventSystem::UnregisterClientAll(IListener *pClient)
{
	multimap<std::string, IListener*>::iterator mmIter = m_mmClientsData.begin();

	while(mmIter !=m_mmClientsData.end())
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_mmClientsData.erase(mmIter);
		}
		else
			mmIter++;
	}
}


void CEventSystem::DispatchEvent(CEvent *pEvent)
{
	pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	range = m_mmClientsData.equal_range(pEvent->GetEventID());

	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		(*mmIter).second->HandleEvent(pEvent);
	}
}

bool CEventSystem::AlreadyRegistered(EVENTID eventID, IListener* pClient)
{
	bool bIsAlreadyRegistered = false;

	pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	range = m_mmClientsData.equal_range(eventID);

	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		if((*mmIter).second == pClient)
		{
			bIsAlreadyRegistered = true;
			break;
		}
	}

	return bIsAlreadyRegistered;
}


void CEventSystem::SendEvent(EVENTID eventID, void* pData, void* pData2)
{
	CEvent newEvent(eventID, pData, pData2);

	m_lCurrentEvents.push_back(newEvent);
}

void CEventSystem::ProcessEvents(void)
{
	while(m_lCurrentEvents.size())
	{
		DispatchEvent(&m_lCurrentEvents.front());
		m_lCurrentEvents.pop_front();
	}
}

void CEventSystem::ClearEvents(void)
{
	m_lCurrentEvents.clear();
}

void CEventSystem::ShutdownEventSystem(void)
{
	m_mmClientsData.clear();
}
