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
				DEPTH,
				MAX
			};
		private:
			std::vector<std::unique_ptr<CRenderTarget>> m_aRenderTaget; // �e�탌���_�[�^�[�Q�b�g
			std::unique_ptr<CDepthStencil> m_pDepthStencil;				// ���ʂ����f�v�X�X�e���V��

		public:
			CGBuffer();
			~CGBuffer();

			// *@�`��O�ݒ�
			void SetUpMultiRenderTarget();
			
			// *@�`��O�ݒ�
			// *@�J���[�̂�
			void SetUpColorRenderTarget();
			
			// *@�eø�����PSSet
			void SetUpTextures();

			// *@ø������̺�߰
			// *@�V�[���`�悳�ꂽø�������GBuffer�ɃR�s�[����
			void CopyTexture();

			// *@�X�v���C�g�`��
			// *@VS,PS�ݒ�͌Ăяo���Ă��Ȃ�
			void DrawSpriteRender(const ETexture e, float width, float height);

			// *@�w�肳�ꂽø����擾
			ID3D11ShaderResourceView* GetRenderTexture(const ETexture e) { return m_aRenderTaget[int(e)]->GetSRV(); };

			void SetSRV(const ETexture e);

#if BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

#endif // !__G_BUFFER_H__

