#include "CPlayer.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CGame.h"
#include "CXboxInput.h"
#include "CPauseMenuState.h"
#include "CSGD_TextureManager.h"
#include "CBullet.h"
#include "CSpeedRamp.h"
#include "CCamera.h"
#include "CMessage.h"
#include "CMessageSystem.h"
#include "CKeyBinds.h"
#include "CMainMenuState.h"
#include "CNumPlayers.h"
#include "COptionState.h"
#include <math.h>
CPlayer::CPlayer(CXboxInput* pController)
{
	m_nType = OBJECT_PLAYER;

	m_pController1 = pController;
	SetCollisionDelay(0.0f);
	

	SetBulletImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/bullet.png",D3DCOLOR_XRGB(255, 255, 255)));
	SetMissileImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MissilePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
	m_pCamera = new CCamera();

	m_pES = CEventSystem::GetInstance ();
	CKeyBinds* tempkeys = m_pController1->GetKB();
	//tempkeys->SetShootAccept(XINPUT_GAMEPAD_Y);
	m_pES->RegisterClient ("CameraCollision", this);
	m_pES->RegisterClient("powerup_power", this);
	m_fFireTimer = 0.0;

}
CPlayer::~CPlayer(void)
{
	m_pES->UnregisterClient ("CameraCollision", this);
	m_pES->UnregisterClient("powerup_power",this);
	delete m_pCamera;
}

void CPlayer::Update(float fElapsedTime)
{
	CSGD_DirectInput* m_pDI = CSGD_DirectInput::GetInstance();
	
	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		if( COptionState::GetInstance()->IsVertical())
			m_pCamera->AttachTo(this,200.0f,300.0f);
		else
			m_pCamera->AttachTo(this, 400.0f, 150.0f);
	}
	else
		m_pCamera->AttachTo(this,400.0f,300.0f);
	m_pCamera->Update();

	m_fFireTimer += fElapsedTime;
	static float m_ftimer;
	m_ftimer += fElapsedTime;
	if( m_ftimer >= 3.0f)
	{
		m_ftimer = 0.0f;
		SetKillCount(GetKillCount() + 1);
	}


	if(CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = m_pController1->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		BYTE lTrig = xState.Gamepad.bLeftTrigger;
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		CKeyBinds* tempkeys = m_pController1->GetKB();
		float firerate;
		if(GetSelectedWeapon() == WEAPON_PISTOL)
			firerate = GetFireDelay();
		else if(GetSelectedWeapon() == WEAPON_RPG)
			firerate = this->GetFireDelayMissile();
		else if(GetSelectedWeapon() == WEAPON_SPECIAL)
			firerate = GetFireDelay();
		if(m_fFireTimer >= firerate)
		{
			/*if(xState.Gamepad.wButtons & tempkeys->GetShoot())
			{
				CMessageSystem* pMS = CMessageSystem::GetInstance();
				PlayBullet();
				pMS->SendMsg(new CCreatePlayerBulletMessage(this));
				m_fFireTimer = 0.0f;
				m_pController1->Vibrate(20000,20000);
			}*/
			if(xState.Gamepad.wButtons & tempkeys->GetShoot())
			{
				m_fFireTimer = 0.0f;
				CMessageSystem* pMS = CMessageSystem::GetInstance();
				switch(GetSelectedWeapon())
				{
				case WEAPON_PISTOL:
				{
					PlayBullet();
					pMS->SendMsg(new CCreatePlayerBulletMessage(this));
				}
					break;
				case WEAPON_RPG:
				{
					PlayBullet();
					pMS->SendMsg(new CCreatePlayerMissileMessage(this));
				}
					break;
				case WEAPON_SPECIAL:
				{
					SetPowerUpBar(0);
				}
					break;
				}
			}
		}

		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			//	CGame::GetInstance()->ResetInputDelay();
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_START)
			{
				CGame::GetInstance()->AddState(CPauseMenuState::GetInstance());
				CPauseMenuState::GetInstance()->SetController(m_pController1);
				CGame::GetInstance()->ResetInputDelay();
			}
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER )
			{
				IncrementWeapon();
				if(GetSelectedWeapon() > 2)
					SetSelectedWeapon(0);
			}
		/*	if(xState.Gamepad.wButtons & tempkeys->GetBack())
			{
				if(GetShieldBar() <= 0)
					SetHealth(GetHealth() - 10);
				else
					SetShieldBar(GetShieldBar() - 10);
			}
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			{
				if(GetPowerUpBar() < 100)
					SetPowerUpBar(GetPowerUpBar() + 10);

			}
			if(xState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				if(GetPowerUpBar() >= GetMaxPowerUp())
					SetPowerUpBar(0);
			}*/

		}
		if(rTrig > 5)
		{
			//m_pController1->Vibrate(rTrig * 150,rTrig * 150);
			SetAccelerating(true);
			if(GetSpeed() < GetMaxSpeed())
				SetSpeed(GetSpeed() + (GetAcceleration() * fElapsedTime));
		}
		else
			SetAccelerating(false);
		if(lTrig > 5)
		{
			//SetAccelerating(true);
			if(GetSpeed() > (-0.5f * GetMaxSpeed()))
				SetSpeed(GetSpeed() - (GetAcceleration() * fElapsedTime));
		}
		

		if(CGame::GetInstance()->GetInputDelay() >= 0.15f)
		{
			//CGame::GetInstance()->ResetInputDelay();

		}
		if(CGame::GetInstance()->GetThumbDelay() >= 0.00f)
		{
			//CGame::GetInstance()->ResetThumbDelay();

			if(x < -16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)// && y < 15000 && y > -15000)
			{
				SetRotation(GetRotation() - (GetRotationRate() * fElapsedTime));
				tVector2D tempdir = GetDirection();
				tempdir.fX = 0;
				tempdir.fY = -1;
				tempdir = Vector2DRotate(tempdir, GetRotation());
				//tempdir = tempdir * GetSpeed();
				SetDirection(tempdir);
				Rotate(GetRotation());
			}
			else if(x > 16000|| xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)// && y < 15000 && y > -15000)
			{
				SetRotation(GetRotation() + (GetRotationRate() * fElapsedTime));
				tVector2D tempdir = GetDirection();
				tempdir.fX = 0;
				tempdir.fY = -1;
				tempdir = Vector2DRotate(tempdir, GetRotation());
				//tempdir = tempdir * GetSpeed();
				SetDirection(tempdir);
				Rotate(GetRotation());
			}

		}
	}
	else
	{
		
			if(m_pDI->KeyDown(DIK_UP))
			{
				SetAccelerating(true);
				if(GetSpeed() < GetMaxSpeed())
					SetSpeed(GetSpeed() + (GetAcceleration() * fElapsedTime));
			}
			else
				SetAccelerating(false);

		
			if(m_pDI->KeyDown(DIK_DOWN))
			{
				if(GetSpeed() > (-0.5f * GetMaxSpeed()))
					SetSpeed(GetSpeed() - (GetAcceleration() * fElapsedTime));
			}


		if(m_pDI->KeyDown(DIK_LEFT))
		{
			
			SetRotation(GetRotation() - (GetRotationRate() * fElapsedTime));
			tVector2D tempdir = GetDirection();
			tempdir.fX = 0;
			tempdir.fY = -1;
			tempdir = Vector2DRotate(tempdir, GetRotation());
			//tempdir = tempdir * GetSpeed();
			SetDirection(tempdir);
			Rotate(GetRotation());
		}

		if(m_pDI->KeyDown(DIK_RIGHT))
		{
			SetRotation(GetRotation() + (GetRotationRate() * fElapsedTime));
			tVector2D tempdir = GetDirection();
			tempdir.fX = 0;
			tempdir.fY = -1;
			tempdir = Vector2DRotate(tempdir, GetRotation());
			//tempdir = tempdir * GetSpeed();
			SetDirection(tempdir);
			Rotate(GetRotation());
		}
		
		if(m_pDI->KeyDown(DIK_SPACE))
		{
			float firerate;
			if(GetSelectedWeapon() == WEAPON_PISTOL)
				firerate = GetFireDelay();
			else if(GetSelectedWeapon() == WEAPON_RPG)
				firerate = this->GetFireDelayMissile();
			else if(GetSelectedWeapon() == WEAPON_SPECIAL)
				firerate = GetFireDelay();
			if(m_fFireTimer >= firerate)
			{
				m_fFireTimer = 0.0f;
				CMessageSystem* pMS = CMessageSystem::GetInstance();
				switch(GetSelectedWeapon())
				{
					case WEAPON_PISTOL:
					{
						PlayBullet();
						pMS->SendMsg(new CCreatePlayerBulletMessage(this));
					}
					break;
					case WEAPON_RPG:
					{
						PlayBullet();
						pMS->SendMsg(new CCreatePlayerMissileMessage(this));
					}
					break;
					case WEAPON_SPECIAL:
					{
						//if(GetPowerUpBar() >= GetMaxPowerUp())
						//{
							//SetPowerUpBar(0);
							switch(m_nPlayerType)
							{
								case CAR_MINI:
									{
										PlayBullet();
										pMS->SendMsg(new CCreateMiniSpecialMessage(this));
									}
									break;
								case CAR_VETTE:
									{
										
									}
									break;
								case CAR_HUMMER:
									{
										
									}
									break;
								case CAR_TRUCK:
									{
										PlayBullet();
										//pMS->SendMsg(new CCreateTruckSpecialMessage(this));
									}
									break;
							}
						//}
					}
					break;
				}
			}
		}
		if(m_pDI->KeyPressed(DIK_LCONTROL))
		{
			IncrementWeapon();
				if(GetSelectedWeapon() > 2)
					SetSelectedWeapon(0);
		}
		if(m_pDI->KeyPressed(DIK_Z))
		{
			if(GetShieldBar() <= 0)
				SetHealth(GetHealth() - 10);
			else
				SetShieldBar(GetShieldBar() - 10);
		}
		if(m_pDI->KeyPressed(DIK_X))
		{
			if(GetPowerUpBar() < 100)
				SetPowerUpBar(GetPowerUpBar() + 10);

		}
	}

	SetCollisionDelay(GetCollisionDelay() + fElapsedTime);
	if(GetCollisionDelay() >= 0.2)
	{
		SetCollisionDelay(0.0f);
		m_pController1->Vibrate();
	}
	/*if(GetSpeed() < GetMaxSpeed())
	SetSpeed(GetMaxSpeed());*/
	//else if(GetSpeed() < (GetMaxSpeed() *-1))
	//	SetSpeed((GetMaxSpeed() *-1));
	//tVector2D tempdir = GetDirection();
	//tempdir.fX = 0;
	//tempdir.fY = -1;
	//tempdir = Vector2DRotate(tempdir, GetRotation());
	//tempdir = tempdir * GetSpeed();
	//SetDirection(tempdir);
	//float rotate = AngleBetweenVectors(tempdir,GetVelocity());

	//tVector2D tempvel = GetVelocity();
	//float fx = tempvel.fX;
	//float fy = tempvel.fY;
	//tempvel.fX = 0;
	//tempvel.fY = -1;

	//tempvel = Vector2DRotate(tempvel,);
	//tempvel.fX *= fx;
	//tempvel.fY *= fy;
	//SetVelocity(tempvel);
	//tempdir = tempdir * GetSpeed();

	CCar::Update(fElapsedTime);
}

void CPlayer::Render(CCamera* camera)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	CCar::Render(camera);

	// HUD informations
	RECT health;
	RECT shield;
	RECT power;
	RECT weapon;
	RECT minimap;
	RECT score;

	score.left = 0;
	score.top = 340;
	score.right = score.left + 80;
	score.bottom = score.top + 150;
	pD3D->DrawRect(score,255,255,255);
	pD3D->DrawText("Scoreboard",2,350,0,0,0);

	weapon.left = 0;
	weapon.top = 500;
	weapon.right = weapon.left + 90;
	weapon .bottom = weapon.top + 100;
	switch(GetSelectedWeapon())
	{
	case WEAPON_PISTOL:
		pD3D->DrawRect(weapon,255,255,255);
		break;
	case WEAPON_RPG:
		pD3D->DrawRect(weapon,255,0,0);
		break;
	case WEAPON_SPECIAL:
		pD3D->DrawRect(weapon,0,0,255);
		break;
	}
	pD3D->DrawText("weapon",10,550,0,0,0);
	
	

	minimap.left = 650;
	minimap.top = 450;
	minimap.right = minimap.left + 150;
	minimap.bottom = minimap.top + 150;
	pD3D->DrawRect(minimap,255,255,255);
	pD3D->DrawText("minimap",660,525,0,0,0);

	health.left = 90;
	health.top = 520;
	health.right = (LONG)(health.left + (GetHealth()/GetMaxHealth()) * 100);
	health.bottom = health.top + 30;
	pD3D->DrawRect(health,0,255,0);


	shield.left = 90;
	shield.top = 520;
	shield.right = (LONG)(shield.left + (GetShieldBar()/GetMaxShield()) * 100);
	shield.bottom = shield.top + 30;
	pD3D->DrawRect(shield,105,105,105);
	pD3D->DrawText("health",100,530,0,0,0);

	power.left = 90;
	power.top = 550;
	power.right = power.left + (int)((GetPowerUpBar()/GetMaxPowerUp()) * 80);
	power.bottom = power.top + 20;
	pD3D->DrawRect(power,0,80,255);
	pD3D->DrawText("power",100,550,0,255,0);

	
}

bool CPlayer::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(pBase == this)
		return false;
	if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_PLAYER)
	{
		CCar* tempcar = (CCar*)pBase;
		//float centerx = tempcar->GetPosX();
		//float centery = tempcar->GetPosY();
		float centerx = tempcar->GetCX1();
		float centery = tempcar->GetCY1();
		float myx = GetCX1();
		float myy = GetCY1();
		
		float distance = sqrt(((centerx - myx)*(centerx - myx)) + ((centery - myy)*(centery - myy)));

		if(distance <= (GetRadius() + tempcar->GetRadius()))
		{
			tVector2D othervel = tempcar->GetOverallVelocity();
			tVector2D currentvel = GetOverallVelocity();

			float myfx = abs(currentvel.fX);
			float myfy = abs(currentvel.fY);
			float hisfx = abs(othervel.fX);
			float hisfy = abs(othervel.fY);
	
			
			
			tVector2D tobeapplied;
			tobeapplied.fX = 0;
			tobeapplied.fY = 0;
			if((myfx+myfy) > (hisfx+hisfy))
			{
				currentvel = currentvel * -0.8f;
				tobeapplied = currentvel;
				SetVelocity(tobeapplied);
				tempcar->SetVelocity(tobeapplied * -1.0f);
			}
			else
			{
				othervel = othervel * -0.8f;
				tobeapplied = othervel;
				tempcar->SetVelocity(tobeapplied);
				SetVelocity(tobeapplied * -1.0f);
			}

		
			float myspeed = GetSpeed();
			float hisspeed = tempcar->GetSpeed();
			float speedtouse;
			if(myspeed > hisspeed)
			{
				myspeed = myspeed * 0.3f;
				SetSpeed(-1.0f * myspeed);
				tempcar->SetSpeed(myspeed);
			}
			else
			{
				hisspeed = hisspeed * 0.3f;
				tempcar->SetSpeed(-1.0f * hisspeed);
				SetSpeed(hisspeed);
			}



			
		}
	}
	


	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_ENEMY)
		{
			
			// old rectangle collision - save for reference
			
			//float speed = GetSpeed();
			//if(speed >= -10 && speed < 10)
			//{
			//	SetSpeed(-10);
			//	m_pController1->Vibrate(10000,10000);
			//}
			//else
			//{
			//	PlayCrash();
			//	m_pController1->Vibrate(40000,40000);
			//	m_fCollisionDelay = 0.0f;
			//	CCar* tempcar = (CCar*)pBase;
			//	//tempcar->SetDirection(GetDirection());
			//	//tempcar->SetSpeed(GetSpeed() * 0.2f);
			//	tempcar->SetVelocity(GetDirection());
			//	SetSpeed((GetSpeed() * -1) + (GetSpeed() * 0.2f));
			//}
			//speed = GetSpeed();

			//return true;
		}
		else if(pBase->GetType() == OBJECT_BULLET)
		{
			return false;
		}
		else if(pBase->GetType() == OBJECT_SPEEDRAMP)
		{
			SetSpeed(0);
			CSpeedRamp* tempramp = (CSpeedRamp*)pBase;
			SetVelocity((tempramp->GetVelDir() * (GetMaxSpeed() + 100) ));
			//SetSpeed(GetMaxSpeed() + 300.0f);
			return true;
		}


	}
	return false;
}

void CPlayer::HandleEvent(CEvent* pEvent)
{
	if (this == pEvent->GetParam ())
	{
		if (pEvent->GetEventID () == "CameraCollision")
		{
			int abc = 0;
			abc = 20;
			//m_pMS->SendMsg (new CDestroyBulletMessage(this));
		}
		else if(pEvent->GetEventID() == "damage")
		{
			CBullet* tempbullet = (CBullet*)pEvent->GetParam2();
			float damage = tempbullet->GetDamage();
			if(GetShieldBar() >= 0)
			{
				SetShieldBar(GetShieldBar() - tempbullet->GetDamage());
			}
			else
			{
				SetHealth(GetHealth() - tempbullet->GetDamage());
			}

		}
		else if(pEvent->GetEventID() == "powerup_power")
		{
			SetPowerUpBar(GetPowerUpBar() + 20);
		}
	}
	
}