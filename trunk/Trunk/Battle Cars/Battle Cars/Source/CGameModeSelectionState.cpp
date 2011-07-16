#include "CGameModeSelectionState.h"
#include "CDeathmatchMode.h"
#include "CTutorialMode.h"
#include "CTimeChallengeMode.h"
#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CXboxInput.h"
#include "CPlayer.h"
#include "CGamePlayState.h"
#include "CLevelSelectionState.h"
#include "CNumPlayers.h"
#include "CGame.h"
#include "CCollectionMode.h"

enum options{WS_DM,WS_TT,WS_CC, WS_TUT,WS_EXIT};
CGameModeSelectionState::CGameModeSelectionState(void)
{
	m_pTM  = NULL;
	m_pPF  = NULL;
	m_pFM  = NULL;
	m_pDI  = NULL;

	m_nBGImageID = -1;
}

CGameModeSelectionState::~CGameModeSelectionState(void)
{

}

CGameModeSelectionState* CGameModeSelectionState::GetInstance(void)
{
	static CGameModeSelectionState instance;
	return &instance;
}

void CGameModeSelectionState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();

	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));
	m_pPF = new CPrintFont(m_nFontID);

	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/mainmenu_bg.jpg");
}

void CGameModeSelectionState::Exit(void)
{
	delete m_pPF;
}
bool CGameModeSelectionState::Input(void)
{
	if(CGame::GetInstance()->ControllerInput())
	{
		XINPUT_STATE xState = CGame::GetInstance()->GetController1()->GetState();

		//m_pController1->ReadInputState();
		
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				return this->HandleEnter();
			}
			else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				CGame::GetInstance()->RemoveState(this);
			}
		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
				CGame::GetInstance()->ResetThumbDelay();
				if(x < 8000 && x > -8000 && y > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					
						m_nSelection--;
						m_pFM->PlaySound(m_nMenuMove);
						if(m_nSelection < 0)
							m_nSelection = 3;

					
				}
				else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					
						m_nSelection++;
						m_pFM->PlaySound(m_nMenuMove);
						if(m_nSelection > 3)
							m_nSelection = 0;

					
				}
		}
	}
	else
	{
		if(m_pDI->KeyPressed(DIK_ESCAPE))
		{
			CGame::GetInstance()->RemoveState(this);
		}

		if(m_pDI->KeyPressed(DIK_RETURN))
		{
			return this->HandleEnter();
		}

		if(m_pDI->KeyPressed(DIK_UP))
		{
			
				m_nSelection--;
				m_pFM->PlaySound(m_nMenuMove);
				if(m_nSelection < 0)
					m_nSelection = 3;

		}

		if(m_pDI->KeyPressed(DIK_DOWN))
		{
		
				m_nSelection++;
				m_pFM->PlaySound(m_nMenuMove);
				if(m_nSelection > 3)
					m_nSelection = 0;

		}


	}
	return true;
}

void CGameModeSelectionState::Update(float fElapsedTIme)
{
	m_pFM->Update();
}

void CGameModeSelectionState::Render(void)
{
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	m_pTM->Draw(m_nBGImageID, 0,0,2.9f, 1.8f);

	m_pPF->Print("Game Mode Selection", 50,100,1.0f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("Deathmatch",100,200,0.8f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("Time Trial",100,300,0.8f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("Collection",100,400,0.8f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("Tutorial",100,500,0.8f,D3DCOLOR_XRGB(255,0,0));

	switch(m_nSelection)
	{
	case WS_DM:
		m_pPF->Print("Deathmatch",100,200,0.8f,D3DCOLOR_XRGB(0,255,0));
		m_pPF->Print(" --- Fight for a set amount of time in a map of your choosing. The winner", 300,200,0.5f,D3DCOLOR_XRGB(255,255,255));
		m_pPF->Print("	   is determined based on the number of kills made during the game.",300,230,0.5f,D3DCOLOR_XRGB(255,255,255));

		break;
	case WS_TT:
		m_pPF->Print("Time Trial",100,300,0.8f,D3DCOLOR_XRGB(0,255,0));
		m_pPF->Print(" --- Fight for your survival, Kill as many enemies until you die ", 300.0f, 300.0f, 0.5f, D3DCOLOR_XRGB(255,255,255));
		m_pPF->Print("     or until time runs out. You win if you kill at least 20 enemies.", 300.0f, 330.0f, 0.5f, D3DCOLOR_XRGB(255,255,255));

		break;
	case WS_CC:
		m_pPF->Print("Collection",100,400,0.8f,D3DCOLOR_XRGB(0,255,0));
		break;
	case WS_TUT:
		m_pPF->Print("Tutorial",100,500,0.8f,D3DCOLOR_XRGB(0,255,0));
		m_pPF->Print(" --- New to the game? learn the rules and controls of the game in this", 300.0f, 500.0f, 0.5f, D3DCOLOR_XRGB(255,255,255));
		m_pPF->Print("     In-depth Tutorial level.", 300.0f, 530.0f, 0.5f, D3DCOLOR_XRGB(255,255,255));
		break;

	}
}

bool CGameModeSelectionState::HandleEnter(void)
{
	switch(m_nSelection)
	{
	case WS_DM:
		
		CGamePlayState::GetInstance()->SetGameMode(CDeathmatchMode::GetInstance());
		CGamePlayState::GetInstance()->SetTimeTrial(false);
		CGamePlayState::GetInstance()->SetCollectionChallenge(false);
		CGame::GetInstance()->AddState(CLevelSelectionState::GetInstance());
		break;
	case WS_TT:
		CGamePlayState::GetInstance()->SetGameMode(CTimeChallengeMode::GetInstance());
		CGamePlayState::GetInstance()->SetTimeTrial(true);
		CGamePlayState::GetInstance()->SetCollectionChallenge(false);
		CGame::GetInstance()->AddState(CLevelSelectionState::GetInstance());
		
		break;
	case WS_CC:
		CGamePlayState::GetInstance()->SetGameMode(CCollectionMode::GetInstance());
		CGamePlayState::GetInstance()->SetTimeTrial(false);
		CGamePlayState::GetInstance()->SetCollectionChallenge(true);
		CGame::GetInstance()->AddState(CLevelSelectionState::GetInstance());
		
		break;
	case WS_TUT:
		CGamePlayState::GetInstance()->SetGameMode(CTutorialMode::GetInstance());
		CGame::GetInstance()->AddState(CLevelSelectionState::GetInstance());
		
		
		break;
	case WS_EXIT:
		CGame::GetInstance()->RemoveState(this);
		break;

	}
	return true;
}