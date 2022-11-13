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
	namespace Game
	{
	}

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
			//friend class CScene;
		public:
			//--- �G�C���A�X
			using ObjList = std::list< std::shared_ptr<CGameObject> >;
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
			using TagObjMap = std::unordered_map<std::string, gameObjWeakList>;

		private:
			ObjList m_objMgr;						// ���ۂ̏���
			ObjList m_addObjList;					// �� �ɒǉ�����I�u�W�F�N�g�i�[�p
			ObjList m_dontDestroyMgr;				// shared_ptr�̐����𗘗p���āA�V�[���J�ڎ��ɓn����j�����X�g
			TagObjMap m_tagMap;						// tag�����p
			std::weak_ptr<CScene> m_pAffiliationScene;

		private:
			// *�ǉ��҂����X�g��z��ɒǉ�����
			bool ObjectListUpdate();

			// *�z��ǉ�
			void SetGameObject(std::shared_ptr<CGameObject> obj);
			// *tagMap�ړ�
			void TagMove(std::string NextTag, std::weak_ptr<CGameObject> obj);
			// *@�V�[���擾
			std::weak_ptr<CScene> GetScene() 
			{ 
				return m_pAffiliationScene; 
			};
			
		public:
			CGameObjectManager();
			~CGameObjectManager();
			void SetAffiliationScene(std::shared_ptr<CScene> scene)
			{
				m_pAffiliationScene = scene;
			};
			// ��v����
			void Init();
			void Uninit();
			void Update();
			void UpdateInDebug();
			void FixedUpdate();
			void AllUninit()
			{
				m_objMgr.clear();
				m_addObjList.clear();
				m_dontDestroyMgr.clear();
			}

			// �K�v�Œ���̃Q�[���I�u�W�F�N�g�쐬
			void CreateBasicObject();

			// *�O���ŃQ�[���I�u�W�F�N�g����肽�����ɌĂяo���֐�
			// *���������l����Ɛ����Ə������͕������� + �z��ɉ����邽��
			std::shared_ptr<CGameObject> CreateGameObject(CGameObject* pObj = nullptr);

			// *�I�u�W�F�N�g�̔j��
			bool DestroyObject(std::weak_ptr<CGameObject> pObj);

			// *�I�u�W�F�N�g�ǉ��҂��ɒǉ�
			inline void AddGameObject(std::shared_ptr<CGameObject> obj) 
			{
				m_addObjList.push_back(obj); 
			}

			//--- �Q�b�^�[�E�Z�b�^�[
			// �������X�g(������int��weak)
			inline ObjList GetList() 
			{
				return m_objMgr; 
			}
			// *@�������X�g(weak�p)
			inline WeakList GetList(int)
			{ 
				WeakList ret;
				for (auto & obj : m_objMgr)
					ret.push_back(obj);
				return ret;
			}

			// *�I�u�W�F�N�g�㏑��
			inline void SetObjList(std::list<std::shared_ptr<CGameObject>> list)
			{
				m_objMgr = list;
			}

			// *@�I�u�W�F�N�g����(���O)
			std::weak_ptr<CGameObject> FindGameObj(std::string name) 
			{
				for (ObjList::iterator it = m_objMgr.begin(); it != m_objMgr.end(); ++it) 
				{
					if ((*it).get()->GetName() == name)
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *@�I�u�W�F�N�g����(�^�O��)
			std::weak_ptr<CGameObject> FindGameObjWithTag(std::string tag) 
			{
				if (m_tagMap.count(tag) == 0)
				{
					m_tagMap[tag] = gameObjWeakList();
					return std::weak_ptr<CGameObject>();
				}
				return m_tagMap[tag].list.begin()->lock();
			}
			// *�I�u�W�F�N�g����(��޸׽)
			std::weak_ptr<CGameObject> FindGameObjWithTag(CTag tag) 
			{
				ObjList::iterator it = m_objMgr.begin();
				for (; it != m_objMgr.end(); ++it) {
					if ((*it).get()->GetTagPtr()->Compare(tag.GetTag()))
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *�I�u�W�F�N�g����(��޸׽)
			std::list<std::weak_ptr<CGameObject>> FindGameObjctsWithTag(std::string tag)
			{
				if (m_tagMap.count(tag) == 0)
				{
					m_tagMap[tag] = gameObjWeakList();
					return std::list<std::weak_ptr<CGameObject>>();
				}
				return m_tagMap[tag].list;
			}

			// *��j��o�^
			inline void DontDestroy(std::weak_ptr<CGameObject> ptr) 
			{
				m_dontDestroyMgr.push_back(ptr.lock()); 
			}

			// *��j�󃊃X�g�̈����n��
			void PassDontDestroyList(CGameObjectManager* mgr) 
			{
				for (auto & obj : m_dontDestroyMgr)
				{
					mgr->SetGameObject(obj);	// ���ڒǉ�
					mgr->DontDestroy(obj);		// �o�^
					obj->SetScene(mgr->GetScene());	// TODO: DontDestroyScene�����?
				}
			}
		};
	}
}
#endif // !__MANAGER_H__
