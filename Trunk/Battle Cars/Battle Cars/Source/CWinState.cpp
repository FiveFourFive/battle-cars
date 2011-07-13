
#include "CWinState.h"
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


CWinState::CWinState(void)
{
	m_pPF = NULL;
	m_pTM = NULL;
	m_pFM = NULL;
	m_pDI = NULL;
	m_pController1 = NULL;
	m_pController2 = NULL;

	m_fSlotTime = 0.0f;
	m_fSlotDelay = 0.0f;
	m_nNumSlots = 4;
	m_nSlot = 0;
	m_fTotalTurns = rand() % 100 + 100;
	m_fDelay = 0.002;
}

CWinState::~CWinState(void)
{

}

CWinState::CWinState(const CWinState&)
{

}

CWinState& CWinState::operator=(const CWinState&)
{
	return *this;
}

CWinState* CWinState::GetInstance(void)
{
	static CWinState instance;
	return &instance;
}

void CWinState::Enter(void)
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nFontID = m_pTM->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);
	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();
	m_pFM = CSGD_FModManager::GetInstance();
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/FeelSoNumb.mp3",SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
}
void CWinState::Exit(void)
{

	delete m_pPF;
}

bool CWinState::Input(void)
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
		if(m_pDI->KeyPressed(DIK_ESCAPE))
		{
			return this->HandleEnter();
		}
		if(m_pDI->KeyPressed(DIK_RETURN))
		{
			return this->HandleEnter();
		}
	}
	return true;
}

void CWinState::Update(float fElapsedTime)
{
	m_fSlotTime += fElapsedTime;
	if(m_fTotalTurns > 0 )
	{


	if(m_fSlotTime > m_fSlotDelay)
	{
		m_fTotalTurns--;
		m_fSlotTime = 0.0f;
		//m_fSlotDelay = rand() % 5 + 1;
	//	m_fSlotDelay /= 100;
	//	m_fSlotDelay *= m_fTotalTurns;
		m_fSlotDelay += m_fDelay;
		int d = 0;
		m_nSlot++;
		if(m_nSlot >= m_nNumSlots)
			m_nSlot = 0;

	}
	}

	m_pFM->Update();
}

void CWinState::Render(void)
{
	RECT cars;
	cars.left = 400;
	cars.top = 300;
	cars.right = cars.left + 60;
	cars.bottom = cars.top + 100;
	int color = 0;
	char buffer[128];
	sprintf_s(buffer,"%f",m_fTotalTurns);
	m_pD3D->DrawText(buffer,400,280,255,255,255);
	switch(m_nSlot)
	{
	case 0:
		color = 25;
		break;
	case 1:
		color = 100;
		break;
	case 2:
		color = 200;
		break;
	case 3:
		color = 255;
		break;
	}

	m_pD3D->DrawRect(cars,color,0,0);


}

bool CWinState::HandleEnter(void)
{
		m_pFM->PlaySound(m_nMenuSelect);
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance();
	return true;
}