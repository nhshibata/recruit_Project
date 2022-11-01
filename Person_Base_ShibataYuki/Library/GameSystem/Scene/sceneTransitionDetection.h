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
			// *std::function�͌^�ȊO�͉��p������
			// *�֐��|�C���^:����ڰĸ׽�Ŏw��
			// *������:�ϒ�������

			// ����������
			template <class... Args>
			class CReturnVoidFunc 
			{
			protected:
				using Ptr = std::function<void*(Args...)>;
				// �֐��|�C���^
				std::function<void*(Args...)> m_Func;
			public:
				CReturnVoidFunc(std::function<void*(Args...)> func) { m_Func = func; }

				virtual void Call(Args... args) 
				{
					m_Func(args...);
				}
				//virtual void* GetAdress() { return nullptr; };
				void* GetAdress() { return &m_Func; }
			};
			// �֐��^����(�߂�lvoid*�Œ�)
			template <class T, class... Args>
			class CCallFunc : public CReturnVoidFunc<Args...>
			{
			public:
				// �֐��|�C���^�^
				typedef void* (T::*Func)(Args... args);
				
			public:
				CCallFunc(std::function<void*(Args...)> func):CReturnVoidFunc<Args...>(func){ }
				//void Call(Args... args) 
			};
			
		public:
			enum class EFuncType
			{
				CHANGED,
				LOADED,
				UNLOADED,
			};
			using PTR = std::function<void*(void)>;
			/*using ChangePtr = std::function<void*(CScene*, CScene*)>;
			using LoadPtr = std::function<void*(CScene*, int)>;
			using UnloadPtr = std::function<void*(CScene*)>;*/
			
		private:
			std::vector<CReturnVoidFunc<CScene*,CScene*>> m_pChangeFunc;
			std::vector<CReturnVoidFunc<CScene*,int>> m_pLoadFunc;
			std::vector<CReturnVoidFunc<CScene*>> m_pUnloadFunc;

		public:
			CSceneTransitionDetection();
			~CSceneTransitionDetection();

			void Call(CScene* prev, CScene* next);	// *�o�^���ꂽ�֐��̌Ăяo��
			void Call(CScene* scene, int);			// *�o�^���ꂽ�֐��̌Ăяo��
			void Call(CScene* scene);				// *�o�^���ꂽ�֐��̌Ăяo��

			// *�V�[���ؑ֎�
			// *����ڰĸ׽:�n�������֐��̌^
			template <class T>
			void Changed(void* (T::*func)(CScene*, CScene*), T* ptr)
			{
				m_pChangeFunc.push_back(CCallFunc<T, CScene*, CScene*>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}
			// *�V�[���ǂݍ��ݎ�
			// *����ڰĸ׽:�n�������֐��̌^
			template <class T>
			void Loaded(void* (T::*func)(CScene*, int), T* ptr)
			{
				m_pLoadFunc.push_back(CCallFunc<T, CScene*, int>(std::bind(func, ptr, std::placeholders::_1, std::placeholders::_2)));
			}
			// *�V�[���j����
			// *����ڰĸ׽:�n�������֐��̌^
			template <class T>
			void Unloaded(void* (T::*func)(CScene*), T* ptr)
			{
				m_pUnloadFunc.push_back(CCallFunc<T, CScene*>(std::bind(func, ptr, std::placeholders::_1)));
			}
			// *�o�^�����֐������O
			bool StopFunc(void* ptr);		
			
		};
	}
}

#endif // !__SCENE_TRANSITIONDETECTION_H__

