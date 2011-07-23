/////////////////////////////////////////////////
//	File	:	"IAIBaseState.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Pure virtual class which all AI
//				states inheriet from
//				
/////////////////////////////////////////////////

#include "CWanderState.h"
#include "CSGD_Direct3D.h"
#include <cmath>
#include "CLevel.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CAttackState.h"
#include "CNumPlayers.h"
#include "CCharacterSelection.h"
#include "CCar.h"
#include "CGamePlayState.h"


CWanderState::CWanderState(const CWanderState&)
{

}

CWanderState& CWanderState::operator=(const CWanderState&)
{
	return *this;
}

void CWanderState::Update (float fElapsedTime)
{
	if (m_Owner)
	{
		m_Owner->SetSpeed (m_Owner->GetSpeed () + 02.0f);

		if (m_Owner->GetSpeed () > 150.0f)
		{
			m_Owner->SetSpeed (150.0f);
		}
	}


	//////if(m_nCounter%60 == 1)
	//////{
	//////m_vMainCenter.fX = m_Owner->GetPosX() +  m_Owner->GetWidth () / 2;
	//////m_vMainCenter.fY = (m_Owner->GetPosY() - m_fMainCircleRadius);

	////////m_vDirectionCenter.fX = m_vMainCenter.fX;
	////////m_vDirectionCenter.fY = (m_vMainCenter.fY - m_fMainCircleRadius);

	////////find new random point in the little circle;
	//////tVector2D newPoint;

	//////newPoint.fX = (((rand()/(float)RAND_MAX)*((100)-(-10)))+(-10));
	//////newPoint.fY = (((rand()/(float)RAND_MAX)*((100)-(-10)))+(-10));

	//////// move the direction circles center to the new point
	//////m_vDirectionCenter = m_vDirectionCenter + newPoint;

	////////m_fNewPointRadius = sqrt((m_vDirectionCenter.fX - m_vMainCenter.fX) * (m_vDirectionCenter.fX - m_vMainCenter.fX)) + ((m_vDirectionCenter.fY - m_vMainCenter.fY) + (m_vDirectionCenter.fY - m_vMainCenter.fY));

	//////tVector2D CP;
	//////CP.fX = m_vDirectionCenter.fX - m_vMainCenter.fX;
	//////CP.fY = m_vDirectionCenter.fY - m_vMainCenter.fY;

	////////find the angle of the new direction circle
	//////m_fAngle = atan (CP.fY/ CP.fX);

	//////if ((CP.fX < 0 && CP.fY > 0) || (CP.fX < 0 && CP.fY < 0))
	//////{
	//////	m_fAngle = 3.14159265f + m_fAngle; // change to pie later
	//////}

	//////if (CP.fX > 0 && CP.fY < 0)
	//////{
	//////	m_fAngle = 6.28318531f + m_fAngle; // change to 2pi later
	//////}

	////////set the direction circle on the main circles radius
	//////// or in better terms constrain the direction circle the the main circles radius
	//////m_vDirectionCenter.fX = m_vMainCenter.fX + (m_fMainCircleRadius * cos (m_fAngle));
	//////m_vDirectionCenter.fY = m_vMainCenter.fY + (m_fMainCircleRadius * sin (m_fAngle));

	//////tVector2D tempdir = m_Owner->GetDirection();
	//////tVector2D newdir;
	//////newdir.fX = m_vDirectionCenter.fX - (m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f));
	//////newdir.fY = m_vDirectionCenter.fY - (m_Owner->GetPosY()+(m_Owner->GetHeight()*.5f));
	//////float directionRotation = AngleBetweenVectors(tempdir, newdir);
	//////tempdir = Vector2DRotate(tempdir, directionRotation);
	//////tempdir = tempdir * m_Owner->GetSpeed();
	//////m_Owner->SetDirection(tempdir);
	//////float turnLeftOrRight = Steering(tempdir, newdir); 
	//////if(turnLeftOrRight < 0.0f)
	//////	m_Owner->SetRotation(m_Owner->GetRotation()-directionRotation);
	//////else
	//////	m_Owner->SetRotation(m_Owner->GetRotation()+directionRotation);
	//////}
	//////m_nCounter++;
	//////if(m_nCounter > INT_MAX - 1)
	//////	m_nCounter = 0;
	
	
	
	//Wander
	if(m_PowerUpTarget)
	{
		if(m_PowerUpTarget->IsActive())
		{
			switch(m_PowerUpTarget->GetPowerUpType())
			{
				case SHIELD_POWERUP:
					{
						if(m_Owner->GetMaxShield() *.5f > m_Owner->GetShieldBar())
							GrabPowerUp(fElapsedTime);
						else
						{
							m_PowerUpTarget = NULL;
							Wander(fElapsedTime);
						}
					}
					break;
				case HEALTH_POWERUP:
					{
						if(m_Owner->GetMaxHealth() * .7 > m_Owner->GetHealth())
							GrabPowerUp(fElapsedTime);
						else
						{
							m_PowerUpTarget = NULL;
							Wander(fElapsedTime);
						}
					}
					break;
				case WEAPONS_POWERUP:
					{
						if(m_Owner->GetSpecialLevel() == 1)
							GrabPowerUp(fElapsedTime);
						else
						{
							m_PowerUpTarget = NULL;
							Wander(fElapsedTime);
						}
					}
					break;
				case SPECIAL_POWERUP:
					{
						if(m_Owner->GetMaxPowerUp() * .5f > m_Owner->GetPowerUpBar())
							GrabPowerUp(fElapsedTime);
						else
						{
							m_PowerUpTarget = NULL;
							Wander(fElapsedTime);
						}
					}
					break;
			}
		}
		else
		{
			m_PowerUpTarget = NULL;
			Wander(fElapsedTime);
		}
		
	}
	else if(m_SpeedRampTarget)
	{
		UseSpeedRamp(fElapsedTime);
	}
	else
	{
		Wander(fElapsedTime);
		m_PowerUpTarget = NULL;
		m_SpeedRampTarget = NULL;
	}
	//Try to find threats
	if(FindThreat())
	{
		m_Owner->ChangeState(m_Owner->GetAttackState());
		m_PowerUpTarget = NULL;
		m_SpeedRampTarget = NULL;
	}
	else
	{
		FindPowerUps(fElapsedTime);
		if(!m_PowerUpTarget)
			FindSpeedRamp(fElapsedTime);
	}
	
	
	
}

void CWanderState::Render ()
{

}

void CWanderState::Enter ()
{
	//m_Owner->SetRotationRate ();
	//m_Owner->SetMaxSpeed ();
	//m_Owner->s
	//m_nThreatDistance = 0;

	/*m_fMainCircleRadius = 100;

	m_vMainCenter.fX = m_Owner->GetPosX() +  m_Owner->GetWidth () / 2;
	m_vMainCenter.fY = (m_Owner->GetPosY() - m_fMainCircleRadius);

	m_vDirectionCenter.fX = m_vMainCenter.fX;
	m_vDirectionCenter.fY = (m_vMainCenter.fY - m_fMainCircleRadius);
	m_nCounter = 1;*/
	/*if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
	{
		m_Target1 = CCharacterSelection::GetInstance()->GetPlayer1();
		m_Target2 = CCharacterSelection::GetInstance()->GetPlayer2();
	}
	else
	{
		m_Target1 = CCharacterSelection::GetInstance()->GetPlayer1();
		m_Target2 = NULL;
	}*/

	m_Target1 = NULL;
	m_PowerUpTarget = NULL;
	m_SpeedRampTarget = NULL;
	m_bHasTargets = false;
	m_fTargetX = 0.0f;
	m_fTargetY = 0.0f;
	m_fRotationAngle = 0.0f;
	m_fturnLeftOrRight = 0.0f;
	m_fAggroRadius = 450.0f;
}


void CWanderState::Exit ()
{

}

bool CWanderState::FindThreat()
{
	if (m_Owner)
	{
		std::vector <CBase*> cars = CGamePlayState::GetInstance ()->GetCars ();

		for (size_t index = 0; index < cars.size (); index++)
		{
			tVector2D target1Distance;
			m_Target1 = (CCar*)cars[index];

			if (m_Owner != m_Target1)
			{
				if(m_Target1)
				{
					target1Distance.fX = (m_Target1->GetPosX()-(m_Owner->GetPosX()));
					target1Distance.fY = (m_Target1->GetPosY()-(m_Owner->GetPosY()));
				}

				if(Vector2DLength(target1Distance) <= m_fAggroRadius)
				{
					m_Owner->GetAttackState ()->SetTarget(m_Target1);
					return true;
				}
			}
		}
	}

	return false;
	//if(m_Target2)
	//{
	//	target2Distance.fX = (m_Target2->GetPosX()-(m_Owner->GetPosX()));
	//	target2Distance.fY = (m_Target2->GetPosY()-(m_Owner->GetPosY()));
	//
	//	if(Vector2DLength(target1Distance) <= m_fAggroRadius || Vector2DLength(target2Distance) <= m_fAggroRadius)
	//	{
	//		if(Vector2DLength(target1Distance) <= Vector2DLength(target2Distance))
	//			m_Owner->GetAttackState ()->SetTarget(m_Target1);
	//		else
	//			m_Owner->GetAttackState ()->SetTarget(m_Target2);
	//		return true;
	//	}
	//	else
	//		return false;
	//}
	//else 
	//{
		
	//}

}

void CWanderState::Wander(float fElapsedTime)
{
	if(!m_bHasTargets)
	{
		
		//tVector2D m_vTargetLocation;
		m_fTargetX = float(rand()%1500+100);						// so that it works on every map GetTileWidth()*num of tiles horizontally - 100 (since we are using middle of car for position calculations)
		m_fTargetY = float(rand()%1500+100);						// so that it works on every map GetTileHeight()*num of tiles vertically - 100 (since we are using middle of car for position calculations)
		m_bHasTargets = true;

	}
	else
	{
		tVector2D TargetVector;

		TargetVector.fX = m_fTargetX - (m_Owner->GetPosX());
		TargetVector.fY = m_fTargetY - (m_Owner->GetPosY());

		tVector2D currentEnemyDirection = m_Owner->GetDirection();
	
		m_Owner->SetDirection(TargetVector);
		m_fRotationAngle = AngleBetweenVectors(currentEnemyDirection, TargetVector);
		m_fturnLeftOrRight = Steering(currentEnemyDirection, TargetVector); 
		if(m_fturnLeftOrRight < 0.0f)
		{
			
			if(m_fRotationAngle - (m_Owner->GetRotationRate() * fElapsedTime) < 0.0f)
			{
				m_Owner->SetRotation(m_Owner->GetRotation()-m_fRotationAngle);
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, m_Owner->GetRotation());
				m_Owner->SetDirection(direction);
				m_fRotationAngle = 0.0f;
			}
			else
			{
				m_Owner->SetRotation(m_Owner->GetRotation()-(m_Owner->GetRotationRate() * fElapsedTime));
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, m_Owner->GetRotation());
				m_Owner->SetDirection(direction);
				m_fRotationAngle -= (m_Owner->GetRotationRate() * fElapsedTime);
			}
			m_Owner->Rotate(m_Owner->GetRotation());
		}
		else
		{
			
			if(m_fRotationAngle - (m_Owner->GetRotationRate() * fElapsedTime) < 0.0f)
			{
				m_Owner->SetRotation(m_Owner->GetRotation()+m_fRotationAngle);
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, m_Owner->GetRotation());
				m_Owner->SetDirection(direction);
				m_fRotationAngle = 0.0f;
			}
			else
			{
				m_Owner->SetRotation(m_Owner->GetRotation()+(m_Owner->GetRotationRate() * fElapsedTime));
				tVector2D direction;
				direction.fX = 0.0f;
				direction.fY = -1.0f;
				direction = Vector2DRotate(direction, m_Owner->GetRotation());
				m_Owner->SetDirection(direction);
				m_fRotationAngle -= (m_Owner->GetRotationRate() * fElapsedTime);
			}
			m_Owner->Rotate(m_Owner->GetRotation());
		}

		if(currentEnemyDirection.fX < 0.0f && currentEnemyDirection.fY < 0.0f)
		{
	
			if((m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f) <= m_fTargetX) && (m_Owner->GetPosY() + (m_Owner->GetHeight()*.5f) <= m_fTargetY))
				m_bHasTargets = false;
		}
		else if(currentEnemyDirection.fX < 0.0f && currentEnemyDirection.fY >= 0.0f)
		{
			if((m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f) <= m_fTargetX) && (m_Owner->GetPosY() + (m_Owner->GetHeight()*.5f) >= m_fTargetY))
				m_bHasTargets = false;
		}
		else if(currentEnemyDirection.fX >= 0.0f && currentEnemyDirection.fY < 0.0f)
		{
			if((m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f) >= m_fTargetX) && (m_Owner->GetPosY() + (m_Owner->GetHeight()*.5f) <= m_fTargetY))
				m_bHasTargets = false;
		}
		else if(currentEnemyDirection.fX >= 0.0f && currentEnemyDirection.fY >= 0.0f)
		{
			if((m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f) >= m_fTargetX) && (m_Owner->GetPosY() + (m_Owner->GetHeight()*.5f) >= m_fTargetY))
				m_bHasTargets = false;
		}

	}
}

bool CWanderState::FindPowerUps(float fElapsedTime)
{
	if(m_Owner)
	{
		float closestPowerUpDistance = 350.0f;
		tVector2D tempVector, TargetVector;
		//Get Distance from all power ups
		for(unsigned int i = 0; i < m_Owner->GetPowerUps().size(); i++)
		{
			tempVector.fX = (m_Owner->GetPowerUps()[i]->GetPosX()) - (m_Owner->GetPosX());
			tempVector.fY = (m_Owner->GetPowerUps()[i]->GetPosY()) - (m_Owner->GetPosY());
			if(Vector2DLength(tempVector) < closestPowerUpDistance)
			{
				closestPowerUpDistance = Vector2DLength(tempVector);
				TargetVector = tempVector;
				m_PowerUpTarget = m_Owner->GetPowerUps()[i];
			}
		}
		if(closestPowerUpDistance == 350.0f)
		{
			m_PowerUpTarget = NULL;
			return false;
		}
		else
			return true;
	}
	else
	{
		m_PowerUpTarget = NULL;
		return false;
	}
}

void CWanderState::GrabPowerUp(float fElapsedTime)
{
	tVector2D TargetVector;
	TargetVector.fX = (m_PowerUpTarget->GetPosX()) - (m_Owner->GetPosX());
	TargetVector.fY = (m_PowerUpTarget->GetPosY()) - (m_Owner->GetPosY());
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

void CWanderState::FindSpeedRamp(float fElapsedTime)
{
	if(!m_Owner->HasCollidedWithSpeedRamp())
	{
		float closestSpeedRampDistance = 350.0f;
		tVector2D tempVector, TargetVector;
		//Check to see if speed ramps are within range
		for(unsigned int i = 0; i < m_Owner->GetSpeedRamps().size(); i++)
		{
			tempVector.fX = (m_Owner->GetSpeedRamps()[i]->GetPosX()) - (m_Owner->GetPosX());
			tempVector.fY = (m_Owner->GetSpeedRamps()[i]->GetPosY()) - (m_Owner->GetPosY());
			if(Vector2DLength(tempVector) < closestSpeedRampDistance)
			{
				closestSpeedRampDistance = Vector2DLength(tempVector);
				TargetVector = tempVector;
				m_SpeedRampTarget = m_Owner->GetSpeedRamps()[i];
			}
		}
		if(closestSpeedRampDistance == 350.0f)
		{
			m_SpeedRampTarget = NULL;
			return;
		}

	}
	else
		m_SpeedRampTarget = NULL;
}

void CWanderState::UseSpeedRamp(float fElapsedTime)
{
	tVector2D TargetVector;
	TargetVector.fX = (m_SpeedRampTarget->GetPosX()) - (m_Owner->GetPosX());
	TargetVector.fY = (m_SpeedRampTarget->GetPosY()) - (m_Owner->GetPosY());
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