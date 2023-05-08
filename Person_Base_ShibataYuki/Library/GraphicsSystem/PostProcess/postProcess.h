//==========================================================
// [postProcess.h]
//---------------------------------------------------------
// �쐬:2023/02/15 
//---------------------------------------------------------
// ���N���X
//==========================================================

#ifndef __POST_PROCESS_H__
#define __POST_PROCESS_H__

//--- �C���N���[�h��
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
			//--- �V���A���C�Y
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
			CRenderTarget m_pMainRT;		// �ŏI�`���

		public:
			CPostProcess();
			virtual ~CPostProcess();

			// *@�X�v���C�g�`��
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


