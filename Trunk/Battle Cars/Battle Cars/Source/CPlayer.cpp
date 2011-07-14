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
#include "CHUD.h"
#include "ParticleManager.h"
#include "Emittor.h"

CPlayer::CPlayer(CXboxInput* pController)
{
	m_nType = OBJECT_PLAYER;

	m_pController1 = pController;
	SetCollisionDelay(0.0f);
	

	SetBulletImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/bullet.png",D3DCOLOR_XRGB(255, 255, 255)));
	SetMissileImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_MissilePlaceholder.png", D3DCOLOR_XRGB(255, 255, 255)));
	m_pCamera = new CCamera();
	m_pHUD = new CHUD();
	m_pHUD->SetOwner(this);
	m_pES = CEventSystem::GetInstance ();
	CKeyBinds* tempkeys = m_pController1->GetKB();
	//tempkeys->SetShootAccept(XINPUT_GAMEPAD_Y);
	m_pES->RegisterClient ("CameraCollision", this);
	m_pES->RegisterClient("powerup_power", this);
	m_pES->RegisterClient("collision", this);
	m_pES->RegisterClient("health_up", this);
	m_fFireTimer = 0.0;
	m_bIsFlameThrowerOn = false;
	m_bIsIcyGatlingOn = false;
	m_fIcyBullets = 0.0f;
	m_fFlames = 0.0f;

	CEventSystem::GetInstance()->RegisterClient("collision", this);
	SetCollisionEffect(false);

}
CPlayer::~CPlayer(void)
{
	m_pES->UnregisterClient ("CameraCollision", this);
	m_pES->UnregisterClient("powerup_power",this);
	m_pES->UnregisterClient("health_up", this);
	m_pES->UnregisterClient("collision", this);
	delete m_pCamera;
	delete m_pHUD;
}

void CPlayer::Update(float fElapsedTime)
{
	CSGD_DirectInput* m_pDI = CSGD_DirectInput::GetInstance();
	
	if( CNumPlayers::GetInstance()->GetNumberOfPlayers() == 2)
	{
		if( COptionState::GetInstance()->IsVertical())
			m_pCamera->AttachTo(this,CGame::GetInstance()->GetScreenWidth()*0.25f,CGame::GetInstance()->GetScreenHeight()*0.5f);
		else
			m_pCamera->AttachTo(this,CGame::GetInstance()->GetScreenWidth()*0.5f, CGame::GetInstance()->GetScreenHeight()*0.25f);
	}
	else
		m_pCamera->AttachTo(this,CGame::GetInstance()->GetScreenWidth()*0.5f,CGame::GetInstance()->GetScreenHeight()*0.5f);
	m_pCamera->Update();

	m_fFireTimer += fElapsedTime;
	static float m_ftimer;
	m_ftimer += fElapsedTime;
	if(m_bIsFlameThrowerOn)
	{
		m_fFlames = m_fFlames - fElapsedTime;
		if(m_fFlames > 0.0f)
		{
			PlayBullet();		
			CMessageSystem::GetInstance()->SendMsg(new CCreateVetteSpecialMessage(this));
			m_fFireTimer = 0.0f;
		}
		else
			m_bIsFlameThrowerOn = false;
	}
	else if(m_bIsIcyGatlingOn)
	{
		if((int)(m_fIcyBullets)%6 == 0)
		{
			PlayBullet();
			CMessageSystem::GetInstance()->SendMsg(new CCreateHummerSpecialMessage(this));
			m_fFireTimer = 0.0f;
		}
		m_fIcyBullets -= 1.0f;
		if(m_fIcyBullets <= 0.0f)
			m_bIsIcyGatlingOn = false;
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
					switch(GetPlayerType())
							{
								case CAR_MINI:
									{
										PlayBullet();
										pMS->SendMsg(new CCreateMiniSpecialMessage(this));
									}
									break;
								case CAR_VETTE:
									{
										m_fFlames = 1.5f;
										m_bIsFlameThrowerOn = true;
									}
									break;
								case CAR_HUMMER:
									{
										m_fIcyBullets = 40.0f;
										m_bIsIcyGatlingOn = true;
									}
									break;
								case CAR_TRUCK:
									{
										PlayBullet();
										pMS->SendMsg(new CCreateTruckSpecialMessage(this));
									}
									break;
							}
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
							switch(GetPlayerType())
							{
								case CAR_MINI:
									{
										PlayBullet();
										pMS->SendMsg(new CCreateMiniSpecialMessage(this));
									}
									break;
								case CAR_VETTE:
									{
										m_fFlames = 1.5f;
										m_bIsFlameThrowerOn = true;
									}
									break;
								case CAR_HUMMER:
									{
										m_fIcyBullets = 40.0f;
										m_bIsIcyGatlingOn = true;
									}
									break;
								case CAR_TRUCK:
									{
										PlayBullet();
										pMS->SendMsg(new CCreateTruckSpecialMessage(this));
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
	m_pHUD->Update(fElapsedTime);
	CCar::Update(fElapsedTime);
}

void CPlayer::Render(CCamera* camera)
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	CCar::Render(camera);
	char buffer[128];
	sprintf_s(buffer,"%i",GetSpecialLevel());
	pD3D->DrawText(buffer,10,10,255,255,255);
	m_pHUD->Render();
	
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

		float centerx2 = tempcar->GetCX2();
		float centery2 = tempcar->GetCY2();
		float myx2 = GetCX2();
		float myy2 = GetCY2();
		float distance11 = sqrt(((centerx - myx)*(centerx - myx)) + ((centery - myy)*(centery - myy)));
		float distance21 = sqrt(((centerx2 - myx2)*(centerx2 - myx2)) + ((centery2 - myy2)*(centery2 - myy2)));
		float distance12 = sqrt(((centerx2 - myx)*(centerx2 - myx)) + ((centery2 - myy)*(centery2 - myy)));
		float distance22 = sqrt(((centerx - myx2)*(centerx - myx2)) + ((centery - myy2)*(centery - myy2)));
		if(distance11 <= (GetRadius() + tempcar->GetRadius()) || distance21 <= (GetRadius() + tempcar->GetRadius())
			|| distance12 <= (GetRadius() + tempcar->GetRadius()) || distance22 <= (GetRadius() + tempcar->GetRadius()))
		{
			tVector2D othervel = tempcar->GetOverallVelocity();
			tVector2D currentvel = GetOverallVelocity();

			float myfx = abs(currentvel.fX);
			float myfy = abs(currentvel.fY);
			float hisfx = abs(othervel.fX);
			float hisfy = abs(othervel.fY);

			/*if(myfx <= 5 && myfy <= 5)
			{
				currentvel.fX = GetDirection().fX * 20;
				currentvel.fY = GetDirection().fY * 20;
			}
			if(hisfx <= 5 && hisfy <= 5)
			{
				othervel.fX = tempcar->GetDirection().fX * 20;
				othervel.fY = tempcar->GetDirection().fY * 20;
			}*/
			if(GetVelX() > 10)
			{
				SetPosX(GetPosX() + (GetVelX() * 0.001f * -1.0f));
			}
			else
			{
				float vel = GetDirection().fX * 20;
				SetPosX(GetPosX() + (vel * 0.001f * -1.0f));
			}
			if(GetVelY() > 10)
			{
				SetPosY(GetPosY() + (GetVelY() * 0.001f * -1.0f));
			}
			else
			{
				float vel = GetDirection().fY * 20;
				SetPosY(GetPosY() + (vel * 0.001f * -1.0f));
			}
			if(tempcar->GetVelX() > 10)
			{
				tempcar->SetPosX(tempcar->GetPosX() + (tempcar->GetVelX() * 0.001f * -1.0f));
			}
			else
			{
				float vel = tempcar->GetDirection().fX * 20;
				tempcar->SetPosX(tempcar->GetPosX() + (vel * 0.001f * -1.0f));
			}
			if(tempcar->GetVelY() > 10)
			{
				tempcar->SetPosY(tempcar->GetPosY() + (tempcar->GetVelY() * 0.001f * -1.0f));
			}
			else
			{
				float vel = tempcar->GetDirection().fY * 20;
				tempcar->SetPosY(tempcar->GetPosY() + (vel * 0.001f * -1.0f));
			}

			
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

			if( !GetCollisionEffect() )
			{
				CEventSystem::GetInstance()->SendEvent("collision", this);
				SetCollisionEffect(true);
			}

			return true;
		}
		
	}
	


	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_BULLET)
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
	CPlayer* temp_player = (CPlayer*)pEvent->GetParam();
	if (this == temp_player)
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
			if(tempbullet->GetOwner() != this)
			{
			float damage = tempbullet->GetDamage();
			if(tempbullet->GetSlowRate() != 0.0f)
				SetSpeed(GetSpeed()*.75f);
			if(GetShieldBar() >= 0)
			{
				SetShieldBar(GetShieldBar() - tempbullet->GetDamage());
			}
			else
			{
				SetHealth(GetHealth() - tempbullet->GetDamage());
			}
			if(GetHealth() <= 0)
			{
				
				if(GetIsAlive() == true)
					tempbullet->GetOwner()->SetKillCount(tempbullet->GetOwner()->GetKillCount() + 1);
				SetIsAlive(false);
			}
			}

		}
		
		else if(pEvent->GetEventID() == "weapon_level")
		{
			if(GetSpecialLevel() < 4)
				SetSpecialLevel(GetSpecialLevel() + 1);
		}
		else if(pEvent->GetEventID() == "powerup_power")
		{
			if(GetPowerUpBar() < GetMaxPowerUp())
				SetPowerUpBar(GetPowerUpBar() + 20);
			if(GetPowerUpBar() > GetMaxPowerUp())
				SetPowerUpBar(GetMaxPowerUp());
		}
		else if( pEvent->GetEventID() == "collision")
		{
			ParticleManager* pPM = ParticleManager::GetInstance(); 
			Emittor* tempemittor = pPM->CreateEffect(pPM->GetEmittor(COLLISION_EMITTOR), GetCX1(), GetCY1());

			if( tempemittor)
			{
				tempemittor->SetTimeToDie(1.0f);
				pPM->AttachToBasePosition(NULL, tempemittor, GetCX1(), GetCY1());
				SetCollisionEffect(false);
			}
		}
		else if(pEvent->GetEventID() == "health_up")
		{
			if(GetHealth() < GetMaxHealth())
			{
				SetHealth(GetHealth()+20.0f);
				if(GetHealth() > GetMaxHealth())
					SetHealth(GetMaxHealth());
			}
		}
	}
	
}