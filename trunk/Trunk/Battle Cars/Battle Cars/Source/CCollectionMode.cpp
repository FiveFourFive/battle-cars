#include "CCollectionMode.h"
#include "CCar.h"
#include "CGamePlayState.h"
#include "CPlayer.h"
#include "CLevel.h"
#include "CMap.h"
#include "CGame.h"
#include "CWinState.h"
#include "CLossState.h"
#include "ParticleManager.h"
#include "Emittor.h"
#include "CCollectable.h"

CCollectionMode::CCollectionMode(void)
{

}

CCollectionMode::~CCollectionMode(void)
{


}
CCollectionMode* CCollectionMode::GetInstance(void)
{
	static CCollectionMode instance;
	return &instance;
}

void CCollectionMode::CheckCarStatus(CCar* car)
{

}

void CCollectionMode::CheckWinLoss(void)
{
	bool tempFlag = false, gameOver = false;
	if(CGamePlayState::GetInstance()->GetComputerCollectables() + CGamePlayState::GetInstance()->GetPlayerCollectables() == 50)
	{
		if(CGamePlayState::GetInstance()->GetPlayerCollectables() > CGamePlayState::GetInstance()->GetComputerCollectables())
		{
			CWinState::GetInstance()->SetWinner(CGamePlayState::GetInstance()->GetPlayer1());
			CGame::GetInstance()->ChangeState(CWinState::GetInstance());
		}
		else
		{
			
			CGame::GetInstance()->ChangeState(CLossState::GetInstance());
		}
	}

}