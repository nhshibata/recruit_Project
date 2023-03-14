//==========================================================
// [depthOfField.h]
//---------------------------------------------------------
// 作成:2023/02/15 
//---------------------------------------------------------
// シェーダーの魔導書から対応
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

#ifndef __DEPTH_OF_FIELD_H__
#define __DEPTH_OF_FIELD_H__

//--- インクルード部
#include <GraphicsSystem/PostProcess/postProcess.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>

namespace MySpace
{
	namespace Graphics
	{
		class CDepthOfField : public CPostProcess
		{
#pragma region CEREAL
			//--- シリアライズ
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
			CGaussianBlur m_Gauss;			// ガウスブラー
			CRenderTarget m_pMainRT;		// 最終描画先

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