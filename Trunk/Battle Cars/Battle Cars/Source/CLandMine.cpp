#include "CLandMine.h"
#include "CMessageSystem.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CEventSystem.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CCar.h"
#include "CObstacle.h"

CLandMine::CLandMine() : CBullet()
{
	m_fDuration = 20.0f;
	SetBulletType(PROJECTILE_LANDMINE);
	SetLandMineType(LM_LM);
}

CLandMine::~CLandMine()
{
}

void CLandMine::Update(float fElapsedTime)
{
	if(m_fDuration != 20.0f)
		m_fDuration -= fElapsedTime;
	if(m_fDuration <= 0.0f)
		CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
}

void CLandMine::Render(CCamera* camera)
{
	RECT intersect;

	if (!(IntersectRect (&intersect, &GetRect(), &camera->GetRect ())))
	{
		return;
	}

	RECT temppos;
	temppos.left = (LONG)(GetPosX() - camera->GetCamX() + camera->GetRenderPosX());
	temppos.top = (LONG)(GetPosY() - camera->GetCamY() + camera->GetRenderPosY());
	temppos.right = (LONG)(temppos.left + GetWidth());
	temppos.bottom = (LONG)(temppos.top + GetHeight());
	//CSGD_Direct3D::GetInstance()->DrawRect(temppos,255,255,255);
	GetTextureManager()->Draw(GetImageID(),(int)(GetPosX()-(GetWidth() * .5f)) - (int)camera->GetCamX() + (int)camera->GetRenderPosX(),(int)(GetPosY()-(GetHeight()*.5f)) - (int)camera->GetCamY() + (int)camera->GetRenderPosY(),GetScale(),GetScale());
}

bool CLandMine::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(this->GetOwner() == pBase)
		return false;
	if(pBase == this)
		return false;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_PLAYER || pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_BOSS || pBase->GetType() == PROJECTILE_BULLET || pBase->GetType() == OBJECT_BULLET)
		{
			//send damage event
			if(m_nLandMineType == LM_LM)
			{
				CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
				CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage((CBullet*)pBase));
				CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			}
		}
		else if(pBase->GetType() == OBJECT_OBSTACLE)
		{
			CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
			CObstacle* tempobs = (CObstacle*)pBase;
			tVector2D obsvel = tempobs->GetVel();
			
			RECT obsrect = tempobs->GetRect();
			RECT myrect = this->GetRect();
			if(obsrect.left > myrect.right)
			{
				obsvel.fX = (float)(((rand() % 40) + 15) * -1.0f);
			}
			else
			{
				obsvel.fX = (float)(((rand() % 40) + 15));
			}
			if(obsrect.bottom > myrect.top)
			{
				obsvel.fY = (float)(((rand() % 40) + 15) * -1.0f);
			}
			else
			{
				obsvel.fY = (float)(((rand() % 40) + 15));
			}
			tempobs->SetVel(obsvel);
		}
	}
	return false;
}