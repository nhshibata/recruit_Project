//==========================================================
// [enemyBase.h]
//----------------------------------------------------------
// 作成:2023/05/19 
// 更新:2023/06/08 基底クラスを変更
// 
//==========================================================

#ifndef __DARK_ENEMY_BASE_H__
#define __DARK_ENEMY_BASE_H__

#include <GameSystem/DarkGame/moveCharacter.h>
#include <GameSystem/DarkGame/lightCollisionCondition.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CEnemyBase : public CMoveCharacter
	{
	public :
		static constexpr LPCSTR TAG = "Enemy";	// 取得用に公開する

	private:
		std::weak_ptr<CLightCollisionCondition> m_pLightCol;

	public :
		//--- メンバ関数
		CEnemyBase();
		CEnemyBase(CGameObject::Ptr ptr);
		virtual ~CEnemyBase();
		
		virtual void Awake();
		void Update();
		virtual void LateUpdate();

		// *@範囲確認
		bool IsRange();

		// *@発見時処理
		virtual void DiscoveryEffect();
	};

}

#endif // !__DARK_ENEMY_BASE_H__
