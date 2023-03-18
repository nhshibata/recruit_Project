//=====================================================
// [collisionSystem.h]
// 作成:2022/11/9
//----------------------------
// 当たり判定実行ｸﾗｽ
// GameObjectManagerで実装していた機能を見やすくするためｸﾗｽ化
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
			//--- エイリアス
			using COLLISION_VEC = std::vector<std::weak_ptr<CCollision>>;	// 途中破棄された場合、参照しないためweak
		
		public:
			//--- メンバ関数
			CCollisionSystem();
			~CCollisionSystem();

			// *@当たり判定実行
			void CollisionCheck();

			// *@破棄(mapのため、整列はさせない)
			inline bool ExecutSystem(int idx)
			{
				return CMapSystemBase::ExecutSystem(idx);
			}
		};

	}
}

#endif //__GAME_OBJECT_MANAGER_H__

