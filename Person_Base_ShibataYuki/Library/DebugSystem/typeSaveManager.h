//=========================================================
// [typeSaveManager.h]
// �쐬�F2022/07/10
//
// �^��ۑ�����׽�B����ڰĂ��w�肷�邱�ƂŁA�^�̐��|�C���^��SP�̐������s��
// �݌v��A�L�^���邽�߂ɂ͐������Ȃ���΂Ȃ�Ȃ�
// ���܂��Y��ł͂Ȃ����A���N���X�ɉ��z�֐����`���Ȃ���΂����Ȃ�
//=========================================================

#ifndef __TYPE_SAVE_MANAGER_H__
#define __TYPE_SAVE_MANAGER_H__

//--- �C���N���[�h��
#include <CoreSystem/system.h>
#include <CoreSystem/Util/stl.h>
#include <DebugSystem/debug.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>		// gameobject�p ���ꂪ�Ȃ���cereal�ŃG���[���N����

//#ifdef BUILD_MODE

namespace MySpace
{
	namespace Debug
	{
		//--- �O���Q��
		class CTypeSaveManager;
	}
}

namespace MySpace
{
	namespace Debug
	{
		using MySpace::Game::CGameObject;

		// ���N���X
		class CTypeSaveBase
		{
		public:
			// ��qCTypeSaveManager���Ǘ����邽�߂̸׽
			CTypeSaveBase() {};
			virtual ~CTypeSaveBase() {};
			virtual void Set(CGameObject* obj) {};
		};

		// �h���׽
		// �^��ۑ�����
		template <class T1>
		class CTypeSave : public CTypeSaveBase
		{
		public:
			CTypeSave() {};
			~CTypeSave() {};

			virtual void Set(CGameObject* obj) {};

			// ���|�C���^����
			T1* CreateType() { return new T1(); }

			// SP����
			std::shared_ptr<T1> MakeType() { return std::make_shared<T1>(); }
		};

		template <class T1>
		class CComponentTypeSave : public CTypeSave<T1>
		{
			void Set(CGameObject* obj)
			{
				obj->AddComponent<T1>();
			};
		};

		//--- �N���X��`
		// �Ǘ��׽
		class CTypeSaveManager : public CAppSystem<CTypeSaveManager>
		{
			friend class CSingleton<CTypeSaveManager>;
			friend class CAppSystem<CTypeSaveManager>;
		private:
			//--- �G�C���A�X
			using StockMap = std::map<std::string, CTypeSaveBase*>;
			using StockPAIR = std::pair<std::string, CTypeSaveBase*>;
			using StockSharedMap = std::map<std::string, std::shared_ptr<CTypeSaveBase>>;
			using StockSharedPair = std::pair<std::string, std::shared_ptr<CTypeSaveBase>>;

		private:
			//--- �ÓI�����o�ϐ�
			StockMap g_StockType;
		private:
			CTypeSaveManager() {};
		public:
			//--- �����o�֐�
			~CTypeSaveManager();
			void Uninit();

			bool IsType(std::string name);

			// �i�[
			template <class T>
			void SetType() { 
				auto add = new CTypeSave<T>();
				g_StockType.insert(StockPAIR(typeid(T).name(), add)); }
			
			template <class T>
			void SetComponentSave() { 
				auto add = new CComponentTypeSave<T>();
				g_StockType.insert(StockPAIR(typeid(T).name(), add)); }
			
			// �i�[���ꂽ�ۑ��׽�̎擾
			CTypeSaveBase* GetTypeSave(std::string name) { return g_StockType[name]; }

			std::vector<std::string> GetTypeNameList();

			StockSharedMap Convert();

			// ���|�C���^����
			template <class T>
			T* GetTypeSave();

			// SP����
			template <class T>
			std::shared_ptr<T> MakeType();
		};
	}
}

//#endif // !BUILD_MODE

#endif // !__TYPE_SAVE_MANAGER_H__
