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
	m_pTM = NULL;
	m_pPF = NULL;
	m_pFM = NULL;
	m_pDI = NULL;

	for( int i = 0; i < LEVEL_MAX; i++)
	{
		m_ListofLevels[i] = NULL;
	}
}

CLevelSelectionState::~CLevelSelectionState()
{

}

CLevelSelectionState* CLevelSelectionState::GetInstance()
{
	static CLevelSelectionState instance;
	return &instance;
}

void CLevelSelectionState::Enter()
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();
	

	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nFontID = m_pTM->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);

	if( LoadLevel("resource/data/LevelSelectionData.xml") == false)
		MessageBox(0, "Failed to Load Level Image and Data FileName", 0, 0);

	m_nSelection = 0;

}

void CLevelSelectionState::Exit()
{
	for( int i = 0; i < LEVEL_MAX; i++)
	{
		m_pTM->UnloadTexture(m_ListofLevels[i]->ImageID);
		delete m_ListofLevels[i];
		m_ListofLevels[i] = NULL;
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
			CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				return this->HandleEnter();
			}
			else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				CGame::GetInstance()->RemoveState(this);
			}
		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
				CGame::GetInstance()->ResetThumbDelay();
				if(x < 8000 && x > -8000 && y > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					m_nSelection--;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection < 0)
						m_nSelection = LEVEL_MAX - 1;
				}
				else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					m_nSelection++;
					m_pFM->PlaySound(m_nMenuMove);
					if(m_nSelection >= LEVEL_MAX)
						m_nSelection = 0;
				}
		}
	}
	else
	{
		if(m_pDI->KeyPressed(DIK_ESCAPE))
		{
			CGame::GetInstance()->RemoveState(this);
		}

		if(m_pDI->KeyPressed(DIK_RETURN))
		{
			return this->HandleEnter();
		}

		if(m_pDI->KeyPressed(DIK_UP))
		{
				m_nSelection--;
				m_pFM->PlaySound(m_nMenuMove);
				if(m_nSelection < 0)
					m_nSelection = LEVEL_MAX - 1;
		}

		if(m_pDI->KeyPressed(DIK_DOWN))
		{
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
	m_pFM->Update();
}

void CLevelSelectionState::Render()
{
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	RECT temp_render = {0, 0, CGame::GetInstance()->GetScreenWidth(), CGame::GetInstance()->GetScreenHeight()};
	CSGD_Direct3D::GetInstance()->DrawRect( temp_render, 0,0,0);

	for( int i = 0; i < LEVEL_MAX; i++)
	{
		if( i != m_nSelection)
		{
			m_pPF->Print(m_ListofLevels[i]->FileName.c_str(), 10, (i + 1) * 50, 0.5f, D3DCOLOR_XRGB(200, 0, 0));
		}
		else
		{
			m_pPF->Print(m_ListofLevels[i]->FileName.c_str(), 10, (i + 1) * 50, 0.5f, D3DCOLOR_XRGB(0, 255, 0));
			m_pTM->Draw(m_ListofLevels[i]->ImageID, 200, 250, 1.0f, 1.0f);
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
		std::string texturefilename = "resource/graphics/";

		TiXmlElement* pImageName = pLevel->FirstChildElement("LevelImageName");
		if( pImageName != NULL)
		{
			texturefilename += pImageName->GetText();
			m_ListofLevels[index]->ImageID = m_pTM->LoadTexture(texturefilename.c_str());
		}

		TiXmlElement* pDataName = pLevel->FirstChildElement("LevelDataName");
		if( pDataName != NULL)
			m_ListofLevels[index]->FileName = pDataName->GetText();

		index++;
		pLevel = pLevel->NextSiblingElement();
	}

	return true;
}