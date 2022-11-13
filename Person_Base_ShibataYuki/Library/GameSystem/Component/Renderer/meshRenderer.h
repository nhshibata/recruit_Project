//=========================================================
// [meshRenderer.h] 
// �쐬: 2022/06/27
// �쐬: 2022/08/20 �`�掞�ɕ`�悷�邩������s�����߁A�o�E���f�B���O��ǉ�
//---------------------------------------------------------
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
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius)
				);
			}
		private:
			//--- �����o�ϐ�
			Vector3 m_vCenter;			// *@
			bool m_bLightEnable;		// *@
			float m_fBSRadius = 1;		// *@�o�E���f�B���O�X�t�B�A
		
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

			inline void SetLightEnable(bool flg) { m_bLightEnable = flg; };
			inline void SetCenter(Vector3 value) { m_vCenter = value; }
			inline void SetBSRadius(const float value) { m_fBSRadius = value; };

			// over ride
			virtual void SetMaterial(CMeshMaterial mat) {};
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CMeshRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CMeshRenderer)

#endif // !__RENDERER_H_
