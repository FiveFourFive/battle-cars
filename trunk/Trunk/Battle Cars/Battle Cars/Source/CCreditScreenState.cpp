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

	m_nImageID = -1;
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
	
	m_nImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/mainmenu_bg.jpg");
	m_nSoundID = m_pFM->LoadSound("resource/sounds/neoncity.mp3");
	m_pFM->SetVolume(m_nSoundID,CGame::GetInstance()->getSoundBVolume());
	m_pFM->PlaySoundA(m_nSoundID);

}

void CCreditsScreenState::Exit()
{
	m_pFM->StopSound(m_nSoundID);
	m_pFM->UnloadSound(m_nSoundID);
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
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				CGame::GetInstance()->RemoveState(this);
			}
		}
	}
	else
	{
		if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonPressed(1))
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
	m_pTM->Draw(m_nImageID, 0, 0, 2.9f, 1.8f);
	m_pPF->Print("Battle Cars: Mayhem and Destruction", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 328), (int)y[0], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Executive Producer", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 328), (int)y[1], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("John O'Leske", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 264), (int)y[2], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Associate Producer:", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 328), (int)y[3], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Sean Hathaway", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 264), (int)y[4], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Robert Martinez", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 264), (int)y[5], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Shawn \"The New Guy\" Paris", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 264), (int)y[6], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Artist:", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 328), (int)y[7], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Chris Jahosky", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 264), (int)y[8], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Daniel Kahn", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 264), (int)y[9], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Effects: Andy Madruga", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 264), (int)y[10], 0.8f, D3DCOLOR_XRGB(0,255,0));


	m_pPF->Print("Sleepless Burrito Slayers Team Programmers/Members:", (int)(CGame::GetInstance()->GetScreenWidth()*0.1f), (int)y[11], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Andy Madruga", (int)(CGame::GetInstance()->GetScreenWidth()* 0.1f  + 64), (int)y[12], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Wade Shwallon", (int)(CGame::GetInstance()->GetScreenWidth()*0.1f + 64), (int)y[13], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Joshua Fields", (int)(CGame::GetInstance()->GetScreenWidth()*0.1f + 64), (int)y[14], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("John Rostick", (int)(CGame::GetInstance()->GetScreenWidth()* 0.1f  + 64), (int)y[15], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Daniel Kahn", (int)(CGame::GetInstance()->GetScreenWidth()*  0.1f   + 64), (int)y[16], 0.8f, D3DCOLOR_XRGB(0,255,0));

	m_pPF->Print("Special Thanks To:", (int)(CGame::GetInstance()->GetScreenWidth()*0.5f - 328),(int) y[17], 0.8f, D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("Dave Brown", (int)(CGame::GetInstance()->GetScreenWidth()*  0.1f   + 64),(int) y[18], 0.8f, D3DCOLOR_XRGB(0,255,0));


	
	


}

bool CCreditsScreenState::HandleEnter()
{
	return true;
}