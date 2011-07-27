////////////////////////////////////////////////
//	File	:	"CObjectManager.cpp"
//
//	Author	:	Wade Shwallon
//
//	Purpose	:	contains and managse all game objects.
/////////////////////////////////////////////////

#include <Windows.h>
#include "CObjectManager.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "IGameModeInterface.h"
#include "CGamePlayState.h"
#include "CSGD_Direct3D.h"
#include "CBase.h"
#include "CHUD.h"
#include "CPowerUp.h"
#include "CCollectable.h"
#include "CNumPlayers.h"

#include "CGame.h"
#include "CLevel.h"
#include "CMap.h"

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
	CheckStatus();
	while(iter != m_vObjectList.end())
	{
		(*iter)->Update(fElapsedTime);
		iter++;
	}
}

void CObjectManager::RenderObjects(CCamera* camera)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	if(m_vObjectList.size() >= 0)
	for(int i = m_vObjectList.size()-1; i >= 0; i--)
	{
		m_vObjectList[i]->Render(camera);

		if( m_vObjectList[i]->GetType() == OBJECT_PLAYER)
		{
			CPlayer* player = (CPlayer*)(m_vObjectList[i]);
			player->GetHudItem()->Render();
			for( size_t index = 0; index < m_vObjectList.size(); index++)
			{
				if( m_vObjectList[index])
				{
					if( m_vObjectList[index]->GetType() == OBJECT_BULLET )
						continue;

					RECT draw_rect;
					CBase* render_object = (CBase*)m_vObjectList[index];
					CPlayer* offset_object = (CPlayer*)m_vObjectList[i];

					float width_scale = 100 / ((float)CLevel::GetInstance()->GetMap()->GetMapWidth() * (float)CLevel::GetInstance()->GetMap()->GetPixelWidth());
					float height_scale = 100 / ((float)CLevel::GetInstance()->GetMap()->GetMapHeight() * (float)CLevel::GetInstance()->GetMap()->GetPixelHeight());

					draw_rect.left = LONG((render_object->GetPosX()*width_scale) + offset_object->GetHudItem()->GetMiniMapXPos());
					draw_rect.top = LONG(render_object->GetPosY()*height_scale + offset_object->GetHudItem()->GetMiniMapYPos() + 20);
					draw_rect.right = draw_rect.left + 5;
					draw_rect.bottom = draw_rect.top + 5;

					// i = THE PLAYER INDEX, index = the rendering objects index
					if( m_vObjectList[i] == m_vObjectList[index])
					{
						pD3D->DrawRect(draw_rect,0,0,255); 
					}
					else if( m_vObjectList[index]->GetType() == OBJECT_ENEMY)
					{
						pD3D->DrawRect(draw_rect,255,0,0); 
					}
					else if( m_vObjectList[index]->GetType() == OBJECT_BOSS)
					{
						pD3D->DrawRect(draw_rect,255,255,255); 
					}
					else if( m_vObjectList[index]->GetType() == OBJECT_POWERUP )
					{
						PowerUp* power = (PowerUp*)(m_vObjectList[index]);
						if( power->IsActive())
							pD3D->DrawRect(draw_rect,255,240,0); 
					}
					else if( m_vObjectList[index]->GetType() == OBJECT_SPEEDRAMP )
					{
						pD3D->DrawRect(draw_rect,255,0,255); 
					}
					else if( m_vObjectList[index]->GetType() == OBJECT_COLLECTABLE)
					{
						CCollectable* collectable = (CCollectable*)m_vObjectList[index];
						if( collectable->IsActive())
							pD3D->DrawRect(draw_rect,0,255,0); 
					}
					else if (CNumPlayers::GetInstance ()->GetNumberOfPlayers () == 2)
					{
						if (m_vObjectList[index]->GetType () == OBJECT_PLAYER)
						{
							pD3D->DrawRect(draw_rect,0,255,255); 
						}
					}
				
				}
			}
		}
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
		m_vObjectList[i] = NULL;
	}
	m_vObjectList.clear();

}

void CObjectManager::CheckCollisions()
{
	for(size_t i = 0; i < m_vObjectList.size(); i++)
	{
		for(size_t m = 0; m < m_vObjectList.size(); m++)
		{
		//	if(m_vObjectList[i]->GetType() != m_vObjectList[m]->GetType())
				if(m_vObjectList[i]->CheckCollision(m_vObjectList[m]))
					break;
		}

	}
}

void CObjectManager::CheckStatus(void)
{
	
	IGameModeInterface* tempmode = (IGameModeInterface*)CGamePlayState::GetInstance()->GetMode();
	for(size_t i = 0; i < m_vObjectList.size(); i++)
	{
		if(m_vObjectList[i]->GetType() == OBJECT_PLAYER || m_vObjectList[i]->GetType() == OBJECT_ENEMY || m_vObjectList[i]->GetType() == OBJECT_BOSS)
		{
			CCar* tempcar = (CCar*)m_vObjectList[i];
			tempmode->CheckCarStatus(tempcar);
		}

	}
}