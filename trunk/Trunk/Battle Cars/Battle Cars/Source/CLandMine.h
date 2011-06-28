#ifndef CLANDMINE_H_
#define CLANDMINE_H_

#include "CBullet.h"

class CLandMine : public CBullet
{
private:
	float m_fDuration;

public:
	CLandMine();
	~CLandMine();
	void Update(float fElapsedTime);
	void Render(CCamera* camera);

	//Accessors
	float GetDuration() {return m_fDuration;}
	
	//Mutators
	void SetDuration(float dur) {m_fDuration = dur;}
};
#endif