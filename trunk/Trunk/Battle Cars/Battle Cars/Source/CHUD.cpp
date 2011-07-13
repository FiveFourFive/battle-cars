#include <Windows.h>
#include "CHUD.h"
#include "CSGD_Direct3D.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CGame.h"
CHUD::CHUD(void)
{

}

CHUD::~CHUD(void)
{

}

void CHUD::Update(float fElapsedTime)
{


}

void CHUD::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	CCamera* pCamera = m_pOwner->GetCamera();

	RECT health;
	RECT shield;
	RECT power;
	RECT weapon;
	RECT minimap;
	RECT score;

	score.left = 0 + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	score.top = 0 + pCamera->GetHeight() - 260 - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	score.right = score.left + 80;
	score.bottom = score.top + 150;
	pD3D->DrawRect(score,255,255,255);

	weapon.left = 0 + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	weapon.top = 0 + pCamera->GetHeight() - 100 - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	weapon.right = weapon.left + 90;
	weapon .bottom = weapon.top + 100;
	switch(m_pOwner->GetSelectedWeapon())
	{
	case WEAPON_PISTOL:
		pD3D->DrawRect(weapon,255,255,255);
		break;
	case WEAPON_RPG:
		pD3D->DrawRect(weapon,255,0,0);
		break;
	case WEAPON_SPECIAL:
		pD3D->DrawRect(weapon,0,0,255);
		break;
	}

	minimap.left = 0 + pCamera->GetWidth() - 150 - (CGame::GetInstance()->GetScreenWidth()*0.05f);
	minimap.top = 0 + pCamera->GetHeight() - 150 - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	minimap.right = minimap.left + 150;
	minimap.bottom = minimap.top + 150;
	pD3D->DrawRect(minimap,255,255,255);

}