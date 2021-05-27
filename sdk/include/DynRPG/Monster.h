namespace RPG {
	/*! \brief Used for monsters as subtype of battlers
		\sa RPG::monsters
		\sa RPG::Battler
		\sa RPG::Actor
	*/
	class Monster : public Battler { // TLcfgMonsterItem
		public:
			// sizeof(Battler) == 0xD8
			int databaseId; //!< The database ID of the monster
			Image *image; //!< The image of the monster's graphic
			Image *imageMirrored; //!< The <b>mirrored</b> image of the monster's graphic (used when \ref RPG::Battler::mirrored is \c true)
			/*! \brief Frames left until damage blinking is done
				
				If a monster takes damage, it blinks a few time. The blinking
				is started by setting \c blinkTimer to the number of frames the
				blinking should take. The \c blinkTimer is then increased by
				one each frame (while the image will be shown and hidden
				repeatedly), until it reaches zero at which point the blinking
				will be stopped.
			*/
			int blinkTimer;
	};
	
	/*! \ingroup game_objects
		\brief Array of monsters in the current monster group.
		
		Use the zero-based party member ID as index for accessing a monster.
		
		Example:
		\code
int slimeHp = RPG::monsters[2]->hp; // read HP of third monster
		\endcode
		\sa RPG::Battler
		\sa RPG::actors
		\sa RPG::Actor::partyMember
	*/
	static RPG::CatalogPtr<RPG::Monster *> &monsters = (**reinterpret_cast<RPG::CatalogPtr<RPG::Monster *> **>(0x4CDE64));
}
