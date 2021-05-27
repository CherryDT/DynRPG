#define DYNRPG_STATIC
#include "DynRPG.h"

namespace RPG {

#define defaultAccessorBGM(_member_, _function_) \
	Music *System::_function_() { \
		if(_member_->filename.s_str() == "") return dbSystem->_member_; \
		return _member_; \
	}

#define defaultAccessorSE(_member_, _function_) \
	Sound *System::_function_() { \
		if(_member_->filename.s_str() == "") return dbSystem->_member_; \
		return _member_; \
	}

#define defaultAccessorTrans(_member_, _function_) \
	Transition System::_function_() { \
		if(_member_ == TRANS_DEFAULT) return (Transition)dbSystem->_member_; \
		return (Transition)_member_; \
	}

	defaultAccessorBGM(titleBGM, getTitleBGM);
	defaultAccessorBGM(battleBGM, getBattleBGM);
	defaultAccessorBGM(victoryBGM, getVictoryBGM);
	defaultAccessorBGM(innBGM,getInnBGM);
	defaultAccessorBGM(skiffBGM, getSkiffBGM);
	defaultAccessorBGM(shipBGM, getShipBGM);
	defaultAccessorBGM(airshipBGM, getAirshipBGM);
	defaultAccessorBGM(gameOverBGM, getGameOverBGM);

	defaultAccessorSE(cursorSE, getCursorSE);
	defaultAccessorSE(decisionSE, getDecisionSE);
	defaultAccessorSE(cancelSE, getCancelSE);
	defaultAccessorSE(buzzerSE, getBuzzerSE);
	defaultAccessorSE(battleStartSE, getBattleStartSE);
	defaultAccessorSE(fleeSE, getFleeSE);
	defaultAccessorSE(enemyAttackSE, getEnemyAttackSE);
	defaultAccessorSE(enemyDamageSE, getEnemyDamageSE);
	defaultAccessorSE(heroDamageSE, getHeroDamageSE);
	defaultAccessorSE(evasionSE, getEvasionSE);
	defaultAccessorSE(enemyDeathSE, getEnemyDeathSE);
	defaultAccessorSE(itemSE, getItemSE);

	defaultAccessorTrans(teleportEraseTrans, getTeleportEraseTrans);
	defaultAccessorTrans(teleportShowTrans, getTeleportShowTrans);
	defaultAccessorTrans(battleStartEraseTrans, getBattleStartEraseTrans);
	defaultAccessorTrans(battleStartShowTrans, getBattleStartShowTrans);
	defaultAccessorTrans(battleEndEraseTrans, getBattleEndEraseTrans);
	defaultAccessorTrans(battleEndShowTrans, getBattleEndShowTrans);

#undef defaultAccessorBGM
#undef defaultAccessorSE
#undef defaultAccessorTrans

	void SystemGraphic::loadFont(std::string fontName) {
		void **rCanvas;
		DStringPtr s(fontName);
		asm volatile("call *%%esi" : "=a" (rCanvas) : "S" (0x4683AC), "a" (font) : "edx", "ecx", "cc", "memory"); // GetCanvas
		asm volatile("call *%%esi" : "=a" (_eax), "=d" (_edx) : "S" (0x4228CC), "a" (rCanvas[3]), "d" (s.str) : "ecx", "cc", "memory"); // TFont::SetName
	}
}
