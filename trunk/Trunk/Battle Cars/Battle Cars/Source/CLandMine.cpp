#include "CLandMine.h"
#include "CMessageSystem.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CEventSystem.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CCar.h"

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
	if((GetPosX() - camera->GetCamX()) < 0 || (GetPosX() - camera->GetCamX()) > camera->GetRenderPosX() + camera->GetWidth())
		return;
	RECT temppos;
	temppos.left = (LONG)(GetPosX() - camera->GetCamX() + camera->GetRenderPosX());
	temppos.top = (LONG)(GetPosY() - camera->GetCamY() + camera->GetRenderPosY());
	temppos.right = (LONG)(temppos.left + GetWidth());
	temppos.bottom = (LONG)(temppos.top + GetHeight());
	CSGD_Direct3D::GetInstance()->DrawRect(temppos,255,255,255);
	GetTextureManager()->Draw(GetImageID(),(int)(GetPosX()-(GetWidth() * .5f)) - (int)camera->GetCamX() + (int)camera->GetRenderPosX(),(int)(GetPosY()-(GetHeight()*.5f)) - (int)camera->GetCamY() + (int)camera->GetRenderPosY(),GetScale(),GetScale());
}

bool CLandMine::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;
	if(this->GetOwner() == pBase)
		return false;
	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		if(pBase->GetType() == OBJECT_PLAYER || pBase->GetType() == OBJECT_ENEMY)
		{
			//send damage event
			if(m_nLandMineType == LM_LM)
			{
				CMessageSystem::GetInstance()->SendMsg(new CDestroyBulletMessage(this));
				CEventSystem::GetInstance()->SendEvent("damage",pBase,this);
			}
		}
	}
	return false;
}