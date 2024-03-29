
#include "CLoadingState.h"
#include "CSGD_TextureManager.h"
#include "ParticleManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "COptionState.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CHowToPlayState.h"
#include "CGamerProfile.h"
#include "CXboxInput.h"
#include "CWinState.h"
#include "CKeyBinds.h"
#include "CNumPlayers.h"
#include "CPowerUp.h"
#include "CEnemy.h"
#include "CObjectManager.h"
#include "CCharacterSelection.h"
#include "CLevelSelectionState.h"
#include "CMainMenuState.h"
#include "CSGD_FModManager.h"
#include "CCharacterSelection.h"
#include "CNumPlayers.h"
#include "CThread.h"
CLoadingState * CLoadingState::GetInstance()
{
	static CLoadingState instance;
	return &instance;
}
bool CLoadingState::HandleEnter()
{
	return true;
}
unsigned int __stdcall Load(void* load)
{



	ParticleManager::GetInstance()->LoadEmittor("resource/data/collision.xml");
	ParticleManager::GetInstance()->LoadEmittor("resource/data/missle_flame.xml");
	ParticleManager::GetInstance()->LoadEmittor("resource/data/explosion.xml");
	ParticleManager::GetInstance()->LoadEmittor("resource/data/car_exploded.xml");
	ParticleManager::GetInstance()->LoadEmittor("resource/data/FlameThrower.xml");
	ParticleManager::GetInstance()->LoadEmittor("resource/data/barrel_explode.xml");

	CGamePlayState::GetInstance()->SetLevel();

	CMessageSystem::GetInstance()->SendMsg (new CCreateLevelMessage());
	CMessageSystem::GetInstance()->ProcessMessages ();
	*((bool*)load) = true;
	return 0;
}
void CLoadingState::Enter(void)
{	
	loading = false;

	/*CGamePlayState::GetInstance()->GetCars().push_back(CCharacterSelection::GetInstance()->GetPlayer1());
	if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
	{
		CGamePlayState::GetInstance()->GetCars().push_back(CCharacterSelection::GetInstance()->GetPlayer2());
	}*/

	CThreadSystem::GetInstance()->StartThread(Load,(void*)&loading);

	timeStamp=(float)(timeGetTime());
	CSGD_FModManager::GetInstance()->StopSound(CMainMenuState::GetInstance()->GetBackgroundMusicID());
	m_nBGImageID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/gamestates images/mainmenu_bg.jpg");
	m_nAnimID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Weapons/BC_MG.png");
	m_nBackgroundMusicID = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Superbeast.mp3",SGD_FMOD_LOOPING);
	m_nCountDown = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdown.mp3");
	m_nCountDownEnd = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/Countdowntone.mp3");
	//CSGD_FModManager::GetInstance()->PlaySound(m_nBackgroundMusicID);

	//ParticleManager::GetInstance()->LoadEmittor("resource/data/collision.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/missle_flame.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/explosion.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/car_exploded.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/FlameThrower.xml");
	//ParticleManager::GetInstance()->LoadEmittor("resource/data/barrel_explode.xml");

	pFont = new CPrintFont(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0)));

}


void CLoadingState::Exit(void)
{
	delete pFont;
}

bool CLoadingState::Input(void)
{
	return true;
}

void CLoadingState::Update(float fElapsedTime)
{
	if((loading))
	{
		timeStamp=(float)(timeGetTime());
		
		CGame::GetInstance()->ClearStates(CGamePlayState::GetInstance());
	}
}

void CLoadingState::Render(void)
{
	static float rot = 0.0f;
	
	rot +=.04f;
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	CSGD_TextureManager::GetInstance()->Draw(m_nBGImageID, 0,0,2.9f, 1.8f);
	pFont->Print("Loading",CGame::GetInstance()->GetScreenWidth()/2-150,500,3.0f,D3DCOLOR_ARGB(255,255,0,0));
	CSGD_TextureManager::GetInstance()->Draw(m_nAnimID, CGame::GetInstance()->GetScreenWidth()-128,CGame::GetInstance()->GetScreenHeight()-128,1.0f,1.0f,NULL,64,64,rot);
}
