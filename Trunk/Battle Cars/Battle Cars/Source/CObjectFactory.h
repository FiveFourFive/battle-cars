/////////////////////////////////////////////////
//	File	:	"CObjectFactory.h"
//
//	Author	:	Wade Shwallon
//
//	Purpose	:	contains function pointers for the 
//				creation of game objects.
//				
/////////////////////////////////////////////////

#ifndef _COBJECTFACTORY_H
#define _COBJECTFACTORY_H

#include <map>
using std::map;

template <typename ClassIDType, typename BaseClassType>
class CObjectFactory
{

private:

	typedef BaseClassType*(*ObjectCreator)(void);

	map<ClassIDType, ObjectCreator> m_ObjectCreator;

	static CObjectFactory<ClassIDType, BaseClassType> m_pInstance;

	// Private for singletons
	CObjectFactory(void) {}
	CObjectFactory(const CObjectFactory&);
	CObjectFactory& operator=(const CObjectFactory&);
	~CObjectFactory(void) {}

	template<typename DerivedClassType>
	static BaseClassType* ConstructObject(void)
	{
		DerivedClassType toReturn = new DerivedClassType;
		return toReturn;
	}

public:

	static CObjectFactory<ClassIDType, BaseClassType>* GetInstance(void)
	{
		return &m_pInstance;
	}

	template<typename DerivedClassType>
	bool RegisterClassType(ClassIDType id)
	{
		std::pair<ClassIDType, ObjectCreator> objectDef;

		objectDef.first = id;
		objectDef.second = ConstructObject<DerivedClassType>;

		m_ObjectCreator.insert(objectDef);

		return true;
	}

	bool UnregisterClassType(ClassIDType id)
	{
		map<ClassIDType, ObjectCreator>::iterator iter = m_ObjectCreator.find(id);

		if(iter == m_ObjectCreator.end())
			return false;

		m_ObjectCreator.erase(iter);

		return true;
	}

	void ShutDownFactory(void)
	{
		m_ObjectCreator.clear();
	}


};

template<typename ClassIDType, typename BaseClassType>
CObjectFactory<ClassIDType, BaseClassType>
CObjectFactory<ClassIDType, BaseClassType>::m_pInstance;



#endif