#include "CGamePlayState.h"

#include "CObjectManager.h"

#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"

#include "CStopWatch.h"
#include "CPrintFont.h"

#include "CGame.h"
#include "CMainMenuState.h"
#include "CLossState.h"
#include "CWinState.h"
#include "COptionState.h"
#include "CPauseMenuState.h"
#include "CBullet.h"
#include "CSpeedRamp.h"
#include "CPlayer.h"
#include "CCar.h"
#include "SGD_Math.h"
#include "CCamera.h"
#include "CEnemy.h"
#include "CPowerUp.h"

#include "CLevel.h"

#include <shlobj.h>
#include <direct.h>

#include "CXboxInput.h"
#include "ParticleManager.h"
#include "Emittor.h"
#include "tinyxml.h"
#include "CLandMine.h"
#include "CNumPlayers.h"
#include "CCharacterSelection.h"
#include "CLevelSelectionState.h"
#include "CTutorialMode.h"
#include "CDeathmatchMode.h"
#include "CKeyboardKeyBinds.h"
#include "CKeyBinds.h"
#include "CGamerProfile.h"
#include "Gamer_Profile.h"
#include "CBoss.h"
#include "CObstacle.h"
#include "CCollectable.h"
#include "CCollectionMode.h"
#include "CCollectState.h"
#include "CHUD.h"
#include "CMap.h"
#include "CThread.h"
void LoadCharacters();


CGamePlayState::CGamePlayState(void)
{
	m_pD3D = NULL;
	m_pTM =	NULL;
	m_pFM =	NULL;
	m_pDI =	NULL;
	m_pOM =	NULL;
	m_pOF =	NULL;
	m_pMS = NULL;
	m_pPF = NULL;
	m_pES = NULL;

	m_pPlayer1KB			 = NULL;
	m_pPlayer1KeyboardKB	 = NULL;
}

CGamePlayState::CGamePlayState(const CGamePlayState&)
{

}

CGamePlayState& CGamePlayState::operator=(const CGamePlayState&)
{
	return *this;
}

CGamePlayState* CGamePlayState::GetInstance(void)
{
	static CGamePlayState instance;
	return &instance;
}


void CGamePlayState::Enter(void)
{
	//m_pMode = new CDeathmatchMode();

	int offset = 0;

	m_pD3D	=	CSGD_Direct3D::GetInstance();


	m_pTM	=	CSGD_TextureManager::GetInstance();
	m_pFM	=	CSGD_FModManager::GetInstance();
	m_pDI	=	CSGD_DirectInput::GetInstance();
	m_pOM	=	CObjectManager::GetInstance();
	m_pOF	=	CObjectFactory<string, CBase>::GetInstance();
	m_pMS	=	CMessageSystem::GetInstance ();
	m_pES	=	CEventSystem::GetInstance ();

	m_pPM	=	ParticleManager::GetInstance();
	//m_pMS->InitMessageSystem (MessageProc);
	m_pPF = new CPrintFont(m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0)));

	//CSGD_FModManager::GetInstance()->PlaySound(m_nBackgroundMusicID);
	/*m_pMS->SendMsg (new CCreateLevelMessage());
	m_pMS->ProcessMessages ();*/

	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();

	CGame::GetInstance()->ResetInputDelay();
	player = CCharacterSelection::GetInstance()->GetPlayer1();
	player->Rotate(0.0f);
	player->SetPlayerNum(1);
	player->SetType(OBJECT_PLAYER);
	player->Rotate(0);
	CBoss* boss = new CBoss(CCharacterSelection::GetInstance()->GetPlayer1()->GetController());
	CBoss* miniboss = new CBoss(CCharacterSelection::GetInstance()->GetPlayer1()->GetController());
	miniboss->SetMiniBoss(true);
	miniboss->SetHealth(200.0f);
	miniboss->SetMaxHealth(200.0f);
	miniboss->SetShieldBar(100.0f);
	miniboss->SetMaxShield(100.0f);
	miniboss->SetAcceleration(2.0f);
	miniboss->SetPosX(float(rand()%1400+200));
	miniboss->SetPosY(float(rand()%1400+200));
	miniboss->SetCarId(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniBossPlaceHolder.png"));

	player->SetName ("Player 1");
	cars.push_back (player);

	if(!m_bCollectionChallenge)
	{
		

		characters = CCharacterSelection::GetInstance()->GetList();

		if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
		{
			player2 = CCharacterSelection::GetInstance()->GetPlayer2();
			//m_pOM->AddObject(player2);
			player2->Rotate(0.0f);
			player2->SetPosX(500);
			player2->SetPosY(400);
			player2->SetPlayerNum(2);
			player2->SetController(m_pController2);
			player2->SetName ("Player 2");
			cars.push_back (player2);
		}
		else
		{
			int player2index = rand()%4;
			while(player2index == CCharacterSelection::GetInstance()->GetPlayer1()->GetPlayerType())
				player2index = rand()%4;
			player2 = characters[player2index];
		}
		cars.push_back (miniboss);
		m_pOM->AddObject(miniboss);
	}
	cars = Level->SetCarSpawn (cars);

	

	RECT health_rect;
	health_rect.left = 5;
	health_rect.top = 0;
	health_rect.right = 256;
	health_rect.bottom = 256;

	miniboss->SetHealthImageRect(&health_rect,0);
	miniboss->SetHealthImageRect(&health_rect,1);
	miniboss->SetHealthImageRect(&health_rect,2);
	miniboss->SetHealthImageRect(&health_rect,3);
	miniboss->SetScale(0.4f);
	boss->SetHealthImageRect(&health_rect,0);
	boss->SetHealthImageRect(&health_rect,1);
	boss->SetHealthImageRect(&health_rect,2);
	boss->SetHealthImageRect(&health_rect,3);
	boss->SetScale(0.4f);

	bosses.push_back(boss);
	bosses.push_back(miniboss);

	m_bCountDown = false;
	m_fEnlarge = 0.0f;
	m_bPlaying = false;
	m_bBossHasSpawned = false;
	m_bBossHasDied = false;
	m_bMiniBossHasDied = false;
	m_bMiniBossHasSpawned = false;
	m_fRespawnMiniBossTimer = 0.0f;
	m_fRespawnBossTimer = 0.0f;
	m_fCountDown = 0.0f;
	m_nCollectableTotalComputer = 0;
	m_nCollectableTotalPlayer = 0;
	m_nCrateID = m_pTM->LoadTexture("resource/graphics/crate.png");
	m_nBarrelID = m_pTM->LoadTexture("resource/graphics/steeldrum.png");

	//if(!m_bCollectionChallenge)
	//{
	//	

	//	characters = CCharacterSelection::GetInstance()->GetList();

	//	if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
	//	{
	//		player2 = CCharacterSelection::GetInstance()->GetPlayer2();
	//		//m_pOM->AddObject(player2);
	//		player2->Rotate(0.0f);
	//		player2->SetPosX(500);
	//		player2->SetPosY(400);
	//		player2->SetPlayerNum(2);
	//		player2->SetController(m_pController2);
	//		cars.push_back (player2);
	//	}
	//	else
	//	{
	//		int player2index = rand()%4;
	//		while(player2index == CCharacterSelection::GetInstance()->GetPlayer1()->GetPlayerType())
	//			player2index = rand()%4;
	//		player2 = characters[player2index];
	//	}
	//	
	//}

	m_nBgMusicID = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Superbeast.mp3",SGD_FMOD_LOOPING);
	m_nCountDown = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdown.mp3");
	m_nCountDownEnd = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdowntone.mp3");
	
	
	time = 100;
	m_fElapsedSecond = 0.0f;
	score = 0;

	m_pPlayer1KB = CGamerProfile::GetInstance()->GetActiveProfile()->GetControllerBinds();
	m_pPlayer1KeyboardKB = CGamerProfile::GetInstance()->GetActiveProfile()->GetKeyboardBinds();

	Setvolume();


	//// game obstacles //////
	//m_nCrateID = m_pTM->LoadTexture("resource/graphics/crate.png");
	//m_nBarrelID = m_pTM->LoadTexture("resource/graphics/steeldrum.png");

	//// game obstacles /////
	/*m_nBgMusicID = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Superbeast.mp3",SGD_FMOD_LOOPING);
	m_nCountDown = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdown.mp3");
	m_nCountDownEnd = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdowntone.mp3");*/
	m_pFM->SetVolume(m_nCountDown,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nCountDownEnd,CGame::GetInstance()->getSoundBVolume());

	m_nMiniMapOverlayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_overlay.png");
	m_nMiniMapMiddlelayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_middlelay.png");
	m_nMiniMapUnderlayIndex=m_pTM->LoadTexture("resource/graphics/HUDS/minimap_underlay.png");

	if(m_bCollectionChallenge)
	{
		//Create the collectables
		for(int i = 0; i < 50; i++)
		{
			CCollectable* collectable = new CCollectable();
			collectables.push_back(collectable);
			m_pOM->AddObject(collectable);
		}
		CCollectionMode::GetInstance()->SetCollectables(collectables);
		collectionChallengeBoss = new CEnemy(CCharacterSelection::GetInstance()->GetPlayer1()->GetController()); 
		collectionChallengeBoss->SetPosX(500.0f);
		collectionChallengeBoss->SetPosY(500.0f);
		collectionChallengeBoss->SetSpeed(0.0f);
		collectionChallengeBoss->SetMaxSpeed(200.0f);
		collectionChallengeBoss->SetAcceleration(100.0f);
		collectionChallengeBoss->SetHealth(150.0f);
		collectionChallengeBoss->SetMaxHealth(150.0f);
		collectionChallengeBoss->SetCarId(m_pTM->LoadTexture("Resource/Graphics/BattleCars_MiniBossPlaceHolder.png"));
		collectionChallengeBoss->ChangeState(collectionChallengeBoss->GetCollectState ());
		collectionChallengeBoss->GetCollectState ()->SetCollectables (collectables);
		RECT health_rect;
		health_rect.left = 5;
		health_rect.top = 0;
		health_rect.right = 256;
		health_rect.bottom = 256;

		collectionChallengeBoss->SetHealthImageRect(&health_rect,0);
		collectionChallengeBoss->SetHealthImageRect(&health_rect,1);
		collectionChallengeBoss->SetHealthImageRect(&health_rect,2);
		collectionChallengeBoss->SetHealthImageRect(&health_rect,3);
		collectionChallengeBoss->SetScale(0.4f);

		m_lScores.push_back (collectionChallengeBoss);
		m_pOM->AddObject(collectionChallengeBoss);
	}
	else
		collectionChallengeBoss = NULL;

	for (size_t i = 0; i < cars.size (); i++)
	{
		if (cars[i]->GetType () != OBJECT_PLAYER)
		{
			((CEnemy*)cars[i])->SetSpeedRamps (ramps);
			((CEnemy*)cars[i])->SetPowerUps (power_ups);
			((CEnemy*)cars[i])->GetCollectState ()->SetCollectables (collectables);
		}
	}

	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		if( COptionState::GetInstance()->IsVertical())
		{
			player->GetCamera()->AttachTo(player,CGame::GetInstance()->GetScreenWidth()*0.25f,CGame::GetInstance()->GetScreenHeight()*0.5f);
			player->GetCamera()->SetWidth((int)(CGame::GetInstance()->GetScreenWidth()*0.5f));
			player->GetCamera()->SetHeight((int)(CGame::GetInstance()->GetScreenHeight()));
		}
		else
		{
			player->GetCamera ()->AttachTo(player,CGame::GetInstance()->GetScreenWidth()*0.5f, CGame::GetInstance()->GetScreenHeight()*0.25f);
			player->GetCamera()->SetWidth((int)(CGame::GetInstance()->GetScreenWidth()));
			player->GetCamera()->SetHeight((int)(CGame::GetInstance()->GetScreenHeight()*0.5f));
		}

		if (player2)
		{
			if( COptionState::GetInstance()->IsVertical())
			{
				player2->GetCamera ()->AttachTo(player2,CGame::GetInstance()->GetScreenWidth()*0.25f,CGame::GetInstance()->GetScreenHeight()*0.5f);
				player2->GetCamera()->SetWidth((int)(CGame::GetInstance()->GetScreenWidth()*0.5f));
				player2->GetCamera()->SetHeight((int)(CGame::GetInstance()->GetScreenHeight()));
			}
			else
			{
				player2->GetCamera ()->AttachTo(player2,CGame::GetInstance()->GetScreenWidth()*0.5f, CGame::GetInstance()->GetScreenHeight()*0.25f);
				player2->GetCamera()->SetWidth((int)(CGame::GetInstance()->GetScreenWidth()));
				player2->GetCamera()->SetHeight((int)(CGame::GetInstance()->GetScreenHeight()*0.5f));
			}
		}

		player->GetCamera()->Update();
		if (player2)
			player2->GetCamera()->Update();
	}
	else
	{
		player->GetCamera ()->AttachTo(player,CGame::GetInstance()->GetScreenWidth()*0.5f,CGame::GetInstance()->GetScreenHeight()*0.5f);
		player->GetCamera()->SetWidth(CGame::GetInstance()->GetScreenWidth());
		player->GetCamera()->SetHeight(CGame::GetInstance()->GetScreenHeight());
		player->GetCamera()->Update();
	}

	m_pFM->SetVolume(m_nBgMusicID,CGame::GetInstance()->getSoundBVolume());

	obstacles = Level->SetObstacleSpawn ();
	ramps = Level->SetSpeedRampSpawn ();
	power_ups = Level->SetPowerUpSpawn ();


	Level->ResetSpawns ();
	//m_lScores.push_back(player);
	//if(CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	//	m_lScores.push_back(player2);

	for (int i = 0; i < (int)cars.size (); i++)
	{
		m_lScores.push_back ((CCar*)cars[i]);
	}

	//player->SetKillCount (50);
}

void CGamePlayState::Exit(void)
{
	m_pFM->UnloadSound(m_nBgMusicID);
	m_pFM->UnloadSound(m_nCountDown);
	m_pFM->UnloadSound(m_nCountDownEnd);

	for(unsigned int i = 0; i < collectables.size(); i++)
	{
		collectables[i]->Release();
	}
	collectables.clear();

	for(unsigned int i = 0; i < characters.size(); i++)
	{						
		characters[i]->Release();
		characters[i] = NULL;
	}
	characters.clear ();

	for(unsigned int i = 0; i < cars.size(); i++)
	{
		if (cars[i]->GetType () == OBJECT_PLAYER)
		{
			if(((CPlayer*)cars[i])->GetPlayerNum() == 1)
			{
				if (this->m_pMode == CCollectionMode::GetInstance ())
				{
					CGame::GetInstance()->SetScore(((CPlayer*)cars[i])->GetCollected());
				}
				else
				{
					CGame::GetInstance()->SetScore(((CPlayer*)cars[i])->GetKillCount());
				}
			}
		}

		if (cars[i] != player && cars[i] != player2 && cars[i]->GetType () != OBJECT_BOSS)
		{
			cars[i]->Release();
			cars[i] = NULL;
		}
	}
	cars.clear();

	for(unsigned int i = 0; i < bosses.size(); i++)
	{
		bosses[i]->Release();
	}
	bosses.clear();

	for(unsigned int i = 0; i < obstacles.size(); i++)
	{
		obstacles[i]->Release();
	}
	obstacles.clear();

	for(unsigned int i = 0; i < ramps.size(); i++)
	{
		ramps[i]->Release();
	}
	ramps.clear();

	for(unsigned int i = 0; i < power_ups.size(); i++)
	{
		power_ups[i]->Release();
	}
	power_ups.clear();

	if(collectionChallengeBoss)
	{
		collectionChallengeBoss->Release();
		collectionChallengeBoss = NULL;
	}

	m_pPM->ShutDownParticleManager();
	m_pPM = NULL;
	m_lScores.clear();

	m_pTM->UnloadTexture(m_nMiniMapOverlayIndex);
	m_pTM->UnloadTexture(m_nMiniMapMiddlelayIndex);
	m_pTM->UnloadTexture(m_nMiniMapUnderlayIndex);

	m_pES->ClearEvents ();
	m_pES->ShutdownEventSystem ();

	Level->Shutdown ();

	m_pFM->StopSound(m_nBgMusicID);
	delete m_pPF;
	if(m_pOF)
	{
		m_pOF->ShutDownFactory();
		m_pOF = NULL;
	}
	if(m_pOM)
	{
		m_pOM->RemoveAllObjects();
		m_pOM = NULL;
		m_pOM->DeleteInstance();
	}
}

bool CGamePlayState::Input()
{
	if(CGame::GetInstance()->ControllerInput())
	{
		XINPUT_STATE xState = m_pController1->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;

		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			
			if(!m_bCountDown)
			{
				if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					CGame::GetInstance()->ResetInputDelay();
					m_pFM->PlaySound(m_nCountDown);
					m_bCountDown = true;

				}
			}
		}

		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetThumbDelay();
		}
	}
	else
	{

		if(!m_bCountDown)
		{
			if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonDown(0))
			{
				m_pFM->PlaySound(m_nCountDown);
				m_bCountDown = true;
			}
		}
		if(m_bPlaying)
		{
			if(m_pMode==CTutorialMode::GetInstance())
			{
				if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonDown(4))
				{
					((CTutorialMode*)m_pMode)->ToggleSkipTutorial();
				}
				if(((CTutorialMode*)m_pMode)->GetCurrMessage()==1)
				{
					if(!((CTutorialMode*)m_pMode)->GetTryStart())
					{
						if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonDown(0))
						{
							((CTutorialMode*)m_pMode)->SetTryStart(true);
							((CTutorialMode*)m_pMode)->SetCurrMessage(2);
						}
					}
				}
				if(((CTutorialMode*)m_pMode)->GetCurrMessage()==2)
				{
					if(!((CTutorialMode*)m_pMode)->GetTryMoved())
					{
						if(m_pDI->KeyPressed(DIK_UP)||m_pDI->JoystickGetLStickDirDown(DIR_UP))
						{
							((CTutorialMode*)m_pMode)->SetTryMoved(true);
							((CTutorialMode*)m_pMode)->SetCurrMessage(3);
						}
					}
				}
				if(((CTutorialMode*)m_pMode)->GetCurrMessage()==3)
				{
					if(!((CTutorialMode*)m_pMode)->GetTryTurn())
					{
						if(m_pDI->KeyPressed(DIK_LEFT) || m_pDI->KeyPressed(DIK_RIGHT))
						{
							((CTutorialMode*)m_pMode)->SetTryTurn(true);
							((CTutorialMode*)m_pMode)->SetCurrMessage(4);
						}
					}
				}
				
				if(((CTutorialMode*)m_pMode)->GetCurrMessage()==4)
				{
					if(!((CTutorialMode*)m_pMode)->GetTryShoot())
					{
						if(m_pDI->KeyPressed(DIK_SPACE)||m_pDI->JoystickButtonDown(0))
						{
							((CTutorialMode*)m_pMode)->SetTryShoot(true);
							((CTutorialMode*)m_pMode)->SetCurrMessage(5);
						}
					}
				}
				
				if(((CTutorialMode*)m_pMode)->GetCurrMessage()==5)
				{
					if(!((CTutorialMode*)m_pMode)->GetTryToggleWeapon())
					{
						if(m_pDI->KeyPressed(DIK_LCONTROL)||m_pDI->JoystickButtonDown(3))
						{
							((CTutorialMode*)m_pMode)->SetTryToggleWeapon(true);
							((CTutorialMode*)m_pMode)->SetCurrMessage(6);
						}
					}
				}
				if(((CTutorialMode*)m_pMode)->GetCurrMessage()==6)
				{
					if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonDown(0))
					{
						((CTutorialMode*)m_pMode)->ToggleSkipTutorial();
					}
				}
			}
			if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonDown(4))
			{
				CGame::GetInstance()->AddState(CPauseMenuState::GetInstance());
			}
		}
	}
	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{
	if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nBgMusicID))
		CSGD_FModManager::GetInstance()->PlaySound(m_nBgMusicID);

	scoretimer += fElapsedTime;
	if(scoretimer >= 1.0f)
	{
		scoretimer = 0.0f;
		SortScores();
	}

	if( COptionState::GetInstance()->IsVertical() )
	{
		player->GetCamera()->SetRenderPosX(0);
		player->GetCamera()->SetRenderPosY(0);
		if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
		{
			player2->GetCamera()->SetRenderPosX(CGame::GetInstance()->GetScreenWidth() * 0.5f);
			player2->GetCamera()->SetRenderPosY(0);
		}
	}
	else
	{
		player->GetCamera()->SetRenderPosX(0);
		player->GetCamera()->SetRenderPosY(0);
		if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
		{
			player2->GetCamera()->SetRenderPosX(0);
			player2->GetCamera()->SetRenderPosY(CGame::GetInstance()->GetScreenHeight()*0.5f);
		}
	}

	if(m_bPlaying)
	{
		m_fElapsedSecond += fElapsedTime;
		m_ftimer += fElapsedTime;

		if( m_fElapsedSecond >= 1.0f)
		{
			m_fElapsedSecond = 0.0f;
			time -= 1;
		}
		
		if(!m_bCollectionChallenge)
		{
			if(time <= 30)
			{
				m_bMiniBossHasDied = false;
				bosses[1]->SetHealth(0);
			}
			if(m_bMiniBossHasDied)
			{
				m_fRespawnMiniBossTimer += fElapsedTime;
				if(m_fRespawnMiniBossTimer > 3.0f)
				{
					m_bMiniBossHasDied = false;
					bosses[1] = new CBoss(CCharacterSelection::GetInstance()->GetPlayer1()->GetController());
					bosses[1]->SetMiniBoss(true);
					bosses[1]->SetHealth(65.0f);
					bosses[1]->SetMaxHealth(65.0f);
					bosses[1]->SetShieldBar(30.0f);
					bosses[1]->SetMaxShield(30.0f);
					bosses[1]->SetAcceleration(3.5f);
					bosses[1]->SetPosX(float(rand()%1400+200));
					bosses[1]->SetPosY(float(rand()%1400+200));
					bosses[1]->SetSpecialLevel(1);
					bosses[1]->SetCarId(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniBossPlaceHolder.png"));
					m_fRespawnMiniBossTimer = 0.0f;
					m_pOM->AddObject(bosses[1]);
				}
			}
			if(time <= 30 && !m_bBossHasSpawned)
			{
				m_bBossHasSpawned = true;
				m_pOM->AddObject(bosses[0]);
			}
			if(m_bBossHasDied)
			{
				m_fRespawnBossTimer += fElapsedTime;
				if(m_fRespawnBossTimer >= 3.0f)
				{
					m_bBossHasDied = false;
					bosses[0] = new CBoss(CCharacterSelection::GetInstance()->GetPlayer1()->GetController());
					m_bBossHasSpawned = false;
					m_fRespawnBossTimer = 0.0f;
				}
			}
		}
		m_pFM->Update();
		m_pOM->UpdateObjects(fElapsedTime);
		m_pPM->UpdateEmittors(fElapsedTime);
		m_pES->ProcessEvents ();
		m_pMS->ProcessMessages ();
		
	
	}
	else
	{
		if(m_bCountDown)
		{
			m_fCountDown += fElapsedTime;
			m_fEnlarge += fElapsedTime;
			if(m_fEnlarge >= 1.0f)
			{
				m_pFM->PlaySound(m_nCountDown);
				m_fEnlarge = 0.0f;
			}
			if(m_fCountDown >= 5.0f)
			{
				m_pFM->PlaySound(m_nCountDownEnd);
				m_bPlaying = true;
			}
		}
	}
	m_pMode->CheckWinLoss();

		
}

void CGamePlayState::Render(void)
{

	Level->Render (player->GetCamera ());
	m_pOM->RenderObjects(player->GetCamera());
	m_pPM->RenderEmittors(player->GetCamera());

	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		Level->Render(player2->GetCamera());
		m_pOM->RenderObjects(player2->GetCamera());
		m_pPM->RenderEmittors(player2->GetCamera());
	}

	if(m_pMode==CTutorialMode::GetInstance())
	{
		m_pPF->Print(CTutorialMode::GetInstance()->GetMessage(0).c_str(),CGame::GetInstance()->GetScreenWidth()/4,0,1.0f,D3DCOLOR_ARGB(255,255,255,255));
		m_pPF->Print(CTutorialMode::GetInstance()->GetMessage(CTutorialMode::GetInstance()->GetCurrMessage()).c_str(),CGame::GetInstance()->GetScreenWidth()/4,CGame::GetInstance()->GetScreenHeight()/2,1.0f,D3DCOLOR_ARGB(255,255,255,255));
	}

	
	if(m_bCollectionChallenge)
	{
		char buffer[100];
		sprintf_s(buffer, "%d", m_nCollectableTotalPlayer);
	//	m_pPF->Print(buffer, 10, 10, 1.0f, D3DCOLOR_XRGB(255, 255, 255)); 

	}

	if(!m_bPlaying)
	{
		char buffer[32];
		float tempcount = 5.0f-m_fCountDown;
		sprintf_s(buffer,"%.2f",tempcount);
		m_pPF->Print(buffer, CGame::GetInstance()->GetScreenWidth()/2-50,280,1.0,D3DCOLOR_XRGB(255, 255, 255));
	}

	if( time >= 0 )
	{
		char timebuff[32];
		int minutes = time / 60;
		int seconds = time - (60 * minutes);
		if( seconds < 10 )
			sprintf_s(timebuff, "%i:%i%i", minutes,0, seconds);
		else
			sprintf_s(timebuff, "%i:%i", minutes, seconds);
		if(time > 10)
			m_pPF->Print(timebuff, CGame::GetInstance()->GetScreenWidth()/2-50, 30, 1.0f, D3DCOLOR_XRGB(255,255,255)); 
		else
		{
			if(!m_pFM->IsSoundPlaying(m_nCountDown))
				m_pFM->PlaySound(m_nCountDown);
			if(float(time)/2.0f > 0)
				m_pPF->Print(timebuff, CGame::GetInstance()->GetScreenWidth()/2-100, 30,2.0f, D3DCOLOR_XRGB(255,0,0));
			else
				m_pPF->Print(timebuff, CGame::GetInstance()->GetScreenWidth()/2-100,30,2.0f, D3DCOLOR_XRGB(255,255,255));
		}
	}


	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		if( COptionState::GetInstance()->IsVertical())
		{
			m_pD3D->DrawLine(int(CGame::GetInstance()->GetScreenWidth() * 0.5f) - CLevel::GetInstance()->GetMap()->GetPixelWidth(), 0, int(CGame::GetInstance()->GetScreenWidth() * 0.5f)- CLevel::GetInstance()->GetMap()->GetPixelWidth(), CGame::GetInstance()->GetScreenHeight(), 255,0,0);
	
		}
		else
		{
			m_pD3D->DrawLine(0, int(CGame::GetInstance()->GetScreenHeight()*0.5f) - CLevel::GetInstance()->GetMap()->GetPixelHeight(), CGame::GetInstance()->GetScreenWidth(), int(CGame::GetInstance()->GetScreenHeight()*0.5f)- CLevel::GetInstance()->GetMap()->GetPixelHeight(), 255,0,0);
			
		}

	}

	

}

bool CGamePlayState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nCountDown);
	m_bCountDown = true;
	return true;
}



void CGamePlayState::MessageProc(CBaseMessage* pMsg)
{
	ParticleManager* PM = CGamePlayState::GetInstance()->m_pPM;

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
	case MSG_CREATE_PLAYER_BULLET:
		{

			CCreatePlayerBulletMessage* pCBM = (CCreatePlayerBulletMessage*)pMsg;

			CGamePlayState* pGame = CGamePlayState::GetInstance();

			//CBullet* pBullet = (CBullet*)pGame->m_pOF->CreateObject("CBullet");
			CBullet* pBullet = new CBullet();
			pBullet->SetImageID (pCBM->GetPlayer()->GetBulletImageID());

			tVector2D temp;
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCBM->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (450+ pCBM->GetPlayer()->GetSpeed());

			pBullet->SetScale(0.5f);
			pBullet->SetOwner(pCBM->GetPlayer());
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(5.0f);
			pBullet->SetHeight((int)(64*pBullet->GetScale()));
			pBullet->SetWidth((int)(64*pBullet->GetScale()));
			pBullet->SetPosX(pCBM->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCBM->GetPlayer()->GetPosY());
			pBullet->SetDamage(15);
			pBullet->SetBulletType(PROJECTILE_BULLET);
			pBullet->SetRotation(pCBM->GetPlayer()->GetRotation());
			pGame->m_pOM->AddObject(pBullet);
			pBullet->Release();
			break;
		}
	case MSG_CREATE_PLAYER_MISSILE:
		{
			CCreatePlayerMissileMessage* pCBM = (CCreatePlayerMissileMessage*)pMsg;

			CGamePlayState* pGame = CGamePlayState::GetInstance();

			//CBullet* pBullet = (CBullet*)pGame->m_pOF->CreateObject("CBullet");
			CBullet* pBullet = new CBullet();
			pBullet->SetImageID (pCBM->GetPlayer()->GetMissileImageID());

			tVector2D temp;
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCBM->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (450+ pCBM->GetPlayer()->GetSpeed());

			pBullet->SetScale(0.5f);
			pBullet->SetOwner(pCBM->GetPlayer());
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(5.0f);
			pBullet->SetHeight((int)(64*pBullet->GetScale()));
			pBullet->SetWidth((int)(64*pBullet->GetScale()));
			pBullet->SetPosX(pCBM->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCBM->GetPlayer()->GetPosY());
			pBullet->SetDamage(35);
			pBullet->SetBulletType(PROJECTILE_MISSILE);
			pBullet->SetRotation(pCBM->GetPlayer()->GetRotation());

			ParticleManager* pPM = ParticleManager::GetInstance(); 
			Emittor* tempemittor = pPM->CreateEffect(pPM->GetEmittor(MISSLE_EMITTOR), pBullet->GetPosX() - pBullet->GetWidth() * 0.5f, pBullet->GetPosY() - pBullet->GetHeight()*0.5f);

			if( tempemittor)
			{
				pBullet->SetTracerEmittor(tempemittor->GetID());
				tempemittor->SetTimeToDie(pBullet->GetMaxLife());
				pPM->AttachToBasePosition(pBullet, tempemittor);

			}

			pGame->m_pOM->AddObject(pBullet);
			pBullet->Release();
		}
		break;
	case MSG_CREATE_MINI_SPECIAL:
		{
			CCreateMiniSpecialMessage* pCMS = (CCreateMiniSpecialMessage*)pMsg;
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			//CBullet* pBullet = (CBullet*)pGame->m_pOF->CreateObject("CBullet");
			CBullet* pBullet = new CBullet();
			CBullet* pBullet1 = new CBullet();
			CBullet* pBullet2 = new CBullet();
			CBullet* pBullet3 = new CBullet();
			CBullet* pBullet4 = new CBullet();
			//Missile 1
			pBullet->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
			pBullet->SetScale(0.5f);
			pBullet->SetOwner(pCMS->GetPlayer());
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(1.15f);
			pBullet->SetHeight((int)(64*pBullet->GetScale()));
			pBullet->SetWidth((int)(32*pBullet->GetScale()));
			pBullet->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet->SetDamage(float(12*(pCMS->GetPlayer()->GetSpecialLevel())));
			pBullet->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet->SetBlastRadius(50.0f);
			pBullet->SetRotation(pCMS->GetPlayer()->GetRotation());
			//Missile 2
			pBullet1->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
			pBullet1->SetScale(0.5f);
			pBullet1->SetOwner(pCMS->GetPlayer());
			pBullet1->SetCurLife(0.0f);
			pBullet1->SetMaxLife(1.15f);
			pBullet1->SetHeight((int)(64*pBullet->GetScale()));
			pBullet1->SetWidth((int)(32*pBullet->GetScale()));
			pBullet1->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet1->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet1->SetDamage(float(12*(pCMS->GetPlayer()->GetSpecialLevel())));
			pBullet1->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet1->SetBlastRadius(50.0f);
			pBullet1->SetRotation(pCMS->GetPlayer()->GetRotation());
			//Missile 3
			pBullet2->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
			pBullet2->SetScale(0.5f);
			pBullet2->SetOwner(pCMS->GetPlayer());
			pBullet2->SetCurLife(0.0f);
			pBullet2->SetMaxLife(1.15f);
			pBullet2->SetHeight((int)(64*pBullet->GetScale()));
			pBullet2->SetWidth((int)(32*pBullet->GetScale()));
			pBullet2->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet2->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet2->SetDamage(float(12*(pCMS->GetPlayer()->GetSpecialLevel())));
			pBullet2->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet2->SetBlastRadius(50.0f);
			pBullet2->SetRotation(pCMS->GetPlayer()->GetRotation());
			//Missile 4
			pBullet3->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
			pBullet3->SetScale(0.5f);
			pBullet3->SetOwner(pCMS->GetPlayer());
			pBullet3->SetCurLife(0.0f);
			pBullet3->SetMaxLife(1.15f);
			pBullet3->SetHeight((int)(64*pBullet->GetScale()));
			pBullet3->SetWidth((int)(32*pBullet->GetScale()));
			pBullet3->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet3->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet3->SetDamage(float(12*(pCMS->GetPlayer()->GetSpecialLevel())));
			pBullet3->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet3->SetBlastRadius(50.0f);
			pBullet3->SetRotation(pCMS->GetPlayer()->GetRotation());
			//Missile 5
			pBullet4->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
			pBullet4->SetScale(0.5f);
			pBullet4->SetOwner(pCMS->GetPlayer());
			pBullet4->SetCurLife(0.0f);
			pBullet4->SetMaxLife(1.15f);
			pBullet4->SetHeight((int)(64*pBullet->GetScale()));
			pBullet4->SetWidth((int)(32*pBullet->GetScale()));
			pBullet4->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet4->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet4->SetDamage(float(12*(pCMS->GetPlayer()->GetSpecialLevel())));
			pBullet4->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet4->SetBlastRadius(50.0f);
			pBullet4->SetRotation(pCMS->GetPlayer()->GetRotation());
			//Set up launch vectors
			tVector2D temp;
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()-.5f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCMS->GetPlayer()->GetSpeed());
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()-.25f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCMS->GetPlayer()->GetSpeed());
			pBullet1->SetVelX(temp.fX);
			pBullet1->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (450+ pCMS->GetPlayer()->GetSpeed());
			pBullet2->SetVelX(temp.fX);
			pBullet2->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()+.25f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCMS->GetPlayer()->GetSpeed());
			pBullet3->SetVelX(temp.fX);
			pBullet3->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()+.5f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCMS->GetPlayer()->GetSpeed());
			pBullet4->SetVelX(temp.fX);
			pBullet4->SetVelY(temp.fY);

			pGame->m_pOM->AddObject(pBullet);
			pGame->m_pOM->AddObject(pBullet1);
			pGame->m_pOM->AddObject(pBullet2);
			pGame->m_pOM->AddObject(pBullet3);
			pGame->m_pOM->AddObject(pBullet4);
			pBullet->Release();
			pBullet1->Release();
			pBullet2->Release();
			pBullet3->Release();
			pBullet4->Release();
		}
		break;
		case MSG_CREATE_TRUCK_SPECIAL:
		{
			CCreateTruckSpecialMessage* pTSM = (CCreateTruckSpecialMessage*)pMsg;
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			//CLandMine* pLandMine = (CLandMine*)pGame->m_pOF->CreateObject("CLandMine");
			CLandMine* pLandMine = new CLandMine();
			CLandMine* pLandMine1 = new CLandMine();
			CLandMine* pLandMine2 = new CLandMine();
			pLandMine->SetDuration(20.0f);
			pLandMine->SetScale(1.0f);
			pLandMine->SetOwner(pTSM->GetPlayer());
			pLandMine->SetImageID (pGame->m_pTM->LoadTexture("resource/graphics/BattleCars_LandMinePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
			pLandMine->SetCurLife(0.0f);
			pLandMine->SetMaxLife(30.0f);
			pLandMine->SetHeight((int)(64*pLandMine->GetScale()));
			pLandMine->SetWidth((int)(64*pLandMine->GetScale()));
			pLandMine->SetDamage(float(15*(pTSM->GetPlayer()->GetSpecialLevel())));
			pLandMine->SetBlastRadius(0.0f);
			pLandMine->SetLandMineType(LM_LM);
			pLandMine->SetVelX(0.0f);
			pLandMine->SetVelY(0.0f);
			pLandMine1->SetDuration(20.0f);
			pLandMine1->SetScale(1.0f);
			pLandMine1->SetOwner(pTSM->GetPlayer());
			pLandMine1->SetImageID (pGame->m_pTM->LoadTexture("resource/graphics/BattleCars_LandMinePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
			pLandMine1->SetCurLife(0.0f);
			pLandMine1->SetMaxLife(30.0f);
			pLandMine1->SetHeight((int)(64*pLandMine->GetScale()));
			pLandMine1->SetWidth((int)(64*pLandMine->GetScale()));
			pLandMine1->SetDamage(float(15*(pTSM->GetPlayer()->GetSpecialLevel())));
			pLandMine1->SetBlastRadius(0.0f);
			pLandMine1->SetLandMineType(LM_LM);
			pLandMine1->SetVelX(0.0f);
			pLandMine1->SetVelY(0.0f);
			pLandMine2->SetDuration(20.0f);
			pLandMine2->SetScale(1.0f);
			pLandMine2->SetOwner(pTSM->GetPlayer());
			pLandMine2->SetImageID (pGame->m_pTM->LoadTexture("resource/graphics/BattleCars_LandMinePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
			pLandMine2->SetCurLife(0.0f);
			pLandMine2->SetMaxLife(30.0f);
			pLandMine2->SetHeight((int)(64*pLandMine->GetScale()));
			pLandMine2->SetWidth((int)(64*pLandMine->GetScale()));
			pLandMine2->SetDamage(float(15*(pTSM->GetPlayer()->GetSpecialLevel())));
			pLandMine2->SetBlastRadius(0.0f);
			pLandMine2->SetLandMineType(LM_LM);
			pLandMine2->SetVelX(0.0f);
			pLandMine2->SetVelY(0.0f);
			tVector2D direction = pTSM->GetPlayer()->GetDirection();
			if(direction.fX <=-.75f && (direction.fY <=.25 && direction.fY >= -.25f)) // left
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()+(pTSM->GetPlayer()->GetWidth()*.5f));
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()-(pTSM->GetPlayer()->GetHeight()*.5f));
				pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX());
				pLandMine2->SetPosY(pLandMine->GetPosY() +20+pLandMine->GetHeight());
			}
			else if(direction.fX >=.75f && (direction.fY <= .25f && direction.fY >= -.25f)) // right
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()-(pTSM->GetPlayer()->GetWidth()*.5f)-pLandMine->GetWidth());
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()-(pTSM->GetPlayer()->GetHeight()*.5f));
				pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX());
				pLandMine2->SetPosY(pLandMine->GetPosY() +20.0f+pLandMine->GetHeight());
			}
			else if(direction.fY >= .75f && (direction.fX <= .25f && direction.fX >= -.25f)) // up
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()-(pLandMine->GetWidth()*.5f));
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()-(pTSM->GetPlayer()->GetHeight()*.5f)-pLandMine->GetHeight());
				pLandMine1->SetPosX(pLandMine->GetPosX() - 20.0f-pLandMine->GetWidth());
				pLandMine1->SetPosY(pLandMine->GetPosY());
				pLandMine2->SetPosX(pLandMine->GetPosX() + 20+pLandMine->GetWidth());
				pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fY <= -.75f && (direction.fX <= .25f && direction.fX >= -.25f)) // down
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()-(pLandMine->GetWidth()*.5f));
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()+(pTSM->GetPlayer()->GetHeight()*.5f));
				pLandMine1->SetPosX(pLandMine->GetPosX() - 20.0f-pLandMine->GetWidth());
				pLandMine1->SetPosY(pLandMine->GetPosY());
				pLandMine2->SetPosX(pLandMine->GetPosX() + 20+pLandMine->GetWidth());
				pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fX <= 0 && direction.fY <= 0) // up left
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()+(pTSM->GetPlayer()->GetWidth()*.5f));
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()+(pTSM->GetPlayer()->GetHeight()*.5f));
				pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX() - 20.0f - pLandMine->GetWidth());
				pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fX <= 0 && direction.fY > 0) // down left
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()+(pTSM->GetPlayer()->GetWidth()*.5f));
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()-(pTSM->GetPlayer()->GetHeight()*.5f) - pLandMine->GetHeight());
				pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() + 20.0f + pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX() - 20.0f - pLandMine->GetWidth());
				pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fX > 0 && direction.fY <=0) // up right
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()-(pTSM->GetPlayer()->GetWidth()*.5f) - pLandMine->GetWidth());
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()+(pTSM->GetPlayer()->GetHeight()*.5f));
				pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX() + 20.0f + pLandMine->GetWidth());
				pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fX > 0 && direction.fY  > 0) // down right
			{
				pLandMine->SetPosX(pTSM->GetPlayer()->GetPosX()-(pTSM->GetPlayer()->GetWidth()*.5f) - pLandMine->GetWidth());
				pLandMine->SetPosY(pTSM->GetPlayer()->GetPosY()-(pTSM->GetPlayer()->GetHeight()*.5f) - pLandMine->GetHeight());
				pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() + 20.0f + pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX() + 20.0f + pLandMine->GetWidth());
				pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			pGame->m_pOM->AddObject(pLandMine);
			pGame->m_pOM->AddObject(pLandMine1);
			pGame->m_pOM->AddObject(pLandMine2);
			pLandMine->Release();
			pLandMine1->Release();
			pLandMine2->Release();
		}
		break;
		case MSG_CREATE_VETTE_SPECIAL:
		{
			
			CCreateVetteSpecialMessage* pCVS = (CCreateVetteSpecialMessage*)pMsg;
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			
			tVector2D temp;
			CBullet* pBullet = new CBullet;
			CBullet* pBullet1 = new CBullet;
			CBullet* pBullet2 = new CBullet;
			CBullet* pBullet3 = new CBullet;
			CBullet* pBullet4 = new CBullet;
			pBullet->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
			pBullet->SetScale(1.0f);
			pBullet->SetOwner(pCVS->GetPlayer());
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(1.0f);
			pBullet->SetHeight((int)(8*pBullet->GetScale()));
			pBullet->SetWidth((int)(8*pBullet->GetScale()));
			pBullet->SetPosX(pCVS->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCVS->GetPlayer()->GetPosY());
			pBullet->SetDamage(.45f*(pCVS->GetPlayer()->GetSpecialLevel()));
			pBullet->SetBulletType(PROJECTILE_BULLET);
			pBullet->SetToRender(false);
			pBullet1->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
			pBullet1->SetScale(1.0f);
			pBullet1->SetOwner(pCVS->GetPlayer());
			pBullet1->SetCurLife(0.0f);
			pBullet1->SetMaxLife(1.0f);
			pBullet1->SetHeight((int)(8*pBullet->GetScale()));
			pBullet1->SetWidth((int)(8*pBullet->GetScale()));
			pBullet1->SetPosX(pCVS->GetPlayer()->GetPosX());
			pBullet1->SetPosY(pCVS->GetPlayer()->GetPosY());
			pBullet1->SetDamage(.45f*(pCVS->GetPlayer()->GetSpecialLevel()));
			pBullet1->SetBulletType(PROJECTILE_BULLET);
			pBullet1->SetToRender(false);
			pBullet2->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
			pBullet2->SetScale(1.0f);
			pBullet2->SetOwner(pCVS->GetPlayer());
			pBullet2->SetCurLife(0.0f);
			pBullet2->SetMaxLife(1.0f);
			pBullet2->SetHeight((int)(8*pBullet->GetScale()));
			pBullet2->SetWidth((int)(8*pBullet->GetScale()));
			pBullet2->SetPosX(pCVS->GetPlayer()->GetPosX());
			pBullet2->SetPosY(pCVS->GetPlayer()->GetPosY());
			pBullet2->SetDamage(.45f*(pCVS->GetPlayer()->GetSpecialLevel()));
			pBullet2->SetBulletType(PROJECTILE_BULLET);
			pBullet2->SetToRender(false);
			pBullet3->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
			pBullet3->SetScale(1.0f);
			pBullet3->SetOwner(pCVS->GetPlayer());
			pBullet3->SetCurLife(0.0f);
			pBullet3->SetMaxLife(1.0f);
			pBullet3->SetHeight((int)(8*pBullet->GetScale()));
			pBullet3->SetWidth((int)(8*pBullet->GetScale()));
			pBullet3->SetPosX(pCVS->GetPlayer()->GetPosX());
			pBullet3->SetPosY(pCVS->GetPlayer()->GetPosY());
			pBullet3->SetDamage(.45f*(pCVS->GetPlayer()->GetSpecialLevel()));
			pBullet3->SetBulletType(PROJECTILE_BULLET);
			pBullet3->SetToRender(false);
			pBullet4->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
			pBullet4->SetScale(1.0f);
			pBullet4->SetOwner(pCVS->GetPlayer());
			pBullet4->SetCurLife(0.0f);
			pBullet4->SetMaxLife(1.0f);
			pBullet4->SetHeight((int)(8*pBullet->GetScale()));
			pBullet4->SetWidth((int)(8*pBullet->GetScale()));
			pBullet4->SetPosX(pCVS->GetPlayer()->GetPosX());
			pBullet4->SetPosY(pCVS->GetPlayer()->GetPosY());
			pBullet4->SetDamage(.45f*(pCVS->GetPlayer()->GetSpecialLevel()));
			pBullet4->SetBulletType(PROJECTILE_BULLET);
			pBullet4->SetToRender(false);


			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCVS->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (450+ pCVS->GetPlayer()->GetSpeed());
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);

			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCVS->GetPlayer()->GetRotation()-.03f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCVS->GetPlayer()->GetSpeed());
			pBullet1->SetVelX(temp.fX);
			pBullet1->SetVelY(temp.fY);

			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCVS->GetPlayer()->GetRotation()+.03f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCVS->GetPlayer()->GetSpeed());
			pBullet2->SetVelX(temp.fX);
			pBullet2->SetVelY(temp.fY);

			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCVS->GetPlayer()->GetRotation()-.06f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCVS->GetPlayer()->GetSpeed());
			pBullet3->SetVelX(temp.fX);
			pBullet3->SetVelY(temp.fY);

			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCVS->GetPlayer()->GetRotation()+.06f);
			Vector2DNormalize(temp);
			temp = temp * (450+ pCVS->GetPlayer()->GetSpeed());
			pBullet4->SetVelX(temp.fX);
			pBullet4->SetVelY(temp.fY);

			pGame->m_pOM->AddObject(pBullet);
			pGame->m_pOM->AddObject(pBullet1);
			pGame->m_pOM->AddObject(pBullet2);
			pGame->m_pOM->AddObject(pBullet3);
			pGame->m_pOM->AddObject(pBullet4);

			pBullet->Release();
			pBullet1->Release();
			pBullet2->Release();
			pBullet3->Release();
			pBullet4->Release();
			

		}
		break;
	case MSG_CREATE_HUMMER_SPECIAL:
		{
			CCreateHummerSpecialMessage* pCHS = (CCreateHummerSpecialMessage*)pMsg;
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			CBullet* pBullet = new CBullet;
			//IcyBullet 1
			pBullet->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_IceLaserPlaceholder.png"));
			pBullet->SetScale(1.0f);
			pBullet->SetOwner(pCHS->GetPlayer());
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(3.0f);
			pBullet->SetHeight((int)(32*pBullet->GetScale()));
			pBullet->SetWidth((int)(16*pBullet->GetScale()));
			pBullet->SetDamage(float(8*(pCHS->GetPlayer()->GetSpecialLevel())));
			pBullet->SetBulletType(PROJECTILE_BULLET);
			pBullet->SetSlowRate(20.0f);
			pBullet->SetRotation(pCHS->GetPlayer()->GetRotation());
			pBullet->SetPosX(pCHS->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCHS->GetPlayer()->GetPosY());
			tVector2D temp;
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCHS->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (450+ pCHS->GetPlayer()->GetSpeed());
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);
			pGame->m_pOM->AddObject(pBullet);
			pBullet->Release();
		}
		break;
	case MSG_DESTROY_BULLET:
		{
			CDestroyBulletMessage* pM = (CDestroyBulletMessage*)pMsg;
			CBullet* tempbullet = pM->GetBullet();
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			if(tempbullet->GetBulletType() == PROJECTILE_BULLET)
			{
				if( tempbullet->GetTracerEmittor() > -1 )
 					CGamePlayState::GetInstance()->m_pPM->GetActiveEmittor(tempbullet->GetTracerEmittor())->SetBase(NULL);
				pGame->m_pOM->RemoveObject(tempbullet);
			}
			else if(tempbullet->GetBulletType() == PROJECTILE_MISSILE || tempbullet->GetBulletType() == PROJECTILE_MINI_MISSILE)
			{
				if(tempbullet->GetTracerEmittor() > -1)
					CGamePlayState::GetInstance()->m_pPM->GetActiveEmittor(tempbullet->GetTracerEmittor())->SetBase(NULL);

#pragma region EXPLOSION_EFFECT
				Emittor* smoke_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_SMOKE_EMITTOR), tempbullet->GetPosX(), tempbullet->GetPosY());
				if( smoke_emittor )
				{
 					smoke_emittor->SetTimeToDie(1.5f);
				}

				Emittor* fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST1_EMITTOR), tempbullet->GetPosX() , tempbullet->GetPosY());
				if( fireburst_emittor )
				{
					fireburst_emittor->SetTimeToDie(1.5f);
				}

				fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST2_EMITTOR), tempbullet->GetPosX() , tempbullet->GetPosY());
				if( fireburst_emittor )
				{
					fireburst_emittor->SetTimeToDie(1.5f);				
				}
				fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST3_EMITTOR), tempbullet->GetPosX(), tempbullet->GetPosY());
				if( fireburst_emittor )
				{
					fireburst_emittor->SetTimeToDie(1.5f);
				}

				Emittor* fire_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FLAME_EMITTOR), tempbullet->GetPosX(), tempbullet->GetPosY());
				if( fire_emittor )
				{
					fire_emittor->SetTimeToDie(1.5f);
				}
#pragma endregion
				pGame->m_pOM->RemoveObject(tempbullet);
				tempbullet = NULL;
			}
			else if(tempbullet->GetBulletType() == PROJECTILE_LANDMINE)
			{
				
#pragma region EXPLOSION_EFFECT
				Emittor* smoke_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_SMOKE_EMITTOR), tempbullet->GetPosX(), tempbullet->GetPosY());
				if( smoke_emittor )
				{
 					smoke_emittor->SetTimeToDie(1.5f);
				}

				Emittor* fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST1_EMITTOR), tempbullet->GetPosX() , tempbullet->GetPosY());
				if( fireburst_emittor )
				{
					fireburst_emittor->SetTimeToDie(1.5f);
				}

				fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST2_EMITTOR), tempbullet->GetPosX() , tempbullet->GetPosY());
				if( fireburst_emittor )
				{
					fireburst_emittor->SetTimeToDie(1.5f);				
				}
				fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST3_EMITTOR), tempbullet->GetPosX(), tempbullet->GetPosY());
				if( fireburst_emittor )
				{
					fireburst_emittor->SetTimeToDie(1.5f);
				}

				Emittor* fire_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FLAME_EMITTOR), tempbullet->GetPosX(), tempbullet->GetPosY());
				if( fire_emittor )
				{
					fire_emittor->SetTimeToDie(1.5f);
				}
#pragma endregion

				pGame->m_pOM->RemoveObject(tempbullet);
			}
			break;
		}
	case MSG_CREATE_ENEMY_BULLET_MESSAGE:
		{
			CCreateEnemyBulletMessage* pEBM = (CCreateEnemyBulletMessage*)pMsg;
			CEnemy* tempEnemy = pEBM->GetEnemy();
			if(tempEnemy)
			{
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			
			CBullet* pBullet = new CBullet();
			pBullet->SetImageID (pEBM->GetEnemy()->GetBulletImageID());

			tVector2D temp;
			//temp = pCBM->GetPlayer()->GetDirection();
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pEBM->GetEnemy()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (450+ pEBM->GetEnemy()->GetSpeed());

			pBullet->SetScale(0.5f);


			pBullet->SetOwner(pEBM->GetEnemy());


			/*if ((pCBM->GetPlayer()->GetDirection () > 0))
			pBullet->SetPosX(pCBM->GetPlayer()->GetPosX() + ((pCBM->GetPlayer()->GetWidth()* pCBM->GetPlayer ()->GetScale ())));
			else 
			pBullet->SetPosX(pCBM->GetPlayer()->GetPosX() - ((pCBM->GetPlayer()->GetWidth() * pCBM->GetPlayer ()->GetScale ())));

			if (pCBM->GetPlayer ()->GetPlayerType () == 0)
			pBullet->SetPosY(pCBM->GetPlayer()->GetPosY());
			else
			pBullet->SetPosY(pCBM->GetPlayer()->GetPosY() + (pCBM->GetPlayer()->GetHeight() * pCBM->GetPlayer ()->GetScale () *0.5f) - 4);*/
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(5.0f);
			pBullet->SetHeight((int)(64*pBullet->GetScale()));
			pBullet->SetWidth((int)(64*pBullet->GetScale()));
			pBullet->SetPosX(pEBM->GetEnemy()->GetPosX());
			pBullet->SetPosY(pEBM->GetEnemy()->GetPosY());
			pBullet->SetDamage(10);
			pBullet->SetBulletType(PROJECTILE_BULLET);
			pBullet->SetRotation(pEBM->GetEnemy()->GetRotation());
			pGame->m_pOM->AddObject(pBullet);
			pBullet->Release();
			}
			break;
		}
	case MSG_CREATE_BOSS_MINI_SPECIAL:
		{
			CCreateBossMiniSpecial* pBMS = (CCreateBossMiniSpecial*)pMsg;
			CBoss* tempBoss = pBMS->GetBoss();
			if(tempBoss)
			{
				CGamePlayState* pGame = CGamePlayState::GetInstance();
				CBullet* pBullet = new CBullet();
				CBullet* pBullet1 = new CBullet();
				CBullet* pBullet2 = new CBullet();
				CBullet* pBullet3 = new CBullet();
				CBullet* pBullet4 = new CBullet();
				//Missile 1
				pBullet->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
				pBullet->SetScale(0.5f);
				pBullet->SetOwner(pBMS->GetBoss());
				pBullet->SetCurLife(0.0f);
				pBullet->SetMaxLife(1.15f);
				pBullet->SetHeight((int)(64*pBullet->GetScale()));
				pBullet->SetWidth((int)(32*pBullet->GetScale()));
				pBullet->SetPosX(pBMS->GetBoss()->GetPosX());
				pBullet->SetPosY(pBMS->GetBoss()->GetPosY());
				pBullet->SetDamage(float(8*(pBMS->GetBoss()->GetSpecialLevel())));
				pBullet->SetBulletType(PROJECTILE_MINI_MISSILE);
				pBullet->SetBlastRadius(50.0f);
				pBullet->SetRotation(pBMS->GetBoss()->GetRotation());
				//Missile 2
				pBullet1->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
				pBullet1->SetScale(0.5f);
				pBullet1->SetOwner(pBMS->GetBoss());
				pBullet1->SetCurLife(0.0f);
				pBullet1->SetMaxLife(1.15f);
				pBullet1->SetHeight((int)(64*pBullet->GetScale()));
				pBullet1->SetWidth((int)(32*pBullet->GetScale()));
				pBullet1->SetPosX(pBMS->GetBoss()->GetPosX());
				pBullet1->SetPosY(pBMS->GetBoss()->GetPosY());
				pBullet1->SetDamage(float(8*(pBMS->GetBoss()->GetSpecialLevel())));
				pBullet1->SetBulletType(PROJECTILE_MINI_MISSILE);
				pBullet1->SetBlastRadius(50.0f);
				pBullet1->SetRotation(pBMS->GetBoss()->GetRotation());
				//Missile 3
				pBullet2->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
				pBullet2->SetScale(0.5f);
				pBullet2->SetOwner(pBMS->GetBoss());
				pBullet2->SetCurLife(0.0f);
				pBullet2->SetMaxLife(1.15f);
				pBullet2->SetHeight((int)(64*pBullet->GetScale()));
				pBullet2->SetWidth((int)(32*pBullet->GetScale()));
				pBullet2->SetPosX(pBMS->GetBoss()->GetPosX());
				pBullet2->SetPosY(pBMS->GetBoss()->GetPosY());
				pBullet2->SetDamage(float(8*(pBMS->GetBoss()->GetSpecialLevel())));
				pBullet2->SetBulletType(PROJECTILE_MINI_MISSILE);
				pBullet2->SetBlastRadius(50.0f);
				pBullet2->SetRotation(pBMS->GetBoss()->GetRotation());
				//Missile 4
				pBullet3->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
				pBullet3->SetScale(0.5f);
				pBullet3->SetOwner(pBMS->GetBoss());
				pBullet3->SetCurLife(0.0f);
				pBullet3->SetMaxLife(1.15f);
				pBullet3->SetHeight((int)(64*pBullet->GetScale()));
				pBullet3->SetWidth((int)(32*pBullet->GetScale()));
				pBullet3->SetPosX(pBMS->GetBoss()->GetPosX());
				pBullet3->SetPosY(pBMS->GetBoss()->GetPosY());
				pBullet3->SetDamage(float(8*(pBMS->GetBoss()->GetSpecialLevel())));
				pBullet3->SetBulletType(PROJECTILE_MINI_MISSILE);
				pBullet3->SetBlastRadius(50.0f);
				pBullet3->SetRotation(pBMS->GetBoss()->GetRotation());
				//Missile 5
				pBullet4->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MiniMissilePlaceholder.png",D3DCOLOR_XRGB(255,0,255)));
				pBullet4->SetScale(0.5f);
				pBullet4->SetOwner(pBMS->GetBoss());
				pBullet4->SetCurLife(0.0f);
				pBullet4->SetMaxLife(1.15f);
				pBullet4->SetHeight((int)(64*pBullet->GetScale()));
				pBullet4->SetWidth((int)(32*pBullet->GetScale()));
				pBullet4->SetPosX(pBMS->GetBoss()->GetPosX());
				pBullet4->SetPosY(pBMS->GetBoss()->GetPosY());
				pBullet4->SetDamage(float(8*(pBMS->GetBoss()->GetSpecialLevel())));
				pBullet4->SetBulletType(PROJECTILE_MINI_MISSILE);
				pBullet4->SetBlastRadius(50.0f);
				pBullet4->SetRotation(pBMS->GetBoss()->GetRotation());
				//Set up launch vectors
				tVector2D temp;
				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBMS->GetBoss()->GetRotation()-.5f);
				Vector2DNormalize(temp);
				temp = temp * (450+ pBMS->GetBoss()->GetSpeed());
				pBullet->SetVelX(temp.fX);
				pBullet->SetVelY(temp.fY);
				//
				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBMS->GetBoss()->GetRotation()-.25f);
				Vector2DNormalize(temp);
				temp = temp * (450+ pBMS->GetBoss()->GetSpeed());
				pBullet1->SetVelX(temp.fX);
				pBullet1->SetVelY(temp.fY);
				//
				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBMS->GetBoss()->GetRotation());
				Vector2DNormalize(temp);
				temp = temp * (450+ pBMS->GetBoss()->GetSpeed());
				pBullet2->SetVelX(temp.fX);
				pBullet2->SetVelY(temp.fY);
				//
				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBMS->GetBoss()->GetRotation()+.25f);
				Vector2DNormalize(temp);
				temp = temp * (450+ pBMS->GetBoss()->GetSpeed());
				pBullet3->SetVelX(temp.fX);
				pBullet3->SetVelY(temp.fY);
				//
				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBMS->GetBoss()->GetRotation()+.5f);
				Vector2DNormalize(temp);
				temp = temp * (450+ pBMS->GetBoss()->GetSpeed());
				pBullet4->SetVelX(temp.fX);
				pBullet4->SetVelY(temp.fY);

				pGame->m_pOM->AddObject(pBullet);
				pGame->m_pOM->AddObject(pBullet1);
				pGame->m_pOM->AddObject(pBullet2);
				pGame->m_pOM->AddObject(pBullet3);
				pGame->m_pOM->AddObject(pBullet4);
				pBullet->Release();
				pBullet1->Release();
				pBullet2->Release();
				pBullet3->Release();
				pBullet4->Release();
			}
		}
		break;
	case MSG_CREATE_BOSS_VETTE_SPECIAL:
		{
			CCreateBossVetteSpecial* pBVS = (CCreateBossVetteSpecial*)pMsg;
			CBoss* tempBoss = pBVS->GetBoss();
			if(tempBoss)
			{
				CGamePlayState* pGame = CGamePlayState::GetInstance();
				
				tVector2D temp;
				CBullet* pBullet = new CBullet;
				CBullet* pBullet1 = new CBullet;
				CBullet* pBullet2 = new CBullet;
				CBullet* pBullet3 = new CBullet;
				CBullet* pBullet4 = new CBullet;
				pBullet->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
				pBullet->SetScale(1.0f);
				pBullet->SetOwner(pBVS->GetBoss());
				pBullet->SetCurLife(0.0f);
				pBullet->SetMaxLife(1.0f);
				pBullet->SetHeight((int)(8*pBullet->GetScale()));
				pBullet->SetWidth((int)(8*pBullet->GetScale()));
				pBullet->SetPosX(pBVS->GetBoss()->GetPosX());
				pBullet->SetPosY(pBVS->GetBoss()->GetPosY());
				pBullet->SetDamage(.25f*(pBVS->GetBoss()->GetSpecialLevel()));
				pBullet->SetBulletType(PROJECTILE_BULLET);
				pBullet->SetToRender(false);
				pBullet1->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
				pBullet1->SetScale(1.0f);
				pBullet1->SetOwner(pBVS->GetBoss());
				pBullet1->SetCurLife(0.0f);
				pBullet1->SetMaxLife(1.0f);
				pBullet1->SetHeight((int)(8*pBullet->GetScale()));
				pBullet1->SetWidth((int)(8*pBullet->GetScale()));
				pBullet1->SetPosX(pBVS->GetBoss()->GetPosX());
				pBullet1->SetPosY(pBVS->GetBoss()->GetPosY());
				pBullet1->SetDamage(.25f*(pBVS->GetBoss()->GetSpecialLevel()));
				pBullet1->SetBulletType(PROJECTILE_BULLET);
				pBullet1->SetToRender(false);
				pBullet2->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
				pBullet2->SetScale(1.0f);
				pBullet2->SetOwner(pBVS->GetBoss());
				pBullet2->SetCurLife(0.0f);
				pBullet2->SetMaxLife(1.0f);
				pBullet2->SetHeight((int)(8*pBullet->GetScale()));
				pBullet2->SetWidth((int)(8*pBullet->GetScale()));
				pBullet2->SetPosX(pBVS->GetBoss()->GetPosX());
				pBullet2->SetPosY(pBVS->GetBoss()->GetPosY());
				pBullet2->SetDamage(.25f*(pBVS->GetBoss()->GetSpecialLevel()));
				pBullet2->SetBulletType(PROJECTILE_BULLET);
				pBullet2->SetToRender(false);
				pBullet3->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
				pBullet3->SetScale(1.0f);
				pBullet3->SetOwner(pBVS->GetBoss());
				pBullet3->SetCurLife(0.0f);
				pBullet3->SetMaxLife(1.0f);
				pBullet3->SetHeight((int)(8*pBullet->GetScale()));
				pBullet3->SetWidth((int)(8*pBullet->GetScale()));
				pBullet3->SetPosX(pBVS->GetBoss()->GetPosX());
				pBullet3->SetPosY(pBVS->GetBoss()->GetPosY());
				pBullet3->SetDamage(.25f*(pBVS->GetBoss()->GetSpecialLevel()));
				pBullet3->SetBulletType(PROJECTILE_BULLET);
				pBullet3->SetToRender(false);
				pBullet4->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_FlameParticlePlaceholder.png"));
				pBullet4->SetScale(1.0f);
				pBullet4->SetOwner(pBVS->GetBoss());
				pBullet4->SetCurLife(0.0f);
				pBullet4->SetMaxLife(1.0f);
				pBullet4->SetHeight((int)(8*pBullet->GetScale()));
				pBullet4->SetWidth((int)(8*pBullet->GetScale()));
				pBullet4->SetPosX(pBVS->GetBoss()->GetPosX());
				pBullet4->SetPosY(pBVS->GetBoss()->GetPosY());
				pBullet4->SetDamage(.25f*(pBVS->GetBoss()->GetSpecialLevel()));
				pBullet4->SetBulletType(PROJECTILE_BULLET);
				pBullet4->SetToRender(false);


				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBVS->GetBoss()->GetRotation());
				Vector2DNormalize(temp);
				temp = temp * (450+ pBVS->GetBoss()->GetSpeed());
				pBullet->SetVelX(temp.fX);
				pBullet->SetVelY(temp.fY);

				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBVS->GetBoss()->GetRotation()-.03f);
				Vector2DNormalize(temp);
				temp = temp * (450 + pBVS->GetBoss()->GetSpeed());
				pBullet1->SetVelX(temp.fX);
				pBullet1->SetVelY(temp.fY);

				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBVS->GetBoss()->GetRotation()+.03f);
				Vector2DNormalize(temp);
				temp = temp * (450 + pBVS->GetBoss()->GetSpeed());
				pBullet2->SetVelX(temp.fX);
				pBullet2->SetVelY(temp.fY);

				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBVS->GetBoss()->GetRotation()-.06f);
				Vector2DNormalize(temp);
				temp = temp * (450 + pBVS->GetBoss()->GetSpeed());
				pBullet3->SetVelX(temp.fX);
				pBullet3->SetVelY(temp.fY);

				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,pBVS->GetBoss()->GetRotation()+.06f);
				Vector2DNormalize(temp);
				temp = temp * (450 + pBVS->GetBoss()->GetSpeed());
				pBullet4->SetVelX(temp.fX);
				pBullet4->SetVelY(temp.fY);



				pGame->m_pOM->AddObject(pBullet);
				pGame->m_pOM->AddObject(pBullet1);
				pGame->m_pOM->AddObject(pBullet2);
				pGame->m_pOM->AddObject(pBullet3);
				pGame->m_pOM->AddObject(pBullet4);

				pBullet->Release();
				pBullet1->Release();
				pBullet2->Release();
				pBullet3->Release();
				pBullet4->Release();
			}
		}
		break;
	case MSG_CREATE_BOSS_HUMMER_SPECIAL:
		{
			CCreateBossHummerSpecial* pBHS = (CCreateBossHummerSpecial*)pMsg;
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			CBullet* pBullet = new CBullet;
			//IcyBullet 1
			pBullet->SetImageID (CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_IceLaserPlaceholder.png"));
			pBullet->SetScale(1.0f);
			pBullet->SetOwner(pBHS->GetBoss());
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(3.0f);
			pBullet->SetHeight((int)(32*pBullet->GetScale()));
			pBullet->SetWidth((int)(16*pBullet->GetScale()));
			pBullet->SetDamage(float(1*(pBHS->GetBoss()->GetSpecialLevel())));
			pBullet->SetBulletType(PROJECTILE_BULLET);
			pBullet->SetSlowRate(20.0f);
			pBullet->SetRotation(pBHS->GetBoss()->GetRotation());
			pBullet->SetPosX(pBHS->GetBoss()->GetPosX());
			pBullet->SetPosY(pBHS->GetBoss()->GetPosY());
			tVector2D temp;
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pBHS->GetBoss()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (450 + pBHS->GetBoss()->GetSpeed());
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);
			pGame->m_pOM->AddObject(pBullet);
			pBullet->Release();
		}
		break;
	case MSG_CREATE_BOSS_TRUCK_SPECIAL:
		{
			CCreateBossTruckSpecial* pBTS = (CCreateBossTruckSpecial*)pMsg;
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			//CLandMine* pLandMine = (CLandMine*)pGame->m_pOF->CreateObject("CLandMine");
			CLandMine* pLandMine = new CLandMine();
			//CLandMine* pLandMine1 = new CLandMine();
			//CLandMine* pLandMine2 = new CLandMine();
			pLandMine->SetDuration(20.0f);
			pLandMine->SetScale(1.0f);
			pLandMine->SetOwner(pBTS->GetBoss());
			pLandMine->SetImageID (pGame->m_pTM->LoadTexture("resource/graphics/BattleCars_LandMinePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
			pLandMine->SetCurLife(0.0f);
			pLandMine->SetMaxLife(30.0f);
			pLandMine->SetHeight((int)(64*pLandMine->GetScale()));
			pLandMine->SetWidth((int)(64*pLandMine->GetScale()));
			pLandMine->SetDamage(float(20*(pBTS->GetBoss()->GetSpecialLevel())));
			pLandMine->SetBlastRadius(0.0f);
			pLandMine->SetLandMineType(LM_LM);
			pLandMine->SetVelX(0.0f);
			pLandMine->SetVelY(0.0f);
		/*	pLandMine1->SetDuration(20.0f);
			pLandMine1->SetScale(1.0f);
			pLandMine1->SetOwner(pBTS->GetBoss());
			pLandMine1->SetImageID (pGame->m_pTM->LoadTexture("resource/graphics/BattleCars_LandMinePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
			pLandMine1->SetCurLife(0.0f);
			pLandMine1->SetMaxLife(30.0f);
			pLandMine1->SetHeight((int)(64*pLandMine->GetScale()));
			pLandMine1->SetWidth((int)(64*pLandMine->GetScale()));
			pLandMine1->SetDamage(20*(pBTS->GetBoss()->GetSpecialLevel()));
			pLandMine1->SetBlastRadius(0.0f);
			pLandMine1->SetLandMineType(LM_LM);
			pLandMine1->SetVelX(0.0f);
			pLandMine1->SetVelY(0.0f);
			pLandMine2->SetDuration(20.0f);
			pLandMine2->SetScale(1.0f);
			pLandMine2->SetOwner(pBTS->GetBoss());
			pLandMine2->SetImageID (pGame->m_pTM->LoadTexture("resource/graphics/BattleCars_LandMinePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
			pLandMine2->SetCurLife(0.0f);
			pLandMine2->SetMaxLife(30.0f);
			pLandMine2->SetHeight((int)(64*pLandMine->GetScale()));
			pLandMine2->SetWidth((int)(64*pLandMine->GetScale()));
			pLandMine2->SetDamage(20*(pBTS->GetBoss()->GetSpecialLevel()));
			pLandMine2->SetBlastRadius(0.0f);
			pLandMine2->SetLandMineType(LM_LM);
			pLandMine2->SetVelX(0.0f);
			pLandMine2->SetVelY(0.0f);*/
			tVector2D direction = pBTS->GetBoss()->GetDirection();
			if(direction.fX <=-.75f && (direction.fY <=.25 && direction.fY >= -.25f)) // left
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()+(pBTS->GetBoss()->GetWidth()*.5f));
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()-(pBTS->GetBoss()->GetHeight()*.5f));
				/*pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX());
				pLandMine2->SetPosY(pLandMine->GetPosY() +20+pLandMine->GetHeight());*/
			}
			else if(direction.fX >=.75f && (direction.fY <= .25f && direction.fY >= -.25f)) // right
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()-(pBTS->GetBoss()->GetWidth()*.5f)-pLandMine->GetWidth());
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()-(pBTS->GetBoss()->GetHeight()*.5f));
				//pLandMine1->SetPosX(pLandMine->GetPosX());
				//pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				//pLandMine2->SetPosX(pLandMine->GetPosX());
				//pLandMine2->SetPosY(pLandMine->GetPosY() +20.0f+pLandMine->GetHeight());
			}
			else if(direction.fY >= .75f && (direction.fX <= .25f && direction.fX >= -.25f)) // up
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()-(pLandMine->GetWidth()*.5f));
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()-(pBTS->GetBoss()->GetHeight()*.5f)-pLandMine->GetHeight());
				//pLandMine1->SetPosX(pLandMine->GetPosX() - 20.0f-pLandMine->GetWidth());
				//pLandMine1->SetPosY(pLandMine->GetPosY());
				//pLandMine2->SetPosX(pLandMine->GetPosX() + 20+pLandMine->GetWidth());
				//pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fY <= -.75f && (direction.fX <= .25f && direction.fX >= -.25f)) // down
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()-(pLandMine->GetWidth()*.5f));
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()+(pBTS->GetBoss()->GetHeight()*.5f));
				//pLandMine1->SetPosX(pLandMine->GetPosX() - 20.0f-pLandMine->GetWidth());
				//pLandMine1->SetPosY(pLandMine->GetPosY());
				//pLandMine2->SetPosX(pLandMine->GetPosX() + 20+pLandMine->GetWidth());
				//pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fX <= 0 && direction.fY <= 0) // up left
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()+(pBTS->GetBoss()->GetWidth()*.5f));
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()+(pBTS->GetBoss()->GetHeight()*.5f));
				//pLandMine1->SetPosX(pLandMine->GetPosX());
				//pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				//pLandMine2->SetPosX(pLandMine->GetPosX() - 20.0f - pLandMine->GetWidth());
				//pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fX <= 0 && direction.fY > 0) // down left
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()+(pBTS->GetBoss()->GetWidth()*.5f));
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()-(pBTS->GetBoss()->GetHeight()*.5f) - pLandMine->GetHeight());
				/*pLandMine1->SetPosX(pLandMine->GetPosX());
				pLandMine1->SetPosY(pLandMine->GetPosY() + 20.0f + pLandMine->GetHeight());
				pLandMine2->SetPosX(pLandMine->GetPosX() - 20.0f - pLandMine->GetWidth());
				pLandMine2->SetPosY(pLandMine->GetPosY());*/
			}
			else if(direction.fX > 0 && direction.fY <=0) // up right
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()-(pBTS->GetBoss()->GetWidth()*.5f) - pLandMine->GetWidth());
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()+(pBTS->GetBoss()->GetHeight()*.5f));
				//pLandMine1->SetPosX(pLandMine->GetPosX());
				//pLandMine1->SetPosY(pLandMine->GetPosY() - 20.0f - pLandMine->GetHeight());
				//pLandMine2->SetPosX(pLandMine->GetPosX() + 20.0f + pLandMine->GetWidth());
				//pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			else if(direction.fX > 0 && direction.fY  > 0) // down right
			{
				pLandMine->SetPosX(pBTS->GetBoss()->GetPosX()-(pBTS->GetBoss()->GetWidth()*.5f) - pLandMine->GetWidth());
				pLandMine->SetPosY(pBTS->GetBoss()->GetPosY()-(pBTS->GetBoss()->GetHeight()*.5f) - pLandMine->GetHeight());
				//pLandMine1->SetPosX(pLandMine->GetPosX());
				//pLandMine1->SetPosY(pLandMine->GetPosY() + 20.0f + pLandMine->GetHeight());
				//pLandMine2->SetPosX(pLandMine->GetPosX() + 20.0f + pLandMine->GetWidth());
				//pLandMine2->SetPosY(pLandMine->GetPosY());
			}
			pGame->m_pOM->AddObject(pLandMine);
			//pGame->m_pOM->AddObject(pLandMine1);
			//pGame->m_pOM->AddObject(pLandMine2);
			pLandMine->Release();
			//pLandMine1->Release();
			//pLandMine2->Release();
		}
		break;
		case MSG_DESTROY_BOSS:
			{
				CDestroyBossMessage* pBTS = (CDestroyBossMessage*)pMsg;
				CGamePlayState* pGame = CGamePlayState::GetInstance();

				for(unsigned int i = 0; i < pGame->GetBosses().size(); i++)
				{
					if(pGame->GetBosses()[i] == pBTS->GetBoss())
					{
						pGame->GetBosses()[i] = NULL;
						pGame->GetObjectManager()->RemoveObject(pGame->GetBosses()[i]);
						if(i == 0)
							pGame->SetBossHasDied(true);
						else if(i == 1)
							pGame->SetMiniBossHasDied(true);
					}
				}
			}
			break;
	}
}

void CGamePlayState::SortScores(void)
{
	for(int i = (int)(m_lScores.size()-1); i >= 0; i--)
	{
		for(int j = i; j >= 0; j--)
		{
			if(m_bCollectionChallenge)
			{
				if (m_lScores[i]->GetCollected () > m_lScores[j]->GetCollected ())
				{
					CCar* tempcar;
					tempcar = m_lScores[i];
					m_lScores[i] = m_lScores[j];
					m_lScores[j] = tempcar;
				}
			}else if(m_lScores[i]->GetKillCount() > m_lScores[j]->GetKillCount())
			{
				CCar* tempcar;
				tempcar = m_lScores[i];
				m_lScores[i] = m_lScores[j];
				m_lScores[j] = tempcar;
			}
		}
	}
}

void CGamePlayState::Setvolume(void)
{
	if(m_nCountDownEnd != 0)
	m_pFM->SetVolume(m_nCountDownEnd,CGame::GetInstance()->getSoundAVolume());
	if(m_nCountDown != 0)
	m_pFM->SetVolume(m_nCountDown,CGame::GetInstance()->getSoundAVolume());
	if(m_nBgMusicID != 0)
	m_pFM->SetVolume(m_nBgMusicID,CGame::GetInstance()->getSoundBVolume());
}