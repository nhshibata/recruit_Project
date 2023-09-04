//==========================================================
// [lightCollisionRegistry.h]
//----------------------------------------------------------
// 作成:2023/06/01
//----------------------------------------------------------
// ライトの範囲確認用
// 種別なく判定したらキャッシュする
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
		std::vector<std::string> m_aHitTag; // 接触したタグを格納

	public:
		//--- メンバ関数
		CLightCollisionRegistry();
		CLightCollisionRegistry(CGameObject::Ptr ptr);
		virtual ~CLightCollisionRegistry();

		void Awake();
		void Update();

		// *@情報のクリア
		void Refresh();

		// *@条件を満たしているか
		bool IsRange(std::string spec);

		// *@キャッシュするのが目的のため
		// *@条件を満たしていてもtrueは返さない
		bool Check(const CLightCollision& other);

	};

}

#endif // !__LIGHT_COLLISION_REGISTRY_H__
