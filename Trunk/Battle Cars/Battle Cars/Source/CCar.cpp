
#include "CCar.h"
#include "CGame.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "CSGD_FModManager.h"
#include "CGamePlayState.h"
#include "CCamera.h"
#include <vector>
#include "CSGD_TextureManager.h"
#include <math.h>
#include "CEventSystem.h"
#include "CEvent.h"
#include "CBullet.h"

CCar::CCar(void)
{
	m_fAccelerationRate = 200.0f;
	m_fRotationRate = 3.14f;
	m_fSpeed = 0.0f;
	m_fRotation = 0.0f;
	m_bAccelerating = false;
	m_tDirection.fX = 0.0f;
	m_tDirection.fY = -1.0f;

	m_tVelocity.fX = 0.0f;
	m_tVelocity.fY = 0.0f;

	SetPosX(350);
	SetPosY(225);

	SetWidth(52);
	SetHeight(70);
	m_nCollisionRadius = GetWidth()/2;
	m_nCollisionX1 = GetPosX();
	m_nCollisionY1 = GetPosY() - (GetHeight()*0.5f) + (GetWidth()*0.5f);;
	SetVelY(0);
	m_nKillCount = 0;
	SetVelX(0);
	m_nSelectedWeapon = 0;
	std::vector<CCar*>* templist = CGamePlayState::GetInstance()->GetList();
	templist->push_back(this);

	m_nCrashID = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/crash.mp3");
	m_nBulletSound = CSGD_FModManager::GetInstance()->LoadSound("resource/sounds/bullet1.mp3");

	CSGD_FModManager::GetInstance()->SetVolume(m_nBulletSound,0.5f);
	m_fFireDelay = .50f;
	m_fFireDelayMissiles = 1.0f;
	SetPowerUpBar(100.0f);
	m_nCarID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/cartexture.bmp",D3DCOLOR_XRGB(255, 255, 255));

	CEventSystem::GetInstance()->RegisterClient("damage",this);
}


void CCar::Update(float fElapsedTime)
{

	tVector2D tempdir = GetDirection();
	tVector2D tempvel = GetVelocity();
	//SetDirection(tempdir);
	tempdir = Vector2DNormalize(tempdir);
	tempdir = tempdir * GetSpeed();


	if(tempvel.fX > 0)
	{
		// adjusting these changes the time it takes for the velocity 
		// to get back to the direction vector
		tempvel.fX -= tempvel.fX * 0.05;
	}
	if(tempvel.fY > 0)
	{
		tempvel.fY -= tempvel.fY * 0.05;
	}
	if(tempvel.fX < 0)
	{
		tempvel.fX -= tempvel.fX * 0.05;
	}
	if(tempvel.fY < 0)
	{
		tempvel.fY -= tempvel.fY * 0.05;
	}


	//if(tempvel.fX > 0)
	//{
	//	// adjusting these changes the time it takes for the velocity 
	//	// to get back to the direction vector
	//	tempvel.fX -= 3.0f;
	//}
	//if(tempvel.fY > 0)
	//{
	//	tempvel.fY -= 3.0f;
	//}
	//if(tempvel.fX < 0)
	//{
	//	tempvel.fX += 3.0f;
	//}
	//if(tempvel.fY < 0)
	//{
	//	tempvel.fY += 3.0f;
	//}
	if(tempvel.fX < 3.0f && tempvel.fX > -3.0f)
	{
		tempvel.fX = 0.0f;
	}
	if(tempvel.fY < 3.0f && tempvel.fY > -3.0f)
	{
		tempvel.fY = 0.0f;
	}
	if(GetSpeed() > 0 && !m_bAccelerating)
	{
		SetSpeed(GetSpeed() - 1.0f);
	}
	if(GetSpeed() < 0)
	{
		SetSpeed(GetSpeed() + 1.0f);
	}
	if(GetSpeed() < 1.0f && GetSpeed() > -1.0f)
		SetSpeed(0);


	SetVelocity(tempvel);
	//SetDirection(tempdir);
	SetVelX(m_tVelocity.fX + tempdir.fX);
	SetVelY(m_tVelocity.fY + tempdir.fY);
	m_tOverallVelocity = tempvel + tempdir;
	SetPosX(GetPosX() + (GetVelX() * fElapsedTime));
	SetPosY(GetPosY() + (GetVelY() * fElapsedTime));
	m_nCollisionX1 = m_nCollisionX1 + (GetVelX() * fElapsedTime);
	m_nCollisionY1 = m_nCollisionY1 + (GetVelY() * fElapsedTime);
	CBase::Update(fElapsedTime);
	InBounds();
}

void CCar::Render(CCamera* camera)
{
	if( (GetRect().left - camera->GetCamX()) + 2 < 0 || (GetRect().left - camera->GetCamX()) > camera->GetRenderPosX() + camera->GetWidth() 
		|| (GetRect().top - camera->GetCamY() + 2 < 0) || (GetRect().top - camera->GetCamY()) > camera->GetRenderPosY() + camera->GetHeight() )
		return;

	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	CSGD_TextureManager* m_pTM = CSGD_TextureManager::GetInstance();


	RECT tempcar;
	tempcar.left = (LONG)(GetPosX()- camera->GetCamX());
	tempcar.top = (LONG)(GetPosY()- camera->GetCamY());
	tempcar.right = (LONG)(tempcar.left + GetWidth());
	tempcar.bottom = (LONG)(tempcar.top + GetHeight());
	
	RECT car;

	car.left = 0;
	car.top = 0;
	car.bottom = car.top + GetHeight();
	car.right = car.left + GetWidth();

	m_pTM->Draw(m_nCarID,GetPosX()-(GetWidth()/2)- camera->GetCamX() + camera->GetRenderPosX(),GetPosY()-(GetHeight()/2)- camera->GetCamY() + camera->GetRenderPosY(),1.0f,1.0f,&car,GetWidth()/2,GetHeight()/2,GetRotation());
	//pD3D->DrawRect(tempcar,255,0,0);
	//pD3D->DrawText("BEEP", (int)(GetPosX()- camera->GetCamX() + 10), (int)(GetPosY()- camera->GetCamY() + 35),255,255,255);
	pD3D->DrawLine((int)(GetPosX()- camera->GetCamX() + camera->GetRenderPosX()), (int)(GetPosY()- camera->GetCamY()+ camera->GetRenderPosY()), (int)(GetPosX()- camera->GetCamX() + camera->GetRenderPosX() + GetVelX()), (int)(GetPosY()- camera->GetCamY() + camera->GetRenderPosY() + GetVelY()),255,255,255);
	pD3D->DrawLine(400, 600, 400, 550,255,255,255);
	float dir1 = GetDirection().fX;
	float dir2 = GetDirection().fY;
	pD3D->DrawLine((int)(GetPosX()- camera->GetCamX()+ camera->GetRenderPosX()), (int)(GetPosY()- camera->GetCamY() + camera->GetRenderPosY()), (int)(GetPosX() + (20 * dir1)- camera->GetCamX() + camera->GetRenderPosX()), (int)(GetPosY() + (20 * dir2)- camera->GetCamY() + camera->GetRenderPosY()), 0,255,0);
	char buffer[128];
	sprintf_s(buffer,"fX: %f	fY: %f",dir1,dir2);
	pD3D->DrawText(buffer,(int)(GetPosX()- camera->GetCamX() + camera->GetRenderPosX()), (int)(GetPosY()- camera->GetCamY()+ camera->GetRenderPosY()),0,0,255);
	sprintf_s(buffer,"R: %f",GetRotation());
	pD3D->DrawText(buffer,(int)(GetPosX()- camera->GetCamX()+ camera->GetRenderPosX())+10, (int)(GetPosY()- camera->GetCamY()+ camera->GetRenderPosY())+20,0,0,255);


	// testing collision rotation
	
	//m_nCollisionX1 = GetPosX() - camera->GetCamX();
	//m_nCollisionY1 = GetPosY() - camera->GetCamY();
	pD3D->GetSprite()->Flush();
	RECT tempcircle1;
	tempcircle1.left = m_nCollisionX1 - camera->GetCamX() + camera->GetRenderPosX();
	tempcircle1.top = m_nCollisionY1 - camera->GetCamY() + camera->GetRenderPosY();
	tempcircle1.right = tempcircle1.left + m_nCollisionRadius;
	tempcircle1.bottom = tempcircle1.top + m_nCollisionRadius;

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
	m_nCollisionY1 = GetPosY() - (GetHeight()*0.5f) + (GetWidth()*0.5f);
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
	if(pBase == this)
		return false;

	//if(pBase->GetType() == OBJECT_PLAYER)
	//{
	//	CCar* tempcar = (CCar*)pBase;
	//	float centerx = tempcar->GetCX1();
	//	float centery = tempcar->GetCY1();
	//	float myx = GetCX1();
	//	float myy = GetCY1();
	//	
	//	float distance = sqrt(((centerx - myx)*(centerx - myx)) + ((centery - myy)*(centery - myy)));

	//	if(distance <= (GetRadius() + tempcar->GetRadius()))
	//	{
	//		float speed = GetSpeed();
	//	//	SetPosX(GetPosX() - GetVelX() * 0.001f);
	//	//	SetPosX(GetPosY() - GetVelY() * 0.001f);
	//		//SetVelX(0.0f);
	//		//SetVelY(0.0f);
	//		//if(speed >= -10 && speed < 10)
	//		//{
	//		//	SetSpeed(-10);
	//		//	//m_pController1->Vibrate(10000,10000);
	//		//}
	//		//else
	//		{
	//			PlayCrash();
	//			//m_pController1->Vibrate(40000,40000);
	//		//	m_fCollisionDelay = 0.0f;
	//			//CCar* tempcar = (CCar*)pBase;
	//			//tempcar->SetDirection(GetDirection());
	//			//tempcar->SetSpeed(GetSpeed() * 0.2f);
	//			tVector2D tempvel = GetDirection();
	//			//tempvel = Vector2DNormalize(tempvel);
	//			if(GetSpeed() > 0)
	//				tempvel = tempvel * GetSpeed() * 0.5f;
	//			tVector2D currentvel = tempcar->GetVelocity();
	//			tempvel = tempvel + currentvel;
	//			tempcar->SetVelocity(tempvel);
	//			tempvel.fX *= -1;
	//			tempvel.fY *= -1;
	//			SetVelocity(tempvel);
	//			SetSpeed(0);
	//			//SetSpeed((GetSpeed() * -1) + (GetSpeed() * 0.2f));
	//		}
	//		speed = GetSpeed();
	//		//return true;
	//	}
	//}
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
	if(GetPosX() - (int)(GetWidth()*0.5f) <= 0)
	{
		if(GetVelX() <= 0)
			SetPosX(GetWidth()*0.5f);

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
	if(GetPosY() - (int)(GetHeight() * 0.5f) <= 0)
	{
		if(GetVelY() <= 0)
			SetPosY(GetHeight() * 0.5f);
		//m_fSpeed = -1 * m_fSpeed;
		//SetVelY(100);
	}
	return true;
}

RECT CCar::GetRect()
{
	RECT temp_rect;
	temp_rect.left = (int)GetPosX() - (int)(GetWidth() * 0.5f);
	temp_rect.top = (int)GetPosY() - (int)(GetHeight() * 0.5f);
	temp_rect.right = temp_rect.left + GetWidth();
	temp_rect.bottom = temp_rect.top + GetHeight();

	return temp_rect;
}

void CCar::HandleEvent(CEvent* pEvent)
{
	if(this == pEvent->GetParam())
	{
		
	}
}