
#include "CLossState.h"
#include "CMainMenuState.h"
#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CGame.h"
#include "CSGD_Direct3D.h"
#include "CGamePlayState.h"
#include "CGamerProfile.h"
#include "CXboxInput.h"
#include "CKeyBinds.h"
#include "CNumPlayers.h"
#include "CCar.h"
#include "CPlayer.h"
#include <vector>

CLossState::CLossState(void)
{
	m_pPF = NULL;
	m_pTM = NULL;
	m_pFM = NULL;
	m_pDI = NULL;
	m_pController1 = NULL;
	m_pController2 = NULL;
}

CLossState::~CLossState(void)
{

}

CLossState::CLossState(const CLossState&)
{

}

CLossState& CLossState::operator=(const CLossState&)
{
	return *this;
}

CLossState* CLossState::GetInstance(void)
{
	static CLossState instance;
	return &instance;
}

void CLossState::Enter(void)
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
	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/otherstates.jpg");
}
void CLossState::Exit(void)
{

	delete m_pPF;
}

bool CLossState::Input(void)
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
			if(xState.Gamepad.wButtons & tempkeys->GetAccept())
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
		if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonDown(1))
		{
			return this->HandleEnter();
		}
		if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonDown(0))
		{
			return this->HandleEnter();
		}
	}
	return true;
}

void CLossState::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CLossState::Render(void)
{
	m_pTM->Draw(m_nBGImageID, 0, 0, 1.5f, 1.0f);
	m_pPF->Print("GAME OVER",220,50,1.0f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("FINAL SCORE: ", 100,150,1.0f,D3DCOLOR_XRGB(0,0,255));

	char buffer[128];
	int playerscore = CGame::GetInstance()->GetScore();
	sprintf_s(buffer,"%i", playerscore);

	m_pPF->Print(buffer,500,150,1.0f,D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("PRESS ENTER/BACK TO CONTINUE",60,430,1.0f,D3DCOLOR_XRGB(255,255,255));

}

bool CLossState::HandleEnter(void)
{
	m_pFM->PlaySound(m_nMenuSelect);
	CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	return true;
}