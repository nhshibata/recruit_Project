//=========================================================
//�쐬:2022/05/24
// �Q�[�����s�׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __GAME_APP_H__
#define __GAME_APP_H__

//--- �C���N���[�h��
#include <CoreSystem/Time/fps.h>

// �O���錾
class Application;

namespace MySpace
{
	namespace System
	{
		//--- �N���X��`
		class CGameApp
		{
		private:
			//--- �����o�֐�
			void BeginRender();
			void EndRender();
			
		public:
			CGameApp();
			~CGameApp();

			void Init();
			void Uninit()const;
			void FixedUpdate()const;
			void InputUpdate();
			void Update();
			void Draw();
		};
	}
}

#endif //!__GAME_APP__