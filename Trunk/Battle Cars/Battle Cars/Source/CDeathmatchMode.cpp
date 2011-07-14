#include "CDeathmatchMode.h"
#include "CCar.h"
#include "CGamePlayState.h"
#include "CPlayer.h"
#include <vector>
#include "CLevel.h"
#include "CMap.h"
#include "CGame.h"
#include "CWinState.h"
#include "CLossState.h"
CDeathmatchMode::CDeathmatchMode(void)
{

}

CDeathmatchMode::~CDeathmatchMode(void)
{


}

void CDeathmatchMode::CheckCarStatus(CCar* car)
{
	if(car->GetHealth() <= 0)
	{
		if( car->GetRespawnTimer() > 5.0f )
		{
			car->SetRespawnTimer(0.0f);
			int width_index = CLevel::GetInstance()->GetMap()->GetMapWidth() * CLevel::GetInstance()->GetMap()->GetPixelWidth();
			int height_index = CLevel::GetInstance()->GetMap()->GetMapHeight() * CLevel::GetInstance()->GetMap()->GetPixelHeight();

			car->SetPosX(rand() % width_index + 100);
			car->SetPosY(rand() % height_index + 100);
			car->SetHealth(car->GetMaxHealth());
			car->SetShieldBar(car->GetMaxShield());
			car->SetPowerUpBar(0);
			car->SetSpecialLevel(0);
		}

	}
}

void CDeathmatchMode::CheckWinLoss(void)
{
	if(CGamePlayState::GetInstance()->GetTimeLeft() <= 0 )
	{
		vector<CCar*>* scores;
		scores = CGamePlayState::GetInstance()->GetList();
		if(scores->front()->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)scores->front();
			if(tempplayer->GetPlayerNum() == 1)
			{
				CGame::GetInstance()->ChangeState(CWinState::GetInstance());

			}
			else
			{
				CGame::GetInstance()->ChangeState(CLossState::GetInstance());
			}
		}
	}

}