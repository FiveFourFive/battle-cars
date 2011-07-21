#include "CObstacle.h"
#include "CCar.h"
#include "CLevel.h"

CObstacle::CObstacle(void)
{
	m_vCollisionVel.fX = 0;
	m_vCollisionVel.fY = 0;

}

CObstacle::~CObstacle(void)
{

}

void CObstacle::Update(float fElapsedTime)
{
	SetPosX(GetPosX() + (m_vCollisionVel.fX * fElapsedTime));
	SetPosY(GetPosY() + (m_vCollisionVel.fY * fElapsedTime));

	m_vCollisionVel.fX = m_vCollisionVel.fX - (m_vCollisionVel.fX * 0.01f);
	m_vCollisionVel.fY =m_vCollisionVel.fY - ( m_vCollisionVel.fY * 0.01f);

	CLevel::GetInstance ()->CheckObstacleCollision (this);

	InBounds();
}

void CObstacle::Render(CCamera* camera)
{
	CBase::Render(camera);
}

bool CObstacle::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;

	if(IntersectRect(&intersection,&this->GetRect(),&pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_PLAYER || pBase->GetType() == OBJECT_BOSS)
		{
			tVector2D myvel = m_vCollisionVel;
			CCar* tempcar = (CCar*)pBase;
			tVector2D hisvel = tempcar->GetOverallVelocity();

			myvel = myvel + (hisvel * 0.3f);
			//hisvel = (hisvel * 0.8f);
			SetVel(myvel);
			tempcar->SetSpeed(tempcar->GetSpeed() * 0.6f);
			return true;
		}
		else if(pBase->GetType() == OBJECT_OBSTACLE)
		{
			if(pBase != this)
			{
			tVector2D myvel = m_vCollisionVel;
			CObstacle* tempobs = (CObstacle*)pBase;
			tVector2D hisvel = tempobs->GetVel();

			myvel = (hisvel );
			hisvel = (myvel );

			SetVel(myvel);
			tempobs->SetVel(hisvel);

			return true;
			}
		}


		
	}

	return false;
}