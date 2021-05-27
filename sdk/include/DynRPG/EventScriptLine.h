namespace RPG {
	/*! \brief %Event command type
		
		Each event command can have a number of parameters. Unfortunately,
		documenting all of them (nearly 150) is beyond my time resources
		at the moment. If you need to know how the parameters of a certain
		command work, you can write a simple plugin which logs event commands
		and their parameters and try it out yourself.
		\sa RPG::EventScriptLine
		\sa onEventCommand
		\note <b>Not all enumeration values are documented here</b>, scroll to the top
		to see all possible values (in the general enum overview)!
	*/
	enum EventCommand {
		EVCMD_END_OF_EVENT = 0, //!< End of the event command list
		EVCMD_EMPTY_LINE = 10, //!< Empty line (<tt>\<\></tt>)
		EVCMD_SHOW_MESSAGE = 10110, //!< First line of a "Show Message" command
		EVCMD_ADD_LINE_TO_MESSAGE = 20110, //!< Additional lines of a "Show Message" command
		EVCMD_MESSAGE_OPTIONS = 10120,
		EVCMD_SELECT_FACE = 10130,
		EVCMD_SHOW_CHOICE = 10140, //!< First line of a "Show Choice" command
		EVCMD_CHOICE_CASE = 20140, //!< <tt>[XXXX] handler</tt> lines of a "Show Choice" command
		EVCMD_CHOICE_END = 20141, //!< <tt>: End</tt> line of a "Show Choice" command
		EVCMD_INPUT_NUMBER = 10150,
		EVCMD_CHANGE_SWITCH = 10210,
		EVCMD_CHANGE_VARIABLE = 10220,
		EVCMD_CHANGE_TIMER = 10230,
		EVCMD_CHANGE_MONEY = 10310,
		EVCMD_CHANGE_ITEMS = 10320,
		EVCMD_CHANGE_PARTY = 10330,
		EVCMD_CHANGE_EXP = 10410,
		EVCMD_CHANGE_LEVEL = 10420,
		EVCMD_CHANGE_STATS = 10430,
		EVCMD_CHANGE_SKILL = 10440,
		EVCMD_CHANGE_EQUIPMENT = 10450,
		EVCMD_CHANGE_HP = 10460,
		EVCMD_CHANGE_MP = 10470,
		EVCMD_CHANGE_CONDITION = 10480,
		EVCMD_FULL_RECOVERY = 10490,
		EVCMD_INFLICT_DAMAGE = 10500,
		EVCMD_CHANGE_HERO_NAME = 10610,
		EVCMD_CHANGE_HERO_TITLE = 10620,
		EVCMD_CHANGE_HERO_GRAPHIC = 10630,
		EVCMD_CHANGE_HERO_FACE = 10640,
		EVCMD_CHANGE_VEHICLE_GRAPHIC = 10650,
		EVCMD_CHANGE_SYSTEM_BGM = 10660,
		EVCMD_CHANGE_SYSTEM_SE = 10670,
		EVCMD_CHANGE_SYSTEM_GRAPHIC = 10680,
		EVCMD_CHANGE_TRANSITION = 10690,
		EVCMD_START_COMBAT = 10710, //!< First line of a "Start Combat" command
		EVCMD_BATTLE_VICTORY = 20710, //!< <tt>Victory handler</tt> line of a "Start Combat" command
		EVCMD_BATTLE_ESCAPE = 20711, //!< <tt>Escape handler</tt> line of a "Start Combat" command
		EVCMD_BATTLE_DEFEAT = 20712, //!< <tt>Defeat handler</tt> line of a "Start Combat" command
		EVCMD_BATTLE_END = 20713, //!< <tt>: End</tt> line of a "Start Combat" command
		EVCMD_CALL_SHOP = 10720, //!< First line of a "Call Shop" command
		EVCMD_SHOP_TRANSACTION = 20720, //!< <tt>Transaction handler</tt> line of a "Call Shop" command
		EVCMD_SHOP_CANCEL = 20721, //!< <tt>Cancel handler</tt> line of a "Call Shop" command
		EVCMD_SHOP_END = 20722, //!< <tt>: End</tt> line of "Call Shop" command
		EVCMD_CALL_INN = 10730, //!< First line of a "Call Inn" command
		EVCMD_INN_REST = 20730, //!< <tt>Rest handler</tt> line of a "Call Inn" command
		EVCMD_INN_CANCEL = 20731, //!< <tt>Cancel handler</tt> line of a "Call Inn" command
		EVCMD_INN_END = 20732, //!< <tt>: End</tt> line of a "Call Inn" command
		EVCMD_ENTER_HERO_NAME = 10740,
		EVCMD_TELEPORT = 10810,
		EVCMD_MEMORIZE_LOCATION = 10820,
		EVCMD_GO_TO_MEMORIZED_LOCATION = 10830,
		EVCMD_RIDE_VEHICLE = 10840,
		EVCMD_TELEPORT_VEHICLE = 10850,
		EVCMD_SET_EVENT_LOCATION = 10860,
		EVCMD_SWAP_TWO_EVENT_LOCATIONS = 10870,
		EVCMD_GET_TERRAIN_ID = 10910,
		EVCMD_GET_EVENT_ID = 10920,
		EVCMD_ERASE_SCREEN = 11010,
		EVCMD_SHOW_SCREEN = 11020,
		EVCMD_SET_SCREEN_TONE = 11030,
		EVCMD_FLASH_SCREEN = 11040,
		EVCMD_SHAKE_SCREEN = 11050,
		EVCMD_PAN_SCREEN = 11060,
		EVCMD_WEATHER_EFFECTS = 11070,
		EVCMD_SHOW_PICTURE = 11110,
		EVCMD_MOVE_PICTURE = 11120,
		EVCMD_ERASE_PICTURE = 11130,
		EVCMD_SHOW_BATTLE_ANIMATION = 11210, //!< "Show Battle Animation" in map/common events
		EVCMD_SET_HERO_OPACITY = 11310,
		EVCMD_FLASH_EVENT = 11320,
		EVCMD_MOVE_EVENT = 11330,
		EVCMD_WAIT_UNTIL_MOVED = 11340,
		EVCMD_STOP_ALL_MOVEMENT = 11350,
		EVCMD_WAIT = 11410,
		EVCMD_PLAY_BGM = 11510,
		EVCMD_FADE_OUT_BGM = 11520,
		EVCMD_MEMORIZE_BGM = 11530,
		EVCMD_PLAY_MEMORIZED_BGM = 11540,
		EVCMD_PLAY_SOUND_EFFECT = 11550,
		EVCMD_PLAY_MOVIE = 11560,
		EVCMD_KEY_INPUT_PROCESSING = 11610,
		EVCMD_CHANGE_TILESET = 11710,
		EVCMD_CHANGE_PANORAMA = 11720,
		EVCMD_CHANGE_ENCOUNTER_RATE = 11740,
		EVCMD_CHANGE_SINGLE_TILE = 11750,
		EVCMD_CHANGE_TELEPORT_TARGET = 11810,
		EVCMD_ENABLE_TELEPORT = 11820,
		EVCMD_SET_ESCAPE_LOCATION = 11830,
		EVCMD_ENABLE_ESCAPE = 11840,
		EVCMD_CALL_SAVE_MENU = 11910,
		EVCMD_ENABLE_SAVE = 11930,
		EVCMD_CALL_MAIN_MENU = 11950,
		EVCMD_ENABLE_MAIN_MENU = 11960,
		EVCMD_FORK = 12010, //!< First line of a "Conditional Branch" in map/common events
		EVCMD_FORK_ELSE_CASE = 22010, //!< <tt>: Else Case</tt> line of a "Conditional Branch" in map/common events
		EVCMD_FORK_END = 22011, //!< <tt>: End Case</tt> line of a "Conditional Branch" in map/common events
		EVCMD_LABEL = 12110,
		EVCMD_JUMP_TO_LABEL = 12120,
		EVCMD_START_LOOP = 12210, //!< "Loop" command
		EVCMD_END_LOOP = 22210, //!< <tt>: End</tt> line of a "Loop"
		EVCMD_BREAK_LOOP = 12220,
		EVCMD_STOP_EVENT = 12310,
		EVCMD_DELETE_EVENT = 12320,
		EVCMD_CALL_EVENT = 12330, //!< "Call Event" in map/common events
		EVCMD_COMMENT = 12410, //!< First line of a comment
		EVCMD_ADD_LINE_TO_COMMENT = 22410, //!< Additional lines of a comment
		EVCMD_GAME_OVER = 12420,
		EVCMD_GO_TO_TITLE_SCREEN = 12510,
		EVCMD_CHANGE_CLASS = 1008,
		EVCMD_CHANGE_BATTLE_COMMANDS = 1009,
		EVCMD_CHANGE_ENEMY_HP = 13110,
		EVCMD_CHANGE_ENEMY_MP = 13120,
		EVCMD_CHANGE_ENEMY_CONDITION = 13130,
		EVCMD_REVIVE_ENEMY = 13150,
		EVCMD_CHANGE_BACKDROP = 13210,
		EVCMD_SHOW_BATTLE_ANIMATION_IN_BATTLE = 13260,  //!< "Show Battle Animation" in battle events
		EVCMD_ENABLE_COMBO = 1007,
		EVCMD_FORCE_FLEE = 1006,
		EVCMD_END_BATTLE = 13410,
		EVCMD_FORK_IN_BATTLE = 13310, //!< First line of a "Conditional Branch" in battle events
		EVCMD_FORK_IN_BATTLE_ELSE_CASE = 23310, //!< <tt>: Else Case</tt> line of a "Conditional Branch" in battle events
		EVCMD_FORK_IN_BATTLE_END = 23311, //!< <tt>: End Case</tt> line of a "Conditional Branch" in battle events
		EVCMD_CALL_COMMON_EVENT = 1005 //!< "Call Common Event" in battle events
	};
	
	/*! \brief Represents an event script line
		
		%Event script lines can have one string parameter and unlimited integer
		parameters.
	*/
	class EventScriptLine {
		public:
			void **vTable;
			EventCommand command; //!< The command type
			int treeDepth; //!< The level of indention (\c 0 for none)
			DStringPtr stringParameter; //!< The string parameter
			DArray<int> parameters; //!< The integer parameters
			
			/*! \brief Returns an integer parameter (\c 0 if the parameter doesn't exist)
				\param index Parameter index (zero-based)
				\return The parameter's value or \c 0 if the parameter doesn't exist
			*/
			int parameter(int index);
	};
}
