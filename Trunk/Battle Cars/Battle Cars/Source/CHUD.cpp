#include <Windows.h>
#include "CHUD.h"
#include "CSGD_Direct3D.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CPrintFont.h"
#include "CGame.h"
CHUD::CHUD(void)
{
	m_pPF = new CPrintFont(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0)));

	m_nMiniMapID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/huds/minimap_overlay.png",D3DCOLOR_XRGB(255,255,255));
	m_pTM = CSGD_TextureManager::GetInstance();

	//m_nHealthID = m_pTM->LoadTexture("resource
//	m_nScoreBoardID
	//m_nPistolID = m_pTM->LoadTexture("reoursce
	//m_nMissileID
}

CHUD::~CHUD(void)
{
	delete m_pPF;
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

	score.left = 0 + (CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX();
	score.top = 0 + pCamera->GetHeight() - 260 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY();
	score.right = score.left + 80;
	score.bottom = score.top + 150;
	pD3D->DrawRect(score,255,255,255);

	weapon.left = 0 + (CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX();;
	weapon.top = 0 + pCamera->GetHeight() - 100 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY();
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

	minimap.left = 0 + pCamera->GetWidth() - 150 - (CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX();
	minimap.top = 0 + pCamera->GetHeight() - 150 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY();
	minimap.right = minimap.left + 150;
	minimap.bottom = minimap.top + 150;
	m_pTM->Draw(m_nMiniMapID,minimap.left-64,minimap.top-64,1.0f,1.0f);
	pD3D->DrawRect(minimap,255,255,255);

	health.left = 0 + 90 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	health.top = 0 + pCamera->GetHeight() - 80 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	health.right = (LONG)(health.left + (m_pOwner->GetHealth()/m_pOwner->GetMaxHealth()) * 100);
	health.bottom = health.top + 30;
	pD3D->DrawRect(health,0,255,0);

	shield.left =  0 + 90 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	shield.top =  0 + pCamera->GetHeight() - 80 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	shield.right = (LONG)(shield.left + (m_pOwner->GetShieldBar()/m_pOwner->GetMaxShield()) * 100);
	shield.bottom = shield.top + 30;
	pD3D->DrawRect(shield,105,105,105);

	power.left =  0 + 90 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	power.top = 0 + pCamera->GetHeight() - 50 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	power.right = power.left + (int)((m_pOwner->GetPowerUpBar()/m_pOwner->GetMaxPowerUp()) * 80);
	power.bottom = power.top + 20;
	pD3D->DrawRect(power,0,80,255);

	char scorebuff[32];
	sprintf_s(scorebuff, "SCORE:%i", m_pOwner->GetKillCount());
	m_pPF->Print(scorebuff, 0 + (pCamera->GetWidth()*0.5f) - 100 + pCamera->GetRenderPosX(), pCamera->GetHeight() - 30 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY(), 0.75f, D3DCOLOR_XRGB(255,255,255));
}