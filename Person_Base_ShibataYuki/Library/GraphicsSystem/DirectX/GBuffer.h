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
				DEPTH,
				MAX
			};
		private:
			std::vector<std::unique_ptr<CRenderTarget>> m_aRenderTaget; // 各種レンダーターゲット
			std::unique_ptr<CDepthStencil> m_pDepthStencil;				// 共通したデプスステンシル

		public:
			CGBuffer();
			~CGBuffer();

			// *@描画前設定
			void SetUpMultiRenderTarget();
			
			// *@描画前設定
			// *@カラーのみ
			void SetUpColorRenderTarget();
			
			// *@各ﾃｸｽﾁｬをPSSet
			void SetUpTextures();

			// *@ﾃｸｽﾁｬ情報のｺﾋﾟｰ
			// *@シーン描画されたﾃｸｽﾁｬ情報をGBufferにコピーする
			void CopyTexture();

			// *@スプライト描画
			// *@VS,PS設定は呼び出していない
			void DrawSpriteRender(const ETexture e, float width, float height);

			// *@指定されたﾃｸｽﾁｬ取得
			ID3D11ShaderResourceView* GetRenderTexture(const ETexture e) { return m_aRenderTaget[int(e)]->GetSRV(); };

			void SetSRV(const ETexture e);

#if BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

#endif // !__G_BUFFER_H__

