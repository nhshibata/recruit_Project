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

//--- インクルード部
#include <GraphicsSystem/Manager/shaderManager.h>
#include <GraphicsSystem/Texture/imageResource.h>
#include <GraphicsSystem/Shader/shaderStruct.h>
#include <CoreSystem/Math/myVector.h>

namespace MySpace
{
	namespace Game
	{
		class CShaderAssets
		{
		private:
			static inline MySpace::Graphics::SHADER_RATE m_Rate;	// 実際に使うのはxのみ?

		public:
			static HRESULT Init();
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
