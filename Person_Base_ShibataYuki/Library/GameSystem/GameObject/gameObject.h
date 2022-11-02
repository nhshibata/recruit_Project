//=========================================================
//�쐬:2022/04/19 (�Ηj��)
// �Q�[���I�u�W�F�N�g : ���N���X
// 
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
					CEREAL_NVP(m_eState), CEREAL_NVP(m_Layer), CEREAL_NVP(m_Tag),
					CEREAL_NVP(m_pComponent)/*, CEREAL_NVP(m_Transform)*/
				);
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(cereal::make_nvp("gameObject", cereal::base_class<CBaseObject>(this)),
					CEREAL_NVP(m_eState), CEREAL_NVP(m_Layer), CEREAL_NVP(m_Tag),
					CEREAL_NVP(m_pComponent)/*, CEREAL_NVP(m_Transform)*/
				);
				
				// ���߰��đ���serial������ƃG���[
				for (auto & com : m_pComponent)
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
			//--- �Ē�`
			using Ptr = std::shared_ptr<CGameObject>;
			using PtrWeak = std::weak_ptr<CGameObject>;

		private:
			//--- �Ē�`
			using COMPONENT = std::list<std::shared_ptr<CComponent>>;

			//--- �����o�ϐ�
			COMPONENT m_pComponent;					// �����R���|�[�l���g
			E_ObjectState m_eState;					// ���
			std::shared_ptr<CTag> m_Tag;			// �^�O
			std::shared_ptr<CLayer> m_Layer;		// ���C���[
			std::weak_ptr<CTransform> m_Transform;	// �p�����[�^
			std::weak_ptr<CScene> m_pScene;

		public:
			CGameObject();							// *@�R���X�g���N�^
			CGameObject(std::string name);			// *@���������R���X�g���N�^
			CGameObject(const CGameObject& obj);	// *@�R�s�[�R���X�g���N�^
			virtual ~CGameObject();					// *@�f�X�g���N�^

			virtual void Awake();				// *�������Ăяo�����(����Create)
			virtual void Init();				// *������
			virtual void Update();				// *�Q�[���I�u�W�F�N�g���Ƃ̍X�V
			virtual void LateUpdate();			// *�Q�[���I�u�W�F�N�g���Ƃ̍X�V
			virtual void FixedUpdate();			// *��莞�Ԃ̍X�V

			virtual void OnLoad();					// *@�ް��ǂ݂��ݎ��Ăяo��

			//--- ���Z�q�̃I�[�o�[���[�h(�g���ĂȂ����� 
			bool operator<(CGameObject* other) { return other->GetLayer() < this->GetLayer(); }
			bool operator<(std::shared_ptr<CGameObject> other) { return other->GetLayer() < this->GetLayer(); }
			bool operator>(CGameObject* other) { return other->GetLayer() > this->GetLayer(); }
			bool operator>(std::shared_ptr<CGameObject> other) { return other->GetLayer() > this->GetLayer(); }
			
			//--- �R���|�[�l���g
			// *�������邱�ƂŒǉ�����
			// *������ݒ�A��SP�̐ݒ�Ȃǂ��s���A�������������Ăяo��
			template <class T>
			std::shared_ptr<T> AddComponent()
			{
				//static_assert(std::is_base_of<CComponent, T> == false, "not CComponent");// ���߰��Ă��p�����Ă��邩�m�F

				std::shared_ptr<T> com = std::make_shared<T>(GetPtr().lock());
				com->SetPtr(com);					// ���g�̃|�C���^�̐ݒ�
				com->SetOwner(GetPtr().lock());		// ����gameobject�̐ݒ�

				com.get()->Awake();
				AddComponent(com);				// �z��ւ̒ǉ�
				return com;
			}

			// *�R���|�[�l���g�̒ǉ�
			// *��onj��com�̏ꍇ�Acom�̋��L�ƂȂ�
			std::shared_ptr<CComponent> AddComponent(std::shared_ptr<CComponent> com);
			// *�R���|�[�l���g�̔j��
			template <class T>
			bool RemoveComponent(std::string comName);
			// *�R���|�[�l���g�̔j��(������:SP)
			bool RemoveComponent(std::weak_ptr<CComponent> com);
			// *�R���|�[�l���gList�̎擾
			inline COMPONENT GetComponentList() { return m_pComponent; }

			// *�R���|�[�l���g�̎擾
			// *weak_ptr���Ԃ�
			template <class T>
			std::weak_ptr<T> GetComponent()
			{
				std::weak_ptr<T> component;
				for (COMPONENT::iterator it = m_pComponent.begin(); it != m_pComponent.end(); ++it) 
				{
					component = std::dynamic_pointer_cast<T>((*it));
					// �Ď��Ώۂ̔���
					// lock�̓|�C���^�擾�Bexpired �͔���p?
					if (component.expired())continue;
					return component;
				}
				//return component;
				return std::weak_ptr<T>();	// ����null
			}

			// �Q�b�^�[�E�Z�b�^�[

			inline bool IsVision() { return (m_eState == E_ObjectState::ACTIVE || m_eState == E_ObjectState::WAIT); }	// *�`���Ԃ̊m�F
			inline bool IsActive() { return (m_eState == E_ObjectState::ACTIVE); }										// *��è�ޏ�Ԃ̊m�F
			inline CScene* GetScene() { return m_pScene.lock().get(); }
			inline CTransform* GetTransform() { return m_Transform.lock().get(); };										// *��ݽ̫�т̎擾
			inline virtual std::shared_ptr<CTransform> GetTransform(int n) { return m_Transform.lock(); };
			inline E_ObjectState GetState() { return m_eState; };														// *obj��Ԏ擾
			inline std::shared_ptr<CTag> GetTagPtr() { return m_Tag; };													// *��޸׽�̎擾
			inline std::string GetTag() { return GetTagPtr()->GetTag(); };												// *��ޕ����̎擾
			inline std::shared_ptr<CLayer> GetLayerPtr() { return m_Layer; };											// *ڲԸ׽�̎擾
			inline int GetLayer() { return *m_Layer->GetLayer(); };														// *ڲ԰�ԍ�

			inline void SetScene(std::weak_ptr<CScene> scene) { m_pScene = scene; };
			inline void SetTransform(std::shared_ptr<CTransform> trans) { m_Transform = trans; };
			inline void SetState(const E_ObjectState state) { m_eState = state; }// TODO: ��Ԃɂ��e�q�֌W�ɂ���I�u�W�F�N�g�̏�Ԃ�ς���
			inline void SetLayer(int layer) { m_Layer->SetLayer(layer); };
			inline void SetLayer(CLayer::E_Layer layer) { m_Layer->SetLayer(layer); };
			void SetTag(const std::string tag);

			//--- ���z�֐�
			// �R���|�[�l���g�����Փˊ֐��̌Ăяo��
			void OnCollisionEnter(CGameObject* obj);			// *�����蔻��Ăяo��
			void OnCollisionStay(CGameObject* obj);			// *�����蔻��Ăяo��
			void OnCollisionExit(CGameObject* obj);			// *�����蔻��Ăяo��
			void OnTriggerStay(CGameObject* obj);				// *�����蔻��Ăяo��(�ضް��)
			void OnTriggerEnter(CGameObject* obj);			// *�����蔻��Ăяo��(�ضް��)
			void OnTriggerExit(CGameObject* obj);				// *�����蔻��Ăяo��(�ضް��)
			//void HitCheck(CGameObject* obj);

			//--- �ÓI�����o�֐�
			// �Q�[���I�u�W�F�N�g�֌W�V���[�g�J�b�g�p�֐�?(���܂肢�����@�ł͂Ȃ��C������)
			static std::weak_ptr<CGameObject> FindGameObject(std::string name);						// *���O����
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(std::string tag);				// *�^�O���O����
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(std::string tag);	// *�^�O���O����
			static std::weak_ptr<CGameObject> FindGameObjectWithTag(CTag tag);						// *�^�O����
			static std::list<std::weak_ptr<CGameObject>> FindGameObjectsWithTag(CTag tag);			// *�^�O�������X�g
			static std::weak_ptr<CGameObject> CreateObject(CGameObject* pObj = nullptr);			// *�I�u�W�F�N�g�̐���
			static void Destroy(std::weak_ptr<CGameObject>);										// *�I�u�W�F�N�g�j��
			static void DontDestroy(std::weak_ptr<CGameObject> obj);								// *�I�u�W�F�N�g�j�����O

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CGameObject)

#endif //__GAME_OBJECT_H__
