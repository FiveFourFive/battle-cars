/////////////////////////////////////////////////
//	File	:	"CEnemy.cpp"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains all functions needed for
//				a simple AI
//				
/////////////////////////////////////////////////

#include "CEnemy.h"
#include "CCamera.h"
#include "CWanderState.h"
#include "CMainMenuState.h"
#include "CBullet.h"
#include "CSGD_Direct3D.h"
#include "CSpeedRamp.h"
#include "ParticleManager.h"
#include "Emittor.h"
#include "CGamePlayState.h"
#include "CLevel.h"
#include "CXboxInput.h"

CEnemy::CEnemy(CXboxInput* pController) : CPlayer(pController)
{
	m_nType = OBJECT_ENEMY;
	m_pES = CEventSystem::GetInstance ();

	//m_pES->RegisterClient ("CameraCollision", this);

	m_WanderState = new CWanderState();
	m_FleeState = new CFleeState();
	m_AttackState = new CAttackState();
	m_CollectState = new CCollectState();

	m_WanderState->SetOwner (this);
	m_FleeState->SetOwner (this);
	m_AttackState->SetOwner (this);
	m_CollectState->SetOwner (this);

	m_AICurrentState = m_WanderState;

	EnterState();
	m_fViewRadius = 30.0f;

	CEventSystem::GetInstance()->RegisterClient("collision", this);
	SetCollisionEffect(false);
	SetMaxHealth(150.0);
	SetShieldBar(50.0f);
	SetMaxShield(50.0f);
	SetHealth(150.0f);
	m_fSpeedRampTimer = 0.0f;
	m_bHasCollidedWithSpeedRamp = false;
}

CEnemy::~CEnemy()
{
	CEventSystem::GetInstance()->UnregisterClient("collision", this);

	delete	m_WanderState;
	delete m_FleeState;
	delete m_AttackState;
	delete m_CollectState;
}

void CEnemy::EnterState ()
{
	if (m_AICurrentState)
	{
			m_AICurrentState->Enter ();
	}
}

void CEnemy::Update(float fElapsedTime)
{
	CLevel::GetInstance ()->CheckEnemyCollision (this);

	if (m_AICurrentState)
		m_AICurrentState->Update (fElapsedTime);
	if(m_bHasCollidedWithSpeedRamp)
	{
		m_fSpeedRampTimer += fElapsedTime;
		if(m_fSpeedRampTimer >= 5.0f)
		{
			m_fSpeedRampTimer = 0.0f;
			m_bHasCollidedWithSpeedRamp = false;
		}
	}
	CCar::Update (fElapsedTime);
}

void CEnemy::Render(CCamera* camera)
{
	CCar::Render (camera);

	if (m_AICurrentState)
		m_AICurrentState->Render ();

}

void CEnemy::ChangeState (IAIBaseState* state)
{
	if(m_AICurrentState)
	{
		m_AICurrentState->SetOwner (NULL);
		m_AICurrentState->Exit();
	}

	 m_AICurrentState = state;

	if(m_AICurrentState)
	{
		m_AICurrentState->SetOwner (this);
		m_AICurrentState->Enter();
	}
}

void CEnemy::HandleEvent(CEvent* pEvent)
{
	if(this == pEvent->GetParam())
	{
		if(pEvent->GetEventID() == "damage")
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
				{
					CGamePlayState::GetInstance()->SortScores();
					tempbullet->GetOwner()->SetKillCount(tempbullet->GetOwner()->GetKillCount() + 1);
				}
			}
			}
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
		
		else if(pEvent->GetEventID() == "weapon_level")
		{
			if(GetSpecialLevel() < 4)
				SetSpecialLevel(GetSpecialLevel() + 1);
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

bool CEnemy::CheckCollision(IBaseInterface* pBase)
{
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
if(pBase->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)pBase;
			tempplayer->GetController()->Vibrate(30000,30000);
		}
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
	RECT intersection;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_SPEEDRAMP)
		{
			SetSpeed(0);
			CSpeedRamp* tempramp = (CSpeedRamp*)pBase;
			SetVelocity((tempramp->GetVelDir() * (GetMaxSpeed() + 50) ));
			m_bHasCollidedWithSpeedRamp = true;
			return true;
		}
	}
	return false;
}

void CEnemy::AddCollisionRect (RECT collision)
{
	m_CollisionRECT.push_back (collision);
}