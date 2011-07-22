#include "CCollectState.h"
#include "CEnemy.h"
#include "CGamePlayState.h"
#include "CCollectable.h"
#include "CCar.h"

CCollectState::CCollectState(const CCollectState&)
{

}

CCollectState& CCollectState::operator=(const CCollectState&)
{
	return *this;
}


void CCollectState::Update (float fElapsedTime)
{
	m_Owner->SetSpeed (m_Owner->GetSpeed () + m_Owner->GetAcceleration());

	if (m_Owner->GetSpeed () > m_Owner->GetMaxSpeed())
	{
		m_Owner->SetSpeed (m_Owner->GetMaxSpeed());
	}
	tVector2D TargetVector, tempDistance;
	if(!m_bHasTargets)
	{
		TargetVector.fX = 5000.0f;
		TargetVector.fY = 5000.0f;
		for(unsigned int i = 0; i < collectables.size(); i++)
		{
			if(collectables[i]->IsActive())
			{
				tempDistance.fX = collectables[i]->GetPosX()+(collectables[i]->GetWidth()/2) - m_Owner->GetPosX();
				tempDistance.fY = collectables[i]->GetPosY()+(collectables[i]->GetHeight()/2) - m_Owner->GetPosY();
				if(Vector2DLength(tempDistance) < Vector2DLength(TargetVector))
				{
					TargetVector = tempDistance;
				}		
			}
		}
		m_bHasTargets = true;
	}
	else
	{
		if(!collectables[m_nTarget]->IsActive())
		{
			TargetVector.fX = 5000.0f;
			TargetVector.fY = 5000.0f;
			for(unsigned int i = 0; i < collectables.size(); i++)
			{
				if(collectables[i]->IsActive())
				{
					tempDistance.fX = collectables[i]->GetPosX()+(collectables[i]->GetWidth()/2) - m_Owner->GetPosX();
					tempDistance.fY = collectables[i]->GetPosY()+(collectables[i]->GetHeight()/2) - m_Owner->GetPosY();
					if(Vector2DLength(tempDistance) < Vector2DLength(TargetVector))
					{
						TargetVector = tempDistance;
					}		
				}
			}
		}
		else
		{
			TargetVector.fX = collectables[m_nTarget]->GetPosX() - m_Owner->GetPosX();
			TargetVector.fY = collectables[m_nTarget]->GetPosY() - m_Owner->GetPosY();
		}
	}
	tVector2D currentEnemyDirection = m_Owner->GetDirection();
	m_Owner->SetDirection(TargetVector);
	float m_fRotationAngle = AngleBetweenVectors(currentEnemyDirection, TargetVector);
	float m_fturnLeftOrRight = Steering(currentEnemyDirection, TargetVector); 
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
}

void CCollectState::Render ()
{

}

void CCollectState::Enter ()
{
	collectables = CGamePlayState::GetInstance()->GetCollectables();
	m_bHasTargets = false;
	m_nTarget = 0;
}


void CCollectState::Exit ()
{

}
