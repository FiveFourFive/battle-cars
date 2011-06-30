#include "CKeyBindsState.h"
#include "CMainMenuState.h"
#include "CGame.h"
#include "CPrintFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CXboxInput.h"
#include "CKeyBinds.h"

enum options{WS_ACCEPT,WS_BACK,WS_SHOOT, WS_CHANGE_WEAPON ,WS_EXIT};
CKeyBindsState::CKeyBindsState(void)
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pPF = NULL;
	m_pDI = NULL;
	m_pFM = NULL;
	m_pController = NULL;
}

CKeyBindsState::~CKeyBindsState(void)
{

}

CKeyBindsState* CKeyBindsState::GetInstance(void)
{
	static CKeyBindsState instance;
	return &instance;
}

void CKeyBindsState::Enter(void)
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nFontID = m_pTM->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);
	m_pFM = CSGD_FModManager::GetInstance();
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/FeelSoNumb.mp3",SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nSoundA = m_pFM->LoadSound("resource/sounds/bullet1.mp3");
	m_nSoundB = CMainMenuState::GetInstance()->GetBackgroundMusicID();
	m_nSelection = 0;
	if(m_pController)
	{
		m_pKB = m_pController->GetKB();
	}
}

bool CKeyBindsState::Input(void)
{
	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = m_pController->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		CKeyBinds* tempkeys = m_pController->GetKB();
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & m_pKB->GetAccept())
			{
				HandleEnter();
			}
		if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			
			switch(m_nSelection)
			{
			case WS_ACCEPT:
				if(m_pKB->GetBack() != XINPUT_GAMEPAD_A)
				m_pKB->SetAccept(XINPUT_GAMEPAD_A);
				break;
			case WS_BACK:
				if(m_pKB->GetAccept() != XINPUT_GAMEPAD_A)
				m_pKB->SetBack(XINPUT_GAMEPAD_A);
				break;
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_A);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_A);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			switch(m_nSelection)
			{
			case WS_ACCEPT:
				if(m_pKB->GetBack() != XINPUT_GAMEPAD_B)
				m_pKB->SetAccept(XINPUT_GAMEPAD_B);
				break;
			case WS_BACK:
				if(m_pKB->GetAccept() != XINPUT_GAMEPAD_B)
				m_pKB->SetBack(XINPUT_GAMEPAD_B);
				break;
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_B);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_B);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			switch(m_nSelection)
			{
			case WS_ACCEPT:
				if(m_pKB->GetBack() != XINPUT_GAMEPAD_Y)
				m_pKB->SetAccept(XINPUT_GAMEPAD_Y);
				break;
			case WS_BACK:
				if(m_pKB->GetAccept() != XINPUT_GAMEPAD_Y)
				m_pKB->SetBack(XINPUT_GAMEPAD_Y);
				break;
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_Y);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_Y);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			switch(m_nSelection)
			{
			case WS_ACCEPT:
				if(m_pKB->GetBack() != XINPUT_GAMEPAD_A)
				m_pKB->SetAccept(XINPUT_GAMEPAD_X);
				break;
			case WS_BACK:
				if(m_pKB->GetAccept() != XINPUT_GAMEPAD_A)
				m_pKB->SetBack(XINPUT_GAMEPAD_X);
				break;
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_X);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_X);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			switch(m_nSelection)
			{
			case WS_ACCEPT:
				if(m_pKB->GetBack() != XINPUT_GAMEPAD_LEFT_SHOULDER)
				m_pKB->SetAccept(XINPUT_GAMEPAD_LEFT_SHOULDER);
				break;
			case WS_BACK:
				if(m_pKB->GetAccept() != XINPUT_GAMEPAD_LEFT_SHOULDER)
				m_pKB->SetBack(XINPUT_GAMEPAD_LEFT_SHOULDER);
				break;
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_LEFT_SHOULDER);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_LEFT_SHOULDER);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			switch(m_nSelection)
			{
			case WS_ACCEPT:
				if(m_pKB->GetBack() != XINPUT_GAMEPAD_RIGHT_SHOULDER)
				m_pKB->SetAccept(XINPUT_GAMEPAD_RIGHT_SHOULDER);
				break;
			case WS_BACK:
				if(m_pKB->GetAccept() != XINPUT_GAMEPAD_RIGHT_SHOULDER)
				m_pKB->SetBack(XINPUT_GAMEPAD_RIGHT_SHOULDER);
				break;
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_RIGHT_SHOULDER);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_RIGHT_SHOULDER);
				break;

			}
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
				m_nSelection = 4;
		}
		else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection > 4)
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
			m_nSelection = 4;
	}

	if(m_pDI->KeyPressed(DIK_DOWN))
	{
		m_nSelection++;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection > 4)
			m_nSelection = 0;
	}
	}
	return true;
}

void CKeyBindsState::Update(float fElapsedTime)
{
	m_pFM->Update();
	m_fDelay += fElapsedTime;
}

void CKeyBindsState::Render(void)
{
	RECT temp;					 ////
	temp.left = 0;				 //////
	temp.top = 0;				 // replace this with background image
	temp.right = 800;			 ////////
	temp.bottom = 600;			 //////
	m_pD3D->GetSprite()->Flush();////
	m_pD3D->DrawRect(temp,0,0,0);//
	
	m_pPF->Print("KEY BINDINGS",220,50,1.0f,D3DCOLOR_XRGB(200, 0, 0));

	char buffer[32];


	m_pPF->Print("INPUT DEVICE",150,120,0.5f,D3DCOLOR_XRGB(200, 0, 0));	
	m_pPF->Print("ACCEPT", 150,200,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("BACK", 150,250,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("SHOOT", 150,300,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("CHANGE WEAPON", 150,350,0.5f,D3DCOLOR_XRGB(255,0,0));
	if(CGame::GetInstance()->ControllerInput())
	{
		m_pPF->Print("GAMEPAD",370,120,0.5f,D3DCOLOR_XRGB(255,255,255));
		switch(m_pKB->GetAccept())
		{
		case XINPUT_GAMEPAD_A:
			m_pPF->Print("A",400,200,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_X:
			m_pPF->Print("X",400,200,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_Y:
			m_pPF->Print("Y",400,200,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_B:
			m_pPF->Print("B",400,200,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:
			m_pPF->Print("LEFT BUMPER",400,200,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER:
			m_pPF->Print("RIGHT BUMPER",400,200,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;

		default:

			break;
		}
		switch(m_pKB->GetBack())
		{
		case XINPUT_GAMEPAD_A:
			m_pPF->Print("A",400,250,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_X:
			m_pPF->Print("X",400,250,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_Y:
			m_pPF->Print("Y",400,250,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_B:
			m_pPF->Print("B",400,250,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:
			m_pPF->Print("LEFT BUMPER",400,250,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER:
			m_pPF->Print("RIGHT BUMPER",400,250,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;

		default:

			break;
		}
		switch(m_pKB->GetShoot())
		{
		case XINPUT_GAMEPAD_A:
			m_pPF->Print("A",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_X:
			m_pPF->Print("X",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_Y:
			m_pPF->Print("Y",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_B:
			m_pPF->Print("B",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:
			m_pPF->Print("LEFT BUMPER",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER:
			m_pPF->Print("RIGHT BUMPER",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;

		default:

			break;
		}
		switch(m_pKB->GetChangeWeapon())
		{
		case XINPUT_GAMEPAD_A:
			m_pPF->Print("A",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_X:
			m_pPF->Print("X",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_Y:
			m_pPF->Print("Y",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_B:
			m_pPF->Print("B",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:
			m_pPF->Print("LEFT BUMPER",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER:
			m_pPF->Print("RIGHT BUMPER",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;

		default:

			break;
		}

	}
	else
	{
		m_pPF->Print("KEYBOARD",370,120,0.5f,D3DCOLOR_XRGB(255,255,255));
	}
	m_pPF->Print("EXIT",150,400,0.5f,D3DCOLOR_XRGB(255, 0, 0));
	switch(m_nSelection)
		{
		case WS_ACCEPT:			
			m_pPF->Print("ACCEPT",150,200,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case WS_BACK:
			m_pPF->Print("BACK",150,250,0.5f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case WS_SHOOT:
			m_pPF->Print("SHOOT",150,300,0.5f,D3DCOLOR_XRGB(0, 255, 0));	
			break;
		case WS_CHANGE_WEAPON:
			m_pPF->Print("CHANGE WEAPON",150,350,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		case WS_EXIT:
			m_pPF->Print("EXIT",150,400,0.5f,D3DCOLOR_XRGB(0, 255, 0));
			break;
		}
}

void CKeyBindsState::Exit(void)
{
	m_pTM->UnloadTexture(m_nFontID);
	delete m_pPF;
}

bool CKeyBindsState::HandleEnter(void)
{
	
		if(m_nSelection == WS_EXIT)
		{
			CGame::GetInstance()->RemoveState(this);
			m_pFM->PlaySound(m_nMenuSelect);
		}
	return true;
}