//==========================================================
// [lightCollisionCondition.h]
//----------------------------------------------------------
// �쐬:2023/06/01
//----------------------------------------------------------
// ���C�g�͈̔͊m�F�p
// �������m�F���A�����ێ�����
//==========================================================

#ifndef __LIGHT_COLLISION_CONDITION_H__
#define __LIGHT_COLLISION_CONDITION_H__

#include <GameSystem/DarkGame/lightCollision.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CLightCollisionCondition : public CLightCollision
	{
	private:
		std::string m_SpecName;
		bool m_bHit;

	public:
		//--- �����o�֐�
		CLightCollisionCondition();
		CLightCollisionCondition(CGameObject::Ptr ptr);
		virtual ~CLightCollisionCondition();

		void Awake();
		void Update();

		// *@���̃N���A
		void Refresh();

		// *@�����𖞂����Ă��邩
		bool IsRange(std::string spec);

		// *@�����𖞂������ꍇ��true
		bool Check(const CLightCollision&);

		// *@�������ݒ�
		void SetCondition(const std::string name) { m_SpecName = name; }

	};

}

#endif // !__LIGHT_COLLISION_CONDITION_H__
