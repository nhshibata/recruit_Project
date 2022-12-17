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
#include <CoreSystem/Singleton.h>
#include <CoreSystem/Util/stl.h>
#include <DebugSystem/debug.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>		// gameobject�p ���ꂪ�Ȃ���cereal�ŃG���[���N����

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
		class CTypeSaveManager : public CSingleton<CTypeSaveManager>
		{
			friend class CSingleton<CTypeSaveManager>;
		private:
			//--- �G�C���A�X
			using StockMap = std::map<std::string, CTypeSaveBase*>;
			using StockPAIR = std::pair<std::string, CTypeSaveBase*>;
			using StockSharedMap = std::map<std::string, std::shared_ptr<CTypeSaveBase>>;
			using StockSharedPair = std::pair<std::string, std::shared_ptr<CTypeSaveBase>>;

		private:
			//--- �����o�ϐ�
			StockMap m_aStockType;

		private:
			//--- �����o�֐�
			CTypeSaveManager() {};

		public:
			~CTypeSaveManager();
			void Uninit();

			//--- �Q�b�^�[�E�Z�b�^�[

			// *@�i�[���ꂽ�׽�𖼑O�Ŏ擾
			CTypeSaveBase* GetTypeSave(std::string name) { return m_aStockType[name]; }

			// *@�������Ă���^�C�v�̖��O���擾
			std::vector<std::string> GetTypeNameList();

			// *@���|�C���^����
			template <class T>
			T* GetTypeSave();

			// �i�[
			template <class T>
			void SetType() 
			{ 
				auto add = new CTypeSave<T>();
				m_aStockType.insert(StockPAIR(typeid(T).name(), add));
			}
			
			template <class T>
			void SetComponentSave() 
			{ 
				auto add = new CComponentTypeSave<T>();
				m_aStockType.insert(StockPAIR(typeid(T).name(), add));
			}
			
			// *@SP����
			template <class T>
			std::shared_ptr<T> MakeType();

			// *@�^�C�v���������Ă��邩
			bool IsType(std::string name);

		};
	}
}

//#endif // !BUILD_MODE

#endif // !__TYPE_SAVE_MANAGER_H__
