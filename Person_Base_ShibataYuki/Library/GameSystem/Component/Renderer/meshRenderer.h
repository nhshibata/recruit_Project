//=========================================================
// [meshRenderer.h] 
// �쐬: 2022/06/27
// �X�V: 2022/08/20 �`�掞�ɕ`�悷�邩������s�����߁A�o�E���f�B���O��ǉ�
// �X�V: 2022/12/07 NavMesh�p��Static��ǉ�
//---------------------------------------------------------
// 3D�`��
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __MESH_RENDERER_COMPONENT_H__
#define __MESH_RENDERER_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/renderer.h>
#include <GraphicsSystem/Render/mesh.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		class CMeshRenderer : public CRenderer
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius),CEREAL_NVP(m_nStaticMode)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius), CEREAL_NVP(m_nStaticMode)
				);
			}
		public:
			//--- �񋓑̒�`
			enum class EStaticMode : int // bit�Ǘ�����Ȃ�ǉ�
			{
				NONE = 0,
				NONE_MOVE = 1 << 0,
				DYNAMIC,
				MAX
			};

		private:
			//--- �����o�ϐ�
			Vector3 m_vCenter;				// *@���S���W
			bool m_bLightEnable;			// *@���C�g�L���t���O
			float m_fBSRadius = 1;			// *@�o�E���f�B���O�X�t�B�A
			int m_nStaticMode;				// *@static���
			CMeshMaterial m_MeshMaterial;

		protected:
			void SetInstancing(CMesh* mesh, std::string name = std::string());

		public:
			//--- �����o�֐�
			CMeshRenderer();
			CMeshRenderer(std::shared_ptr<CGameObject> owner);
			virtual ~CMeshRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- �Q�b�^�[�E�Z�b�^�[
			inline bool GetLightEnable() { return m_bLightEnable; };
			inline Vector3 GetCenter() { return m_vCenter; }
			Vector3 GetCenter(int n);
			inline float GetBSRadius()const { return m_fBSRadius; };		// *@�o�E���f�B���O�X�t�B�A
			inline CMeshMaterial* GetMaterial() { return &m_MeshMaterial; }
			inline int GetStatic() { return m_nStaticMode; }

			inline void SetLightEnable(bool flg) { m_bLightEnable = flg; };
			inline void SetCenter(Vector3 value) { m_vCenter = value; }
			inline void SetBSRadius(const float value) { m_fBSRadius = value; };
			inline void SetStatic(EStaticMode value) { m_nStaticMode = static_cast<int>(value); }

#if BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CMeshRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CMeshRenderer)

#endif // !__RENDERER_H_
