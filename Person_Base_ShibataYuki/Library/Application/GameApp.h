//=========================================================
// [GameApp.h]
//---------------------------------------------------------
//�쐬:2022/05/24
//---------------------------------------------------------
// �Q�[�����s�׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __GAME_APP_H__
#define __GAME_APP_H__

//--- �C���N���[�h��
#include <CoreSystem/Time/fps.h>

//--- �O���錾
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
			void BeginRender(Application*);
			void EndRender(Application*);
			
		public:
			CGameApp();
			~CGameApp();

			HRESULT Init(Application*);
			void Uninit(Application*)const;
			void FixedUpdate(Application*)const;
			void InputUpdate();
			void Run(Application*);
			void Draw(Application*);
		};
	}
}

#endif //!__GAME_APP__