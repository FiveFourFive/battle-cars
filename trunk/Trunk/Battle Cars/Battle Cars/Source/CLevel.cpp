/////////////////////////////////////////////////
//	File	:	"CLevel.h"
//
//	Author	:	Josh Fields
//
//	Purpose	:	Contains the functions for rendering
//				and checking map collisions and sending events.
//				
/////////////////////////////////////////////////

#include "CSGD_TextureManager.h"
#include "CSGD_Direct3D.h"
#include "tinyXml.h"
#include "CLevel.h"
#include "CEventSystem.h"
#include "CMap.h"
#include "CTile.h"
#include "CPlayer.h"
#include "CBase.h"
#include "CCamera.h"
#include "CEnemy.h"
#include "CSGD_Direct3D.h"
#include "CObjectManager.h"
#include "CGamePlayState.h"
#include "CSpeedRamp.h"
#include "CObstacle.h"
#include "CPowerUp.h"

CLevel::CLevel()
{
	m_pTM	= CSGD_TextureManager::GetInstance ();
	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pES = CEventSystem::GetInstance();

	LevelMap = new CMap();
}

CLevel* CLevel::GetInstance ()
{
	static CLevel instance;
	return &instance;
}

void CLevel::Shutdown ()
{
	delete LevelMap;
	LevelMap = NULL;
}


bool CLevel::Load (const char* filename)
{
	TiXmlDocument doc;

	if (!LevelMap)
	{
		LevelMap = new CMap();
	}

	if (doc.LoadFile (filename) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement ();

	if (!pRoot)
		return false;

	TiXmlElement* pLevelInfo = pRoot->FirstChildElement ("LevelInfo");
	
	
	int width;
	int height;

	pLevelInfo->Attribute ("Width", &width);
	pLevelInfo->Attribute ("Height", &height);
	LevelMap->SetMapWidth (width);
	LevelMap->SetMapHeight (height);

	pLevelInfo->Attribute ("PixelWidth", &width);
	pLevelInfo->Attribute ("PixelHeight", &height);
	LevelMap->SetPixelWidth (width);
	LevelMap->SetPixelHeight (height);
	 
	LevelMap->SetMapName (pLevelInfo->Value ());

	TiXmlElement* pTileLocation = pRoot->FirstChildElement ("TileLocation");
	

	if (pTileLocation)
	{

		LevelMap->LoadTiles (pTileLocation->GetText());

		TiXmlElement* pEventsLocation = pTileLocation->NextSiblingElement ("EventLocation");
		if (pEventsLocation)
		{
			LevelMap->LoadEvents (pEventsLocation->GetText());
		}	
	}

	return true;
}
	
void CLevel::Render (CCamera* camera)
{
	CTile** Tiles = LevelMap->GetTileList ();
	RECT screen = camera->GetRect();

	int XBegin = 0, YBegin = 0, XEnd = 0, YEnd;

	XBegin = (screen.left / LevelMap->GetPixelWidth());
	YBegin = (screen.top / LevelMap->GetPixelHeight());
	XEnd = (screen.right / LevelMap->GetPixelWidth());
	YEnd = (screen.bottom / LevelMap->GetPixelHeight());

	if (XBegin - 1 >= 0)
	{
		XBegin = XBegin - 1;
	}

	if (YBegin - 1 >= 0)
	{
		YBegin = YBegin - 1;
	}

	if (XEnd + 1 <= LevelMap->GetMapWidth ())
	{
		XEnd = XEnd + 1;
	}

	if (YEnd + 1 <= LevelMap->GetMapHeight ())
	{
		YEnd = YEnd + 1;
	}

	for (int YPos = YBegin; YPos < YEnd; YPos++)
	{
		for (int XPos = XBegin; XPos < XEnd; XPos++)
		{
			int TileXPos = Tiles[YPos][XPos].GetXPos();
			int TileYPos = Tiles[YPos][XPos].GetYPos();

			int PickedXPos = Tiles[YPos][XPos].GetXPicked();
			int PickedYPos = Tiles[YPos][XPos].GetYPicked();

			RECT TileSelection;
			TileSelection.left = PickedXPos * LevelMap->GetPixelWidth();
			TileSelection.top = PickedYPos * LevelMap->GetPixelHeight();
			TileSelection.right = TileSelection.left + LevelMap->GetPixelWidth();
			TileSelection.bottom = TileSelection.top + LevelMap->GetPixelHeight ();
				
			m_pTM->Draw (LevelMap->GetTileImageID (), (int)((TileXPos *  LevelMap->GetPixelWidth()) - camera->GetCamX () + camera->GetRenderPosX ()), 
													  (int)((TileYPos * LevelMap->GetPixelHeight()) - camera->GetCamY () + camera->GetRenderPosY ()), 1.0f, 1.0f, &TileSelection);

			std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "WallCollision")
				{
					CSGD_Direct3D::GetInstance ()->GetSprite()->Flush ();

					RECT Wall = LevelMap->GetCollisionRect(XPos, YPos);
					int width = Wall.right - Wall.left;
					int Height = Wall.bottom - Wall.top;

					Wall.left = (LONG)(Wall.left - camera->GetCamX ());
					Wall.top = (LONG)(Wall.top  - camera->GetCamY ());
					Wall.right = Wall.left + width;
					Wall.bottom = Wall.top + Height;

					CSGD_Direct3D::GetInstance ()->DrawRect (Wall, 255, 255, 255);
				}
		}
		
	}

	CSGD_Direct3D::GetInstance ()->GetSprite()->Flush ();
}


bool CLevel::CheckPlayerCollision (CBase* pBase, CCamera* camera)
{
	CTile** Events = LevelMap->GetEventsList();

	int XBegin = 0, YBegin = 0, XEnd = 0, YEnd;

	XBegin = (camera->GetRect ().left / LevelMap->GetPixelWidth());
	YBegin = (camera->GetRect ().top / LevelMap->GetPixelHeight());
	XEnd = (camera->GetRect ().right / LevelMap->GetPixelWidth());
	YEnd = (camera->GetRect ().bottom / LevelMap->GetPixelHeight());

	if (XBegin - 1 >= 0)
	{
		XBegin = XBegin - 1;
	}

	if (YBegin - 1 >= 0)
	{
		YBegin = YBegin - 1;
	}

	if (XEnd + 1 <= LevelMap->GetMapWidth ())
	{
		XEnd = XEnd + 1;
	}

	if (YEnd + 1 <= LevelMap->GetMapHeight ())
	{
		YEnd = YEnd + 1;
	}

	for (int YPos = YBegin; YPos < YEnd; YPos++)
	{
		for (int XPos = XBegin; XPos < XEnd; XPos++)
		{
			if (pBase->GetType () == OBJECT_PLAYER && (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "WallCollision")
				{
					float TopCenterX = ((CPlayer*)pBase)->GetCX1 ();
					float TopCenterY = ((CPlayer*)pBase)->GetCY1 ();
					float BottomCenterX = ((CPlayer*)pBase)->GetCX2 ();
					float BottomCenterY = ((CPlayer*)pBase)->GetCY2 ();

					float radius = /*(((CPlayer*)pBase)->GetHeight() * 0.5f)*0.5f;*/((CPlayer*)pBase)->GetRadius ();

					RECT Wall = LevelMap->GetCollisionRect(XPos, YPos);
					tVector2D temp = ((CPlayer*)pBase)->GetOverallVelocity();

					//check the top side of the wall
					if ((BottomCenterY + radius >= Wall.top && BottomCenterY < Wall.bottom && (BottomCenterX >= Wall.left && BottomCenterX <= Wall.right)) ||
						(TopCenterY + radius >= Wall.top && TopCenterY < Wall.bottom && (TopCenterX >= Wall.left && TopCenterX <= Wall.right)))
					{
						((CPlayer*)pBase)->SetPosY (((CPlayer*)pBase)->GetPosY()-0.5f);
						temp.fY = -1.0f * ((CPlayer*)pBase)->GetOverallVelocity().fY * 0.6f;
						((CPlayer*)pBase)->SetVelocity (temp);
						((CPlayer*)pBase)->Rotate (((CPlayer*)pBase)->GetRotation ());
						((CPlayer*)pBase)->SetSpeed (0.0f);
					}
					
					if ((BottomCenterY - radius  <= Wall.bottom && BottomCenterY > Wall.top && (BottomCenterX >= Wall.left && BottomCenterX <= Wall.right)) ||
						(TopCenterY - radius  <= Wall.bottom && TopCenterY > Wall.top && (TopCenterX >= Wall.left && TopCenterX <= Wall.right)))//check the bottom side of the wall
					{
						((CPlayer*)pBase)->SetPosY (((CPlayer*)pBase)->GetPosY() + 0.5f);
						temp.fY = -1.0f * ((CPlayer*)pBase)->GetOverallVelocity().fY * 0.6f;
						((CPlayer*)pBase)->SetVelocity (temp);
						((CPlayer*)pBase)->Rotate (((CPlayer*)pBase)->GetRotation ());
						((CPlayer*)pBase)->SetSpeed (0.0f);
					}

					if ((BottomCenterX + radius >= Wall.left && BottomCenterX < Wall.left && (BottomCenterY >= Wall.top && BottomCenterY <= Wall.bottom)) ||
						(TopCenterX + radius >= Wall.left && TopCenterX < Wall.right && (TopCenterY >= Wall.top && TopCenterY <= Wall.bottom)))//check the Left side of the wall
					{
						((CPlayer*)pBase)->SetPosX (((CPlayer*)pBase)->GetPosX() - 0.5f);
						temp.fX = -1.0f * ((CPlayer*)pBase)->GetOverallVelocity().fX * 0.6f;
						((CPlayer*)pBase)->SetVelocity (temp);
						((CPlayer*)pBase)->Rotate (((CPlayer*)pBase)->GetRotation ());
						((CPlayer*)pBase)->SetSpeed (0.0f);
					}
					
					if ((BottomCenterX - radius <= Wall.right && BottomCenterX > Wall.left &&(BottomCenterY >= Wall.top && BottomCenterY <= Wall.bottom)) ||
						(TopCenterX - radius <= Wall.right && TopCenterX > Wall.left &&(TopCenterY >= Wall.top && TopCenterY <= Wall.bottom)))//check the Right side of the wall
					{
						((CPlayer*)pBase)->SetPosX (((CPlayer*)pBase)->GetPosX() + 0.5f);
						temp.fX = -1.0f * ((CPlayer*)pBase)->GetOverallVelocity().fX * 0.6f;
						((CPlayer*)pBase)->SetVelocity (temp);
						((CPlayer*)pBase)->Rotate (((CPlayer*)pBase)->GetRotation ());
						((CPlayer*)pBase)->SetSpeed (0.0f);
					}
				}
			}
		}
	}

	return false;
}

bool CLevel::CheckCameraCollision (CCamera* camera)
{
	RECT intersection;
	CTile** Events = LevelMap->GetEventsList();

	int XBegin = 0, YBegin = 0, XEnd = 0, YEnd;

	XBegin = (camera->GetRect ().left / LevelMap->GetPixelWidth());
	YBegin = (camera->GetRect ().top / LevelMap->GetPixelHeight());
	XEnd = (camera->GetRect ().right / LevelMap->GetPixelWidth());
	YEnd = (camera->GetRect ().bottom / LevelMap->GetPixelHeight());

	if (XBegin - 1 >= 0)
	{
		XBegin = XBegin - 1;
	}

	if (YBegin - 1 >= 0)
	{
		YBegin = YBegin - 1;
	}

	if (XEnd + 1 <= LevelMap->GetMapWidth ())
	{
		XEnd = XEnd + 1;
	}

	if (YEnd + 1 <= LevelMap->GetMapHeight ())
	{
		YEnd = YEnd + 1;
	}

	for (int YPos = YBegin; YPos < YEnd; YPos++)
	{
		for (int XPos = XBegin; XPos < XEnd; XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "CameraCollision")
				{
					if(IntersectRect(&intersection, &LevelMap->GetCollisionRect(XPos, YPos), &camera->GetRect()))
					{
						m_pES->SendEvent ((LevelMap->GetEventsList ())[YPos][XPos].GetName (), camera);

						return true;
					}
				}
			}

		}

	}
	return false;
}

bool CLevel::CheckObstacleCollision (CBase* pBase)
{
	CTile** Events = LevelMap->GetEventsList();
	RECT intersection;

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if (pBase->GetType () == OBJECT_PLAYER && (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "WallCollision")
				{
					if(IntersectRect(&intersection, &LevelMap->GetCollisionRect(XPos, YPos), &pBase->GetRect()))
					{
						tVector2D temp;
						temp.fX = 0.0f;
						temp.fY = 0.0f;

						((CObstacle*)pBase)->SetVel (temp);

						//((CObstacle*)pBase)->SetPosX (LevelMap->GetCollisionRect(XPos, YPos));

						return true;
					}
				}
			}
		}
	}

	return false;
}


void CLevel::SetCarSpawn (vector<CBase*> pBases)
{
	CTile** Events = LevelMap->GetEventsList();
	std::vector <tVector2D> spawnPoints;

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "PlayerSpawn")
				{
					if((LevelMap->GetEventsList ())[YPos][XPos].InUse () == false)
					{
						tVector2D point;
						point.fX = (float)XPos;
						point.fY = (float)YPos;

						spawnPoints.push_back (point);
					}
				}
			}
		}
	}

	unsigned int index = 0;

	while (index < spawnPoints.size () && index < pBases.size ())
	{
		int randomIndex = rand() % spawnPoints.size ();

		if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].InUse () == false)
		{
			(LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].SetInUse (true);

			pBases[index]->SetPosX ((float)(spawnPoints[randomIndex].fX * LevelMap->GetPixelWidth ()));
			pBases[index]->SetPosY ((float)(spawnPoints[randomIndex].fY * LevelMap->GetPixelHeight ()));

			CObjectManager::GetInstance ()->AddObject(pBases[index]);

			index++;
		}
	}
}

std::vector<CSpeedRamp*> CLevel::SetSpeedRampSpawn ()
{
	CTile** Events = LevelMap->GetEventsList();
	std::vector <tVector2D> spawnPoints;

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "SpeedRampSpawn" && (LevelMap->GetEventsList ())[YPos][XPos].InUse () == false)
				{
					tVector2D point;
					point.fX = (float)XPos;
					point.fY = (float)YPos;

					spawnPoints.push_back (point);
				}
			}
		}
	}
	
	//loop tell all spawns are taken
	//check a random index for in use
	// if not in use
	//create a new speed ramp
	// set data
	//decrease the number of spawns left

	unsigned int index = 0;
	std::vector<CSpeedRamp*> ramps;

	while (index < spawnPoints.size ())
	{
		int randomIndex = rand() % spawnPoints.size ();

		if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].InUse () == false)
		{
			(LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].SetInUse (true);
			index++;
			CSpeedRamp* ramp = new CSpeedRamp();
			ramp->SetPosX (spawnPoints[randomIndex].fX * LevelMap->GetPixelWidth ());
			ramp->SetPosY (spawnPoints[randomIndex].fY * LevelMap->GetPixelHeight ());
			ramp->SetImageID (m_pTM->LoadTexture("resource/graphics/speedramp.png"));
			if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 7)
			{
				ramp->RotateVel(0.0f);

			}else if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 8)
			{
				ramp->RotateVel (3.14f);
			}
			else if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 9)
			{
				ramp->RotateVel (3.14f / 2.0f);
			}
			else if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 10)
			{
				ramp->RotateVel ((3.0f/2.0f)*3.14f);
			}
			ramps.push_back (ramp);
			CObjectManager::GetInstance ()->AddObject(ramp);
		}
	}

	return ramps;
}

std::vector<PowerUp*> CLevel::SetPowerUpSpawn ()
{
	CTile** Events = LevelMap->GetEventsList();
	std::vector <tVector2D> spawnPoints;

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "PowerUpSpawn" && (LevelMap->GetEventsList ())[YPos][XPos].InUse () == false)
				{
					tVector2D point;
					point.fX = (float)XPos;
					point.fY = (float)YPos;

					spawnPoints.push_back (point);
				}
			}
		}
	}

	//loop tell all spawns are taken
	//check a random index for in use
	// if not in use
	//create a new power up
	// set data
	//decrease the number of spawns left

	unsigned int index = 0;
	std::vector<PowerUp*> powers;

	while (index < spawnPoints.size ())
	{
		int randomIndex = rand() % spawnPoints.size ();

		if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].InUse () == false)
		{
			(LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].SetInUse (true);
			index++;
			PowerUp* power = new PowerUp();
			power->SetPosX (spawnPoints[randomIndex].fX * LevelMap->GetPixelWidth ());
			power->SetPosY (spawnPoints[randomIndex].fY * LevelMap->GetPixelHeight ());
			power->SetType(OBJECT_POWERUP);
			if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 3)
			{
				power->SetImageID (m_pTM->LoadTexture("resource/graphics/specialup.png"));
				power->SetPowerType(SPECIAL_POWERUP);

			}else if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 4)
			{
				power->SetImageID (m_pTM->LoadTexture("resource/graphics/weaponup.png"));
				power->SetPowerType(WEAPONS_POWERUP);
			}
			else if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 5)
			{
				power->SetImageID (m_pTM->LoadTexture("resource/graphics/healthup.png"));
				power->SetPowerType(HEALTH_POWERUP);
			}
			else if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 6)
			{
				power->SetImageID (m_pTM->LoadTexture("resource/graphics/armorup.png"));
				power->SetPowerType(SHIELD_POWERUP);
			}
			powers.push_back (power);
			CObjectManager::GetInstance ()->AddObject(power);
		}
	}

	return powers;
}

std::vector<CObstacle*> CLevel::SetObstacleSpawn ()
{
	CTile** Events = LevelMap->GetEventsList();
	std::vector <tVector2D> spawnPoints;

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "ObstacleSpawn" && (LevelMap->GetEventsList ())[YPos][XPos].InUse () == false)
				{
					tVector2D point;
					point.fX = (float)XPos;
					point.fY = (float)YPos;

					spawnPoints.push_back (point);
				}
			}
		}
	}

	//loop tell all spawns are taken
	//check a random index for in use
	// if not in use
	//create a new obstacle
	// set data
	//decrease the number of spawns left

	unsigned int index = 0;
	std::vector <CObstacle*> obstacles;

	while (index < spawnPoints.size ())
	{
		int randomIndex = rand() % spawnPoints.size ();

		if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].InUse () == false)
		{
			(LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].SetInUse (true);
			index++;
			CObstacle* obstacle = new CObstacle();
			obstacle->SetPosX (spawnPoints[randomIndex].fX * LevelMap->GetPixelWidth ());
			obstacle->SetPosY (spawnPoints[randomIndex].fY * LevelMap->GetPixelHeight ());
			obstacle->SetType(OBJECT_OBSTACLE);
			if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 1)
			{
				obstacle->SetImageID (CGamePlayState::GetInstance ()->GetCrateImageID());

			}else if ((LevelMap->GetEventsList ())[(int)(spawnPoints[randomIndex].fY)][(int)(spawnPoints[randomIndex].fX)].GetID () == 2)
			{
				obstacle->SetImageID (CGamePlayState::GetInstance ()->GetBarrelImageID());
			}

			obstacles.push_back (obstacle);
			CObjectManager::GetInstance ()->AddObject(obstacle);
		}
	}

	return obstacles;
}

bool CLevel::CheckEnemyCollision (CBase* pBase)
{
	//int StartIndex = (int)(((((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth ()) + (((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelWidth()));
	//int EndIndex = (int)(((((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth()) + (((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelWidth()));

	//if (StartIndex < 0)
	//	StartIndex = 0;

	//for (int i = StartIndex; i <  EndIndex; i++)
	//{
	//	if (pBase->GetType () == OBJECT_ENEMY)
	//	{
	//		//check if collision rect is in enemies radius;
	//		//if(IntersectRect(&intersection, &LevelMap->GetCollisionRect(Index), &pBase->GetRect()))
	//		{
	//				((CEnemy*)pBase)->AddCollisionRect (LevelMap->GetCollisionRect(i));
	//				//m_pES->SendEvent (LevelMap->GetCollisionList()[i].GetName (), pBase);
	//		}
	//	}
	//}

	//if (((CEnemy*)pBase)->GetCollisionRects ().size () > 0)
	//{
	//	return true;
	//}


	
	return false;
}

void CLevel::RestSpawns ()
{
	CTile** Events = LevelMap->GetEventsList();

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				(LevelMap->GetEventsList ())[YPos][XPos].SetInUse (false);
			}
		}
	}
}