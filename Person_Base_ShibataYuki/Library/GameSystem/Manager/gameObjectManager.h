//==========================================================
// [gameObjectManager.h]
// �h���N���X
// 2022/06/07 �쐬
// �׽���ύX���邩��
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

//--- �C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>
#include <CoreSystem/Util/stl.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CGameObjectManager
		{
			friend class CGameObject;
		public:
			//--- �G�C���A�X
			using GameObjList = std::list< std::shared_ptr<CGameObject> >;
			using WeakList = std::list< std::weak_ptr<CGameObject> >;
			
			struct gameObjWeakList 
			{
				WeakList list;
				typename std::list<std::weak_ptr<CGameObject>>::iterator FindObj(std::weak_ptr<CGameObject> obj)
				{
					auto it = list.begin();
					for (; it != list.end(); ++it)
					{
						if ((*it).lock() == obj.lock())
						{
							return it;
						}
					}
					return list.end();
				}
			};
			//--- �G�C���A�X
			// *@tag�����p
			using TagObjMap = std::unordered_map<std::string, gameObjWeakList>;

		private:
			GameObjList m_aGameObjList;					// ���ۂ̏���
			GameObjList m_aAddObjList;					// �� �ɒǉ�����I�u�W�F�N�g�i�[�p
			GameObjList m_aDontDestroyList;				// shared_ptr�̐����𗘗p���āA�V�[���J�ڎ��ɓn����j�����X�g
			TagObjMap m_aTagMap;						// tag�����p
			std::weak_ptr<CScene> m_pAffiliationScene;

		private:
			// *@�ǉ��҂����X�g��z��ɒǉ�
			bool ObjectListUpdate();

			// *@�z��֒ǉ�
			void SetGameObject(std::shared_ptr<CGameObject> obj);
			// *@tagMap�ړ�
			void TagMove(std::string NextTag, std::weak_ptr<CGameObject> obj);
			// *@�����V�[���擾
			std::weak_ptr<CScene> GetScene() 
			{ 
				return m_pAffiliationScene; 
			};
			
		public:
			CGameObjectManager(std::shared_ptr<CScene> scene);
			~CGameObjectManager();
			
			//--- ��v����
			void Init();
			void Uninit();
			void Update();
			void UpdateInDebug();
			void FixedUpdate();
			void AllUninit()
			{
				m_aGameObjList.clear();
				m_aAddObjList.clear();
				m_aDontDestroyList.clear();
			}

			// *@�K�v�Œ���̃Q�[���I�u�W�F�N�g�쐬
			void CreateBasicObject();

			// *@�O���ŃQ�[���I�u�W�F�N�g����肽�����ɌĂяo���֐�
			// *@���������l����Ɛ����Ə������͕������� + �z��ɉ����邽��
			// *@������:CGameObject(��߰���s��)
			std::shared_ptr<CGameObject> CreateGameObject(CGameObject* pObj = nullptr);

			// *@�I�u�W�F�N�g�̔j��
			bool DestroyObject(std::weak_ptr<CGameObject> pObj);

			// *@�I�u�W�F�N�g�ǉ��҂��ɒǉ�
			inline void AddGameObject(std::shared_ptr<CGameObject> obj) 
			{
				m_aAddObjList.push_back(obj); 
			}

			//--- �Q�b�^�[�E�Z�b�^�[
			// @�������X�g(������int��weak)
			inline GameObjList GetList() 
			{
				return m_aGameObjList; 
			}
			// *@�������X�g(weak�p)
			inline WeakList GetList(int)
			{ 
				WeakList ret;
				for (auto & obj : m_aGameObjList)
					ret.push_back(obj);
				return ret;
			}

			// *@�I�u�W�F�N�g�㏑��
			inline void SetObjList(std::list<std::shared_ptr<CGameObject>> list)
			{
				m_aGameObjList = list;
			}

			// *@�I�u�W�F�N�g����(���O)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObj(std::string name)
			{
				for (GameObjList::iterator it = m_aGameObjList.begin(); it != m_aGameObjList.end(); ++it) 
				{
					if ((*it).get()->GetName() == name)
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *@�I�u�W�F�N�g����(�^�O��)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObjWithTag(std::string tag)
			{
				if (m_aTagMap.count(tag) == 0)
				{
					//m_aTagMap[tag] = gameObjWeakList();
					return std::weak_ptr<CGameObject>();
				}
				return m_aTagMap[tag].list.begin()->lock();
			}
			// *@�I�u�W�F�N�g����(��޸׽)
			_NODISCARD std::weak_ptr<CGameObject> FindGameObjWithTag(CTag tag)
			{
				GameObjList::iterator it = m_aGameObjList.begin();
				for (; it != m_aGameObjList.end(); ++it)
				{
					if ((*it).get()->GetTagPtr()->Compare(tag.GetTag()))
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *@�I�u�W�F�N�g����(��޸׽)
			_NODISCARD std::list<std::weak_ptr<CGameObject>> FindGameObjctsWithTag(std::string tag)
			{
				if (m_aTagMap.count(tag) == 0)
				{
					m_aTagMap[tag] = gameObjWeakList();
					return std::list<std::weak_ptr<CGameObject>>();
				}
				return m_aTagMap[tag].list;
			}

			// *@��j��o�^
			inline void DontDestroy(std::weak_ptr<CGameObject> ptr)
			{
				m_aDontDestroyList.push_back(ptr.lock()); 
			}

			// *@��j�󃊃X�g�̈����n��
			void PassDontDestroyList(CGameObjectManager* mgr) 
			{
				for (auto & obj : m_aDontDestroyList)
				{
					mgr->SetGameObject(obj);		// ���ڒǉ�
					mgr->DontDestroy(obj);			// �o�^
					obj->SetScene(mgr->GetScene());	// TODO: DontDestroyScene�����?
				}
			}
		};
	}
}
#endif // !__MANAGER_H__
