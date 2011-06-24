/////////////////////////////////////////////////////////
//	File Name	: "CBase.cpp"
//	Author Name : Wade Shwallon
//	Purpose		: Contains base functionality for game objects
/////////////////////////////////////////////////////////

#include "CBase.h"
//#include "SGD Wrappers\CSGD_TextureManager.h"
//#include "SGD Wrappers\CSGD_FModManager.h"
#include "CGame.h"
#include "CSGD_Direct3D.h"

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
	//	CSGD_TextureManager::GetInstance()->Draw(GetImageID(),
	//												(int)GetPosX(), (int)GetPosY());
}

RECT CBase::GetRect()
{
	RECT tempRect = { (LONG)GetPosX(), (LONG)GetPosY(), (LONG)(GetPosX() + GetWidth()), (LONG)(GetPosY() + GetHeight()) };
	return tempRect;
}

bool CBase::CheckCollision(IBaseInterface* pBase)
{
	RECT intersection;

	if(IntersectRect(&intersection, &GetRect(), &pBase->GetRect()))
		return true;
	return false;
}

void CBase::SetName(char* name)
{
	strcpy_s(m_nName,32,name);
}
char* CBase::GetName()
{
	return m_nName;
}