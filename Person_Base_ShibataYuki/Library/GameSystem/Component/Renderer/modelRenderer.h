//=========================================================
// [modelRenderer.h] 
// �쐬: 2022/06/27
// �X�V: 2022/07/04 ����
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __MODEL_RENDERER_COMPONENT_H__
#define __MODEL_RENDERER_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GraphicsSystem/Manager/modelManager.h>

//--- �萔��`
#define MODEL_PATH				FORDER_DIR(Data/model/)
#define MODEL_PATH2(name)		FORDER_DIR(Data/model/name)
#define CHARACTER_PATH(name)	FORDER_DIR(Data/model/character/name)

namespace MySpace
{
	namespace Game
	{
		class CModelRenderer : public CMeshRenderer
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("modelRender", cereal::base_class<CMeshRenderer>(this)),
					/*CEREAL_NVP(m_pModelCom),*/ CEREAL_NVP(m_modelName)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("modelRender", cereal::base_class<CMeshRenderer>(this)),
					/*CEREAL_NVP(m_pModelCom),*/ CEREAL_NVP(m_modelName)
				);
			}
		public:
			//--- �G�C���A�X
			using PtrWeak = std::weak_ptr<CModelRenderer>;
			
		private:
			//--- �����o�ϐ�
			ModelSharedPtr m_pModel;
			std::string m_modelName;
			UINT m_nVertex;				// ���_��
			TAssimpVertex* m_pVertex;	// ���_�z��
			UINT m_nIndex;				// �C���f�b�N�X��
			UINT* m_pIndex;				// �C���f�b�N�X�z��

		private:
			//--- �����o�֐�
			void InitVertexArray();
			void FinVertexArray();

		public:
			CModelRenderer();	
			CModelRenderer(std::shared_ptr<CGameObject> owner);
			~CModelRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();
			virtual bool DrawAlpha();
			bool Draw(int);

			// *@���C�Ƃ̓����蔻��
			bool CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);

			// *@�����Ƃ̓����蔻��
			bool CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);
			bool CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, std::vector<Vector3>* pX);

			//--- �Q�b�^�[�E�Z�b�^�[
			inline void SetModel(ModelSharedPtr model) { m_pModel = model; /*m_modelName = model.lock()->GetFileName();*/ }
			void SetModel(std::string name);
			
			inline ModelWeakPtr GetModel() { return m_pModel; }
			inline std::string GetModelName() { return m_modelName; }

#ifdef BUILD_MODE

		private:
			//--- ImGui�\���p(�p�X���̊e�g���q���擾����)
			static inline std::vector<std::string> m_aXModelList;
			static inline std::vector<std::string> m_aObjModelList;
			static inline std::vector<std::string> m_aFbxModelList;
		public:
			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CModelRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CModelRenderer)

#endif // !__MODEL_RENDERER_H__
