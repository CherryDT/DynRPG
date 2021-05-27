namespace RPG {
	//! Possible values for the RPG::Action::kind member
	enum ActionKind {
		AK_BASIC, //!< Basic action (attack, defense, escape, etc. - see RPG::BasicAction)
		AK_SKILL, //!< Use a skill
		AK_TRANSFORM, //!< Transform into another monster (do not use for actors)
		AK_ITEM, //!< Use an item (do not use for monsters)
		AK_NONE //!< No action (last action was already executed)
	};

	//! One-byte version of RPG::ActionKind
	typedef unsigned char ActionKind_T;

	//! Possible values for the RPG::Action::basicActionId member
	enum BasicAction {
		BA_ATTACK, //!< Attack
		BA_DOUBLE_ATTACK, //!< Double attack
		BA_DEFEND, //!< Defend
		BA_OBSERVE, //!< Observe battle
		BA_CHARGE, //!< Charge up
		BA_SELF_DESTRUCT, //!< Self-destruct
		BA_ESCAPE, //!< Escape (will always succeed)
		BA_NONE, //!< No action
		BA_CHANGE_ROW //!< Change row (has no visible effects if used by monsters)
	};

	//! One-byte version of RPG::BasicAction
	typedef unsigned char BasicAction_T;

	//! Possible values for the RPG::Action::target member
	enum Target {
		TARGET_NONE, //!< No target (the sounds of the battle animation will still be audible)
		TARGET_ACTOR, //!< Target is an actor, specified in the RPG::Action::targetId member
		TARGET_ALL_ACTORS, //!< Target are all actors at once
		TARGET_MONSTER, //!< Target is a monster, specified in the RPG::Action::targetId member
		TARGET_ALL_MONSTERS //!< Target are all mosnters at once
	};

	//! One-byte version of RPG::Target
	typedef unsigned char Target_T;

	/*! \brief %Action of a battler (normal attack, skill, escape, etc.), including target

		This class is used to describe the action an actor or monster does in battle.
		\note The action of a battler is always tied to the battler itself.
		\sa RPG::Battler
		\sa RPG::ActionKind
		\sa RPG::BasicAction
		\sa RPG::Target
	*/
	class Action { // TLcfgUnitActItem
		public:
			void **vTable;
			ActionKind_T kind; //!< Kind of the action (see RPG::ActionKind)
			BasicAction_T basicActionId; //!< If \c kind is RPG::AK_BASIC: Kind of the basic action (see RPG::BasicAction)
			char _unknown_6; // Was "userData1" earlier
			bool retrying; //!< \c true if the last try to execute the action failed
			int skillId; //!< If \c kind is RPG::AK_SKILL: Database ID of the skill
			int monsterId; //!< If \c kind is RPG::AK_TRANSFORM: Database ID of the monster to transform into
			int itemId; //!< If \c kind is RPG::AK_ITEM: Database ID of the item
			int _unknown_14;
			int _unknown_18;
			int _unknown_1C;
			Target_T target; //!< %Target of the action (see RPG::Target)
			int targetId; //!< If \c target is RPG::TARGET_ACTOR or RPG::TARGET_MONSTER: Zero-based party member index of the target
	};
}
