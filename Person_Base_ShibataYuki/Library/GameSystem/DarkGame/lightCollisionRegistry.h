//==========================================================
// [lightCollisionRegistry.h]
//----------------------------------------------------------
// �쐬:2023/06/01
//----------------------------------------------------------
// ���C�g�͈̔͊m�F�p
// ��ʂȂ����肵����L���b�V������
//==========================================================

#ifndef __LIGHT_COLLISION_REGISTRY_H__
#define __LIGHT_COLLISION_REGISTRY_H__

#include <GameSystem/DarkGame/lightCollision.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CLightCollisionRegistry : public CLightCollision
	{
	private:
		std::vector<std::string> m_aHitTag; // �ڐG�����^�O���i�[

	public:
		//--- �����o�֐�
		CLightCollisionRegistry();
		CLightCollisionRegistry(CGameObject::Ptr ptr);
		virtual ~CLightCollisionRegistry();

		void Awake();
		void Update();

		// *@���̃N���A
		void Refresh();

		// *@�����𖞂����Ă��邩
		bool IsRange(std::string spec);

		// *@�L���b�V������̂��ړI�̂���
		// *@�����𖞂����Ă��Ă�true�͕Ԃ��Ȃ�
		bool Check(const CLightCollision& other);

	};

}

#endif // !__LIGHT_COLLISION_REGISTRY_H__
