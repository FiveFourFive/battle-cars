/////////////////////////////////////////////////////////
//	File Name	: "CBase.cpp"
//	Author Name : Wade Shwallon
//	Purpose		: Contains base functionality for game objects
/////////////////////////////////////////////////////////

#include "CBase.h"
#include "CSGD_TextureManager.h"
//#include "SGD Wrappers\CSGD_FModManager.h"
#include "CGame.h"
#include "CCamera.h"
#include "CSGD_Direct3D.h"
#include "CMap.h"
#include "CLevel.h"
#include "CGamePlayState.h"

CBase::CBase()
{
	m_uiRefCount = 1; // start with a ref to yourself!!
	SetPosX(0);
	SetPosY(0);
	SetImageID(-1);
	SetWidth(100);
	SetHeight(150);
	//m_nExplosionID = CSGD_FModManager::GetInstance()->LoadSound("resource/sound/Explosion.mp3");
	//CSGD_FModManager::GetInstance()->SetVolume(m_nExplosionID,CGame::getInstance()->getSoundAVolume());
	m_nName[32];
	m_nType = OBJECT_BASE;
}

CBase::~CBase()
{

}

void CBase::Update(float fElapsedTime)
{
}


void CBase::Render(CCamera* camera)
{
	RECT intersect;

	if (!(IntersectRect (&intersect, &GetRect(), &camera->GetRect ())))
	{
		return;
	}

	CSGD_TextureManager::GetInstance()->Draw(GetImageID(),int(GetPosX()-camera->GetCamX() + camera->GetRenderPosX() - (GetWidth()/2)), int(GetPosY()-camera->GetCamY() + camera->GetRenderPosY() - (GetHeight()/2)),1.0f,1.0f);
}

RECT CBase::GetRect()
{
	RECT tempRect = { (LONG)GetPosX() - (GetWidth()/2), (LONG)GetPosY() - (GetHeight()/2), (LONG)(GetPosX() + GetWidth()/2), (LONG)(GetPosY() + GetHeight()/2) };
	return tempRect;
}

bool CBase::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;

	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
	{
		
		if(pBase->GetType() == OBJECT_PLAYER || pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_BOSS)
		{

		}
		
		return true;
	}
	return false;
}

void CBase::SetName(char* name)
{
	//strcpy_s(m_nName,32,name);
	m_nName = name;
}
char* CBase::GetName()
{
	return m_nName;
}

void CBase::InBounds(void)
{
	
	if(GetPosX() - (int)(GetWidth()*0.5f) <= 0)
	{
		SetPosX(GetWidth()*0.5f);
	}
	if(GetPosY() - (int)(GetHeight() * 0.5f) <= 0)
	{
		SetPosY(GetHeight() * 0.5f);
	}
	CLevel* level = CGamePlayState::GetInstance()->GetLevel();
	CMap* map = level->GetMap();
	int mapheight = map->GetPixelHeight() * map->GetMapHeight();
	int mapwidth = map->GetPixelWidth() * map->GetMapWidth();
	if(GetPosX() >= mapwidth)
	{
		SetPosX((float)mapwidth);

	}
	if(GetPosY() >= mapheight)
	{
		SetPosY((float)mapheight);
	}
}