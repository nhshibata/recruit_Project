//==========================================================
// [UIObj.h]
// 2022/06/07 �쐬
//----------------------------------------------------------
// TODO: ��肩��
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __UI_GAME_OBJECT_H__
#define __UI_GAME_OBJECT_H__

//---�C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>
#include <CoreSystem/Math/MyMath.h>
#include <GameSystem/Component/Transform/rectTransform.h>

namespace MySpace
{
	namespace Game
	{
		// <Summary>
		// UI�\���p�̃I�u�W�F�N�g
		// 2D�̂��߁ARectTransform������
		// </Summary>
		//--- �N���X��`
		class CUIGameObject final : public CGameObject
		{
		private:
			RectTransWeakPtr m_RectTransform;
		public:
			CUIGameObject();
			~CUIGameObject();

			virtual void Init();
			virtual void Update();
			virtual void LateUpdate();			// �Q�[���I�u�W�F�N�g���Ƃ̍X�V
			virtual void FixedUpdate();		// ��莞�Ԃ̍X�V
			virtual void Draw();
		};
	}
}
#endif // !__UI_OBJ_H__
