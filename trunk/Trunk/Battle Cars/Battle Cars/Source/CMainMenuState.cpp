
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
enum menu {PLAY, OPTIONS, HOW_TO_PLAY, GAMER_PROFILE, HIGH_SCORES, CREDITS, EXIT};

CMainMenuState::CMainMenuState(void)
{
	m_pPF = NULL;
	m_pTM = NULL;
	m_pFM = NULL;
	m_pDI = NULL;
	m_pController1 = NULL;
	m_pController2 = NULL;

	m_bPlayerSelection = false;
	m_nNumberOfPlayers = 0;
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


	m_nFontID = m_pTM->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0));
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

}

void CMainMenuState::Exit(void)
{
	m_pFM->StopSound(m_nBackgroundMusicID);
	m_pTM->UnloadTexture(m_nFontID);
	delete m_pPF;

	m_bPlayerSelection = false;
}

bool CMainMenuState::Input(void)
{
	if( m_bPlayerSelection )
	{
		if( m_pDI->KeyPressed(DIK_UP))
		{
			m_nSelection--;

			if( m_nNumberOfPlayers >= 1)
			{

				if( m_nSelection < 0 )
					m_nSelection = 2;
			}
			else
			{
					if( m_nSelection < 0 )
					m_nSelection = 1;
			}
		}
		if( m_pDI->KeyPressed(DIK_DOWN))
		{
			m_nSelection++;

			if( m_nNumberOfPlayers >= 1)
			{

				if( m_nSelection > 2 )
					m_nSelection = 0;
			}
			else
			{
					if( m_nSelection > 1 )
					m_nSelection = 0;
			}
		}

		if( m_pDI->KeyPressed(DIK_RETURN))
		{
			switch( m_nSelection)
			{
			case 0:
				m_nNumberOfPlayers = 1;
				break;
			case 1:
				m_nNumberOfPlayers = 2;
				break;
			case 2:
				CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
				break;
			}
		}

		return true;
	}

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
		if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
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

	if( m_bPlayerSelection )
	{


		m_pPF->Print("BATTLE CARS",220,50,1.0f,D3DCOLOR_XRGB(200, 0, 0));
		m_pPF->Print("MAYHEM AND DESTRUCTION",50,100,1.0f,D3DCOLOR_XRGB(200, 0, 0));

		m_pPF->Print("1 PLAYER",300,200,0.5f,D3DCOLOR_XRGB(200, 0, 0));
		m_pPF->Print("2 PLAYER",300,250,0.5f,D3DCOLOR_XRGB(200, 0, 0));	
		m_pPF->Print("BEGIN",300,300,0.5f,D3DCOLOR_XRGB(200, 0, 0));


		switch(m_nSelection)
		{
		case 0:			
			m_pPF->Print("1 PLAYER",300,200,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case 1:
			m_pPF->Print("2 PLAYER",300,250,0.5f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case 2:
			m_pPF->Print("BEGIN",300,300,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		}

		char buffer[32];
		sprintf_s(buffer, "NUMBER OF PLAYERS: %i", m_nNumberOfPlayers);
		m_pPF->Print(buffer, 100, 500, 1.0f, D3DCOLOR_XRGB(0, 255, 128));

		return;
	}


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
			m_bPlayerSelection = true;
			//CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
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
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			break;
		case CREDITS:
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			break;
		case EXIT:
			//Exit();
			//CGame::GetInstance()->Shutdown();
			return false;
			break;
		}
	return true;
}



