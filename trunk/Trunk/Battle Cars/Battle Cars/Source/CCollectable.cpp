#include "CCollectable.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CCamera.h"
#include "CGamePlayState.h"
#include "CLevel.h"
#include "CMap.h"

CCollectable::CCollectable()
{
	m_nType = OBJECT_COLLECTABLE;
	int minX = 160;
	int minY = 160;
	int maxX = 6240;//((CLevel::GetInstance ()->GetMap()->GetMapWidth ()) - 30) * (CLevel::GetInstance ()->GetMap()->GetPixelWidth ());
	int maxY = 6240;//((CLevel::GetInstance ()->GetMap()->GetMapHeight ()) - 30) * (CLevel::GetInstance ()->GetMap()->GetPixelHeight ());
	SetPosX((float)(rand()% (maxX - minX + 1) - minX));
	SetPosY((float)(rand()% (maxY - minY + 1) - minY));
	SetWidth(128);
	SetHeight(128);
	
	m_bActive = true;
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_CollectablePlaceHolder.png"));
}

CCollectable::~CCollectable()
{
}

void CCollectable::Update(float fElapsedTime)
{
}

void CCollectable::Render(CCamera* camera)
{
	if(m_bActive)
	{
		CSGD_TextureManager* m_pTM = CSGD_TextureManager::GetInstance();
		m_pTM->Draw(GetImageID(), (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY()))); 
	}
}

bool CCollectable::CheckCollision(IBaseInterface* pBase)
{
	if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_PLAYER)
	{
		RECT collision;
		if(IntersectRect(&collision, &GetRect(), &pBase->GetRect()))
		{
			
			if(pBase->GetType() == OBJECT_PLAYER && m_bActive)
			{
				//Play a sound here as well
				m_bActive = false;
				CGamePlayState::GetInstance()->SetPlayerCollectables(CGamePlayState::GetInstance()->GetPlayerCollectables() + 1);
			}
			else if(pBase->GetType() == OBJECT_ENEMY && m_bActive)
			{
				//play a sound here as well
				m_bActive = false;
				CGamePlayState::GetInstance()->SetComputerCollectables(CGamePlayState::GetInstance()->GetComputerCollectables()+1);
			}
		}
	}
	return false;
}

RECT CCollectable::GetRect()
{
	RECT temprect;
	temprect.left = (int)GetPosX();
	temprect.top = (int)GetPosY();
	temprect.right = temprect.left + GetWidth();
	temprect.bottom = temprect.top + GetHeight();
	return temprect;
}

