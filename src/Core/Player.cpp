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
}

bool Player::IsPlayable(Card* card) const
{
	if (Mana >= card->Cost)
	{
		return true;
	}
	else
	{
		return false;
	}
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
