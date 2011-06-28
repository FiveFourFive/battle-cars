

#include "CCamera.h"
#include "CBase.h"
#include "CGame.h"
#include "CMainMenuState.h"
#include "COptionState.h"

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

	if( CMainMenuState::GetInstance()->GetNumberOfPlayers() == 2)
	{
		if( COptionState::GetInstance()->IsVertical() )
		{
			m_nWidth = CGame::GetInstance()->GetScreenWidth() / 2;
			m_nHeight = CGame::GetInstance()->GetScreenHeight();
		}
		else
		{
			m_nWidth = CGame::GetInstance()->GetScreenWidth();
			m_nHeight = CGame::GetInstance()->GetScreenHeight()/2;
		}

	}
	else
	{
		m_nWidth = CGame::GetInstance()->GetScreenWidth();
		m_nHeight = CGame::GetInstance()->GetScreenHeight();
	}

	temp.left = (int)GetCamX();
	temp.top = (int)GetCamY();
	temp.right = temp.left + m_nWidth;
	temp.bottom = temp.top + m_nHeight;

	return temp;
}

void CCamera::AttachTo( CBase* camera_owner, float offsetX, float offsetY)
{
	owner = camera_owner;

	SetCamX( (owner->GetPosX() - offsetX));
	SetCamY( (owner->GetPosY() - offsetY));
	offX = offsetX;
	offY = offsetY;
}

void CCamera::Update(void)
{
	if( this->camPosX < 0 )
		this->camPosX = 0;
	if( this->camPosY < 0 )
		this->camPosY = 0;
}
