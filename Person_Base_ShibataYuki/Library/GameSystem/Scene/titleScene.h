//=========================================================
//�쐬:2022/04/20 (���j��)
// �V�[���׽ : �h���N���X
//=========================================================
//--- �C���N���[�h�K�[�h
#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

//--- �C���N���[�h��
#include <GameSystem/Scene/scene.h>

namespace MySpace
{
	namespace SceneManager
	{
		//--- �N���X��`
		class CTitleScene : public CScene
		{
		private:

		public:
			CTitleScene();
			~CTitleScene();
			void Init();
			void Uninit();
			void Update();
			void Draw();
		};
	}
}

#endif // !__TITLE_SCENE_H__

