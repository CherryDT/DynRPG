namespace RPG {
	//! Special effects for pictures
	enum PictureEffect {
		PFX_NONE, //!< No effect
		PFX_ROTATE, //!< Rotation
		PFX_RIPPLE //!< Ripple effect ("Waver")
	};
	
	/*! \brief Used for in-game pictures
		
		Pictures have <b>two</b> image buffers. The first one (\ref image) is
		used for the <b>top half</b> of the image, the second one (\ref image2)
		is used for the <b>bottom half</b> of the image. When the picture is
		drawn, the second buffer is drawn on top of the first buffer. I know
		that this is crazy, but it's the way the people from Enterbrain decided
		to do it to allow two different transparency values (a feature which,
		in my opinion, is also crazy and useless). Because this is impractical
		for plugin developers, there is a method called \ref merge which will
		draw the content of the bottom buffer on top of the top buffer and
		clear the bottom buffer afterwards. You may still put your own stuff
		into the \ref image2 buffer, it will be shown on top of \ref image, but
		please remember that <b>when the game is loaded, the %RPG Maker splits
		the picture again</b>. As a "marker" that the image is already merged
		(so that you don't have to do it every frame), you might change a
		palette entry of the bottom image.
		\sa RPG::pictures
		\sa RPG::Image
		\sa RPG::PictureEffect
	*/
	class Picture {
		public:
			void **vTable;
			int id; //!< Picture ID
			int _unknown_8;
			Image *image; //!< RPG::Image buffer of the top half (see class description)
			Image *image2; //!< RPG::Image buffer of the bottom half (see class description)
			DStringPtr filename; //!< Filename of the picture
			double originX; //!< X coordinate of original position (changes automatically when "Move with map" is used)
			double originY; //!< Y coordinate of original position (changes automatically when "Move with map" is used)
			double x; //!< Current X coordinate on screen (use \c std::lround to convert to an integer!)
			double y; //!< Current Y coordinate on screen (use \c std::lround to convert to an integer!)
			bool moveWithMap; //!< Is "Move with map" active?
			int _unknown_3C;
			double magnification; //!< Current magnification
			double transparency; //!< Current transparency for top half
			double transparency2; //!< Current transparency for bottom half
			bool useMaskColor; //!< Use a transparent color?
			int _unknown_5C;
			double red; //!< Current red color effect value
			double green; //!< Current green color effect value
			double blue; //!< Current blue color effect value
			double chroma; //!< Current chroma color effect value
			PictureEffect effect; //!< Current picture effect
			int _unknown_84;
			/*! \brief Strength of picture effect (see details)
				
				For RPG::PFX_ROTATE, negative values will rotate the picture
				conter-clockwise, while zero will pause the rotation.
			*/
			double effectStrength;
			double targetX; //!< Target X coordinate (if moving)
			double targetY; //!< Target Y coordainte (if moving)
			int targetMagnification; //!< Target magnification (if moving)
			int targetTransparency; //!< Target transparency of the top half (if moving)
			int targetTransparency2; //!< Target transparency of the bottom half (if moving)
			int targetRed; //!< Target red color effect value (if moving)
			int targetGreen; //!< Target green color effect value (if moving)
			int targetBlue; //!< Target blue color effect value (if moving)
			int targetChroma; //!< Target chroma color effect value (if moving)
			int targetEffectStrength; //!< Target picture effect strength (if moving)
			int movementTimer; //!< Number of frames left until movement is completed (zero means not moving)
			int _unknown_C4;
			double angle; //!< Current picture angle (\c 256 units equal to 360 degree!)
			
			//! Draws the content of the bottom buffer onto the top buffer and clears the bottom buffer afterwards
			void merge();
			
			/*! \brief Loads and shows a picture
				\param filename Filename
				\param x X position
				\param y Y position
				\param moveWithMap "Move with map" active?
				\param magnification Magnification
				\param transparency Top half transparency
				\param transparency2 Bottom half transparency
				\param useMaskColor Use a transparent color?
				\param red Red color effect value
				\param green Target green color effect value
				\param blue Blue color effect value
				\param chroma Chroma color effect value
				\param effect Picture effect
				\param effectStrength Picture effect strength
				\warning This is an experimental function.
			*/
			void show(
				std::string filename, int x, int y, bool moveWithMap, 
				int magnification, int transparency, int transparency2, 
				bool useMaskColor, int red, int green, int blue, int chroma, 
				RPG::PictureEffect effect, int effectStrength
			);
			
			/*! \brief Initiates a picture movement
				\param x Target X position
				\param y Target Y position
				\param magnification Target magnification
				\param transparency Target top half transparency
				\param transparency2 Target bottom half transparency
				\param red Target red color effect value
				\param green Target green color effect value
				\param blue Target blue color effect value
				\param chroma Target chroma color effect value
				\param effect New picture effect
				\param effectStrength Target picture effect strength
				\param duration Duration of the movement <b>in frames</b>
				\warning This is an experimental function.
			*/
			void move(
				int x, int y, int magnification, int transparency, int transparency2, 
				int red, int green, int blue, int chroma, RPG::PictureEffect effect, int effectStrength,
				int duration
			);
			
			/*! \brief Erases a picture
				\warning This is an experimental function.
			*/
			void erase();
			
			/*! \brief Updates picture movement and effects
				
				This function can be used to update picture movement and
				effects even in different game scenes than the map.
				\warning If this function is called more than once per frame,
				the movement or animation will be too fast.
			*/
			void update();
			
			//! Draws a picture to the screen, even in different game scenes than the map
			void draw();
	};
	
	/*! \ingroup game_objects
		\brief Array of pictures
		
		Use the picture ID as index for accessing a picture.
	*/
	static RPG::NamedCatalogPtr<RPG::Picture *> &pictures = (**reinterpret_cast<RPG::NamedCatalogPtr<RPG::Picture *> **>(0x4CDF3C));
}
