namespace RPG {
	//! Possible values used for the battle row in RPG::Battler
	enum Row {
		ROW_FRONT, //!< Front row
		ROW_BACK //!< Back row
	};
	
	//! \cond
	enum {
		DPT_START = 0x83
	};
	//! \endcond
	
	//! \brief Possible values for RPG::Battler::actionStatus
	enum ActionStatus {
		AS_IDLE, //!< No action is active
		AS_WAITING, //!< Info window is displayed or action is otherwise waiting
		AS_FINAL_PHASE //!< Final phase, damage is displayed, etc.
	};
	
	/*! \brief Used for entities participating in battle, i.e. actors and
		monsters
		
		Even though this is the base class of both RPG::Actor and RPG::Monster,
		some members only apply to one of them.
		\sa RPG::Row
		\sa RPG::ActionStatus
		\sa RPG::Actor
		\sa RPG::Monster
	*/
	class Battler { // TLcfgUnitItem
		public:
			void **vTable;
			/*! \brief One-based ID of the battler
			
				For actors this value is the database ID, for monsters it is the party member ID <b>plus one</b> (e.g. <tt>RPG::monsters[3]->id</tt> should always be \c 4).
				
				If you want to get the database ID of a monster, you have to use RPG::Monster::databaseId.
			*/
			int id;
			int _unknown_8;
			bool notHidden; //!< \c false if the battler is currently hidden (but not dead)
			Action *action; //!< Current action of the battler
			int hp; //!< Current HP
			int mp; //!< Current MP
			int _unknown_1C;
			int _unknown_20;
			int _unknown_24;
			int _unknown_28;
			DArray<short> _unknown_2C;
			/*! \brief Turns elapsed in a certain condition (see details)
				
				This is an array which has as much possible members as there
				are conditions in the database. The values tell you how many
				turns the battler has taken while being affected by a
				certain conditions. If a value is zero, the particular
				condition does not affect this battler.
				
				For example, if <tt>conditions[2]</tt> were 4, this battler
				would have been infected by poison (by default, condition \#2
				is poison) for 4 turns. If it were zero, the battler would not
				be poisoned at the moment.
				
				\warning If you remove all conditions from a hero in battle,
				make sure that the \c animationId is not \c 9, otherwise the
				RPG Maker will try to show the animation for condition \#0
				which doesn't exist. This would result in an error stating
				that an "Event script referenced a condition that does not
				exist". Use a failsafe like this (\c 0 stands for the idle
				animation):
				\code
if(battler->animationId == 9) battler->animationId = 0;
				\endcode
			*/
			DArray<short, 1> conditions;
			int _unknown_3C;
			int _unknown_40;
			/*! \brief Current value of the ATB bar (\c 0 to \c 300000 - see details)
				
				Internally, the ATB bar can have a value between \c 0 and
				\c 300000 where \c 0 means empty and \c 300000 means full.
				If you set a battler's ATB value to a value greater than or
				equal to \c 300000, this will trigger his next action (in case
				of a monster) or open the action window (in case of an actor).
			*/
			int atbValue;
			bool mirrored; //!< \c true if the battler's image is mirrored
			int frameCounter; //!< Internally used
			/*! \brief ID of the battle graphic pose (see details)
				
				This member specifies which battle graphic pose is displayed.
				Look into the "Animations 2" tab of the database to find out
				what the different values mean (e.g. \c 6 means "Taking
				damage").
				\note This member has no meaning to monsters.
			*/
			int animationId;
			int x; //!< Current X coordinate (centered)
			int y; //!< Current Y coordinate (centered)
			int originalX; //!< Original X coordinate (used when an actor moves to attack)
			int originalY; //!< Original Y coordinate (used when an actor moves to attack)
			int _unknown_64;
			int _unknown_68;
			int _unknown_6C;
			Row rowAlt; //!< Internally used, I do not understand the difference from \c row yet
			Row row; //!< Current row (no visible effect on monsters)
			int damagePopupTimer; //!< Frames left until damage popup has finished (zero: no damage popup active)
			int _unknown_7C;
			int flashR; //!< Red value for flashing the battler (\c 0 to \c 31)
			int flashG; //!< Green value for flashing the battler (\c 0 to \c 31)
			int flashB; //!< Blue value for flashing the battler (\c 0 to \c 31)
			int _unknown_8C;
			/*! \brief Intensity for flashing the battler (\c 0 to \c 31 -
				will decrease until flashing is done)
				
				\note Values over 31 will cause the palette to go crazy. Of
				course you can also use this as an intentional effect.
			*/
			double flashIntensity; 
			int flashTimer; //!< Frames left until flashing the battler finished (zero: no flashing active)
			int _unknown_9C;
			int _unknown_A0;
			int _unknown_A4;
			int _unknown_A8;
			int _unknown_AC;
			int turnsTaken; //!< Turns taken by this battler
			bool longAction; //!< \c true if the last action takes longer (info window needed) - I am not completely sure about that
			ActionStatus actionStatus; //!< Status of the current action
			int _unknown_BC;
			int _unknown_C0;
			/*! \brief Conditions which will be displayed in the info window (see details)
				
				At target selection, there will be an info window displayed if
				the target is affected by a condition. The info has five
				columns for condition names. This array contains the database
				IDs of the conditions which are displayed in this window.
				The content of this array will be reset on several occasions.
				You can change this array in your \c onDrawBattleStatusWindow
				handler.
			*/
			int displayedConditions[5];
			
			/*! \brief Returns the name of the actor or monster
				\return The name
			*/
			std::string getName();
			
			/*! \brief Returns the maximal HP
				\return Maximal HP value
			*/
			int getMaxHp();
			
			/*! \brief Returns the maximal MP
				\return Maximal MP value
			*/
			int getMaxMp();
			
			/*! \brief Returns the attack value
				\return Attack value
			*/
			int getAttack();
			
			/*! \brief Returns the defense value
				\return Defense value
			*/
			int getDefense();
			
			/*! \brief Returns the intelligence value
				\return Intelligence value
			*/
			int getIntelligence();
			
			/*! \brief Returns the agility value
				\return Agility value
			*/
			int getAgility();
			
			/*! \brief Sets the current row
				\param newRow New row
				\note This function has no visible effect on monsters
			*/
			void setRow(Row newRow);
			
			/*! \brief Shows a damage popup with a number
				
				This function will make a number pop up on a battler.
				\param number The number to show
				\param color Color to use (\c 0 to \c 19)
				\sa damagePopup(std::string)
			*/
			void damagePopup(int number, int color);
			
			/*! \brief Shows a damage popup with a small text
				
				This function will make a small text pop up on a battler.
				\param text Text to show
				\note It is not possible to show string popups with a color
				other than \c 0.
				\sa damagePopup(int, int)
			*/
			void damagePopup(std::string text);
			
			/*! \brief Checks whether the battler is a monster or an actor
				\return \c true if the battler is a monster, \c false if it is
				an actor
			*/
			bool isMonster();
			
			/*! \brief Flashes a battler in a certain color
				\param r Red value (\c 0 to \c 31)
				\param g Green value (\c 0 to \c 31)
				\param b Blue value (\c 0 to \c 31)
				\param intensity Intensity of the flash (\c 0 to \c 33.5)
				\param duration Duration in frames
			*/
			void flash(int r, int g, int b, int intensity, int duration);
			
			/*! \brief Executes the action of a battler (<b>highly experimental!</b>)
				
				This function can be used to execute an action. You need to set
				the content of the \c action object before.
				\param skipPluginHandlers If \c true, no \ref onDoBattlerAction
				and \ref onBattlerActionDone handlers will be called for this
				action.
				\return \c true if the action was successfully executed,
				otherwise \c false (in this case try again next frame)
				\warning This is a <b>highly experimental</b> function! During
				testing, it has repeatedly caused strange errors and crashes
				when executed under the wrong conditions or to the wrong time!
				It's probably not safe to use it for anything else than
				experiments, to be honest.
				\sa RPG::Action
			*/
			bool executeAction(bool skipPluginHandlers = false);
	};
}
