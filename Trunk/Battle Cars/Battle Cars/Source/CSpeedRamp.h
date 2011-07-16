#ifndef _CSPEEDRAMP_H
#define _CSPEEDRAMP_H

#include "CBase.h"
#include "SGD_Math.h"

class CSpeedRamp : public CBase
{
private:
	tVector2D m_tVelDir;
	float m_fRotation;
public:
	CSpeedRamp(void);

	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	tVector2D GetVelDir(void) { return m_tVelDir; }
	void RotateVel(float rot) { m_tVelDir = Vector2DRotate(m_tVelDir,rot); m_fRotation = rot; }
	bool CheckCollision(IBaseInterface* pBase);
};



#endif