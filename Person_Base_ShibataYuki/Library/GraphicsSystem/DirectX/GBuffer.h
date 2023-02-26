//==========================================================
// [GBuffer.h]
//---------------------------------------------------------
// �쐬:2023/02/15
//---------------------------------------------------------
// �I�t�X�N���[�������_�����O�p
// �ݒ���s���Aø����擾�Ȃǂ��s��
// StackCamera�Ɏg��
//==========================================================

#ifndef __G_BUFFER_H__
#define __G_BUFFER_H__

//--- �C���N���[�h��
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

			// *@�`��O�ݒ�
			void SetUpMultiRenderTarget();
			
			// *@�eø�����PSSet
			void SetUpTextures();

			// *@�X�v���C�g�`��
			void DrawSpriteRender(const ETexture e, float width, float height);

			// *@�w�肳�ꂽø����擾
			ID3D11ShaderResourceView* GetRenderTexture(const ETexture e) { return m_aRenderTaget[int(e)]->GetSRV(); };
		};
	}
}

#endif // !__G_BUFFER_H__

