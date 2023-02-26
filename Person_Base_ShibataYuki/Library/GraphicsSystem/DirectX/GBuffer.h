//==========================================================
// [GBuffer.h]
//---------------------------------------------------------
// 作成:2023/02/15
//---------------------------------------------------------
// オフスクリーンレンダリング用
// 設定を行い、ﾃｸｽﾁｬ取得なども行う
// StackCameraに使う
//==========================================================

#ifndef __G_BUFFER_H__
#define __G_BUFFER_H__

//--- インクルード部
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <GraphicsSystem/DirectX/depthStencil.h>
#include <vector>

namespace MySpace
{
	namespace Graphics
	{
		class CGBuffer
		{
		public:
			enum class ETexture
			{
				COLOR,
				NORMAL,
				WORLD,
				MAX
			};
		private:
			std::vector<std::unique_ptr<CRenderTarget>> m_aRenderTaget;
			std::unique_ptr<CDepthStencil> m_pDepthStencil;

		public:
			CGBuffer();
			~CGBuffer();

			// *@描画前設定
			void SetUpMultiRenderTarget();
			
			// *@各ﾃｸｽﾁｬをPSSet
			void SetUpTextures();

			// *@スプライト描画
			void DrawSpriteRender(const ETexture e, float width, float height);

			// *@指定されたﾃｸｽﾁｬ取得
			ID3D11ShaderResourceView* GetRenderTexture(const ETexture e) { return m_aRenderTaget[int(e)]->GetSRV(); };
		};
	}
}

#endif // !__G_BUFFER_H__

