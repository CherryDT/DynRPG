namespace RPG {
	/*! \brief In-game scenes

		Each scene has its own drawing method and its own data.
		You may also "invent" a new scene and draw its contents using the
		\ref onFrame callback.
		\sa onFrame
		\sa RPG::Screen::update
	*/
	enum Scene {
		SCENE_MAP, //!< %Map
		SCENE_MENU, //!< Game menu
		SCENE_BATTLE, //!< Battle
		SCENE_SHOP, //!< Shop
		SCENE_NAME, //!< %Hero naming screen
		SCENE_FILE, //!< Save <b>or</b> load menu
		SCENE_TITLE, //!< Title screen
		SCENE_GAME_OVER, //!< Game over screen
		SCENE_DEBUG //!< Debug screen (\c F9 menu)
	};

	//! One-byte version of RPG::Scene
	typedef unsigned char Scene_T;

	/*! \brief Used to store the system and "system2" graphics
		\sa RPG::System
		\sa RPG::Image::setSystemPalette
	*/
	class SystemGraphic {
		public:
			void **vTable;
			Image *systemImage; //!< \c %System image
			int _unknown_8;
			//! \cond
			Canvas *font;
			//! \endcond
			int _unknown_10;
			int _unknown_14;
			Image *system2Image; //!< \c System2 image

			/*! \brief Loads the font used for text drawing
				\param fontName Name of the font
				\warning This is an experimental function.
			*/
			void loadFont(std::string fontName);
	};

	//! Possible values for RPG::System::messagePosition
	enum MessagePosition {
		MSGPOS_TOP, //!< Top
		MSGPOS_MIDDLE, //!< Middle
		MSGPOS_BOTTOM //!< Bottom
	};

	//! Possible values for RPG::System::facePosition
	enum FacePosition {
		FACEPOS_LEFT, //!< Left
		FACEPOS_RIGHT //!< Right
	};

	//! Possible values for RPG::System::atbMode
	enum ATBMode {
		ATBM_ACTIVE, //!< "Active" mode (enemies also attack in selection menus)
		ATBM_WAIT //!< "Wait" mode
	};

	//! One-byte version or RPG::ATBMode
	typedef unsigned char ATBMode_T;


	/*! \brief Used for system data which can change in-game

		\note Many things are only stored in this class if they were changed
		and are now different from the default in the database, thus you
		should use the getter methods to access these things.
		\sa RPG::system
		\sa RPG::DBSystem
		\sa RPG::dbSystem
	*/
	class System {
		public:
			void **vTable;
			Scene_T scene; //!< Current game scene (see RPG::Scene)
			/*! \brief Internal frame counter (see details!)

				This frame counter will count every update of the game scene.
				Please <b>read the \ref onFrame documentation</b> too!
				\sa onFrame
			*/
			int frameCounter;
			DStringPtr systemGraphicFilename; //!< Filename of the system graphic (empty for default)
			bool systemTiled; //!< Is the window background tiled?
			SystemFont systemFont; //!< Current system font
			SystemGraphic *systemGraphic; //!< Current system and system2 graphic
			DArray<bool, 1> switches; //!< %Switches (see also RPG::Switches!)
			DArray<int, 1> variables; //!< %Variables (see also RPG::Variables!)
			bool messageTransparent; //!< Is the message background invisible? (see also RPG::BattleSettings::transparentWindows)
			MessagePosition messagePosition; //!< Position of the message
			bool messageAutoPos; //!< Prevent hero from being overlapped by a message?
			bool messageModal; //!< Should messages pause other event activites?
			DStringPtr faceFilename; //!< Filename of the current face
			int faceID; //!< ID of the sprite in the faceset
			FacePosition facePosition; //!< Position of the face
			bool faceMirrored; //!< Is the face mirrored?
			Image *faceImage; //!< RPG::Image of the face
			bool messageActive; //!< Is a message active?
			bool musicFade; //!< Is the music fading?
			Music *currentBGM; //!< Current background music
			Music *pedestrianBGM; //!< Background music which should be restored after the vehicle has been exited
			Music *mapBGM; //!< Background music on the map (used to restore music after battle)
			Music *memorizedBGM; //!< Memorized music
			Music *titleBGM; //!< Current title screen music (empty for default)
			Music *battleBGM; //!< Current battle music (empty for default)
			Music *victoryBGM; //!< Current victory music (empty for default)
			Music *innBGM; //!< Current inn music (empty for default)
			Music *skiffBGM; //!< Current skiff music (empty for default)
			Music *shipBGM; //!< Current ship music (empty for default)
			Music *airshipBGM; //!< Current airship music (empty for default)
			Music *gameOverBGM; //!< Current game over music (empty for default)
			Sound *cursorSE; //!< Current cursor sound (empty for default)
			Sound *decisionSE; //!< Current decision sound (empty for default)
			Sound *cancelSE; //!< Current cancel sound (empty for default)
			Sound *buzzerSE; //!< Current buzzer sound (empty for default)
			Sound *battleStartSE; //!< Current sound played at battle start (empty for default)
			Sound *fleeSE; //!< Current sound played when a battler escapes (empty for default)
			Sound *enemyAttackSE; //!< Current sound played when a monster attacks (empty for default)
			Sound *enemyDamageSE; //!< Current sound played when a monster is damaged (empty for default)
			Sound *heroDamageSE; //!< Current sound played when an actor is damaged (empty for default)
			Sound *evasionSE; //!< Current sound played when an attack is evaded (empty for default)
			Sound *enemyDeathSE; //!< Current sound played when a monster is killed (empty for default)
			Sound *itemSE; //!< Current sound played when an item is used (empty for default)
			Transition_T teleportEraseTrans; //!< Current screen erasing transition on teleport (see RPG::Transition)
			Transition_T teleportShowTrans; //!< Current screen showing transition on teleport (see RPG::Transition)
			Transition_T battleStartEraseTrans; //!< Current screen erasing transition on battle start (see RPG::Transition)
			Transition_T battleStartShowTrans; //!< Current screen showing transition on battle start (see RPG::Transition)
			Transition_T battleEndEraseTrans; //!< Current screen erasing transition on battle end (see RPG::Transition)
			Transition_T battleEndShowTrans; //!< Current screen showing transition on battle end (see RPG::Translation)
			bool teleportAllowed; //!< Are teleport skills allowed?
			bool escapeAllowed; //!< Are escaped skills allowed?
			bool saveAllowed; //!< Is saving allows?
			bool menuAllowed; //!< Is the game menu allowed?
			DStringPtr defaultBackdrop; //!< Filename of default backdrop
			int saveCount; //!< Number of times saved
			int _unknown_C4;
			ATBMode_T atbMode; //!< ATB mode

	#define defaultAccessorBGM(_member_, _function_) RPG::Music *_function_()
	#define defaultAccessorSE(_member_, _function_) RPG::Sound *_function_()
	#define defaultAccessorTrans(_member_, _function_) RPG::Transition _function_()

			//! Returns the current title screen music
			defaultAccessorBGM(titleBGM, getTitleBGM);
			//! Returns the current battle music
			defaultAccessorBGM(battleBGM, getBattleBGM);
			//! Returns the current victory music
			defaultAccessorBGM(victoryBGM, getVictoryBGM);
			//! Returns the current inn music
			defaultAccessorBGM(innBGM,getInnBGM);
			//! Returns the current skiff music
			defaultAccessorBGM(skiffBGM, getSkiffBGM);
			//! Returns the current ship music
			defaultAccessorBGM(shipBGM, getShipBGM);
			//! Returns the current airship music
			defaultAccessorBGM(airshipBGM, getAirshipBGM);
			//! Returns the current game over music
			defaultAccessorBGM(gameOverBGM, getGameOverBGM);

			//! Returns the current cursor sound
			defaultAccessorSE(cursorSE, getCursorSE);
			//! Returns the current decision sound
			defaultAccessorSE(decisionSE, getDecisionSE);
			//! Returns the current cancel sound
			defaultAccessorSE(cancelSE, getCancelSE);
			//! Returns the current buzzer sound
			defaultAccessorSE(buzzerSE, getBuzzerSE);
			//! Returns the current battle start sound
			defaultAccessorSE(battleStartSE, getBattleStartSE);
			//! Returns the current escape sound
			defaultAccessorSE(fleeSE, getFleeSE);
			//! Returns the current monster attack sound
			defaultAccessorSE(enemyAttackSE, getEnemyAttackSE);
			//! Returns the current monster damage sound
			defaultAccessorSE(enemyDamageSE, getEnemyDamageSE);
			//! Returns the current actor damage sound
			defaultAccessorSE(heroDamageSE, getHeroDamageSE);
			//! Returns the current evasion sound
			defaultAccessorSE(evasionSE, getEvasionSE);
			//! Returns the current monster death sound
			defaultAccessorSE(enemyDeathSE, getEnemyDeathSE);
			//! Returns the current item sound
			defaultAccessorSE(itemSE, getItemSE);

			//! Returns the current teleport screen erasing transition
			defaultAccessorTrans(teleportEraseTrans, getTeleportEraseTrans);
			//! Returns the current teleport screen showing transition
			defaultAccessorTrans(teleportShowTrans, getTeleportShowTrans);
			//! Returns the current battle start screen erasing transition
			defaultAccessorTrans(battleStartEraseTrans, getBattleStartEraseTrans);
			//! Returns the current battle start screen showing transition
			defaultAccessorTrans(battleStartShowTrans, getBattleStartShowTrans);
			//! Returns the current battle end screen erasing transition
			defaultAccessorTrans(battleEndEraseTrans, getBattleEndEraseTrans);
			//! Returns the current battle end screen showing transition
			defaultAccessorTrans(battleEndShowTrans, getBattleEndShowTrans);

	#undef defaultAccessorBGM
	#undef defaultAccessorSE
	#undef defaultAccessorTrans

	};

	/*! \ingroup game_objects
		\brief Object of "system" data, used for values which can be changed
		in-game
		\sa RPG::dbSystem
	*/
	static RPG::System *&system = (**reinterpret_cast<RPG::System ***>(0x4CDC7C));
}
