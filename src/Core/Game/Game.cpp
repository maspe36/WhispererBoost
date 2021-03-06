#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Game/Game.h"
#include "../../../include/Core/Game/Player.h"
#include "../../../include/Core/Game/Card.h"
#include "../../../include/Core/Derived/Creature.h"
#include "../../../include/Core/Derived/Constant.h"
#include "../../../include/Core/Derived/Spell.h"
#include "../../../include/Core/Utility/Action.h"
#include "../../../include/Core/Utility/Factory.h"
#include "../../../include/Network/Server.h"

#include <thread>
#include "boost/thread.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


bool isCharPresent(std::string &data, char c)
{
	std::size_t found = data.find(c);
	if (found != std::string::npos)
	{
		return true;
	}
	return false;
}

/* Internal method, if a card is dead,
handle the result (adding to the graveyard, checking effects, etc).
Used for the remove_if's in ClearDeadCards() */
bool IsDeadHandler(Card* card)
{
	if (card->Alive)
	{
		return false;
	}
	// Put the card in the graveyard
	card->Owner->Graveyard.push_back(card);
	return true;
}

/* Internal wrapper method, just check if a given card is dead.
Used for the remove_if's in ClearDeadCards() */
bool IsDead(Card* card)
{
	if (card->Alive)
	{
		return false;
	}

	return true;
}

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

void Game::PlayState()
{
	// Write the status one time when the game starts
	WriteGameStatus();

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
	ActivePlayer->DrawCard();
	WriteToPlayers("It is " + ActivePlayer->Name + "'s turn");
}

void Game::EndTurn()
{
	WriteToPlayers(ActivePlayer->Name + " turn end");
	ChangeActivePlayer();
}

void Game::AttackPlay(std::string attack)
{
	// TODO: Use <p><c><p>?<c>

	WriteToPlayers(ActivePlayer->Name + " attack: " + attack);
}

void Game::CardPlay(int index) const
{
	Card* card = nullptr;

	try
	{
		card = ActivePlayer->Hand.at(index);
	}
	catch (exception e)
	{
		std::cout << ActivePlayer->Name << " attempted to play a card out of range from their hand" << std::endl;
	}

	if (card != nullptr)
	{
		if (ActivePlayer->IsPlayable(card))
		{
			ActivePlayer->SubtractCost(card);
			ActivePlayer->PlayCard(card);
			ActivePlayer->AddMana(card);
			ActivePlayer->RemoveFromHand(card);
		}
	}
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
	for (auto player : Players) 
	{
		// Remove dead constants
		player->Constants.erase(
			remove_if(player->Constants.begin(), player->Constants.end(), IsDeadHandler),
			player->Constants.end());

		// Remove dead creatures
		player->Creatures.erase(
			remove_if(player->Creatures.begin(), player->Creatures.end(), IsDeadHandler),
			player->Creatures.end());

		// Remove dead spells
		player->Spells.erase(
			remove_if(player->Spells.begin(), player->Spells.end(), IsDeadHandler),
			player->Spells.end());
	}

	// Remove any cards from the card order
	CardOrder.erase(
		remove_if(CardOrder.begin(), CardOrder.end(), IsDead),
		CardOrder.end());

	// Now do a check to see if we need to recursively call this method
	// Things like downfall effects can trigger this case
	for (auto player : Players) 
	{	
		// Check for dead creatures
		for (auto creature : player->Creatures)
		{
			if (!creature->Alive) 
			{
				ClearDeadCards();
			}
		}

		// Check for dead constants
		for (auto constant : player->Constants)
		{
			if (!constant->Alive)
			{
				ClearDeadCards();
			}
		}

		// All spells should be resolved at this point, so don't check for them.
	}
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
				// This player should get sent sensitve information.
				// For example hand contents instead of count.
				player->m_Client->Write(writePlayer->GetStatus(false));
			}
			else
			{
				// This player should not be sent sensitive information.
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

Game::Game(vector<Player*> players, Server* server, Factory* factory)
	: PLAYER_HEALTH(30), PLAYER_MANA({1,1,1,1,1,1}), Players(players), ActivePlayer(players.at(0)), ActiveIndex(0), m_Server(server), m_Factory(factory)
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
	if (isCharPresent(data, splitter)) 
	{
		std::string substring = data.substr(data.find(splitter) + 1);
		return substring;
	}
	else
	{
		return "";
	}
}

BOOST_PYTHON_MODULE(Game)
{
	using namespace boost::python;
	
	typedef vector<Player*> vetorPlayer;
	typedef vector<Card*> vectorCard;
	typedef vector<Action*> vectorAction;
	
	class_<vetorPlayer>("vetorPlayer")
		.def(vector_indexing_suite<vetorPlayer>());
	class_<vectorCard>("vectorCard")
		.def(vector_indexing_suite<vectorCard>());
	class_<vectorAction>("vectorAction")
		.def(vector_indexing_suite<vectorAction>());
	
	class_<Game>("Game", no_init)
		.def_readwrite("Players", &Game::Players)
		.def_readwrite("CardOrder", &Game::CardOrder)
		.def_readwrite("History", &Game::History)
		.def_readwrite("ActivePlayer", &Game::ActivePlayer)
		.def_readwrite("ActiveIndex", &Game::ActiveIndex);

}
