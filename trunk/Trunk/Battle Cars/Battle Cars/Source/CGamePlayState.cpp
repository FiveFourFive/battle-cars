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
	m_pFM->SetVolume(m_nBackgroundMusicID,0.2f);
	m_pFM->PlaySound(m_nBackgroundMusicID);
	CGame::GetInstance()->ResetInputDelay();
	player = new CPlayer();
	dummy = new CEnemy();
	power_up = new PowerUp();
	player->SetPosX(500);
	player->SetPosY(500);
	player->SetHealth(100);
	player->SetMaxHealth(100);
	player->SetPowerUpBar(0);
	player->SetShieldBar(100);
	player->SetMaxPowerUp(100);
	player->SetMaxSpeed(200);
	player->SetMaxShield(100);
	player->SetVelX(-20);
	player->SetType(OBJECT_PLAYER);

	dummy->SetPosX(50);
	dummy->SetPosY(50);
	dummy->SetVelX(20);
	dummy->SetVelY(20);
	dummy->SetSpeed(100);
	dummy->SetType(OBJECT_ENEMY);
	dummy->SetKillCount(5);
	Level->SetSpawn (player);

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
	m_pOM->AddObject(speedy);
	m_pOM->AddObject(player);
	m_pOM->AddObject(dummy);
	m_pOM->AddObject(power_up);
	m_bCountDown = false;
	m_fEnlarge = 0.0f;
	m_bPlaying = false;
	m_fCountDown = 0.0f;

	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();


	m_pD3D->DrawRect(lowerhalf, 58,58,58);
	regular_load.right += 75;
	m_pD3D->DrawRect(regular_load, 0, 0, 255);


	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

	m_pPM->LoadEmittor("resource/data/temp.xml");
	//Level.Load ("Resource/Data/TestMap.xml");
	Emittor* tempemittor = m_pPM->GetEmittor(0);

	/*temp.left = 0;
	temp.top = 0;
	temp.right = temp.left + CGame::GetInstance ()->GetScreenWidth();
	temp.bottom = temp.top + CGame::GetInstance ()->GetScreenHeight();*/

	if( tempemittor)
	{
		//tempemittor->SetIsDead(false);
		//tempemittor->SetIsActive(false);
	}

	time = 120;
	m_fElapsedSecond = 0.0f;
	score = 0;

	m_pPM->AttachToBasePosition(player, tempemittor, 0, 0);

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
	player->Release();
	dummy->Release();
	speedy->Release();
	m_pPM->ShutDownParticleManager();
	m_lScores.clear();

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
				if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
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
			if(m_pDI->KeyPressed(DIK_M))
			{
				CGame::GetInstance()->RemoveState(this);
				CGame::GetInstance()->AddState(CMainMenuState::GetInstance());
			}
			if(m_pDI->KeyPressed(DIK_P))
			{
				CGame::GetInstance()->AddState(CPauseMenuState::GetInstance());
			}
		}
	}
	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{
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
		m_pMS->ProcessMessages ();

		if(player->GetHealth() <= 0)
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			CGame::GetInstance()->ResetInputDelay();
		}
		for(unsigned int i = 0; i < m_lScores.size() - 1; i++)
		{
			for(unsigned int m = 1; m< m_lScores.size(); m++)
			{
				if(m_lScores[i]->GetKillCount() < m_lScores[m]->GetKillCount())
				{
					CCar* tempcar = m_lScores[i];
					m_lScores[i] = m_lScores[m];
					m_lScores[m] = tempcar;
				}
			}
		}

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
	if( time < 0 )
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
	m_pPM->UpdateEmittors(fElapsedTime);
}

void CGamePlayState::Render(void)
{
	RECT cam = player->GetCamera()->GetRect();
	Level->Render (player->GetCamera ()->GetRect ());

	m_pOM->RenderObjects();
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

	m_pPM->RenderEmittors();
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

			level->Load ("Resource/Data/TestMap.xml");

			break;
		}
	case MSG_CREATE_PLAYER_BULLET:
		{

			CCreatePlayerBulletMessage* pCBM = (CCreatePlayerBulletMessage*)pMsg;

			CGamePlayState* pGame = CGamePlayState::GetInstance();

			//CBullet* pBullet = (CBullet*)pGame->m_pOF->CreateObject("CBullet");
			CBullet* pBullet = new CBullet();
			pBullet->SetType (2);
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

			pGame->m_pOM->AddObject(pBullet);

			pBullet->Release();
			break;
		}
	case MSG_DESTROY_BULLET:
		{
			CDestroyBulletMessage* pM = (CDestroyBulletMessage*)pMsg;
			CBullet* tempbullet = pM->GetBullet();
			CGamePlayState* pGame = CGamePlayState::GetInstance();
			pGame->m_pOM->RemoveObject(tempbullet);
			break;
		}
	}
}