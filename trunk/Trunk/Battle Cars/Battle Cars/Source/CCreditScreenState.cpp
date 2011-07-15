#include "CCreditsScreenState.h"
#include "ParticleManager.h"
#include "CPrintFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CXboxInput.h"
#include "CGame.h"
#include "CKeyBinds.h"
#include "CMainMenuState.h"

CCreditsScreenState::CCreditsScreenState()
{
	m_pD3D			= NULL;
	m_pTM			= NULL;
	m_pPF			= NULL;
	m_pFM			= NULL;
	m_pDI			= NULL;
	m_pController1	= NULL;
}

CCreditsScreenState::~CCreditsScreenState()
{

}

CCreditsScreenState* CCreditsScreenState::GetInstance()
{
	static CCreditsScreenState instance;
	return &instance;
}

void CCreditsScreenState::Enter()
{
	for( int i = 0; i < PPLTOCREDIT; i++)
	{
		y[i] = CGame::GetInstance()->GetScreenHeight() + (100.0f * i);
	}

	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();
	m_pController1 = CGame::GetInstance()->GetController1();

	m_pPF = new CPrintFont(m_nFontID);
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));

}

void CCreditsScreenState::Exit()
{
	delete m_pPF;
}

bool CCreditsScreenState::Input()
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
				CGame::GetInstance()->RemoveState(this);
			}
		}
	}
	else
	{
		if(m_pDI->KeyPressed(DIK_ESCAPE))
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		}
	}
	return true;
}

void CCreditsScreenState::Update(float fElapsedTime)
{
	for( int i = 0; i < PPLTOCREDIT; i++)
	{
		y[i] -= 50.0f * fElapsedTime;
	}

	if( y[PPLTOCREDIT - 1] < 0.0f)
	{
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
}

void CCreditsScreenState::Render()
{
	m_pPF->Print("Battle Cars: Mayhem and Destruction", CGame::GetInstance()->GetScreenWidth()*0.5f - 328, y[0], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Sleepless Burrito Slayers Team Members:", CGame::GetInstance()->GetScreenWidth()*0.1f, y[1], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Andy Madruga", CGame::GetInstance()->GetScreenWidth()* 0.1f  + 64, y[2], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Wade Shwallon", CGame::GetInstance()->GetScreenWidth()*0.1f + 64, y[3], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Joshua Fields", CGame::GetInstance()->GetScreenWidth()*0.1f + 64, y[4], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("John Rostick", CGame::GetInstance()->GetScreenWidth()* 0.1f  + 64, y[5], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Daniel Khan", CGame::GetInstance()->GetScreenWidth()*  0.1f   + 64, y[6], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Associate Producer:", CGame::GetInstance()->GetScreenWidth()*0.5f - 328, y[7], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Sean Hathaway", CGame::GetInstance()->GetScreenWidth()*0.5f - 264, y[8], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Executive Producer", CGame::GetInstance()->GetScreenWidth()*0.5f - 328, y[9], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("John Jacob Jingle HeimerSchmidtz", CGame::GetInstance()->GetScreenWidth()*0.5f - 264, y[10], 0.8f, D3DCOLOR_XRGB(0,255,0));






}

bool CCreditsScreenState::HandleEnter()
{
	return true;
}