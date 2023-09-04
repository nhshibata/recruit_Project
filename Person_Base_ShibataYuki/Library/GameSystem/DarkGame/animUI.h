//==========================================================
// [animUI.h]
//----------------------------------------------------------
// �쐬:2023/06/08
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
		//--- �����o�֐�
		CAnimeUI();
		CAnimeUI(CGameObject::Ptr ptr);

		virtual void Awake();
		void Init();
		virtual void Update();

		// *@��Ұ��݊J�n
		virtual void StartAnim();
		// *@��Ұ��ݒ����m�F
		virtual bool IsAnim();

	};

}

#endif // !__DARK_ANIMATION_UI_H__
