#include "CGamerProfile.h"
#include <string>
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
#include "CKeyBinds.h"
#include "CKeyboardKeyBinds.h"

CGamerProfile* CGamerProfile::instance = NULL;

CGamerProfile::CGamerProfile()
{
	m_pD3D  = NULL;
	m_pDI   = NULL;
	m_pTM   = NULL;
	m_pFM   = NULL;
	m_pPF   = NULL;
	editProfile = false;

	PosX = 0;
	PosX = (int)(CGame::GetInstance()->GetScreenWidth() * 0.3f);

	for( int i = 0; i < 4; i++)
	{
		PosY[i] = (i + 1) * 100;;
	}
	PosY[4] = 500;

	CurrPos = 200;

	LoadProfiles("resource/data/gamer_profile.xml");
	m_bSave = false;

	activeProfile = new Gamer_Profile();
	activeProfile->operator()(*m_vUserProfiles[0]);
}

CGamerProfile::~CGamerProfile()
{
	for( unsigned int i = 0; i < m_vUserProfiles.size(); i++)
	{
		delete m_vUserProfiles[i];
	}

	m_vUserProfiles.clear();

	delete activeProfile;
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
	m_pPF  	 = new CPrintFont(m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0)));

	BGMusicID = m_pFM->LoadSound("Resource/Sounds/NeonCity.mp3", SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");

	m_pFM->SetVolume(BGMusicID, CGame::GetInstance()->getSoundBVolume());
	m_pFM->SetVolume(m_nMenuSelect, CGame::GetInstance()->getSoundAVolume());
	m_pFM->SetVolume(m_nMenuMove, CGame::GetInstance()->getSoundAVolume());


	temp = 'A';
	m_pFM->PlaySoundA(BGMusicID);
	m_nProfileState = LOADSAVE_STATE;
}

void CGamerProfile::Exit()
{
	SaveProfiles("resource/data/gamer_profile.xml");

	CGame::GetInstance()->SetSoundAVolume(activeProfile->m_sfx);
	CGame::GetInstance()->SetSoundBVolume(activeProfile->m_background);

	m_pFM->StopSound(BGMusicID);
	m_pFM->UnloadSound(BGMusicID);

	delete m_pPF;
}

bool CGamerProfile::Input()
{
		if(CGame::GetInstance()->ControllerInput())
		{
			XINPUT_STATE xState = CGame::GetInstance()->GetController1()->GetState();
			BYTE rTrig = xState.Gamepad.bRightTrigger;
			float x = xState.Gamepad.sThumbLX;
			float y = xState.Gamepad.sThumbLY;

				if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
				{
					
					if( m_nProfileState == LOADSAVE_STATE)
					{
						if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
						{
							CGame::GetInstance()->ResetInputDelay();
							m_pFM->PlaySoundA(m_nMenuMove);
							m_nEntrySelection--;
						}
						if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
						{
							CGame::GetInstance()->ResetInputDelay();
							m_pFM->PlaySoundA(m_nMenuMove);
							m_nEntrySelection++;
						}
						if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
						{
							CGame::GetInstance()->ResetInputDelay();
							HandleEnter();
						}
						if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
						{
							CGame::GetInstance()->ResetInputDelay();
							m_pFM->PlaySoundA(m_nMenuSelect);
							CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
						}
					}
					else if( m_nProfileState == EDITNAME_STATE )
					{
							if( xState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
							{
								CGame::GetInstance()->ResetInputDelay();
								if( activeProfile->m_sUserName.size() > 0)
								{
									activeProfile->m_sUserName.erase(activeProfile->m_sUserName.begin() + activeProfile->m_sUserName.size() - 1);
									m_pFM->PlaySoundA(m_nMenuMove);
								}
							}
							if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
							{
								CGame::GetInstance()->ResetInputDelay();
								m_pFM->PlaySoundA(m_nMenuSelect);
								activeProfile->m_sUserName += temp;
								temp = 'A';
							}
							if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
							{
								CGame::GetInstance()->ResetInputDelay();
								m_nProfileState = LOADSAVE_STATE;
								m_pFM->PlaySoundA(m_nMenuSelect);
							}
					}
					else if(m_nProfileState == SAVESLOT_STATE || m_nProfileState == LOADSLOT_STATE )
					{
						if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
						{
							CGame::GetInstance()->ResetInputDelay();
							CurrPos -= 100;
							m_pFM->PlaySoundA(m_nMenuMove);

						}
						if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
						{
							CGame::GetInstance()->ResetInputDelay();
							CurrPos += 100;
							m_pFM->PlaySoundA(m_nMenuMove);

						}
						if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
						{
							CGame::GetInstance()->ResetInputDelay();
							HandleEnter();
						}
						if( xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
						{
							CGame::GetInstance()->ResetInputDelay();
							m_nProfileState = LOADSAVE_STATE;
							m_pFM->PlaySoundA(m_nMenuSelect);
						}
					}
				}
				if(CGame::GetInstance()->GetThumbDelay() >= 0.15f)
				{
					

					if( m_nProfileState == LOADSAVE_STATE)
					{
						if( x < -16000 )
						{
							CGame::GetInstance()->ResetThumbDelay();
							m_nEntrySelection--;
							m_pFM->PlaySoundA(m_nMenuMove);

						}
						if( x > 16000 )
						{
							CGame::GetInstance()->ResetThumbDelay();
							m_nEntrySelection++;
							m_pFM->PlaySoundA(m_nMenuMove);

						}
					}
					if( m_nProfileState == EDITNAME_STATE )
					{
						if(x < 8000 && x > -8000 && y > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
						{
							CGame::GetInstance()->ResetThumbDelay();
							m_pFM->PlaySoundA(m_nMenuMove);
							temp++;
						}
						else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
						{
							CGame::GetInstance()->ResetThumbDelay();
							m_pFM->PlaySoundA(m_nMenuMove);
							temp--;
						}
					}
					else if(m_nProfileState == SAVESLOT_STATE || m_nProfileState == LOADSLOT_STATE )
					{
						if(x < 8000 && x > -8000 && y > 16000)
						{
							CGame::GetInstance()->ResetThumbDelay();
							m_pFM->PlaySoundA(m_nMenuMove);
							CurrPos -= 100;
						}
						else if(x < 8000 && x > -8000 && y < -16000)
						{
							CGame::GetInstance()->ResetThumbDelay();
							m_pFM->PlaySoundA(m_nMenuMove);
							CurrPos += 100;
						}
					}
				}
		}
		else
		{
			if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
			{

				if( m_nProfileState == LOADSAVE_STATE)
				{
					if(  m_pDI->KeyDown(DIK_LEFT)||m_pDI->JoystickGetLStickDirDown(DIR_LEFT))
					{
						m_nEntrySelection--;
						m_pFM->PlaySoundA(m_nMenuMove);
						CGame::GetInstance()->ResetInputDelay();
					}
					if(  m_pDI->KeyDown(DIK_RIGHT)||m_pDI->JoystickGetLStickDirDown(DIR_RIGHT))
					{
						m_nEntrySelection++;
						m_pFM->PlaySoundA(m_nMenuMove);
						CGame::GetInstance()->ResetInputDelay();
					}
					if( m_pDI->KeyDown(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
					{
						CGame::GetInstance()->ResetInputDelay();
						HandleEnter();
					}
					if( m_pDI->KeyDown(DIK_ESCAPE)||m_pDI->JoystickButtonPressed(1))
					{
						m_pFM->PlaySoundA(m_nMenuSelect);
						CGame::GetInstance()->ResetInputDelay();
						CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
					}
				}
				else if( m_nProfileState == EDITNAME_STATE )
				{
					if( m_pDI->KeyDown(DIK_BACKSPACE)||m_pDI->JoystickButtonPressed(1))
					{
						CGame::GetInstance()->ResetInputDelay();
						if( activeProfile->m_sUserName.size() > 0)
						{
							activeProfile->m_sUserName.erase(activeProfile->m_sUserName.begin() + activeProfile->m_sUserName.size() - 1);
							m_pFM->PlaySoundA(m_nMenuMove);
						}
					}
					if(  m_pDI->KeyDown(DIK_UP)||m_pDI->JoystickGetLStickDirDown(DIR_UP))
					{
						CGame::GetInstance()->ResetInputDelay();
						temp++;
						m_pFM->PlaySoundA(m_nMenuMove);
					}
					if(  m_pDI->KeyDown(DIK_DOWN)||m_pDI->JoystickGetLStickDirDown(DIR_DOWN))
					{
						CGame::GetInstance()->ResetInputDelay();
						temp--;
						m_pFM->PlaySoundA(m_nMenuMove);
					}

					if( m_pDI->KeyDown(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
					{
						CGame::GetInstance()->ResetInputDelay();
						m_pFM->PlaySoundA(m_nMenuSelect);
						activeProfile->m_sUserName += temp;
						temp = 'A';
					}

					if( m_pDI->KeyDown(DIK_ESCAPE)||m_pDI->JoystickButtonPressed(1))
					{
						CGame::GetInstance()->ResetInputDelay();
						m_pFM->PlaySoundA(m_nMenuSelect);
						m_nProfileState = LOADSAVE_STATE;
					}
				}
				else if(m_nProfileState == SAVESLOT_STATE || m_nProfileState == LOADSLOT_STATE )
				{
					if( m_pDI->KeyDown(DIK_UP))
					{
						CGame::GetInstance()->ResetInputDelay();
						CurrPos -= 100;
						m_pFM->PlaySoundA(m_nMenuMove);
					}
					if( m_pDI->KeyDown(DIK_DOWN))
					{
						CGame::GetInstance()->ResetInputDelay();
						CurrPos += 100;
						m_pFM->PlaySoundA(m_nMenuMove);
					}
					if( m_pDI->KeyDown(DIK_RETURN)||m_pDI->JoystickButtonPressed(0))
					{
						CGame::GetInstance()->ResetInputDelay();
						HandleEnter();
					}
					if( m_pDI->KeyDown(DIK_ESCAPE)||m_pDI->JoystickButtonPressed(1))
					{
						CGame::GetInstance()->ResetInputDelay();
						m_pFM->PlaySoundA(m_nMenuSelect);
						m_nProfileState = LOADSAVE_STATE;
					}
				
				}
			}
		}

		if( m_nEntrySelection < 0 )
			m_nEntrySelection = 2;
		if( m_nEntrySelection > 2)
			m_nEntrySelection = 0;

		if( temp < 64 )
			temp = 122;
		if( temp > 122 )
			temp = 64;

		if( CurrPos > 400 )
			CurrPos = 200;
		if( CurrPos < 200)
			CurrPos = 400;

		return true;
}

void CGamerProfile::Update(float fElapsedTime)
{
	m_pFM->Update();
}

void CGamerProfile::Render()
{
	m_pPF->Print("GAMER PROFILES", 100, PosY[0], 1.5f, D3DCOLOR_XRGB(255,255,0));

	if( m_nProfileState == LOADSAVE_STATE)
	{
		if( m_nEntrySelection == 0 )
		{
			m_pPF->Print("Save", (int)(CGame::GetInstance()->GetScreenWidth() * 0.2f),(int)(CGame::GetInstance()->GetScreenHeight()*0.4f), 1.0f, D3DCOLOR_XRGB(0,255,0));
			m_pPF->Print("Selecting this Option allows you to save your current profile. \nUse this option to save keybinds, sound settings, and Unlocked cars.", (int)(CGame::GetInstance()->GetScreenWidth() * 0.2f),(int)(CGame::GetInstance()->GetScreenHeight()*0.8f), 0.6f, D3DCOLOR_XRGB(0,255,0));
		}
		else																   
			m_pPF->Print("Save", (int)(CGame::GetInstance()->GetScreenWidth() * 0.2f), (int)(CGame::GetInstance()->GetScreenHeight()*0.4f), 0.8f, D3DCOLOR_XRGB(200,0,0));

		if( m_nEntrySelection == 1 )
		{
			m_pPF->Print("Load", (int)(CGame::GetInstance()->GetScreenWidth() * 0.4f),(int)( CGame::GetInstance()->GetScreenHeight()*0.4f), 1.0f, D3DCOLOR_XRGB(0,255,0));
			m_pPF->Print("Selecting this Option allows you to Load up a profile. \nUse this option to load keybinds, sound settings and unlocked cars.", (int)(CGame::GetInstance()->GetScreenWidth() * 0.2f),(int)(CGame::GetInstance()->GetScreenHeight()*0.8f), 0.6f, D3DCOLOR_XRGB(0,255,0));
		}
		else
			m_pPF->Print("Load", (int)(CGame::GetInstance()->GetScreenWidth() * 0.4f),(int)( CGame::GetInstance()->GetScreenHeight()*0.4f), 0.8f, D3DCOLOR_XRGB(200,0,0));

		if( m_nEntrySelection == 2 )
		{
			m_pPF->Print("Edit Name", (int)(CGame::GetInstance()->GetScreenWidth() * 0.6f), (int)(CGame::GetInstance()->GetScreenHeight() * 0.4f), 1.0f, D3DCOLOR_XRGB(0,255,0));
			m_pPF->Print("Selecting this Option allows you to Edit the current profiles name. \nUse this option to edit the profile name for highscore statistics.", (int)(CGame::GetInstance()->GetScreenWidth() * 0.2f),(int)(CGame::GetInstance()->GetScreenHeight()*0.8f), 0.6f, D3DCOLOR_XRGB(0,255,0));
		}
		else							   
			m_pPF->Print("Edit Name", (int)(CGame::GetInstance()->GetScreenWidth() * 0.6f), (int)(CGame::GetInstance()->GetScreenHeight() * 0.4f), 0.8f, D3DCOLOR_XRGB(200,0,0));

		char buffer[64];
		sprintf_s(buffer, "Current Profile: %s", activeProfile->m_sUserName.c_str());
		m_pPF->Print(buffer, (int)(CGame::GetInstance()->GetScreenWidth() * 0.1f), (int)(CGame::GetInstance()->GetScreenHeight() * 0.9f),0.8f, D3DCOLOR_XRGB(128,128,255));

		if(CGame::GetInstance()->ControllerInput())
			m_pPF->Print("B To Exit",40,(int)(CGame::GetInstance()->GetScreenHeight()-70),1.0f,D3DCOLOR_XRGB(255,0,0));
		else
			m_pPF->Print("ESC To Exit",(int)(CGame::GetInstance()->GetScreenWidth() * 0.7f),(int)(CGame::GetInstance()->GetScreenHeight()*0.7f),1.0f,D3DCOLOR_XRGB(255,0,0));

	}
	else if( m_nProfileState == LOADSLOT_STATE || m_nProfileState == SAVESLOT_STATE )
	{
		for( unsigned int i = 0; i < m_vUserProfiles.size(); i++)
		{
			if( CurrPos == PosY[i+1])
			{
				m_pPF->Print(m_vUserProfiles[i]->m_sUserName.c_str(), PosX, PosY[i+1], 1.0f, D3DCOLOR_XRGB(0,255,0));
			}
			else
				m_pPF->Print(m_vUserProfiles[i]->m_sUserName.c_str(), PosX, PosY[i+1], 0.5f, D3DCOLOR_XRGB(255,0,0));
		}

		int count =  m_vUserProfiles.size(); // The number of slots taken up to decide how many slots are available.

		while( count < 3 )
		{
			count++;

			if( CurrPos == PosY[count] )
			{
				m_pPF->Print("--Empty Slot--", PosX, PosY[count], 1.0f, D3DCOLOR_XRGB(0,255,0));
			}
			else
				m_pPF->Print("--Empty Slot--", PosX, PosY[count], 0.5f, D3DCOLOR_XRGB(200,0,0));
		}

		m_pPF->Print(activeProfile->m_sUserName.c_str(), (int)(CGame::GetInstance()->GetScreenWidth() * 0.1f), (int)(CGame::GetInstance()->GetScreenHeight() * 0.9f),0.8f, D3DCOLOR_XRGB(128,128,255));
	}
	else if( m_nProfileState == EDITNAME_STATE )
	{
		m_pPF->Print(activeProfile->m_sUserName.c_str(), PosX, PosY[2], 1.0f, D3DCOLOR_XRGB(128,128,255));
		string awesome;
		awesome += temp;
		m_pPF->Print(awesome.c_str(), PosX + (26 * activeProfile->m_sUserName.length()) , PosY[2], 1.0f, D3DCOLOR_XRGB(128,128,255));

		m_pPF->Print(activeProfile->m_sUserName.c_str(), (int)(CGame::GetInstance()->GetScreenWidth() * 0.1f), (int)(CGame::GetInstance()->GetScreenHeight() * 0.9f),0.8f, D3DCOLOR_XRGB(128,128,255));
	}
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
	int ind = 0;
	while( pGamerProfile )
	{
		Gamer_Profile* temp = new Gamer_Profile();

		temp->m_sUserName = pGamerProfile->GetText();
		TiXmlElement* Sounds = pGamerProfile->FirstChildElement("Sounds");
		TiXmlElement* KB = pGamerProfile->FirstChildElement("Controller_Keybinds");
		TiXmlElement* Cars = pGamerProfile->FirstChildElement("cars");

		double sfx;
		double bg;
		Sounds->Attribute("SFX",&sfx);
		Sounds->Attribute("Background",&bg);
		temp->m_sfx = (float)sfx/100.0f;
		temp->m_background = (float)bg/100.0f;
		
		CKeyBinds* tempkb = new CKeyBinds();
		CKeyboardKeyBinds* tempkkb = new CKeyboardKeyBinds();
		int shoot;
		int changeweapon;

		int kshoot;
		int kchangeweapon;
		int kforward;
		int kbackward;
		int kleft;
		int kright;

		KB->Attribute("shoot",&shoot);
		KB->Attribute("change_weapon",&changeweapon);
		KB->Attribute("kshoot",&kshoot);
		KB->Attribute("kchange_weapon",&kchangeweapon);
		KB->Attribute("kaccelerate",&kforward);
		KB->Attribute("kbreak",&kbackward);
		KB->Attribute("kleft",&kleft);
		KB->Attribute("kright",&kright);
		tempkb->SetShoot(shoot);
		tempkb->SetChangeWeapon(changeweapon);
		tempkkb->SetShoot(kshoot);
		tempkkb->SetChangeWeapon(kchangeweapon);
		tempkkb->SetForward(kforward);
		tempkkb->SetBackward(kbackward);
		tempkkb->SetLeft(kleft);
		tempkkb->SetRight(kright);
		temp->m_pKB = tempkb;
		temp->m_pKKB = tempkkb;

		int mini;
		int vette;
		int humvee;
		int truck;

		Cars->Attribute("mini",&mini);
		Cars->Attribute("vette",&vette);
		Cars->Attribute("humvee",&humvee);
		Cars->Attribute("truck",&truck);

		temp->cars[0] = mini;
		temp->cars[1] = vette;
		temp->cars[2] = humvee;
		temp->cars[3] = truck;
		temp->index = ind;
		ind++;
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

		if( m_vUserProfiles[i]->m_pKB )
		{
			int shoot = m_vUserProfiles[i]->m_pKB->GetShoot();
			int change_weapon = m_vUserProfiles[i]->m_pKB->GetChangeWeapon();

			TiXmlElement* pKB = new TiXmlElement("Controller_Keybinds");
			pKB->SetAttribute("shoot",shoot);
			pKB->SetAttribute("change_weapon",change_weapon);
		

			if( m_vUserProfiles[i]->m_pKKB )
			{
				int kshoot = m_vUserProfiles[i]->m_pKKB->GetShoot();

				int forward = m_vUserProfiles[i]->m_pKKB->Getforward();
				int backward = m_vUserProfiles[i]->m_pKKB->Getbackward();
				int left = m_vUserProfiles[i]->m_pKKB->GetLeft();
				int right = m_vUserProfiles[i]->m_pKKB->GetRight();
				int kchange_weapon = m_vUserProfiles[i]->m_pKKB->GetChangeWeapon();
				pKB->SetAttribute("kshoot",kshoot);
		
				pKB->SetAttribute("kaccelerate",forward);
				pKB->SetAttribute("kbreak",backward);
				pKB->SetAttribute("kleft",left);
				pKB->SetAttribute("kright",right);
				pKB->SetAttribute("kchange_weapon", kchange_weapon);
			}

			pProfile->LinkEndChild(pKB);
		}
		
		TiXmlElement* pSounds = new TiXmlElement("Sounds");
		float sfx;
		float background;
		sfx = m_vUserProfiles[i]->m_sfx * 100;
		background = m_vUserProfiles[i]->m_background * 100;
		TiXmlElement* pCars = new TiXmlElement("cars");
		pCars->SetAttribute("mini",m_vUserProfiles[i]->cars[0]);
		pCars->SetAttribute("vette",m_vUserProfiles[i]->cars[1]);
		pCars->SetAttribute("humvee",m_vUserProfiles[i]->cars[2]);
		pCars->SetAttribute("truck",m_vUserProfiles[i]->cars[3]);
		pProfile->LinkEndChild(pCars);

		pSounds->SetAttribute("SFX",(int)sfx);
		pSounds->SetAttribute("Background",(int)background);
		pProfile->LinkEndChild(pSounds);
		
		pRoot->LinkEndChild(pProfile);
/////////////////////////////////////////////////////////////////////
	}

	if(doc.SaveFile(szXmlFileName) == false)
		return false;

	return true;
}


bool CGamerProfile::HandleEnter(void)
{
	m_pFM->PlaySoundA(m_nMenuSelect);
	if( m_nProfileState == LOADSAVE_STATE )
	{
		switch( m_nEntrySelection )
		{
		case 0:
			m_nProfileState = SAVESLOT_STATE;
			m_bSave = true;
			break;
		case 1:
			m_nProfileState = LOADSLOT_STATE;
			m_bSave = false;
			break;
		case 2:
			m_nProfileState = EDITNAME_STATE;
			break;
		}
	}
	else if( m_nProfileState == LOADSLOT_STATE )
	{
		switch(CurrPos)
		{
		case 200:
			delete activeProfile;
			activeProfile = new Gamer_Profile();
			activeProfile->operator()(*m_vUserProfiles[0]);
			activeProfile->index = 0;
			CGame::GetInstance()->SetPlayerProfile(activeProfile);
			break;
		case 300:
			delete activeProfile;
			activeProfile = new Gamer_Profile();
			activeProfile->operator()(*m_vUserProfiles[1]);
			activeProfile->index = 1;
			CGame::GetInstance()->SetPlayerProfile(activeProfile);
			break;
		case 400:
			delete activeProfile;
			activeProfile = new Gamer_Profile();
			activeProfile->operator()(*m_vUserProfiles[2]);
			activeProfile->index = 2;
			CGame::GetInstance()->SetPlayerProfile(activeProfile);
			break;
		}
	}
	else if( m_nProfileState = SAVESLOT_STATE )
	{
		switch( CurrPos)
		{
		case 200:
			if( m_vUserProfiles.size() >= 1 )
			{
				int result = MessageBox(CGame::GetInstance()->GetHWND(),"Are you sure you want to overwrite the current saved file?", "Are You Sure?", MB_YESNO | MB_ICONWARNING);
				if( result == IDYES )
				{
					delete m_vUserProfiles[0];
					m_vUserProfiles[0] = SaveProfile();
				}
			}
			else
				m_vUserProfiles.push_back(SaveProfile());
			break;
		case 300:
			if( m_vUserProfiles.size() >= 2 )
			{
				int result = MessageBox(CGame::GetInstance()->GetHWND(),"Are you sure you want to overwrite the current saved file?", "Are You Sure?", MB_YESNO | MB_ICONWARNING);
				if( result == IDYES )
				{
					delete m_vUserProfiles[1];
					m_vUserProfiles[1] = SaveProfile();
				}
			}
			else
				m_vUserProfiles.push_back(SaveProfile());
			break;
		case 400:
			if( m_vUserProfiles.size() >= 3 )
			{
				int result = MessageBox(CGame::GetInstance()->GetHWND(),"Are you sure you want to overwrite the current saved file?", "Are You Sure?", MB_YESNO | MB_ICONWARNING);
				if( result == IDYES )
				{
					delete m_vUserProfiles[2];
					m_vUserProfiles[2] = SaveProfile();
				}
			}
			else
				m_vUserProfiles.push_back(SaveProfile());
			break;
		}
	}

	return true;
}

Gamer_Profile* CGamerProfile::SaveProfile()
{
	Gamer_Profile* profile_to_save = new Gamer_Profile();
	profile_to_save->operator()(*activeProfile);
	profile_to_save->m_sfx = CGame::GetInstance()->getSoundAVolume();
	profile_to_save->m_background = CGame::GetInstance()->getSoundBVolume();

	return profile_to_save;
}

void CGamerProfile::SaveWinnerCar(int index)
{
	Gamer_Profile* profile_to_save = new Gamer_Profile();
	profile_to_save->operator()(*activeProfile);
	profile_to_save->m_sfx = CGame::GetInstance()->getSoundAVolume();
	profile_to_save->m_background = CGame::GetInstance()->getSoundBVolume();
	delete m_vUserProfiles[index];
	m_vUserProfiles[index] = profile_to_save;
	CGamerProfile::GetInstance()->SaveProfiles("resource/data/gamer_profile.xml");
}

