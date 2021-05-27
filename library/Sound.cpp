#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	Sound::Sound() {
		init();
		filename = "(OFF)";
		volume = 100;
		speed = 100;
		pan = 50;
	}

	Sound::Sound(Sound &ref) {
		init();
		set(ref);
	}

	Sound::Sound(std::string filename, int volume, int speed, int pan) {
		init();
		this->filename = filename;
		this->volume = volume;
		this->speed = speed;
		this->pan = pan;
	}

	Sound::~Sound() {
		filename = "";
	}

	void Sound::set(Sound &ref) {
		filename = ref.filename.str;
		volume = ref.volume;
		speed = ref.speed;
		pan = ref.pan;
	}

	void Sound::play() {
		if(filename == "(OFF)") {
			asm volatile("call *%%esi" : : "S" (0x471FBC) : "eax", "edx", "ecx", "cc", "memory"); // CallHarmonyStopSound
		} else {
			asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx) : "S" (0x48B538), "a" (system), "d" (this) : "ecx", "cc", "memory"); // CallHarmonyPlaySound
		}
	}

	void Sound::stop() {
		Sound().play();
	}

	void Sound::init() {
		vTable = (void **)0x475E14;
	}
}
