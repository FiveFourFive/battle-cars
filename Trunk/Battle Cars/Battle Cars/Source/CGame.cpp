
/////////////////////////////////////////////////////////
//	File Name : "CGame.cpp"
//	Author Name : Wade Shwallon
//	Purpose :  contains all game related code
/////////////////////////////////////////////////////////

#include <ctime>
#include "CGame.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CMainMenuState.h"
#include "COptionState.h"
#include "CSGD_DirectInput.h"
#include "CXboxInput.h"
//#include <XInput.h>
CGame::CGame()
{
	m_pD3D				=	NULL;
	m_pTM				=	NULL;
	m_pFM				=	NULL;
	m_pDI				=	NULL;
	m_pGameState		=	NULL;
	m_pMainMenuState	=	NULL;
	m_pController1 = NULL;
	m_pController2 = NULL;
	m_bController = false;

	m_fElapsedTime		=	0;
	m_ScreenWidth	= 800;
	m_ScreenHeight	= 600;

	srand (unsigned int (time (NULL)));
}

CGame::~CGame()
{

}

CGame::CGame(const CGame& game)
{

}

CGame& CGame::operator =(const CGame& game)
{
	return *this;
}

CGame* CGame::GetInstance()
{
	static CGame instance;
	return &instance;
}

void CGame::Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	// Get pointers to singletons
	m_pD3D	=	CSGD_Direct3D::GetInstance();
	m_pTM	=	CSGD_TextureManager::GetInstance();
	m_pFM	=	CSGD_FModManager::GetInstance();
	m_pDI	=	CSGD_DirectInput::GetInstance();

	// Controllers
	m_pController1 = new CXboxInput(1);
	m_pController2 = new CXboxInput(2);

	m_bController = m_pController1->Connected();
	m_fInputDelay = 1.0f;
	m_fThumbDelay = 1.0f;
	// Initialize Singletons
	m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, false);
	m_pTM->InitTextureManager( m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	m_pFM->InitFModManager(hWnd);
	m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD /*| DI_MOUSE*/, 0);
	// Initialize States
	m_pMainMenuState = CMainMenuState::GetInstance();
	this->AddState(m_pMainMenuState);
	//this->AddState(COptionState::GetInstance());
	// Set GameState
	//ChangeState(CMainMenuState::GetInstance());

	/*m_nSoundAVolume = 0.10f;
	m_nSoundBVolume = 0.8f;
	m_panning = 0.0f;*/
	m_nSoundAVolume = 0.5f;
	m_nSoundBVolume = 0.5f;
	m_stopWatch.Start();
}

bool CGame::Main()
{
	////////////////////////////////////////
	//GameTime
	m_fElapsedTime = (float)m_stopWatch.GetElapsedTime();
	m_stopWatch.Reset();
	////////////////////////////////////////


	// Input
	if( Input() == false )
	{
		return false;
	}

	// Update
	Update(m_fElapsedTime);

	// Render
	Render();

	return true;
}

bool CGame::Input()
{
	static bool windowed = true;
	m_pDI->ReadDevices();


	////xState.Gamepad.wButtons |= XINPUT_GAMEPAD_X;
	if(m_bController)
	{
		m_pController1->ReadInputState();
		m_pController2->ReadInputState();

		XINPUT_STATE xState = m_pController1->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;

		if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			windowed = !windowed;
			m_pD3D->ChangeDisplayParam(m_ScreenWidth, m_ScreenHeight, windowed);
			return true;
		}
	}
	else
	{
		if(((m_pDI->KeyDown(DIK_LALT) && m_pDI->KeyDown(DIK_RETURN))))
		{
			windowed = !windowed;
			m_pD3D->ChangeDisplayParam(m_ScreenWidth, m_ScreenHeight, windowed);
			return true;
		}

		if (((m_pDI->KeyDown(DIK_RALT) && m_pDI->KeyDown(DIK_RETURN))))
		{
			windowed = !windowed;
			m_pD3D->ChangeDisplayParam(m_ScreenWidth, m_ScreenHeight, windowed);
			return true;
		}
	}

	return m_vGameStates.back()->Input();
}

void CGame::Update(float fElapsedTime)
{
	m_fInputDelay += fElapsedTime;
	m_fThumbDelay += fElapsedTime;
	if(!m_pController1->Connected())
		m_bController = false;
	m_vGameStates.back()->Update(fElapsedTime);
}

void CGame::Render()
{
	m_pD3D->Clear(0, 0, 0);
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();
	for(unsigned int i = 0; i < m_vGameStates.size(); i++)
		m_vGameStates[i]->Render();
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();

}

void CGame::ChangeState(IGameState* state)
{
	if(m_vGameStates.back())
	{
		m_vGameStates.back()->Exit();
		m_vGameStates.pop_back();
	}

	m_vGameStates.push_back(state);

	if(m_vGameStates.back())
		m_vGameStates.back()->Enter();

}

void CGame::AddState(IGameState* state)
{
	m_vGameStates.push_back(state);
	m_vGameStates.back()->Enter();
}

void CGame::RemoveState(IGameState* state)
{
	m_vGameStates.back()->Exit();
	m_vGameStates.pop_back();

}


void CGame::Shutdown()
{
	delete m_pController1;
	delete m_pController2;
	for(unsigned int i = 0; i < m_vGameStates.size(); i++)
	{
		m_vGameStates.back()->Exit();
		m_vGameStates.pop_back();
	}

	// Shutdown in opposite order
	if(m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}

	if(m_pFM)
	{
		m_pFM->ShutdownFModManager();
		m_pFM = NULL;
	}

	if(m_pTM)
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = NULL;
	}

	if(m_pD3D)
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = NULL;
	}


}

float CGame::getSoundAVolume()
{
	return m_nSoundAVolume;
}
float CGame::getSoundBVolume()
{
	return m_nSoundBVolume;
}

void CGame::SetSoundAVolume(float newvol)
{
	m_nSoundAVolume = newvol;
}
void CGame::SetSoundBVolume(float newvol)
{
	m_nSoundBVolume = newvol;
}

float CGame::getPanning()
{
	return m_panning;
}


void CGame::SetPanning(float left)
{
	m_panning = left;
}