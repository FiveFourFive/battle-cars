#include "CHighScoreState.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CGame.h"
#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CGamerProfile.h"
#include "CXboxInput.h"
#include "CMainMenuState.h"
#include <fstream>
#include <string>
using namespace std;
CHighscoreState::CHighscoreState(void)
{
	//highscores = new HighScore[10];
}

CHighscoreState::~CHighscoreState(void)
{

}

CHighscoreState::CHighscoreState(const CHighscoreState&)
{

}

CHighscoreState& CHighscoreState::operator=(const CHighscoreState&)
{
	return *this;
}

CHighscoreState* CHighscoreState::GetInstance(void)
{
	static CHighscoreState instance;
	return &instance;
}

void CHighscoreState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();

	m_pTM = CSGD_TextureManager::GetInstance();
	
	m_pController1 = CGame::GetInstance()->GetController1();
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));
	m_pPF = new CPrintFont(m_nFontID);

	m_pFM = CSGD_FModManager::GetInstance();
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
	LoadScores();
	SortScores();
}

void CHighscoreState::Exit(void)
{
	SortScores();
	SaveScores();
}
bool CHighscoreState::Input(void)
{
	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = m_pController1->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		//CKeyBinds* tempkeys = m_pController1->GetKB();
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
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
		if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
		{
			return this->HandleEnter();
		}
	}
	return true;
}

void CHighscoreState::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CHighscoreState::Render(void)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	RECT temp;					 
	temp.left = 0;				 
	temp.top = 0;				 
	temp.right = 800;			 
	temp.bottom = 600;			 
	pD3D->GetSprite()->Flush();
	pD3D->DrawRect(temp,0,0,0);

	m_pPF->Print("Top 10 High Scores",520,100,1.0f,D3DCOLOR_XRGB(255,0,0));

	for(int i = 0; i < 10; i++)
	{
		char buffer[128];
		sprintf_s(buffer," %i",m_vScoreList[i].score);
		string name = m_vScoreList[i].name;
		string scores = buffer;
		string periods;
		
		for(unsigned int j = 0; j < 20-name.size() - ((i+1)/10); j++)
		{
			periods += ".";
		}
		char buffer2[12];
		sprintf_s(buffer2,"%i ",i+1);
		string player =buffer2 + name + periods + scores;
		m_pPF->Print(player.c_str(),150,200 + (50*i),1.0f,D3DCOLOR_XRGB(255,255,255));
		//m_pPF->Print(buffer,100,100 + (50*i),1.0f,D3DCOLOR_XRGB(255,255,255));
	}

}

void CHighscoreState::SortScores(void)
{
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(m_vScoreList[i].score > m_vScoreList[j].score)
			{
				HighScore tempscore;
				tempscore = m_vScoreList[i];
				m_vScoreList[i] = m_vScoreList[j];
				m_vScoreList[j] = tempscore;
			}
		}
	}
}

HighScore* CHighscoreState::LoadScores(void)
{
	ifstream in;
	in.open("resource/data/HighScores.txt",ios_base::in);

	char* tempname = new char[32];
	int score;

	if(in.is_open())
	{
		for(int i = 0; i < 10; i++)
		{
			in >> tempname;
			in >> score;
			in.ignore(INT_MAX,'\n');

			HighScore temp;
			temp.name = tempname;
			temp.score = score;
			//highscores[i] = temp;
			m_vScoreList.push_back(temp);
			tempname = new char[32];
		}

	}
	in.close();
	return NULL;
}

void CHighscoreState::SaveScores(void)
{
	ofstream out;
	out.open("resource/data/HighScores.txt",ios_base::out);
	HighScore tempscore;
	if(out.is_open())
	{
		for(int i = 0; i < 10; i++)
		{
			tempscore = m_vScoreList[i];
			out << tempscore.name;
			out << ' ';
			out << tempscore.score;
			out << '\n';
		}

	}
	out.close();
}

HighScore* CHighscoreState::GetScores(void)
{
	return highscores[0];
}
bool CHighscoreState::AddScore(HighScore addme)
{
	for(int i = 0; i < 10; i++)
	{
		if(m_vScoreList[i].score < addme.score)
		{

			for(int j = 9; j > i; j--)
			{
				m_vScoreList[j] = m_vScoreList[j-1];
			
			}
			m_vScoreList[i] = addme;
			return true;
		}
	}
	return false;
}

bool CHighscoreState::HandleEnter(void)
{
	CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	return true;
};