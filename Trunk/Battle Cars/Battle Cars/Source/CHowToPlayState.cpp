
#include "CHowToPlayState.h"

#include "CMainMenuState.h"
#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CGame.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CXboxInput.h"
#include "CKeyBinds.h"
enum menu {EXIT};

CHowToPlayState::CHowToPlayState(void)
{
	m_pPF = NULL;
	m_pTM = NULL;
	m_pFM = NULL;
	m_pDI = NULL;
	m_pController1 = NULL;
	m_pController2 = NULL;
}

CHowToPlayState::~CHowToPlayState(void)
{

}

CHowToPlayState::CHowToPlayState(const CHowToPlayState&)
{

}

CHowToPlayState& CHowToPlayState::operator=(const CHowToPlayState&)
{
	return *this;
}

CHowToPlayState* CHowToPlayState::GetInstance(void)
{
	static CHowToPlayState instance;
	return &instance;
}
void CHowToPlayState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();

	m_pTM = CSGD_TextureManager::GetInstance();
	
	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));
	m_pPF = new CPrintFont(m_nFontID);

	m_pFM = CSGD_FModManager::GetInstance();
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/optionstate.jpg");

	m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());

}

void CHowToPlayState::Exit(void)
{
	delete m_pPF;
}

bool CHowToPlayState::Input(void)
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
			
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				CGame::GetInstance()->ResetInputDelay();
				return HandleEnter();
			}

		}


		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetThumbDelay();

		}

	}
	else
	{
		if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonPressed(1))
		{
			CGame::GetInstance()->RemoveState(this);
		}
		if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
		{
			return this->HandleEnter();
		}
	}
	return true;
}

void CHowToPlayState::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CHowToPlayState::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	m_pTM->Draw(m_nBGImageID, 0, 0, 1.0f, 1.0f);
	m_pPF->PrintCentered("HOW TO PLAY",CGame::GetInstance()->GetScreenWidth()/2,50,2.0f,D3DCOLOR_XRGB(255, 0, 0));
	m_pPF->Print("Keyboard",250,130,1.5f,D3DCOLOR_XRGB(0,0,255));
	m_pPF->Print("Arrow Keys - MOVEMENT",200,250,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Spacebar - SHOOT",200,300,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Left Control - CHANGE WEAPONS",200,350,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Escape - PAUSE",200,400,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Enter - CONTINUE",200,450,0.7f,D3DCOLOR_XRGB(0, 0, 255));

	m_pPF->Print("Keyboard",850,130,1.5f,D3DCOLOR_XRGB(0,0,255));
	m_pPF->Print("Left Stick - Steering",800,250,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("X - SHOOT",800,300,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Left Bumper - CHANGE WEAPONS",800,350,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Start - PAUSE",800,400,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Right Trigger - Accelerate",800,450,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("Left Trigger - Break/Reverse",800,600,0.7f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->PrintCentered("PRESS ESC OR B (GAMEPAD) TO EXIT",CGame::GetInstance()->GetScreenWidth()/2,800,1.0f,D3DCOLOR_XRGB(0, 255, 0));

}

bool CHowToPlayState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nMenuSelect);
	CGame::GetInstance()->RemoveState(this);

	return true;
}