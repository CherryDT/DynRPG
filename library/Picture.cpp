#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	void Picture::merge() {
		if(image->pixels && image2->pixels) {
			image->draw(0, 0, image2, 0, 0, image->width, image->height, 0);
			image2->clear();
		}
	}

	void Picture::show(
		std::string filename, int x, int y, bool moveWithMap,
		int magnification, int transparency, int transparency2,
		bool useMaskColor, int red, int green, int blue, int chroma,
		PictureEffect effect, int effectStrength
	) {
		DStringPtr s(filename);
		asm volatile(
			"pushl %7;"
			"pushl %8;"
			"pushl %9;"
			"pushl %10;"
			"pushl %11;"
			"pushl %12;"
			"pushl %13;"
			"pushl %14;"
			"pushl %15;"
			"pushl %16;"
			"pushl %17;"
			"pushl %18;"
			"call *%%esi"
			: "=a" (_eax), "=d" (_edx), "=c" (_ecx) :
			"S" (0x4C1964), "a" (this), "d" (s.str), "c" (x), "m" (y), "m" (moveWithMap),
			"m" (magnification), "m" (transparency), "m" (transparency2),
			"m" (useMaskColor), "m" (red), "m" (green), "m" (blue), "m" (chroma),
			"m" (effect), "m" (effectStrength) : "cc", "memory"
		);
	}

	void Picture::move(
		int x, int y, int magnification, int transparency, int transparency2,
		int red, int green, int blue, int chroma, PictureEffect effect, int effectStrength,
		int duration
	) {
		volatile DStringPtr s("DUMMY"); // This is needed so that the function gets a stack frame, otherwise "m" will produce [esp+x] (sigh!) instead of [ebp+x], interfering with the push! [Added 2020-01-08]
		asm volatile(
			"pushl %7;"
			"pushl %8;"
			"pushl %9;"
			"pushl %10;"
			"pushl %11;"
			"pushl %12;"
			"pushl %13;"
			"pushl %14;"
			"pushl %15;"
			"pushl %16;"
			"call *%%esi"
			: "=a" (_eax), "=d" (_edx), "=c" (_ecx) :
			"S" (0x4C1AD8), "a" (this), "d" (x), "c" (y), "m" (magnification),
			"m" (transparency), "m" (transparency2),
			"m" (red), "m" (green), "m" (blue), "m" (chroma),
			"m" (effect), "m" (effectStrength), "m" (duration) : "cc", "memory"
		);
	}

	void Picture::erase() {
		filename = "";
		image->init(0, 0);
		image2->init(0, 0);
	}

	void Picture::update() {
		asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x4C1C20), "a" (this) : "edx", "ecx", "cc", "memory");
	}

	void Picture::draw() {
		asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x4C1E6C), "a" (this) : "edx", "ecx", "cc", "memory");
	}
}
