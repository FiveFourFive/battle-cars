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

CEnemy::CEnemy(CXboxInput* pController) : CPlayer(pController)
{
	m_nType = OBJECT_ENEMY;
	m_pES = CEventSystem::GetInstance ();

	m_pES->RegisterClient ("CameraCollision", this);

	m_AICurrentState = CWanderState::GetInstance();

	m_AICurrentState->SetOwner (this);
	EnterState();
	m_fViewRadius = 30.0f;
}

CEnemy::~CEnemy()
{

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
	if (m_AICurrentState)
		m_AICurrentState->Update (fElapsedTime);
	
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
		}
		
	}
}

bool CEnemy::CheckCollision(IBaseInterface* pBase)
{
	if(this == pBase)
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


		
			
		}
	}
	return false;
}

void CEnemy::AddCollisionRect (RECT collision)
{
	m_CollisionRECT.push_back (collision);
}