

#include "CCamera.h"
#include "CBase.h"
#include "CGame.h"
CCamera::CCamera()
{
	SetCamX(0);
	SetCamY(0);
}

CCamera::~CCamera()
{

}

RECT CCamera::GetRect()
{
	RECT temp;

	temp.left = (int)GetCamX();
	temp.top = (int)GetCamY();
	temp.right = temp.left + 800;
	temp.bottom = temp.top + 600;

	return temp;
}

void CCamera::AttachTo( CBase* camera_owner, float offsetX, float offsetY)
{
	owner = camera_owner;

	SetCamX( owner->GetPosX() - offsetX);
	SetCamY( owner->GetPosY() - offsetY);
	offX = offsetX;
	offY = offsetY;
}

void CCamera::Update(void)
{
	camPosX = owner->GetPosX() - offX;
	camPosY = owner->GetPosY() - offY;
}
