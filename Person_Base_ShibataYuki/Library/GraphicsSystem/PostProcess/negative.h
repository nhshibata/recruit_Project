//==========================================================
// [negative.h]
//---------------------------------------------------------
// �쐬:2023/03/07
//---------------------------------------------------------
//==========================================================

#ifndef __NEGATIVE_H__
#define __NEGATIVE_H__

//--- �C���N���[�h��
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Graphics
	{
		class CNegative : public CPostProcess
		{
#pragma region CEREAL
			//--- �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("negative", cereal::base_class<CPostProcess>(this)),
						CEREAL_NVP(m_fNega)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("negative", cereal::base_class<CPostProcess>(this)),
						CEREAL_NVP(m_fNega)
				);
			}
#pragma endregion

		private:
			CRenderTarget m_pMainRT;		// �ŏI�`���
			float m_fNega;

		public:
			CNegative();
			~CNegative();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

			ID3D11ShaderResourceView* GetResource()override;

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CNegative)

#endif // !__NEGATIVE_H__