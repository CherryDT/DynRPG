#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	int Map::getCameraX() {
		return cameraSubX / 16;
	}

	int Map::getCameraY() {
		return cameraSubY / 16;
	}

	void Map::setCamera(int x, int y) {
		x *= 16;
		y *= 16;
		int *p = (**(int ***)(0x4CDE54));
		p[43] += x - cameraSubX;
		p[44] += y - cameraSubY;
		p[45] += x - cameraSubX;
		p[46] += y - cameraSubY;
	}

	void Map::moveCamera(int offsetX, int offsetY, int speed) {
		int *p = (**(int ***)(0x4CDE54));
		p[45] += offsetX * 16;
		p[46] += offsetY * 16;
		p[47] = speed;
	}

	int Map::getWidth() {
		return (**(int ***)(0x4CDD14))[5];
	}

	int Map::getHeight() {
		return (**(int ***)(0x4CDD14))[6];
	}

	void Map::updateEvents() {
		asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x4AB8B4), "a" (events.ptr) : "edx", "ecx", "cc", "memory");
	}
}
