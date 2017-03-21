#include "..\include\Core\Game.h"

#include "Core\Player.h"
#include "Network\Server.h"

void Game::Start()
{
	std::cout << "Game started!" << std::endl;

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
		//i->Draw(5);
	}

	// Let them decide what cards to keep
	int playersDone = 0;
	vector<Player*> tempPlayers = Players;
	Player* tempPlayer;

	std::cout << tempPlayers.size() << std::endl;

	for (auto player : Players)
	{
		player->m_Client->Write("Select the cards you wish to keep...");
		//player->m_Client->Write(print cards in hand)
	}

	while (true) 
	{
		for (Player* player : tempPlayers)
		{
			if (!player->Mulligan.empty())
			{
				// Store the pointer of the player done mulliganning
				tempPlayer = player;
				playersDone++;
			}
		}

		// Attempt to remove it
		tempPlayers.erase(std::remove(tempPlayers.begin(), tempPlayers.end(), tempPlayer), tempPlayers.end());

		// Are we done?
		if (playersDone == Players.size()) 
		{
			break;
		}
	}

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

	for (auto player : Players) 
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

Game::Game(vector<Player*> players, Server* server)
	: Players(players), m_Server(server), PLAYER_HEALTH(30), PLAYER_MANA(1), Active(players.at(0)), ActiveIndex(0)
{
	std::cout << "Game created, initializing game settings..." << std::endl;

	for (auto p : Players)
	{
		std::cout << p->Name << " is here with us today!" << endl;
	}

	// Right now the first player given is the first active player (first to play)
	// Set all the players heal and starting mana according to this games settings
	for (auto player : Players)
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