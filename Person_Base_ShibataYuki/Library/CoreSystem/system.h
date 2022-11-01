//=========================================================
// [system.h]
// �쐬�F2022/07/29
//----------------------
// �V���O���g���ɂ��Ǘ�����A�v���P�[�V�����׽����̈ꊇ�Ǘ��ڍs�̂��߂̸׽
//=========================================================

#ifndef __SYSTEM_BASE_H__
#define __SYSTEM_BASE_H__

#include <Application/Application.h>
#include <Application/systemManager.h>
#include <string>

namespace MySpace
{
	namespace System
	{
		template <typename T>
		class CAppSystem : public CSingleton<T>
		{
			friend class CSingleton<T>;
		private:
			//--- �����o�ϐ�
			Application* m_pApp;
		public:
			//CProperty<CSystemManager*> MainSystem;

		protected:
			//--- �����o�֐�
			CAppSystem() 
			{ 
			};
			//~CAppSystem() {};

		public:
			void SetApp(Application* app) { m_pApp = app; }
			Application* GetApp() { return m_pApp; }

			// �ǂݍ��݁E�������ݏ����֐�
			// �R���X�g���N�^�A���邢�͔C�ӂ̃^�C�~���O�ŌĂяo��
			virtual void LoadSystem() {};
			virtual void SaveSystem() {};
			static void SetUninit() { CAppSystem<T>::Get().Uninit(); };
		};
	}

	// �^�F���p (�Y�ꂪ���Ȃ���)
	using MySpace::System::CAppSystem;

#if 0
	namespace System
	{
		//class CAppSystem
		class CApp
		{
			friend class Application;
		protected:
			Application* m_pApp;

			//--- �����o�֐�
			CApp():m_pApp(nullptr) {};
			CApp(Application* app):m_pApp(app) {};
			virtual ~CApp() {};
		public:
			virtual HRESULT Init();
			virtual void Uninit();
		};
	}


#endif // 0

}
#endif // !__SYSTEM_BASE_H__

