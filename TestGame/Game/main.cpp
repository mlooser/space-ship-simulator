#include "SpaceShipGame.h"
#include "Logger.h"


int main(void)
{
	MustacheEngine::Logger::GetInstance()->LogLine("Starting game");

	SpaceShip::SpaceShipGame game;	
	game.Run();

	MustacheEngine::Logger::GetInstance()->LogLine("Game ended");
	return 0;
}

