#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	int Character::getScreenX() {
		int ret;
		asm volatile("call *%%esi" : "=a" (ret) : "S" (0x4C46E8), "a" (this) : "edx", "ecx", "cc", "memory");
		return ret;
	}

	int Character::getScreenY() {
		int ret;
		asm volatile("call *%%esi" : "=a" (ret) : "S" (0x4C47C4), "a" (this) : "edx", "ecx", "cc", "memory");
		return ret;
	}

	void Character::setFixedStep(int newStep) {
		stepFrameCounter = -1;
		step = newStep;
	}

	void Character::setNormalStep() {
		stepFrameCounter = 0;
		step = 3;
	}

	void Character::setAnimationType(AnimationType type) {
		animationType = type;
		fixedDirection = type == ANI_FIXED_DIR_NORMAL || type == ANI_FIXED_DIR_STEPPING || type == ANI_FIXED_GRAPHIC;
	}

	bool Character::isMovePossible(int fromX, int fromY, int toX, int toY) {
		bool ret;
		asm volatile(
			"pushl %7;"
			"pushl %8;"
			"call *%%esi"
			: "=a" (ret), "=d" (_edx), "=c" (_ecx) : "S" (vTable[11]), "a" (this), "d" (fromX), "c" (fromY), "m" (toX), "m" (toY) : "cc", "memory"
		);
		return ret;
	}

	std::string Character::getName() {
		if(vTable == reinterpret_cast<void **>(0x4AAC8C)) {
			return reinterpret_cast<Event *>(this)->eventData->name;
		} else if(this == hero) {
			Actor *firstHero = Actor::partyMember(0);
			if(firstHero) return firstHero->getName(); else return "";
		} else {
			return "";
		}
	}

	void Character::move(const char* data, int length, bool repeatPattern, bool ignoreImpossible, int frequency) {
		asm volatile(
			"pushl %7;"
			"pushl %8;"
			"pushl %9;"
			"call *%%esi"
			: "=a" (_eax), "=d" (_edx), "=c" (_ecx) : "S" (0x4C43EC), "a" (this), "d" (data), "c" (length), "m" (frequency), "m" (repeatPattern), "m" (ignoreImpossible) : "cc", "memory"
		);
		// Constraints for repeatPattern and ignoreImpossible are "m" and not "g" because they are bool and trying to push an 8-bit register won't work
		// Also, frequency is now "m" too because of possible "g" bugs
	}

	void Character::stop() {
		asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x4C445C), "a" (this) : "edx", "ecx", "cc", "memory");
	}

	void Character::doStep(Direction direction) {
		asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx) : "S" (vTable[12]), "a" (this), "d" (direction) : "ecx", "cc", "memory");
	}

	bool Event::doesEventPageExist(int id) {
		void **script;
		asm volatile("call *%%esi" : "=a" (script), "=d" (_edx) : "S" (0x47A0D0), "a" ((**reinterpret_cast<void ****>(0x4CDD14))[15]), "d" (this->id) : "ecx", "cc", "memory");
		NamedCatalogPtr<void *> pages;
		pages.ptr = reinterpret_cast<NamedCatalog<void *> *>(script[5]);
		return pages[id] != NULL;
	}

	HeroControlMode Hero::getControlMode() {
		unsigned char &a1 = *(unsigned char*)0x4A9B87;
		unsigned char &a2 = *(unsigned char*)0x4A9A04;
		if(a2 == 0xC3) return CONTROL_NOTHING;
		if(a1 == 0x7F) return CONTROL_EVERYTHING_EXCEPT_MOVEMENT;
		return CONTROL_EVERYTHING;
	}

	void Hero::setControlMode(HeroControlMode controlMode) {
		unsigned char &a1 = *(unsigned char*)0x4A9B87;
		unsigned char &a2 = *(unsigned char*)0x4A9A04;
		switch(controlMode) {
			case CONTROL_EVERYTHING:
				a1 = 0x02;
				a2 = 0x53;
				break;
			case CONTROL_EVERYTHING_EXCEPT_MOVEMENT:
				a1 = 0x7F;
				a2 = 0x53;
				break;
			case CONTROL_NOTHING:
				a1 = 0x7F;
				a2 = 0xC3;
				break;
		}
	}
}
