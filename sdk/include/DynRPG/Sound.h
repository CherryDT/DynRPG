namespace RPG {
	/*! \brief Used for sound effect settings
		
		This class is used to store the settings of a sound effect "item".
		This class is one of the exceptions to the "do not instantiate" rule,
		because it may be created and destroyed like any other C++ object.
		However, RPG::Sound pointers in RPG classes still shouldn't be
		assigned, you have to use the \ref set method instead.
		\sa set
		\sa RPG::Music
	*/
	class Sound {
		public:
			void **vTable;
			DStringPtr filename; //!< Filename
			int volume; //!< Volume (\c 0 to \c 100)
			int speed; //!< Speed (\c 100 is normal)
			int pan; //!< Pan (\c 0 to \c 100, \c 50 is normal)
			
			//! Simple constructor, creates an \c (OFF) sound item which will stop all sounds when played
			Sound();
			
			/*! \brief Copy constructor
				\param ref RPG::Sound item to copy
				\sa set
			*/
			Sound(RPG::Sound &ref);
			
			/*! \brief Extended constructor, creates a sound item based on the given settings
				\param filename Filename
				\param volume Volume
				\param speed Speed
				\param pan Pan value
			*/
			Sound(std::string filename, int volume, int speed, int pan);
			
			//! Destructor
			~Sound();
			
			/*! \brief Sets a sound item to the settings of another
				
				This method should be used to "assign" a sound item to an
				RPG::Sound pointer in an RPG class.
				
				Example:
				\code
RPG::Sound newSound("Cursor2", 100, 100, 50);
RPG::system->cursorSE->set(newSound);
				\endcode
				\sa Sound(RPG::Sound &)
			*/
			void set(RPG::Sound &ref);
			
			//! Plays the sound
			void play();
			
			//! Stops all sounds
			static void stop();
			
		private:
			void init();
	};
}
