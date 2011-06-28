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

CEnemy::CEnemy()
{
	m_nType = OBJECT_ENEMY;
	m_pES = CEventSystem::GetInstance ();

	m_pES->RegisterClient ("CameraCollision", this);

	m_AICurrentState = CWanderState::GetInstance();

	m_AICurrentState->SetOwner (this);

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

}

bool CEnemy::CheckCollision(IBaseInterface* pBase)
{
	return false;
}

void CEnemy::AddCollisionRect (RECT collision)
{
	m_CollisionRECT.push_back (collision);
}