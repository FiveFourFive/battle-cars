/////////////////////////////////////////////////
//	File	:	"CAttackState.cpp"
//
//	Author	:	John Rostick
//
//	Purpose	:	Enemy Attack State definitions
//				
//				
/////////////////////////////////////////////////

#include "CAttackState.h"
#include "CWanderState.h"
#include "CFleeState.h"
#include "CSGD_Direct3D.h"
#include <cmath>
#include "CLevel.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CMessageSystem.h"

CAttackState::CAttackState(const CAttackState&)
{

}

CAttackState& CAttackState::operator=(const CAttackState&)
{
	return *this;
}

CAttackState* CAttackState::GetInstance()
{
	static CAttackState instance;
	return &instance;
}

void CAttackState::Update (float fElapsedTime)
{
	//Handle acceleration and speed
	m_Owner->SetSpeed (m_Owner->GetSpeed () + 2.50f);

	if (m_Owner->GetSpeed () > 150.0f)
	{
		m_Owner->SetSpeed (150.0f);
	}
	if(!StillThreat())
		m_Owner->ChangeState(CWanderState::GetInstance());
	Chase(fElapsedTime);
	//Fire Weapons
	m_fFireTimer += fElapsedTime;
	if(m_fFireTimer >= m_fFireRate)
	{
		m_fFireTimer = 0.0f;
		CMessageSystem::GetInstance()->SendMsg(new CCreateEnemyBulletMessage(m_Owner));
	}
	if(Damaged())
	{
		CFleeState::GetInstance()->SetTarget(m_Target);
		m_Owner->ChangeState(CFleeState::GetInstance());
	}

}

void CAttackState::Render ()
{

}

void CAttackState::Enter ()
{
	m_fAggroRadius = 650.0f;
	m_fFireRate = 1.0f;
	m_fFireTimer = 0.0f;
}


void CAttackState::Exit ()
{

}

void CAttackState::Chase(float fElapsedTime)
{
	tVector2D TargetVector;
	if(m_Target && m_Owner)
	{
	TargetVector.fX = (m_Target->GetPosX()) - (m_Owner->GetPosX());
	TargetVector.fY = (m_Target->GetPosY()) - (m_Owner->GetPosY());
	tVector2D currentEnemyDirection = m_Owner->GetDirection();
	float rotationAngle = AngleBetweenVectors(currentEnemyDirection, TargetVector);
	float turnLeftOrRight = Steering(currentEnemyDirection, TargetVector);
	if(turnLeftOrRight < 0.0f)
	{
		if(rotationAngle - (m_Owner->GetRotationRate() * fElapsedTime) < 0.0f)
		{
			m_Owner->SetRotation(m_Owner->GetRotation()-rotationAngle);
			tVector2D direction;
			direction.fX = 0.0f;
			direction.fY = -1.0f;
			direction = Vector2DRotate(direction, m_Owner->GetRotation());
			m_Owner->SetDirection(direction);
			rotationAngle = 0.0f;
		}
		else
		{
			m_Owner->SetRotation(m_Owner->GetRotation()-(m_Owner->GetRotationRate() * fElapsedTime));
			tVector2D direction;
			direction.fX = 0.0f;
			direction.fY = -1.0f;
			direction = Vector2DRotate(direction, m_Owner->GetRotation());
			m_Owner->SetDirection(direction);
			rotationAngle -= (m_Owner->GetRotationRate() * fElapsedTime);
		}
		m_Owner->Rotate(m_Owner->GetRotation());
	}
	else
	{
		if(rotationAngle - (m_Owner->GetRotationRate() * fElapsedTime) < 0.0f)
		{
			m_Owner->SetRotation(m_Owner->GetRotation()+rotationAngle);
			tVector2D direction;
			direction.fX = 0.0f;
			direction.fY = -1.0f;
			direction = Vector2DRotate(direction, m_Owner->GetRotation());
			m_Owner->SetDirection(direction);
			rotationAngle = 0.0f;
		}
		else
		{
			m_Owner->SetRotation(m_Owner->GetRotation()+(m_Owner->GetRotationRate() * fElapsedTime));
			tVector2D direction;
			direction.fX = 0.0f;
			direction.fY = -1.0f;
			direction = Vector2DRotate(direction, m_Owner->GetRotation());
			m_Owner->SetDirection(direction);
			rotationAngle -= (m_Owner->GetRotationRate() * fElapsedTime);
		}
		m_Owner->Rotate(m_Owner->GetRotation());
	}
	}
}

bool CAttackState::StillThreat()
{
	tVector2D target1Distance;
	if(m_Target && m_Owner)
	{
		target1Distance.fX = (m_Target->GetPosX())-(m_Owner->GetPosX());
		target1Distance.fY = (m_Target->GetPosY())-(m_Owner->GetPosY());
		if(Vector2DLength(target1Distance) <= m_fAggroRadius)
		{
			if(Vector2DLength(target1Distance) <= 225.0f)
				m_Owner->SetSpeed(0.0f);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool CAttackState::Damaged()
{
	if(m_Owner)
	{
		if(m_Owner->GetHealth() <= 30.0f)
		{
			return true;
		}
		else
			return false;
	}
	return false;
}