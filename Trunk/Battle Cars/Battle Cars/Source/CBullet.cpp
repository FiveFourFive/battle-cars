#include "CBullet.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CMessage.h"
#include "CMessageSystem.h"
#include "CCamera.h"
#include "CEvent.h"
#include "CEventSystem.h"
#include "Emittor.h"
#include "ParticleManager.h"
#include "CObstacle.h"
#include "CLevel.h"

CBullet::CBullet(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nType = OBJECT_BULLET;
	m_nBulletType = PROJECTILE_BULLET;
	m_fBlastRadius = 0.0f;
	m_fSlowRate = 0.0f;
	trace_particle = -1;

	m_bToRender = true;

	//SetOwner(NULL);
}

void CBullet::Update(float fElapsedTime)
{
	if (!CLevel::GetInstance ()->CheckBulletCollision (this))
	{
		m_fCurLife += fElapsedTime;
		SetPosX(GetPosX() + GetVelX() * fElapsedTime);
		SetPosY(GetPosY() + GetVelY() * fElapsedTime);

		ParticleManager* pPM = ParticleManager::GetInstance();

		if( trace_particle > -1 )
		{
			Emittor* tracer_effect = pPM->GetActiveEmittor(trace_particle);

			float tempX1 = GetPosX();
			float tempY1 = GetPosY() + ((GetHeight()/2));
			float cx = GetPosX();
			float posx = cx + (cos(GetRotation())) * (tempX1 - cx) - sin(GetRotation()) * (tempY1 - GetPosY());
			float posy = GetPosY() + (sin(GetRotation())) * (tempX1 - cx) + cos(GetRotation()) * (tempY1 - GetPosY());

			if( tracer_effect)
				pPM->AttachToBasePosition(NULL, tracer_effect, posx, posy);
		}

		if(m_fCurLife >= m_fMaxLife)
		{
			if( trace_particle > -1)
			{
				Emittor* tracer_effect = pPM->GetActiveEmittor(trace_particle);
				if( tracer_effect)
				{
					tracer_effect->SetTimeToDie(0.0f);
					tracer_effect->SetBase(NULL);
				}
				trace_particle = -1;
				
			}
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
		}
	}
}

void CBullet::Render(CCamera* camera)
{
	if( (GetPosX() - camera->GetCamX()) < 0 || (GetPosX() - camera->GetCamX()) > camera->GetRenderPosX() + camera->GetWidth() || (GetRect().top - camera->GetCamY() + 2 < 0) 
		|| (GetRect().top - camera->GetCamY()) > camera->GetRenderPosY() + camera->GetHeight() )
		return;

	if( m_bToRender )
		m_pTM->Draw(GetImageID(),int(GetPosX() - camera->GetCamX() + camera->GetRenderPosX()),int(GetPosY() - camera->GetCamY() + camera->GetRenderPosY()),m_fScale,m_fScale, 
			NULL,m_pTM->GetTextureWidth(GetImageID()) *0.5f,m_pTM->GetTextureHeight(GetImageID())*0.5f, GetRotation());
	//m_pTM->Draw(GetImageID(),(int)GetPosX(),(int)GetPosY(),0.5f,0.5f,NULL,GetPosX()-(GetWidth()),GetPosY()-(GetHeight()),m_fRotation);

}

bool CBullet::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	ParticleManager* pPM = ParticleManager::GetInstance();
	if(GetOwner() == pBase)
			return false;
	
	if( GetType() == PROJECTILE_BULLET )
		return false;

	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)pBase;
			if( trace_particle > -1)
			{
				Emittor* tracer_effect = pPM->GetActiveEmittor(trace_particle);
				if( tracer_effect)
				{
					tracer_effect->SetTimeToDie(0.0f);
					tracer_effect->SetBase(NULL);
				}
				trace_particle = -1;
			}
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			// handle what happens to player)
			CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			return true;
		}
		else if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_BOSS)
		{
			if(GetOwner()->GetType() == OBJECT_PLAYER || GetOwner()->GetType() == OBJECT_ENEMY)
			{
			CEnemy* tempenemy = (CEnemy*)pBase;
			if( trace_particle > -1)
			{
				Emittor* tracer_effect = pPM->GetActiveEmittor(trace_particle);
				if( tracer_effect)
				{
					tracer_effect->SetTimeToDie(0.0f);
					tracer_effect->SetBase(NULL);
				}
				trace_particle = -1;
			}
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			// handle what happens to enemy
			CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			return true;
			}
			else
				return false;
		}
		else if(pBase->GetType() == OBJECT_OBSTACLE)
		{
			if( trace_particle > -1)
			{
				Emittor* tracer_effect = pPM->GetActiveEmittor(trace_particle);
				if( tracer_effect)
				{
					tracer_effect->SetTimeToDie(0.0f);
					tracer_effect->SetBase(NULL);
				}
				trace_particle = -1;
			}
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			CObstacle* tempobs = (CObstacle*)pBase;
			tVector2D obsvel = tempobs->GetVel();
			obsvel.fX = this->GetVelX() * 0.4f;
			obsvel.fY = this->GetVelY() * 0.4f;
			tempobs->SetVel(obsvel);

			tempobs->SetHit(true);
		}
	}
	return false;
}