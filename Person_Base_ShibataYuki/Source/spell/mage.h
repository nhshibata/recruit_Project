//=========================================================
// [mage.h] 
// 作成:2022/09/29
//---------------------------------------------------------
// キャラクター
//=========================================================

//--- インクルードガード
#ifndef __MAGE_COMPONENT_H__
#define __MAGE_COMPONENT_H__

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <battleStatus.h>
#include <commandSpell.h>

namespace Spell
{
	using namespace MySpace::Game;

	class CMage : public CComponent
	{
	public:
		static constexpr const char* TAG_NAME = "Mage";
	private:
		CCommandSpell* m_pCommand;
	public:
		CProperty<CBattleStatus*> Status;
		// UIポインタ
	public:
		//--- メンバ関数
		CMage():m_pCommand(nullptr){};
		CMage(CGameObject::Ptr owner) :CComponent(owner), m_pCommand(nullptr) {};
		~CMage() {};

		void Awake();
		void Init();
		void Update();

		STSpell* GetSpell() { if (m_pCommand->Execution.accessor) { return m_pCommand->Present(); } return nullptr; }
	};
}

#endif // !__MAGE_COMPONENT_H__