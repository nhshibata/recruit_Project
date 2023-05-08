//==========================================================
// [darkness.h]
//---------------------------------------------------------
// 作成:2023/04/09
//---------------------------------------------------------
//==========================================================

#ifndef __DARKNESS_H__
#define __DARKNESS_H__

//--- インクルード部
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Graphics
	{
		class CDarkness : public CPostProcess
		{
#pragma region CEREAL
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("darkness", cereal::base_class<CPostProcess>(this))
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("darkness", cereal::base_class<CPostProcess>(this))
				);
			}
#pragma endregion
		private:

		public:
			CDarkness();
			~CDarkness();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CDarkness)

#endif // !__MONOCHROME_H__