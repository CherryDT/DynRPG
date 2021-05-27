#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	Music::Music() {
		init();
		filename = "(OFF)";
		fadeInTime = 0;
		volume = 100;
		speed = 100;
		pan = 50;
	}

	Music::Music(Music &ref) {
		init();
		set(ref);
	}

	Music::Music(std::string filename, int fadeInTime, int volume, int speed, int pan) {
		init();
		this->filename = filename;
		this->fadeInTime = fadeInTime;
		this->volume = volume;
		this->speed = speed;
		this->pan = pan;
	}

	Music::~Music() {
		filename = "";
	}

	void Music::set(Music &ref) {
		filename = ref.filename.str;
		fadeInTime = ref.fadeInTime;
		volume = ref.volume;
		speed = ref.speed;
		pan = ref.pan;
	}

	void Music::play() {
		asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx), "=c" (_ecx) : "S" (0x48B408), "a" (system), "d" (this), "c" (0) : "cc", "memory"); // CallHarmonyPlayMusic
		if(system->currentBGM) system->currentBGM->set(*this);
		if(system->scene == SCENE_MAP && system->mapBGM) system->mapBGM->set(*this);
	}

	void Music::stop() {
		Music().play();
	}

	void Music::fadeOut(int time) {
		asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx) : "S" (0x48B4E0), "a" (system), "d" (time) : "ecx", "cc", "memory"); // CallHarmonyFadeOutMusic
	}

	void Music::init() {
		vTable = (void **)0x475E98;
	}
}
