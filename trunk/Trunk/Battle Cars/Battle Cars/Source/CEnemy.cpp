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
	return false;
}

void CEnemy::AddCollisionRect (RECT collision)
{
	m_CollisionRECT.push_back (collision);
}