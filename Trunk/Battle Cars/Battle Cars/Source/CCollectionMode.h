#ifndef CCOLLECTIONMODE_H_
#define CCOLLECTIONMODE_H_

#include "IGameModeInterface.h"
#include <vector>
using namespace std;

class CCar;
class CCollectable;

class CCollectionMode : public IGameModeInterface
{
private:
	vector<CCollectable*> collectables;

	CCollectionMode(void);
	~CCollectionMode(void);
	CCollectionMode& operator=(const CCollectionMode&);
	CCollectionMode(const CCollectionMode&);

public:
	static CCollectionMode* GetInstance(void);
	void CheckCarStatus(CCar* car);

	void CheckWinLoss(void);
	//void (*HandleCar) (CCar* car);
	void SetCollectables(vector<CCollectable*> col) {collectables = col;}
	vector<CCollectable*> GetCollectables() {return collectables;}
};



#endif