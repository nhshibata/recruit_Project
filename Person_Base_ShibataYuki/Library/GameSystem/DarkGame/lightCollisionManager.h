//==========================================================
// [lightCollisionManager.h]
//----------------------------------------------------------
// 作成:2023/06/01
//----------------------------------------------------------
// ライトの範囲確認クラスを管理する
//==========================================================

#ifndef __LIGHT_COLLISION_MANAGER_H__
#define __LIGHT_COLLISION_MANAGER_H__

#include <GameSystem/Component/component.h>

#pragma region ForwardDeclaration

namespace DarkGame 
{
	class CLightCollision;
}

#pragma endregion


namespace DarkGame
{
	using namespace MySpace::Game;

	class CLightCollisionManager : public CComponent
	{
	private:
		std::vector<CLightCollision*> m_aLights; // シーン中に整理する予定はないので、動的配列

	public:
		static constexpr LPCSTR TAG = "LightManager";

	public:
		//--- メンバ関数
		CLightCollisionManager();
		CLightCollisionManager(CGameObject::Ptr ptr);
		virtual ~CLightCollisionManager();

		void Awake();
		void Init();
		void Update();

		// *@追加
		void AddList(CLightCollision* add) { m_aLights.push_back(add); }

	};
}

#endif // !__LIGHT_COLLISION_MANAGER_H__
