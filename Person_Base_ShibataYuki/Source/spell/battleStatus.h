//=========================================================
// [battleStatus.h] 
// �쐬:2022/09/21
//---------------------------------------------------------
// �L�����N�^�[����������
//=========================================================

#ifndef __BATTLE_STATUS_COMPONENT_H__
#define __BATTLE_STATUS_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <CoreSystem/property.h>

namespace Spell
{
	// *@�X�e�[�^�X�׽
	class CBattleStatus : public MySpace::Game::CComponent
	{
	private:
		//--- �����o�ϐ�
		//std::vector<CPolygonRenderer*> m_pLifeRender;// �̗͕\��

	public:
		CProperty<int> Life;
		CProperty<int> Atk;		// �K�v?
		CProperty<int> Damage;	// +�l�̓_���[�W�A-�͉񕜂���

	public:
		//--- �����o�֐�
		CBattleStatus() {};
		CBattleStatus(std::shared_ptr<MySpace::Game::CGameObject> owner) : MySpace::Game::CComponent(owner)
		{};
		~CBattleStatus() {};

		void Awake();
		void Init();
		void Update();
		void FixedUpdate();
		
		// *@�m�F
		inline bool IsLife() { return Life.accessor > 0; }
	};
}
#endif // !__COMMAND_COMPONENT_H__
