#include "CLevelSelectionState.h"

#include "CPrintFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CXboxInput.h"
#include "CGamePlayState.h"
#include "SGD_Math.h"
#include "CNumPlayers.h"

#include "CGame.h"
#include "tinyxml.h"

CLevelSelectionState::CLevelSelectionState()
{
	m_pPF = NULL;

	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();

	for( int i = 0; i < LEVEL_MAX; i++)
	{
		m_ListofLevels[i] = NULL;
	}

	m_nBGImageID = -1;

	if( LoadLevel("resource/data/LevelSelectionData.xml") == false)
		MessageBox(0, "Failed to Load Level Image and Data FileName", 0, 0);
}

CLevelSelectionState::~CLevelSelectionState()
{
	for( int i = 0; i < LEVEL_MAX; i++)
	{
		delete m_ListofLevels[i];
		m_ListofLevels[i] = NULL;
	}
}

CLevelSelectionState* CLevelSelectionState::GetInstance()
{
	static CLevelSelectionState instance;
	return &instance;
}

void CLevelSelectionState::Enter()
{

	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));
	m_pFM->SetVolume(m_nMenuSelect,CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove,CGame::GetInstance()->getSoundAVolume());
	m_pPF = new CPrintFont(m_nFontID);

	m_nBGImageID = m_pTM->LoadTexture("resource/graphics/gamestates images/optionstate.jpg");
	m_nSelection = 0;

	color = D3DCOLOR_ARGB(255,255,255,255);
	isSet = false;
	m_nCurrentFrame = 0;

	for( int i = 0; i < 8; i++)
	{
		m_nMovieID[i]=-1;
	}
	for( int i = 0; i < 2; i++)
	{
		render_ids[i]=-1;
	}

	m_nMovieID[0] = m_pTM->LoadTexture("resource/videos/level1/1.bmp", D3DCOLOR_ARGB(255,255,255,255));
	m_nMovieID[1] = m_pTM->LoadTexture("resource/videos/level1/2.bmp", D3DCOLOR_ARGB(255,255,255,255));
	m_nMovieID[2] = m_pTM->LoadTexture("resource/videos/level2/1.bmp", D3DCOLOR_ARGB(255,0,255,255));
	m_nMovieID[3] = m_pTM->LoadTexture("resource/videos/level2/2.bmp", D3DCOLOR_ARGB(255,0,255,255));
	m_nMovieID[4] = m_pTM->LoadTexture("resource/videos/level3/1.bmp", D3DCOLOR_ARGB(255,0,255,255));
	m_nMovieID[5] = m_pTM->LoadTexture("resource/videos/level3/2.bmp", D3DCOLOR_ARGB(255,0,255,255));
	m_nMovieID[6] = m_pTM->LoadTexture("resource/videos/level4/1.bmp", D3DCOLOR_ARGB(255,0,255,255));
	m_nMovieID[7] = m_pTM->LoadTexture("resource/videos/level4/2.bmp", D3DCOLOR_ARGB(255,0,255,255));
}

void CLevelSelectionState::Exit()
{
	for( int i = 0; i < 6; i++)
	{
		m_pTM->UnloadTexture(m_nMovieID[i]);
	}

	delete m_pPF;
}

bool CLevelSelectionState::Input()
{
	if(CGame::GetInstance()->ControllerInput())
	{
		XINPUT_STATE xState = CGame::GetInstance()->GetController1()->GetState();
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				CGame::GetInstance()->ResetInputDelay();
				return this->HandleEnter();
			}
			else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				CGame::GetInstance()->ResetInputDelay();
				CGame::GetInstance()->RemoveState(this);
			}
		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			
				if(x < 8000 && x > -8000 && y > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
						CGame::GetInstance()->ResetThumbDelay();
					isSet = false;
					m_nSelection--;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection < 0)
						m_nSelection = LEVEL_MAX - 1;
				}
				else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
						CGame::GetInstance()->ResetThumbDelay();
					isSet = false;
					m_nSelection++;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection >= LEVEL_MAX)
						m_nSelection = 0;
				}
		}
	}
	else
	{
		if(m_pDI->KeyPressed(DIK_ESCAPE)||m_pDI->JoystickButtonDown(1))
		{
			CGame::GetInstance()->RemoveState(this);
		}

		if(m_pDI->KeyPressed(DIK_RETURN)||m_pDI->JoystickButtonDown(0))
		{
			return this->HandleEnter();
		}

		if(m_pDI->KeyPressed(DIK_UP)||m_pDI->JoystickGetLStickDirDown(DIR_UP))
		{
			isSet = false;
			m_nSelection--;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection < 0)
				m_nSelection = LEVEL_MAX - 1;
		}

		if(m_pDI->KeyPressed(DIK_DOWN)||m_pDI->JoystickGetLStickDirDown(DIR_DOWN))
		{
			isSet = false;
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection >= LEVEL_MAX)
				m_nSelection = 0;
		}

	}
	return true;
}

void CLevelSelectionState::Update(float fElapsedTime)
{
	LoadVideo();

	m_fMovieTimer += fElapsedTime;
	if( m_fMovieTimer >= 9.0f )
	{
		m_fMovieTimer = 0.0f;
		m_nCurrentFrame++;
		if( m_nCurrentFrame > 1 )
			m_nCurrentFrame = 0;
	}

	if( m_fMovieTimer < 3.0f )
	{
		Alpha = unsigned char(((255 - 0) * (m_fMovieTimer / 3.0f)));

		color = D3DCOLOR_ARGB(Alpha, 255,255,255);
	}
	else if( m_fMovieTimer > 6.0f)
	{
		Alpha = unsigned char(255 - ((255 - 0) * (m_fMovieTimer / 3.0f)));
		if( Alpha <= 1 )
			Alpha = 255;
		color = D3DCOLOR_ARGB(Alpha, 255,255,255);
	}
	m_pFM->Update();

}

void CLevelSelectionState::Render()
{
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	m_pTM->Draw(m_nBGImageID, 0, 0, 1.0f, 1.0f);

	m_pPF->PrintCentered("Level Selection",CGame::GetInstance()->GetScreenWidth()/2,50,2.0f,D3DCOLOR_XRGB(255,0,0));

	for( int i = 0; i < LEVEL_MAX; i++)
	{
		string buffer = m_ListofLevels[i]->FileName;
		buffer.erase(buffer.begin() + buffer.length() - 4, buffer.end());

		if( i != m_nSelection)
		{
			m_pPF->Print(buffer.c_str(), 30, (i + 1) * 100 + 100, 1.0f, D3DCOLOR_XRGB(200, 0, 0));
		}
		else
		{
			m_pPF->Print(buffer.c_str(), 30, (i + 1) * 100 + 100, 1.0f, D3DCOLOR_XRGB(0, 255, 0));
			m_pTM->Draw(render_ids[m_nCurrentFrame], 450, 250, 0.4f, 0.4f, NULL,0, 0, 0, color);
		}
	}


}

bool CLevelSelectionState::HandleEnter()
{
	CGame::GetInstance()->AddState(CNumPlayers::GetInstance());
	return true;
}


bool CLevelSelectionState::LoadLevel(const char* szXmlFileName)
{
	TiXmlDocument doc;
	int index = 0;

	if(doc.LoadFile(szXmlFileName) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if(!pRoot)
		return false;

	TiXmlElement* pLevel = pRoot->FirstChildElement("Level");

	while(pLevel != NULL)
	{
		m_ListofLevels[index] = new SLevel();

		TiXmlElement* pDataName = pLevel->FirstChildElement("LevelDataName");
		if( pDataName != NULL)
		{
			m_ListofLevels[index]->FileName = pDataName->GetText();
			m_ListofLevels[index]->FileName += ".xml";
		}
		else
			MessageBox(0,"Failed to load data name",0,0);

		index++;
		pLevel = pLevel->NextSiblingElement();
	}

	return true;
}

void CLevelSelectionState::LoadVideo()
{
	if( !isSet )
	{
		isSet = true;
		m_fMovieTimer = 0.0f;
		m_nCurrentFrame = 0;


		switch(m_nSelection)
		{
		case LEVEL_ONE:
			{
				render_ids[0] = m_nMovieID[0];
				render_ids[1] = m_nMovieID[1];

			}
			break;
		case LEVEL_TWO:
			{
				render_ids[0] = m_nMovieID[2];
				render_ids[1] = m_nMovieID[3];
			}
			break;

		case LEVEL_THREE:
			{
				render_ids[0] = m_nMovieID[4];
				render_ids[1] = m_nMovieID[5];
			}
			break;
			case LEVEL_FOUR:
			{
				render_ids[0] = m_nMovieID[6];
				render_ids[1] = m_nMovieID[7];
			}
			break;
		}
	}
}