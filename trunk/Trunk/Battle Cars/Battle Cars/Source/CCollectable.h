#ifndef CCOLLECTABLE_H_
#define CCOLLECTABLE_H_

#include "CBase.h"

class CCollectable : public CBase
{
private:
	bool m_bActive;
	int m_nSoundID;

public:
	CCollectable();
	~CCollectable();

	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	bool CheckCollision(IBaseInterface* pBase);

	RECT GetRect();
	bool IsActive() {return m_bActive;}

};

#endif