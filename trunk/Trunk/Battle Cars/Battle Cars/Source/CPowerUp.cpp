#include "CPowerUp.h"
#include "CCamera.h"
#include "CSGD_Direct3D.h"
#include "CEventSystem.h"
#include "CEvent.h"
#include "CCar.h"
PowerUp::PowerUp()
{
	m_nType = SHIELD_POWERUP;
	//SetName("Power Up");
	SetPosX(1500);
	SetPosY(1700);
	m_fRespawn = 0.0f;
	m_fRespawnDelay = 5.0f;
	m_bActive = true;
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
	if( (GetPosX()- camera->GetCamX()) < 0 || (GetPosX() - camera->GetCamX()) > camera->GetWidth() || GetPosY() - camera->GetCamY() < 0 ||
		GetPosY() - camera->GetCamY() > camera->GetHeight())
		return;

	if(m_bActive)
	{
		CSGD_Direct3D* m_pD3D = CSGD_Direct3D::GetInstance();

		RECT temp_render_rect = { (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())), 0, 0};
		temp_render_rect.right = temp_render_rect.left + 40;
		temp_render_rect.bottom = temp_render_rect.top + 40;

		if( GetPowerUpType() == SPECIAL_POWERUP)
		{
			m_pD3D->DrawRect(temp_render_rect, 0, 0, 255);
			m_pD3D->DrawTextA("Special_Power_Up", (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())), 255,255,255);
		}
		else if( GetPowerUpType() == SHIELD_POWERUP)
		{
			m_pD3D->DrawRect(temp_render_rect, 128, 128, 128);
			m_pD3D->DrawTextA("Shield_Power_Up", (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())), 255,255,255);
		}
		else if( GetPowerUpType() == HEALTH_POWERUP)
		{
			m_pD3D->DrawRect(temp_render_rect, 0, 255, 0);
			m_pD3D->DrawTextA("Health_Power_Up", (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())), 255,255,255);
		}
		else if( GetPowerUpType() == WEAPONS_POWERUP)
		{
			m_pD3D->DrawRect(temp_render_rect, 255, 255, 0);
			m_pD3D->DrawTextA("Weapons_Power_Up", (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())), 255,255,255);

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
					CCar* car = (CCar*)pBase;
					CEventSystem::GetInstance()->SendEvent("weapon_level",car);
				}
				else if(m_nPtype == SPECIAL_POWERUP)
				{
					CEventSystem::GetInstance()->SendEvent("powerup_power",pBase);
				}
				else if(m_nPtype == HEALTH_POWERUP)
				{
					CEventSystem::GetInstance()->SendEvent("health_up", pBase);
				}
				m_bActive = false;
				m_fRespawn = 0.0f;
				return true;
				}
			}
	}
	return false;
}