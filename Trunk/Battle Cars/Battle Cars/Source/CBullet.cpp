#include "CBullet.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CMessage.h"
#include "CMessageSystem.h"
CBullet::CBullet(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nType = OBJECT_BULLET;
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

void CBullet::Render(void)
{
	RECT temppos;
	temppos.left = (LONG)GetPosX();
	temppos.top = (LONG)GetPosY();
	temppos.right = (LONG)(temppos.left + GetWidth());
	temppos.bottom = (LONG)(temppos.top + GetHeight());
	CSGD_Direct3D::GetInstance()->DrawRect(temppos,255,255,255);
	m_pTM->Draw(GetImageID(),GetPosX(),GetPosY(),m_fScale,m_fScale);
	//m_pTM->Draw(GetImageID(),(int)GetPosX(),(int)GetPosY(),0.5f,0.5f,NULL,GetPosX()-(GetWidth()),GetPosY()-(GetHeight()),m_fRotation);
}

bool CBullet::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(GetOwner() == pBase)
			return false;

		if(pBase->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)pBase;
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			// handle what happens to player

			return true;
		}
		else if(pBase->GetType() == OBJECT_ENEMY)
		{
			CEnemy* tempenemy = (CEnemy*)pBase;
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			// handle what happens to enemy

			return true;
		}
	}
	return false;
}