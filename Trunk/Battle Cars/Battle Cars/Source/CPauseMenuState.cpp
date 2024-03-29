
#include "CPauseMenuState.h"
#include "CMainMenuState.h"
#include "COptionState.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CPrintFont.h"
#include "CXboxInput.h"
#include "CKeyBinds.h"
enum options{RESUME,OPTIONS,QUIT};

CPauseMenuState::CPauseMenuState(void)
{
	m_pTM = NULL;
	m_pPF = NULL;
	m_pDI = NULL;
	m_pFM = NULL;
}

CPauseMenuState* CPauseMenuState::GetInstance(void)
{
	static CPauseMenuState instance;
	return &instance;
}

void CPauseMenuState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();

	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);

	m_pFM = CSGD_FModManager::GetInstance();

	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
}

bool CPauseMenuState::Input(void)
{

	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController->ReadInputState();
		XINPUT_STATE xState = m_pController->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		CKeyBinds* tempkeys = m_pController->GetKB();
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			
		if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			CGame::GetInstance()->ResetInputDelay();
			return HandleEnter();
		}
		else if(xState.Gamepad.wButtons & tempkeys->GetAccept())
		{
			CGame::GetInstance()->ResetInputDelay();
			return HandleEnter();
		}
		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			
		if(x < 8000 && x > -8000 && y > 16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			CGame::GetInstance()->ResetThumbDelay();
			m_nSelection--;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection < 0)
				m_nSelection = 2;
		}
		else if(x < 8000 && x > -8000 && y < -16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			CGame::GetInstance()->ResetThumbDelay();
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection > 2)
				m_nSelection = 0;
		}
		}
	}
	else
	{
	if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
	{
		HandleEnter();

	}
	if(m_pDI->KeyPressed(DIK_UP)||m_pDI->JoystickGetLStickDirDown(DIR_UP))
	{
		m_nSelection--;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection < 0)
			m_nSelection = 2;
	}

	if(m_pDI->KeyPressed(DIK_DOWN)||m_pDI->JoystickGetLStickDirDown(DIR_DOWN))
	{
		m_nSelection++;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection > 2)
			m_nSelection = 0;
	}
	}
	return true;
}

void CPauseMenuState::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CPauseMenuState::Render(void)
{
	m_pPF->PrintCentered("PAUSED",CGame::GetInstance()->GetScreenWidth()/2,50,2.0f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->PrintCentered("RESUME",CGame::GetInstance()->GetScreenWidth()/2,250,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	m_pPF->PrintCentered("OPTIONS",CGame::GetInstance()->GetScreenWidth()/2,300,1.0f,D3DCOLOR_XRGB(200, 0, 0));	
	m_pPF->PrintCentered("QUIT",CGame::GetInstance()->GetScreenWidth()/2,350,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	switch(m_nSelection)
		{
		case RESUME:			
			m_pPF->PrintCentered("RESUME",CGame::GetInstance()->GetScreenWidth()/2,250,1.0f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case OPTIONS:
			m_pPF->PrintCentered("OPTIONS",CGame::GetInstance()->GetScreenWidth()/2,300,1.0f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case QUIT:
			m_pPF->PrintCentered("QUIT",CGame::GetInstance()->GetScreenWidth()/2,350,1.0f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		}

}

void CPauseMenuState::Exit(void)
{
	delete m_pPF;
}

bool CPauseMenuState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nMenuSelect);
		switch(m_nSelection)
		{
		case RESUME:			
			CGame::GetInstance()->RemoveState(this);
			break;
		case OPTIONS:
			CGame::GetInstance()->AddState(COptionState::GetInstance());
			break;
		case QUIT:
			CGame::GetInstance()->RemoveState(this);
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			break;
		}
	return true;
}