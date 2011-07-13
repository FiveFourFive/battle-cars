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


CWanderState::CWanderState(const CWanderState&)
{

}

CWanderState& CWanderState::operator=(const CWanderState&)
{
	return *this;
}

CWanderState* CWanderState::GetInstance()
{
	static CWanderState instance;
	return &instance;
}

void CWanderState::Update (float fElapsedTime)
{
	m_Owner->SetSpeed (m_Owner->GetSpeed () + 10.0f);

	if (m_Owner->GetSpeed () > 150.0f)
	{
		m_Owner->SetSpeed (150.0f);
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
	Wander(fElapsedTime);
	//Try to find threats
	if(FindThreat())
		m_Owner->ChangeState(CAttackState::GetInstance());
	
	
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
	if(CNumPlayers::GetInstance()->GetNumberOfPlayers() > 1)
	{
		m_Target1 = CCharacterSelection::GetInstance()->GetPlayer1();
		m_Target2 = CCharacterSelection::GetInstance()->GetPlayer2();
	}
	else
	{
		m_Target1 = CCharacterSelection::GetInstance()->GetPlayer1();
		m_Target2 = NULL;
	}

	m_bHasTargets = false;
	m_fTargetX = 0.0f;
	m_fTargetY = 0.0f;
	m_fRotationAngle = 0.0f;
	m_fturnLeftOrRight = 0.0f;
	m_fAggroRadius = 650.0f;
}


void CWanderState::Exit ()
{

}

bool CWanderState::FindThreat()
{
	//if (CLevel::GetInstance ()->CheckEnemyCollision (m_Owner))
	//{
	//	/*m_rtPredictMove.left = m_vPredictMove1.fX;
	//	m_rtPredictMove.top = m_vPredictMove1.fY;

	//	m_rtPredictMove.right = m_vPredictMove3.fX;
	//	m_rtPredictMove.bottom = m_vPredictMove2.fY;*/

	//	for (int i = 0; i < m_Owner->GetCollisionRects ().size (); i++)
	//	{
	//		RECT intersection;
	//		float distance;

	//		//if (IntersectRect(&intersection, &m_Owner->GetCollisionRects ()[i], &m_rtPredictMove))
	//		{
	//			//find new distance
	//			RECT ownerTemp = m_Owner->GetRect ();

	//			ownerTemp.left = ownerTemp.right / 2;
	//			ownerTemp.top = ownerTemp.bottom / 2;

	//			RECT collisionTemp = m_Owner->GetCollisionRects ()[i];

	//			collisionTemp.left = collisionTemp.right / 2;
	//			collisionTemp.top = collisionTemp.bottom / 2;

	//			distance = sqrt ((float)((collisionTemp.left - ownerTemp.left) * (collisionTemp.left - ownerTemp.left)) + ((collisionTemp.right - ownerTemp.right) * (collisionTemp.right - ownerTemp.right)));

	//			/*if (m_nThreatDistance == 0)
	//			{
	//				m_nThreatDistance = distance;

	//			}else if (m_nThreatDistance > distance)
	//			{
	//				m_nThreatDistance = distance;
	//			}*/
	//		}
	//	}
	//	return true;
	//}


	tVector2D target1Distance, target2Distance;
	if(m_Target1)
	{
		target1Distance.fX = (m_Target1->GetPosX()+(m_Target1->GetWidth()*.5f))-(m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f));
		target1Distance.fY = (m_Target1->GetPosY()+(m_Target1->GetHeight()*.5f))-(m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f));
	}
	if(m_Target2)
	{
		target2Distance.fX = (m_Target2->GetPosX()+(m_Target2->GetWidth()*.5f))-(m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f));
		target2Distance.fY = (m_Target2->GetPosY()+(m_Target2->GetHeight()*.5f))-(m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f));
	
		if(Vector2DLength(target1Distance) <= m_fAggroRadius || Vector2DLength(target2Distance) <= m_fAggroRadius)
		{
			if(Vector2DLength(target1Distance) <= Vector2DLength(target2Distance))
				CAttackState::GetInstance()->SetTarget(m_Target1);
			else
				CAttackState::GetInstance()->SetTarget(m_Target2);
			return true;
		}
		else
			return false;
	}
	else 
	{
		if(Vector2DLength(target1Distance) <= m_fAggroRadius)
		{
			CAttackState::GetInstance()->SetTarget(m_Target1);
			return true;
		}
		else
			return false;
	}

}

void CWanderState::Wander(float fElapsedTime)
{
	if(!m_bHasTargets)
	{
		
		tVector2D m_vTargetLocation;
		m_fTargetX = rand()%1500+100;						// so that it works on every map GetTileWidth()*num of tiles horizontally - 100 (since we are using middle of car for position calculations)
		m_fTargetY = rand()%1500+100;						// so that it works on every map GetTileHeight()*num of tiles vertically - 100 (since we are using middle of car for position calculations)
		m_bHasTargets = true;

	}
	else
	{
		tVector2D TargetVector;

		TargetVector.fX = m_fTargetX - (m_Owner->GetPosX()+(m_Owner->GetWidth()*.5f));
		TargetVector.fY = m_fTargetY - (m_Owner->GetPosY() + (m_Owner->GetHeight()*.5f));

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