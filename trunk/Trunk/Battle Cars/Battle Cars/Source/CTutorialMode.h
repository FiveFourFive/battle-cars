#ifndef _CTUTORIALMODE_H
#define _CTUTORIALMODE_H

#include "IGameModeInterface.h"
#include <string>

using std::string;
class CCar;



class CTutorialMode : public IGameModeInterface
{
private:
	bool m_bskipTutorial;
	bool m_bTryStart;
	bool m_bTryMoved;
	bool m_bTryTurn;
	bool m_bTryShoot;
	bool m_bTryToggleWeapon;

	int m_iCurrMessage;

	string m_szMessages[7];

	CTutorialMode(void);
	~CTutorialMode(void);
	CTutorialMode& operator=(const CTutorialMode&);
	CTutorialMode(const CTutorialMode&);
public:
	static CTutorialMode* GetInstance(void);
	void CheckCarStatus(CCar* car);

	void CheckWinLoss(void);
	void ToggleSkipTutorial(){m_bskipTutorial=!m_bskipTutorial;};
	void SetTryMoved(bool TryMoved){m_bTryMoved=TryMoved;}
	void SetTryTurn(bool TryTurn){m_bTryTurn=TryTurn;}
	void SetTryShoot(bool TryShoot){m_bTryShoot=TryShoot;}
	void SetTryStart(bool TryStart){m_bTryStart=TryStart;}
	void SetTryToggleWeapon(bool TryToggleWeapon){m_bTryToggleWeapon=TryToggleWeapon;}
	void SetCurrMessage(int message){m_iCurrMessage=message;}
	int GetCurrMessage(){return m_iCurrMessage;}
	string GetMessage(int i){return m_szMessages[i];}
	bool GetTryMoved(){return m_bTryMoved;}
	bool GetTryTurn(){return m_bTryTurn;}
	bool GetTryShoot(){return m_bTryShoot;}
	bool GetTryStart(){return m_bTryStart;}
	bool GetTryToggleWeapon(){return m_bTryToggleWeapon;}
	//void (*HandleCar) (CCar* car);

};



#endif