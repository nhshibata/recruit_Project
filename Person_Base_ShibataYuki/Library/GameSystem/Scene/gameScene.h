//=========================================================
//�쐬:2022/04/20 (���j��)
// �V�[���׽ : �h���N���X
//=========================================================
//--- �C���N���[�h�K�[�h
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

//--- �C���N���[�h��
#include <GameSystem/Scene/scene.h>

namespace MySpace
{
	namespace SceneManager
	{
		//--- �N���X��`
		class CGameScene : public CScene
		{
		private:

		protected:
			//--- ���ފ֐�

		public:
			CGameScene();
			~CGameScene();
			void Init();
			void Uninit();
			void Update();
			void Draw();

		};
	}
}
#endif // !__GAME_SCENE_H__