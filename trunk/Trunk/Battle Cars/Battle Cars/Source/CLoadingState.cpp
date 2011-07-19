
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
#include "CPowerUp.h"
#include "CEnemy.h"
#include "CObjectManager.h"
#include "CCharacterSelection.h"
#include "CLevelSelectionState.h"


CLoadingState * CLoadingState::GetInstance()
{
	static CLoadingState instance;
	return &instance;
}
bool CLoadingState::HandleEnter()
{
	return false;
}
void CLoadingState::Enter(void)
{	
	/*
	timeStamp = timeGetTime();
	CLevel* level = CGamePlayState::GetInstance ()->GetLevel();
	std::string level_file_name = "resource/data/";
	char buffer[32];
	sprintf_s(buffer,32, "level%i/", CLevelSelectionState::GetInstance()->GetSelection() + 1);
	level_file_name += buffer;
	std::string temp_file = CLevelSelectionState::GetInstance()->GetLevel(CLevelSelectionState::GetInstance()->GetSelection())->FileName;
	level_file_name += temp_file;

	level->Load (level_file_name.c_str());
	*/
	
	//CGamePlayState::GetInstance()->SetPrintFont(new CPrintFont(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0))));
	//CGamePlayState::GetInstance()->SetBGMusicID( CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Superbeast.mp3",SGD_FMOD_LOOPING));
	//CGamePlayState::GetInstance()->SetCountDown(CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdown.mp3"));
	//CGamePlayState::GetInstance()->SetCountDownEnd(CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdowntone.mp3"));
	//CSGD_FModManager::GetInstance()->PlaySound(CGamePlayState::GetInstance()->GetBGMusicID());
	//CGame::GetInstance()->ResetInputDelay();

	//CGamePlayState::GetInstance()->SetDummy(new CEnemy(CCharacterSelection::GetInstance()->GetPlayer1()->GetController()));
	//PowerUp* power_up = new PowerUp();
	//power_up->SetPosX(1000.0f);
	//power_up->SetPosY(1000.0f);
	//power_up->SetType(OBJECT_POWERUP);
	//power_up->SetPowerType(HEALTH_POWERUP);
	//CGamePlayState::GetInstance()->GetPUPS().push_back(power_up);
	//power_up = new PowerUp();
	//CGamePlayState::GetInstance()->GetPUPS().push_back(power_up);
	//power_up = new PowerUp();
	//CGamePlayState::GetInstance()->GetPUPS().push_back(power_up);
	//
	//CGamePlayState::GetInstance()->SetDummy2(new CCar());
	//CGamePlayState::GetInstance()->GetDummy()->SetPosX(1200);
	//CGamePlayState::GetInstance()->GetDummy()->SetPosY(1200);
	//CGamePlayState::GetInstance()->GetDummy()->SetHealth(100.0f);
	//CGamePlayState::GetInstance()->GetDummy()->SetShieldBar(0.0f);
	//CGamePlayState::GetInstance()->GetDummy()->SetVelX(0);
	//CGamePlayState::GetInstance()->GetDummy()->SetVelY(0);
	//CGamePlayState::GetInstance()->GetDummy()->SetSpeed(0);
	//CGamePlayState::GetInstance()->GetDummy()->SetMaxHealth(100);
	//CGamePlayState::GetInstance()->GetDummy()->SetType(OBJECT_ENEMY);
	//CGamePlayState::GetInstance()->GetDummy()->SetKillCount(5);
	//CGamePlayState::GetInstance()->GetDummy()->Rotate(0.0f);
	//CGamePlayState::GetInstance()->GetDummy2()->SetPosX(450);
	//CGamePlayState::GetInstance()->GetDummy2()->SetPosY(325);
	//CGamePlayState::GetInstance()->GetDummy2()->SetHealth(100);
	//CGamePlayState::GetInstance()->GetDummy2()->SetVelX(0);
	//CGamePlayState::GetInstance()->GetDummy2()->SetVelY(0);
	//CGamePlayState::GetInstance()->GetDummy2()->SetSpeed(0);
	//CGamePlayState::GetInstance()->GetDummy2()->SetType(OBJECT_ENEMY);
	//CGamePlayState::GetInstance()->GetDummy2()->SetKillCount(5);
	//CGamePlayState::GetInstance()->GetDummy2()->Rotate(0.0f);
	//CGamePlayState::GetInstance()->GetDummy()->EnterState ();
	//CSpeedRamp* speedy = new CSpeedRamp();
	//CGamePlayState::GetInstance()->GetRamps().push_back(speedy);
	//CGamePlayState::GetInstance()->GetDummy()->SetSpeedRamps(CGamePlayState::GetInstance()->GetRamps());
	//CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetDummy2());
	//CObjectManager::GetInstance()->AddObject(speedy);
	//CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetDummy());
	//CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetPUPS()[0]);
	//CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetPUPS()[1]);
	//CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetPUPS()[2]);
	//
	//CGamePlayState::GetInstance()->SetCharacters(CCharacterSelection::GetInstance()->GetList());
	//CGamePlayState::GetInstance()->SetPlayer1(CCharacterSelection::GetInstance()->GetPlayer1());
	//if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
	//{
	//	CGamePlayState::GetInstance()->SetPlayer2(CCharacterSelection::GetInstance()->GetPlayer2());
	//	CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetPlayer2());
	//}
	//else
	//{
	//	int player2index = rand()%4;
	//	while(player2index == CCharacterSelection::GetInstance()->GetPlayer1()->GetPlayerType())
	//		player2index = rand()%4;
	//	CGamePlayState::GetInstance()->SetPlayer2(CGamePlayState::GetInstance()->GetCharacters()[player2index]);
	//}
	//CGamePlayState::GetInstance()->GetPlayer2()->SetPosX(400);
	//CGamePlayState::GetInstance()->GetPlayer1()->Rotate(0.0f);
	//CGamePlayState::GetInstance()->GetPlayer2()->Rotate(0.0f);
	//CGamePlayState::GetInstance()->GetPlayer2()->SetPosX(500);
	//CGamePlayState::GetInstance()->GetPlayer2()->SetPosY(400);
	//CGamePlayState::GetInstance()->GetPlayer1()->SetPlayerNum(1);
	//CGamePlayState::GetInstance()->GetPlayer1()->SetType(OBJECT_PLAYER);
	//CGamePlayState::GetInstance()->GetPlayer1()->Rotate(0);
	//CGamePlayState::GetInstance()->GetPlayer2()->Rotate(0);
	//CGamePlayState::GetInstance()->GetPlayer2()->SetController(CGamePlayState::GetInstance()->GetController2());
	//CGamePlayState::GetInstance()->GetPlayer1()->SetPosX(400);
	//CGamePlayState::GetInstance()->GetPlayer1()->SetPosY(400);
	//CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetPlayer1());
	//CGamePlayState::GetInstance()->GetPUPS()[1]->SetPosX(CGamePlayState::GetInstance()->GetPlayer1()->GetPosX() + 200);
	//CGamePlayState::GetInstance()->GetPUPS()[1]->SetPosY(CGamePlayState::GetInstance()->GetPlayer1()->GetPosY() + 200);
	//CGamePlayState::GetInstance()->GetPUPS()[1]->SetType(OBJECT_POWERUP);
	//CGamePlayState::GetInstance()->GetPUPS()[1]->SetPowerType(WEAPONS_POWERUP);
	//CGamePlayState::GetInstance()->GetPUPS()[2]->SetPosX(CGamePlayState::GetInstance()->GetPlayer1()->GetPosX() + 400);
	//CGamePlayState::GetInstance()->GetPUPS()[2]->SetPosY(CGamePlayState::GetInstance()->GetPlayer1()->GetPosY() + 400);
	//CGamePlayState::GetInstance()->GetPUPS()[2]->SetType(OBJECT_POWERUP);
	//CGamePlayState::GetInstance()->GetPUPS()[2]->SetPowerType(SPECIAL_POWERUP);
	//CObjectManager::GetInstance()->AddObject(CGamePlayState::GetInstance()->GetPUPpower());
	/*
	m_bCountDown = false;
	m_fEnlarge = 0.0f;
	m_bPlaying = false;
	m_fCountDown = 0.0f;
	Level->SetSpawn (CGamePlayState::GetInstance()->GetPlayer2());
	m_pPM->LoadEmittor("resource/data/collision.xml");
	m_pPM->LoadEmittor("resource/data/missle_flame.xml");
	time = 60;
	m_fElapsedSecond = 0.0f;
	score = 0;
	m_pPlayer1KB = CGamerProfile::GetInstance()->GetActiveProfile()->GetControllerBinds();
	m_pPlayer1KeyboardKB = CGamerProfile::GetInstance()->GetActiveProfile()->GetKeyboardBinds();
	m_nMiniMapOverlayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_overlay.png");
	m_nMiniMapMiddlelayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_middlelay.png");
	m_nMiniMapUnderlayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_underlay.png");*/
	
	CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
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
//	if(timeGetTime()-timeStamp>1000)
//	{

//	}
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