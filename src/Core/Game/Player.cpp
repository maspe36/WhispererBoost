#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Game/Player.h"
#include "../../../include/Core/Game/Game.h"
#include "../../../include/Core/Game/Card.h"
#include "../../../include/Core/Derived/Constant.h"
#include "../../../include/Core/Derived/Creature.h"
#include "../../../include/Core/Derived/Spell.h"
#include "../../../include/Network/Client.h"

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>

#include <algorithm>
#include <random>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

void Player::DrawCard()
{
	Card* temp = Deck.front();
	Deck.erase(Deck.begin());
	Hand.push_back(temp);
}

void Player::Draw(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		DrawCard();
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

void Player::AddMana(Card* card)
{
	TotalMana[static_cast<int>(card->Color)] += 1;
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

bool Player::IsDead() const
{
	if (Health <= 0)
	{
		return true;
	}
	
	if (!Alive)
	{
		return true;
	}

	return false;
}

std::string Player::GetStatus(bool sensitive)
{
	std::string status = "";

	status.append(Name + ":\n");
	status.append("Health: " + to_string(Health) + "\n");
	status.append("Deck Size: " + to_string(Deck.size()) + "\n");

	status.append("Available Mana:\n");
	status.append("Black: " + to_string(Mana[static_cast<int>(Card::ColorVariants::Black)]) + "\n");
	status.append("Blue: " + to_string(Mana[static_cast<int>(Card::ColorVariants::Blue)]) + "\n");
	status.append("Brown: " + to_string(Mana[static_cast<int>(Card::ColorVariants::Brown)]) + "\n");
	status.append("Green: " + to_string(Mana[static_cast<int>(Card::ColorVariants::Green)]) + "\n");
	status.append("Red: " + to_string(Mana[static_cast<int>(Card::ColorVariants::Red)]) + "\n");
	status.append("White: " + to_string(Mana[static_cast<int>(Card::ColorVariants::White)]) + "\n");

	status.append("Total Mana:\n");
	status.append("Black: " + to_string(TotalMana[static_cast<int>(Card::ColorVariants::Black)]) + "\n");
	status.append("Blue: " + to_string(TotalMana[static_cast<int>(Card::ColorVariants::Blue)]) + "\n");
	status.append("Brown: " + to_string(TotalMana[static_cast<int>(Card::ColorVariants::Brown)]) + "\n");
	status.append("Green: " + to_string(TotalMana[static_cast<int>(Card::ColorVariants::Green)]) + "\n");
	status.append("Red: " + to_string(TotalMana[static_cast<int>(Card::ColorVariants::Red)]) + "\n");
	status.append("White: " + to_string(TotalMana[static_cast<int>(Card::ColorVariants::White)]) + "\n");

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

	status.append("-----------------------------------\n");

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

BOOST_PYTHON_MODULE(Player)
{
	using namespace boost::python;

	typedef vector<int> vectorInt;
	typedef vector<Card*> vectorCard;
	typedef vector<Creature*> vectorCreature;
	typedef vector<Constant*> vectorConstant;
	typedef vector<Spell*> vectorSpell;
	
	class_<vectorInt>("vectorInt")
		.def(vector_indexing_suite<vectorInt>());
	class_<vectorCard>("vectorCard")
		.def(vector_indexing_suite<vectorCard>());
	class_<vectorCreature>("vectorCreature")
		.def(vector_indexing_suite<vectorCreature>());
	class_<vectorConstant>("vectorConstant")
		.def(vector_indexing_suite<vectorConstant>());
	class_<vectorSpell>("vectorSpell")
		.def(vector_indexing_suite<vectorSpell>());

	class_<Player>("Player", no_init)
		.def_readwrite("Name", &Player::Name)
		.def_readwrite("Health", &Player::Health)
		.def_readwrite("Mana", &Player::Mana)
		.def_readwrite("Alive", &Player::Alive)
		.def_readwrite("Hand", &Player::Hand)
		.def_readwrite("Mulligan", &Player::Mulligan)
		.def_readwrite("Deck", &Player::Deck)
		.def_readwrite("Graveyard", &Player::Graveyard)
		.def_readwrite("Constants", &Player::Constants)
		.def_readwrite("Creatures", &Player::Creatures)
		.def_readwrite("Spells", &Player::Spells)
		.def_readwrite("CurrentGame", &Player::CurrentGame)
		.def_readwrite("m_Client", &Player::m_Client)
		.def_readwrite("Deck", &Player::Deck)
		.def("DrawCard", &Player::DrawCard)
		.def("Draw", &Player::Draw)
		.def("PlayCard", &Player::PlayCard)
		.def("RemoveFromHand", &Player::RemoveFromHand)
		.def("IsDead", &Player::IsDead);
}