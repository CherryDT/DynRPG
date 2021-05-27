namespace RPG {
	//! Not implemented yet
	typedef void Chipset;

	/*! \brief Used for accessing and manipulating the current map environment
		\sa RPG::map
	*/
	class Map {
		public:
			void **vTable;
			int cameraSubX; //!< X position of the camera <b>in units of 1/16th of a pixel</b>
			int cameraSubY; //!< Y position of the camera <b>in units of 1/16th of a pixel</b>
			int _unknown_C;
			int _unknown_10;
			Chipset *chipset; //!< Pointer to the map's chipset (not yet implemented)
			NamedCatalogPtr<Event *> events; //!< Array of events (use event ID as index)

			//! Returns the current X position of the camera in pixels (this value means the position of the upper-left screen pixel of the screen on the map)
			int getCameraX();

			//! Returns the current Y position of the camera in pixels (this value means the position of the upper-left screen pixel of the screen on the map)
			int getCameraY();

			/*! \brief Sets a new camera position
				\param x New X position (pixels)
				\param y New Y position (pixels)
				\note This method also updates the camera "memory", so that the
				"Return to origin" function works
			*/
			void setCamera(int x, int y);

			/*! \brief Moves the camera (pixel-exact)

				This function can move the camera the specified amount of
				pixels in the specified speed.
				\param offsetX Amount of pixels to move horizontally (may also be negative)
				\param offsetY Amount of pixels to move vertically (may also be negative)
				\param speed Exact speed, in units of 3.75 pixels per second (1/16 tile per frame)
				\sa RPG::Character::customExactSpeed
			*/
			void moveCamera(int offsetX, int offsetY, int speed);

			//! Returns the width of the current map
			int getWidth();

			//! Returns the height of the current map
			int getHeight();

			/*! \brief Updates the start conditions of events after a change

				This function can be used to re-check the start conditions of
				all events. This is necessary if you modify a switch from
				DynRPG which is start condition of an event, for example.
			*/
			void updateEvents();
	};

	/*! \ingroup game_objects
		\brief The current map environment (camera, events, etc.)

		For accessing events, use <tt>RPG::map->events[<i>event ID</i>]</tt>.
	*/
	static RPG::Map *&map = (**reinterpret_cast<RPG::Map ***>(0x4CDD74));
}
