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
#include "COptionState.h"
#include "CPauseMenuState.h"
#include "CNumPlayers.h"
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
#include "CKeyBinds.h"
#include "ParticleManager.h"
#include "CCharacterSelection.h"
#include "Emittor.h"
#include "tinyxml.h"
#include "CLandMine.h"
#include "CLevelSelectionState.h"

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
	m_pCharacters = CCharacterSelection::GetInstance()->GetList();

	RECT lowerhalf = { 200, 500, 600, 540};
	RECT regular_load = { 200, 500, 201, 540};
	int offset = 0;

	Level = CLevel::GetInstance ();

	m_pD3D	=	CSGD_Direct3D::GetInstance();

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();


	m_pD3D->GetSprite()->Flush();
	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	m_pD3D->DrawRect(regular_load, 0, 0, 255);
	m_pD3D->GetSprite()->Flush();


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

	m_pTM	=	CSGD_TextureManager::GetInstance();
	m_pFM	=	CSGD_FModManager::GetInstance();
	m_pDI	=	CSGD_DirectInput::GetInstance();
	m_pOM	=	CObjectManager::GetInstance();
	m_pOF	=	CObjectFactory<string, CBase>::GetInstance();
	m_pMS	=	CMessageSystem::GetInstance ();
	m_pES = CEventSystem::GetInstance ();

	m_pPM	=	ParticleManager::GetInstance();
	m_pMS->InitMessageSystem (MessageProc);

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	regular_load.right += 25;
	m_pD3D->DrawRect(regular_load, 0, 0, 255);


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

	m_pMS->SendMsg (new CCreateLevelMessage());
	m_pMS->ProcessMessages ();

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	regular_load.right += 100;
	m_pD3D->DrawRect(regular_load, 0, 0, 255);


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();
	m_pPF = new CPrintFont(m_pTM->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0)));



	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/Superbeast.mp3",SGD_FMOD_LOOPING);
	m_nCountDown = m_pFM->LoadSound("resource/sounds/Countdown.mp3");
	m_nCountDownEnd = m_pFM->LoadSound("resource/sounds/Countdowntone.mp3");
	m_pFM->PlaySound(m_nBackgroundMusicID);
	CGame::GetInstance()->ResetInputDelay();

	
	dummy = new CEnemy();
	power_up = new PowerUp();
	dummy2 = new CCar();

	dummy->SetPosX(1500);
	dummy->SetPosY(1800);
	dummy->SetVelX(0);
	dummy->SetVelY(0);
	dummy->SetSpeed(0);
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

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();


	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	regular_load.right += 100;
	m_pD3D->DrawRect(regular_load, 0, 0, 255);


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	regular_load.right += 25;
	m_pD3D->DrawRect(regular_load, 0, 0, 255);


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

	speedy = new CSpeedRamp();
	tVector2D tempVector;
	tempVector.fX = 1;
	tempVector.fY = 1;
	dummy->SetDirection(tempVector);
	//dummy->SetVelX(10);
	m_pOM->AddObject(dummy2);
	m_pOM->AddObject(speedy);
	m_pOM->AddObject(dummy);
	m_pOM->AddObject(power_up);
	player = CCharacterSelection::GetInstance()->GetPlayer1();
	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
		player2 = CCharacterSelection::GetInstance()->GetPlayer2();
	else
		player2 = m_pCharacters[3];

	player->Rotate(0.0f);
	player2->Rotate(0.0f);
	player2->SetController(m_pController2);
	m_pOM->AddObject(player);
	m_pOM->AddObject(player2);
	m_bCountDown = false;
	m_fEnlarge = 0.0f;
	m_bPlaying = false;
	m_fCountDown = 0.0f;

	Level->SetSpawn (player2);

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();


	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	regular_load.right += 75;
	m_pD3D->DrawRect(regular_load, 0, 0, 255);


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

	m_pPM->LoadEmittor("resource/data/collision.xml");

	time = 120;
	m_fElapsedSecond = 0.0f;
	score = 0;

	player2->SetPosX(550);

	

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();


	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	regular_load.right += 75;
	m_pD3D->DrawRect(regular_load, 0, 0, 255);


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();


}

void CGamePlayState::Exit(void)
{
	for(unsigned int i = 0; i < m_pCharacters.size(); i++)
	{
		m_pCharacters[i]->Release();
	}
	//player->Release();
	//player2->Release();
	//player2->Release();
	dummy->Release();
	speedy->Release();
	dummy2->Release();
	m_pPM->ShutDownParticleManager();
	m_pPM = NULL;
	m_lScores.clear();
	power_up->Release ();

	m_pES->ClearEvents ();
	m_pES->ShutdownEventSystem ();

	Level->Shutdown ();

	m_pFM->StopSound(m_nBackgroundMusicID);
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
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = m_pController1->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		CKeyBinds* tempkeys = m_pController1->GetKB();
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;

		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetInputDelay();
			/*if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_START)
			{
				CGame::GetInstance()->AddState(CPauseMenuState::GetInstance());
				CPauseMenuState::GetInstance()->SetController(m_pController1);
				return true;
			}*/
			if(!m_bCountDown)
			{
				if(xState.Gamepad.wButtons & tempkeys->GetAccept())
				{

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
			if(m_pDI->KeyPressed(DIK_RETURN))
			{
				m_pFM->PlaySound(m_nCountDown);
				m_bCountDown = true;
			}
		}
		if(m_bPlaying)
		{
			if(m_pDI->KeyPressed(DIK_P) || m_pDI->KeyPressed(DIK_ESCAPE))
			{
				CGame::GetInstance()->AddState(CPauseMenuState::GetInstance());
			}
		}
	}
	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{
	m_pFM->SetVolume(m_nCountDownEnd,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nCountDown,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nBackgroundMusicID,CGame::GetInstance()->getSoundBVolume());

	if( COptionState::GetInstance()->IsVertical() )
	{
		player->GetCamera()->SetRenderPosX(0);
		player->GetCamera()->SetRenderPosY(0);

	
		player2->GetCamera()->SetRenderPosX(400);
		player2->GetCamera()->SetRenderPosY(0);
	}
	else
	{
		player->GetCamera()->SetRenderPosX(0);
		player->GetCamera()->SetRenderPosY(0);

	
		player2->GetCamera()->SetRenderPosX(0);
		player2->GetCamera()->SetRenderPosY(300);
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

		if( m_ftimer >= 3.0f)
		{
			m_ftimer = 0.0f;
			score += 1;
		}

		m_pFM->Update();
		m_pOM->UpdateObjects(fElapsedTime);
		Level->CheckCameraCollision (player->GetCamera ());
		Level->CheckWorldCollision (player);

		m_pES->ProcessEvents ();
		m_pPM->UpdateEmittors(fElapsedTime);
		m_pMS->ProcessMessages ();
		if(player->GetHealth() <= 0)
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			CGame::GetInstance()->ResetInputDelay();
		}
		for(unsigned int i = 0; i < m_lScores.size() - 1; i++)
		{
			for(unsigned int m = 0; m< m_lScores.size(); m++)
			{
				if(m_lScores[m]->GetKillCount() > m_lScores[i]->GetKillCount())
				{
					CCar* tempcar = m_lScores[m];
					m_lScores[m] = m_lScores[i];
					m_lScores[i] = tempcar;
				}
			}
		}

		
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
	if( time < 0 )
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}

		
}

void CGamePlayState::Render(void)
{
	Level->Render (player->GetCamera ());
	m_pOM->RenderObjects(player->GetCamera());

	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		Level->Render(player2->GetCamera());
		m_pOM->RenderObjects(player2->GetCamera());
	}

	


	if(!m_bPlaying)
	{
		char buffer[32];
		float tempcount = 5.0f-m_fCountDown;
		sprintf_s(buffer,"%.2f",tempcount);
		m_pPF->Print(buffer,380,280,1.0,D3DCOLOR_XRGB(255, 255, 255));
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
		m_pPF->Print(timebuff, 350, 30, 1.0, D3DCOLOR_XRGB(255,255,255)); 
	}
	for(unsigned int i = 0; i < m_lScores.size(); i++)
	{
		char buffer[32];
		int color = 0;
		sprintf_s(buffer,"%i) %i",i+1,m_lScores[i]->GetKillCount());
		if(m_lScores[i]->GetType() == OBJECT_PLAYER)
		{
			color = 255;
		}
		//color = 255;
		m_pD3D->DrawText(buffer,2,370+(12*i),0,color,0);
	}
	char scorebuff[32];
	sprintf_s(scorebuff, "SCORE:%i", score);
	m_pPF->Print(scorebuff, 380, 550, 1.0, D3DCOLOR_XRGB(255,255,255));

	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		if( COptionState::GetInstance()->IsVertical())
		{
			m_pD3D->DrawLine(400, 0, 400, 600, 255,0,0);
			player2->GetCamera()->AttachTo(player2, 200, 300);
			player->GetCamera()->AttachTo(player, 200, 300);
		}
		else
		{
			m_pD3D->DrawLine(0, 300, 800, 300, 255,0,0);
			player2->GetCamera()->AttachTo(player2, 400, 150);
			player->GetCamera()->AttachTo(player, 400, 150);
		}

	}

	
	

	m_pPM->RenderEmittors(player->GetCamera());
}

bool CGamePlayState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nCountDown);
	m_bCountDown = true;
	return true;
}



void CGamePlayState::MessageProc(CBaseMessage* pMsg)
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
	case MSG_CREATE_PLAYER_BULLET:
		{

			CCreatePlayerBulletMessage* pCBM = (CCreatePlayerBulletMessage*)pMsg;

			CGamePlayState* pGame = CGamePlayState::GetInstance();

			//CBullet* pBullet = (CBullet*)pGame->m_pOF->CreateObject("CBullet");
			CBullet* pBullet = new CBullet();
			pBullet->SetImageID (pCBM->GetPlayer()->GetBulletImageID());

			tVector2D temp;
			//temp = pCBM->GetPlayer()->GetDirection();
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCBM->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * 350;

			pBullet->SetScale(0.5f);
			pBullet->SetOwner(pCBM->GetPlayer());
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
			pBullet->SetPosX(pCBM->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCBM->GetPlayer()->GetPosY());
			pBullet->SetDamage(2);
			pBullet->SetBulletType(PROJECTILE_BULLET);
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
			//temp = pCBM->GetPlayer()->GetDirection();
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCBM->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * 350;

			pBullet->SetScale(0.5f);
			pBullet->SetOwner(pCBM->GetPlayer());
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
			pBullet->SetHeight((int)(32*pBullet->GetScale()));
			pBullet->SetWidth((int)(64*pBullet->GetScale()));
			pBullet->SetPosX(pCBM->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCBM->GetPlayer()->GetPosY());
			pBullet->SetDamage(4);
			pBullet->SetBulletType(PROJECTILE_MISSILE);
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
			pBullet->SetImageID (pCMS->GetPlayer()->GetMissileImageID());
			pBullet->SetScale(0.5f);
			pBullet->SetOwner(pCMS->GetPlayer());
			pBullet->SetCurLife(0.0f);
			pBullet->SetMaxLife(3.0f);
			pBullet->SetHeight((int)(32*pBullet->GetScale()));
			pBullet->SetWidth((int)(64*pBullet->GetScale()));
			pBullet->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet->SetDamage(8);
			pBullet->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet->SetBlastRadius(50.0f);
			//Missile 2
			pBullet1->SetImageID (pCMS->GetPlayer()->GetMissileImageID());
			pBullet1->SetScale(0.5f);
			pBullet1->SetOwner(pCMS->GetPlayer());
			pBullet1->SetCurLife(0.0f);
			pBullet1->SetMaxLife(3.0f);
			pBullet1->SetHeight((int)(32*pBullet->GetScale()));
			pBullet1->SetWidth((int)(64*pBullet->GetScale()));
			pBullet1->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet1->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet1->SetDamage(8);
			pBullet1->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet1->SetBlastRadius(50.0f);
			//Missile 3
			pBullet2->SetImageID (pCMS->GetPlayer()->GetMissileImageID());
			pBullet2->SetScale(0.5f);
			pBullet2->SetOwner(pCMS->GetPlayer());
			pBullet2->SetCurLife(0.0f);
			pBullet2->SetMaxLife(3.0f);
			pBullet2->SetHeight((int)(32*pBullet->GetScale()));
			pBullet2->SetWidth((int)(64*pBullet->GetScale()));
			pBullet2->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet2->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet2->SetDamage(8);
			pBullet2->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet2->SetBlastRadius(50.0f);
			//Missile 4
			pBullet3->SetImageID (pCMS->GetPlayer()->GetMissileImageID());
			pBullet3->SetScale(0.5f);
			pBullet3->SetOwner(pCMS->GetPlayer());
			pBullet3->SetCurLife(0.0f);
			pBullet3->SetMaxLife(3.0f);
			pBullet3->SetHeight((int)(32*pBullet->GetScale()));
			pBullet3->SetWidth((int)(64*pBullet->GetScale()));
			pBullet3->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet3->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet3->SetDamage(8);
			pBullet3->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet3->SetBlastRadius(50.0f);
			//Missile 5
			pBullet4->SetImageID (pCMS->GetPlayer()->GetMissileImageID());
			pBullet4->SetScale(0.5f);
			pBullet4->SetOwner(pCMS->GetPlayer());
			pBullet4->SetCurLife(0.0f);
			pBullet4->SetMaxLife(3.0f);
			pBullet4->SetHeight((int)(32*pBullet->GetScale()));
			pBullet4->SetWidth((int)(64*pBullet->GetScale()));
			pBullet4->SetPosX(pCMS->GetPlayer()->GetPosX());
			pBullet4->SetPosY(pCMS->GetPlayer()->GetPosY());
			pBullet4->SetDamage(8);
			pBullet4->SetBulletType(PROJECTILE_MINI_MISSILE);
			pBullet4->SetBlastRadius(50.0f);
			//Set up launch vectors
			tVector2D temp;
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()-.5f);
			Vector2DNormalize(temp);
			temp = temp * 350;
			pBullet->SetVelX(temp.fX);
			pBullet->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()-.25f);
			Vector2DNormalize(temp);
			temp = temp * 350;
			pBullet1->SetVelX(temp.fX);
			pBullet1->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation());
			Vector2DNormalize(temp);
			temp = temp * 350;
			pBullet2->SetVelX(temp.fX);
			pBullet2->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()+.25f);
			Vector2DNormalize(temp);
			temp = temp * 350;
			pBullet3->SetVelX(temp.fX);
			pBullet3->SetVelY(temp.fY);
			//
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,pCMS->GetPlayer()->GetRotation()+.5f);
			Vector2DNormalize(temp);
			temp = temp * 350;
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
	case MSG_DESTROY_BULLET:
		{
			CDestroyBulletMessage* pM = (CDestroyBulletMessage*)pMsg;
			CBullet* tempbullet = pM->GetBullet();
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			if(tempbullet->GetBulletType() == PROJECTILE_BULLET)
				pGame->m_pOM->RemoveObject(tempbullet);
			else if(tempbullet->GetBulletType() == PROJECTILE_MISSILE || tempbullet->GetBulletType() == PROJECTILE_MINI_MISSILE)
			{
				CLandMine* pLandMine = new CLandMine();
				pLandMine->SetDuration(.10f);
				pLandMine->SetScale(2.0f);
				pLandMine->SetOwner(tempbullet->GetOwner());
				pLandMine->SetImageID (pGame->m_pTM->LoadTexture("resource/graphics/BattleCars_MissileExplosionPlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
				pLandMine->SetCurLife(0.0f);
				pLandMine->SetMaxLife(1000000.0f);
				pLandMine->SetHeight((int)(64*pLandMine->GetScale()));
				pLandMine->SetWidth((int)(64*pLandMine->GetScale()));
				pLandMine->SetPosX(tempbullet->GetPosX());
				pLandMine->SetPosY(tempbullet->GetPosY());
				if(tempbullet->GetBulletType() == PROJECTILE_MISSILE)
					pLandMine->SetDamage(1);
				else
					pLandMine->SetDamage(7);
				pLandMine->SetBlastRadius(tempbullet->GetBlastRadius());
				pLandMine->SetLandMineType(LM_EXPLOSION);
				pGame->m_pOM->AddObject(pLandMine);
				pGame->m_pOM->RemoveObject(tempbullet);
				pLandMine->Release();
			}
			else if(tempbullet->GetBulletType() == PROJECTILE_LANDMINE)
				pGame->m_pOM->RemoveObject(tempbullet);
			break;
		}
	}
}