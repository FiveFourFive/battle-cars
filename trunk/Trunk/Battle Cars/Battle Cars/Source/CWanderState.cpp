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
	m_Owner->SetSpeed (m_Owner->GetSpeed () + 0.1f);

	if (m_Owner->GetSpeed () > 100.0f)
	{
		m_Owner->SetSpeed (100.0f);
	}

	m_vMainCenter.fX = m_Owner->GetPosX() +  m_Owner->GetWidth () / 2;
	m_vMainCenter.fY = (m_Owner->GetPosY() - m_fMainCircleRadius);

	//m_vDirectionCenter.fX = m_vMainCenter.fX;
	//m_vDirectionCenter.fY = (m_vMainCenter.fY - m_fMainCircleRadius);

	//find new random point in the little circle;
	tVector2D newPoint;

	newPoint.fX = rand() % 20 - 10;
	newPoint.fY = rand() % 20 - 10;

	// move the direction circles center to the new point
	m_vDirectionCenter = m_vDirectionCenter + newPoint;

	//m_fNewPointRadius = sqrt((m_vDirectionCenter.fX - m_vMainCenter.fX) * (m_vDirectionCenter.fX - m_vMainCenter.fX)) + ((m_vDirectionCenter.fY - m_vMainCenter.fY) + (m_vDirectionCenter.fY - m_vMainCenter.fY));

	tVector2D CP;
	CP.fX = m_vDirectionCenter.fX - m_vMainCenter.fX;
	CP.fY = m_vDirectionCenter.fY - m_vMainCenter.fY;

	//find the angle of the new direction circle
	m_fAngle = atan (CP.fY/ CP.fX);

	if ((CP.fX < 0 && CP.fY > 0) || (CP.fX < 0 && CP.fY < 0))
	{
		m_fAngle = 3.14159265f + m_fAngle; // change to pie later
	}

	if (CP.fX > 0 && CP.fY < 0)
	{
		m_fAngle = 6.28318531f + m_fAngle; // change to 2pi later
	}

	//set the direction circle on the main circles radius
	// or in better terms constrain the direction circle the the main circles radius
	m_vDirectionCenter.fX = m_vMainCenter.fX + (m_fMainCircleRadius * cos (m_fAngle));
	m_vDirectionCenter.fY = m_vMainCenter.fY + (m_fMainCircleRadius * sin (m_fAngle));

	tVector2D tempdir = m_vDirectionCenter;

	tempdir = Vector2DRotate(tempdir, m_fAngle);
	tempdir = tempdir * m_Owner->GetSpeed();
	m_Owner->SetDirection(tempdir);
}

void CWanderState::Render ()
{
	RECT tempcar;
	tempcar.left = (LONG)(m_vMainCenter.fX);
	tempcar.top = (LONG)(m_vMainCenter.fY);
	tempcar.right = (LONG)(tempcar.left + 20);
	tempcar.bottom = (LONG)(tempcar.top + 20);

	CSGD_Direct3D::GetInstance ()->DrawRect(tempcar, 0,255,0);

	tempcar.left = (LONG)(m_vDirectionCenter.fX );
	tempcar.top = (LONG)(m_vDirectionCenter.fY);
	tempcar.right = (LONG)(tempcar.left + 10);
	tempcar.bottom = (LONG)(tempcar.top + 10);

	CSGD_Direct3D::GetInstance ()->DrawRect(tempcar, 0,255,255);

//	for (float i = 0; i < 6.28f;)
	{

		//CSGD_Direct3D::GetInstance ()->DrawLine((int)(m_vDirectionCenter.fX), (int)(m_vDirectionCenter.fY), (int)(m_vDirectionCenter.fX), (int)(m_vDirectionCenter.fY), 0,255,0);
	}
}

void CWanderState::Enter ()
{
	//m_Owner->SetRotationRate ();
	//m_Owner->SetMaxSpeed ();
	//m_Owner->s
	//m_nThreatDistance = 0;

	m_fMainCircleRadius = 100;

	m_vMainCenter.fX = m_Owner->GetPosX() +  m_Owner->GetWidth () / 2;
	m_vMainCenter.fY = (m_Owner->GetPosY() - m_fMainCircleRadius);

	m_vDirectionCenter.fX = m_vMainCenter.fX;
	m_vDirectionCenter.fY = (m_vMainCenter.fY - m_fMainCircleRadius);
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
	return false;
}