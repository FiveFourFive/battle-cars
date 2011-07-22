#include "CObstacle.h"
#include "CCar.h"
#include "CLevel.h"
#include "CSGD_Direct3D.h"
#include "CCamera.h"
#include "SGD_Math.h"
#include "ParticleManager.h"
#include "Emittor.h"

CObstacle::CObstacle(void)
{
	m_vCollisionVel.fX = 0;
	m_vCollisionVel.fY = 0;

	m_bHit = false;
	m_fRespawnTimer = 25.0f;
	m_fBlowUpTimer = 5.0f;
	ParticleCreated = false;
	m_bActive = true;
}

CObstacle::~CObstacle(void)
{

}

void CObstacle::Update(float fElapsedTime)
{
	CLevel::GetInstance ()->CheckObstacleCollision (this);

	SetPosX(GetPosX() + (m_vCollisionVel.fX * fElapsedTime));
	SetPosY(GetPosY() + (m_vCollisionVel.fY * fElapsedTime));

	/*m_vCollisionVel.fX = m_vCollisionVel.fX - (m_vCollisionVel.fX * 0.05f);
	m_vCollisionVel.fY = m_vCollisionVel.fY - (m_vCollisionVel.fY * 0.05f);*/
	if(m_vCollisionVel.fX > 0)
	{
		m_vCollisionVel.fX = m_vCollisionVel.fX - 0.8f;//(m_vCollisionVel.fX * 0.05f);
		
	}
	else if(m_vCollisionVel.fX < 0)
	{
		m_vCollisionVel.fX = m_vCollisionVel.fX + 0.8f;
		
	}
	if(m_vCollisionVel.fY > 0)
	{
		m_vCollisionVel.fY =m_vCollisionVel.fY - 0.8f;//( m_vCollisionVel.fY * 0.05f);
		
	}
	else if(m_vCollisionVel.fY < 0)
	{
		m_vCollisionVel.fY = m_vCollisionVel.fY + 0.8f;

	}
	if(m_vCollisionVel.fX >= -1.0f && m_vCollisionVel.fX <= 1.0f)
		m_vCollisionVel.fX = 0.0f;
	if(m_vCollisionVel.fY >= -1.0f && m_vCollisionVel.fY <= 1.0f)
		m_vCollisionVel.fY = 0.0f;

	CLevel::GetInstance ()->CheckObstacleCollision (this);

	InBounds();

	if( GetObstacleType() == BARREL_OBSTACLES)
	{
		if( m_bHit )
		{
			if( !ParticleCreated )
			{
				ParticleManager* PM = ParticleManager::GetInstance();
				Emittor* blow_emittor = PM->CreateEffect(PM->GetEmittor(BARREL_EMITTOR), this->GetPosX(), this->GetPosY());

				if( blow_emittor )
				{
					m_nBarrelBurstingID = blow_emittor->GetID();
					blow_emittor->SetTimeToDie(5.0f);
					PM->AttachToBasePosition(this, blow_emittor);
				}
				ParticleCreated = true;
			}

			if( m_fBlowUpTimer > 0.0f)
			{
				ParticleManager* PM = ParticleManager::GetInstance();

				m_fBlowUpTimer -= fElapsedTime;
				PM->AttachToBasePosition(this, PM->GetActiveEmittor(m_nBarrelBurstingID));
			}

			if( m_fBlowUpTimer <= 0.0f)
			{
				m_fRespawnTimer-= fElapsedTime;

				if( m_bActive )
				{
					ParticleManager* PM = ParticleManager::GetInstance();
#pragma region EXPLOSION_EFFECT
					Emittor* smoke_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_SMOKE_EMITTOR), this->GetPosX(), this->GetPosY());
					if( smoke_emittor )
					{
 						smoke_emittor->SetTimeToDie(1.5f);
					}

					Emittor* fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST1_EMITTOR), this->GetPosX() , this->GetPosY());
					if( fireburst_emittor )
					{
						fireburst_emittor->SetTimeToDie(1.5f);
					}

					fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST2_EMITTOR), this->GetPosX() , this->GetPosY());
					if( fireburst_emittor )
					{
						fireburst_emittor->SetTimeToDie(1.5f);				
					}
					fireburst_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FIREBURST3_EMITTOR), this->GetPosX(), this->GetPosY());
					if( fireburst_emittor )
					{
						fireburst_emittor->SetTimeToDie(1.5f);
					}

					Emittor* fire_emittor = PM->CreateEffect(PM->GetEmittor(EXPLOSION_FLAME_EMITTOR), this->GetPosX(), this->GetPosY());
					if( fire_emittor )
					{
						fire_emittor->SetTimeToDie(1.5f);
					}
#pragma endregion

				}

				m_bActive = false;

				if( m_fRespawnTimer <= 0.0f )
				{
					m_bHit = false;
					m_fBlowUpTimer = 5.0f;
					m_fRespawnTimer = 25.0f;
					SetPosX(GetSpawnPosition().fX);
					SetPosY(GetSpawnPosition().fY);

					ParticleCreated = false;
					m_bActive = true;
				}
			}
		}
	}
}

void CObstacle::Render(CCamera* camera)
{
	if( m_bActive )
		CBase::Render(camera);

}

bool CObstacle::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;

	if( m_bActive )
	{
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
				m_bHit = true;
				return true;
			}
			else if(pBase->GetType() == OBJECT_OBSTACLE)
			{
				if(pBase != this)
				{
				tVector2D myvel = m_vCollisionVel;
				CObstacle* tempobs = (CObstacle*)pBase;
				tVector2D hisvel = tempobs->GetVel();

			float myfx = abs(myvel.fX);
			float myfy = abs(myvel.fY);
			float hisfx = abs(hisvel.fX);
			float hisfy = abs(hisvel.fY);
			if((myfx + myfy) >= (hisfx + hisfy))
			{
			SetPosX(GetPosX() + (hisvel.fX * 0.05f));
			SetPosY(GetPosY() + (hisvel.fY * 0.05f));

			tempobs->SetPosX(tempobs->GetPosX() + (m_vCollisionVel.fX * 0.05f));
			tempobs->SetPosY(tempobs->GetPosY() + (m_vCollisionVel.fY * 0.05f));

			SetVel(hisvel);
			tempobs->SetVel(myvel);
			
				m_bHit = true;

				return true;
				}
	
			}
		}


		
		}

	}
	return false;
}