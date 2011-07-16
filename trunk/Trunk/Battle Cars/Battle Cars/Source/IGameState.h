#ifndef _IGAMESTATE
#define _IGAMESTATE

class IGameState
{
private:
	virtual bool HandleEnter(void) = 0;
public:
	virtual ~IGameState()= 0{};
	virtual void Enter() = 0; // Enters the Game State
	virtual bool Input() = 0; // Input
	virtual void Update(float fElapsedTime) = 0; // Update
	virtual void Render() = 0; // Draw
	virtual void Exit() = 0; // Leaves the Game State
	
};

#endif