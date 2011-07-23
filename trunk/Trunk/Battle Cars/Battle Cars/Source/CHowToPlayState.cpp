
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
			CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & tempkeys->GetBack())
			{
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

	RECT temp;					 
	temp.left = 0;				 
	temp.top = 0;				 
	temp.right = 800;			 
	temp.bottom = 600;			 
	pD3D->GetSprite()->Flush();
	pD3D->DrawRect(temp,0,0,0);

	m_pPF->Print("HOW TO PLAY",220,50,1.0f,D3DCOLOR_XRGB(255, 0, 0));
	m_pPF->Print("W A S D - MOVEMENT",100,100,0.5f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("SPACEBAR - SHOOT",100,150,0.5f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("LEFT CONTROL - CHANGE WEAPONS",100,200,0.5f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("P - PAUSE",100,250,0.5f,D3DCOLOR_XRGB(0, 0, 255));
	m_pPF->Print("ENTER - CONTINUE",100,300,0.5f,D3DCOLOR_XRGB(0, 0, 255));

	m_pPF->Print("PRESS ESC OR B (GAMEPAD) TO EXIT",150,500,0.5f,D3DCOLOR_XRGB(0, 255, 0));

}

bool CHowToPlayState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nMenuSelect);
	CGame::GetInstance()->RemoveState(this);

	return true;
}