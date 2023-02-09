//=========================================================
// [shaderAssets.h]
//---------------------------------------------------------
// 作成:2023/02/09
//---------------------------------------------------------
// 各種シェーダー素材用
// 仕様上、static関数多数
//=========================================================

#ifndef __SHADER_ASSETS_H__
#define __SHADER_ASSETS_H__

#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Texture/imageResource.h>
#include <CoreSystem/Math/myVector.h>

namespace MySpace
{
	namespace Game
	{
		struct SHADER_RATE
		{
			float nega;
			float mono;
			float dummy;
			float dummy_one;
		};

		class CShaderAssets
		{
		private:
			static inline MySpace::Graphics::CImageResource m_pToonResource;
			static inline SHADER_RATE m_Rate;	// 実際に使うのはxのみ?

		public:
			static void Init();
			static void Uninit();

			static void ToonSet();
			static void NegaSet();


#ifdef BUILD_MODE
			static void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

#endif // !__SHADER_ASSETS_H__
