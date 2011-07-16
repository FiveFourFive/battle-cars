#include "CTutorialMode.h"
#include "CMainMenuState.h"
#include "CCar.h"
#include "CGamePlayState.h"
#include "CPlayer.h"
#include <vector>
#include "CLevel.h"
#include "CMap.h"
#include "CGame.h"
#include "CWinState.h"
#include "CLossState.h"
#include "ParticleManager.h"
#include "Emittor.h"

CTutorialMode::CTutorialMode(void)
{
	m_iCurrMessage=1;
	m_bskipTutorial=false;
	m_bTryStart=false;
	m_bTryMoved=false;
	m_bTryTurn=false;
	m_bTryShoot=false;
	m_bTryToggleWeapon=false;
	m_szMessages[0]="Press ESC to return to main menu";
	m_szMessages[1]="Press Enter to start the game";
	m_szMessages[2]="Press Up to move forward";
	m_szMessages[3]="Use the Left and Right Arrows to turn";
	m_szMessages[4]="Press Space to shoot";
	m_szMessages[5]="Press L-Ctrl to toggle weapons";
	m_szMessages[6]="Congratulations! you have at least\n the IQ of a squirrel\n press enter to exit to main menu";
}

CTutorialMode::~CTutorialMode(void)
{


}
CTutorialMode* CTutorialMode::GetInstance(void)
{
	static CTutorialMode instance;
	return &instance;
}

void CTutorialMode::CheckCarStatus(CCar* car)
{

}

void CTutorialMode::CheckWinLoss(void)
{
	if(m_bskipTutorial)
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
}