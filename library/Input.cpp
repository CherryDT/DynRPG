#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	int *Input::key(Key keyId) {
		if(keyId >= KEY_0) {
			return keys2k3 + (keyId - KEY_0) * 8;
		} else {
			return keys + keyId * 8;
		}
	}

	bool Input::pressed(Key keyId) {
		int *k = key(keyId);
		for(int i = 0; i < 8; i++) {
			if(GetAsyncKeyState(k[i]) & 0x8000) return true;
		}
		return false;
	}

	void Input::update() {
		asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x4974F4), "a" (this) : "edx", "ecx", "cc", "memory");
	}
}
