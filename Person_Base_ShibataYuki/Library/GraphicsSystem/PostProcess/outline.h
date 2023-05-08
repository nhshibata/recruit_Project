//==========================================================
// [outline.h]
//---------------------------------------------------------
// �쐬:2023/03/07
//---------------------------------------------------------
//==========================================================

#ifndef __OUTLINE_H__
#define __OUTLINE_H__

//--- �C���N���[�h��
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Graphics
	{
		class COutline : public CPostProcess
		{
#pragma region CEREAL
			//--- �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("outline", cereal::base_class<CPostProcess>(this))
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("outline", cereal::base_class<CPostProcess>(this))
				);
			}
#pragma endregion

		private:

		public:
			COutline();
			~COutline();

			static HRESULT InitShader();

			void DrawSprite(CGBuffer* pGBuf);

#if BUILD_MODE
			void ImGuiDebug();
#endif // BIULD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::COutline)

#endif // !__OUTLINE_H__

