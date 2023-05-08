//==========================================================
// [monochrome.h]
//---------------------------------------------------------
// 作成:2023/03/07
//---------------------------------------------------------
//==========================================================

#ifndef __MONOCHROME_H__
#define __MONOCHROME_H__

//--- インクルード部
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Graphics
	{
		class CMonochrome : public CPostProcess
		{
#pragma region CEREAL
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("monochorme", cereal::base_class<CPostProcess>(this))
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("monochorme", cereal::base_class<CPostProcess>(this))
				);
			}
#pragma endregion
		private:
			static inline ImageSharedPtr m_pToon;

		public:
			CMonochrome();
			~CMonochrome();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CMonochrome)

#endif // !__MONOCHROME_H__