//==========================================================
// [animUI.h]
//----------------------------------------------------------
// 作成:2023/06/08
// 
//==========================================================

#ifndef __DARK_ANIMATION_UI_H__
#define __DARK_ANIMATION_UI_H__

#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CAnimeUI : public CComponent
	{
	protected:
		std::weak_ptr<CPolygonRenderer> m_pPolygon;
		bool m_bIsAnim;
		float m_fAnimationTime;

	public:
		//--- メンバ関数
		CAnimeUI();
		CAnimeUI(CGameObject::Ptr ptr);

		virtual void Awake();
		void Init();
		virtual void Update();

		// *@ｱﾆﾒｰｼｮﾝ開始
		virtual void StartAnim();
		// *@ｱﾆﾒｰｼｮﾝ中か確認
		virtual bool IsAnim();

	};

}

#endif // !__DARK_ANIMATION_UI_H__
