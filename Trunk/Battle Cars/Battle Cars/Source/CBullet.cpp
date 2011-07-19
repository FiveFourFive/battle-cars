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
CBullet::CBullet(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nType = OBJECT_BULLET;
	m_nBulletType = PROJECTILE_BULLET;
	m_fBlastRadius = 0.0f;
	m_fSlowRate = 0.0f;
	trace_particle = -1;

	//SetOwner(NULL);
}

void CBullet::Update(float fElapsedTime)
{
	m_fCurLife += fElapsedTime;
	SetPosX(GetPosX() + GetVelX() * fElapsedTime);
	SetPosY(GetPosY() + GetVelY() * fElapsedTime);

	ParticleManager* pPM = ParticleManager::GetInstance();

	if( trace_particle > -1 )
		pPM->AttachToBasePosition(this, pPM->GetActiveEmittor(trace_particle), GetWidth()*0.5f, GetHeight()*0.5f);

	if(m_fCurLife >= m_fMaxLife)
	{
		if( trace_particle > -1)
		{
			pPM->GetEmittor(trace_particle)->SetTimeToDie(0.0f);
			pPM->GetEmittor(trace_particle)->SetBase(NULL);
		}
		CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
	}
}

void CBullet::Render(CCamera* camera)
{
	if( (GetPosX() - camera->GetCamX()) < 0 || (GetPosX() - camera->GetCamX()) > camera->GetRenderPosX() + camera->GetWidth() )
		return;

	RECT temppos;
	temppos.left = (LONG)(GetPosX() - camera->GetCamX() + camera->GetRenderPosX());
	temppos.top = (LONG)(GetPosY() - camera->GetCamY() + camera->GetRenderPosY());
	temppos.right = (LONG)(temppos.left + GetWidth());
	temppos.bottom = (LONG)(temppos.top + GetHeight());
	//CSGD_Direct3D::GetInstance()->DrawRect(temppos,255,255,255);
	m_pTM->Draw(GetImageID(),(int)GetPosX() - camera->GetCamX() + camera->GetRenderPosX(),(int)GetPosY() - camera->GetCamY() + camera->GetRenderPosY(),m_fScale,m_fScale, 
		NULL, GetWidth()/2, GetHeight()/2, GetRotation());
	//m_pTM->Draw(GetImageID(),(int)GetPosX(),(int)GetPosY(),0.5f,0.5f,NULL,GetPosX()-(GetWidth()),GetPosY()-(GetHeight()),m_fRotation);
}

bool CBullet::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	ParticleManager* pPM = ParticleManager::GetInstance();
	if(GetOwner() == pBase)
			return false;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		

		if(pBase->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)pBase;
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			if( trace_particle > -1)
			{
				pPM->GetEmittor(trace_particle)->SetTimeToDie(0.0f);
				pPM->GetEmittor(trace_particle)->SetBase(NULL);
			}
			// handle what happens to player)
			CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			return true;
		}
		else if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_BOSS)
		{
			if(GetOwner()->GetType() == OBJECT_PLAYER)
			{
			CEnemy* tempenemy = (CEnemy*)pBase;
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			if( trace_particle > -1)
			{
				pPM->GetEmittor(trace_particle)->SetTimeToDie(0.0f);
				pPM->GetEmittor(trace_particle)->SetBase(NULL);
			}
			// handle what happens to enemy
			CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			return true;
			}
			else
				return false;
		}
		else if(pBase->GetType() == OBJECT_OBSTACLE)
		{
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			CObstacle* tempobs = (CObstacle*)pBase;
			tVector2D obsvel = tempobs->GetVel();
			obsvel.fX = this->GetVelX() * 0.4f;
			obsvel.fY = this->GetVelY() * 0.4f;
			tempobs->SetVel(obsvel);
		}
	}
	return false;
}