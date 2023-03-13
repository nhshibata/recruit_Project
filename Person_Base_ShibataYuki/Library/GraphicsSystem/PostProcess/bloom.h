//==========================================================
// [bloom.h]
//---------------------------------------------------------
// 作成:2023/02/15 
//---------------------------------------------------------
// シェーダーの魔導書から対応
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

#ifndef __BLOOM_H__
#define __BLOOM_H__

//--- インクルード部
#include <GraphicsSystem/PostProcess/postProcess.h>
#include <GraphicsSystem/PostProcess/gaussianBlur.h>

namespace MySpace
{
	namespace Graphics
	{
		class CBloom : public CPostProcess
		{
#pragma region CEREAL
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("bloom", cereal::base_class<CPostProcess>(this)),
						CEREAL_NVP(m_fBlurPower), CEREAL_NVP(m_fThreshold)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("bloom", cereal::base_class<CPostProcess>(this)),
						CEREAL_NVP(m_fBlurPower), CEREAL_NVP(m_fThreshold)
				);
			}
#pragma endregion
		private:
			float m_fBlurPower;				// ブラー強さ
			float m_fThreshold;				// しきい値
			CGaussianBlur m_Gauss;			// ガウスブラー
			CRenderTarget m_pLuminnceRT;	// 輝度抽出用
			CRenderTarget m_pMainRT;		// 最終描画先

		public:
			CBloom();
			~CBloom();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

			_NODISCARD ID3D11ShaderResourceView* GetResource()override;

			_NODISCARD const float GetPower()const { return m_fBlurPower; }
			void SetPower(const float value) { m_fBlurPower = value; }

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};
		
	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CBloom)

#endif // !__BLOOM_H__

