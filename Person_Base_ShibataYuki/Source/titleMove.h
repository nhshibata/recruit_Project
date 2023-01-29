//========================================================
// [titleMove.h]
//--------------------------------------------------------
// �쐬:2023/01/10 
//--------------------------------------------------------
// 
//========================================================

#ifndef __TITLE_MOVE_H__
#define __TITLE_MOVE_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/polygonRenderer.h>

namespace Spell
{
	using namespace MySpace::Game;

	class CTitleMove : public CComponent
	{
	private:
		std::weak_ptr<CPolygonRenderer> m_pPolygon;
		std::vector<Vector2> m_aCurveList;
		Vector2 m_vStartPos;

	public:
		CTitleMove();
		CTitleMove(CGameObject::Ptr owner);
		~CTitleMove();

		// *@�������Ăяo��
		virtual void Awake();
		void Init()override;

		void Update();

		void SetStartPos(const Vector2 pos);
	};

}

#endif // !__TITLE_MOVE_H__
