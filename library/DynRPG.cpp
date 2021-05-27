/*
	DynRPG v0.10
	RM2k3 Plugin SDK
	by David "Cherry" Trapp
	http://cherrytree.at
*/

#include <windows.h>
#define DYNRPG_STATIC
#include "DynRPG.h"
#include <stdio.h>
#include <sstream>

namespace RPG {
	void showError(std::string message, int eventId, int pageId, int lineId) {
		std::stringstream s;
		s << "An error has occured:\r\n" << message << "\r\n";
		if(eventId > 0) {
			s << "...at map event " << eventId << ", page " << pageId << ", line " << lineId + 1 << ".";
		} else if(eventId < 0) {
			s << "...at common event " << 0 - eventId << ", line " << lineId + 1 << ".";
		} else {
			s << "...at an unknown or battle event, line " << lineId + 1 << ".";
		}
		s << "\r\nContinue game?";
		HWND hWnd = NULL;
		if(RPG::screen) {
			try {
				hWnd = RPG::screen->getCanvasHWND();
				if(GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) hWnd = GetAncestor(hWnd, GA_ROOT);
			} catch(...) {

			}
			if(MessageBox(hWnd, s.str().c_str(), "DynRPG", MB_YESNO | MB_ICONWARNING) != IDYES && IsWindow(hWnd)) {
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
		}
	}

	void updateBattle() {
		asm volatile("call *%%esi" : "=a" (_eax) : "S" (0x497244), "a" (**sceneObjects[SCENE_BATTLE]) : "edx", "ecx", "cc", "memory");
	}

	void updateBattleEvents(BattleEventUpdateMode mode, Battler *battler) {
		asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx), "=c" (_ecx) : "S" (0x499800), "a" (**sceneObjects[SCENE_BATTLE]), "d" (battler), "c" (mode) : "cc", "memory");
	}

	int getSelectedMonsterIndex() {
		int *window = ((int **)**sceneObjects[SCENE_BATTLE])[15];
		if(!window[20]) return -1;
		return window[window[17] + 30];
	}

	std::string getSkillName(int id) {
		DStringPtr *p = (**reinterpret_cast<NamedCatalogPtr<DStringPtr *> **>(0x4CDBC4))[id];
		if(!p) return "";
		return p[2];
	}

	std::string getSkillDescription(int id) {
		DStringPtr *p = (**reinterpret_cast<NamedCatalogPtr<DStringPtr *> **>(0x4CDBC4))[id];
		if(!p) return "";
		return p[3];
	}

	std::string getItemName(int id) {
		DStringPtr *p = (**reinterpret_cast<NamedCatalogPtr<DStringPtr *> **>(0x4CDB14))[id];
		if(!p) return "";
		return p[2];
	}

	std::string getItemDescription(int id) {
		DStringPtr *p = (**reinterpret_cast<NamedCatalogPtr<DStringPtr *> **>(0x4CDB14))[id];
		if(!p) return "";
		return p[3];
	}

	std::string getConditionName(int id) {
		DStringPtr *p = (**reinterpret_cast<NamedCatalogPtr<DStringPtr *> **>(0x4CDE84))[id];
		if(!p) return "";
		return p[2];
	}

	std::map<std::string, std::string> loadConfiguration(char *sectionName, char *filename) {
		char *buffer = new char[32768];
		char *pKey = buffer;
		std::map<std::string, std::string> data;

		std::string sFilename(".\\");
		if(filename) {
			sFilename += filename;
		} else {
			sFilename += "DynRPG.ini";
		}
		GetPrivateProfileSection(sectionName, buffer, 32767, sFilename.c_str());

		while(*pKey) {
			std::string strKey = pKey;
			std::string::size_type nPos = strKey.find('=', 0);
			data[strKey.substr(0, nPos)] = strKey.substr(nPos + 1);
			pKey += strlen(pKey) + 1;
		}

		delete[] buffer;
		return data;
	}

	std::string encode(int value) {
		std::string ret;
		for(int i = 0; value != 0; i++) {
			ret.insert(0, 1, (value & 0x7F) | (i ? 0x80 : 0));
			value >>= 7;
		}
		return ret;
	}

	std::string encode(std::string value) {
		std::string ret = encode(value.length());
		for(std::string::iterator it = value.begin(); it < value.end(); it++) {
			ret += encode(*it);
		}
		return ret;
	}
}
