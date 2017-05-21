#include "../include/Core/Game.h"

#include "Core/Player.h"
#include "Network/Server.h"
#include "Core/Derived/Constant.h"
#include "Core/Derived/Creature.h"
#include "Core/Derived/Spell.h"

#include <thread>
#include "boost/thread.hpp"

void Game::Start()
{
	cout << "Game started!" << endl;

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

	for (auto player : Players)
	{
		player->m_Client->Write(player->Hand);
	}

	int playersDone = 0;
	vector<Player*> tempPlayers = Players;

	MulliganLoop(playersDone, tempPlayers);

	// Make sure all players have 5 cards in hand
	for (auto i : Players)
	{
		size_t HandSize = i->Hand.size();
		if (HandSize < 5)
		{
			//i->Draw(5 - HandSize);
		}
	}
}

void Game::MulliganLoop(int done, vector<Player*> players) const
{
	Player* tempPlayer;

	while (true)
	{
		for (Player* player : players)
		{
			if (!player->Mulligan.empty())
			{
				// Store the pointer of the player done mulliganning
				tempPlayer = player;
				done++;
			}
		}

		// Attempt to remove it
		players.erase(remove(players.begin(), players.end(), tempPlayer), players.end());

		// Are we done?
		if (done == Players.size())
		{
			break;
		}
	}
}

void Game::PlayState() const
{
	// Listen for messages from the correct client
	while (true)
	{
		if (!ActivePlayer->m_Client->Listening)
		{
			ActivePlayer->m_Client->StartListening();
		}
	}
}

void Game::StartTurn() const
{
	ActivePlayer->RefillMana();
	WriteToPlayers("It is " + ActivePlayer->Name + "'s turn");
}

void Game::EndTurn()
{
	WriteToPlayers(ActivePlayer->Name + " turn end");
	ChangeActivePlayer();
}

void Game::AttackPlay(std::string attack)
{
	WriteToPlayers(ActivePlayer->Name + " attack: " + attack);
}

void Game::CardPlay(int index) const
{
	Card* card = ActivePlayer->Hand.at(index);

	if (ActivePlayer->IsPlayable(card))
	{
		ActivePlayer->SubtractCost(card);
		ActivePlayer->PlayCard(card);
	}
	
	// The card was played, lets remove it from the active players hand
	ActivePlayer->RemoveFromHand(card);
}

void Game::HandlePlay(string play)
{
	switch (play.at(0)) 
	{
		case CARD_PROTOCOL:
		{
			int index = stoi(GetAfterChar(play, CARD_PROTOCOL));
			CardPlay(index);
			break;
		}

		case ATTACK_PROTOCOL:
		{
			std::string attack = GetAfterChar(play, ATTACK_PROTOCOL);
			AttackPlay(attack);
			break;
		}

		case END_PROTOCOL:
		{
			EndTurn();
			break;
		}

		default: {};
	}

	WriteGameStatus();
}

void Game::ChangeActivePlayer()
{
	//If the active player is the last item in the vector
	if (ActiveIndex == (Players.size() - 1))
	{
		//Set it to the front of the vector
		ActiveIndex = 0;
	}
	else
	{
		ActiveIndex++;
	}

	// Set the active player pointer
	ActivePlayer = Players.at(ActiveIndex);
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

void Game::WriteToPlayers(std::string data) const
{
	for (auto player : Players)
	{
		if (player->m_Client->GetSocket().is_open())
		{
			player->m_Client->Write(data);
		}
	}
}

void Game::WriteGameStatus()
{
	for (auto player : Players)
	{
		for (auto writePlayer : Players)
		{
			if (player == writePlayer)
			{
				player->m_Client->Write(writePlayer->GetStatus(false));
			}
			else
			{
				player->m_Client->Write(writePlayer->GetStatus(true));
			}
		}
	}
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
		// There is one or less players alive
		return true;
	}
	return false;
}

Game::Game(vector<Player*> players, Server* server)
	: PLAYER_HEALTH(30), PLAYER_MANA({1,1,1,1,1,1}), Players(players), ActivePlayer(players.at(0)), ActiveIndex(0), m_Server(server)
{
	cout << "Game created, initializing game settings..." << endl;

	for (auto p : Players)
	{
		cout << p->Name << " has joined the game" << endl;
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

std::string Game::GetAfterChar(std::string data, char splitter)
{
	std::string substring = data.substr(data.find(splitter) + 1);
	return substring;
}

