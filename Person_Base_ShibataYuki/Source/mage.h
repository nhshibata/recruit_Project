//=========================================================
// [mage.h] 
// �쐬:2022/09/29
//---------------------------------------------------------
// �L�����N�^�[
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __MAGE_COMPONENT_H__
#define __MAGE_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <battleStatus.h>
#include <commandSpell.h>

namespace Spell
{
	class CMage : public CComponent
	{
	public:
		static constexpr const char* TAG_NAME = "Mage";
	private:
		CCommandSpell* m_pCommand;
	public:
		CProperty<CBattleStatus*> Status;
		// UI�|�C���^
	public:
		//--- �����o�֐�
		CMage() {};
		CMage(CGameObject::Ptr owner) :CComponent(owner){};
		~CMage() {};

		void Awake();
		void Init();
		void Update();

		STSpell* GetSpell() { if (m_pCommand->Execution.accessor) { return m_pCommand->Present(); } return nullptr; }
	};
}

#endif // !__MAGE_COMPONENT_H__