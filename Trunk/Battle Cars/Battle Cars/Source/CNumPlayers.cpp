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

	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/mainmenu_bg.jpg");
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
			CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				HandleEnter();
			}
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				CGame::GetInstance()->RemoveState(this);
			}

		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetThumbDelay();
			if(x < 8000 && x > -8000 && y > 16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				m_nSelection--;
				if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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

				if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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

		if( m_pDI->KeyPressed(DIK_UP))
		{
			m_nSelection--;

			if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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
		if( m_pDI->KeyPressed(DIK_DOWN))
		{
			m_nSelection++;

			if( m_nNumberOfPlayers >= 1)
				{
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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
					if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
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

		if( m_pDI->KeyPressed(DIK_RETURN))
		{
			HandleEnter();
		}
		if(m_pDI->KeyPressed(DIK_ESCAPE))
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

	m_pTM->Draw(m_nBGImageID, 0,0, 2.9f, 1.8f);
	
	m_pPF->Print("BATTLE CARS",220,50,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->Print("MAYHEM AND DESTRUCTION",50,100,1.0f,D3DCOLOR_XRGB(200, 0, 0));

	m_pPF->Print("1 PLAYER",300,200,0.5f,D3DCOLOR_XRGB(200, 0, 0));
	if( CGamePlayState::GetInstance()->GetGameMode() == CTimeChallengeMode::GetInstance())
	{
		m_pPF->Print("2 PLAYER",300,250,0.5f,D3DCOLOR_XRGB(128, 128, 128));	
	}
	else
	{
		m_pPF->Print("2 PLAYER",300,250,0.5f,D3DCOLOR_XRGB(200, 0, 0));	
	}
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
		m_nNumberOfPlayers = 2;
		m_nSelection = 2;
		break;
	case 2:
		CGame::GetInstance()->ChangeState(CCharacterSelection::GetInstance());
		break;
	}

	return true;
}