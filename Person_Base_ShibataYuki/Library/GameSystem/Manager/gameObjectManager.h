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

#pragma region _forward_declaration
namespace MySpace
{
	namespace Game
	{
		class CCollision;
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
		public:
			using ObjList = std::list< std::shared_ptr<CGameObject> >;
			using WeakList = std::list< std::weak_ptr<CGameObject> >;
			using COLLISION_VEC = std::vector<std::weak_ptr<CCollision>>;	// �r���j�����ꂽ�ꍇ�A�Q�Ƃ��Ȃ�����weak

		private:
			COLLISION_VEC m_pCollisionComponent;	// �����蔻��׽
			ObjList m_objMgr;
			ObjList m_addObjList;					// �ǉ��I�u�W�F�N�g�i�[�p
			ObjList m_dontDestroyMgr;				// shared_ptr�̐����𗘗p���āA�V�[���J�ڎ��ɓn����j�����X�g
			
		private:
			// *�ǉ��҂����X�g��z��ɒǉ�����
			bool ObjectListUpdate();
			// *�����蔻��p�֐�
			void CollisionCheck();
			// *�z��ǉ�
			void SetGameObject(std::shared_ptr<CGameObject> obj) 
			{
				if (!obj.get()->IsPtrIn <ObjList, std::shared_ptr<CGameObject>>(m_objMgr, obj))
				{
					m_objMgr.push_back(obj); 
				}
			}

		public:
			CGameObjectManager();
			~CGameObjectManager();

			void Init();
			void Uninit();
			void Update();
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
			std::shared_ptr<CGameObject> CreateGameObject(CGameObject* pObj = nullptr)
			{
				std::shared_ptr<CGameObject> spObj;
				if (pObj){
					spObj = std::make_shared<CGameObject>(*pObj);
				}
				else {
					spObj = std::make_shared<CGameObject>();
				}
				// ���g��weakPtr��n��
				spObj.get()->SetPtr(spObj);
				// ������
				spObj->SetName(std::string("GameObj_"+ std::to_string(static_cast<int>(m_objMgr.size() + m_addObjList.size()) + 1)));
				spObj.get()->Awake();	// ����OnCreate�ȋC������
				AddGameObject(spObj);	// �ǉ��҂����X�g�ɒǉ�
				//spObj.get()->StartUp();
				//SetGameObject(spObj);
				return spObj;
			};

			// *�I�u�W�F�N�g�̔j��
			bool DestroyObject(std::weak_ptr<CGameObject> pObj)
			{
				// ����
				auto it = std::find(m_objMgr.begin(), m_objMgr.end(), pObj.lock());
				if (it == m_objMgr.end()) return false;	// �z��ɋ��Ȃ�
				m_objMgr.erase(it);

				if(auto dontIt = std::find(m_dontDestroyMgr.begin(), m_dontDestroyMgr.end(), pObj.lock()) ;
						dontIt != m_dontDestroyMgr.end())
					m_dontDestroyMgr.erase(it);	// FIXME: ��������������邩�v�m�F

				pObj.lock().reset();
				if (pObj.lock()) { return false; }	// �Ď��Ώۂ��c���Ă���z�肵�Ȃ��G���[
				return true;
			}

			// *�����蔻��˗�
			inline void SetColComponent(std::weak_ptr<CCollision> com) { m_pCollisionComponent.push_back(com); }

			// *�I�u�W�F�N�g�ǉ��҂��ɒǉ�
			inline void AddGameObject(std::shared_ptr<CGameObject> obj) { m_addObjList.push_back(obj); }

			// �Q�b�^�[�E�Z�b�^�[
			inline ObjList GetList() { return m_objMgr; }
			inline WeakList GetList(int)
			{ 
				WeakList ret;
				for (auto & obj : m_objMgr)ret.push_back(obj);
				return ret;
			}

			// *�I�u�W�F�N�g�㏑��
			inline void SetObjList(std::list<std::shared_ptr<CGameObject>> list) { m_objMgr = list; }

			// *�I�u�W�F�N�g����(���O)
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
			// *�I�u�W�F�N�g����(�^�O��)
			std::weak_ptr<CGameObject> FindGameObjWithTag(std::string tag) 
			{
				for (ObjList::iterator it = m_objMgr.begin(); it != m_objMgr.end(); ++it) 
				{
					if ((*it).get()->GetTagPtr()->Compare(tag))
					{
						return (*it);
					}
				}
				return std::shared_ptr<CGameObject>();
			}
			// *�I�u�W�F�N�g����(��޸׽)
			std::weak_ptr<CGameObject> FindGameObjWithTag(CTag tag) {
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
			std::list<std::weak_ptr<CGameObject>> FindGameObjs(CTag tag) {
				std::list<std::weak_ptr<CGameObject>> list(0);
				for (ObjList::iterator it = m_objMgr.begin(); it != m_objMgr.end(); ++it) {
					if ((*it).get()->GetTagPtr()->Compare(tag.GetTag()))
					{
						list.push_back((*it));
					}
				}
				return list;
			}

			// *��j��o�^
			inline void DontDestroy(std::weak_ptr<CGameObject> ptr) { m_dontDestroyMgr.push_back(ptr.lock()); }

			// *��j�󃊃X�g�̈����n��
			void PassDontDestroyList(CGameObjectManager* mgr) 
			{
				for (auto & obj : m_dontDestroyMgr)
				{
					mgr->SetGameObject(obj);	// ���ڒǉ�
					mgr->DontDestroy(obj);		// �o�^
				}
			}
		};
	}
}
#endif // !__MANAGER_H__
