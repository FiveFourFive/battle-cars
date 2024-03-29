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
CDeathmatchMode* CDeathmatchMode::GetInstance(void)
{
	static CDeathmatchMode instance;
	return &instance;
}

void CDeathmatchMode::CheckCarStatus(CCar* car)
{
	if(car->GetHealth() <= 0)
	{
		ParticleManager* pPM = ParticleManager::GetInstance();
		static bool isSet = false;

		car->SetVelX(0);
		car->SetVelY(0);

		if( car->GetIsAlive() )
		{
			car->SetIsAlive(false);

#pragma region EXPLOSION_EFFECT
			Emittor* smoke_emittor = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_SMOKE_EMITTOR), car->GetPosX(), car->GetPosY());
			if( smoke_emittor )
			{
 				smoke_emittor->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(car, smoke_emittor);
			}

			Emittor* fireburst_emittor = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FIREBURST1_EMITTOR), car->GetPosX() , car->GetPosY());
			if( fireburst_emittor )
			{
				fireburst_emittor->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(car, fireburst_emittor);
			}

			Emittor* fireburst_emittor1 = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FIREBURST2_EMITTOR), car->GetPosX() , car->GetPosY());
			if( fireburst_emittor1 )
			{
				fireburst_emittor1->SetTimeToDie(1.5f);				
				pPM->AttachToBasePosition(car, fireburst_emittor1);
			}
			Emittor* fireburst_emittor2 = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FIREBURST3_EMITTOR), car->GetPosX(), car->GetPosY());
			if( fireburst_emittor2 )
			{
				fireburst_emittor2->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(car, fireburst_emittor2);
			}

			Emittor* fire_emittor = pPM->CreateEffect(pPM->GetEmittor(EXPLOSION_FLAME_EMITTOR), car->GetPosX(), car->GetPosY());
			if( fire_emittor )
			{
				fire_emittor->SetTimeToDie(1.5f);
				pPM->AttachToBasePosition(car, fire_emittor);
			}
#pragma endregion
		}

		if( car->GetRespawnTimer() > 1.0f )
		{
			if( !isSet)
			{
				isSet = true;

#pragma region BURNING_EFFECT
				Emittor* burning_emittor = pPM->CreateEffect(pPM->GetEmittor(AFTEREXPLOSION_SMOKE_EMITTOR), car->GetPosX(), car->GetPosY());
				if( burning_emittor )
				{
					burning_emittor->SetTimeToDie(3.5f);
					pPM->AttachToBasePosition(car, burning_emittor);
				}
				burning_emittor = pPM->CreateEffect(pPM->GetEmittor(AFTEREXPLOSION_FLAME_EMITTOR), car->GetPosX(), car->GetPosY());
				if( burning_emittor )
				{
					burning_emittor->SetTimeToDie(3.5f);
					pPM->AttachToBasePosition(car, burning_emittor);
				}
#pragma endregion
			}
		}

		if( car->GetRespawnTimer() > 5.0f )
		{
			isSet = false;

			car->SetRespawnTimer(0.0f);
			car->SetIsAlive(true);
			/*int width_index = CLevel::GetInstance()->GetMap()->GetMapWidth() * CLevel::GetInstance()->GetMap()->GetPixelWidth();
			int height_index = CLevel::GetInstance()->GetMap()->GetMapHeight() * CLevel::GetInstance()->GetMap()->GetPixelHeight();*/

			CLevel::GetInstance ()->ResetCarSpawn (car);

			car->SetHealth(car->GetMaxHealth());
			car->SetShieldBar(car->GetMaxShield());
			car->SetPowerUpBar(0);
			car->SetSpecialLevel(1);
			car->SetVelX(0);
			car->SetVelY(0);
			car->SetSpeed(0);
			tVector2D vel;
			vel.fX = 0.0f;
			vel.fY = 0.0f;
			car->SetVelocity(vel);
			car->Rotate(0.0f);
		}

	}
}

void CDeathmatchMode::CheckWinLoss(void)
{
	if(CGamePlayState::GetInstance()->GetTimeLeft() <= 0 )
	{
		vector<CCar*>* scores;
		scores = CGamePlayState::GetInstance()->GetList();
		/*if(scores->front()->GetType() == OBJECT_PLAYER)
		{
			CPlayer* tempplayer = (CPlayer*)scores->front();
			CWinState::GetInstance()->SetWinner(tempplayer);
			tempplayer->AddRef();
			if(tempplayer->GetPlayerNum() == 1)
			{
				CGame::GetInstance()->ChangeState(CWinState::GetInstance());
				
			}
			else
			{
				CGame::GetInstance()->ChangeState(CLossState::GetInstance());
				CWinState::GetInstance()->SetWinner(NULL);
			}
		}
		else
		{
			CWinState::GetInstance()->SetWinner(NULL);
			CGame::GetInstance()->ChangeState(CLossState::GetInstance());
		}*/

		if (scores->front ()->GetKillCount () > 0)
		{
			if (scores->front () == CGamePlayState::GetInstance ()->GetPlayer1 ())
			{
				CGamePlayState::GetInstance ()->GetPlayer1 ()->AddRef ();
				CWinState::GetInstance()->SetWinner(CGamePlayState::GetInstance ()->GetPlayer1 ());
				CGame::GetInstance()->ChangeState(CWinState::GetInstance());
			
			}else if (scores->front () == CGamePlayState::GetInstance ()->GetPlayer2 ())
			{
				CGamePlayState::GetInstance ()->GetPlayer2 ()->AddRef ();
				CWinState::GetInstance()->SetWinner(CGamePlayState::GetInstance ()->GetPlayer2 ());
				CGame::GetInstance()->ChangeState(CWinState::GetInstance());

			}else
			{
				CWinState::GetInstance()->SetWinner(NULL);
				CGamePlayState::GetInstance ()->GetPlayer1 ()->AddRef ();
				CGamePlayState::GetInstance ()->GetPlayer2 ()->AddRef ();
				CLossState::GetInstance()->SetLosser1 (CGamePlayState::GetInstance ()->GetPlayer1 ());
				CLossState::GetInstance()->SetLosser2 (CGamePlayState::GetInstance ()->GetPlayer2 ());
				CGame::GetInstance()->ChangeState(CLossState::GetInstance());
			}
		}else
		{
			CWinState::GetInstance()->SetWinner(NULL);
			CGamePlayState::GetInstance ()->GetPlayer1 ()->AddRef ();
			CGamePlayState::GetInstance ()->GetPlayer2 ()->AddRef ();
			CLossState::GetInstance()->SetLosser1 (CGamePlayState::GetInstance ()->GetPlayer1 ());
			CLossState::GetInstance()->SetLosser2 (CGamePlayState::GetInstance ()->GetPlayer2 ());
			CGame::GetInstance()->ChangeState(CLossState::GetInstance());
		}
	}

}