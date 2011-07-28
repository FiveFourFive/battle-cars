#include <Windows.h>
#include "CHUD.h"
#include "CSGD_Direct3D.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CPrintFont.h"
#include "CGamePlayState.h"
#include <vector>
#include "CGame.h"
#include "CNumPlayers.h"
#include "CCollectionMode.h"

CHUD::CHUD(void)
{
	m_pPF = new CPrintFont(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0)));

	m_nMiniMapID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/huds/minimap_overlay.png",D3DCOLOR_XRGB(255,255,255));
	m_pTM = CSGD_TextureManager::GetInstance();

	m_nScoreBoardID=m_pTM->LoadTexture("resource/graphics/huds/score_hud.png");
	m_nHealthID = m_pTM->LoadTexture("resource/graphics/huds/health_bar_underlay.png");
	m_nMiniBG = m_pTM->LoadTexture("resource/graphics/huds/minimap_underlay.png");
	m_nMiniBG2 = m_pTM->LoadTexture("resource/graphics/huds/minimap_middlelay.png");
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

	m_pTM->Draw(m_nScoreBoardID, int((CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX()), int(pCamera->GetHeight() - 250 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY()),1.2f,1.5f);

	m_pTM->Draw(m_nPistolID,int((CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX()), int(pCamera->GetHeight() - 100 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY()));
	
	m_fMiniMapPosX = 0 + pCamera->GetWidth() - 170 - (CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX();
	m_fMiniMapPosY = 0 + pCamera->GetHeight() - 150 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY();
	minimap.left =	(LONG)(m_fMiniMapPosX);
	minimap.top =	(LONG)(m_fMiniMapPosY);
	minimap.right = (LONG)(minimap.left + 140);
	minimap.bottom =(LONG)(minimap.top + 150);
	//pD3D->DrawRect(minimap,255,255,255);
	m_fMiniMapPosX = 0 + pCamera->GetWidth() - 160 - (CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX();
	minimap.left =	(LONG)(m_fMiniMapPosX);
	minimap.top =	(LONG)(m_fMiniMapPosY);
	minimap.right = (LONG)(minimap.left + 140);
	minimap.bottom =(LONG)(minimap.top + 150);
	m_pTM->Draw(m_nMiniBG,minimap.left-64,minimap.top-64,1.0f,1.0f);
	m_pTM->Draw(m_nMiniBG2,minimap.left-64,minimap.top-64,1.0f,1.0f);
	m_pTM->Draw(m_nMiniMapID,minimap.left-64,minimap.top-64,1.0f,1.0f);
	

	m_pTM->Draw(m_nHealthID, int(128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f)),int(pCamera->GetHeight() - 94 + pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f)));
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	health.left = (LONG)(0 + 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f));
	health.top = (LONG)(0 + pCamera->GetHeight() - 80 + pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f));
	health.right = (LONG)(health.left + (m_pOwner->GetHealth()/m_pOwner->GetMaxHealth()) * 120);
	health.bottom = (LONG)(health.top + 30);
	pD3D->DrawRect(health,0,255,0);

	shield.left =   (LONG)(0 + 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f));
	shield.top =   (LONG)(0 + pCamera->GetHeight() - 80 + pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f));
	shield.right = (LONG)(shield.left + (m_pOwner->GetShieldBar()/m_pOwner->GetMaxShield()) * 120);
	shield.bottom =  (LONG)(shield.top + 30);
	pD3D->DrawRect(shield,105,105,105);

	m_pTM->Draw(m_nHealthID, int(128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f)),int(pCamera->GetHeight() - 50 + pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f)),0.75f,.5f);
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	power.left =   (LONG)(0 + 128 + pCamera->GetRenderPosX() + (CGame::GetInstance()->GetScreenWidth()*0.05f));
	power.top =  (LONG)(8 + pCamera->GetHeight() - 50 + pCamera->GetRenderPosY() - (CGame::GetInstance()->GetScreenHeight()*0.05f));
	power.right =  (LONG)(power.left + (int)((m_pOwner->GetPowerUpBar()/m_pOwner->GetMaxPowerUp()) * 90));
	power.bottom = (LONG)( power.top + 14);
	pD3D->DrawRect(power,0,80,255);

	char scorebuff[32];
	if (CGamePlayState::GetInstance ()->GetMode () == CCollectionMode::GetInstance ())
	{
		sprintf_s(scorebuff, "SCORE:%i", m_pOwner->GetCollected () * 7);
	}
	else
	{
		sprintf_s(scorebuff, "SCORE:%i", m_pOwner->GetKillCount() * 7);
	}
	m_pPF->Print(scorebuff, int(0 + (pCamera->GetWidth()*0.5f) - 70 + pCamera->GetRenderPosX()), int(pCamera->GetHeight() - 30 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY()), 0.75f, D3DCOLOR_XRGB(255,255,255));

	vector<CCar*> scores = CGamePlayState::GetInstance()->GetScores();
	char buffer[32];
	DWORD color = 0;

	int MaxScoreBoard = scores.size();

	if (MaxScoreBoard > 5)
	{
		MaxScoreBoard = 5;
	}

	for(int i = 0; i < MaxScoreBoard; i++)
	{
		if (CGamePlayState::GetInstance ()->GetMode () == CCollectionMode::GetInstance ())
		{
			sprintf_s(buffer,"%i)%i",i+1,scores[i]->GetCollected ());
		}
		else
		{
			sprintf_s(buffer,"%i)%i",i+1,scores[i]->GetKillCount());
		}

  		if (CNumPlayers::GetInstance()->GetNumberOfPlayers () == 2) 
		{
			if (scores[i] == CGamePlayState::GetInstance ()->GetPlayer2 ())
			{
				color = D3DCOLOR_ARGB(255,0,255,255);
			}else if(scores[i] == CGamePlayState::GetInstance ()->GetPlayer1 ())
			{
				color = D3DCOLOR_ARGB(255,0,255,0);
			}else
				color = D3DCOLOR_ARGB(255,255,0,0);

		}else if(scores[i] == CGamePlayState::GetInstance ()->GetPlayer1 ())
		{
			color = D3DCOLOR_ARGB(255,0,255,0);
		}
		else
			color = D3DCOLOR_ARGB(255,255,0,0);

		m_pPF->Print(buffer,int((CGame::GetInstance()->GetScreenWidth()*0.05f) + pCamera->GetRenderPosX()),int(pCamera->GetHeight() - 240 - (CGame::GetInstance()->GetScreenHeight()*0.05f) + pCamera->GetRenderPosY()+(20*i)),1.0f,color);

	}
}	