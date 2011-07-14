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
	/*m_ClientDatabase.Insert (eventID, pClient);*/

}

void CEventSystem::UnregisterClient(EVENTID eventID, IListener *pClient)
{
	/*pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	range = m_ClientDatabase.equal_range(eventID);

	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_ClientDatabase.erase(mmIter);
			break;
		}
	}*/

	for (size_t index = 0; index < m_ClientEvents.size (); index++)
	{
		if (m_ClientEvents[index] == eventID && m_Clients[index] == pClient)
		{
			m_ClientEvents.erase (m_ClientEvents.begin () + index);
			m_Clients.erase (m_Clients.begin () + index);
			break;
		}
	}
	//m_ClientEvents.push_back (eventID);
	//m_Clients.push_back (pClient);

	/*for (int index = 0; index < m_ClientDatabase.GetListAmount(); index++)
	{
		if (*(m_ClientDatabase.second (index)) == pClient)
		{
			m_ClientDatabase.Earse (pClient);
			break;
		}
	}*/
}


void CEventSystem::UnregisterClientAll(IListener *pClient)
{
	/*multimap<std::string, IListener*>::iterator mmIter = m_ClientDatabase.begin();

	while(mmIter !=m_ClientDatabase.end())
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_ClientDatabase.erase(mmIter);
		}
		else
			mmIter++;
	}*/

	for (size_t index = 0; index < m_ClientEvents.size (); index++)
	{
		if (m_Clients[index] == pClient)
		{
			m_ClientEvents.erase (m_ClientEvents.begin () + index);
			m_Clients.erase (m_Clients.begin () + index);
			index --;
		}
	}

	/*m_ClientEvents.push_back (eventID);
	m_Clients.push_back (pClient);*/

	/*for (int index = 0; index < m_ClientDatabase.GetListAmount(); index++)
	{
		if (*(m_ClientDatabase.second (index)) == pClient)
		{
			m_ClientDatabase.Earse (pClient);
			index--;
		}
	}*/
}


void CEventSystem::DispatchEvent(CEvent *pEvent)
{

	/*pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	range = m_ClientDatabase.equal_range(pEvent->GetEventID());


	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		(*mmIter).second->HandleEvent(pEvent);
	}*/

	for (size_t index = 0; index < m_Clients.size (); index++)
	{
		(*(m_Clients.begin () + index))->HandleEvent (pEvent);
	}

	/*m_ClientEvents.push_back (eventID);
	m_Clients.push_back (pClient)*/;

	/*ClientDatabase<EVENTID, IListener*>* range = m_ClientDatabase.Equal_Range (pEvent->GetEventID ());

	for (int index = 0; index < m_ClientDatabase.GetListAmount(); index++)
	{
		(*m_ClientDatabase.second (index))->HandleEvent (pEvent);
	}*/
}

bool CEventSystem::AlreadyRegistered(EVENTID eventID, IListener* pClient)
{
	bool IsRegistered = false;
	bool IsClientRegistered = false;

	/*pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	range = m_ClientDatabase.equal_range(eventID);

	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		if((*mmIter).second == pClient)
		{
			bIsAlreadyRegistered = true;
			break;
		}
	}*/

	

	for( size_t i = 0; i < m_Clients.size(); i++)
	{
		if((*(m_Clients.begin () + i)) == pClient)
		{
			IsClientRegistered = true;

			for (size_t index = 0; index < m_ClientEvents.size (); index++)
			{
				if ((*(m_ClientEvents.begin () + index)) == eventID)
				{
					IsRegistered = true;
					break;
				}
			}
		}
	}

	if( !IsRegistered )
		m_ClientEvents.push_back (eventID);

	if( !IsClientRegistered )
		m_Clients.push_back (pClient);

	return IsRegistered;
}


void CEventSystem::SendEvent(EVENTID eventID, void* pData, void* pData2)
{
	CEvent newEvent(eventID, pData, pData2);

	m_CurrentEvents.push_back(newEvent);
}

void CEventSystem::ProcessEvents(void)
{
	while(m_CurrentEvents.size())
	{
		DispatchEvent(&m_CurrentEvents.front());
		m_CurrentEvents.pop_front();
	}
}

void CEventSystem::ClearEvents(void)
{
	m_CurrentEvents.clear();
}

void CEventSystem::ShutdownEventSystem(void)
{
	//m_ClientDatabase.Clear();
	m_ClientEvents.clear ();
	m_Clients.clear ();
}
