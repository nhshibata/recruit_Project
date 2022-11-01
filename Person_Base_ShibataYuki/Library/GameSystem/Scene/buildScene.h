//=========================================================
// �V�[���׽ : �h���N���X
//=========================================================
//--- �C���N���[�h�K�[�h
#ifndef __BUILD_SCENE_H__
#define __BUILD_SCENE_H__

//--- �C���N���[�h��
#include <GameSystem/Scene/scene.h>

namespace MySpace
{
	namespace SceneManager
	{
		//--- �N���X��`
		class CBuildScene : public CScene
		{
		private:

		public:
			CBuildScene();
			CBuildScene(std::string name);
			~CBuildScene();
			void Init();
			void Uninit();
			void Update();
			void Draw();
		};
	}
}

#endif // !__BUILD_SCENE_H__

