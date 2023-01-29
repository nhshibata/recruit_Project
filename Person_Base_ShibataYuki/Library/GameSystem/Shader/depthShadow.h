//=========================================================
// [depthShadow.h]
//---------------------------------------------------------
// 作成:2023/01/23
//---------------------------------------------------------
//=========================================================

#ifndef __DEPTH_SHADOW_H__
#define __DEPTH_SHADOW_H__

//--- インクルード部
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <GraphicsSystem/DirectX/depthStencil.h>
#include <GraphicsSystem/Shader/constantBuffer.h>

namespace MySpace
{
	namespace Game
	{

		class CDepthShadow
		{
		private:
			//--- メンバ変数
			std::shared_ptr<MySpace::Graphics::CDepthStencil> m_pDepthStencil;	// 書き込み先
			std::shared_ptr<MySpace::Graphics::CRenderTarget> m_pRenderTarget;	// 書き込み先
			static inline const LPCSTR m_LightCB = "sunLightCB";
			static inline const LPCSTR m_DepthWriteCB = "DepthWriteCB";

		public:
			CDepthShadow();
			~CDepthShadow();

			void InitShader();

			// *@描画前
			// *@data clear
			// *@Bind PS and VS
			void Begin();

			// *@描画後
			// *@描画先再設定
			void End();

			// *@ﾃｸｽﾁｬ設定
			void SetUpTexture(UINT slot);

			std::shared_ptr<MySpace::Graphics::CConstantBuffer> GetCB();

			ID3D11ShaderResourceView* GetResource() { return m_pRenderTarget->GetSRV(); }
		};

	}
}

#endif // !__DEPTH_SHADOW_H__

