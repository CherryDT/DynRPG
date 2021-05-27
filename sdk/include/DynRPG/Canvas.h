namespace RPG {
	/*! \brief %Image buffer with 16 bit color depth, used as canvas for multiple 8-bit images
		\sa RPG::Screen::canvas
		\sa RPG::Image
	*/
	class Canvas {
		public:
			void **vTable;
			DBitmap *bitmap; //!< Underlying 16-bit bitmap
			/*! \brief Number of bytes you have to add to a pixel address to
				get the address of the pixel in the same column of the next
				pixel row (see details)
				
				Normally, this number is negative because bitmaps are stored
				from bottom to top.
			*/
			int lineSize;
			/*! \brief Brightness of the screen (see details)
				
				Supported values are from \c 0 (completely black) to \c 200
				(completely white). \c 100 is the normal value. This value is
				changed during "fading" screen transitions.
				
				\note Values higher than \c 200 will cause the colors to go
				crazy. Of course, this can also be used as an intentional
				effect, for example the value \c 204 will only use the colors
				blue, white and black.
			*/
			int brightness;
			
			/*! \brief Returns a reference to a pixel
				\param x X coordinate
				\param y Y coordinate
				\return Reference to the pixel
			*/
			unsigned short &pixel(int x, int y);
			
			/*! \brief Returns a pointer to a row of pixel data
				\param y Y coordinate
				\return Pointer to the first pixel of the row
				\note If you loop through rows, it is way faster to use
				<tt>getScanline(0)</tt> once and then always add \c lineSize to
				get to the next row instead of calling \c getScanline for every
				row.
			*/
			unsigned short *getScanline(int y);
			
			//! Returns the width of the bitmap
			int width();
			
			//! Returns the height of the bitmap
			int height();
			
			/*! \brief Draws an RPG::Image or a part of it onto the canvas
				
				This function will draw a certain part of an RPG::Image onto
				the canvas. You can omit the last four parameters to draw the
				whole image.
				
				The image's brightness will automatically be adjusted to match
				the canvas \c brightness. To avoid this, you can save the
				canvas \c brightness, set it to 100, draw the image and then
				restore the original \c brightness of the canvas.
				\param x X coordinate to draw to (upper-left origin)
				\param y Y coordinate to draw to (upper-left origin)
				\param image %Image to draw
				\param srcX X coordinate of the area to use from the source
				image
				\param srcY Y coordinate of the area to use from the source
				image
				\param srcWidth Width of the area to draw (defaults to the
				whole image)
				\param srcHeight Height of the area to draw (defaults to the
				whole image)
				\warning This function is quite slow, especially if you draw
				an image which uses a mask color or an \c alpha value different
				from \c 255. Please also see the \ref optimization guidelines!
				\sa RPG::Image::draw
				\sa RPG::Canvas::drawStretched
				\sa RPG::Canvas::drawCenteredZoomed
			*/
			void draw(int x, int y, RPG::Image *image, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);
			
			/*! \brief Draws an RPG::Image or a part of it onto the canvas,
				stretched to a certain rectangle
				
				This function will draw a certain part of an RPG::Image onto
				the canvas. You can omit the last four parameters to draw the
				whole image. The image (or the part of it) will be stretched
				so it fits into the given rectangle
				
				The image's brightness will automatically be adjusted to match
				the canvas \c brightness. To avoid this, you can save the
				canvas \c brightness, set it to 100, draw the image and then
				restore the original \c brightness of the canvas.
				\param x X coordinate to draw to (upper-left origin)
				\param y Y coordinate to draw to (upper-left origin)
				\param width Target width of the image after stretching
				\param height Target height of the image after stretching
				\param image %Image to draw
				\param srcX X coordinate of the area to use from the source
				image
				\param srcY Y coordinate of the area to use from the source
				image
				\param srcWidth Width of the area to draw (defaults to the
				whole image)
				\param srcHeight Height of the area to draw (defaults to the
				whole image)
				\warning This function is quite slow, especially if you draw
				an image which uses a mask color or an \c alpha value different
				from \c 255. Please also see the \ref optimization guidelines!
				\sa RPG::Canvas::drawCenteredZoomed
				\sa RPG::Canvas::draw
			*/
			void drawStretched(int x, int y, int width, int height, RPG::Image *image, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);

			/*! \brief Draws an RPG::Image or a part of it onto the canvas,
				centered to a certain point, and zooms it as specified
				
				This function will draw a certain part of an RPG::Image onto
				the canvas. You can omit the last four parameters to draw the
				whole image. You can specify horizontal and vertical
				magnification, and the image's center.
				
				The image's brightness will automatically be adjusted to match
				the canvas \c brightness. To avoid this, you can save the
				canvas \c brightness, set it to 100, draw the image and then
				restore the original \c brightness of the canvas.
				\param x X coordinate to draw to (center origin)
				\param y Y coordinate to draw to (center origin)
				\param zoomX Horizontal magnification (\c 1.0 is normal)
				\param zoomY Vertical magnification (\c 1.0 is normal)
				\param image %Image to draw
				\param srcX X coordinate of the area to use from the source
				image
				\param srcY Y coordinate of the area to use from the source
				image
				\param srcWidth Width of the area to draw (defaults to the
				whole image)
				\param srcHeight Height of the area to draw (defaults to the
				whole image)
				\warning This function is quite slow, especially if you draw
				an image which uses a mask color or an \c alpha value different
				from \c 255 or \c zoomX and/or \c zoomY values different from
				\c 1.0. Please also see the \ref optimization guidelines!
				\sa RPG::Canvas::drawStretched
				\sa RPG::Canvas::draw
			*/
			void drawCenteredZoomed(int x, int y, double zoomX, double zoomY, RPG::Image *image, int srcX = 0, int srcY = 0, int srcWidth = -1, int srcHeight = -1);
			
			/*! \brief Converts a 16-bit pixel value to 24 bit
				\param pixel16 16-bit pixel value (\c RRRRRGGGGGGBBBBB)
				\return 24-bit pixel value (\c
				00000000BBBBBBBBGGGGGGGGRRRRRRRR)
			*/
			static inline unsigned int convert16To24Bit(unsigned short pixel16) {
				unsigned int ret = (unsigned int)(pixel16 & 0xF800) >> 8;
				ret |= (unsigned int)(pixel16 & 0x7E0) << 5;
				ret |= (unsigned int)(pixel16 & 0x3F) << 19;
				return ret;
			}
			
			/*! \brief Converts a 16-bit pixel value to 32 bit with 100%
				opacity
				\param pixel16 16-bit pixel value (\c RRRRRGGGGGGBBBBB)
				\return 32-bit pixel value (\c
				11111111BBBBBBBBGGGGGGGGRRRRRRRR)
			*/
			static inline unsigned int convert16To32Bit(unsigned short pixel16) {
				return convert16To24Bit(pixel16) | 0xFF000000;
			}
			
			/*! \brief Converts a 24-bit pixel value to 16 bit
				\param pixel24 24-bit pixel value (\c
				00000000BBBBBBBBGGGGGGGGRRRRRRRR)
				\return 16-bit pixel value (\c RRRRRGGGGGGBBBBB)
			*/
			static inline unsigned short convert24To16Bit(unsigned int pixel24) {
				unsigned short ret = (unsigned short)(pixel24 & 0xF8) << 8;
				ret |= (unsigned int)(pixel24 & 0xFC00) >> 5;
				ret |= (unsigned int)(pixel24 & 0xF80000) >> 19;
				return ret;
			}
			
			/*! \brief Converts a 32-bit pixel value to 16 bit by combining it
				with an existing pixel
				
				The result is the 16-bit pixel value which is the result of
				drawing the \c pixel32 "on top" of the \c prevPixel16.
				\param pixel32 32-bit pixel value (\c AAAAAAAABBBBBBBBGGGGGGGGRRRRRRRR)
				\param prevPixel16 16-bit pixel value to use as basis for the alpha channel
				\return 16-bit pixel value (\c RRRRRGGGGGGBBBBB)
			*/
			static inline unsigned short convert32To16Bit(unsigned int pixel32, unsigned short prevPixel16) {
				// 16 = RRRRRGGGGGGBBBBB
				// 32 = AAAAAAAABBBBBBBBGGGGGGGGRRRRRRRR
				unsigned char alpha = (unsigned char)(pixel32 >> 24);
				if(!alpha) return convert24To16Bit(pixel32);
				#define c(v1, v2) ((v1) * (alpha + 1) + (v2) * (256 - alpha))
				unsigned short ret = (unsigned short)(c((unsigned char)(pixel32), (unsigned char)(prevPixel16 << 3))) >> 3;
				ret |= (unsigned short)(c((unsigned char)(pixel32 >> 8), (unsigned char)((prevPixel16 & 0x7E0) >> 3)) & 0xFC) << 3;
				ret |= (unsigned short)(c((unsigned char)(pixel32 >> 16), (unsigned char)((prevPixel16 & 0xF800) >> 8)) & 0xF8) << 8;
				#undef c
				return ret;
			}
	};
}
