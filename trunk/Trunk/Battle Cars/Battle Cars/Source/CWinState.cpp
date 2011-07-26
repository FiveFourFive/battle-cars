
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
#include "CHighScoreState.h"
#include "CGamerProfile.h"
#include "Gamer_Profile.h"

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
	m_fTotalTurns = float(rand() % 100 + 100);
	m_fDelay = 0.002f;
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
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);
	m_pController1 = CGame::GetInstance()->GetController1();
	m_pController2 = CGame::GetInstance()->GetController2();
	m_pFM = CSGD_FModManager::GetInstance();
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/FeelSoNumb.mp3",SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/otherstates.jpg");
		m_pFM->SetVolume(m_nBackgroundMusicID,CGame::GetInstance()->getSoundBVolume());
	m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
	miniID = m_pTM->LoadTexture("resource/graphics/BC_Mini.png");
	vetteID = m_pTM->LoadTexture("resource/graphics/BC_Corvette.png");
	humveeID = m_pTM->LoadTexture("resource/graphics/BC_Humvee.png");
	truckID = m_pTM->LoadTexture("resource/graphics/BC_Truck.png");

	int playerscore = CGame::GetInstance()->GetScore();
	CHighscoreState::GetInstance()->LoadScores();
	
	HighScore addme;

	Gamer_Profile* tempprofile = CGamerProfile::GetInstance()->GetActiveProfile();
	string name = tempprofile->GetUserNameA();
	char* buffer = new char[38];
	strcpy_s(buffer,38,name.c_str());

	for(int i = 0; i < 4; i++)
	{
		m_bRoll = false;
		if(tempprofile->cars[i] == false)
		{
			m_bRoll = true;
			break;
		}
	}


	addme.name = buffer;
	addme.score = CGame::GetInstance()->GetScore();
	CHighscoreState::GetInstance()->AddScore(addme);

	CHighscoreState::GetInstance()->SaveScores();
}
void CWinState::Exit(void)
{
	if(id == miniID)
		CGamerProfile::GetInstance()->GetActiveProfile()->cars[0] = 1;
	else if(id == vetteID)
		CGamerProfile::GetInstance()->GetActiveProfile()->cars[1] = 1;
	else if(id == humveeID)
		CGamerProfile::GetInstance()->GetActiveProfile()->cars[2] = 1;
	else if(id == truckID)
		CGamerProfile::GetInstance()->GetActiveProfile()->cars[3] = 1;
	CGamerProfile::GetInstance()->SaveWinnerCar(CGamerProfile::GetInstance()->GetActiveProfile()->index);
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
			
			if(xState.Gamepad.wButtons & tempkeys->GetBack())
			{
				CGame::GetInstance()->ResetInputDelay();
				return HandleEnter();
			}
			if(xState.Gamepad.wButtons & tempkeys->GetAccept())
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
		if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonDown(1))
		{
			return this->HandleEnter();
		}
		if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
		{
			return this->HandleEnter();
		}
	}
	return true;
}

void CWinState::Update(float fElapsedTime)
{
	m_fSlotTime += fElapsedTime;
	if(m_bRoll)
	{
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
	}
	m_pFM->Update();
}

void CWinState::Render(void)
{
	m_pTM->Draw(m_nBGImageID, 0, 0, 1.5f, 1.0f);
	m_pPF->Print("Game Over You Win",220,50,1.0f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("Final Score: ", 100,150,1.0f,D3DCOLOR_XRGB(0,0,255));
	char buffer[128];
	int playerscore = CGame::GetInstance()->GetScore();
	sprintf_s(buffer,"%i", playerscore);
	m_pPF->Print(buffer,500,150,1.0f,D3DCOLOR_XRGB(0,255,0));
	m_pPF->Print("PRESS ENTER/BACK TO CONTINUE",60,430,1.0f,D3DCOLOR_XRGB(255,255,255));
	
	RECT cars;
	cars.left = 600;
	cars.top = 300;
	cars.right = cars.left + 60;
	cars.bottom = cars.top + 100;
	int color = 0;
	
	sprintf_s(buffer,"%f",m_fTotalTurns);
	//m_pD3D->DrawText(buffer,400,280,255,255,255);
	Gamer_Profile* tempprofile = CGamerProfile::GetInstance()->GetActiveProfile();
	if(m_bRoll)
	{
	switch(m_nSlot)
	{
	case 0:
		if(tempprofile->cars[0] == 0)
		{
			id = miniID;
		break;
		}
	case 1:
		if(tempprofile->cars[1] == 0)
		{
		id = vetteID;
		break;
		}
	case 2:
		if(tempprofile->cars[2] == 0)
		{
		id = humveeID;
		break;
		}
	case 3:
		if(tempprofile->cars[3] == 0)
		{
		id = truckID;
		break;
		}
	}
	if(id >= 0)
		m_pTM->Draw(id,800,250,1.0f,1.0f);
	}
	//m_pD3D->DrawRect(cars,color,0,0);


}

bool CWinState::HandleEnter(void)
{
		m_pFM->PlaySound(m_nMenuSelect);
		CGame::GetInstance()->ChangeState(CHighscoreState::GetInstance());
		//CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	return true;
}