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
#include "CKeyboardKeyBinds.h"
#include "CNumPlayers.h"
#include "COptionState.h"
#include <math.h>
#include "CHUD.h"
#include "ParticleManager.h"
#include "Emittor.h"
#include "Gamer_Profile.h"
#include "CGamerProfile.h"
#include "CLevel.h"
#include "CGamePlayState.h"

CPlayer::CPlayer(CXboxInput* pController)
{
	m_nType = OBJECT_PLAYER;

	m_pController1 = pController;
	SetCollisionDelay(0.0f);
	

	SetBulletImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/bullet.png",D3DCOLOR_XRGB(255, 255, 255)));
	SetMissileImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BC_Missile.png"));

	Weapons.push_back ( CSGD_TextureManager::GetInstance()->LoadTexture("Resource/Graphics/HUDS/Weapon_Icons/Pistoal_PlaceHolder.png"));
	Weapons.push_back ( CSGD_TextureManager::GetInstance()->LoadTexture("Resource/Graphics/HUDS/Weapon_Icons/ranged08.png"));
	controller = false;
	m_pCamera = new CCamera();
	m_pHUD = new CHUD();
	m_pHUD->SetOwner(this);
	m_pES = CEventSystem::GetInstance ();
	CKeyBinds* tempkeys = m_pController1->GetKB();
	//tempkeys->SetShootAccept(XINPUT_GAMEPAD_Y);
	//m_pES->RegisterClient ("WallCollision", this);
	m_pES->RegisterClient("powerup_power", this);
	m_pES->RegisterClient("powerup_shield", this);
	m_pES->RegisterClient("collision", this);
	m_pES->RegisterClient("health_up", this);
	m_fFireTimer = 0.0;
	m_bIsFlameThrowerOn = false;
	m_bIsIcyGatlingOn = false;
	m_fIcyBullets = 0.0f;
	m_fFlames = 0.0f;

	SetFlameThrowerEmittorID(-1);

	CEventSystem::GetInstance()->RegisterClient("collision", this);
	SetCollisionEffect(false);

	isSet = false;
	m_fBurstDelay = 0.15f;

}
CPlayer::~CPlayer(void)
{
	//m_pES->UnregisterClient ("WallCollision", this);
	m_pES->UnregisterClient("powerup_power",this);
	m_pES->UnregisterClient("health_up", this);
	m_pES->UnregisterClient("collision", this);
	m_pES->UnregisterClient("powerup_shield", this);
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
	CLevel::GetInstance ()->CheckPlayerCollision (this, this->GetCamera ());

	if( GetHealth() <= 0.0f)
	{
		SetRespawnTimer(GetRespawnTimer() + fElapsedTime);
		if( GetFlameThrowerEmittorID() > -1)
		{
			Emittor* flame_thrower = ParticleManager::GetInstance()->GetActiveEmittor(GetFlameThrowerEmittorID());
			if(flame_thrower)
				flame_thrower->SetTimeToDie(0.0f);
		}
		m_bIsFlameThrowerOn = false;
		SetFlameThrowerEmittorID(-1);
		return;
	}

	m_fFireTimer += fElapsedTime;
	static float m_ftimer;
	m_ftimer += fElapsedTime;
	if(m_bIsFlameThrowerOn)
	{
		ParticleManager* PM = ParticleManager::GetInstance();
		Emittor* flame_thrower = NULL;

		m_fBurstDelay -= fElapsedTime;


		if( !isSet )
		{
				isSet = true;

				tVector2D temp;
				temp.fX = 0;
				temp.fY = -1;
				temp = Vector2DRotate(temp,GetRotation());
				Vector2DNormalize(temp);
				temp = temp * (350+ GetSpeed());
				
				flame_thrower = PM->CreateEffect(PM->GetEmittor(FLAMETHROWER_EMITTOR), GetPosX(), GetPosY(), temp.fX, temp.fY);

				if( flame_thrower )
				{
					SetFlameThrowerEmittorID( flame_thrower->GetID());
					flame_thrower->SetTimeToDie(2.0f);
				}

		}

		if( flame_thrower == NULL )
		{
			flame_thrower = PM->GetActiveEmittor(GetFlameThrowerEmittorID());
			
		}

		if( flame_thrower )
		{
			tVector2D temp;
			temp.fX = 0;
			temp.fY = -1;
			temp = Vector2DRotate(temp,GetRotation());
			Vector2DNormalize(temp);
			temp = temp * (350+ GetSpeed());
			flame_thrower->SetAcceleration(temp.fX, temp.fY);

			PM->AttachToBase(this, flame_thrower);
		}

		m_fFlames = m_fFlames - fElapsedTime;
		if(m_fFlames > 0.0f)
		{
			//PlayBullet();	
			if( m_fBurstDelay <= 0 )
			{
				CMessageSystem::GetInstance()->SendMsg(new CCreateVetteSpecialMessage(this));
				m_fBurstDelay = 0.15f;
			}
			m_fFireTimer = 0.0f;
		}
		else
		{
			m_bIsFlameThrowerOn = false;
			SetFlameThrowerEmittorID(-1);
			isSet = false;
		}
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
	if(m_nPlayerNum == 1)
	{
		if(CGame::GetInstance()->ControllerInput())
			controller = true;
		else
			controller = false;
		
	}
	else if(m_nPlayerNum == 2)
		{
			if(CGame::GetInstance()->Controller2Connected())
				controller = true;
			else
			controller = false;
		}
		
	if(controller)//CGame::GetInstance()->ControllerInput())
	{
		//m_pController1->ReadInputState();
		XINPUT_STATE xState = m_pController1->GetState();
		BYTE rTrig = xState.Gamepad.bRightTrigger;
		BYTE lTrig = xState.Gamepad.bLeftTrigger;
		float x = xState.Gamepad.sThumbLX;
		float y = xState.Gamepad.sThumbLY;
		CKeyBinds* tempkeys = CGamerProfile::GetInstance()->GetActiveProfile()->m_pKB;
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
					if( GetPowerUpBar() >= GetMaxPowerUp() )
					{
						SetPowerUpBar(0.0f);
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
			if(xState.Gamepad.wButtons & tempkeys->GetChangeWeapon() )
			{
				CGame::GetInstance()->ResetInputDelay();
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
		
		if(CGame::GetInstance()->GetThumbDelay() >= 0.00f)
		{
			//CGame::GetInstance()->ResetThumbDelay();
			if(GetTurnable())
			{
			if(x < -16000 || xState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)// && y < 15000 && y > -15000)
			{
				CGame::GetInstance()->ResetThumbDelay();
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
				CGame::GetInstance()->ResetThumbDelay();
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
	}
	else
	{
		CKeyboardKeyBinds* tempkeys = CGamerProfile::GetInstance()->GetActiveProfile()->m_pKKB;

		if(m_pDI->KeyDown(tempkeys->Getforward()))
			{
				SetAccelerating(true);
				if(GetSpeed() < GetMaxSpeed())
					SetSpeed(GetSpeed() + (GetAcceleration() * fElapsedTime));
			}
			else
				SetAccelerating(false);

		
		if(m_pDI->KeyDown(tempkeys->Getbackward()))
			{
				if(GetSpeed() > (-0.5f * GetMaxSpeed()))
					SetSpeed(GetSpeed() - (GetAcceleration() * fElapsedTime));
			}

		if(GetTurnable())
		{
		if(m_pDI->KeyDown(tempkeys->GetLeft()))
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

		if(m_pDI->KeyDown(tempkeys->GetRight()))
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
		if(m_pDI->KeyDown(tempkeys->GetShoot()))
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
						if(GetPowerUpBar() >= GetMaxPowerUp())
						{
							SetPowerUpBar(0);
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
					}
					break;
				}
			}
		}
		if(m_pDI->KeyPressed(tempkeys->GetChangeWeapon()))
		{
			IncrementWeapon();
				if(GetSelectedWeapon() > 2)
					SetSelectedWeapon(0);
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
	if(m_pController1)
		m_pController1->Vibrate();
	CCar::Render(camera);

}

bool CPlayer::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(pBase == this)
		return false;
	if(pBase->GetType() == OBJECT_OBSTACLE || pBase->GetType() == OBJECT_BULLET)
		return false;
	if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_PLAYER || pBase->GetType() == OBJECT_BOSS)
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
			this->GetController()->Vibrate(30000,30000);


			SetTurnable(false);
			tempcar->SetTurnable(false);
			tVector2D othervel = tempcar->GetOverallVelocity();
			tVector2D currentvel = GetOverallVelocity();

			float myfx = abs(currentvel.fX);
			float myfy = abs(currentvel.fY);
			float hisfx = abs(othervel.fX);
			float hisfy = abs(othervel.fY);

			float myxpos = GetPosX();
			float myypos = GetPosY();
			float hisxpos = tempcar->GetPosX();
			float hisypos = tempcar->GetPosY();

			float xmove = 0;
			float ymove = 0;

			if(myxpos < hisxpos)
				xmove = -1.0f;
			else
				xmove = 1.0f;
			if(myypos < hisypos)
				ymove = -1.0f;
			else
				ymove = 1.0f;
			int count = 0;
			while(distance11 <= (GetRadius() + tempcar->GetRadius() ) || distance21 <= (GetRadius() + tempcar->GetRadius() )
				 || distance12 <= (GetRadius() + tempcar->GetRadius() ) || distance22 <= (GetRadius() + tempcar->GetRadius()))
			{

				SetPosX(GetPosX() + xmove);
				SetPosY(GetPosY() + ymove);
				myx = myx + xmove;
				myy = myy + ymove;
				myx2 = myx2 + xmove;
				myy2 = myy2 + ymove;
				//centerx = centerx + xmove;
				//centery = centery + ymove;
				//centerx2 =centerx2 + xmove;
				//centery2 = centery2 + ymove;
				count++;
				if(count > 10)
					break;
				distance11 = sqrt(((centerx - myx)*(centerx - myx)) + ((centery - myy)*(centery - myy)));
				distance21 = sqrt(((centerx2 - myx2)*(centerx2 - myx2)) + ((centery2 - myy2)*(centery2 - myy2)));
				distance12 = sqrt(((centerx2 - myx)*(centerx2 - myx)) + ((centery2 - myy)*(centery2 - myy)));
				distance22 = sqrt(((centerx - myx2)*(centerx - myx2)) + ((centery - myy2)*(centery - myy2)));
			}
			
			/*tVector2D tobeapplied;
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
			}*/

		
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
			SetSpeed(0);
			tempcar->SetSpeed(0);
			this->SetVelocity(othervel);
			tempcar->SetVelocity(currentvel);
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
		if(pBase->GetType() == OBJECT_SPEEDRAMP)
		{
			SetSpeed(0);
			CSpeedRamp* tempramp = (CSpeedRamp*)pBase;
			tVector2D myvel = GetVelocity();
			tVector2D boostvel = (tempramp->GetVelDir() * (GetMaxSpeed() + 50) );
			myvel.fX = myvel.fX + boostvel.fX;
			myvel.fY = myvel.fY + boostvel.fY;
			SetVelocity(myvel);
			SetSpeed(GetMaxSpeed());
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
		if(pEvent->GetEventID() == "damage")
		{
			CBullet* tempbullet = (CBullet*)pEvent->GetParam2();
			if(tempbullet->GetOwner() != this)
			{
			float damage = tempbullet->GetDamage();
			if(tempbullet->GetSlowRate() != 0.0f)
				SetSpeed(GetSpeed()*.75f);
			if(GetShieldBar() > 0)
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
				{
					tempbullet->GetOwner()->SetKillCount(tempbullet->GetOwner()->GetKillCount() + 1);
					CGamePlayState::GetInstance()->SortScores();
				}
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
			SetHealth(GetMaxHealth());
			
		}
		else if( pEvent->GetEventID() == "powerup_shield")
		{
			if( GetShieldBar() < GetMaxShield() )
			{
				SetShieldBar(GetMaxShield());
			}
		}
	}
	
}