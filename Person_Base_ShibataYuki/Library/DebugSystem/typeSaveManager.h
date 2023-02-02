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
#pragma warning(push)
#pragma warning(disable:4100)
			virtual std::string Set(CGameObject* obj) { return  std::string(); };
#pragma warning(pop)
		};

		// �h���׽
		// �^��ۑ�����
		template <class T1>
		class CTypeSave : public CTypeSaveBase
		{
		public:
			CTypeSave() {};
			~CTypeSave() {};

			virtual std::string Set(CGameObject* obj) { return obj->GetName(); };

			// ���|�C���^����
			T1* CreateType() { return new T1(); }

			// SP����
			std::shared_ptr<T1> MakeType() { return std::make_shared<T1>(); }
		};

		template <class T1>
		class CComponentTypeSave : public CTypeSave<T1>
		{
			std::string Set(CGameObject* obj)
			{
#pragma warning(suppress: 26444)
				obj->AddComponent<T1>();
				return typeid(T1).name();
			};
		};

		//--- �N���X��`
		class CTypeSaveManager
		{
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
			CTypeSaveManager() = default;
			~CTypeSaveManager();
		public:

			void Uninit();

			//--- �Q�b�^�[�E�Z�b�^�[
			// *@�V���O���g���̎擾
			static CTypeSaveManager* Get() { static CTypeSaveManager pInstance; return &pInstance; }

			// *@�i�[���ꂽ�׽�𖼑O�Ŏ擾
			CTypeSaveBase* GetTypeSave(std::string name);

			// *@�������Ă���^�C�v�̖��O���擾
			std::vector<std::string> GetTypeNameList();

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
			
			// *@�^�C�v���������Ă��邩
			bool IsType(std::string name);

			// *@SP����
			//template <class T>
			//std::shared_ptr<T> MakeType();

			// *@���|�C���^����
			//template <class T>
			//T* GetTypeSave();
		
		};
	}
}

//#endif // !BUILD_MODE

#endif // !__TYPE_SAVE_MANAGER_H__
