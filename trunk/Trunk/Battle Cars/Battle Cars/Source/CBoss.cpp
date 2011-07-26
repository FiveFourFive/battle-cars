#include "CBoss.h"
#include "CCamera.h"
#include "ParticleManager.h"
#include "Emittor.h"
#include "CBullet.h"
#include "CMessageSystem.h"
#include "CNumPlayers.h"
#include "CCharacterSelection.h"
#include "CSGD_TextureManager.h"
#include "CGamePlayState.h"

CBoss::CBoss(CXboxInput* pController) : CEnemy(pController)
{
	m_nType = OBJECT_BOSS;
	if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
	{
		m_Player1 = CCharacterSelection::GetInstance()->GetPlayer1();
		m_Player2 = CCharacterSelection::GetInstance()->GetPlayer2();
	}
	else
	{
		m_Player1 = CCharacterSelection::GetInstance()->GetPlayer1();
		m_Player2 = NULL;
	}
	this->SetAcceleration(2.0f);
	this->SetArmor(100.0f);
	this->SetCarId(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_BossPlaceHolder.png"));
	this->SetHealth(200.0f);
	this->SetShieldBar(100.0f);
	this->SetMaxShield(100.0f);
	this->SetMaxSpeed(175.0f);
	this->SetMaxPowerUp(100.0f);
	this->SetPosX(float(rand()%1400+200));
	this->SetPosY(float(rand()%1400+200));
	this->SetPlayerType(rand()%CAR_NUM);
	this->SetSpecialLevel(1);
	this->SetRotationRate(6.28f);
	SetWidth(52);
	SetHeight(70);
	SetImageWidth(256);
	SetImageHeight(256);
	SetRadius((float)(GetWidth()/2.0f));
	SetCollisionX1(GetPosX());
	SetCollisionY1(GetPosY() - (GetHeight()*0.5f) + (GetWidth()*0.5f));
	SetCollisionX2(GetPosX());
	SetCollisionY2(GetPosY() + (GetHeight()*0.5f) - (GetWidth()*0.5f));

	m_bHasTarget = false;
	m_bGatlingIsOn = false;
	m_bFlameThrowerIsOn = false;
	m_bIsMiniBoss = false;
	m_bIsEnraged = false;
	m_nFireBullets = 0;
	m_nIceBullets = 0;
	m_nTarget = 1;
	m_fFireTimer = 0.0f;
	m_fIceTimer = 0.0f;
}

CBoss::~CBoss()
{
	
}

void CBoss::Update(float fElapsedTime)
{
	if( GetHealth() <= 0.0f )
	{
		SetRespawnTimer( GetRespawnTimer() + fElapsedTime);
		return;
	}

	//THis class needs to find the closest player and attack.
	// it will fire three rounds at a time and act in the same manner as the attack state 
	// except for it won't flee.  
	// it will also get enraged with 30% life left which will give it faster movement and more power.
	SetSpeed (GetSpeed () + 20.0f);

	if (GetSpeed () > GetMaxSpeed())
	{
		SetSpeed (GetMaxSpeed());
	}
	

	//Find the closest target and get a vector to him
	SelectTarget();
	//Go after the target
	ChaseTarget(fElapsedTime);
	//Fire at the target
	FireAtTarget(fElapsedTime);

	CCar::Update (fElapsedTime);
	if(GetHealth() < 0.0f)
		CMessageSystem::GetInstance()->SendMsg(new CDestroyBossMessage(this));
}

void CBoss::Render(CCamera* camera)
{
	CCar::Render(camera);
}

void CBoss::AddCollisionRect(RECT collision)
{
	GetCollisionRects().push_back(collision);
}

void CBoss::HandleEvent(CEvent* pEvent)
{
	if(this == pEvent->GetParam())
	{
		if(pEvent->GetEventID() == "collision")
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
		else if(pEvent->GetEventID() == "damage")
		{
			CBullet* tempbullet = (CBullet*)pEvent->GetParam2();
			if(tempbullet->GetOwner() != this)
			{
				float damage = tempbullet->GetDamage();
				if(tempbullet->GetSlowRate() != 0.0f)
					SetSpeed(GetSpeed()*.75f);
				if(GetShieldBar() >= 0)
					SetShieldBar(GetShieldBar() - damage);
				else
					SetHealth(GetHealth() - damage);
				if(GetHealth() < 0)
				{
					if(GetIsAlive() == true)
					{
						tempbullet->GetOwner()->SetKillCount(tempbullet->GetOwner()->GetKillCount() + 1);
						CGamePlayState::GetInstance()->SortScores();
					}
				}
				if(!m_bIsEnraged && !m_bIsMiniBoss)
				{
					if(GetHealth() < (GetMaxHealth()*.3f))
					{
						SetSpecialLevel(this->GetSpecialLevel() + 1);
						SetMaxSpeed(GetMaxSpeed()*1.3f);
						if(GetSpecialLevel() > 3)
							SetSpecialLevel(3);
						m_bIsEnraged = true;
					}
				}
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
				SetShieldBar(GetShieldBar() + 20.0f);
			}
			if( GetShieldBar() > GetMaxShield() )
				SetShieldBar(GetMaxShield());
		}
	}
}

bool CBoss::CheckCollision(IBaseInterface* pBase)
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
			SetVelocity((tempramp->GetVelDir() * (GetMaxSpeed() + 50) ));
			return true;
		}
	}
	return false;
}

void CBoss::SelectTarget()
{
	if(!m_bHasTarget)
	{
		tVector2D target1Distance, target2Distance;
		if(m_Player2)
		{
			target1Distance.fX = m_Player1->GetPosX() - GetPosX();
			target1Distance.fY = m_Player1->GetPosY() - GetPosY();
			target2Distance.fX = m_Player2->GetPosX() - GetPosX();
			target2Distance.fY = m_Player2->GetPosY() - GetPosY();
			if(Vector2DLength(target1Distance) < Vector2DLength(target2Distance))
				m_nTarget = 1;
			else
				m_nTarget = 2;
		}
		else
		{
				m_nTarget = 1;
		}
		m_bHasTarget = true;

	}
	else
	{
		if(m_Player2)
		{
			if(m_Player1->GetHealth() < (m_Player1->GetMaxHealth()*.3f) && m_Player1->GetHealth() < m_Player2->GetHealth())
				m_nTarget = 1;
			else if(m_Player2->GetHealth() < (m_Player2->GetMaxHealth()*.3f) && m_Player2->GetHealth() < m_Player2->GetHealth())
				m_nTarget = 2;
		}
	}
}

void CBoss::ChaseTarget(float fElapsedTime)
{
	tVector2D TargetVector;
	tVector2D currentDirection = GetDirection();
	if(m_nTarget == 1 && m_Player1)
	{
		TargetVector.fX = m_Player1->GetPosX() - GetPosX();
		TargetVector.fY = m_Player1->GetPosY() - GetPosY();
		if(Vector2DLength(TargetVector) < 130.0f)
			SetSpeed(0.0f);
		float rotationAngle = AngleBetweenVectors(currentDirection, TargetVector);
		float turnLeftOrRight = Steering(currentDirection, TargetVector);
		if(turnLeftOrRight < 0.0f)
		{
			if(rotationAngle - (GetRotationRate() * fElapsedTime) < 0.0f)
			{
				SetRotation(GetRotation()-rotationAngle);
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, GetRotation());
				SetDirection(direction);
				rotationAngle = 0.0f;
			}
			else
			{
				SetRotation(GetRotation()-(GetRotationRate() * fElapsedTime));
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, GetRotation());
				SetDirection(direction);
				rotationAngle -= (GetRotationRate() * fElapsedTime);
			}
			Rotate(GetRotation());
		}
		else
		{
			if(rotationAngle - (GetRotationRate() * fElapsedTime) < 0.0f)
			{
				SetRotation(GetRotation()+rotationAngle);
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction,GetRotation());
				SetDirection(direction);
				rotationAngle = 0.0f;
			}
			else
			{
				SetRotation(GetRotation()+(GetRotationRate() * fElapsedTime));
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, GetRotation());
				SetDirection(direction);
				rotationAngle -= (GetRotationRate() * fElapsedTime);
			}
			Rotate(GetRotation());
		}
	}
	else if(m_nTarget == 2 && m_Player2)
	{
		TargetVector.fX = m_Player2->GetPosX() - GetPosX();
		TargetVector.fY = m_Player2->GetPosY() - GetPosY();
		if(Vector2DLength(TargetVector) < 225.0f)
			SetSpeed(0.0f);
		float rotationAngle = AngleBetweenVectors(currentDirection, TargetVector);
		float turnLeftOrRight = Steering(currentDirection, TargetVector);
		if(turnLeftOrRight < 0.0f)
		{
			if(rotationAngle - (GetRotationRate() * fElapsedTime) < 0.0f)
			{
				SetRotation(GetRotation()-rotationAngle);
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, GetRotation());
				SetDirection(direction);
				rotationAngle = 0.0f;
			}
			else
			{
				SetRotation(GetRotation()-(GetRotationRate() * fElapsedTime));
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, GetRotation());
				SetDirection(direction);
				rotationAngle -= (GetRotationRate() * fElapsedTime);
			}
			Rotate(GetRotation());
		}
		else
		{
			if(rotationAngle - (GetRotationRate() * fElapsedTime) < 0.0f)
			{
				SetRotation(GetRotation()+rotationAngle);
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction,GetRotation());
				SetDirection(direction);
				rotationAngle = 0.0f;
			}
			else
			{
				SetRotation(GetRotation()+(GetRotationRate() * fElapsedTime));
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, GetRotation());
				SetDirection(direction);
				rotationAngle -= (GetRotationRate() * fElapsedTime);
			}
			Rotate(GetRotation());
		}
	}
}

void CBoss::FireAtTarget(float fElapsedTime)
{
	if(m_bFlameThrowerIsOn)
	{
		m_nFireBullets--;
		CMessageSystem::GetInstance()->SendMsg(new CCreateBossVetteSpecial(this));
		if(m_nFireBullets <= 0)
			m_bFlameThrowerIsOn = false;
	}
	else if(m_bGatlingIsOn)
	{
		m_fIceTimer += fElapsedTime;
		if(m_fIceTimer >= .3f)
		{
			m_nIceBullets--;
			CMessageSystem::GetInstance()->SendMsg(new CCreateBossHummerSpecial(this));
			if(m_nIceBullets <= 0)
				m_bGatlingIsOn = false;
			m_fIceTimer = 0.0f;
		}
	}

	m_fFireTimer += fElapsedTime;
	if(m_fFireTimer > 3.0f)
	{
		m_fFireTimer = 0.0f;
		switch(GetPlayerType())
		{
		case CAR_MINI:
			{
				CMessageSystem::GetInstance()->SendMsg(new CCreateBossMiniSpecial(this));
			}
			break;
		case CAR_VETTE:
			{
				CMessageSystem::GetInstance()->SendMsg(new CCreateBossVetteSpecial(this));
				m_bFlameThrowerIsOn = true;
				m_nFireBullets = 20;
			}
			break;
		case CAR_HUMMER:
			{
				CMessageSystem::GetInstance()->SendMsg(new CCreateBossHummerSpecial(this));
				m_bGatlingIsOn = true;
				m_nIceBullets = 5;
			}
			break;
		case CAR_TRUCK:
			{
				CMessageSystem::GetInstance()->SendMsg(new CCreateBossTruckSpecial(this));
			}
			break;
		}
	}
	
}