//=========================================================
// [component.h]
//---------------------------------------------------------
//�쐬:2022/04/19
//---------------------------------------------------------
// �R���|�[�l���g�׽ : ���N���X
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#define NOMINMAX

//--- �C���N���[�h��
#include <GameSystem/Component/BaseComponent.h>
#include <GameSystem/GameObject/gameObject.h>

#include <CoreSystem/Util/cerealCommon.h>
#include <CoreSystem/Math/MyMath.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Game
	{
		//--- �O���Q��
		class CTransform;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CComponent : public CBaseComponent
		{
		protected:
			friend CGameObject;
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("Component", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_pSelfPtr), /*CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("Component", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_pSelfPtr),/* CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
		public:
			//--- �G�C���A�X
			using Ptr = std::weak_ptr<CComponent>;

		private:
			//--- �����o�ϐ�
			Ptr m_pSelfPtr;							// ���g�̃X�}�[�g�|�C���^
			std::weak_ptr<CGameObject> m_pOwner;	// �e�I�u�W�F�N�g	
			bool m_bActive;							// �A�N�e�B�u���

		public:
			//--- ���ފ֐�
			CComponent();
			CComponent(const CComponent& copy);
			CComponent(std::shared_ptr<CGameObject> owner);
			virtual ~CComponent();

			//--- ��{
			// *@�������ɌĂяo�����
			virtual void Awake() = 0;

			// *@������ ���R���|�[�l���g�̎擾�Ȃǂ��s��
			virtual void Init();

			// *@�I������
			virtual void Uninit();
			// *@�X�V �K������
			virtual void Update();
			// *@�x���X�V
			virtual void LateUpdate();
			// *@��莞�Ԃ̍X�V
			virtual void FixedUpdate();
			// *@�ǂݍ��ݎ��Ăяo���֐�
			virtual void OnLoad();

			//--- �����蔻��Ăяo��
			// �Փ˂����Ƃ��ɌĂяo�����֐�
			// �������������ɃI�[�o�[���C�h����

			// *@�����蔻��
			// *@�Փˎ�
			virtual void OnCollisionEnter(CGameObject* obj);
			// *@�����蔻��
			// *@�Փˏ��
			virtual void OnCollisionStay(CGameObject* obj);
			// *@�����蔻��
			// *@�ՓˏI����
			virtual void OnCollisionExit(CGameObject* obj);
			
			// *@�����蔻��
			// *@�Փˎ�
			virtual void OnTriggerEnter(CGameObject* obj);
			// *@�����蔻��
			// *@�Փˏ��
			virtual void OnTriggerStay(CGameObject* obj);
			// *@�����蔻��
			// *@�ՓˏI����
			virtual void OnTriggerExit(CGameObject* obj);

			//--- CGameObject
			// *@gameobject�׽�����AddComponent�Ăяo��
			// *@����������Ő��|�C���^�擾
			template <class T>
			inline std::shared_ptr<T> AddComponent() { return m_pOwner.lock()->AddComponent<T>(); }

#pragma warning(push)
#pragma warning(disable:4100)
			// *@gameobject�׽�����AddComponent�Ăяo��
			template <class T>
			inline T* AddComponent(int n) { return m_pOwner.lock()->AddComponent<T>().get(); }
#pragma warning(pop)

			// *@�����傩��^�w�肵���R���|�[�l���g���擾����
			template <class T>
			inline T* GetComponent() { std::weak_ptr<T> com = m_pOwner.lock()->GetComponent<T>(); return com.lock().get(); };

			//--- �Z�b�^�[�E�Q�b�^�[
			// *@���g��SP��h���N���X�փL���X�g
			template <class T>
			inline std::shared_ptr<T> BaseToDerived() { return std::dynamic_pointer_cast<T>(m_pSelfPtr.lock()); }

			// *@�g�����X�t�H�[���|�C���^�擾
			CTransform* Transform()const;

			// *@tag�擾
			// *@owner��null�Ȃ�G���[
			std::string Tag()const;
			
			// *@���C���[�ԍ��擾
			int GetLayer()const;

			// *@�^���̎擾
			inline std::string GetName() { return typeid(*this).name(); }

			// *@weak_ptr(Sp)�̎擾			
			inline Ptr GetPtr() const { return m_pSelfPtr; }

			// *@Component�̎�����GameObject�^�̎擾
			// *@�������Ȃ��Ő��|�C���^���󂯎��
			// *@�����������weak_ptr���󂯎��
			inline CGameObject* GetOwner() const { return m_pOwner.lock().get(); }

#pragma warning(push)
#pragma warning(disable:4100)
			// *@Component�̎�����GameObject�^�̎擾
			// *@�������Ȃ��Ő��|�C���^���󂯎��
			// *@�����������weak_ptr���󂯎��
			inline std::weak_ptr<CGameObject> GetOwner(int no)const { return m_pOwner.lock(); };
#pragma warning(pop)

			// *@���߰��Ă̱�è�ސݒ�
			inline void SetActive(bool flg) { m_bActive = flg; };

			// *@���g��SP�������n��
			// NOTE: �I�u�W�F�N�g�A�R���|�[�l���g�������Ƀ|�C���^��n���Ȃ���΂����Ȃ�
			inline void SetPtr(Ptr ptr) { m_pSelfPtr = ptr; }

			// *@�������ݒ�
			inline void SetOwner(std::weak_ptr<CGameObject> pOwner) { m_pOwner = pOwner; }

			// *@��è�ޏ�Ԏ擾
			// *@�������Active�łȂ����false
			bool IsActive();
		
		};

	}

}
using namespace MySpace::Game;
using namespace MySpace::MyMath;

CEREAL_REGISTER_TYPE(MySpace::Game::CComponent)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, CBaseComponent<CComponent>)

#endif __COMPONENT_H__
