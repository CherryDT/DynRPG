namespace RPG {
	//! Not implemented yet
	typedef void AuroraDrawMain;
	
	/*! \brief Used for the screen, including window properties and FPS
		\sa RPG::screen
	*/
	class Screen { // TAuroraDraw
		public:
			void **vTable;
			AuroraDrawMain *auroraDrawMain; //!< Not implemented yet
			int _unknown_8;
			int _unknown_C;
			int _unknown_10;
			int _unknown_14;
			int _unknown_18;
			int _unknown_1C;
			int _unknown_20;
			int _unknown_24;
			int _unknown_28;
			int _unknown_2C;
			int _unknown_30;
			int _unknown_34;
			int _unknown_38;
			bool fullScreen; //!< Is fullscreen active?
			bool largeWindow; //!< Is the large window (640x480) active?
			Canvas *canvas; //!< RPG::Canvas which should be used for drawing on the screen
			int _unknown_44;
			int _unknown_48;
			int _unknown_4C;
			int _unknown_50;
			int _unknown_54;
			int _unknown_58;
			int _unknown_5C;
			int _unknown_60;
			int _unknown_64;
			int _unknown_68;
			int _unknown_6C;
			int _unknown_70;
			int _unknown_74;
			int _unknown_78;
			int _unknown_7C;
			int _unknown_80;
			int _unknown_84;
			int _unknown_88;
			int _unknown_8C;
			int _unknown_90;
			int _unknown_94;
			int _unknown_98;
			int _unknown_9C;
			int _unknown_A0;
			int _unknown_A4;
			int _unknown_A8;
			int _unknown_AC;
			int _unknown_B0;
			int _unknown_B4;
			int _unknown_B8;
			int _unknown_BC;
			double millisecondsPerFrame; //!< Milliseconds a frame should take
			int maxFPS; //!< Maximum FPS
			
			/*! \brief Returns the HWND of the window control on which the
				screen is drawn
				\note In windowed mode, this is not the window itself, but a
				child of it.
			*/
			HWND getCanvasHWND();
			
			/*! \brief Changes the framerate (and thus the overall speed of the
				game)
				
				This function changes the speed of the whole game by changing
				the number of frames rendered per second. The default is \c 60.
				\param fps New framerate
			*/
			void setFPS(int fps);
			
			/*! \brief Redraws the screen for a certain scene
				
				This function completely redraws the screen and uses the
				drawing method of the specified scene. Normally, you would use
				the current scene for that. This function will also wait for
				the next frame without using 100% CPU, so that you can use this
				function to do something which takes longer than one frame.
				\param scene Scene to draw
			*/
			void update(RPG::Scene scene);
	};
	
	/*! \ingroup game_objects
		\brief The screen, including window properties, FPS and the drawing
		canvas
	*/
	static RPG::Screen *&screen = (**reinterpret_cast<RPG::Screen ***>(0x4CDB24));
}
