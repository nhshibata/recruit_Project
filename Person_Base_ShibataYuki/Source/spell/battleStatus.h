//=========================================================
// [battleStatus.h] 
// 作成:2022/09/21
//---------------------------------------------------------
// キャラクターが所持する
//=========================================================

#ifndef __BATTLE_STATUS_COMPONENT_H__
#define __BATTLE_STATUS_COMPONENT_H__

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <CoreSystem/property.h>

namespace Spell
{
	// *@ステータスｸﾗｽ
	class CBattleStatus : public MySpace::Game::CComponent
	{
	private:
		//--- メンバ変数
		//std::vector<CPolygonRenderer*> m_pLifeRender;// 体力表示

	public:
		CProperty<int> Life;
		CProperty<int> Atk;		// 必要?
		CProperty<int> Damage;	// +値はダメージ、-は回復する

	public:
		//--- メンバ関数
		CBattleStatus() {};
		CBattleStatus(std::shared_ptr<MySpace::Game::CGameObject> owner) : MySpace::Game::CComponent(owner)
		{};
		~CBattleStatus() {};

		void Awake();
		void Init();
		void Update();
		void FixedUpdate();
		
		// *@確認
		inline bool IsLife() { return Life.accessor > 0; }
	};
}
#endif // !__COMMAND_COMPONENT_H__
