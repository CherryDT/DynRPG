namespace RPG {
	/*! \brief Used for the data of actors which can be changed in-game.

		\note Many things are only stored in this class if they were changed
		and are now different from the default in the database, thus you
		should use the getter methods to access these things.\n\n
		The members ending with \c Diff are "offsets" which are added to the
		default value (which is calculated from the current level, class and
		equipment).
		\sa RPG::actors
		\sa RPG::DBActor
		\sa RPG::dbActors
		\sa RPG::Battler
	*/
	class Actor : public Battler { // TLcfgPlayerItem
		public:
			// sizeof(Battler) == 0xD8
			DStringPtr name; //!< Name ("\x01" means database default)
			DStringPtr degree; //!< Degree ("\x01" means database default)
			DStringPtr charsetFilename; //!< Filename of the charset (empty means database default)
			int charsetId; //!< ID of the sprite in the charset file (only valid if \c charsetFilename isn't empty)
			int charsetTransparency; //!< Transparency of the charset (\c 0 to \c 8, only valid if \c charsetFilename isn't empty)
			DStringPtr facesetFilename; //!< Filename of the faceset (empty means database default)
			int facesetId; //!< ID of the sprite in the faceset file (only valid if \c facesetFilename isn't empty)
			int level; //!< Current level
			int exp; //!< Current experience points
			int maxHpDiff; //!< Difference from the default maximal HP
			int maxMpDiff; //!< Difference from the default maximal MP
			int attackDiff; //!< Difference from the default attack value
			int defenseDiff; //!< Difference from the default defense value
			int intelligenceDiff; //!< Difference from the default intelligence value
			int agilityDiff; //!< Difference from the default agility value
			int _unknown_114;
			int _unknown_118;
			short weaponId; //!< Database ID of the current weapon (zero for none)
			short shieldId; //!< Database ID of the current shield (or second-hand weapon, zero for none)
			short armorId; //!< Database ID of the current armor (zero for none)
			short helmetId; //!< Database ID of the current helmet (zero for none)
			short accessoryId; //!< Database ID of the current accessory (zero for none)
			bool twoWeapons; //!< Can the actor hold two weapons?
			bool lockEquipemnt; //!< Is the actor's equipment unchangable by the player?
			bool autoBattle; //!< Is the battle AI activated for this actor?
			bool mightyGuard; //!< Is the "mighty guard" mode activated for this actor?
			int battleGraphicId; //!< Database ID of the battle graphic (zero for database default)
			bool customBattleCommands; //!< \c true if the \c battleCommands member is valid, otherwise the database default should be used
			int battleCommands[7]; //!< The current battle commands for this actor (\c 0 and \c -1 both mean "no command at this position")

			/*! \brief Returns the current degree
				\return Current degree
			*/
			std::string getDegree();

			/*! \brief Returns the current charset filename
				\return Current charset filename
				\sa getCharsetId
				\sa getCharsetTransparency
			*/
			std::string getCharsetFilename();

			/*! \brief Returns the current charset ID
				\return ID of sprite in charsets
				\sa getCharsetFilename
				\sa getCharsetTransparency
			*/
			int getCharsetId();

			/*! \brief Returns the current charset transparency
				\return Transparency value (see RPG::Character::transparency
				documentation)
				\sa getCharsetFilename
				\sa getCharsetId
			*/
			int getCharsetTransparency();

			/*! \brief Returns the current faceset filename
				\return Current faceset filename
				\sa getFacesetId
			*/
			std::string getFacesetFilename();

			/*! \brief Returns the current faceset ID
				\return ID of sprite in faceset
				\sa getFacesetFilename
			*/
			int getFacesetId();

			/*! \brief Returns the current battle graphic ID
				\return Database ID of the current battle graphic
			*/
			int getBattleGraphicId();

			/*! \brief Returns the current battle command on a certain position
				\param index Zero-based index of the available commands for
				this actor (\c 0 to \c 6)
				\return Database ID of the battle command on the given position
				or either \c 0 or \c -1 if there is none
			*/
			int getBattleCommand(int index);

			/*! \brief Returns a certain party member
				\param index Zero-based party member index (\c 0 to \c 3)
				\return Pointer to the RPG::Actor object of the particular actor
			*/
			static RPG::Actor *partyMember(int index);
	};

	/*! \ingroup game_objects
		\brief Array of actors from the database, used for properties which
		can change in-game.

		Use the database ID as index for accessing an actor.

		Example:
		\code
int zackHp = RPG::actors[1]->hp;
		\endcode
		\sa RPG::dbActors
	*/
	static RPG::NamedCatalogPtr<RPG::Actor *> &actors = (**reinterpret_cast<RPG::NamedCatalogPtr<RPG::Actor *> **>(0x4CDDC8));
}
