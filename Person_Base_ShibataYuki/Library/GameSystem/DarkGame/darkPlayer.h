//==========================================================
// [darkPlayer.h]
//----------------------------------------------------------
// 作成:2023/05/21
// 更新:2023/05/21 cpp実装
// 更新:2023/06/08 基底クラス変更
// 
//==========================================================

#ifndef __DRAK_PLAYER_H__
#define __DRAK_PLAYER_H__

#include <GameSystem/DarkGame/moveCharacter.h>
#include <GameSystem/DarkGame/lightCollisionRegistry.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CDarkPlayer : public CMoveCharacter
	{
	public:
		//--- メンバ変数
		static constexpr LPCSTR TAG = "Player";	// 取得用に公開する

	private:
		std::weak_ptr<CLightCollisionRegistry> m_pLightCol;

	public:
		//--- メンバ関数
		CDarkPlayer();
		CDarkPlayer(CGameObject::Ptr ptr);
		~CDarkPlayer();

		void Awake();
		void Update();
		void LateUpdate();


	};

}

#endif // !__DRAK_PLAYER_H__
