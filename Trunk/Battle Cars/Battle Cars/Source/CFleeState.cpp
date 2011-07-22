/////////////////////////////////////////////////
//	File	:	"CFleeState.cpp"
//
//	Author	:	John Rostick
//
//	Purpose	:	Enemy Flee State definitions
//				
//				
/////////////////////////////////////////////////

#include "CFleeState.h"
#include "CWanderState.h"
#include "CAttackState.h"
#include "CSGD_Direct3D.h"
#include <cmath>
#include "CLevel.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CMessageSystem.h"

CFleeState::CFleeState(const CFleeState&)
{

}

CFleeState& CFleeState::operator=(const CFleeState&)
{
	return *this;
}


void CFleeState::Update (float fElapsedTime)
{
	if( m_Owner->GetHealth() <= 0.0f)
		return;

	//Handle acceleration and speed
	m_Owner->SetSpeed (m_Owner->GetSpeed () + 2.0f);

	if (m_Owner->GetSpeed () > 150.0f)
	{
		m_Owner->SetSpeed (150.0f);
	}
	//Flee
	if(m_bFindingHealth)
		FindHealth(fElapsedTime);
	else
		Flee(fElapsedTime);
	//Check to see if healed
	if(!Damaged())
	{
			m_Owner->ChangeState(m_Owner->GetWanderState ());
	}
	//Check to see if far enough away
	if(Escaped())
		m_bFindingHealth = true;
	else
		m_bFindingHealth = false;
	
}

void CFleeState::Render ()
{

}

void CFleeState::Enter ()
{
	
}


void CFleeState::Exit ()
{

}

bool CFleeState::Damaged()
{
	if(m_Owner)
	{
	if(m_Owner->GetHealth() >= 70.0f)
		return false;
	return true;
	}
	else
		return true;
}

void CFleeState::Flee(float fElapsedTime)
{
	tVector2D TargetVector;
	if(m_Target && m_Owner)
	{
	TargetVector.fX = (m_Target->GetPosX()) - (m_Owner->GetPosX());
	TargetVector.fY = (m_Target->GetPosY()) - (m_Owner->GetPosY());
	tVector2D currentEnemyDirection = m_Owner->GetDirection();
	TargetVector = TargetVector * -1;
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

bool CFleeState::Escaped()
{
	tVector2D target1Distance;
	if(m_Target && m_Owner)
	{
		target1Distance.fX = (m_Target->GetPosX())-(m_Owner->GetPosX());
		target1Distance.fY = (m_Target->GetPosY())-(m_Owner->GetPosY());
		if(Vector2DLength(target1Distance) >= 650.0f)
			return true;
		else
			return false;
	}
	else
		return false;
}

void CFleeState::FindHealth(float fElapsedTime)
{
	tVector2D tempVector, TargetVector;
	float closestHealthPowerUp = 10000000.0f;
	if(m_Owner)
	{
		//Find closest health power up
		for(unsigned int i = 0; i < m_Owner->GetPowerUps().size(); i++)
		{
			if(m_Owner->GetPowerUps()[i]->GetPowerUpType() == HEALTH_POWERUP)
			{
				tempVector.fX = (m_Owner->GetPowerUps()[i]->GetPosX()) - (m_Owner->GetPosX());
				tempVector.fY = (m_Owner->GetPowerUps()[i]->GetPosY()) - (m_Owner->GetPosY());
				if(Vector2DLength(tempVector) < closestHealthPowerUp)
				{
					closestHealthPowerUp = Vector2DLength(tempVector);
					TargetVector = tempVector;
				}
			}
		}
		if(closestHealthPowerUp == 10000000.0f)
		{
			Flee(fElapsedTime);
			return;
		}
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
		}
	}
}