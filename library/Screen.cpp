#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	HWND Screen::getCanvasHWND() {
		try {
			return (**reinterpret_cast<HWND ****>(0x4CE048))[25][96];
		} catch(...) {
			return NULL;
		}
	}

	void Screen::setFPS(int fps) {
		millisecondsPerFrame = 1000.0 / (double)fps;
		maxFPS = fps;
	}

	void Screen::update(Scene scene) {
		asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x48CD40), "a" (**sceneObjects[scene]) : "edx", "ecx", "cc", "memory");
	}
}
