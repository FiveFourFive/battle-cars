#include "CDeathmatchMode.h"
#include "CCar.h"

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
		car->SetPosX(rand() % 5000 + 100);
		car->SetPosY(rand() % 5000 + 100);
		car->SetHealth(car->GetMaxHealth());
		car->SetShieldBar(car->GetMaxShield());
		car->SetPowerUpBar(0);
		car->SetSpecialLevel(0);
	}
}