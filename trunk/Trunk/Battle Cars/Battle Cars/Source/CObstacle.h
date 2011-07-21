#ifndef _COBSTACLE_H
#define _COBSTACLE_H

#include "CBase.h"

#include "SGD_Math.h"
class IBaseInterface;
class CObstacle : public CBase
{
private:
	tVector2D m_vCollisionVel;

	RECT m_rectWall;
	RECT m_rectWallCollision;

public:
	CObstacle(void);
	~CObstacle(void);
	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	bool CheckCollision(IBaseInterface* pBase);

	void SetVel(tVector2D vel) { m_vCollisionVel = vel; }
	tVector2D GetVel(void) { return m_vCollisionVel; }

	void SetWallRect (RECT collision) {m_rectWall = collision;}
	void SetWallCollisionRect (RECT collision) {m_rectWallCollision = collision;}
};



#endif