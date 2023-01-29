//=========================================================
// [mageSociety.h] 
// 作成:2022/09/29
//---------------------------------------------------------
// キャラクター管理
//=========================================================

//--- インクルードガード
#ifndef __MAGE_SOCIETY_COMPONENT_H__
#define __MAGE_SOCIETY_COMPONENT_H__

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>

namespace Spell
{
	class CMage;
};

namespace Spell
{
	using namespace MySpace::Game;

	class CMageSociety : public CComponent
	{
	public:
		static constexpr const char* TAG_NAME = "MageSociety";

	private:
		std::vector<std::weak_ptr<CMage>> m_spMages;

	public:
		CMageSociety() {}
		CMageSociety(CGameObject::Ptr owner):CComponent(owner) {}
		~CMageSociety() {}

		void Awake();
		void Init();
		void Update();
		void FixedUpdate();
	};

};
#endif //!__MAGE_SOCIETY_COMPONENT_H__