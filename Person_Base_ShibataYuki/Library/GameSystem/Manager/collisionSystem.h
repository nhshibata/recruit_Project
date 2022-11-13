//=====================================================
// [collisionSystem.h]
// 作成:2022/11/9
//----------------------------
//
//=====================================================

//--- インクルードガード
#ifndef __COLLISION_SYSTEM_H__
#define __COLLISION_SYSTEM_H__

//--- インクルード部
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
			using COLLISION_VEC = std::vector<std::weak_ptr<CCollision>>;				// 途中破棄された場合、参照しないためweak
			//using COLLISION_MAP = std::unordered_map<int, std::weak_ptr<CCollision>>;	// 途中破棄された場合、参照しないためweak
		private:
		
		public:
			CCollisionSystem();
			~CCollisionSystem();

			// *当たり判定用関数
			void CollisionCheck();
		};

	}
}

#endif //__GAME_OBJECT_MANAGER_H__

