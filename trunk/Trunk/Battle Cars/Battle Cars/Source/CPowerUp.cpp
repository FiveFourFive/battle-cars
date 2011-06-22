#include "CPowerUp.h"

#include "CSGD_Direct3D.h"

PowerUp::PowerUp()
{
	m_nType = SHIELD_POWERUP;
	//SetName("Power Up");
	SetPosX(700);
	SetPosY(350);
}

PowerUp::~PowerUp()
{

}


void PowerUp::Update(float fElapsedTime)
{

}

void PowerUp::Render()
{
	CSGD_Direct3D* m_pD3D = CSGD_Direct3D::GetInstance();

	m_pD3D->DrawRect(GetRect(), 128, 128, 128);
	m_pD3D->DrawTextA("Power_Up", GetPosX(), GetPosY(), 255,255,255);

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

	return false;
}