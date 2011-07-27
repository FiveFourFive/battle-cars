#include "CCharacterSelection.h"

#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CXboxInput.h"
#include "CMainMenuState.h"
#include "CPlayer.h"
#include "CGamePlayState.h"
#include "CLoadingState.h"
#include "CNumPlayers.h"
#include "CGamerProfile.h"
#include "Gamer_Profile.h"

#include "SGD_Math.h"

#include "CGame.h"
#include "tinyxml.h"

CCharacterSelection::CCharacterSelection()
{
	m_pTM  = NULL;
	m_pPF  = NULL;
	m_pFM  = NULL;
	m_pDI  = NULL;

	currPlayer = NULL;
	m_nSelection = 0;

	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();
}

CCharacterSelection::~CCharacterSelection()
{
	/*for( unsigned int i = 0; i < m_vPlayerList.size(); i++)
	{
	if( m_vPlayerList[i])
	{
	delete m_vPlayerList[i];
	m_vPlayerList[i] = NULL;
	}
	}*/
}

void CCharacterSelection::Enter()
{
	m_vPlayerList.clear();

	if( LoadCharacters() == false)
		MessageBox(NULL, "Failed to Load characters", 0, 0);

	for( int i = 0; i < 4; i++)
	{
		isAvailable[i] = true;
	}

	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nIncorrectSelection = m_pFM->LoadSound("resource/sounds/buzzer2.mp3");
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nIncorrectSelection,CGame::GetInstance()->getSoundAVolume());

	m_pPF = new CPrintFont(m_nFontID);

	currPlayer = m_vPlayerList[0];

	m_bPlayer1_turn = true;
	m_bPlayer2_turn = false;
	m_nSelection = 0;

	for(int i = 0; i < 4; i++)
	{
		if(CGamerProfile::GetInstance()->GetActiveProfile()->cars[i] == 0)
			isAvailable[i] = false;
	}

}

void CCharacterSelection::Exit()
{
	m_pFM->UnloadSound(m_nIncorrectSelection);
	m_pTM->UnloadTexture(m_nFontID);
	delete m_pPF;
	//m_pFM->StopSound(CMainMenuState::GetInstance()->GetBackgroundMusicID());
	/*for( unsigned int i = 0; i < m_vPlayerList.size(); i++)
	{
	delete m_vPlayerList[i];
	m_vPlayerList[i] = NULL;
	}*/
}

CCharacterSelection* CCharacterSelection::GetInstance()
{
	static CCharacterSelection instance;
	return &instance;
}

bool CCharacterSelection::Input()
{
	if( !IsPlayer1Selecting() && !IsPlayer2Selecting() )
	{
		m_nSelection = 4;
	}
	if(IsPlayer1Selecting() && !IsPlayer2Selecting() || !IsPlayer1Selecting() && !IsPlayer2Selecting())
	{
		if(CGame::GetInstance()->ControllerInput())
		{
			XINPUT_STATE xState = CGame::GetInstance()->GetController1()->GetState();

			//m_pController1->ReadInputState();

			if( IsPlayer1Selecting() )
				xState = CGame::GetInstance()->GetController1()->GetState();



			float x = xState.Gamepad.sThumbLX;
			float y = xState.Gamepad.sThumbLY;
			if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
			{
				
				if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					CGame::GetInstance()->ResetInputDelay();
					return this->HandleEnter();
				}
				else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
				{
					CGame::GetInstance()->ResetInputDelay();
					for( unsigned int i = 0; i < m_vPlayerList.size(); i++)
					{
						delete m_vPlayerList[i];
						m_vPlayerList[i] = NULL;
					}
					CGame::GetInstance()->RemoveState(this);
				}
			}
			if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
			{
				
				if(x < 8000 && x > -8000 && y > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					if( IsPlayer1Selecting()  )
					{
						CGame::GetInstance()->ResetThumbDelay();
						m_nSelection--;
						m_pFM->PlaySound(m_nMenuMove);
						if(m_nSelection < 0)
							m_nSelection = 3;

						currPlayer = m_vPlayerList[m_nSelection];
					}
				}
				else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					CGame::GetInstance()->ResetThumbDelay();
					if( IsPlayer1Selecting()  )
					{
						m_nSelection++;
						m_pFM->PlaySound(m_nMenuMove);
						if(m_nSelection > 3)
							m_nSelection = 0;

						currPlayer = m_vPlayerList[m_nSelection];
					}
				}
			}
		}
		else
		{
			if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonPressed(1))
			{
				for( unsigned int i = 0; i < m_vPlayerList.size(); i++)
				{
					delete m_vPlayerList[i];
					m_vPlayerList[i] = NULL;
				}
				CGame::GetInstance()->RemoveState(this);
			}

			if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
			{
				return this->HandleEnter();
			}

			if(m_pDI->KeyPressed(DIK_UP)||m_pDI->JoystickGetLStickDirDown(DIR_UP))
			{
				if( IsPlayer1Selecting()  )
				{
					m_nSelection--;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection < 0)
						m_nSelection = 3;

					currPlayer = m_vPlayerList[m_nSelection];
				}
			}

			if(m_pDI->KeyPressed(DIK_DOWN)||m_pDI->JoystickGetLStickDirDown(DIR_DOWN))
			{
				if( IsPlayer1Selecting()  )
				{
					m_nSelection++;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection > 3)
						m_nSelection = 0;

					currPlayer = m_vPlayerList[m_nSelection];
				}
			}


		}
	}
	else
	{
		if(CGame::GetInstance()->Controller2Connected())
		{
			XINPUT_STATE xState = CGame::GetInstance()->GetController2()->GetState();

			//m_pController1->ReadInputState();

			if(IsPlayer2Selecting() )
				xState = CGame::GetInstance()->GetController2()->GetState();



			float x = xState.Gamepad.sThumbLX;
			float y = xState.Gamepad.sThumbLY;
			if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
			{
				
				if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					CGame::GetInstance()->ResetInputDelay();
					return this->HandleEnter();
				}
				else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
				{
					CGame::GetInstance()->ResetInputDelay();
					for( unsigned int i = 0; i < m_vPlayerList.size(); i++)
					{
						delete m_vPlayerList[i];
						m_vPlayerList[i] = NULL;
					}
					CGame::GetInstance()->RemoveState(this);
				}
			}
			if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
			{
				
				if(x < 8000 && x > -8000 && y > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					if(IsPlayer2Selecting() )
					{
						CGame::GetInstance()->ResetThumbDelay();
						m_nSelection--;
						m_pFM->PlaySound(m_nMenuMove);
						if(m_nSelection < 0)
							m_nSelection = 3;

						currPlayer = m_vPlayerList[m_nSelection];
					}
				}
				else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					CGame::GetInstance()->ResetThumbDelay();
					if(IsPlayer2Selecting() )
					{
						m_nSelection++;
						m_pFM->PlaySound(m_nMenuMove);
						if(m_nSelection > 3)
							m_nSelection = 0;

						currPlayer = m_vPlayerList[m_nSelection];
					}
				}
			}
		}
		else
		{
			if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonPressed(1))
			{
				for( unsigned int i = 0; i < m_vPlayerList.size(); i++)
				{
					delete m_vPlayerList[i];
					m_vPlayerList[i] = NULL;
				}
				CGame::GetInstance()->RemoveState(this);
			}

			if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
			{
				return this->HandleEnter();
			}

			if(m_pDI->KeyPressed(DIK_UP))
			{
				if(IsPlayer2Selecting() )
				{
					m_nSelection--;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection < 0)
						m_nSelection = 3;

					currPlayer = m_vPlayerList[m_nSelection];
				}
			}

			if(m_pDI->KeyPressed(DIK_DOWN))
			{
				if(IsPlayer2Selecting() )
				{
					m_nSelection++;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection > 3)
						m_nSelection = 0;

					currPlayer = m_vPlayerList[m_nSelection];
				}
			}


		}
	}
	return true;
}

void CCharacterSelection::Update(float fElapsedTime)
{
	m_pFM->Update();

}

void CCharacterSelection::Render()
{
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	RECT temp_render = {0, 0, CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight()};
	CSGD_Direct3D::GetInstance()->DrawRect( temp_render, 0,0,0);

	m_pPF->PrintCentered("Car Selection",CGame::GetInstance()->GetScreenWidth()/2,50,2.0f,D3DCOLOR_XRGB(255,0,0));

	if( isAvailable[CAR_MINI])
		m_pPF->Print( "Mighty Mini", 10, 250, 1.0f, D3DCOLOR_XRGB(200, 0, 0));
	else
		m_pPF->Print( "Mighty Mini", 10, 250, 1.0f, D3DCOLOR_XRGB(128, 128, 128));

	if( isAvailable[CAR_VETTE])
		m_pPF->Print( "Flaming Fury", 10, 300, 1.0f, D3DCOLOR_XRGB(200, 0, 0));
	else
		m_pPF->Print( "Flaming Fury", 10, 300, 1.0f, D3DCOLOR_XRGB(128, 128, 128));

	if( isAvailable[CAR_HUMMER])
		m_pPF->Print( "The Hammer", 10, 350, 1.0f, D3DCOLOR_XRGB(200, 0, 0));
	else
		m_pPF->Print( "The Hammer", 10, 350, 1.0f, D3DCOLOR_XRGB(128, 128, 128));

	if( isAvailable[CAR_TRUCK])
		m_pPF->Print( "Redrucker", 10, 400, 1.0f, D3DCOLOR_XRGB(200, 0, 0));
	else
		m_pPF->Print( "Redrucker", 10, 400, 1.0f, D3DCOLOR_XRGB(128, 128, 128));


	m_pPF->Print( "START", 10,450,1.0f,D3DCOLOR_XRGB(200,0,0));

	switch( m_nSelection )
	{
	case CAR_MINI:
		{
			m_pPF->Print( "Mighty Mini", 10, 250, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
			m_pPF->Print( "Special: Shotgun of mini rockets", 275, 675, 0.9f, D3DCOLOR_XRGB(103,197,166));
		}
		break;
	case CAR_VETTE:
		{
			m_pPF->Print( "Flaming FUry", 10, 300, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
			m_pPF->Print( "Special: Flame thrower", 275, 675, 0.9f, D3DCOLOR_XRGB(103,197,166));
		}
		break;
	case CAR_HUMMER:
		{
			m_pPF->Print( "The Hammer", 10, 350, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
			m_pPF->Print( "Special: Gatling gun", 275, 675, 0.9f, D3DCOLOR_XRGB(103,197,166));
		}
		break;
	case CAR_TRUCK:
		{
			m_pPF->Print( "Redrucker", 10, 400, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
			m_pPF->Print( "Special: Land mines", 275, 675, 0.9f, D3DCOLOR_XRGB(103,197,166));
		}
		break;
	case CAR_NUM:
		{
			m_pPF->Print( "START", 10,450,1.0f,D3DCOLOR_XRGB(0,255,0));
		}
		break;
	}

	m_pTM->Draw(currPlayer->GetImageID(), (CGame::GetInstance()->GetScreenWidth()/4)*3, 300, 1.0f, 1.0f, &currPlayer->GetImageRect());

	RECT render_health_rect;
	render_health_rect.left = 650;
	render_health_rect.top = 250;
	int width = (int)Lerp(0.0f, 100.0f, ((currPlayer->GetHealth() + currPlayer->GetArmor())/ 180.0f));
	render_health_rect.right = render_health_rect.left + width;
	render_health_rect.bottom = render_health_rect.top + 40;

	m_pPF->Print("Health", 450, 250, 1.0f, D3DCOLOR_XRGB(200,0,0));
	CSGD_Direct3D::GetInstance()->DrawRect(render_health_rect, 128,128,0);
	char buffer[32];
	sprintf_s(buffer, "%.0f", (currPlayer->GetHealth() + currPlayer->GetArmor()));
	m_pPF->Print(buffer, 650, 260, 1.0f, D3DCOLOR_XRGB(200,0,0));

	RECT render_speed_rect;
	render_speed_rect.left = 650;
	render_speed_rect.top = 325;
	width = (int)Lerp(0.0f, 100.0f,(currPlayer->GetMaxSpeed() / 300.0f));
	render_speed_rect.right = render_speed_rect.left + width;
	render_speed_rect.bottom = render_speed_rect.top + 40;

	m_pPF->Print("Speed", 450, 325, 1.0f, D3DCOLOR_XRGB(200,0,0));
	CSGD_Direct3D::GetInstance()->DrawRect(render_speed_rect, 128,128,0);
	sprintf_s(buffer, "%.0f", currPlayer->GetMaxSpeed());
	m_pPF->Print(buffer, 650, 325, 1.0f, D3DCOLOR_XRGB(200,0,0));

	RECT render_acceleration_rect;
	render_acceleration_rect.left = 650;
	render_acceleration_rect.top = 400;
	width = (int)Lerp(0.0f, 100.0f, (currPlayer->GetAcceleration() / 560.0f));
	render_acceleration_rect.right = render_acceleration_rect.left + width;
	render_acceleration_rect.bottom = render_acceleration_rect.top + 40;

	m_pPF->Print("Accel.", 450, 400, 1.0f, D3DCOLOR_XRGB(200,0,0));
	CSGD_Direct3D::GetInstance()->DrawRect(render_acceleration_rect, 128,128,0);
	sprintf_s(buffer, "%.0f", currPlayer->GetAcceleration());
	m_pPF->Print(buffer, 650, 410, 1.0f, D3DCOLOR_XRGB(200,0,0));

	RECT render_agility_rect;
	render_agility_rect.left = 650;
	render_agility_rect.top = 475;
	width = (int)Lerp(0.0f, 100.0f, (currPlayer->GetRotationRate() / 3.14f));
	render_agility_rect.right = render_agility_rect.left + width;
	render_agility_rect.bottom = render_agility_rect.top + 40;

	m_pPF->Print("AGILITY", 450, 475, 1.0f, D3DCOLOR_XRGB(200,0,0));
	CSGD_Direct3D::GetInstance()->DrawRect(render_agility_rect, 128,128,0);
	sprintf_s(buffer, "%.2f", currPlayer->GetRotationRate());
	m_pPF->Print(buffer, 650, 485, 1.0f, D3DCOLOR_XRGB(200,0,0));


	if( IsPlayer1Selecting() )
	{
		m_pPF->Print("Player 1 is choosing", 250, 750, 1.0f, D3DCOLOR_XRGB(0,0,255));
	}
	else if( IsPlayer2Selecting() )
	{
		m_pPF->Print("Player 2 is choosing", 250, 750, 1.0f, D3DCOLOR_XRGB(0,0,255));
	}
	else
	{
		m_pPF->Print("SELECT BEGIN TO START THE GAME", 250, 750, 1.0f, D3DCOLOR_XRGB(0,0,255));
	}
}

bool CCharacterSelection::HandleEnter()
{
	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		if( !IsPlayer1Selecting() && IsPlayer2Selecting())
		{
			switch( m_nSelection )
			{
			case CAR_MINI:
				{
					if( isAvailable[CAR_MINI] )
					{
						m_player2 = m_vPlayerList[CAR_MINI];
						m_bPlayer2_turn = false;
						isAvailable[CAR_MINI] = false;
						m_player2->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/rocket_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_VETTE:
				{
					if( isAvailable[CAR_VETTE] )
					{
						m_player2 = m_vPlayerList[CAR_VETTE];
						m_bPlayer2_turn = false;
						isAvailable[CAR_VETTE] = false;
						m_player2->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/flamerthrower_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_HUMMER:
				{
					if( isAvailable[CAR_HUMMER] )
					{
						m_player2 = m_vPlayerList[CAR_HUMMER];
						m_bPlayer2_turn = false;
						isAvailable[CAR_HUMMER] = false;
						m_player2->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/minigun_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_TRUCK:
				{
					if( isAvailable[CAR_TRUCK] )
					{
						m_player2 = m_vPlayerList[CAR_TRUCK];
						m_bPlayer2_turn = false;
						isAvailable[CAR_TRUCK] = false;
						m_player2->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/mine_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			}
		}

		if( IsPlayer1Selecting() && !IsPlayer2Selecting())
		{
			switch( m_nSelection )
			{
			case CAR_MINI:
				{
					if( isAvailable[CAR_MINI] )
					{
						m_player1 = m_vPlayerList[CAR_MINI];
						m_bPlayer1_turn = false;
						m_bPlayer2_turn = true;
						isAvailable[CAR_MINI] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/rocket_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_VETTE:
				{
					if( isAvailable[CAR_VETTE] )
					{
						m_player1 = m_vPlayerList[CAR_VETTE];
						m_bPlayer1_turn = false;
						m_bPlayer2_turn = true;
						isAvailable[CAR_VETTE] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/flamerthrower_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_HUMMER:
				{
					if( isAvailable[CAR_HUMMER] )
					{
						m_player1 = m_vPlayerList[CAR_HUMMER];
						m_bPlayer1_turn = false;
						m_bPlayer2_turn = true;
						isAvailable[CAR_HUMMER] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/minigun_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_TRUCK:
				{
					if( isAvailable[CAR_TRUCK] )
					{
						m_player1 = m_vPlayerList[CAR_TRUCK];
						m_bPlayer1_turn = false;
						m_bPlayer2_turn = true;
						isAvailable[CAR_TRUCK] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/mine_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			}
		}


		if( !IsPlayer1Selecting() && !IsPlayer2Selecting())
		{
			if( m_nSelection == CAR_NUM)
			{
				//CGame::GetInstance()->ClearStates(CGamePlayState::GetInstance());
				//CGame::GetInstance()->AddState(CLoadingState::GetInstance());
				CGame::GetInstance()->ChangeState(CLoadingState::GetInstance());
			}
		}
	}
	else
	{
		if( IsPlayer1Selecting())
		{
			switch( m_nSelection )
			{
			case CAR_MINI:
				{
					if( isAvailable[CAR_MINI] )
					{
						m_player1 = m_vPlayerList[CAR_MINI];
						m_bPlayer1_turn = false;
						isAvailable[CAR_MINI] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/rocket_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_VETTE:
				{
					if( isAvailable[CAR_VETTE] )
					{
						m_player1 = m_vPlayerList[CAR_VETTE];
						m_bPlayer1_turn = false;
						isAvailable[CAR_VETTE] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/flamerthrower_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_HUMMER:
				{
					if( isAvailable[CAR_HUMMER] )
					{
						m_player1 = m_vPlayerList[CAR_HUMMER];
						m_bPlayer1_turn = false;
						isAvailable[CAR_HUMMER] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/minigun_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			case CAR_TRUCK:
				{
					if( isAvailable[CAR_TRUCK] )
					{
						m_player1 = m_vPlayerList[CAR_TRUCK];
						m_bPlayer1_turn = false;
						isAvailable[CAR_TRUCK] = false;
						m_player1->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/mine_icon.png"));
					}
					else
					{
						if(!m_pFM->GetInstance()->IsSoundPlaying(m_nIncorrectSelection))
						m_pFM->PlaySoundA(m_nIncorrectSelection);
					}
				}
				break;
			}
		}

		if( !IsPlayer1Selecting())
		{
			if( m_nSelection == CAR_NUM)
			{
				//CGame::GetInstance()->ClearStates(CGamePlayState::GetInstance());
				//CGame::GetInstance()->AddState(CLoadingState::GetInstance());
				CGame::GetInstance()->ChangeState(CLoadingState::GetInstance());
			}
		}
	}


	return true;
}

bool CCharacterSelection::LoadCharacters()
{
	TiXmlDocument doc;
	int counter = 0;
	if(doc.LoadFile("resource/data/characters.xml") == false)
		return false;
	TiXmlElement* pRoot = doc.RootElement();
	if(!pRoot)
		return false;



	TiXmlElement* pCharacterRoot = pRoot->FirstChildElement("character");
	while(pCharacterRoot)
	{
		CPlayer* character = new CPlayer(CGame::GetInstance()->GetController1());
		//character->AddWeaponIcon(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/HUDS/Weapon_Icons/rocket_icon.png"));
		int armor, accel, maxspeed;
		double rate;
		float rotate;
		RECT temp_image_rect;
		RECT health_rect;

		if(pCharacterRoot->Attribute("armor", &armor))
			character->SetArmor((float)armor);
		if(pCharacterRoot->Attribute("accel", &accel))
			character->SetAcceleration((float)accel);
		if(pCharacterRoot->Attribute("maxspeed", &maxspeed))
			character->SetMaxSpeed((float)maxspeed);
		if(pCharacterRoot->Attribute("rate", &rate))
		{
			rotate = 3.14f-(float)rate;
			character->SetRotationRate(rotate);
		}

		pCharacterRoot->Attribute("left", (int*)(&temp_image_rect.left));
		pCharacterRoot->Attribute("top", (int*)(&temp_image_rect.top));
		pCharacterRoot->Attribute("right", (int*)(&temp_image_rect.right));
		pCharacterRoot->Attribute("bottom", (int*)(&temp_image_rect.bottom));

		pCharacterRoot->Attribute("h1_left", (int*)(&health_rect.left));
		pCharacterRoot->Attribute("h1_top", (int*)(&health_rect.top));
		pCharacterRoot->Attribute("h1_right", (int*)(&health_rect.right));
		pCharacterRoot->Attribute("h1_bottom", (int*)(&health_rect.bottom));

		character->SetHealthImageRect(&health_rect,0);

		pCharacterRoot->Attribute("h2_left", (int*)(&health_rect.left));
		pCharacterRoot->Attribute("h2_top", (int*)(&health_rect.top));
		pCharacterRoot->Attribute("h2_right", (int*)(&health_rect.right));
		pCharacterRoot->Attribute("h2_bottom", (int*)(&health_rect.bottom));

		character->SetHealthImageRect(&health_rect, 1);

		pCharacterRoot->Attribute("h3_left", (int*)(&health_rect.left));
		pCharacterRoot->Attribute("h3_top", (int*)(&health_rect.top));
		pCharacterRoot->Attribute("h3_right", (int*)(&health_rect.right));
		pCharacterRoot->Attribute("h3_bottom", (int*)(&health_rect.bottom));

		character->SetHealthImageRect(&health_rect, 2);

		pCharacterRoot->Attribute("h4_left", (int*)(&health_rect.left));
		pCharacterRoot->Attribute("h4_top", (int*)(&health_rect.top));
		pCharacterRoot->Attribute("h4_right", (int*)(&health_rect.right));
		pCharacterRoot->Attribute("h4_bottom", (int*)(&health_rect.bottom));

		character->SetHealthImageRect(&health_rect, 3);

		character->SetImageRect(&temp_image_rect);

		const char* buffer = pCharacterRoot->GetText();

		character->SetHealth(100);
		character->SetMaxHealth(100);
		character->SetPowerUpBar(0);
		character->SetShieldBar(100);
		character->SetMaxPowerUp(100);
		character->SetMaxShield(100);
		character->SetVelX(0);
		character->SetVelY(0);
		tVector2D temp = {0,0};
		character->SetSpeed(0);
		character->SetType(OBJECT_PLAYER);
		character->SetPosX(350);
		character->SetPosY(225);
		character->SetPlayerType(counter);
		string temp_buffer = "resource/graphics/";
		temp_buffer += buffer;
		character->SetImageID(m_pTM->LoadTexture(temp_buffer.c_str()));
		character->SetCarId(m_pTM->LoadTexture("resource/graphics/sbs_sprites_cars.png"));

		counter++;
		m_vPlayerList.push_back(character);
		pCharacterRoot = pCharacterRoot->NextSiblingElement("character");
	}
	return true;
}