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
#include "ParticleManager.h"
#include "Emittor.h"

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
		if( car->GetIsAlive() )
		{
			car->SetIsAlive(false);

			ParticleManager* pPM = ParticleManager::GetInstance();


			Emittor* smoke_emittor = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_SMOKE_EMITTOR), car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			if( smoke_emittor )
			{
				smoke_emittor->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(NULL, smoke_emittor,  car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			}

			Emittor* fireburst_emittor = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FIREBURST1_EMITTOR), car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			if( fireburst_emittor )
			{
				fireburst_emittor->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(NULL, fireburst_emittor,  car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			}

			Emittor* fireburst_emittor1 = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FIREBURST2_EMITTOR), car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			if( fireburst_emittor1 )
			{
				fireburst_emittor1->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(NULL, fireburst_emittor1,  car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			}
			Emittor* fireburst_emittor2 = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FIREBURST3_EMITTOR), car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			if( fireburst_emittor2 )
			{
				fireburst_emittor2->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(NULL, fireburst_emittor2,  car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			}

			Emittor* fire_emittor = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FLAME_EMITTOR), car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			if( fire_emittor )
			{
				fire_emittor->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(NULL, fire_emittor,  car->GetPosX() + (car->GetWidth() * 0.5f), car->GetPosY() + (car->GetHeight() * 0.5f));
			}
		}

		if( car->GetRespawnTimer() > 5.0f )
		{
			car->SetRespawnTimer(0.0f);
			car->SetIsAlive(true);
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
		if(scores->back()->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)scores->back();
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