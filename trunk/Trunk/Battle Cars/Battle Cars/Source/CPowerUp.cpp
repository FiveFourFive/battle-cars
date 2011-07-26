#include "CPowerUp.h"
#include "CCamera.h"
#include "CSGD_Direct3D.h"
#include "CEventSystem.h"
#include "CEvent.h"
#include "CCar.h"
#include "CSGD_TextureManager.h"
PowerUp::PowerUp()
{
	m_nType = SHIELD_POWERUP;
	//SetName("Power Up");
	SetPosX(1500);
	SetPosY(1700);
	m_fRespawn = 0.0f;
	m_fRespawnDelay = 5.0f;
	m_bActive = true;
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/healthup.png"));
}

PowerUp::~PowerUp()
{

}


void PowerUp::Update(float fElapsedTime)
{
	if(!m_bActive)
		m_fRespawn += fElapsedTime;
	if(m_fRespawn >= m_fRespawnDelay)
		m_bActive = true;
}

void PowerUp::Render(CCamera* camera)
{
	RECT intersect;

	if (!(IntersectRect (&intersect, &GetRect(), &camera->GetRect ())))
	{
		return;
	}

	if(m_bActive)
	{
		CSGD_Direct3D* m_pD3D = CSGD_Direct3D::GetInstance();

		if( GetPowerUpType() == SPECIAL_POWERUP)
		{
			CSGD_TextureManager::GetInstance()->Draw(GetImageID(), (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), 
				(int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())));
		}
		else if( GetPowerUpType() == SHIELD_POWERUP)
		{
			CSGD_TextureManager::GetInstance()->Draw(GetImageID(), (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), 
			(int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())));
		}
		else if( GetPowerUpType() == HEALTH_POWERUP)
		{
			CSGD_TextureManager::GetInstance()->Draw(GetImageID(), (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), 
			(int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())));
		}
		else if( GetPowerUpType() == WEAPONS_POWERUP)
		{
			CSGD_TextureManager::GetInstance()->Draw(GetImageID(), (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), 
			(int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())));
		}

	}
}

RECT PowerUp::GetRect()
{
	RECT temp;
	temp.left = (int)GetPosX();
	temp.top = (int)GetPosY();
	temp.right = temp.left + 40;
	temp.bottom = temp.top + 40;
	return temp;

}

bool PowerUp::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(pBase == this)
		return false;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(m_bActive)
			{
				if(pBase->GetType() == OBJECT_PLAYER || pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_BOSS)
				{
				if(m_nPtype == WEAPONS_POWERUP)
				{
					CEventSystem::GetInstance()->SendEvent("weapon_level",pBase);
				}
				else if(m_nPtype == SPECIAL_POWERUP)
				{
					CEventSystem::GetInstance()->SendEvent("powerup_power",pBase);
				}
				else if(m_nPtype == HEALTH_POWERUP)
				{
					CEventSystem::GetInstance()->SendEvent("health_up", pBase);
				}
				else if(m_nPtype == SHIELD_POWERUP)
				{
					CEventSystem::GetInstance()->SendEvent("powerup_shield", pBase);
				}
				m_bActive = false;
				m_fRespawn = 0.0f;
				return true;
				}
			}
	}
	return false;
}