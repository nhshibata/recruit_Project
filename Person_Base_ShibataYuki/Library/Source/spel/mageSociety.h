//=========================================================
// [mageSociety.h] 
// �쐬:2022/09/29
//---------------------------------------------------------
// �L�����N�^�[�Ǘ�
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __MAGE_SOCIETY_COMPONENT_H__
#define __MAGE_SOCIETY_COMPONENT_H__

//--- �C���N���[�h��
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