namespace RPG {
	/*! \brief Used for background music settings
		
		This class is used to store the settings of a background music "item".
		This class is one of the exceptions to the "do not instantiate" rule,
		because it may be created and destroyed like any other C++ object.
		However, RPG::Music pointers in RPG classes still shouldn't be
		assigned, you have to use the \ref set method instead.
		\sa set
		\sa RPG::Sound
	*/
	class Music {
		public:
			void **vTable;
			DStringPtr filename; //!< Filename
			int fadeInTime; //!< Time to fade in (in milliseconds)
			int volume; //!< Volume (\c 0 to \c 100)
			int speed; //!< Speed (\c 100 is normal)
			int pan; //!< Pan (\c 0 to \c 100, \c 50 is normal)
			
			//! Simple constructor, creates an \c (OFF) music item which will stop the music when played
			Music();
			
			/*! \brief Copy constructor
				\param ref RPG::Music item to copy
				\sa set
			*/
			Music(RPG::Music &ref);
			
			/*! \brief Extended constructor, creates a music item based on the given settings
				\param filename Filename
				\param fadeInTime Time to fade in
				\param volume Volume
				\param speed Speed
				\param pan Pan value
			*/
			Music(std::string filename, int fadeInTime, int volume, int speed, int pan);
			
			//! Destructor
			~Music();
			
			/*! \brief Sets a music item to the settings of another
				
				This method should be used to "assign" a music item to an
				RPG::Music pointer in an RPG class.
				
				Example:
				\code
RPG::Music newMusic("Boss1", 0, 100, 100, 50);
RPG::system->battleBGM->set(newMusic);
				\endcode
				\sa Music(RPG::Music &)
			*/
			void set(RPG::Music &ref);
			
			//! Plays the music
			void play();
			
			//! Stops the current music
			static void stop();
			
			/*! \brief Fades out the current music
				\param time Duration (in milliseconds)
			*/
			static void fadeOut(int time);
			
		private:
			void init();
	};
}
