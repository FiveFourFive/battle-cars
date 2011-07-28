
#include "COptionState.h"
#include "CMainMenuState.h"
#include "CGame.h"
#include "CPrintFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CGamePlayState.h"
#include "CXboxInput.h"
#include "CKeyBinds.h"
#include "CKeyBindsState.h"
#include "CPlayer.h"
#include "CCamera.h"
enum options{WS_EFFECTS,WS_MUSIC,WS_INPUT, WS_CAMERAVIEW ,WS_KEYBINDS,WS_EXIT};

COptionState::COptionState(void)
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pPF = NULL;
	m_pDI = NULL;
	m_pFM = NULL;

	m_bVertical = true;
	m_nBGImageID = -1;
}

COptionState::~COptionState()
{

}

COptionState* COptionState::GetInstance(void)
{
	static COptionState instance;
	return &instance;
}

void COptionState::Enter(void)
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);
	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();
	m_pFM = CSGD_FModManager::GetInstance();
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/FeelSoNumb.mp3",SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nSoundA = m_pFM->LoadSound("resource/sounds/bullet1.mp3");
	m_nSoundB = CMainMenuState::GetInstance()->GetBackgroundMusicID();

	m_pFM->SetVolume(m_nBackgroundMusicID,CGame::GetInstance()->getSoundBVolume());
	m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/optionstate.jpg");
	
	//m_pFM->PlaySound(m_nBackgroundMusicID);

}

bool COptionState::Input(void)
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
			
		if(xState.Gamepad.wButtons & tempkeys->GetAccept())
		{
			CGame::GetInstance()->ResetInputDelay();
			return this->HandleEnter();
		}
		else if(xState.Gamepad.wButtons & tempkeys->GetBack())
		{
			CGame::GetInstance()->ResetInputDelay();
			CGame::GetInstance()->RemoveState(this);
		}
		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			
		if(x < 8000 && x > -8000 && y > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			CGame::GetInstance()->ResetThumbDelay();
			m_nSelection--;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection < 0)
				m_nSelection = 5;
		}
		else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			CGame::GetInstance()->ResetThumbDelay();
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection > 5)
				m_nSelection = 0;
		}
		}
	if(m_fDelay > 0.20f)
	{
		
	if(x < -16000)
	{
		CGame::GetInstance()->ResetThumbDelay();
		m_fDelay = 0.0f;
		switch(m_nSelection)
		{
		case WS_EFFECTS:
			{
				float soundA = CGame::GetInstance()->getSoundAVolume();
				if(soundA > 0.10f)
				{
					CGame::GetInstance()->SetSoundAVolume(soundA -0.1f);
					m_pFM->SetVolume(m_nSoundA,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
					m_pFM->PlaySound(m_nSoundA);
				}
				break;
			}
		case WS_MUSIC:
			{
				float soundB = CGame::GetInstance()->getSoundBVolume();
				if(soundB > 0.10f)
				{
					CGame::GetInstance()->SetSoundBVolume(soundB -0.10f);
					m_pFM->SetVolume(m_nSoundB,CGame::GetInstance()->getSoundBVolume());
					if(CGamePlayState::GetInstance()->GetBackgroundMusicID() != -1)
					{
						m_pFM->SetVolume(CGamePlayState::GetInstance()->GetBackgroundMusicID(),CGame::GetInstance()->getSoundBVolume());
					}
				}
				break;
			}
		case WS_INPUT:
			{
				CGame::GetInstance()->SetControllerInput(!CGame::GetInstance()->ControllerInput());
				break;
			}
		case WS_CAMERAVIEW:
			{
				m_bVertical = !m_bVertical;
				break;
			}
		}
	}
	if(x > 16000)
	{
		CGame::GetInstance()->ResetThumbDelay();
		m_fDelay = 0.0f;
		switch(m_nSelection)
		{
		case WS_EFFECTS:
			{
				float soundA = CGame::GetInstance()->getSoundAVolume();
				if(soundA < 1.0f)
				{
					CGame::GetInstance()->SetSoundAVolume(soundA + 0.1f);
					m_pFM->SetVolume(m_nSoundA,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
					m_pFM->PlaySound(m_nSoundA);
				}
				break;
			}
		case WS_MUSIC:
			{
				float soundB = CGame::GetInstance()->getSoundBVolume();
				if(soundB < 1.0f)
				{
					CGame::GetInstance()->SetSoundBVolume(soundB +0.10f);
					m_pFM->SetVolume(m_nSoundB,CGame::GetInstance()->getSoundBVolume());
					if(CGamePlayState::GetInstance()->GetBackgroundMusicID() != -1)
					{
						m_pFM->SetVolume(CGamePlayState::GetInstance()->GetBackgroundMusicID(),CGame::GetInstance()->getSoundBVolume());
					}
				}
				break;
			}
			case WS_INPUT:
			{
				CGame::GetInstance()->SetControllerInput(!CGame::GetInstance()->ControllerInput());
				break;
			}
			case WS_CAMERAVIEW:
				{
					m_bVertical = !m_bVertical;
					break;
				}
		}
	}
	}
	}
	else
	{

	if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonDown(1))
	{
		CGame::GetInstance()->RemoveState(this);
	}
	if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
	{
		return this->HandleEnter();
	}
	if(m_pDI->KeyPressed(DIK_UP)||m_pDI->JoystickGetLStickDirPressed(DIR_UP))
	{
		m_nSelection--;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection < 0)
			m_nSelection = 5;
	}

	if(m_pDI->KeyPressed(DIK_DOWN)||m_pDI->JoystickGetLStickDirPressed(DIR_DOWN))
	{
		m_nSelection++;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection > 5)
			m_nSelection = 0;
	}
	if(m_fDelay > 0.20f)
	{
		
	if(m_pDI->KeyDown(DIK_LEFT)||m_pDI->JoystickGetLStickDirPressed(DIR_LEFT))
	{
		m_fDelay = 0.0f;
		switch(m_nSelection)
		{
		case WS_EFFECTS:
			{
				float soundA = CGame::GetInstance()->getSoundAVolume();
				if(soundA > 0.10f)
				{
					CGame::GetInstance()->SetSoundAVolume(soundA -0.1f);
					m_pFM->SetVolume(m_nSoundA,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
					m_pFM->PlaySound(m_nSoundA);
				}
				break;
			}
		case WS_MUSIC:
			{
				float soundB = CGame::GetInstance()->getSoundBVolume();
				if(soundB > 0.10f)
				{
					CGame::GetInstance()->SetSoundBVolume(soundB -0.10f);
					m_pFM->SetVolume(m_nSoundB,CGame::GetInstance()->getSoundBVolume());
					if(CGamePlayState::GetInstance()->GetBackgroundMusicID() != -1)
					{
						m_pFM->SetVolume(CGamePlayState::GetInstance()->GetBackgroundMusicID(),CGame::GetInstance()->getSoundBVolume());
					}
				}
				break;
			}
		case WS_INPUT:
			{
				bool input = CGame::GetInstance()->ControllerInput();
				if(input)
				{
					CGame::GetInstance()->SetControllerInput(!input);
				}
				else
				{
					if(CGame::GetInstance()->GetController1()->Connected())
						CGame::GetInstance()->SetControllerInput(true);
				}
				break;
			}
		case WS_CAMERAVIEW:
				{
					m_bVertical = !m_bVertical;
					break;
				}
		}
	}
	if(m_pDI->KeyDown(DIK_RIGHT)||m_pDI->JoystickGetLStickDirPressed(DIR_RIGHT))
	{
		m_fDelay = 0.0f;
		switch(m_nSelection)
		{
		case WS_EFFECTS:
			{
				float soundA = CGame::GetInstance()->getSoundAVolume();
				if(soundA < 1.0f)
				{
					CGame::GetInstance()->SetSoundAVolume(soundA + 0.1f);
					m_pFM->SetVolume(m_nSoundA,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
					m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
					m_pFM->PlaySound(m_nSoundA);
				}
				break;
			}
		case WS_MUSIC:
			{
				float soundB = CGame::GetInstance()->getSoundBVolume();
				if(soundB < 1.0f)
				{
					CGame::GetInstance()->SetSoundBVolume(soundB +0.10f);
					m_pFM->SetVolume(m_nSoundB,CGame::GetInstance()->getSoundBVolume());
					if(CGamePlayState::GetInstance()->GetBackgroundMusicID() != -1)
					{
						m_pFM->SetVolume(CGamePlayState::GetInstance()->GetBackgroundMusicID(),CGame::GetInstance()->getSoundBVolume());
					}
				}
				break;
			}
			case WS_INPUT:
			{
				bool input = CGame::GetInstance()->ControllerInput();
				if(input)
				{
					CGame::GetInstance()->SetControllerInput(!input);
				}
				else
				{
					if(CGame::GetInstance()->GetController1()->Connected())
						CGame::GetInstance()->SetControllerInput(true);
				}
				break;
			}
			case WS_CAMERAVIEW:
				{
					m_bVertical = !m_bVertical;
					break;
				}
		}
	}
	}


	}
	return true;
}

void COptionState::Update(float fElapsedTime)
{
	m_pFM->Update();
	m_fDelay += fElapsedTime;
}

void COptionState::Render(void)
{								 
	m_pTM->Draw(m_nBGImageID, 0, 0, 1.0f, 1.0f);
	
	m_pPF->PrintCentered("OPTIONS",CGame::GetInstance()->GetScreenWidth()/2,50,2.0f,D3DCOLOR_XRGB(200, 0, 0));

	char buffer[32];
	int tempvol;
	m_pPF->Print("SFX",CGame::GetInstance()->GetScreenWidth()/5 * 2,250,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	tempvol = (int)(CGame::GetInstance()->getSoundAVolume() * 100);
	sprintf_s(buffer,"%i",tempvol);
	m_pPF->Print(buffer,CGame::GetInstance()->GetScreenWidth()/6 * 4 ,250,1.0f,D3DCOLOR_XRGB(255,255,255));
	
	m_pPF->Print("MUSIC",CGame::GetInstance()->GetScreenWidth()/5 * 2,300,1.0f,D3DCOLOR_XRGB(200, 0, 0));	
	tempvol = (int)(CGame::GetInstance()->getSoundBVolume() * 100);
	sprintf_s(buffer,"%i",tempvol);
	m_pPF->Print(buffer,CGame::GetInstance()->GetScreenWidth()/6 * 4 ,300,1.0f,D3DCOLOR_XRGB(255,255,255));

	m_pPF->Print("INPUT DEVICE",CGame::GetInstance()->GetScreenWidth()/5 * 2,350,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	if(CGame::GetInstance()->ControllerInput())
	{
		m_pPF->Print("GAMEPAD",CGame::GetInstance()->GetScreenWidth()/6 * 4 ,350,1.0f,D3DCOLOR_XRGB(255,255,255));
	}
	else
	{
		m_pPF->Print("KEYBOARD",CGame::GetInstance()->GetScreenWidth()/6 * 4 ,350,1.0f,D3DCOLOR_XRGB(255,255,255));
	}

	m_pPF->Print("CAMERA SPLIT", CGame::GetInstance()->GetScreenWidth()/5 * 2, 400, 1.0f, D3DCOLOR_XRGB(200, 0, 0 ));
	if( m_bVertical )
	{
		m_pPF->Print("VERTICAL",CGame::GetInstance()->GetScreenWidth()/6 * 4 ,400,1.0f,D3DCOLOR_XRGB(255,255,255));
	}
	else
	{
		m_pPF->Print("HORIZONTAL",CGame::GetInstance()->GetScreenWidth()/6 * 4 ,400,1.0f,D3DCOLOR_XRGB(255,255,255));
	}
	m_pPF->Print("EDIT KEY BINDINGS",CGame::GetInstance()->GetScreenWidth()/5 * 2,450,1.0f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("EXIT",CGame::GetInstance()->GetScreenWidth()/5 * 2,500,1.0f,D3DCOLOR_XRGB(200, 0, 0));

	switch(m_nSelection)
		{
		case WS_EFFECTS:			
			m_pPF->Print("SFX",CGame::GetInstance()->GetScreenWidth()/5 * 2,250,1.0f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case WS_MUSIC:
			m_pPF->Print("MUSIC",CGame::GetInstance()->GetScreenWidth()/5 * 2,300,1.0f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case WS_INPUT:
			m_pPF->Print("INPUT DEVICE",CGame::GetInstance()->GetScreenWidth()/5 * 2,350,1.0f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case WS_CAMERAVIEW:
			m_pPF->Print("CAMERA SPLIT",CGame::GetInstance()->GetScreenWidth()/5 * 2,400,1.0f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case WS_KEYBINDS:
			m_pPF->Print("EDIT KEY BINDINGS",CGame::GetInstance()->GetScreenWidth()/5 * 2,450,1.0f,D3DCOLOR_XRGB(0,255,0));
			break;
		case WS_EXIT:
			m_pPF->Print("EXIT",CGame::GetInstance()->GetScreenWidth()/5 * 2,500,1.0f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		}
}

void COptionState::Exit(void)
{
	//m_pFM->StopSound(m_nBackgroundMusicID);
	m_pTM->UnloadTexture(m_nFontID);
	CGamePlayState::GetInstance()->Setvolume();
	delete m_pPF;
}

bool COptionState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nMenuSelect);
		if(m_nSelection == WS_EXIT)
		{
			CGame::GetInstance()->RemoveState(this);
		}
		if(m_nSelection == WS_KEYBINDS)
		{
			CKeyBindsState::GetInstance()->SetController(m_pController1);
			CGame::GetInstance()->AddState(CKeyBindsState::GetInstance());
			
		}
	return true;
}