//==========================================================
// [sceneTransitionDetection.h]
// �쐬:2022/08/20
//----------------------------------------------------------
// �V�[������Ăяo�������֐��|�C���^��o�^����׽
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __SCENE_TRANSITION_DETECTION_H__
#define __SCENE_TRANSITION_DETECTION_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/FuncPtr/funcPtr.h>

namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
}

namespace MySpace
{
	namespace SceneManager
	{
		// *�֐��|�C���^�Ǘ��׽
		class CSceneTransitionDetection
		{
		private:
			// *@std::function�͌^�ȊO�͉��p������
			// *@�֐��|�C���^:����ڰĸ׽�Ŏw��
			// *@������:�ϒ�������

			// ����������N���X
			template <class... Args>
			class CFunctionBinder 
			{
			protected:
				//--- �����o�ϐ�
				std::function<void*(Args...)> m_Func;
			public:
				// *@�R���X�g���N�^
				CFunctionBinder(std::function<void*(Args...)> func)
					:m_Func(func)
				{}

				// *@�֐��Ăяo��
				virtual void Call(Args... args) 
				{
					m_Func(args...);
				}
				// *@�A�h���X�擾
				void* GetAdress() { return &m_Func; }
			};
			// �֐��^����N���X(�߂�lvoid*�Œ�)
			template <class T, class... Args>
			class CFunctionBinderCall : public CFunctionBinder<Args...>
			{
			public:
				// *@�R���X�g���N�^
				CFunctionBinderCall(std::function<void*(Args...)> func)
					:CFunctionBinder<Args...>(func)
				{}
			};
			
		public:
			//--- ��`
			using PTR = std::function<void*(void)>;
			
		private:
			//--- �����o�ϐ�
			std::vector<CFunctionBinder<CScene*,CScene*>> m_pChangeFunc;
			std::vector<CFunctionBinder<CScene*,int>> m_pLoadFunc;
			std::vector<CFunctionBinder<CScene*>> m_pUnloadFunc;

		public:
			//--- �����o�֐�
			CSceneTransitionDetection();
			~CSceneTransitionDetection();

			// *@�o�^���ꂽ�֐��Ăяo��
			// *@�V�[���J��
			void Call(CScene* prev, CScene* next);

			// *@�o�^���ꂽ�֐��̌Ăяo��
			// *@�V�[���ǂݍ��ݎ�
			void Call(CScene* scene, int);

			// *@�o�^���ꂽ�֐��̌Ăяo��
			// *@�V�[���j����
			void Call(CScene* scene);				

			// *@�V�[���ؑ֎�
			// *@����ڰĸ׽:�n�������֐��̌^
			template <class T>
			void Changed(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_pChangeFunc.push_back(CFunctionBinderCall<T, CScene*, CScene*>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}

			// *@�V�[���ǂݍ��ݎ�
			// *@����ڰĸ׽:�n�������֐��̌^
			template <class T>
			void Loaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_pLoadFunc.push_back(CFunctionBinderCall<T, CScene*, int>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}

			// *@�V�[���j����
			// *@����ڰĸ׽:�n�������֐��̌^
			template <class T>
			void Unloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_pUnloadFunc.push_back(CFunctionBinderCall<T, CScene*>(std::bind(func, ptr, std::placeholders::_1)));
			}

			// *@�o�^�����֐����O
			bool StopFunc(void* ptr);		
			
		};
	}
}

#endif // !__SCENE_TRANSITIONDETECTION_H__

