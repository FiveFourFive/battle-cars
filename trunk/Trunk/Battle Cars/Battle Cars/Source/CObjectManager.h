////////////////////////////////////////////////
//	File	:	"CObjectManager.h"
//
//	Author	:	Wade Shwallon
//
//	Purpose	:	contains and managse all game objects.
/////////////////////////////////////////////////

#ifndef _COBJECTMANAGER_H
#define _COBJECTMANAGER_H
#include <vector>
using std::vector;

#include "IBaseInterface.h"
class CCamera;
class CObjectManager
{

private:
	vector<IBaseInterface*> m_vObjectList;
	static CObjectManager* m_pInstance;

	CObjectManager(void);
	~CObjectManager(void);
	CObjectManager(const CObjectManager&);
	CObjectManager& operator=(const CObjectManager&);

public:
	static CObjectManager* GetInstance(void);
	void DeleteInstance(void);
	void UpdateObjects(float fElapsedTime);
	void RenderObjects(CCamera* camera);
	void AddObject(IBaseInterface* pObject);
	void RemoveObject(IBaseInterface* pObject);
	void RemoveAllObjects(void);
	void CheckCollisions(void);
	void CheckStatus(void);

};


#endif