#define BOOST_PYTHON_STATIC_LIB
#include "../../../include/Core/Utility/Action.h"
#include "../../../include/Core/Game/Card.h"
#include "../../../include/Core/Game/Player.h"

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


Action::Action()
	: CardTargets(), PlayerTargets(), Attacker(nullptr), Target(nullptr), TargetOwner(nullptr), Owner(nullptr), Type()
{
}

Action::Action(Player* Owner, ActionTypeVariants ActionType)
	: CardTargets(), PlayerTargets(), Attacker(nullptr), Target(nullptr), TargetOwner(nullptr), Owner(Owner), Type(ActionType)
{
}

Action::Action(std::vector<Card*> CardTargets, Player* Owner, ActionTypeVariants ActionType)
	: CardTargets(CardTargets), PlayerTargets(), Attacker(nullptr), Target(nullptr), TargetOwner(nullptr), Owner(Owner), Type(ActionType)
{
}

Action::Action(std::vector<Player*> PlayerTargets, Player* Owner, ActionTypeVariants ActionType)
	: CardTargets(), PlayerTargets(PlayerTargets), Attacker(nullptr), Target(nullptr), TargetOwner(nullptr), Owner(Owner), Type(ActionType)
{
}

Action::Action(Creature* Attacker, Creature* Target, Player* Owner, ActionTypeVariants ActionType)
	: CardTargets(), PlayerTargets(), Attacker(Attacker), Target(Target), TargetOwner(nullptr), Owner(Owner), Type(ActionType)
{
}

Action::Action(Creature* Attacker, Player* TargetOwner, Player* Owner, ActionTypeVariants ActionType)
	: CardTargets(), PlayerTargets(), Attacker(Attacker), Target(nullptr), TargetOwner(TargetOwner), Owner(Owner), Type(ActionType)
{
}

Action::Action(std::vector<Card*> CardTargets, std::vector<Player*> PlayerTargets, Player* Owner, ActionTypeVariants ActionType)
	: CardTargets(CardTargets), PlayerTargets(PlayerTargets), Attacker(nullptr), Target(nullptr), TargetOwner(nullptr), Owner(Owner), Type(ActionType)
{
}

Action::Action(std::vector<Card*> CardTargets, std::vector<Player*> PlayerTargets, Creature* Attacker, Creature* Target, Player* TargetOwner, Player* Owner, ActionTypeVariants ActionType)
	: CardTargets(CardTargets), PlayerTargets(PlayerTargets), Attacker(Attacker), Target(Target), TargetOwner(TargetOwner), Owner(Owner), Type(ActionType)
{
}

Action::~Action()
{
}

BOOST_PYTHON_MODULE(Action)
{
	using namespace boost::python;

	typedef std::vector<Card*> vectorCard;
	typedef std::vector<Player*> vectorPlayer;
	
	class_<vectorCard>("vectorCard")
		.def(vector_indexing_suite<vectorCard>());
	class_<vectorPlayer>("vectorPlayer")
		.def(vector_indexing_suite<vectorPlayer>());

	enum_<Action::ActionTypeVariants>("ActionTypeVariants")
		.value("Attack", Action::ActionTypeVariants::Attack)
		.value("Damage", Action::ActionTypeVariants::Damage)
		.value("Heal", Action::ActionTypeVariants::Heal)
		.value("Buff", Action::ActionTypeVariants::Buff)
		.value("Debuff", Action::ActionTypeVariants::Debuff)
		.value("Draw", Action::ActionTypeVariants::Draw)
		.value("Discard", Action::ActionTypeVariants::Discard)
		.value("Summon", Action::ActionTypeVariants::Summon)
		.value("Destroy", Action::ActionTypeVariants::Destroy)
		.value("TurnStart", Action::ActionTypeVariants::TurnStart)
		.value("TurnEnd", Action::ActionTypeVariants::TurnEnd);

	class_<Action>("Action", init<vectorCard, vectorPlayer, Creature*, Creature*, Player*, Player*, Action::ActionTypeVariants>());
}