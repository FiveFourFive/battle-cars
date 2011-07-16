
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
#include "ParticleManager.h"
#include "CMap.h"
#include "Emittor.h"
#include "CLevel.h"

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
	SetIsAlive(true);
	m_tOverallVelocity.fX = 0.0f;
	m_tOverallVelocity.fY = 0.0f;

	SetPosX(350);
	SetPosY(225);
	m_nSpecialLevel = 1;
	SetWidth(52);
	SetHeight(70);
	SetImageWidth(256);
	SetImageHeight(256);
	m_nCollisionRadius = (float)(GetWidth()/2.0f);
	m_nCollisionX1 = GetPosX();
	m_nCollisionY1 = GetPosY() - (GetHeight()*0.5f) + (GetWidth()*0.5f);
	m_nCollisionX2 = GetPosX();
	m_nCollisionY2 = GetPosY() + (GetHeight()*0.5f) - (GetWidth()*0.5f);
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
	m_nCarID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/BC_Truck.png");

	m_fCollisionEffect = 0.0f;
	m_fRespawnTimer = 0.0f;

	CEventSystem::GetInstance()->RegisterClient("damage",this);
	CEventSystem::GetInstance()->RegisterClient("collision", this);
	CEventSystem::GetInstance()->RegisterClient("weapon_level",this);
}


void CCar::Update(float fElapsedTime)
{
	if( GetHealth() <= 0.0f)
	{
		m_fRespawnTimer += fElapsedTime;
		return;
	}

	m_fCollisionEffect += fElapsedTime;

	tVector2D tempdir = GetDirection();
	tVector2D tempvel = GetVelocity();
	//SetDirection(tempdir);
	tempdir = Vector2DNormalize(tempdir);
	tempdir = tempdir * GetSpeed();

	if(tempvel.fX > 0)
	{
		// adjusting these changes the time it takes for the velocity 
		// to get back to the direction vector
		tempvel.fX -= tempvel.fX * 0.05f;
	}
	if(tempvel.fY > 0)
	{
		tempvel.fY -= tempvel.fY * 0.05f;
	}
	if(tempvel.fX < 0)
	{
		tempvel.fX -= tempvel.fX * 0.05f;
	}
	if(tempvel.fY < 0)
	{
		tempvel.fY -= tempvel.fY * 0.05f;
	}

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
	m_nCollisionX2 = m_nCollisionX2 + (GetVelX() * fElapsedTime);
	m_nCollisionY2 = m_nCollisionY2 + (GetVelY() * fElapsedTime);
	CBase::Update(fElapsedTime);
	InBounds(fElapsedTime);
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
	

	m_pTM->Draw(m_nCarID,(int)GetPosX()-(GetWidth()/2)- (int)camera->GetCamX() + (int)camera->GetRenderPosX(),
		(int)GetPosY()-(GetHeight()/2)- (int)camera->GetCamY() + (int)camera->GetRenderPosY(),0.4f,0.4f,NULL, GetImageWidth()/2,GetImageHeight()/2,GetRotation());
	//pD3D->DrawRect(tempcar,255,0,0);
	//pD3D->DrawText("BEEP", (int)(GetPosX()- camera->GetCamX() + 10), (int)(GetPosY()- camera->GetCamY() + 35),255,255,255);
	//pD3D->DrawLine((int)(GetPosX()- camera->GetCamX() + camera->GetRenderPosX()), (int)(GetPosY()- camera->GetCamY()+ camera->GetRenderPosY()), (int)(GetPosX()- camera->GetCamX() + camera->GetRenderPosX() + GetVelX()), (int)(GetPosY()- camera->GetCamY() + camera->GetRenderPosY() + GetVelY()),255,255,255);
	//pD3D->DrawLine(400, 600, 400, 550,255,255,255);
	float dir1 = GetDirection().fX;
	float dir2 = GetDirection().fY;
	//pD3D->DrawLine((int)(GetPosX()- camera->GetCamX()+ camera->GetRenderPosX()), (int)(GetPosY()- camera->GetCamY() + camera->GetRenderPosY()), (int)(GetPosX() + (20 * dir1)- camera->GetCamX() + camera->GetRenderPosX()), (int)(GetPosY() + (20 * dir2)- camera->GetCamY() + camera->GetRenderPosY()), 0,255,0);
	char buffer[128];
	sprintf_s(buffer,"fX: %f	fY: %f",dir1,dir2);
	//pD3D->DrawText(buffer,(int)(GetPosX()- camera->GetCamX() + camera->GetRenderPosX()), (int)(GetPosY()- camera->GetCamY()+ camera->GetRenderPosY()),0,0,255);
	sprintf_s(buffer,"R: %f",GetRotation());
	//pD3D->DrawText(buffer,(int)(GetPosX()- camera->GetCamX()+ camera->GetRenderPosX())+10, (int)(GetPosY()- camera->GetCamY()+ camera->GetRenderPosY())+20,0,0,255);


	// testing collision rotation
	
	//m_nCollisionX1 = GetPosX() - camera->GetCamX();
	//m_nCollisionY1 = GetPosY() - camera->GetCamY();
	pD3D->GetSprite()->Flush();
	RECT tempcircle1;
	tempcircle1.left = (int)(m_nCollisionX1 - camera->GetCamX() + camera->GetRenderPosX());
	tempcircle1.top = (int)(m_nCollisionY1 - camera->GetCamY() + camera->GetRenderPosY());
	tempcircle1.right = tempcircle1.left + (int)m_nCollisionRadius;
	tempcircle1.bottom = tempcircle1.top + (int)m_nCollisionRadius;

	//pD3D->DrawRect(tempcircle1,255,255,255);

	RECT tempcircle2;
	tempcircle2.left = m_nCollisionX2 - camera->GetCamX() + camera->GetRenderPosX();
	tempcircle2.top = m_nCollisionY2 - camera->GetCamY() + camera->GetRenderPosY();
	tempcircle2.right = tempcircle2.left + m_nCollisionRadius;
	tempcircle2.bottom = tempcircle2.top + m_nCollisionRadius;

	//pD3D->DrawRect(tempcircle2,0,0,255);
	// end collision testing


}
void CCar::Rotate(float angle)
{
	float newangle = angle / 180.0f * 3.14159f;

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


	m_nCollisionX2 = GetPosX();
	m_nCollisionY2 = GetPosY() + (GetHeight() * 0.5f) - (GetWidth() * 0.5f);

	float tempX2 = m_nCollisionX2;
	float tempY2 = m_nCollisionY2;
	angle = angle + (3.14159f);
	m_nCollisionX2 = GetPosX() + (cos(angle)) * (tempX1 - GetPosX()) - sin(angle) * (tempY1 - GetPosY());
	m_nCollisionY2 = GetPosY() + (sin(angle)) * (tempX1 - GetPosX()) + cos(angle) * (tempY1 - GetPosY()); 
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
	if(this == pBase)
		return false;
	if(pBase->GetType() == OBJECT_ENEMY || pBase->GetType() == OBJECT_PLAYER)
	{
		CCar* tempcar = (CCar*)pBase;
		//float centerx = tempcar->GetPosX();
		//float centery = tempcar->GetPosY();
		float centerx = tempcar->GetCX1();
		float centery = tempcar->GetCY1();
		float myx = GetCX1();
		float myy = GetCY1();

		float centerx2 = tempcar->GetCX2();
		float centery2 = tempcar->GetCY2();
		float myx2 = GetCX2();
		float myy2 = GetCY2();
		float distance11 = sqrt(((centerx - myx)*(centerx - myx)) + ((centery - myy)*(centery - myy)));
		float distance21 = sqrt(((centerx2 - myx2)*(centerx2 - myx2)) + ((centery2 - myy2)*(centery2 - myy2)));
		float distance12 = sqrt(((centerx2 - myx)*(centerx2 - myx)) + ((centery2 - myy)*(centery2 - myy)));
		float distance22 = sqrt(((centerx - myx2)*(centerx - myx2)) + ((centery - myy2)*(centery - myy2)));
		if(distance11 <= (GetRadius() + tempcar->GetRadius()) || distance21 <= (GetRadius() + tempcar->GetRadius())
			|| distance12 <= (GetRadius() + tempcar->GetRadius()) || distance22 <= (GetRadius() + tempcar->GetRadius()))
		{
			tVector2D othervel = tempcar->GetOverallVelocity();
			tVector2D currentvel = GetOverallVelocity();

			float myfx = abs(currentvel.fX);
			float myfy = abs(currentvel.fY);
			float hisfx = abs(othervel.fX);
			float hisfy = abs(othervel.fY);
			
			tVector2D tobeapplied;
			tobeapplied.fX = 0;
			tobeapplied.fY = 0;
			if((myfx+myfy) > (hisfx+hisfy))
			{
				currentvel = currentvel * -0.8f;
				tobeapplied = currentvel;
				SetVelocity(tobeapplied);
				tempcar->SetVelocity(tobeapplied * -1.0f);
			}
			else
			{
				othervel = othervel * -0.8f;
				tobeapplied = othervel;
				tempcar->SetVelocity(tobeapplied);
				SetVelocity(tobeapplied * -1.0f);
			}

		
			float myspeed = GetSpeed();
			float hisspeed = tempcar->GetSpeed();
			
			if(myspeed > hisspeed)
			{
				myspeed = myspeed * 0.3f;
				SetSpeed(-1.0f * myspeed);
				tempcar->SetSpeed(myspeed);
			}
			else
			{
				hisspeed = hisspeed * 0.3f;
				tempcar->SetSpeed(-1.0f * hisspeed);
				SetSpeed(hisspeed);
			}


		
			return true;
		}
		
	}
	return false;
}

bool CCar::InBounds(float fElapsedTime)
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
		Rotate(GetRotation());

		return false;
		//SetVelX(0);
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
		Rotate(GetRotation());
		return false;
		//m_fSpeed = -1 * m_fSpeed;
		//SetVelY(100);
		//SetVelY(0);
	}
	CLevel* level = CGamePlayState::GetInstance()->GetLevel();
	CMap* map = level->GetMap();
	int mapheight = map->GetPixelHeight() * map->GetMapHeight();
	int mapwidth = map->GetPixelWidth() * map->GetMapWidth();
	if(GetPosX() >= mapwidth)
	{
		SetPosX(mapwidth);
		Rotate(GetRotation());
		return false;
	}
	if(GetPosY() >= mapheight)
	{
		SetPosY(mapheight);
		Rotate(GetRotation());
		return false;
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
	CCar* tempcar = (CCar*)pEvent->GetParam();
	
	if(this == pEvent->GetParam())
	{
		if( pEvent->GetEventID() == "collision")
		{
			ParticleManager* pPM = ParticleManager::GetInstance(); 
			Emittor* tempemittor = pPM->CreateEffect(pPM->GetEmittor(COLLISION_EMITTOR), GetCX1(), GetCY1());

			if( tempemittor)
			{
				tempemittor->SetTimeToDie(1.0f);
				pPM->AttachToBasePosition(NULL, tempemittor, GetCX1(), GetCY1());
			}
		}
		else if(pEvent->GetEventID() == "weapon_level")
		{
			if(GetSpecialLevel() < 4)
				SetSpecialLevel(GetSpecialLevel() + 1);
		}
		else if(pEvent->GetEventID() == "powerup_power")
		{
			if(GetPowerUpBar() < GetMaxPowerUp())
				SetPowerUpBar(GetPowerUpBar() + 20);
			if(GetPowerUpBar() > GetMaxPowerUp())
				SetPowerUpBar(GetMaxPowerUp());
		}
	}
}