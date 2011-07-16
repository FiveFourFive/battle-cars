
#include "CLoadingState.h"
#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "COptionState.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CHowToPlayState.h"
#include "CGamerProfile.h"
#include "CXboxInput.h"
#include "CWinState.h"
#include "CKeyBinds.h"
#include "CNumPlayers.h"
#include "CLevelSelectionState.h"

/*
CLoadingState * CLoadingState::GetInstance()
{
	static CLoadingState instance;
	return &instance;
}*/

void CLoadingState::Enter(void)
{
//	CPsuedoSync * pSync = CPsuedoSync::GetInstance();

	
	CMessageSystem * m_pMS = CMessageSystem::GetInstance();
	m_pMS->InitMessageSystem(MessageProc);
//	pSync->_ASSIGN((&CMessageSystem::SendMsg),m_pMS,new CCreateLevelMessage());
	//pSync->_ASSIGN(CMessageSystem::ProcessMessages, m_pMS,NULL);

	/*
	m_pPF = new CPrintFont(m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0)));
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/Superbeast.mp3",SGD_FMOD_LOOPING);
	m_nCountDown = m_pFM->LoadSound("resource/sounds/Countdown.mp3");
	m_nCountDownEnd = m_pFM->LoadSound("resource/sounds/Countdowntone.mp3");
	m_pFM->PlaySound(m_nBackgroundMusicID);
	CGame::GetInstance()->ResetInputDelay();
	dummy = new CEnemy(CCharacterSelection::GetInstance()->GetPlayer1()->GetController());
	PowerUp* power_up = new PowerUp();
	power_up->SetPosX(1000.0f);
	power_up->SetPosY(1000.0f);
	power_up->SetType(OBJECT_POWERUP);
	power_up->SetPowerType(HEALTH_POWERUP);
	power_ups.push_back(power_up);
	power_up = new PowerUp();
	power_ups.push_back(power_up);
	power_up = new PowerUp();
	power_ups.push_back(power_up);
	dummy2 = new CCar();
	dummy->SetPosX(1200);
	dummy->SetPosY(1200);
	dummy->SetHealth(100.0f);
	dummy->SetShieldBar(0.0f);
	dummy->SetVelX(0);
	dummy->SetVelY(0);
	dummy->SetSpeed(0);
	dummy->SetMaxHealth(100);
	dummy->SetType(OBJECT_ENEMY);
	dummy->SetKillCount(5);
	dummy->Rotate(0.0f);
	dummy2->SetPosX(450);
	dummy2->SetPosY(325);
	dummy2->SetHealth(100);
	dummy2->SetVelX(0);
	dummy2->SetVelY(0);
	dummy2->SetSpeed(0);
	dummy2->SetType(OBJECT_ENEMY);
	dummy2->SetKillCount(5);
	dummy2->Rotate(0.0f);
	dummy->EnterState ();
	speedy = new CSpeedRamp();
	m_pOM->AddObject(dummy2);
	m_pOM->AddObject(speedy);
	m_pOM->AddObject(dummy);
	m_pOM->AddObject(power_ups[0]);
	m_pOM->AddObject(power_ups[1]);
	m_pOM->AddObject(power_ups[2]);
	characters = CCharacterSelection::GetInstance()->GetList();
	player = CCharacterSelection::GetInstance()->GetPlayer1();
	if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
	{
		player2 = CCharacterSelection::GetInstance()->GetPlayer2();
		m_pOM->AddObject(player2);
	}
	else
	{
		int player2index = rand()%4;
		while(player2index == CCharacterSelection::GetInstance()->GetPlayer1()->GetPlayerType())
			player2index = rand()%4;
		player2 = characters[player2index];
	}
	player2->SetPosX(400);
	player->Rotate(0.0f);
	player2->Rotate(0.0f);
	player2->SetPosX(500);
	player2->SetPosY(400);
	player->SetPlayerNum(1);
	player->SetType(OBJECT_PLAYER);
	player->Rotate(0);
	player2->Rotate(0);
	player2->SetController(m_pController2);
	player->SetPosX(400);
	player->SetPosY(400);
	m_pOM->AddObject(player);
	power_ups[1]->SetPosX(player->GetPosX() + 200);
	power_ups[1]->SetPosY(player->GetPosY() + 200);
	power_ups[1]->SetType(OBJECT_POWERUP);
	power_ups[1]->SetPowerType(WEAPONS_POWERUP);
	power_ups[2]->SetPosX(player->GetPosX() + 400);
	power_ups[2]->SetPosY(player->GetPosY() + 400);
	power_ups[2]->SetType(OBJECT_POWERUP);
	power_ups[2]->SetPowerType(SPECIAL_POWERUP);
	m_pOM->AddObject(power_up_power);
	m_bCountDown = false;
	m_fEnlarge = 0.0f;
	m_bPlaying = false;
	m_fCountDown = 0.0f;
	Level->SetSpawn (player2);
	m_pPM->LoadEmittor("resource/data/collision.xml");
	m_pPM->LoadEmittor("resource/data/missle_flame.xml");
	time = 60;
	m_fElapsedSecond = 0.0f;
	score = 0;
	m_pPlayer1KB = CGamerProfile::GetInstance()->GetActiveProfile()->GetControllerBinds();
	m_pPlayer1KeyboardKB = CGamerProfile::GetInstance()->GetActiveProfile()->GetKeyboardBinds();
	m_nMiniMapOverlayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_overlay.png");
	m_nMiniMapMiddlelayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_middlelay.png");
	m_nMiniMapUnderlayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_underlay.png");
	*/

}

void CLoadingState::Exit(void)
{
}

bool CLoadingState::Input(void)
{
	return true;
}

void CLoadingState::Update(float fElapsedTime)
{
}

void CLoadingState::Render(void)
{

}


void CLoadingState::MessageProc(CBaseMessage* pMsg)
{
	switch(pMsg->GetMsgID())
	{
	case MSG_CREATE_LEVEL:
		{
			CLevel* level = CGamePlayState::GetInstance ()->GetLevel();
			std::string level_file_name = "resource/data/";
			char buffer[32];
			sprintf_s(buffer,32, "level%i/", CLevelSelectionState::GetInstance()->GetSelection() + 1);
			level_file_name += buffer;
			std::string temp_file = CLevelSelectionState::GetInstance()->GetLevel(CLevelSelectionState::GetInstance()->GetSelection())->FileName;
			level_file_name += temp_file;

			level->Load (level_file_name.c_str());
			break;
		}
	};
}