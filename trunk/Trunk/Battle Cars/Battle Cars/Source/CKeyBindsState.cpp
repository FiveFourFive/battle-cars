#include "CKeyBindsState.h"
#include "CMainMenuState.h"
#include "CGame.h"
#include "CPrintFont.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_FModManager.h"
#include "CSGD_DirectInput.h"
#include "CGamerProfile.h"
#include "Gamer_Profile.h"
#include "CXboxInput.h"
#include "CKeyBinds.h"
#include "CKeyboardKeyBinds.h"
enum options{WS_SHOOT, WS_CHANGE_WEAPON ,WS_FORWARD,WS_BACKWARD,WS_LEFT,WS_RIGHT,WS_EXIT};
CKeyBindsState::CKeyBindsState(void)
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pPF = NULL;
	m_pDI = NULL;
	m_pFM = NULL;
	m_pController = NULL;
}

CKeyBindsState::~CKeyBindsState(void)
{

}

CKeyBindsState* CKeyBindsState::GetInstance(void)
{
	static CKeyBindsState instance;
	return &instance;
}

void CKeyBindsState::Enter(void)
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nFontID = m_pTM->LoadTexture("resource/graphics/BC_Font.png",D3DCOLOR_XRGB(0, 0, 0));

	m_pPF = new CPrintFont(m_nFontID);
	m_pFM = CSGD_FModManager::GetInstance();
	m_nBackgroundMusicID = m_pFM->LoadSound("resource/sounds/FeelSoNumb.mp3",SGD_FMOD_LOOPING);
	m_nMenuSelect = m_pFM->LoadSound("resource/sounds/menuselect.mp3");
	m_nMenuMove = m_pFM->LoadSound("resource/sounds/menuchange.mp3");
	m_nSoundA = m_pFM->LoadSound("resource/sounds/bullet1.mp3");
	m_nSoundB = CMainMenuState::GetInstance()->GetBackgroundMusicID();
	m_nSelection = 0;
	m_nSelected = 0;

	m_pKB = CGamerProfile::GetInstance()->GetActiveProfile()->m_pKB;
	m_pKeyboardKB = CGamerProfile::GetInstance()->GetActiveProfile()->m_pKKB;
	if(CGame::GetInstance()->ControllerInput())
		m_nMaxOptions = 2;
	else
		m_nMaxOptions = 6;
}

bool CKeyBindsState::Input(void)
{
	if(CGame::GetInstance()->ControllerInput())
		m_nMaxOptions = 2;
	else
		m_nMaxOptions = 6;
	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = m_pController->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		CKeyBinds* tempkeys = m_pController->GetKB();
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & m_pKB->GetAccept())
			{
				HandleEnter();
			}
		if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			
			switch(m_nSelection)
			{
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_A);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_A);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			switch(m_nSelection)
			{
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_B);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_B);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			switch(m_nSelection)
			{

			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_Y);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_Y);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			switch(m_nSelection)
			{
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_X);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_X);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			switch(m_nSelection)
			{
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_LEFT_SHOULDER);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_LEFT_SHOULDER);
				break;

			}
		}
		else if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			switch(m_nSelection)
			{
			case WS_SHOOT:
				m_pKB->SetShoot(XINPUT_GAMEPAD_RIGHT_SHOULDER);
				break;
			case WS_CHANGE_WEAPON:
				m_pKB->SetChangeWeapon(XINPUT_GAMEPAD_RIGHT_SHOULDER);
				break;

			}
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
				m_nSelection = m_nMaxOptions;
		}
		else if(x < 8000 && x > -8000 && y < -16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			m_nSelection++;
			m_pFM->PlaySound(m_nMenuMove);
			if(m_nSelection > m_nMaxOptions)
				m_nSelection = 0;
		}
		}

	}
	else
	{
		if(m_bSelected)
			m_bSelected = SendKeyBinds();
		else
		{
		if(m_pDI->KeyPressed(DIK_RETURN))
		{
			m_nSelected = 255;
			if(m_nSelection != WS_EXIT)
				m_bSelected = true;
		}
		
	if(m_pDI->KeyPressed(DIK_ESCAPE) && !m_bSelected)
	{
		CGame::GetInstance()->RemoveState(this);
	}
	if(m_pDI->KeyPressed(DIK_RETURN) && !m_bSelected)
	{
		return this->HandleEnter();
	}
	if(m_pDI->KeyPressed(DIK_UP) && !m_bSelected)
	{
		m_nSelection--;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection < 0)
			m_nSelection = m_nMaxOptions;
	}

	if(m_pDI->KeyPressed(DIK_DOWN) && !m_bSelected)
	{
		m_nSelection++;
		m_pFM->PlaySound(m_nMenuMove);
		if(m_nSelection > m_nMaxOptions)
			m_nSelection = 0;
	}
	
	}

}
	return true;
}

void CKeyBindsState::Update(float fElapsedTime)
{
	m_pFM->Update();
	m_fDelay += fElapsedTime;
}

void CKeyBindsState::Render(void)
{
	RECT temp;					 ////
	temp.left = 0;				 //////
	temp.top = 0;				 // replace this with background image
	temp.right = 800;			 ////////
	temp.bottom = 600;			 //////
	m_pD3D->GetSprite()->Flush();////
	m_pD3D->DrawRect(temp,0,0,0);//
	
	m_pPF->Print("KEY BINDINGS",220,50,1.0f,D3DCOLOR_XRGB(200, 0, 0));
	
	 
	m_pPF->Print("INPUT DEVICE",150,120,0.5f,D3DCOLOR_XRGB(200, 0, 0));	

	if(CGame::GetInstance()->ControllerInput())
	{
		m_pPF->Print("SHOOT", 150,300,0.5f,D3DCOLOR_XRGB(255,0,0));
		m_pPF->Print("CHANGE WEAPON", 150,350,0.5f,D3DCOLOR_XRGB(255,0,0));
		m_pPF->Print("GAMEPAD",370,120,0.5f,D3DCOLOR_XRGB(255,255,255));
		m_pPF->Print("EXIT",150,400,0.5f,D3DCOLOR_XRGB(255,0,0));
		
		switch(m_pKB->GetShoot())
		{
		case XINPUT_GAMEPAD_A:
			m_pPF->Print("A",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_X:
			m_pPF->Print("X",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_Y:
			m_pPF->Print("Y",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_B:
			m_pPF->Print("B",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:
			m_pPF->Print("LEFT BUMPER",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER:
			m_pPF->Print("RIGHT BUMPER",400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;

		default:

			break;
		}
		switch(m_pKB->GetChangeWeapon())
		{
		case XINPUT_GAMEPAD_A:
			m_pPF->Print("A",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_X:
			m_pPF->Print("X",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_Y:
			m_pPF->Print("Y",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_B:
			m_pPF->Print("B",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_LEFT_SHOULDER:
			m_pPF->Print("LEFT BUMPER",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		case XINPUT_GAMEPAD_RIGHT_SHOULDER:
			m_pPF->Print("RIGHT BUMPER",400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
			break;
		default:

			break;
		}

	switch(m_nSelection)
	{
	case WS_SHOOT:
		m_pPF->Print("SHOOT", 150,300,0.5f,D3DCOLOR_XRGB(0,255,0));
			
		break;
	case WS_CHANGE_WEAPON:
		m_pPF->Print("CHANGE WEAPON", 150,350,0.5f,D3DCOLOR_XRGB(0,255,0));
		break;
	case 2:
		m_pPF->Print("EXIT", 150,400,0.5f,D3DCOLOR_XRGB(0,255,0));
		break;
	}
	}
	else
	{
		m_pPF->Print("KEYBOARD",370,120,0.5f,D3DCOLOR_XRGB(255,255,255));
	m_pPF->Print("SHOOT", 150,250,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("CHANGE WEAPON", 150,300,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("ACCELERATE",150,350,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("BREAK",150,400,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("TURN LEFT",150,450,0.5f,D3DCOLOR_XRGB(255,0,0));
	m_pPF->Print("TURN RIGHT",150,500,0.5f,D3DCOLOR_XRGB(255,0,0));

		m_pPF->Print("EXIT",150,550,0.5f,D3DCOLOR_XRGB(255,0,0));
		char buffer[128];
		strcpy_s(buffer,_countof(buffer),GetKeyBound(m_pKeyboardKB->GetShoot()));
		m_pPF->Print(buffer,400,250,0.5f,D3DCOLOR_XRGB(255,255,255));
		strcpy_s(buffer,_countof(buffer),GetKeyBound(m_pKeyboardKB->GetChangeWeapon()));
		m_pPF->Print(buffer,400,300,0.5f,D3DCOLOR_XRGB(255,255,255));
		strcpy_s(buffer,_countof(buffer),GetKeyBound(m_pKeyboardKB->Getforward()));
		m_pPF->Print(buffer,400,350,0.5f,D3DCOLOR_XRGB(255,255,255));
		strcpy_s(buffer,_countof(buffer),GetKeyBound(m_pKeyboardKB->Getbackward()));
		m_pPF->Print(buffer,400,400,0.5f,D3DCOLOR_XRGB(255,255,255));
		strcpy_s(buffer,_countof(buffer),GetKeyBound(m_pKeyboardKB->GetLeft()));
		m_pPF->Print(buffer,400,450,0.5f,D3DCOLOR_XRGB(255,255,255));
		strcpy_s(buffer,_countof(buffer),GetKeyBound(m_pKeyboardKB->GetRight()));
		m_pPF->Print(buffer,400,500,0.5f,D3DCOLOR_XRGB(255,255,255));

	switch(m_nSelection)
	{
	case WS_SHOOT:
		m_pPF->Print("SHOOT", 150,250,0.5f,D3DCOLOR_XRGB(0,255-m_nSelected,m_nSelected));
		break;
	case WS_CHANGE_WEAPON:
		m_pPF->Print("CHANGE WEAPON", 150,300,0.5f,D3DCOLOR_XRGB(0,255-m_nSelected,m_nSelected));
		break;
	case WS_FORWARD:
		m_pPF->Print("ACCELERATE", 150,350,0.5f,D3DCOLOR_XRGB(0,255-m_nSelected,m_nSelected));
		break;
	case WS_BACKWARD:
		m_pPF->Print("BREAK", 150,400,0.5f,D3DCOLOR_XRGB(0,255-m_nSelected,m_nSelected));
		break;
	case WS_LEFT:
		m_pPF->Print("TURN LEFT", 150,450,0.5f,D3DCOLOR_XRGB(0,255-m_nSelected,m_nSelected));
		break;
	case WS_RIGHT:
		m_pPF->Print("TURN RIGHT", 150,500,0.5f,D3DCOLOR_XRGB(0,255-m_nSelected,m_nSelected));
		break;
	case WS_EXIT:
		m_pPF->Print("EXIT", 150,550,0.5f,D3DCOLOR_XRGB(0,255-m_nSelected,m_nSelected));
		break;
	}

	}

}

void CKeyBindsState::Exit(void)
{
	m_pTM->UnloadTexture(m_nFontID);
	delete m_pPF;
	CGamerProfile::GetInstance()->GetActiveProfile()->m_pKKB = m_pKeyboardKB;
	CGamerProfile::GetInstance()->GetActiveProfile()->m_pKB = m_pKB;
	//CGame::GetInstance()->SetKB(m_pKeyboardKB);
}

bool CKeyBindsState::HandleEnter(void)
{
	if(CGame::GetInstance()->ControllerInput())
	{
		if(m_nSelection == 2)
		{
			CGame::GetInstance()->RemoveState(this);
			m_pFM->PlaySound(m_nMenuSelect);
		}
	}
	else
	{
		if(m_nSelection == WS_EXIT)
		{
			CGame::GetInstance()->RemoveState(this);
			m_pFM->PlaySound(m_nMenuSelect);
		}
	}
	return true;
}
bool CKeyBindsState::SendKeyBinds(void)
{
	if(m_pDI->KeyPressed(DIK_RETURN))
		{
		return SetKeyBind(DIK_RETURN);
		
		}
	else if( m_pDI->KeyPressed(DIK_ESCAPE))
		{
		return SetKeyBind(DIK_ESCAPE);
		
		}
	else if( m_pDI->KeyPressed(DIK_A))
		{
		return SetKeyBind(DIK_A);
		
		}
	else if( m_pDI->KeyPressed(DIK_B))
		{
		return SetKeyBind(DIK_B);
		
		}
	else if( m_pDI->KeyPressed(DIK_C))
		{
		return SetKeyBind(DIK_C);
		
		}
	else if( m_pDI->KeyPressed(DIK_D))
		{
		return SetKeyBind(DIK_D);
		
		}
	else if( m_pDI->KeyPressed(DIK_E))
		{
		return SetKeyBind(DIK_E);
		
		}
	else if( m_pDI->KeyPressed(DIK_F))
		{
		return SetKeyBind(DIK_F);
		
		}
	else if( m_pDI->KeyPressed(DIK_G))
		{
		return SetKeyBind(DIK_G);
		
		}
	else if( m_pDI->KeyPressed(DIK_H))
		{
		return SetKeyBind(DIK_H);
		
		}
	else if( m_pDI->KeyPressed(DIK_I))
		{
		return SetKeyBind(DIK_I);
		
		}
	else if( m_pDI->KeyPressed(DIK_J))
		{
		return SetKeyBind(DIK_J);
		
		}
	else if( m_pDI->KeyPressed(DIK_K))
		{
		return SetKeyBind(DIK_K);
		
		}
	else if( m_pDI->KeyPressed(DIK_L))
		{
		return SetKeyBind(DIK_L);
		
		}
	else if( m_pDI->KeyPressed(DIK_M))
		{
		return SetKeyBind(DIK_M);
		
		}
	else if( m_pDI->KeyPressed(DIK_N))
		{
		return SetKeyBind(DIK_N);
		
		}
	else if( m_pDI->KeyPressed(DIK_O))
		{
		return SetKeyBind(DIK_O);
		
		}
	else if( m_pDI->KeyPressed(DIK_P))
		{
		return SetKeyBind(DIK_P);
		
		}
	else if( m_pDI->KeyPressed(DIK_Q))
		{
		return SetKeyBind(DIK_Q);
		
		}
	else if( m_pDI->KeyPressed(DIK_R))
		{
		return SetKeyBind(DIK_R);
		
		}
	else if( m_pDI->KeyPressed(DIK_S))
		{
		return SetKeyBind(DIK_S);
		
		}
	else if( m_pDI->KeyPressed(DIK_T))
		{
		return SetKeyBind(DIK_T);
		
		}
	else if( m_pDI->KeyPressed(DIK_U))
		{
		return SetKeyBind(DIK_U);
		
		}
	else if( m_pDI->KeyPressed(DIK_V))
		{
		return SetKeyBind(DIK_V);
		
		}
	else if( m_pDI->KeyPressed(DIK_W))
		{
		return SetKeyBind(DIK_W);
		
		}
	else if( m_pDI->KeyPressed(DIK_X))
		{
		return SetKeyBind(DIK_X);
		
		}
	else if( m_pDI->KeyPressed(DIK_Y))
		{
		return SetKeyBind(DIK_Y);
		
		}
	else if( m_pDI->KeyPressed(DIK_Z))
		{
		return SetKeyBind(DIK_Z);
		
		}
	else if( m_pDI->KeyPressed(DIK_UP))
		{
		return SetKeyBind(DIK_UP);
		
		}
	else if( m_pDI->KeyPressed(DIK_DOWN))
		{
		return SetKeyBind(DIK_DOWN);
		
		}
	else if( m_pDI->KeyPressed(DIK_LEFT))
		{
		return SetKeyBind(DIK_LEFT);
		
		}
	else if( m_pDI->KeyPressed(DIK_RIGHT))
		{
		return SetKeyBind(DIK_RIGHT);
		
		}
	else if( m_pDI->KeyPressed(DIK_LCONTROL))
		{
		return SetKeyBind(DIK_LCONTROL);
		
		}
	else if( m_pDI->KeyPressed(DIK_RCONTROL))
		{
		return SetKeyBind(DIK_RCONTROL);
		
		}
	else if( m_pDI->KeyPressed(DIK_SPACE))
		{
		return SetKeyBind(DIK_SPACE);	
		
		}
	else if( m_pDI->KeyPressed(DIK_LSHIFT))
		{
			return SetKeyBind(DIK_LSHIFT);
			
		}
	else if( m_pDI->KeyPressed(DIK_RSHIFT))
		{
			return SetKeyBind(DIK_RSHIFT);
			
		}
	else if( m_pDI->KeyPressed(DIK_TAB))
		{
			return SetKeyBind(DIK_TAB);
			
		}
	else if( m_pDI->KeyPressed(DIK_1))
		{
			return SetKeyBind(DIK_1);
			
		}
	else if( m_pDI->KeyPressed(DIK_2))
		{
			return SetKeyBind(DIK_2);
			
		}
	else if( m_pDI->KeyPressed(DIK_3))
		{
			return SetKeyBind(DIK_3);
			
		}
	else if( m_pDI->KeyPressed(DIK_4))
		{
			return SetKeyBind(DIK_4);
			
		}
	else if( m_pDI->KeyPressed(DIK_5))
		{
			return SetKeyBind(DIK_5);
			
		}
	else if( m_pDI->KeyPressed(DIK_6))
		{
			return SetKeyBind(DIK_6);
			
		}
	else if( m_pDI->KeyPressed(DIK_7))
		{
			return SetKeyBind(DIK_7);
			
		}
	else if( m_pDI->KeyPressed(DIK_8))
		{
			return SetKeyBind(DIK_8);
			
		}
	else if( m_pDI->KeyPressed(DIK_9))
		{
			return SetKeyBind(DIK_9);
			
		}
	else if( m_pDI->KeyPressed(DIK_0))
		{
			return SetKeyBind(DIK_0);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD1))
		{
			return SetKeyBind(DIK_NUMPAD1);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD2))
		{
			return SetKeyBind(DIK_NUMPAD2);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD3))
		{
			return SetKeyBind(DIK_NUMPAD3);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD4))
		{
			return SetKeyBind(DIK_NUMPAD4);
			
		}
	else if(m_pDI->KeyPressed(DIK_NUMPAD6))
		{
			return SetKeyBind(DIK_NUMPAD6);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD6))
		{
			return SetKeyBind(DIK_NUMPAD6);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD7))
		{
			return SetKeyBind(DIK_NUMPAD7);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD8))
		{
			return SetKeyBind(DIK_NUMPAD8);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD9))
		{
			return SetKeyBind(DIK_NUMPAD9);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPAD0))
		{
			return SetKeyBind(DIK_NUMPAD0);
			
		}
	else if( m_pDI->KeyPressed(DIK_NUMPADENTER))
		{
			return SetKeyBind(DIK_NUMPADENTER);
			
		}
	m_nSelected = 0;
	return true;
}
char* CKeyBindsState::GetKeyBound(int keycode)
{
	ZeroMemory(buffer,128);
	switch(keycode)
	{
	case DIK_RETURN:
		{
		strcpy_s(buffer,_countof(buffer),"ENTER");
		break;
		}
	case DIK_ESCAPE:
		{
		strcpy_s(buffer,_countof(buffer),"ESC");
		break;
		}
	case DIK_A:
		{
		strcpy_s(buffer,_countof(buffer),"A");	
		break;
		}
	case DIK_B:
		{
		strcpy_s(buffer,_countof(buffer),"B");	
		break;
		}
	case DIK_C:
		{
		strcpy_s(buffer,_countof(buffer),"C");	
		break;
		}
	case DIK_D:
		{
		strcpy_s(buffer,_countof(buffer),"D");	
		break;
		}
	case DIK_E:
		{
		strcpy_s(buffer,_countof(buffer),"E");	
		break;
		}
	case DIK_F:
		{
		strcpy_s(buffer,_countof(buffer),"F");	
		break;
		}
	case DIK_G:
		{
		strcpy_s(buffer,_countof(buffer),"G");	
		break;
		}
	case DIK_H:
		{
		strcpy_s(buffer,_countof(buffer),"H");	
		break;
		}
	case DIK_I:
		{
		strcpy_s(buffer,_countof(buffer),"I");	
		break;
		}
	case DIK_J:
		{
		strcpy_s(buffer,_countof(buffer),"J");	
		break;
		}
	case DIK_K:
		{
		strcpy_s(buffer,_countof(buffer),"K");	
		break;
		}
	case DIK_L:
		{
		strcpy_s(buffer,_countof(buffer),"L");	
		break;
		}
	case DIK_M:
		{
		strcpy_s(buffer,_countof(buffer),"M");	
		break;
		}
	case DIK_N:
		{
		strcpy_s(buffer,_countof(buffer),"N");	
		break;
		}
	case DIK_O:
		{
		strcpy_s(buffer,_countof(buffer),"O");	
		break;
		}
	case DIK_P:
		{
		strcpy_s(buffer,_countof(buffer),"P");	
		break;
		}
	case DIK_Q:
		{
		strcpy_s(buffer,_countof(buffer),"Q");	
		break;
		}
	case DIK_R:
		{
		strcpy_s(buffer,_countof(buffer),"R");	
		break;
		}
	case DIK_S:
		{
		strcpy_s(buffer,_countof(buffer),"S");	
		break;
		}
	case DIK_T:
		{
		strcpy_s(buffer,_countof(buffer),"T");	
		break;
		}
	case DIK_U:
		{
		strcpy_s(buffer,_countof(buffer),"U");	
		break;
		}
	case DIK_V:
		{
		strcpy_s(buffer,_countof(buffer),"V");	
		break;
		}
	case DIK_W:
		{
		strcpy_s(buffer,_countof(buffer),"W");	
		break;
		}
	case DIK_X:
		{
		strcpy_s(buffer,_countof(buffer),"X");	
		break;
		}
	case DIK_Y:
		{
		strcpy_s(buffer,_countof(buffer),"Y");	
		break;
		}
	case DIK_Z:
		{
		strcpy_s(buffer,_countof(buffer),"Z");	
		break;
		}
	case DIK_UP:
		{
		strcpy_s(buffer,_countof(buffer),"UP ARROW");	
		break;
		}
	case DIK_DOWN:
		{
		strcpy_s(buffer,_countof(buffer),"DOWN ARROW");	
		break;
		}
	case DIK_LEFT:
		{
		strcpy_s(buffer,_countof(buffer),"LEFT ARROW");	
		break;
		}
	case DIK_RIGHT:
		{
		strcpy_s(buffer,_countof(buffer),"RIGHT ARROW");	
		break;
		}
	case DIK_LCONTROL:
		{
			strcpy_s(buffer,_countof(buffer),"LEFT CONTROL");	
			break;
		}
	case DIK_RCONTROL:
		{
			strcpy_s(buffer,_countof(buffer),"RIGHT CONTROL");	
			break;
		}
	case DIK_SPACE:
		{
			strcpy_s(buffer,_countof(buffer),"SPACEBAR");	
			break;
		}
	case DIK_LSHIFT:
		{
			strcpy_s(buffer,_countof(buffer),"LEFT SHIFT");	
			break;
		}
	case DIK_RSHIFT:
		{
			strcpy_s(buffer,_countof(buffer),"RIGHT SHIFT");	
			break;
		}
	case DIK_TAB:
		{
			strcpy_s(buffer,_countof(buffer),"TAB");	
			break;
		}
	case DIK_1:
		{
			strcpy_s(buffer,_countof(buffer),"1");	
			break;
		}
	case DIK_2:
		{
			strcpy_s(buffer,_countof(buffer),"2");	
			break;
		}
	case DIK_3:
		{
			strcpy_s(buffer,_countof(buffer),"3");	
			break;
		}
	case DIK_4:
		{
			strcpy_s(buffer,_countof(buffer),"4");	
			break;
		}
	case DIK_5:
		{
			strcpy_s(buffer,_countof(buffer),"5");	
			break;
		}
	case DIK_6:
		{
			strcpy_s(buffer,_countof(buffer),"6");	
			break;
		}
	case DIK_7:
		{
			strcpy_s(buffer,_countof(buffer),"7");	
			break;
		}
	case DIK_8:
		{
			strcpy_s(buffer,_countof(buffer),"8");	
			break;
		}
	case DIK_9:
		{
			strcpy_s(buffer,_countof(buffer),"9");	
			break;
		}
	case DIK_0:
		{
			strcpy_s(buffer,_countof(buffer),"0");	
			break;
		}
	case DIK_NUMPAD1:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 1");	
			break;
		}
	case DIK_NUMPAD2:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 2");	
			break;
		}
	case DIK_NUMPAD3:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 3");	
			break;
		}
	case DIK_NUMPAD4:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 4");	
			break;
		}
	case DIK_NUMPAD5:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 5");	
			break;
		}
	case DIK_NUMPAD6:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 6");	
			break;
		}
	case DIK_NUMPAD7:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 7");	
			break;
		}
	case DIK_NUMPAD8:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 8");	
			break;
		}
	case DIK_NUMPAD9:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPA 9");	
			break;
		}
	case DIK_NUMPAD0:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD 0");	
			break;
		}
	case DIK_NUMPADENTER:
		{
			strcpy_s(buffer,_countof(buffer),"NUMPAD ENTER");	
			break;
		}
	}

	return buffer;
}

bool CKeyBindsState::SetKeyBind(int keycode)
{
	switch(m_nSelection)
	{
	case WS_SHOOT:
		m_pKeyboardKB->SetShoot(keycode);
		return false;
		break;
	case WS_CHANGE_WEAPON:
		m_pKeyboardKB->SetChangeWeapon(keycode);
		return false;
		break;
	case WS_FORWARD:
		m_pKeyboardKB->SetForward(keycode);
		return false;
		break;
	case WS_BACKWARD:
		m_pKeyboardKB->SetBackward(keycode);
		return false;
		break;
	case WS_LEFT:
		m_pKeyboardKB->SetLeft(keycode);
		return false;
		break;
	case WS_RIGHT:
		m_pKeyboardKB->SetRight(keycode);
		return false;
		break;
	}
	return true;
}