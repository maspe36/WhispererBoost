#include "..\include\Core\Game.h"

#include "Core\Player.h"

void Game::Start()
{
	// Inital draw and mulligan
	MulliganState();

	// "Normal" game loop logic
	PlayState();
}

void Game::MulliganState()
{
	// Each player draws 5 cards
	for (auto i : Players)
	{
		i->Draw(5);
	}

	// Let them decide what cards to keep


	// Make sure all players have 5 cards in hand
	for (auto i : Players)
	{
		size_t HandSize = i->Hand.size();
		if (HandSize < 5)
		{
			i->Draw(5 - HandSize);
		}
	}
}

void Game::PlayState()
{
}

void Game::StartTurn()
{
}

void Game::EndTurn()
{
}

void Game::ChangeActivePlayer()
{
	//If the active player is the last item in the vector
	if (ActiveIndex == (Players.size() - 1)) {
		//Set it to the front of the vector
		ActiveIndex = 0;
	}
	else {
		ActiveIndex++;
	}
	
	// Set the active player pointer
	Active = Players.at(ActiveIndex);
}

void Game::ClearDeadCards()
{
}

void Game::ClearDeadPlayers()
{
}

void Game::CheckEffects(Action* action)
{
}

bool Game::IsGameOver()
{
	int aliveCounter = 0;

	for (Player* player : Players) 
	{
		if (player->Alive) 
		{
			aliveCounter++;
		}
	}

	if (aliveCounter <= 1)
	{
		// There is one or less player alive
		return true;
	}
	return false;
}

Game::Game(vector<Player*> players)
	: Players(players), PLAYER_HEALTH(30), PLAYER_MANA(1), Active(players.at(0)), ActiveIndex(0)
{
	// Right now the first player given is the first active player (first to play)
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