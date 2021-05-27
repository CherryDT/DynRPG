#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	unsigned short &Canvas::pixel(int x, int y) {
		return getScanline(y)[x];
	}

	unsigned short *Canvas::getScanline(int y) {
		unsigned short *ret;
		asm volatile("call *%%esi" : "=a" (ret), "=d" (_edx) : "S" (0x4683B8), "a" (this), "d" (y) : "ecx", "cc", "memory");  // GetAuroraBoardPixelLineAddress
		return ret;
	}

	int Canvas::width() {
		int ret;
		asm volatile("call *%%esi" : "=a" (ret) : "S" (0x4680CC), "a" (this) : "edx", "ecx", "cc", "memory");  // GetBoardBitmapWidth
		return ret;
	}

	int Canvas::height() {
		int ret;
		asm volatile("call *%%esi" : "=a" (ret) : "S" (0x4680D8), "a" (this) : "edx", "ecx", "cc", "memory");  // GetBoardBitmapHeight
		return ret;
	}

	void Canvas::draw(int x, int y, Image *image, int srcX, int srcY, int srcWidth, int srcHeight) {
		if(srcWidth == -1) srcWidth = image->width;
		if(srcHeight == -1) srcHeight = image->height;
		RECT rc;
		rc.left = srcX;
		rc.top = srcY;
		rc.right = srcX + srcWidth;
		rc.bottom = srcY + srcHeight;
		asm volatile(
			"pushl %7;"
			"pushl %8;"
			"call *%%esi;"
			: "=a" (_eax), "=d" (_edx), "=c" (_ecx) : "S" (0x4684C0), "a" (this), "d" (x), "c" (y), "m" (image), "D" (&rc) : "cc", "memory"
		);
		// Changed constraints to "m" for image and "D" for &rc because the new GCC would assign &rc to ESP even though I had put "g"...
		// I think it's a bug, and it messes up everything because it generates a "push esp"!
	}

	void Canvas::drawStretched(int x, int y, int width, int height, Image *image, int srcX, int srcY, int srcWidth, int srcHeight) {
		if(srcWidth == -1) srcWidth = image->width;
		if(srcHeight == -1) srcHeight = image->height;
		if(width == srcWidth && height == srcHeight) {
			draw(x, y, image, srcX, srcY, srcWidth, srcHeight);
			return;
		}
		RECT rc1;
		RECT rc2;
		rc1.left = srcX;
		rc1.top = srcY;
		rc1.right = srcX + srcWidth;
		rc1.bottom = srcY + srcHeight;
		rc2.left = x;
		rc2.top = y;
		rc2.right = x + width;
		rc2.bottom = y + height;
		asm volatile(
			"pushl %7;"
			"call *%%esi;"
			: "=a" (_eax), "=d" (_edx), "=c" (_ecx) : "S" (0x4688B0), "a" (this), "d" (&rc2), "c" (image), "D" (&rc1) : "cc", "memory"
		);
		// See comment above about "D"
	}

	void Canvas::drawCenteredZoomed(int x, int y, double zoomX, double zoomY, Image *image, int srcX, int srcY, int srcWidth, int srcHeight) {
		if(srcWidth == -1) srcWidth = image->width;
		if(srcHeight == -1) srcHeight = image->height;
		if(zoomX == 1.0 && zoomY == 1.0) {
			draw(x - (srcWidth + 1) / 2, y - (srcHeight + 1) / 2, image, srcX, srcY, srcWidth, srcHeight);
			return;
		}
		int newWidth = (int)((double)srcWidth * zoomX + 0.5);
		int newHeight = (int)((double)srcHeight * zoomY + 0.5);
		drawStretched(x - (newWidth + 1) / 2, y - (newHeight + 1) / 2, newWidth, newHeight, image, srcX, srcY, srcWidth, srcHeight);
	}
}
