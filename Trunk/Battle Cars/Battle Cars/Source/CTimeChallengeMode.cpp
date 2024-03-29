#include "CTimeChallengeMode.h"
#include "CCar.h"
#include "CPlayer.h"
#include "Emittor.h"
#include "ParticleManager.h"
#include "CLevel.h"
#include "CMap.h"
#include "CCharacterSelection.h"
#include "CWinState.h"
#include "CLossState.h"
#include "CGame.h"
#include "CGamePlayState.h"

CTimeChallengeMode::CTimeChallengeMode()
{
	m_bGameOver = false;
}

CTimeChallengeMode::~CTimeChallengeMode()
{

}

CTimeChallengeMode* CTimeChallengeMode::GetInstance()
{
	static CTimeChallengeMode instance;
	return &instance;
}

void CTimeChallengeMode::CheckCarStatus(CCar* car)
{
	if(car->GetHealth() <= 0)
	{
		if( car == (CCar*)CCharacterSelection::GetInstance()->GetPlayer1() || CGamePlayState::GetInstance()->GetTimeLeft() <= 0 || CCharacterSelection::GetInstance()->GetPlayer1()->GetKillCount() >= 10)
		{
			m_bGameOver = true;
			return;
		}

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
			int width_index = CLevel::GetInstance()->GetMap()->GetMapWidth() * CLevel::GetInstance()->GetMap()->GetPixelWidth();
			int height_index = CLevel::GetInstance()->GetMap()->GetMapHeight() * CLevel::GetInstance()->GetMap()->GetPixelHeight();

			CLevel::GetInstance ()->ResetCarSpawn (car);

			car->SetHealth(car->GetMaxHealth());
			car->SetShieldBar(car->GetMaxShield());
			car->SetPowerUpBar(0);
			car->SetSpecialLevel(1);
		}

	}
}

void CTimeChallengeMode::CheckWinLoss()
{
	if(m_bGameOver || (CGamePlayState::GetInstance()->GetTimeLeft() <= 0))
	{
		vector<CCar*>* scores;
		scores = CGamePlayState::GetInstance()->GetList();
		if(scores->front()->GetType() == OBJECT_PLAYER)
		{
			if (scores->front ()->GetKillCount () > 9)
			{
				CGamePlayState::GetInstance ()->GetPlayer1 ()->AddRef ();
				CWinState::GetInstance()->SetWinner(CGamePlayState::GetInstance ()->GetPlayer1 ());
				CGame::GetInstance()->ChangeState(CWinState::GetInstance());
			}else
			{
				CWinState::GetInstance()->SetWinner(NULL);
				CGamePlayState::GetInstance ()->GetPlayer1 ()->AddRef ();
				CLossState::GetInstance()->SetLosser1 (CGamePlayState::GetInstance ()->GetPlayer1 ());
				CLossState::GetInstance()->SetLosser2 (NULL);
				CGame::GetInstance()->ChangeState(CLossState::GetInstance());
			}
			/*CPlayer* tempplayer = (CPlayer*)scores->front();
			if( tempplayer->GetKillCount() >= 10)
				CWinState::GetInstance()->SetWinner(tempplayer);
			else
			{
				tempplayer = (CPlayer*)scores->back() - 1;
				CWinState::GetInstance()->SetWinner(tempplayer);
			}
			if(tempplayer->GetPlayerNum() == 1)
			{
				CGame::GetInstance()->ChangeState(CWinState::GetInstance());
				
			}
			else
			{
				CGame::GetInstance()->ChangeState(CLossState::GetInstance());
			}*/
		}
		else
		{
			CWinState::GetInstance()->SetWinner(NULL);
			CGamePlayState::GetInstance ()->GetPlayer1 ()->AddRef ();
			CLossState::GetInstance()->SetLosser1 (CGamePlayState::GetInstance ()->GetPlayer1 ());
			CLossState::GetInstance()->SetLosser2 (NULL);
			CGame::GetInstance()->ChangeState(CLossState::GetInstance());

		}

		m_bGameOver = false;
	}
}