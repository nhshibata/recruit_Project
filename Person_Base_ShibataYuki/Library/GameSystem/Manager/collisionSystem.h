//=====================================================
// [collisionSystem.h]
// �쐬:2022/11/9
//----------------------------
// �����蔻����s�׽
// GameObjectManager�Ŏ������Ă����@�\�����₷�����邽�߸׽��
//=====================================================

//--- �C���N���[�h�K�[�h
#ifndef __COLLISION_SYSTEM_H__
#define __COLLISION_SYSTEM_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/stl.h>
#include <GameSystem/Manager/mapSystemBase.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Game
	{
		class CCollision;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		class CCollisionSystem : public CMapSystemBase<std::weak_ptr<CCollision>>
		{
		private:
			//--- �G�C���A�X
			using COLLISION_VEC = std::vector<std::weak_ptr<CCollision>>;	// �r���j�����ꂽ�ꍇ�A�Q�Ƃ��Ȃ�����weak
		
		public:
			//--- �����o�֐�
			CCollisionSystem();
			~CCollisionSystem();

			// *@�����蔻����s
			void CollisionCheck();

			// *@�j��(map�̂��߁A����͂����Ȃ�)
			inline bool ExecutSystem(int idx)
			{
				return CMapSystemBase::ExecutSystem(idx);
			}
		};

	}
}

#endif //__GAME_OBJECT_MANAGER_H__

