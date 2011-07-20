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
		}
		
	}

	CSGD_Direct3D::GetInstance ()->GetSprite()->Flush ();
}


bool CLevel::CheckPlayerCollision (CBase* pBase, CCamera* camera)
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
			if (pBase->GetType () == OBJECT_PLAYER && (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "WallCollision")
				{
					if(IntersectRect(&intersection, &LevelMap->GetCollisionRect(XPos, YPos), &pBase->GetRect()))
					{
						((CPlayer*)pBase)->SetWallCollisionRect (intersection);
						((CPlayer*)pBase)->SetWallRect (LevelMap->GetCollisionRect(XPos, YPos));
						m_pES->SendEvent ((LevelMap->GetEventsList ())[YPos][XPos].GetName (), pBase);

						return true;
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

void CLevel::SetCarSpawn (CBase* pBase)
{
	CTile** Events = LevelMap->GetEventsList();

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
						pBase->SetPosX (XPos * LevelMap->GetPixelWidth ());
						pBase->SetPosY (YPos * LevelMap->GetPixelHeight ());
						pBase->SetSpawnPosX (pBase->GetPosX ());
						pBase->SetSpawnPosY (pBase->GetPosY ());
						(LevelMap->GetEventsList ())[YPos][XPos].SetInUse (true);
						return;
					}
				}
			}
		}
	}
}

void CLevel::SetSpeedRampSpawn (CBase* pBase)
{
	CTile** Events = LevelMap->GetEventsList();

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "SpeedRampSpawn" && (LevelMap->GetEventsList ())[YPos][XPos].InUse () == false)
				{
					pBase->SetPosX (XPos * LevelMap->GetPixelWidth ());
					pBase->SetPosY (YPos * LevelMap->GetPixelHeight ());
					(LevelMap->GetEventsList ())[YPos][XPos].SetInUse (true);
					return;
				}
			}
		}
	}
}

void CLevel::SetPowerUpSpawn (CBase* pBase)
{
	CTile** Events = LevelMap->GetEventsList();

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "PowerUpSpawn" && (LevelMap->GetEventsList ())[YPos][XPos].InUse () == false)
				{
					pBase->SetPosX (XPos * LevelMap->GetPixelWidth ());
					pBase->SetPosY (YPos * LevelMap->GetPixelHeight ());
					(LevelMap->GetEventsList ())[YPos][XPos].SetInUse (true);
					return;
				}
			}
		}
	}
}

void CLevel::SetObsticleSpawn (CBase* pBase)
{
	CTile** Events = LevelMap->GetEventsList();

	for (int YPos = 0; YPos < LevelMap->GetMapHeight(); YPos++)
	{
		for (int XPos = 0; XPos < LevelMap->GetMapWidth(); XPos++)
		{
			if ( (LevelMap->GetEventsList ())[YPos][XPos].GetType () != -1)
			{
				std::string name = (LevelMap->GetEventsList ())[YPos][XPos].GetName ();
				if (name == "ObsticleSpawn" && (LevelMap->GetEventsList ())[YPos][XPos].InUse () == false)
				{
					pBase->SetPosX (XPos * LevelMap->GetPixelWidth ());
					pBase->SetPosY (YPos * LevelMap->GetPixelHeight ());
					pBase->SetSpawnPosX (pBase->GetPosX ());
					pBase->SetSpawnPosY (pBase->GetPosY ());
					(LevelMap->GetEventsList ())[YPos][XPos].SetInUse (true);
					return;
				}
			}
		}
	}
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