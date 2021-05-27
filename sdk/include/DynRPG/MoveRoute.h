namespace RPG {
	/*! \brief "Move event" command type
		
		A few of these commands take parameters, see details. For information
		how to use these commands, see the RPG::Character::move documentation.
		\sa RPG::Character::move
		\note <b>Not all enumeration values are documented here</b>, scroll to the top
		to see all possible values (in the general enum overview)!
	*/
	enum MoveType {
		MT_MOVE_UP,
		MT_MOVE_RIGHT,
		MT_MOVE_DOWN,
		MT_MOVE_LEFT,
		MT_MOVE_UP_RIGHT,
		MT_MOVE_DOWN_RIGHT,
		MT_MOVE_DOWN_LEFT,
		MT_MOVE_UP_LEFT,
		MT_MOVE_RANDOMLY,
		MT_MOVE_TOWARD_HERO,
		MT_MOVE_AWAY_FROM_HERO,
		MT_MOVE_FORWARD,
		MT_FACE_UP,
		MT_FACE_RIGHT,
		MT_FACE_DOWN,
		MT_FACE_LEFT,
		MT_TURN_RIGHT,
		MT_TURN_LEFT,
		MT_TURN_AROUND,
		MT_TURN_RANDOMLY,
		MT_FACE_RANDOMLY,
		MT_FACE_TOWARD_HERO,
		MT_FACE_AWAY_FROM_HERO,
		MT_WAIT,
		MT_BEGIN_JUMP,
		MT_END_JUMP,
		MT_LOCK_FACING,
		MT_UNLOCK_FACING,
		MT_INCREASE_SPEED,
		MT_DECREASE_SPEED,
		MT_INCREASE_FREQUENCY,
		MT_DECREASE_FREQUENCY,
		/*! \brief Turns a switch ON
			
			This command takes one integer parameter: The switch ID.
			\sa RPG::MT_SWITCH_OFF
			\sa RPG::Switches
		*/
		MT_SWITCH_ON,
		/*! \brief Turns a switch OFF
			
			This command takes one integer parameter: The switch ID.
			\sa RPG::MT_SWITCH_ON
			\sa RPG::Switches
		*/
		MT_SWITCH_OFF,
		/*! \brief Changes the charset graphic of the character
			
			This command takes two parameters:
				<ul><li>The charset filename (string)</li>
				<li>The charset sprite ID (integer)</li></ul>
		*/
		MT_CHANGE_GRAPHIC,
		/*! \brief Plays a sound effect
			
			This command takes four parameters:
				<ul><li>The sound filename (string)</li>
				<li>The sound volume (integer)</li>
				<li>The sound speed (integer)</li>
				<li>The sound pan value (integer)</li></ul>
			\sa RPG::Sound
		*/
		MT_PLAY_SE,
		MT_PHASING_MODE_ON,
		MT_PHASING_MODE_OFF,
		MT_STOP_ANIMATION,
		MT_RESUME_ANIMATION,
		MT_INCREASE_TRANSPARENCY,
		MT_DECREASE_TRANSPARENCY
	};
	
	//! Not implemented yet
	typedef void MoveRouteItem;
	
	/*! \brief Used for the move route of an character (also set by the "Move Event" command)
		\sa RPG::MoveType
		\sa RPG::Character::move
	*/
	class MoveRoute {
		public:
			void **vTable;
			DListPtr<MoveRouteItem *> moves; //!< List of movement commands (internal storage, not yet implemented)
			DArray<char> encodedMoves; //!< Encoded movement command list (see RPG::Character::move)
			bool repeatPattern; //!< \c true if the movement should be repeated until RPG::Character::stop (or "Halt all movement") is called
			bool ignoreImpossible; //!< \c true if impossible movement commands should be skipped
	};
}
