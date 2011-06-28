#ifndef CLANDMINE_H_
#define CLANDMINE_H_

#include "CBullet.h"
#include "CCamera.h"

enum landmines{LM_EXPLOSION, LM_LM};
class CLandMine : public CBullet
{
private:
	float m_fDuration;
	int m_nLandMineType;
public:
	CLandMine();
	~CLandMine();
	void Update(float fElapsedTime);
	void Render(CCamera* camera);
	bool CheckCollision(IBaseInterface* pBase);

	//Accessors
	float GetDuration() {return m_fDuration;}
	int GetLandMineType() {return m_nLandMineType;}
	//Mutators
	void SetDuration(float dur) {m_fDuration = dur;}
	void SetLandMineType(int type) {m_nLandMineType = type;}
};
#endif