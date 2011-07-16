/////////////////////////////////////////////////////////
//	File Name	: "CBase.cpp"
//	Author Name : Wade Shwallon
//	Purpose		: Contains base functionality for game objects
/////////////////////////////////////////////////////////

#ifndef _CBASE
#define _CBASE

#include "IBaseInterface.h"
#include <string>
enum type{OBJECT_BASE, OBJECT_BULLET, OBJECT_PLAYER, OBJECT_ENEMY, OBJECT_BOSS, OBJECT_SPEEDRAMP,OBJECT_POWERUP,OBJECT_COLLECTABLE,OBJECT_OBSTACLE, OBJECT_CAMERA};

class CCamera;

class CBase : public IBaseInterface
{
private:
	// reference counter
	unsigned int m_uiRefCount;

	// Dimensions
	int m_nWidth;
	int m_nHeight;

	// Position
	float m_nPosX;
	float m_nPosY;

	// Image
	int m_nImageID;
	int m_nExplosionID;


	float m_nRespawnDelay;
	char* m_nName;
protected:
	// Type
	int m_nType;

public:

	CBase();
	virtual ~CBase();
	int GetExplosionID() { return m_nExplosionID; }
	int GetExplosion() { return m_nExplosionID;} 

	////////////////////////////////////////////////////////
	//	Function:	Accessors
	//
	//	Purpose:	To get the specified type.
	////////////////////////////////////////////////////////
	float GetPosX()	{return m_nPosX;}
	float GetPosY()	{return m_nPosY;}
	int GetImageID(){return m_nImageID;}
	int GetType() { return m_nType; }
	int GetWidth()	{return m_nWidth;}
	int GetHeight() {return m_nHeight;}
	char* GetName();
	float GetRespawnDelay() { return m_nRespawnDelay; }
	////////////////////////////////////////////////////////
	//	Function:	Modifiers
	//
	//	Purpose:	To get the specified type.
	////////////////////////////////////////////////////////
	void SetRespawnDelay(float delay) { m_nRespawnDelay = delay; }
	void SetPosX(float nPosX) {m_nPosX = nPosX;}
	void SetPosY(float nPosY) {m_nPosY = nPosY;}
	void SetImageID(int nImageID) {m_nImageID = nImageID;}
	void SetWidth(int nWidth)	{m_nWidth = nWidth;}
	void SetHeight(int nHeight)	{m_nHeight = nHeight;}
	void SetType(int type) { m_nType = type; }

	void SetName(char* name);
	//Note: to input at top of Update if needed
	virtual void Update(float fElapsedTime);

	virtual void Render(CCamera* camera);

	void AddRef() { m_uiRefCount++; }

	void Release()
	{
		m_uiRefCount--;

		if(!m_uiRefCount)
		{
			delete this;
		}
	}

	virtual RECT GetRect();
	void InBounds(void);
	virtual bool CheckCollision(IBaseInterface* pBase);
};


#endif