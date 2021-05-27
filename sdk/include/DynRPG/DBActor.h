namespace RPG {
	//! Not implemented yet
	typedef void ActorLearning;
	
	/*! \brief Used for the default data of actors which is defined in the
		database
		
		\note The members of this class either mean default values of a
		property or they are used for properties which aren't supposed to be
		changed in-game, thus the members won't be saved and loaded
		automatically (as it is the case with RPG::Actor), but only loaded once
		at startup.\n\n
		If you want to access data which could be changed in-game, use the
		members and methods of RPG::Actor instead.
		\sa RPG::dbActors
		\sa RPG::Actor
		\sa RPG::actors
	*/
	class DBActor {
		public:
			void **vTable;
			int id; //!< ID of the actor
			DStringPtr name; //!< Default name
			DStringPtr degree; //!< Default degree
			DStringPtr charsetFilename; //!< Default charset filename
			int charsetId; //!< Default charset ID
			/*! \brief Default charset transparency (see details)
				
				Unlike RPG::Actor::charsetTransparency, this member is a simple
				boolean value. \c true means a transparency value of \c 3 while
				\c false means a transparency value of \c 0.
			*/
			bool charsetTransparent;
			int minLevel; //!< Minimum level
			int maxLevel; //!< Maximum level
			bool allowCriticalHit; //!< Critical hits allows?
			int criticalHitProbability; //!< Probability of a critical hit
			DStringPtr facesetFilename; //!< Default faceset filename
			int facesetId; //!< Default faceset ID
			bool twoWeapons; //!< Does the actor use a second weapon instead of the shield?
			bool aiControl; //!< Is the actor controlled by the computer in battle?
			bool lockEquipment; //!< Is the equipment locked?
			bool mightyGuard; //!< Does defending reduce the damage more than usual?
#pragma pack(push, 2)
			ArrayBaseOne<short, 99> maxHp; //!< Array of maximum HP values for each level
			ArrayBaseOne<short, 99> maxMp; //!< Array of maximum MP values for each level
			ArrayBaseOne<short, 99> attack; //!< Array of attack values for each level
			ArrayBaseOne<short, 99> defense; //!< Array of defense values for each level
			ArrayBaseOne<short, 99> intelligence; //!< Array of intelligence values for each level
			ArrayBaseOne<short, 99> agility; //!< Array of agility values for each level
#pragma pack(pop)
			int expPrimary; //!< Primary experience curve parameter
			int expSecondary; //!< Secondary experience curve parameter
			int expTertiary; //!< Tertiary experience curve parameter
			short weaponId; //!< Default weapon ID
			short shieldId; //!< Default shield ID
			short armorId; //!< Default armor ID
			short helmetId; //!< Default helmet ID
			short accessoryId; //!< Default accessory ID
			int unarmedBattleAnimationId; //!< Battle animation ID for attacks without weapon
			ActorLearning *skillLearning; //!< Skill learning data (not yet implemented)
			int _unknown_4FC;
			int _unknown_500;
			int _unknown_504;
			int _unknown_508;
			int _unknown_50C;
			int _unknown_510;
			int classId; //!< Default class ID
			int battleGraphicId; //!< Battle graphic ID
			int _unknown_51C;
			int battleCommands[7]; //!< Default battle commands (\c 0 and \c -1 both mean "no command at this position")
	};
	
	/*! \ingroup game_objects
		\brief Array of actors from the database, used for default values and
		properties which are not supposed to be changed in-game.
		
		Use the database ID as index for accessing an actor.
		
		Example:
		\code
int zackUnarmedAnimation = RPG::dbActors[1]->unarmedBattleAnimationId;
		\endcode
		\sa RPG::actors
	*/
	static RPG::NamedCatalogPtr<RPG::DBActor *> &dbActors = (**reinterpret_cast<RPG::NamedCatalogPtr<RPG::DBActor *> **>(0x4CDD54));
}
