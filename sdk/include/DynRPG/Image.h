namespace RPG {
	//! \cond
	class Image;
	//! \endcond
	
	enum {
		MASK_NONE = -1 //!< Used for RPG::Image::draw if no transparent color should be used
	};
	
	/*! \brief Color effect settings for RPG::Image objects
		\sa RPG::Image
	*/
	class ColorControl {
		public:
			void **vTable;
			Image *parentImage; //!< Pointer to the image to which the color effect belongs
			int red; //!< Red value (\c 0 to \c 255)
			int green; //!< Green value (\c 0 to \c 255)
			int blue; //!< Blue value (\c 0 to \c 255)
			int chroma; //!< Chroma value (\c 0 to \c 255)
	};
	
	/*! \brief Used for image buffers (8 bit)
		
		This class is the only class of which instances may be created by the
		plugin developer, using the \ref create and \ref destroy methods. It is
		used for all kinds of images (8 bit). There are two palette arrays: The
		\ref palette array is used to store the actual 24-bit colors, and the
		\ref appliedPalette array is used to store special calculated values
		corresponding to the current screen brightness. It uses 16-bit colors.
		The \ref applyPalette method is used to recalculate the
		\ref appliedPalette values. The \ref appliedPaletteBrightness member
		stores the brightness value for which the \ref appliedPalette was
		created. If it doesn't equal to the current screen brightness, the
		%RPG Maker will automatically call \ref applyPalette when the image is
		drawn.
		
		I know that this is overkill and it would have been easier to
		draw the images to the screen first and then reduce the brightness of
		the pixels on the screen, but the %RPG Maker decided to do it this way.
		\note Drawing images to a RPG::Canvas is very slow, as well as the
		\ref drawText method. Please see the \ref optimization section!
		\sa RPG::ColorControl
		\sa RPG::Canvas
		\sa RPG::Canvas::brightness
	*/
	class Image {
		public:
			void **vTable;
			unsigned char *pixels; //!< Pointer to direct pixel data (stored from top to bottom)
			int palette[256]; //!< Palette array (24 bit)
			short appliedPalette[256]; //!< Processed palette array (16 bit - do not use directly)
			int width; //!< Width of the image
			int height; //!< Height of the image
			bool useMaskColor; //!< If \c true, color \c 0 will be used as transparent color
			int alpha; //!< Alpha value (\c 0 is invisible, \c 255 is fully visible)
			bool autoResize; //!< If \c true, the image will automatically resize when loaded from a file
			ColorControl *colorControl1; //!< First color effect
			ColorControl *colorControl2; //!< Second color effect
			int appliedPaletteBrightness; //!< Brightness for which the \ref appliedPalette was calculated
			
			//! Applies palette changes
			void applyPalette();
			
			/*! \brief Returns a reference to a certain pixel
				\param x X coordinate of the pixel
				\param y Y coordinate of the pixel
				\return Reference to the pixel
			*/
			unsigned char &pixel(int x, int y);
			
			/*! \brief Frees the image and sets its size to zero
				\sa init
				\sa clear
				\sa destroy
			*/
			void free();
			
			/*! \brief Clears the image and initializes it to a new size
				\param newWidth New image width
				\param newHeight New image height
				\sa free
			*/
			void init(int newWidth, int newHeight);
			
			/*! \brief Copies a palette to the image and applies it
				\param newPalette Pointer to the palette array to copy
				\sa setSystemPalette
				\sa copy
			*/
			void setPalette(int* newPalette);
			
			/*! \brief Copies an RPG::Image with all its attributes to another
				
				Instead of <tt>*image2 = *image1;</tt>, you must use
				<tt>image2->copy(image1);</tt>.
				\param image The image to copy (source)
				\sa draw(int, int, Image *, int, int, int, int, int)
				\sa create(RPG::Image *)
			*/
			void copy(RPG::Image *image);
			
			/*! \brief Loads an image from a file
				
				This function will load an image from a BMP, PNG or XYZ file.
				\param filename Path and filename of the file to load,
				<b>including folder name and file extension</b>.
				\param throwErrors If \c true, an error will be shown when the
				image doesn't exist, otherwise the image will just be empty.
				\param autoResize If \c true, the image will automatically resize
				to the size of the image in the file.
			*/
			void loadFromFile(std::string filename, bool throwErrors = true, bool autoResize = true);
			
			/*! \brief Copies pixels into the image
				\param x X coordinate to start drawing to
				\param y Y coordinate to start drawing to
				\param newPixels Pointer to the pixel data to copy
				\param srcWidth Width of area to copy
				\param srcHeight Height of the area to copy
				\param srcLineWidth Width of a pixel row in the source image
				\param maskColor Color which should be transparent (can also be
				RPG::MASK_NONE)
				\note Please also see the \ref optimization section!
				\sa draw(int, int, RPG::Image *, int, int, int, int, int)
			*/
			void draw(int x, int y, unsigned char *newPixels, int srcWidth, int srcHeight, int srcLineWidth, int maskColor = 0);
			
			/*! \brief Draws another image (or a part of it) onto the image
				
				This part can be used to blit another image, or a certain part
				of it. Please keep in mind that this will produce strange
				results if the other image has a different palette.
				\param x Upper-left X coordinate at the destination image
				\param y Upper-left Y coordinate at the destination image
				\param image %Image to draw
				\param srcX Upper-left X coordinate at the source image
				\param srcY Upper-left Y coordinate at the source image
				\param srcWidth Width of the area to copy (defaults to the
				whole image)
				\param srcHeight Height of the area to copy (defaults to the
				whole image)
				\param maskColor Color which should be transparent (can also be
				RPG::MASK_NONE)
				\note Please also see the \ref optimization section!
				\sa draw(int, int, unsigned char *, int, int, int, int)
			*/
			void draw(int x, int y, RPG::Image *image, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1, int maskColor = 0);
			
			/*! \brief Clears the image without resizing or freeing it
				\sa free
				\sa destroy
			*/
			void clear();
			
			/*! \brief Copies the palette from the system (necessary for
				\ref drawText)
				\sa drawText
				\sa setPalette
			*/
			void setSystemPalette();
			
			/*! \brief Draws text onto the image
				
				This method will draw text onto the image, using the current
				system font and system graphic. Glyphs (like <tt>$A</tt>) work
				too.
				\param x Upper-left X position
				\param y Upper-left Y position
				\param text Text to draw
				\param color Text color to use (\c 0 to \c 19)
				\pre The image should have the same palette as the system
				graphic (use \ref setSystemPalette).
				\warning This method is quite slow. Do not use it too often.
				Please also read the \ref optimization section!\n\n
				In the image, there must be at least <b>8 pixels of extra space
				to the right</b>, otherwise it
				will corrupt memory! The text itself needs 16 pixels vertically
				and 6 pixels horizontally (per character).
				\sa setSystemPalette
			*/
			void drawText(int x, int y, std::string text, int color);
			
			/*! \brief Creates an empty image
				\return Pointer to the image
				\sa create(int, int)
				\sa create(RPG::Image *)
				\sa destroy
			*/
			static RPG::Image *create();
			
			/*! \brief Creates an image with a certain width and height
				\param newWidth Width of the new image
				\param newHeight Height of the new image
				\return Pointer to the image
				\sa create()
				\sa create(RPG::Image *)
				\sa destroy
				\sa init
			*/
			static RPG::Image *create(int newWidth, int newHeight);
			
			/*! \brief Creates a copy of an image
				\param templateImage %Image to copy
				\return Pointer to the new image
				\sa create()
				\sa create(int, int)
				\sa destroy
				\sa copy
			*/
			static RPG::Image *create(RPG::Image *templateImage);
			
			/*! \brief Destroys an image
				\param image Pointer to the image which should be destroyed
				\note This function automatically sets the pointer to zero
				after destroying the image.
				\sa create
				\sa free
				\sa clear
			*/
			static void destroy(RPG::Image *&image);
	};
}
