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
			void BeginRender(Application& app);
			void EndRender(Application& app);
			
		public:
			CGameApp();
			~CGameApp();

			void Init(Application& app);
			void Uninit(Application& app)const;
			void FixedUpdate(Application& app)const;
			void InputUpdate();
			void Update(Application& app)const;
			void Draw(Application& app);
		};
	}
}

#endif //!__GAME_APP__