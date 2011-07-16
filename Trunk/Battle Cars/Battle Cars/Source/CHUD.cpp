#include <Windows.h>
#include "CHUD.h"
#include "CSGD_Direct3D.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CPrintFont.h"
#include "CGamePlayState.h"
#include <vector>
#include "CGame.h"
CHUD::CHUD(void)
{
	m_pPF = new CPrintFont(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0)));

	m_nMiniMapID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/huds/minimap_overlay.png",D3DCOLOR_XRGB(255,255,255));
	m_pTM = CSGD_TextureManager::GetInstance();

	m_nScoreBoardID=m_pTM->LoadTexture("resource/graphics/huds/score_hud.png");
	m_nHealthID = m_pTM->LoadTexture("resource/graphics/huds/health_bar_underlay.png");
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
	m_nPistolID = m_pOwner->GetWeaponIcon();

	RECT health;
	RECT shield;
	RECT power;
	RECT minimap;

	m_pTM->Draw(m_nScoreBoardID, (CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX(), pCamera->GetHeight() - 260 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY());

	m_pTM->Draw(m_nPistolID,(CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX(), pCamera->GetHeight() - 100 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY());

	m_fMiniMapPosX = 0 + pCamera->GetWidth() - 150 - (CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX();
	m_fMiniMapPosY = 0 + pCamera->GetHeight() - 150 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY();
	minimap.left = m_fMiniMapPosX;
	minimap.top = m_fMiniMapPosY;
	minimap.right = minimap.left + 150;
	minimap.bottom = minimap.top + 150;
	m_pTM->Draw(m_nMiniMapID,minimap.left-64,minimap.top-64,1.0f,1.0f);
	pD3D->DrawRect(minimap,255,255,255);

	m_pTM->Draw(m_nHealthID, 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f),pCamera->GetHeight() - 94 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	health.left = 0 + 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	health.top = 0 + pCamera->GetHeight() - 80 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	health.right = (LONG)(health.left + (m_pOwner->GetHealth()/m_pOwner->GetMaxHealth()) * 100);
	health.bottom = health.top + 30;
	pD3D->DrawRect(health,0,255,0);

	shield.left =  0 + 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	shield.top =  0 + pCamera->GetHeight() - 80 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	shield.right = (LONG)(shield.left + (m_pOwner->GetShieldBar()/m_pOwner->GetMaxShield()) * 100);
	shield.bottom = shield.top + 30;
	pD3D->DrawRect(shield,105,105,105);

	m_pTM->Draw(m_nHealthID, 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f),pCamera->GetHeight() - 50 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f),1.0f,.5f);
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	power.left =  0 + 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f);
	power.top = 8 + pCamera->GetHeight() - 50 - pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f);
	power.right = power.left + (int)((m_pOwner->GetPowerUpBar()/m_pOwner->GetMaxPowerUp()) * 80);
	power.bottom = power.top + 14;
	pD3D->DrawRect(power,0,80,255);

	char scorebuff[32];
	sprintf_s(scorebuff, "SCORE:%i", m_pOwner->GetKillCount());
	m_pPF->Print(scorebuff, 0 + (pCamera->GetWidth()*0.5f) - 100 + pCamera->GetRenderPosX(), pCamera->GetHeight() - 30 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY(), 0.75f, D3DCOLOR_XRGB(255,255,255));

	vector<CCar*> scores = CGamePlayState::GetInstance()->GetScores();
	char buffer[32];
	DWORD color = 0;
	for(int i = 0; i < scores.size()-1; i++)
	{

		sprintf_s(buffer,"%i)%i",i+1,scores[i]->GetKillCount());
  		if(scores[i]->GetType() == OBJECT_PLAYER)
		{
			color = D3DCOLOR_ARGB(255,0,255,0);
		}
		m_pPF->Print(buffer,(CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX(),pCamera->GetHeight() - 240 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY()+(20*i),1.0f,color);

	}
}	