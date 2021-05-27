#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	std::string Actor::getDegree() {
		if(degree.s_str() == "\x01") return dbActors[id]->degree;
		return degree;
	}

	std::string Actor::getCharsetFilename() {
		if(charsetFilename.s_str() == "") return dbActors[id]->charsetFilename;
		return charsetFilename;
	}

	int Actor::getCharsetId() {
		if(charsetFilename.s_str() == "") return dbActors[id]->charsetId;
		return charsetId;
	}

	int Actor::getCharsetTransparency() {
		if(charsetFilename.s_str() == "") return dbActors[id]->charsetTransparent ? 3 : 0;
		return charsetTransparency;
	}

	std::string Actor::getFacesetFilename() {
		if(facesetFilename.s_str() == "") return dbActors[id]->facesetFilename;
		return facesetFilename;
	}

	int Actor::getFacesetId() {
		if(facesetFilename.s_str() == "") return dbActors[id]->facesetId;
		return facesetId;
	}

	int Actor::getBattleGraphicId() {
		if(battleGraphicId == 0) return dbActors[id]->battleGraphicId;
		return battleGraphicId;
	}

	int Actor::getBattleCommand(int index) {
		if(!customBattleCommands) return dbActors[id]->battleCommands[index];
		return battleCommands[index];
	}

	Actor *Actor::partyMember(int index) {
		Actor *ret = NULL;
		asm volatile("call *%%esi" : "=a" (ret), "=d" (_edx) : "S" (0x4A6014), "a" (**(void ***)0x4CDB74), "d" (index) : "ecx", "cc", "memory"); // GetPartyMember
		return ret;
	}
}
