//=========================================================
// [CGameObject.h]
//-----------------------
// �쐬:2022/04/19
// �X�V:2023/01/11 AddComponent�֐���SetComponent��ǉ�
//-----------------------
// �Q�[�����I�u�W�F�N�g�N���X
// ���߰��Ă������Ƃ��o����A�^�O�ƃ��C���[��ݒ�ł��� 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

//--- �C���N���[�h��
#include <GameSystem/GameObject/BaseObject.h>
#include <GameSystem/GameObject/tag.h>
#include <GameSystem/GameObject/layer.h>
//#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		//--- �O���Q��
		class CComponent;
		class CTransform;
		class CRenderer;
	}
}
namespace MySpace
{
	namespace SceneManager
	{
		class CScene;
	}
}

namespace MySpace
{
	using MySpace::SceneManager::CScene;
	
	namespace Game
	{
		//--- �N���X��`
		class CGameObject : public CBaseObject<CGameObject>
		{
			friend CComponent;
		private:
			//--- �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(cereal::make_nvp("gameObject", cereal::base_class<CBaseObject>(this)),
					CEREAL_NVP(m_eState), CEREAL_NVP(m_pLayer), CEREAL_NVP(m_pTag),
					CEREAL_NVP(m_aComponent)/*, CEREAL_NVP(m_pTransform)*/
				);
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(cereal::make_nvp("gameObject", cereal::base_class<CBaseObject>(this)),
					CEREAL_NVP(m_eState), CEREAL_NVP(m_pLayer), CEREAL_NVP(m_pTag),
					CEREAL_NVP(m_aComponent)/*, CEREAL_NVP(m_pTransform)*/
				);
				
				// ���߰��đ���serial������ƃG���[
				for (auto & com : m_aComponent)
				{
					com->SetOwner(GetPtr().lock());
				}
			}
		public:
			//--- �񋓑̒�`
			enum E_ObjectState // *@�I�u�W�F�N�g�̏��(�{��bool?)
			{
				ACTIVE,				// *@�X�V���
				WAIT,				// *@�ҋ@(�`��͂����)
				DESTROY,			// *@�폜
				TAKEOVER,			// *@�����p���i�g���܂킵)
				STOP,				// *@�f�o�b�O?
				MAX_OBJECT_STATE,	// *@�ő吔
			};
			//--- �G�C���A�X
			using Ptr = std::shared_ptr<CGameObject>;
			using PtrWeak = std::weak_ptr<CGameObject>;
		private:
			using Component_List = std::list<std::shared_ptr<CComponent>>;

		private:
			//--- �����o�ϐ�
			Component_List m_aComponent;			// �����R���|�[�l���g
			E_ObjectState m_eState;					// ���
			std::unique_ptr<CTag> m_pTag;			// �^�O
			std::unique_ptr<CLayer> m_pLayer;		// ���C���[
			std::weak_ptr<CTransform> m_pTransform;	// �p�����[�^
			std::weak_ptr<CScene> m_pScene;			// �����V�[��

		private:
			// *@�R���|�[�l���g�̒ǉ�����			
			void ComponentAddPreparation(std::shared_ptr<CComponent> com);

		public:
			//--- �����o�֐�
			CGameObject();							// *@�R���X�g���N�^
			CGameObject(std::string name);			// *@���������R���X�g���N�^
			CGameObject(const CGameObject& obj);	// *@�R�s�[�R���X�g���N�^
			virtual ~CGameObject();					// *@�f�X�g���N�^

			virtual void Awake();					// *@�������Ăяo�����(����Create)
			virtual void Init();					// *@������
			virtual void Update();					// *@�Q�[���I�u�W�F�N�g���ƍX�V
			virtual void LateUpdate();				// *@�Q�[���I�u�W�F�N�g���ƍŏI�X�V
			virtual void FixedUpdate();				// *@��莞�ԍX�V

			virtual void OnLoad();					// *@�ް��ǂ݂��ݎ��Ăяo��

			//--- ���Z�q�̃I�[�o�[���[�h(�g���ĂȂ����� 
			bool operator<(CGameObject* other) { return other->GetLayer() < this->GetLayer(); }
			bool operator<(std::shared_ptr<CGameObject> other) { return other->GetLayer() < this->GetLayer(); }
			bool operator>(CGameObject* other) { return other->GetLayer() > this->GetLayer(); }
			bool operator>(std::shared_ptr<CGameObject> other) { return other->GetLayer() > this->GetLayer(); }
			
			//--- �R���|�[�l���g�֘A

			// *@�������邱�ƂŒǉ�����
			// *@������ݒ�A��SP�̐ݒ�Ȃǂ��s���A�������������Ăяo��
			template <class T>
			std::shared_ptr<T> AddComponent()
			{
				std::shared_ptr<T> com = std::make_shared<T>(GetPtr().lock());
				m_aComponent.push_back(com);	// �z��ւ̒ǉ�
				ComponentAddPreparation(com);	// ���߰��ď���
				com.get()->Awake();			// �������Ăяo��
				return com;
			}

			// *@�R���|�[�l���g�̔j��
			template <class T>
			bool RemoveComponent(std::string comName);
			// *@�R���|�[�l���g�̔j��(������:SP)
			bool RemoveComponent(std::weak_ptr<CComponent> com);

			// *@�R���|�[�l���gList�̎擾
			_NODISCARD inline Component_List GetComponentList() { return m_aComponent; }

			// *@�R���|�[�l���g�̎擾
			// *@weak_ptr���Ԃ�
			template <class T>
			_NODISCARD std::weak_ptr<T> GetComponent()
			{
				std::weak_ptr<T> component;
				for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it) 
				{
					component = std::dynamic_pointer_cast<T>((*it));
					// �Ď��Ώۂ̔���
					// lock�̓|�C���^�擾�Bexpired �͔���p?
					if (component.expired())continue;
					return component;
				}
				return std::weak_ptr<T>();	// ����null
			}

			//--- �Q�b�^�[�E�Z�b�^�[
			// *@�`���Ԃ̊m�F
			inline bool IsVision() { return (m_eState == E_ObjectState::ACTIVE || m_eState == E_ObjectState::WAIT); }
			// *@��è�ޏ�Ԃ̊m�F
			inline bool IsActive() { return (m_eState == E_ObjectState::ACTIVE); }
			// *@�����V�[���̎擾
			inline CScene* GetScene() { return m_pScene.lock().get(); }
			// *@��ݽ̫�т̎擾
			// *@������:int(sp�擾)
			inline CTransform* GetTransform() { return m_pTransform.lock().get(); }
			// *@��ݽ̫�т̎擾
			inline virtual std::shared_ptr<CTransform> GetTransform(int n) { return m_pTransform.lock(); };
			// *@obj��Ԏ擾
			inline E_ObjectState GetState() { return m_eState; };															
			// *@��޸׽�̎擾
			inline CTag* GetTagPtr() { return m_pTag.get(); };
			// *@��ޕ����̎擾
			inline std::string GetTag() { return GetTagPtr()->GetTag(); };											
			// *@ڲԸ׽�̎擾
			inline CLayer* GetLayerPtr() { return m_pLayer.get(); };
			// *@ڲ԰�ԍ�
			inline int GetLayer() { return *m_pLayer->GetLayer(); };														

			// *@�V�[���̐ݒ�
			// *@TODO:�Ăяo����ꏊ�𐧌�����ׂ�
			inline void SetScene(std::weak_ptr<CScene> scene) { m_pScene = scene; };
			// *@���C���[�ԍ��̕ύX
			inline void SetLayer(int layer) { m_pLayer->SetLayer(layer); };
			// *@���C���[�׽�̐ݒ�
			inline void SetLayer(CLayer::E_Layer layer) { m_pLayer->SetLayer(layer); };
			// *@�^�O���̐ݒ�
			void SetObjTag(const std::string tag);
			// *@��Ԃɂ��e�q�֌W�ɂ���I�u�W�F�N�g�̏�Ԃ�ς���
			void SetState(const E_ObjectState state);
			//inline void SetTransform(std::shared_ptr<CTransform> trans) { m_pTransform = trans; };

			//--- �R���|�[�l���g�����Փˊ֐��̌Ăяo��
			// *@�����蔻��Ăяo��
			// *@�Փˎ�����
			void OnCollisionEnter(CGameObject* obj);			
			// *@�����蔻��Ăяo��
			// *@�Փˏ�Ԕ���
			void OnCollisionStay(CGameObject* obj);				
			// *@�����蔻��Ăяo��
			// *@�ՓˏI��������
			void OnCollisionExit(CGameObject* obj);				

			// *@�����蔻��Ăяo��(�ضް��)
			// *@�Փˎ�����
			void OnTriggerStay(CGameObject* obj);				
			// *@�����蔻��Ăяo��(�ضް��)
			// *@�Փˏ�Ԕ���
			void OnTriggerEnter(CGameObject* obj);				
			// *@�����蔻��Ăяo��(�ضް��)
			// *@�ՓˏI��������
			void OnTriggerExit(CGameObject* obj);				

			//--- �ÓI�����o�֐�
			// �Q�[���I�u�W�F�N�g�֌W�V���[�g�J�b�g�p�֐�?(���܂肢�����@�ł͂Ȃ��C������)			
			
			// *@���O����
			static std::weak_ptr<CGameObject> FindGameObject(std::string name);						
			// *@�^�O������
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(std::string tag);				
			// *@�^�O������
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(std::string tag);	
			// *@�^�O�׽����
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(CTag tag);						
			// *@�^�O�������X�g�擾
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(CTag tag);			
			// *@�I�u�W�F�N�g�̐���
			static std::weak_ptr<CGameObject> CreateObject(CGameObject* pObj = nullptr);			
			// *@�I�u�W�F�N�g�̐���(Debug��)
			static std::shared_ptr<CGameObject> CreateDebugObject(std::shared_ptr<CGameObject>);	
			// *@�I�u�W�F�N�g�j��
			static void Destroy(std::weak_ptr<CGameObject>);										
			// *@�I�u�W�F�N�g�j�����O�o�^
			static void DontDestroy(std::weak_ptr<CGameObject> obj);								

#ifdef BUILD_MODE
		private:
			int m_nCopyCnt = 0;
		public:
			virtual void ImGuiDebug();
#endif // BUILD_MODE
		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CGameObject)

#endif //__GAME_OBJECT_H__
