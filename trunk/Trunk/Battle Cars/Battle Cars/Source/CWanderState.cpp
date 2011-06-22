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
	//set up prediction 


	//find nearest threat
	/*if (FindThreat())
	{

	}*/
	//apply direction force if nearest threat is found
	// decrease speed based on direction force

	// no threat found increase speed tell max speed is reached
	//calculate velocity

	//find new random point in the little circle;
	//tVector2D newPoint;

	//newPoint.fX = rand() % 3 - 1;
	//newPoint.fY = rand() % 3 - 1;

	// move the direction circles center to the new point
	//m_vDirectionCenter = m_vDirectionCenter + newPoint;

	//m_fNewPointRadius = sqrt((m_vDirectionCenter.fX - m_vMainCenter.fX) * (m_vDirectionCenter.fX - m_vMainCenter.fX)) + ((m_vDirectionCenter.fY - m_vMainCenter.fY) + (m_vDirectionCenter.fY - m_vMainCenter.fY));

	//find the angle of the new direction circle
	//m_fAngle = atan (m_vDirectionCenter.fY/ m_vDirectionCenter.fX);

	//set the direction circle on the main circles radius
	//m_vDirectionCenter.fX = m_fMainCircleRadius * sin (m_fAngle);
	//m_vDirectionCenter.fY = m_fMainCircleRadius * cos (m_fAngle);
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

	/*m_fMainCircleRadius = 3;

	m_vMainCenter.fX = m_Owner->GetPosX() / 2;
	m_vMainCenter.fY = m_fMainCircleRadius + (m_Owner->GetHeight () / 2);

	m_vDirectionCenter.fX = m_vMainCenter.fX;
	m_vMainCenter.fY = (-1) * (m_fMainCircleRadius + m_vMainCenter.fY);*/
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