
#include "CCar.h"
#include "CGame.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_FModManager.h"
#include "CGamePlayState.h"
#include "CCamera.h"
#include <vector>
#include <math.h>
CCar::CCar(void)
{
	m_fAccelerationRate = 100.0f;
	m_fRotationRate = 3.14f;
	m_fSpeed = 0.0f;
	m_fRotation = 0.0f;

	m_tDirection.fX = 0.0f;
	m_tDirection.fY = -1.0f;

	m_tVelocity.fX = 0.0f;
	m_tVelocity.fY = 0.0f;

	SetPosX(350);
	SetPosY(225);
	m_nCollisionX1 = 350;
	m_nCollisionY1 = 205;
	SetWidth(52);
	SetHeight(70);

	SetVelY(0);
	m_nKillCount = 0;
	SetVelX(0);
	m_nSelectedWeapon = 0;
	std::vector<CCar*>* templist = CGamePlayState::GetInstance()->GetList();
	templist->push_back(this);

	m_nCrashID = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/crash.mp3");
	m_nBulletSound = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/bullet1.mp3");

	CSGD_FModManager::GetInstance()->SetVolume(m_nBulletSound,0.5f);
	m_fFireDelay = 0.2f;
}


void CCar::Update(float fElapsedTime)
{

	tVector2D tempdir = GetDirection();
	tVector2D tempvel = GetVelocity();
	//SetDirection(tempdir);
	//tempdir = tempdir * GetSpeed();


	if(tempvel.fX > 0)
	{
		// adjusting these changes the time it takes for the velocity 
		// to get back to the direction vector
		tempvel.fX -= 0.6f;
	}
	if(tempvel.fY > 0)
	{
		tempvel.fY -= 0.6f;
	}
	if(tempvel.fX < 0)
	{
		tempvel.fX += 0.6f;
	}
	if(tempvel.fY < 0)
	{
		tempvel.fY += 0.6f;
	}
	if(tempvel.fX < 0.6f && tempvel.fX > -0.6f)
	{
		tempvel.fX = 0.0f;
	}
	if(tempvel.fY > 0.6f && tempvel.fY > -0.6f)
	{
		tempvel.fY = 0.0f;
	}
	if(GetSpeed() > 0)
	{
		SetSpeed(GetSpeed() - 0.2f);
	}
	if(GetSpeed() < 0)
	{
		SetSpeed(GetSpeed() + 0.2f);
	}
	if(GetSpeed() < 0.2f && GetSpeed() > -0.2f)
		SetSpeed(0);


	SetVelocity(tempvel);
	SetVelX(m_tVelocity.fX + tempdir.fX);
	SetVelY(m_tVelocity.fY + tempdir.fY);
	SetPosX(GetPosX() + (GetVelX() * fElapsedTime));
	SetPosY(GetPosY() + (GetVelY() * fElapsedTime));
	m_nCollisionX1 = m_nCollisionX1 + (GetVelX() * fElapsedTime);
	m_nCollisionY1 = m_nCollisionY1 + (GetVelY() * fElapsedTime);
	CBase::Update(fElapsedTime);
	InBounds();
}

void CCar::Render(CCamera* camera)
{

	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	
	RECT tempcar;
	tempcar.left = (LONG)(GetPosX()- camera->GetCamX());
	tempcar.top = (LONG)(GetPosY()- camera->GetCamY());
	tempcar.right = (LONG)(tempcar.left + GetWidth());
	tempcar.bottom = (LONG)(tempcar.top + GetHeight());
	pD3D->DrawRect(tempcar,255,0,0);
	pD3D->DrawText("BEEP", (int)(GetPosX()- camera->GetCamX() + 10), (int)(GetPosY()- camera->GetCamY() + 35),255,255,255);
	pD3D->DrawLine((int)(GetPosX()- camera->GetCamX()), (int)(GetPosY()- camera->GetCamY()), (int)(GetPosX()- camera->GetCamX() + GetVelX()), (int)(GetPosY()- camera->GetCamY() + GetVelY()),255,255,255);
	pD3D->DrawLine(400, 600, 400, 550,255,255,255);
	float dir1 = GetDirection().fX;
	float dir2 = GetDirection().fY;
	pD3D->DrawLine((int)(GetPosX()- camera->GetCamX()), (int)(GetPosY()- camera->GetCamY()), (int)(GetPosX()- camera->GetCamX() + (20 * (dir1))), (int)(GetPosY()- camera->GetCamY() + (20 * (dir2))), 0,255,0);
	char buffer[128];
	sprintf_s(buffer,"fX: %f	fY: %f",dir1,dir2);
	pD3D->DrawText(buffer,(int)(GetPosX()- camera->GetCamX()), (int)(GetPosY()- camera->GetCamY()),0,0,255);
	sprintf_s(buffer,"R: %f",GetRotation());
	pD3D->DrawText(buffer,(int)(GetPosX()- camera->GetCamX())+10, (int)(GetPosY()- camera->GetCamY())+20,0,0,255);
	// testing collision rotation
	
	//m_nCollisionX1 = GetPosX() - camera->GetCamX();
	//m_nCollisionY1 = GetPosY() - camera->GetCamY();
	RECT tempcircle1;
	tempcircle1.left = m_nCollisionX1 - camera->GetCamX();
	tempcircle1.top = m_nCollisionY1 - camera->GetCamY();
	tempcircle1.right = tempcircle1.left + 10;
	tempcircle1.bottom = tempcircle1.top + 10;

	pD3D->DrawRect(tempcircle1,255,255,255);



	// end collision testing


}
void CCar::Rotate(float angle)
{
	float newangle = angle / 180 * 3.14159;

	/*float s = sin(angle);
	float c = sin(angle);

	m_nCollisionX1 -= GetPosX();
	m_nCollisionY1 -= GetPosY();

	float newx = m_nCollisionX1 * c - m_nCollisionY1 * s;
	float newy = m_nCollisionX1 * s + m_nCollisionY1 * c;

	m_nCollisionX1 = newx + GetPosX();
	m_nCollisionY1 = newx + GetPosY();*/
	m_nCollisionX1 = GetPosX();
	m_nCollisionY1 = GetPosY() - 20;
	//m_nCollisionX1 = GetPosX() + (cos(newangle)) * (m_nCollisionX1 - GetPosX()) - sin(newangle) * (m_nCollisionY1 - GetPosY());
	//m_nCollisionY1 = GetPosY() + (sin(newangle)) * (m_nCollisionX1 - GetPosX()) + cos(newangle) * (m_nCollisionY1 - GetPosY());
	float tempX1 = m_nCollisionX1;
	float tempY1 = m_nCollisionY1;
	m_nCollisionX1 = GetPosX() + (cos(angle)) * (tempX1 - GetPosX()) - sin(angle) * (tempY1 - GetPosY());
	m_nCollisionY1 = GetPosY() + (sin(angle)) * (tempX1 - GetPosX()) + cos(angle) * (tempY1 - GetPosY());
}
void CCar::PlayCrash(void)
{
	CSGD_FModManager::GetInstance()->SetVolume(m_nCrashID,CGame::GetInstance()->getSoundAVolume());
	CSGD_FModManager::GetInstance()->PlaySound(m_nCrashID);
}
void CCar::PlayBullet(void)
{
	CSGD_FModManager::GetInstance()->SetVolume(m_nBulletSound,CGame::GetInstance()->getSoundAVolume());
	CSGD_FModManager::GetInstance()->PlaySound(m_nBulletSound);
}

bool CCar::CheckCollision(IBaseInterface* pBase)
{

	return false;
}

bool CCar::InBounds(void)
{
	//if(GetPosX() >= CGame::GetInstance()->GetScreenWidth() - this->GetWidth())
	//{
	//	if(GetVelX() >= 0)
	//		SetPosX((float)CGame::GetInstance()->GetScreenWidth() - this->GetWidth());

	//	//m_fSpeed = -1 * m_fSpeed;
	//	//SetVelX(-100);
	//}
	if(GetPosX() <= 0)
	{
		if(GetVelX() <= 0)
			SetPosX(0);

		//m_fSpeed = -1 * m_fSpeed;
		//SetVelX(100);
	}
	//if(GetPosY() >= CGame::GetInstance()->GetScreenHeight() - this->GetHeight())
	//{
	//	//float vel = GetVelY();
	//	if(GetVelY() >= 0)
	//	{
	//		SetPosY((float)CGame::GetInstance()->GetScreenHeight() - this->GetHeight());
	///*		tVector2D tempdir = GetDirection();
	//		tempdir.fX = 0;
	//		tempdir.fY = -1;
	//		float angle = AngleBetweenVectors(GetDirection(),tempdir);
	//		tempdir = Vector2DRotate(tempdir, (-1.0f *angle + 1.57f));
	//		SetDirection(tempdir);
	//		SetVelY(tempdir.fY * GetSpeed());*/
	//	}
	//	/*vel = GetVelY();
	//	vel = GetVelY();*/
	//	//	SetDirection(tempdir);
	//	//SetVelY(GetVelY() * -100);
	//	//m_fSpeed = -1 * m_fSpeed;
	//	//SetVelY(-100);
	//	//SetRotation(GetRotation() + 45);
	//}
	if(GetPosY() <= 0)
	{
		if(GetVelY() <= 0)
			SetPosY(0);
		//m_fSpeed = -1 * m_fSpeed;
		//SetVelY(100);
	}
	return true;
}
