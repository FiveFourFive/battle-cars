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

		TiXmlElement* pCollisionLocation = pTileLocation->NextSiblingElement ("CollisionLocation");
		if (pCollisionLocation)
		{
			LevelMap->LoadCollisions (pCollisionLocation->GetText());

			TiXmlElement* pEventsLocation = pCollisionLocation->NextSiblingElement ("EventLocation");
			if (pEventsLocation)
			{
				LevelMap->LoadEvents (pEventsLocation->GetText());

				TiXmlElement* pSpawnLocation = pEventsLocation->NextSiblingElement ("SpawnLocation");
				if (pSpawnLocation)
				{
					LevelMap->LoadSpawns (pSpawnLocation->GetText());
				}
			}
		}		
	}

	return true;
}
	
void CLevel::Render (RECT screen)
{
	CTile* Tiles = LevelMap->GetTileList ();


	int StartIndex = ((screen.top / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth ()) + (screen.left / LevelMap->GetPixelWidth());
	int EndIndex = ((screen.bottom / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth()) + (screen.right / LevelMap->GetPixelWidth());

	if (StartIndex < 0)
		StartIndex = 0;

	int i = 0;
	int j = 0;

	for (int Index = StartIndex; Index < EndIndex; Index++, i++)
	{
		int TileXPos = Tiles[Index].GetIndex() % LevelMap->GetMapWidth();
		int TileYPos = Tiles[Index].GetIndex() / LevelMap->GetMapHeight();

		/*if (TileXPos *  LevelMap->GetPixelWidth() > screen.right || TileYPos * LevelMap->GetPixelHeight() > screen.bottom
			|| TileXPos *  LevelMap->GetPixelWidth() < screen.left || TileYPos * LevelMap->GetPixelHeight() < screen.top )
		{
			continue;
		}*/

		int PickedXPos = Tiles[Index].GetXPos();
		int PickedYPos = Tiles[Index].GetYPos();

		RECT TileSelection;
		TileSelection.left = PickedXPos * LevelMap->GetPixelWidth();
		TileSelection.top = PickedYPos * LevelMap->GetPixelHeight();
		TileSelection.right = TileSelection.left + LevelMap->GetPixelWidth();
		TileSelection.bottom = TileSelection.top + LevelMap->GetPixelHeight ();

		if( i == 100)
		{
			i = 0;
			j++;
		}
		
		m_pTM->Draw (LevelMap->GetTileImageID (), /*TileXPos*/ i *  LevelMap->GetPixelWidth(),/*TileYPos */j * LevelMap->GetPixelHeight(), 1.0f, 1.0f, &TileSelection);
	}

	CSGD_Direct3D::GetInstance ()->GetSprite()->Flush ();
}


bool CLevel::CheckWorldCollision (CBase* pBase)
{
	RECT intersection;

	int StartIndex = ((((CPlayer*)pBase)->GetCamera ()->GetRect().top / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth ()) + (((CPlayer*)pBase)->GetCamera ()->GetRect().left / LevelMap->GetPixelWidth());
	int EndIndex = ((((CPlayer*)pBase)->GetCamera ()->GetRect().bottom / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth()) + (((CPlayer*)pBase)->GetCamera ()->GetRect().right / LevelMap->GetPixelWidth());

	if (StartIndex < 0)
		StartIndex = 0;

	CTile* collisionList = LevelMap->GetCollisionList();

	for (int Index = StartIndex; Index < EndIndex; Index++)
	{
		if (pBase->GetType () == OBJECT_PLAYER)
		{
			if(IntersectRect(&intersection, &LevelMap->GetCollisionRect(Index), &pBase->GetRect()))
			{
				/*if (intersection.right - intersection.left < intersection.bottom - intersection.top && LevelMap->GetCollisionRect(Index).left < pBase->GetRect ().left)
				{*/
					((CPlayer*)pBase)->SetCollisionRect (LevelMap->GetCollisionRect(Index));
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), pBase);
				/*}else if (intersection.right - intersection.left < intersection.bottom - intersection.top && LevelMap->GetCollisionRect(Index).right > pBase->GetRect ().right)
				{
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), pBase);
				}else if (intersection.bottom - intersection.top < intersection.right - intersection.left && LevelMap->GetCollisionRect(Index).top < pBase->GetRect ().top)
				{
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), pBase);
				}else if (intersection.bottom - intersection.top < intersection.right - intersection.left && LevelMap->GetCollisionRect(Index).bottom > pBase->GetRect ().bottom)
				{
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), pBase);
				}*/

				return true;
			}
		}
	}

	return false;
}

bool CLevel::CheckCameraCollision (CCamera* camera)
{
	RECT intersection;

	int StartIndex = ((camera->GetRect().top / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth ()) + (camera->GetRect().left / LevelMap->GetPixelWidth());
	int EndIndex = ((camera->GetRect().bottom / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth()) + (camera->GetRect().right / LevelMap->GetPixelWidth());

	if (StartIndex < 0)
		StartIndex = 0;

	CTile* collisionList = LevelMap->GetCollisionList();

	for (int Index = StartIndex; Index < EndIndex; Index++)
	{
		//if (collisionList[Index].GetName ()) look for CAMERACOLLISION
		{
			if(IntersectRect(&intersection, &LevelMap->GetCollisionRect(Index), &camera->GetRect()))
			{
				((CPlayer*)camera->GetOwner ())->SetCollisionRect (LevelMap->GetCollisionRect(Index));
				m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), camera->GetOwner ());

				/*RECT collisionrect = LevelMap->GetCollisionRect(Index);
				RECT camerarect = camera->GetRect ();

				if (intersection.right - intersection.left <= intersection.bottom - intersection.top && LevelMap->GetCollisionRect(Index).left < camera->GetRect ().left)
				{
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), camera);
				}else if (intersection.right - intersection.left <= intersection.bottom - intersection.top && LevelMap->GetCollisionRect(Index).right > camera->GetRect ().right)
				{
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), camera);
				}else if (intersection.bottom - intersection.top <= intersection.right - intersection.left && LevelMap->GetCollisionRect(Index).top < camera->GetRect ().top)
				{
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), camera);
				}else if (intersection.bottom - intersection.top <= intersection.right - intersection.left && LevelMap->GetCollisionRect(Index).bottom > camera->GetRect ().bottom)
				{
					m_pES->SendEvent (LevelMap->GetCollisionList()[Index].GetName (), camera);
				}*/

				return true;
			}
		}
	}

	return false;
}

void CLevel::SetSpawn (CBase* pBase)
{
	CTile* spawnList = LevelMap->GetSpawnList ();

	for (int i = 0; i <  LevelMap->GetTotalIndexSize(); i++)
	{
		if (!spawnList[i].InUse () && spawnList[i].GetType () != -1) // && type;
		{
			pBase->SetPosX ((float)((spawnList[i].GetIndex () % LevelMap->GetMapWidth()) * LevelMap->GetPixelWidth()));
			pBase->SetPosY ((float)((spawnList[i].GetIndex () / LevelMap->GetMapHeight()) * LevelMap->GetPixelHeight()));

			spawnList->SetInUse (true);

			return;
		}
	}
}

bool CLevel::CheckEnemyCollision (CBase* pBase)
{
	int StartIndex = (int)(((((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth ()) + (((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelWidth()));
	int EndIndex = (int)(((((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelHeight()) * LevelMap->GetMapWidth()) + (((CEnemy*)pBase)->GetViewRadius () / LevelMap->GetPixelWidth()));

	if (StartIndex < 0)
		StartIndex = 0;

	for (int i = StartIndex; i <  EndIndex; i++)
	{
		if (pBase->GetType () == OBJECT_ENEMY)
		{
			//check if collision rect is in enemies radius;
			//if(IntersectRect(&intersection, &LevelMap->GetCollisionRect(Index), &pBase->GetRect()))
			{
					((CEnemy*)pBase)->AddCollisionRect (LevelMap->GetCollisionRect(i));
					//m_pES->SendEvent (LevelMap->GetCollisionList()[i].GetName (), pBase);
			}
		}
	}

	if (((CEnemy*)pBase)->GetCollisionRects ().size () > 0)
	{
		return true;
	}
	
	return false;
}