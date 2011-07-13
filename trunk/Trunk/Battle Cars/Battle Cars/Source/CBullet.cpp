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
CBullet::CBullet(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nType = OBJECT_BULLET;
	m_nBulletType = PROJECTILE_BULLET;
	m_fBlastRadius = 0.0f;
	m_fSlowRate = 0.0f;
}

void CBullet::Update(float fElapsedTime)
{
	m_fCurLife += fElapsedTime;
	SetPosX(GetPosX() + GetVelX() * fElapsedTime);
	SetPosY(GetPosY() + GetVelY() * fElapsedTime);
	if(m_fCurLife >= m_fMaxLife)
	{
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
	CSGD_Direct3D::GetInstance()->DrawRect(temppos,255,255,255);
	m_pTM->Draw(GetImageID(),(int)GetPosX() - camera->GetCamX() + camera->GetRenderPosX(),(int)GetPosY() - camera->GetCamY() + camera->GetRenderPosY(),m_fScale,m_fScale, 
		NULL, GetWidth()/2, GetHeight()/2, GetRotation());
	//m_pTM->Draw(GetImageID(),(int)GetPosX(),(int)GetPosY(),0.5f,0.5f,NULL,GetPosX()-(GetWidth()),GetPosY()-(GetHeight()),m_fRotation);
}

bool CBullet::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(GetOwner() == pBase)
			return false;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		

		if(pBase->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)pBase;
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			// handle what happens to player)
			CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			return true;
		}
		else if(pBase->GetType() == OBJECT_ENEMY)
		{
			CEnemy* tempenemy = (CEnemy*)pBase;
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			// handle what happens to enemy
			CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			return true;
		}
	}
	return false;
}