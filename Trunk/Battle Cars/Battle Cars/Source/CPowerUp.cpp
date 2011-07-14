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

		m_pD3D->DrawRect(temp_render_rect, 128, 128, 128);
		m_pD3D->DrawTextA("Power_Up", (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY())), 255,255,255);
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
				if(pBase->GetType() == OBJECT_PLAYER || pBase->GetType() == OBJECT_ENEMY)
				{
				if(m_nPtype == WEAPONS_POWERUP)
				{
					CCar* car = (CCar*)pBase;
					CEventSystem::GetInstance()->SendEvent("weapon_level",car);
				}
				else if(m_nPtype == SHIELD_POWERUP)
				{
					CEventSystem::GetInstance()->SendEvent("powerup_power",pBase);
				}
				m_bActive = false;
				m_fRespawn = 0.0f;
				return true;
				}
			}
	}
	return false;
}