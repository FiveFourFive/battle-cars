
#include "CMainMenuState.h"
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
#include "CGameModeSelectionState.h"
#include "CLevelSelectionState.h"
#include "CCreditsScreenState.h"
#include "ParticleManager.h"

enum menu {PLAY, OPTIONS, HOW_TO_PLAY, GAMER_PROFILE, HIGH_SCORES, CREDITS, EXIT};

CMainMenuState::CMainMenuState(void)
{
	m_pPF = NULL;
	m_pTM = NULL;
	m_pFM = NULL;
	m_pDI = NULL;
	m_pController1 = NULL;
	m_pController2 = NULL;

	m_nBGImageID = -1;

}
CMainMenuState::~CMainMenuState(void)
{
	
}

CMainMenuState::CMainMenuState(const CMainMenuState&)
{

}

CMainMenuState& CMainMenuState::operator=(const CMainMenuState&)
{
	return *this;
}

CMainMenuState* CMainMenuState::GetInstance(void)
{
	static CMainMenuState instance;
	return &instance;
}

void CMainMenuState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();

	m_pTM = CSGD_TextureManager::GetInstance();
	
	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();

	
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/collision.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/missle_flame.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/explosion.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/car_exploded.xml");
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));
	m_pPF = new CPrintFont(m_nFontID);

	m_pFM = CSGD_FModManager::GetInstance();
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/Afterlife.mp3",SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	float soundavol = CGame::GetInstance()->getSoundAVolume();
	float soundbvol = CGame::GetInstance()->getSoundBVolume();
	//m_pFM->SetVolume(m_nBackgroundMusicID,CGame::GetInstance()->getSoundBVolume());
	//m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	//m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
	m_pFM->PlaySound(m_nBackgroundMusicID);
	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/mainmenu_bg.jpg");

}

void CMainMenuState::Exit(void)
{
	m_pFM->StopSound(m_nBackgroundMusicID);
	m_pTM->UnloadTexture(m_nFontID);
	if( m_pPF )
	{
		delete m_pPF;
		m_pPF = NULL;
	}

}

bool CMainMenuState::Input(void)
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
		if(xState.Gamepad.wButtons & tempkeys->GetAccept())
		{
			return this->HandleEnter();
		}


		if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			//int x = 0;
			//x = XINPUT_GAMEPAD_START;
			//int y = 0;
			//y = x;
			return this->HandleEnter();
		}

		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetThumbDelay();
		if(x < 8000 && x > -8000 && y > 16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			
			m_nSelection--;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection < 0)
				m_nSelection = 6;
		}
		else if(x < 8000 && x > -8000 && y < -16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection > 6)
				m_nSelection = 0;
		}
		}
		
	}
	else
	{

		if(m_pDI->KeyPressed(DIK_RETURN))
		{
			return this->HandleEnter();
		}
	
		if(m_pDI->KeyPressed(DIK_UP))
		{
			m_nSelection--;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection < 0)
				m_nSelection = 6;
		}

		if(m_pDI->KeyPressed(DIK_DOWN))
		{
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection > 6)
				m_nSelection = 0;
		}
	}
	return true;
}

void CMainMenuState::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CMainMenuState::Render(void)
{

	m_pTM->Draw(m_nBGImageID, 0,0, 2.9f, 1.8f);
	
	m_pPF->Print("BATTLE CARS",220,50,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("MAYHEM AND DESTRUCTION",50,100,1.0f,D3DCOLOR_XRGB(200, 0, 0));

	m_pPF->Print("PLAY",300,200,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("OPTIONS",300,250,0.5f,D3DCOLOR_XRGB(200, 0, 0));	
	m_pPF->Print("HOW TO PLAY",300,300,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("GAMER PROFILE",300,350,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("HIGHSCORES",300,400,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("CREDITS",300,450,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("EXIT",300,500,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	switch(m_nSelection)
		{
		case PLAY:			
			m_pPF->Print("PLAY",300,200,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case OPTIONS:
			m_pPF->Print("OPTIONS",300,250,0.5f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case HOW_TO_PLAY:
			m_pPF->Print("HOW TO PLAY",300,300,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case GAMER_PROFILE:
			m_pPF->Print("GAMER PROFILE",300,350,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case HIGH_SCORES:
			m_pPF->Print("HIGHSCORES",300,400,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case CREDITS:
			m_pPF->Print("CREDITS",300,450,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case EXIT:
			m_pPF->Print("EXIT",300,500,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		}
}

bool CMainMenuState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nMenuSelect);
		switch(m_nSelection)
		{
		case PLAY:
			CGame::GetInstance()->AddState(CGameModeSelectionState::GetInstance());
			break;
		case OPTIONS:
			CGame::GetInstance()->AddState(COptionState::GetInstance());
			break;
		case HOW_TO_PLAY:
			CGame::GetInstance()->AddState(CHowToPlayState::GetInstance());
			break;
		case GAMER_PROFILE:
			CGame::GetInstance()->ChangeState(CGamerProfile::GetInstance());
			break;
		case HIGH_SCORES:
			//CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			break;
		case CREDITS:
			CGame::GetInstance()->ChangeState(CCreditsScreenState::GetInstance());
			break;
		case EXIT:
			//Exit();
			//CGame::GetInstance()->Shutdown();
			return false;
			break;
		}
	return true;
}



