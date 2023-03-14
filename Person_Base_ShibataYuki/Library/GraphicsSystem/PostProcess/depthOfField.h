//==========================================================
// [depthOfField.h]
//---------------------------------------------------------
// �쐬:2023/02/15 
//---------------------------------------------------------
// �V�F�[�_�[�̖���������Ή�
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

#ifndef __DEPTH_OF_FIELD_H__
#define __DEPTH_OF_FIELD_H__

//--- �C���N���[�h��
#include <GraphicsSystem/PostProcess/postProcess.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>

namespace MySpace
{
	namespace Graphics
	{
		class CDepthOfField : public CPostProcess
		{
#pragma region CEREAL
			//--- �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("depthOfField", cereal::base_class<CPostProcess>(this)),
						CEREAL_NVP(m_fPower)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("depthOfField", cereal::base_class<CPostProcess>(this)),
						CEREAL_NVP(m_fPower)
				);
			}
#pragma endregion
		private:
			float m_fPower;
			CGaussianBlur m_Gauss;			// �K�E�X�u���[
			CRenderTarget m_pMainRT;		// �ŏI�`���

		public:
			CDepthOfField();
			~CDepthOfField();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

			_NODISCARD ID3D11ShaderResourceView* GetResource()override;

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CDepthOfField)

#endif // !__DEPTH_OF_FIELD_H__