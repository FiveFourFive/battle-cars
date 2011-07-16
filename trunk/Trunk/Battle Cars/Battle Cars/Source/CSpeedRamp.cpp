#include "CSpeedRamp.h"
#include "CSGD_Direct3D.h"
#include "CCamera.h"
#include "CSGD_TextureManager.h"

CSpeedRamp::CSpeedRamp(void)
{
	m_nType = OBJECT_SPEEDRAMP;
	SetWidth(100);
	SetHeight(150);
	SetPosX(675.0f);
	SetPosY(675.0f);
	m_tVelDir.fX = 0.0f;
	m_tVelDir.fY = -1.0f;
}

void CSpeedRamp::Update(float fElapsedTime)
{

}

void CSpeedRamp::Render(CCamera* camera)
{
	if( (GetPosX()- camera->GetCamX()) < 0 || (GetPosX() - camera->GetCamX()) > camera->GetWidth() || GetPosY() - camera->GetCamY() < 0 ||
		GetPosY() - camera->GetCamY() > camera->GetHeight())
		return;

	//RECT tempdraw;
	//CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	//tempdraw.left = (int)(GetPosX() - camera->GetCamX() + (int)camera->GetRenderPosX());
	//tempdraw.top = (int)(GetPosY() - camera->GetCamY() + (int)camera->GetRenderPosY());
	//tempdraw.right = tempdraw.left + GetWidth();
	//tempdraw.bottom = tempdraw.top + GetHeight();
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(), (int)(GetPosX() - camera->GetCamX() + (int)camera->GetRenderPosX()), (int)(GetPosY() - camera->GetCamY() + (int)camera->GetRenderPosY()));
	//pD3D->DrawRect(tempdraw,255,0,255);
}

bool CSpeedRamp::CheckCollision(IBaseInterface* pBase)
{

	return true;
}
