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

			ID3D11ShaderResourceView* GetResource()override;

			const float GetPower()const { return m_fBlurPower; }
			void SetPower(const float value) { m_fBlurPower = value; }

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};
		
	}
}

#endif // !__BLOOM_H__

