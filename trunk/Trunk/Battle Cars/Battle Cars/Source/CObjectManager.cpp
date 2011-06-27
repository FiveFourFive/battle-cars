////////////////////////////////////////////////
//	File	:	"CObjectManager.cpp"
//
//	Author	:	Wade Shwallon
//
//	Purpose	:	contains and managse all game objects.
/////////////////////////////////////////////////

#include "CObjectManager.h"
#include "CPlayer.h"
#include "CCamera.h"

CObjectManager* CObjectManager::m_pInstance = NULL;

CObjectManager::CObjectManager(void)
{

}

CObjectManager::~CObjectManager(void)
{

}

CObjectManager* CObjectManager::GetInstance(void)
{
	if(m_pInstance == NULL)
		m_pInstance = new CObjectManager();
	return m_pInstance;
}

void CObjectManager::DeleteInstance(void)
{
	if(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CObjectManager::UpdateObjects(float fElapsedTime)
{
	vector<IBaseInterface*>::iterator iter = m_vObjectList.begin();
	CheckCollisions();
	while(iter != m_vObjectList.end())
	{
		(*iter)->Update(fElapsedTime);
		iter++;
	}
}

void CObjectManager::RenderObjects(CCamera* camera)
{
	//CCamera* temp_player_camera = NULL;
	for(size_t i = 0; i < m_vObjectList.size(); i++)
	{
		/*CPlayer* temp_player = (CPlayer*)m_vObjectList[i];
		if( temp_player->GetType() == OBJECT_PLAYER)
			temp_player_camera = temp_player->GetCamera();*/

		//if( temp_player_camera )
			m_vObjectList[i]->Render(camera);
	}
}

void CObjectManager::AddObject(IBaseInterface* pObject)
{
	if(pObject == NULL)
		return;
	
	m_vObjectList.push_back(pObject);

	pObject->AddRef();
}

void CObjectManager::RemoveObject(IBaseInterface* pObject)
{
	if(pObject == NULL)
		return;
	vector<IBaseInterface*>::iterator iter = m_vObjectList.begin();
	for(; iter != m_vObjectList.end(); iter++)
	{
		if((*iter) == pObject)
		{
			(*iter)->Release();

			iter = m_vObjectList.erase(iter);
			break;
		}
	}

}

void CObjectManager::RemoveAllObjects(void)
{
	for(size_t i = 0; i < m_vObjectList.size(); i++)
	{
		m_vObjectList[i]->Release();
	}
	m_vObjectList.clear();

}

void CObjectManager::CheckCollisions()
{
	for(size_t i = 0; i < m_vObjectList.size(); i++)
	{
		for(size_t m = 0; m < m_vObjectList.size(); m++)
		{
			if(m_vObjectList[i]->GetType() != m_vObjectList[m]->GetType())
				if(m_vObjectList[i]->CheckCollision(m_vObjectList[m]))
					break;
		}

	}
}