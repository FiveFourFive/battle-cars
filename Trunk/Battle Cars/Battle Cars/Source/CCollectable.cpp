#include "CCollectable.h"
#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "CCamera.h"
#include "CGamePlayState.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CMap.h"

CCollectable::CCollectable()
{
	m_nType = OBJECT_COLLECTABLE;
	int minX = 5;
	int minY = 5;
	int maxX = ((CLevel::GetInstance ()->GetMap()->GetMapWidth ()) - 5);
	int maxY = ((CLevel::GetInstance ()->GetMap()->GetMapHeight ()) - 5);
	SetPosX((float)(rand()% (maxX - minX + 1) + minX));
	SetPosY((float)(rand()% (maxY - minY + 1) + minY));

	SetPosX (GetPosX () * CLevel::GetInstance ()->GetMap ()->GetPixelWidth ());
	SetPosY (GetPosY () * CLevel::GetInstance ()->GetMap ()->GetPixelHeight ());

	if (this->GetPosX () < 160 || this->GetPosY () < 160)
	{
		int temp = 0;
		temp = 128;
	}
	SetWidth(128);
	SetHeight(128);
	
	m_bActive = true;
	SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BattleCars_CollectablePlaceHolder.png"));
}

CCollectable::~CCollectable()
{
}

void CCollectable::Update(float fElapsedTime)
{
}

void CCollectable::Render(CCamera* camera)
{
	if(m_bActive)
	{
		CSGD_TextureManager* m_pTM = CSGD_TextureManager::GetInstance();
		m_pTM->Draw(GetImageID(), (int)(GetPosX() - camera->GetCamX() + (int)(camera->GetRenderPosX())), (int)(GetPosY() - camera->GetCamY() + (int)(camera->GetRenderPosY()))); 
	}
}

bool CCollectable::CheckCollision(IBaseInterface* pBase)
{
	if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_PLAYER)
	{
		RECT collision;
		if(IntersectRect(&collision, &GetRect(), &pBase->GetRect()))
		{
			if (m_bActive)
			{
				m_bActive = false;
				((CCar*)pBase)->SetCollected (((CCar*)pBase)->GetCollected () + 1);
			}
			//if(pBase->GetType() == OBJECT_PLAYER && m_bActive)
			//{
			//	//Play a sound here as well
			//	
			//	CGamePlayState::GetInstance()->GetPlayer1 ()->SetCollected (CGamePlayState::GetInstance()->GetPlayer1 ()->GetCollected () + 1);
			//}
			//else if(pBase->GetType() == OBJECT_ENEMY && m_bActive)
			//{
			//	//play a sound here as well
			//	m_bActive = false;
			//	CGamePlayState::GetInstance()->GetCollectionBoss()->SetCollected (CGamePlayState::GetInstance()->GetCollectionBoss ()->GetCollected () + 1);
			//}
		}
	}
	return false;
}

RECT CCollectable::GetRect()
{
	RECT temprect;
	temprect.left = (int)GetPosX();
	temprect.top = (int)GetPosY();
	temprect.right = temprect.left + GetWidth();
	temprect.bottom = temprect.top + GetHeight();
	return temprect;
}

