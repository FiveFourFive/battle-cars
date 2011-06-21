#include "CGamerProfile.h"

#include <stdio.h>
#include "tinyxml.h"
#include "CGame.h"

#include "Gamer_Profile.h"
#include "CMainMenuState.h"

#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CPrintFont.h"
#include "CXboxInput.h"

CGamerProfile* CGamerProfile::instance = NULL;

CGamerProfile::CGamerProfile()
{
	m_pD3D  = NULL;
	m_pDI   = NULL;
	m_pTM   = NULL;
	m_pFM   = NULL;
	m_pPF   = NULL;
	activeProfile = NULL;
	editProfile = false;

	PosX = 0;

	for( int i = 0; i < 4; i++)
	{
		PosY[i] = (i + 1) * 100;;
	}
	PosY[4] = 500;

	CurrPos = 200;
}

CGamerProfile::~CGamerProfile()
{
	for( unsigned int i = 0; i < m_vUserProfiles.size(); i++)
	{
		delete m_vUserProfiles[i];
	}

	m_vUserProfiles.clear();
}

CGamerProfile* CGamerProfile::GetInstance()
{
	static CGamerProfile instance;
	return &instance;
}

void CGamerProfile::Enter()
{

	m_pD3D 	 = CSGD_Direct3D::GetInstance();
	m_pDI  	 = CSGD_DirectInput::GetInstance();
	m_pTM  	 = CSGD_TextureManager::GetInstance();
	m_pFM  	 = CSGD_FModManager::GetInstance();
	m_pPF  	 = new CPrintFont(m_pTM->LoadTexture("resource/graphics/FontPlaceholder.png",D3DCOLOR_XRGB(0, 0, 0)));

	BGMusicID = m_pFM->LoadSound("Resource/Sounds/NeonCity.mp3", FMOD_LOOP_NORMAL);
	PosX = 350;

	LoadProfiles("resource/data/gamer_profile.xml");

	temp = 'A';

}

void CGamerProfile::Exit()
{

	m_pFM->StopSound(BGMusicID);
	m_pFM->UnloadSound(BGMusicID);

	delete m_pPF;

	for( unsigned int i = 0; i < m_vUserProfiles.size(); i++)
	{
		delete m_vUserProfiles[i];
	}

	m_vUserProfiles.clear();
}

bool CGamerProfile::Input()
{
	if( editProfile )
	{

		if(CGame::GetInstance()->ControllerInput())
		{
			XINPUT_STATE xState = CGame::GetInstance()->GetController1()->GetState();
			BYTE rTrig = xState.Gamepad.bRightTrigger;
			float x = xState.Gamepad.sThumbLX;
			float y = xState.Gamepad.sThumbLY;

			if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
			{
				CGame::GetInstance()->ResetInputDelay();

				if( xState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
				{
					if( activeProfile->m_sUserName.size() > 0)
						activeProfile->m_sUserName.erase(activeProfile->m_sUserName.begin() + activeProfile->m_sUserName.size() - 1);
				}
				if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
				{
					activeProfile->m_sUserName += temp;
					temp = 'A';
				}
				if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
				{
					if( editProfile )
					editProfile = false;

					SaveProfiles("resource/data/gamer_profile.xml");
				}

			}

			if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
			{
				CGame::GetInstance()->ResetThumbDelay();
				if(x < 8000 && x > -8000 && y > 16000)
				{
					temp++;
				}
				else if(x < 8000 && x > -8000 && y < -16000)
				{
					temp--;
				}
			}
		}
		else
		{
			if( m_pDI->KeyPressed(DIK_BACKSPACE))
			{
				if( activeProfile->m_sUserName.size() > 0)
					activeProfile->m_sUserName.erase(activeProfile->m_sUserName.begin() + activeProfile->m_sUserName.size() - 1);
			}
			if(  m_pDI->KeyPressed(DIK_UP))
			{
				temp++;
			}
			if(  m_pDI->KeyPressed(DIK_DOWN))
			{
				temp--;
			}

			if( m_pDI->KeyPressed(DIK_RETURN))
			{
				activeProfile->m_sUserName += temp;
				temp = 'A';
			}

			if( m_pDI->KeyPressed(DIK_ESCAPE))
			{
				if( editProfile )
					editProfile = false;

				SaveProfiles("resource/data/gamer_profile.xml");
			}
		}

		if( temp < 64 )
			temp = 90;
		if( temp > 90 )
			temp = 64;

		return true;
	}

	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = CGame::GetInstance()->GetController1()->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;

		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetInputDelay();

			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				CurrPos -= 100;
			}
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				CurrPos += 100;
			}
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				HandleEnter();
			}
			if( xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
				CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		}

		if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetThumbDelay();
			if(x < 8000 && x > -8000 && y > 16000)
			{
				CurrPos -= 100;
			}
			else if(x < 8000 && x > -8000 && y < -16000)
			{
				CurrPos += 100;
			}
		}
	}
	else
		if( m_pDI->KeyPressed(DIK_UP))
			CurrPos -= 100;
		if( m_pDI->KeyPressed(DIK_DOWN))
			CurrPos += 100;
		if( m_pDI->KeyPressed(DIK_RETURN))
			HandleEnter();

		if( m_pDI->KeyPressed(DIK_ESCAPE))
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	if( CurrPos > 500 )
		CurrPos = 200;
	if( CurrPos < 200)
		CurrPos = 500;


	return true;
}

void CGamerProfile::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CGamerProfile::Render()
{
	if( editProfile)
	{
		m_pPF->Print(activeProfile->m_sUserName.c_str(), PosX, PosY[2], 1.0f, D3DCOLOR_XRGB(128,128,255));
		string awesome;
		awesome += temp;
		m_pPF->Print(awesome.c_str(), PosX + 32 * activeProfile->m_sUserName.length() , PosY[2], 1.0f, D3DCOLOR_XRGB(128,128,255));
		return;
	}

	m_pPF->Print("GAMER PROFILES", 100, PosY[0], 1.5f, D3DCOLOR_XRGB(255,255,0));

	for( unsigned int i = 0; i < m_vUserProfiles.size(); i++)
	{
		if( CurrPos == PosY[i+1])
		{
			m_pPF->Print(m_vUserProfiles[i]->m_sUserName.c_str(), PosX, PosY[i+1], 1.0f, D3DCOLOR_XRGB(0,255,0));
		}
		else
			m_pPF->Print(m_vUserProfiles[i]->m_sUserName.c_str(), PosX, PosY[i+1], 0.5f, D3DCOLOR_XRGB(255,0,0));
	}
	if( CurrPos == PosY[4])
		{
			m_pPF->Print("EDIT PROFILE NAME", 100, PosY[4], 0.5f, D3DCOLOR_XRGB(0,255,0));
		}
	else
		m_pPF->Print("EDIT PROFILE NAME", 100, PosY[4], 0.5f, D3DCOLOR_XRGB(255,0,0));

}

bool CGamerProfile::LoadProfiles(const char* szXmlFileName)
{
	// Temporarily load up in XML unless process is slow.
	TiXmlDocument doc;

	if(doc.LoadFile(szXmlFileName) == false)
	{
		MessageBox(0, "Failed to Load File", 0,0);
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();
	if( !pRoot)
	{
		MessageBox(0, "Failed to load XML Root", 0,0);
		return false;
	}

#pragma region Gamer_Profiles

	TiXmlElement* pGamerProfile = pRoot->FirstChildElement("Gamer_Profile");

	while( pGamerProfile )
	{
		Gamer_Profile* temp = new Gamer_Profile();

		temp->m_sUserName = pGamerProfile->GetText();

		m_vUserProfiles.push_back(temp);

		pGamerProfile = pGamerProfile->NextSiblingElement();
	}
#pragma endregion

	return true;
}

bool CGamerProfile::SaveProfiles(const char* szXmlFileName)
{
	TiXmlDocument doc;

	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0", "utf-8", "");

	// Step1: Cut a hole in the box
	// Step2: Put your dec in the box
	doc.LinkEndChild(pDec);

	TiXmlElement* pRoot = new TiXmlElement("Gamer_Profiles");
	doc.LinkEndChild(pRoot);

	for( unsigned int i = 0; i < m_vUserProfiles.size(); i++)
	{
/////////////////////////////////////////////////////////////////////
		TiXmlElement* pProfile = new TiXmlElement("Gamer_Profile");

		TiXmlText* pText = new TiXmlText(m_vUserProfiles[i]->m_sUserName.c_str());
		pProfile->LinkEndChild(pText);

		pRoot->LinkEndChild(pProfile);
/////////////////////////////////////////////////////////////////////
	}

	if(doc.SaveFile(szXmlFileName) == false)
		return false;

	return true;
}


bool CGamerProfile::HandleEnter(void)
{
	switch(CurrPos)
	{
	case 200:
		activeProfile = m_vUserProfiles[0];
		break;
	case 300:
		activeProfile = m_vUserProfiles[1];
		break;
	case 400:
		activeProfile = m_vUserProfiles[2];
		break;
	case 500:
		if( !activeProfile )
		{
			MessageBox(0, "Error no profile selected", 0, 0);
			break;
		}

		editProfile = true;
		break;
	}

	return true;
}
