#include "../include/Core/Player.h"

#include "Core/Card.h"
#include "Core/Derived/Constant.h"
#include "Core/Derived/Creature.h"
#include "Core/Derived/Spell.h"
#include "Core/Game.h"

#include "Network/Client.h"

#include <algorithm>
#include <random>

void Player::Draw()
{
	Card* temp = Deck.front();
	Deck.erase(Deck.begin());
	Hand.push_back(temp);
}

void Player::Draw(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		Card* temp = Deck.front();
		Deck.erase(Deck.begin());
		Hand.push_back(temp);
	}
}

void Player::Shuffle()
{
	shuffle(begin(Deck), end(Deck), Seed);
}

void Player::RefillMana()
{
	Mana = TotalMana;
}

void Player::PlayCard(Card* card)
{
	// Let this players game know the order of all cards played
	CurrentGame->CardOrder.push_back(card);

	// Place the card in the correct play area
	if (Constant* ConstantCard = dynamic_cast<Constant*>(card))
	{
		Constants.push_back(ConstantCard);
	}
	else if (Creature* CreatureCard = dynamic_cast<Creature*>(card))
	{
		Creatures.push_back(CreatureCard);
	}
	else if (Spell* SpellCard = dynamic_cast<Spell*>(card))
	{
		Spells.push_back(SpellCard);
	}

	CurrentGame->WriteToPlayers(Name + " played " + card->Name);
}

void Player::RemoveFromHand(Card * card)
{
	Hand.erase(remove(Hand.begin(), Hand.end(), card), Hand.end());
}

void Player::SubtractCost(Card* card)
{
	for (int i = 0; i < card->Cost.size(); i++)
	{
		Mana[i] -= card->Cost[i];
	}
}

bool Player::IsPlayable(Card* card) const
{
	for (int i = 0; i < Mana.size(); i++)
	{
		if (card->Cost[i] > Mana[i])
		{
			return false;
		}
	}
	return true;
}

std::string Player::GetStatus(bool sensitive)
{
	std::string status = "";

	status.append(Name + ":\n");
	status.append("Health: " + to_string(Health) + "\n");
	status.append("Deck Size: " + to_string(Deck.size()) + "\n");

	status.append("Available Mana:\n");
	status.append("Black: " + to_string(Mana[0]) + "\n");
	status.append("Blue: " + to_string(Mana[1]) + "\n");
	status.append("Brown: " + to_string(Mana[2]) + "\n");
	status.append("Green: " + to_string(Mana[3]) + "\n");
	status.append("Red: " + to_string(Mana[4]) + "\n");
	status.append("White: " + to_string(Mana[5]) + "\n");

	status.append("Total Mana:\n");
	status.append("Black: " + to_string(TotalMana[0]) + "\n");
	status.append("Blue: " + to_string(TotalMana[1]) + "\n");
	status.append("Brown: " + to_string(TotalMana[2]) + "\n");
	status.append("Green: " + to_string(TotalMana[3]) + "\n");
	status.append("Red: " + to_string(TotalMana[4]) + "\n");
	status.append("White: " + to_string(TotalMana[5]) + "\n");

	// Only give this players hand size if we are sending this to a different player
	if (sensitive)
	{
		status.append("Hand: " + to_string(Hand.size()) + "\n");
	}
	else
	{
		status.append("Hand:\n");
		for (auto card : Hand)
		{
			status.append(card->Name + "\n");
		}
	}
	
	status.append("Constants:\n");
	for (auto card : Constants)
	{
		status.append(card->Name + "\n");
	}

	status.append("Creatures:\n");
	for (auto card : Creatures)
	{
		status.append(card->Name + "\n");
		status.append(to_string(card->Attack) + " : " + to_string(card->Defense) + "\n");
	}

	status.append("Graveyard:\n");
	for (auto card : Graveyard)
	{
		status.append(card->Name + "\n");
	}

	return status;
}

Player::Player(string name, vector<Card*> deck, Client::pointer client)
	: Name(name), Alive(true), Deck(deck), Seed(random_device{}()), m_Client(client)
{
	// Set the owner to this for all cards in the players deck
	for (Card* card : Deck)
	{
		card->Owner = this;
	}
}

Player::~Player()
{
}
