#include "CPowerUp.h"

#include "CSGD_Direct3D.h"
#include "CEventSystem.h"
#include "CEvent.h"
PowerUp::PowerUp()
{
	m_nType = SHIELD_POWERUP;
	//SetName("Power Up");
	SetPosX(700);
	SetPosY(350);
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

void PowerUp::Render()
{
	if(m_bActive)
	{
	CSGD_Direct3D* m_pD3D = CSGD_Direct3D::GetInstance();

	m_pD3D->DrawRect(GetRect(), 128, 128, 128);
	m_pD3D->DrawTextA("Power_Up", GetPosX(), GetPosY(), 255,255,255);
	}
}

RECT PowerUp::GetRect()
{
	RECT temp;
	temp.left = GetPosX();
	temp.top = GetPosY();
	temp.right = temp.left + 40;
	temp.bottom = temp.top + 40;
	return temp;

}

bool PowerUp::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_PLAYER)
		{
			CEventSystem::GetInstance()->SendEvent("powerup",pBase);
			m_bActive = false;
			m_fRespawn = 0.0f;
			return true;
		}
	}
	return false;
}