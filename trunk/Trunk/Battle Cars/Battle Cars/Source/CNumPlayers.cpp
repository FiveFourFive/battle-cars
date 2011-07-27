#include "CNumPlayers.h"

#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CXboxInput.h"

#include "CGame.h"
#include "CCharacterSelection.h"
#include "CGamePlayState.h"
#include "CTimeChallengeMode.h"
#include "CCollectionMode.h"

CNumPlayers::CNumPlayers()
{
	m_nNumberOfPlayers = 0;

	m_pTM  = NULL;
	m_pPF  = NULL;
	m_pFM  = NULL;
	m_pDI  = NULL;

	m_nBGImageID = -1;
}

CNumPlayers::~CNumPlayers()
{

}

void CNumPlayers::Enter()
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();
	m_pPF = new CPrintFont(m_nFontID);

	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));
	m_nIncorrectSelection = m_pFM->LoadSound("resource/sounds/buzzer2.mp3");
	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/optionstate.jpg");

	m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nIncorrectSelection,CGame::GetInstance()->getSoundAVolume());

	m_nSelection = 0;
	this->m_nNumberOfPlayers = 0;

}

CNumPlayers* CNumPlayers::GetInstance()
{
	static CNumPlayers instance;
	return &instance;
}

void CNumPlayers::Exit()
{
	delete m_pPF;
}

bool CNumPlayers::Input()
{

	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = CGame::GetInstance()->GetController1()->GetState();
//		BYTE rTrig = xState.Gamepad.bRightTrigger;
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				CGame::GetInstance()->ResetInputDelay();
				HandleEnter();
			}
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				CGame::GetInstance()->ResetInputDelay();
				CGame::GetInstance()->RemoveState(this);
			}

		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			
			if(x < 8000 && x > -8000 && y > 16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				CGame::GetInstance()->ResetThumbDelay();
				m_nSelection--;
				if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
							m_nSelection = 2;
					}
					else
					{
						if( m_nSelection < 0 )
							m_nSelection = 2;
					}
				}
				else
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
						if( m_nSelection < 0 )
							m_nSelection = 0;
					}
					else
					{
						if( m_nSelection < 0 )
							m_nSelection = 1;
					}
				}
				m_pFM->PlaySound(m_nMenuMove);

			}
			else if(x < 8000 && x > -8000 && y < -16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				m_nSelection++;
				CGame::GetInstance()->ResetThumbDelay();
				if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
							m_nSelection = 2;
					}
					else
					{
						if( m_nSelection > 2 )
							m_nSelection = 0;
					}
				}
				else
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
						if( m_nSelection > 0 )
							m_nSelection = 0;
					}
					else
					{
						if( m_nSelection > 1 )
							m_nSelection = 0;
					}
				}
				m_pFM->PlaySound(m_nMenuMove);
			}
		}

	}
	else
	{

		if( m_pDI->KeyPressed(DIK_UP)||m_pDI->JoystickGetLStickDirDown(DIR_UP))
		{
			m_nSelection--;

			if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
							m_nSelection = 2;
					}
					else
					{
						if( m_nSelection < 0 )
							m_nSelection = 2;
					}
				}
				else
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
						if( m_nSelection < 0 )
							m_nSelection = 0;
					}
					else
					{
						if( m_nSelection < 0 )
							m_nSelection = 1;
					}
				}
			m_pFM->PlaySound(m_nMenuMove);
		}
		if( m_pDI->KeyPressed(DIK_DOWN)||m_pDI->JoystickGetLStickDirDown(DIR_DOWN))
		{
			m_nSelection++;

			if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
							m_nSelection = 2;
					}
					else
					{
						if( m_nSelection > 2 )
							m_nSelection = 0;
					}
				}
				else
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || CGamePlayState::GetInstance()->GetGameMode() == CCollectionMode::GetInstance())
					{
						if( m_nSelection > 0 )
							m_nSelection = 0;
					}
					else
					{
						if( m_nSelection > 1 )
							m_nSelection = 0;
					}
				}
			m_pFM->PlaySound(m_nMenuMove);
		}

		if( m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
		{
			HandleEnter();
		}
		if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonDown(1))
		{
			CGame::GetInstance()->RemoveState(this);
		}
	}

		return true;
}

void CNumPlayers::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CNumPlayers::Render()
{
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	m_pTM->Draw(m_nBGImageID, 0,0, 1.0f, 1.0f);
	
	m_pPF->PrintCentered("BATTLE CARS",CGame::GetInstance()->GetScreenWidth()/2,50,2.0f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->PrintCentered("MAYHEM AND DESTRUCTION",CGame::GetInstance()->GetScreenWidth()/2,100,2.0f,D3DCOLOR_XRGB(200, 0, 0));

	m_pPF->PrintCentered("1 Player",CGame::GetInstance()->GetScreenWidth()/2,250,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance() || !CGame::GetInstance()->Controller2Connected())
	{
		m_pPF->PrintCentered("2 Player",CGame::GetInstance()->GetScreenWidth()/2,350,1.0f,D3DCOLOR_XRGB(128, 128, 128));	
	}
	else
	{
		m_pPF->PrintCentered("2 Player",CGame::GetInstance()->GetScreenWidth()/2,350,1.0f,D3DCOLOR_XRGB(200, 0, 0));	
	}
	m_pPF->PrintCentered("BEGIN",CGame::GetInstance()->GetScreenWidth()/2,500,1.0f,D3DCOLOR_XRGB(200, 0, 0));


	switch(m_nSelection)
	{
	case 0:			
		m_pPF->PrintCentered("1 Player",CGame::GetInstance()->GetScreenWidth()/2,250,1.0f,D3DCOLOR_XRGB(0, 255, 0));
		break;
	case 1:

		m_pPF->PrintCentered("2 Player",CGame::GetInstance()->GetScreenWidth()/2,350,1.0f,D3DCOLOR_XRGB(0, 255, 0));	
		if(!CGame::GetInstance()->ControllerInput() || !CGame::GetInstance()->Controller2Connected())
		{
			m_pPF->Print(" ---- 2 player requires atleast",CGame::GetInstance()->GetScreenWidth()/2 + 100,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			m_pPF->Print("      player one to use a gamepad.",CGame::GetInstance()->GetScreenWidth()/2 + 100,375,0.5f,D3DCOLOR_XRGB(255,255,255));
		}

		break;
	case 2:
		m_pPF->PrintCentered("BEGIN",CGame::GetInstance()->GetScreenWidth()/2,500,1.0f,D3DCOLOR_XRGB(0, 255, 0));
		break;
	}

	char buffer[32];
	sprintf_s(buffer, "NUMBER OF PLAYERS: %i", m_nNumberOfPlayers);
	m_pPF->PrintCentered(buffer, CGame::GetInstance()->GetScreenWidth()/2, 600, 2.0f, D3DCOLOR_XRGB(0, 255, 128));

	
}

bool CNumPlayers::HandleEnter(void)
{
	switch( m_nSelection)
	{
	case 0:
		m_nNumberOfPlayers = 1;
		m_nSelection = 2;
		break;
	case 1:
		if(CGame::GetInstance()->ControllerInput() || CGame::GetInstance()->Controller2Connected())
		{
		m_nNumberOfPlayers = 2;
		m_nSelection = 2;
		}
		else
			m_pFM->PlaySound(m_nIncorrectSelection);
		break;
	case 2:
		CGame::GetInstance()->AddState(CCharacterSelection::GetInstance());
		break;
	}

	return true;
}