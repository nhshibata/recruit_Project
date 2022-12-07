//=========================================================
//�쐬:2022/04/19 (�Ηj��)
// �R���|�[�l���g�׽ : ���N���X
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

//--- �C���N���[�h��
#define NOMINMAX

#include <GameSystem/Component/BaseComponent.h>
#include <GameSystem/GameObject/gameObject.h>

#include <CoreSystem/Util/cerealCommon.h>
#include <CoreSystem/Math/MyMath.h>
#include <Application/screen.h>
#include <CoreSystem/Time/fps.h>
#include <memory>

//---
#define LIVE_POINTER			0

#pragma region ForwardDeclaration
namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Game
	{
		//--- �O���Q��
		class CGameObject;
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
					CEREAL_NVP(m_spThisPtr), /*CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("Component", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_spThisPtr),/* CEREAL_NVP(m_pOwner),*/ CEREAL_NVP(m_bActive)
				);
			}
		public:
			//--- �Ē�`
			using Ptr = std::weak_ptr<CComponent>;
		private:
			//--- �����o�ϐ�
			// *@���g�̃X�}�[�g�|�C���^
			Ptr m_spThisPtr;
			// *@�e�I�u�W�F�N�g
			std::weak_ptr<CGameObject> m_pOwner;
			// *@�A�N�e�B�u���
			bool m_bActive;
#if BUILD_MODE
			std::string m_Name;
#endif // BUILD_MODE


		public:
			//--- ���ފ֐�
			CComponent();
			CComponent(std::shared_ptr<CGameObject> owner);
			virtual ~CComponent() {};

			//--- ��{
			// *@ �������ɌĂяo�����
			virtual void Awake() {};
			// *@ ������ ���R���|�[�l���g�̎擾�Ȃǂ��s��
			virtual void Init() {};				
			// *@ �X�V �K������
			virtual void Update() {};			
			// *@ �x���X�V
			virtual void LateUpdate() {};		
			// *@ ��莞�Ԃ̍X�V
			virtual void FixedUpdate() {};		
			// *@ �ǂݍ��ݎ��Ăяo���֐�
			virtual void OnLoad();

			//--- ���z�֐�
			// �Փ˂����Ƃ��ɌĂяo�����֐�
			// �������������ɃI�[�o�[���C�h����

			// *@ �ڂ������Ă���I�u�W�F�N�g���n�����
			virtual void OnCollisionStay(CGameObject* obj) {};
			// *@ �ڂ��n�߂��I�u�W�F�N�g���n�����
			virtual void OnCollisionEnter(CGameObject* obj) {};
			// *@ ���ꂽ�I�u�W�F�N�g���n�����
			virtual void OnCollisionExit(CGameObject* obj) {};
			// *@ �ڂ������Ă���I�u�W�F�N�g���n�����(�g���K�[)
			virtual void OnTriggerStay(CGameObject* obj) {};
			// *@ �ڂ��n�߂��I�u�W�F�N�g���n�����(�g���K�[)
			virtual void OnTriggerEnter(CGameObject* obj) {};
			// *@ ���ꂽ�I�u�W�F�N�g���n�����(�g���K�[)
			virtual void OnTriggerExit(CGameObject* obj) {};

			//--- CGameObject
			// *@gameobject�׽�����AddComponent�Ăяo��
			// *@����������Ő��|�C���^�擾
			template <class T>
			inline std::shared_ptr<T> AddComponent()
			{
				return m_pOwner.lock()->AddComponent<T>();
			}

			// *@gameobject�׽�����AddComponent�Ăяo��
			template <class T>
			inline T* AddComponent(int n)
			{
				return m_pOwner.lock()->AddComponent<T>().get();
			}

			// *@gameobject�׽�֒��ں��߰��Ă�ǉ����� �g��Ȃ�?
			inline void AddComponent(std::shared_ptr<CComponent> com)
			{
				m_pOwner.lock()->AddComponent(com);
			}

			// *@�����傩��^�w�肵���R���|�[�l���g���擾����
			template <class T>
			inline T* GetComponent()
			{
				std::weak_ptr<T> com = m_pOwner.lock()->GetComponent<T>();
				return com.lock().get();
			};

			//--- �Z�b�^�[�E�Q�b�^�[
			// *@�g�����X�t�H�[���|�C���^�擾
			CTransform* Transform()const;

			// *@ tag�擾
			// *@ owner��null�Ȃ�G���[
			std::string Tag()const;
			
			// *@���C���[�ԍ��擾
			int GetLayer()const;

			// *@���g��SP��h���N���X�փL���X�g
			template <class T>
			inline std::shared_ptr<T> BaseToDerived() { return std::dynamic_pointer_cast<T>(m_spThisPtr.lock()); }

			// *@�^���̎擾
			inline std::string GetName() { return typeid(*this).name(); }

			// *@weakPtr�̎擾			
			inline Ptr GetPtr() { return m_spThisPtr; }

			// *@Component�̎�����GameObject�^�̎擾
			// *@�������Ȃ��Ő��|�C���^���󂯎��
			// *@�����������weak_ptr���󂯎��
			inline CGameObject* GetOwner() { return m_pOwner.lock().get(); }

			// *@Component�̎�����GameObject�^�̎擾
			// *@�������Ȃ��Ő��|�C���^���󂯎��
			// *@�����������weak_ptr���󂯎��
			inline std::weak_ptr<CGameObject> GetOwner(int no) { return m_pOwner.lock(); };

			// *@���߰��Ă̱�è�ސݒ�
			inline void SetActive(bool flg) { m_bActive = flg; };

			// *@���g��SP�������n��
			// NOTE: �I�u�W�F�N�g�A�R���|�[�l���g�������Ƀ|�C���^��n���Ȃ���΂����Ȃ�
			inline void SetPtr(Ptr ptr) { m_spThisPtr = ptr; }

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
