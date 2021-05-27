namespace RPG {
	//! Possible values for RPG::BattleSettings::layout
	enum BattleLayout {
		BL_TRADITIONAL, //!< "Traditional" layout
		BL_ALTERNATIVE, //!< "Alternative" layout
		BL_GAUGE //!< "Gauge" layout
	};
	
	//! One-byte version of RPG::BattleLayout
	typedef unsigned char BattleLayout_T;
	
	/*! \brief Used for battle settings (layout, etc.)
		\sa RPG::battleSettings
		\sa RPG::System
	*/
	class BattleSettings {
		public:
			void **vTable;
			int _unknown_4;
			BattleLayout_T layout; //!< Battle layout
			int _unknown_C;
			bool largeWindows; //!< Are the "large windows" active?
			bool _unknown_11;
			bool _unknown_12;
			bool _unknown_13;
			bool transparentWindows; //!< Are transparent windows active? (This affects the whole game even though the %RPG Maker guys have put it into the battle settings.)
	};
	
	/*! \ingroup game_objects
		\brief Battle settings
		
		Please mind that this object is not saved and loaded automatically, but only initialized once at startup.
	*/
	static RPG::BattleSettings *&battleSettings = (**reinterpret_cast<RPG::BattleSettings ***>(0x4CDD60));
}
