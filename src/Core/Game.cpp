#include "..\include\Core\Game.h"

#include "Core\Player.h"

Game::Game(vector<Player*> players) 
	:Players(players), PLAYER_HEALTH(30), PLAYER_MANA(1)
{
	// Set all the players heal and starting mana according to this games settings
	for (Player* player : Players)
	{
		player->Health = PLAYER_HEALTH;
		player->Mana = PLAYER_MANA;
		player->TotalMana = PLAYER_MANA;
		player->CurrentGame = this;
	}
}

Game::~Game()
{
}
