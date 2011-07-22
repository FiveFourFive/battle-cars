#include "CObstacle.h"
#include "CCar.h"
#include "CLevel.h"
#include "CSGD_Direct3D.h"
#include "CCamera.h"
#include "SGD_Math.h"

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

	/*m_vCollisionVel.fX = m_vCollisionVel.fX - (m_vCollisionVel.fX * 0.05f);
	m_vCollisionVel.fY = m_vCollisionVel.fY - (m_vCollisionVel.fY * 0.05f);*/
	if(m_vCollisionVel.fX > 0)
	{
		m_vCollisionVel.fX = m_vCollisionVel.fX - 0.05f;//(m_vCollisionVel.fX * 0.05f);
		if(m_vCollisionVel.fX <= 1.0f)
			m_vCollisionVel.fX = 0.0f;
	}
	else if(m_vCollisionVel.fX < 0)
	{
		m_vCollisionVel.fX = m_vCollisionVel.fX + 0.05f;
		if(m_vCollisionVel.fX >= -1.0f)
			m_vCollisionVel.fX = 0.0f;
	}
	if(m_vCollisionVel.fY > 0)
	{
		m_vCollisionVel.fY =m_vCollisionVel.fY - 0.05f;//( m_vCollisionVel.fY * 0.05f);
		if(m_vCollisionVel.fY <= 1.0f)
			m_vCollisionVel.fY = 0.0f;
	}
	else if(m_vCollisionVel.fY < 0)
	{
		m_vCollisionVel.fY = m_vCollisionVel.fY + 0.05f;
		if(m_vCollisionVel.fY >= -1.0f)
			m_vCollisionVel.fY = 0.0f;

	}
	CLevel::GetInstance ()->CheckObstacleCollision (this);

	InBounds();
}

void CObstacle::Render(CCamera* camera)
{
	CBase::Render(camera);
	RECT obs;
	// = GetRect();
	obs.left = GetPosX() - camera->GetCamX() + camera->GetRenderPosX();
	obs.top = GetPosY() - camera->GetCamY() + camera->GetRenderPosY();
	obs.right = obs.left + GetWidth();
	obs.bottom = obs.top + GetWidth();
	CSGD_Direct3D::GetInstance()->DrawRect(obs,0,0,0);

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

			//tVector2D bounce = hisvel;
			//
			//SetPosX(GetPosX() + (bounce.fX * 0.01f * -1.0f));
			//SetPosY(GetPosY() + (bounce.fY * 0.01f * -1.0f));

			//bounce = tempobs->GetVel();

			//tempobs->SetPosX(tempobs->GetPosX() + (bounce.fX * 0.01f * -1.0f));
			//tempobs->SetPosY(tempobs->GetPosY() + (bounce.fY * 0.01f * -1.0f));
			/*if(hisvel.fX > myvel.fX && hisvel.fY > myvel.fY)
			{
					myvel = (hisvel );
					hisvel = (hisvel * -1.0f * 0.5f );
			}
			else
			{
					hisvel = (myvel );
					myvel = (myvel * -1.0f * 0.5f );
			}*/

		

			SetVel(hisvel);
			tempobs->SetVel(myvel);

			return true;
			}
		}


		
	}

	return false;
}