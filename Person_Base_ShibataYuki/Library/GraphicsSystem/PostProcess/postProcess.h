//==========================================================
// [postProcess.h]
//---------------------------------------------------------
// 作成:2023/02/15 
//---------------------------------------------------------
// 基底クラス
//==========================================================

#ifndef __POST_PROCESS_H__
#define __POST_PROCESS_H__

//--- インクルード部
#include <d3d11.h>
#include <GraphicsSystem/DirectX/renderTarget.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Graphics
	{
		class CGBuffer;
	}
}

namespace MySpace
{
	namespace Graphics
	{
		class CPostProcess
		{
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
			}
			template<class Archive>
			void load(Archive& archive)
			{
			}

		protected:
			CRenderTarget m_pMainRT;		// 最終描画先

		public:
			CPostProcess();
			virtual ~CPostProcess();

			// *@スプライト描画
			virtual void DrawSprite(CGBuffer* pGBuf);
			void DrawSprite(CGBuffer* pGBuf, std::string ps, std::string vs);

			_NODISCARD virtual ID3D11ShaderResourceView* GetResource();

			_NODISCARD std::string GetTypeName() { return typeid(*this).name(); }

#if BUILD_MODE
			virtual void ImGuiDebug();
#endif // BIULD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CPostProcess)

#endif // !__POST_PROCESS_H__


