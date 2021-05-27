#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {
	EventScriptLine *EventScriptData::line(int index) {
		if(lines && lines->list) return lines->list->get(index);
		return NULL;
	}
}
