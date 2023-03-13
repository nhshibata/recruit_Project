//==========================================================
// [gaussianBlur.h]
//---------------------------------------------------------
// �쐬:2023/02/15 
//---------------------------------------------------------
// �V�F�[�_�[�̖��������̂܂�
// https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/f65ac5cacd4de3c260282069fecf4b254d59207c/MiniEngine/GaussianBlur.cpp
//==========================================================

#ifndef __GAUSSIAN_BLUR_H__
#define __GAUSSIAN_BLUR_H__

#include <GraphicsSystem/DirectX/renderTarget.h>

namespace MySpace
{
	namespace Graphics
	{
		class CGaussianBlur 
		{
			//--- �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(CEREAL_NVP(m_weight), CEREAL_NVP(m_eFormat), CEREAL_NVP(m_fWidth), CEREAL_NVP(m_fHeight)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(CEREAL_NVP(m_weight), CEREAL_NVP(m_eFormat),CEREAL_NVP(m_fWidth), CEREAL_NVP(m_fHeight)
				);
			}
			
		private:
			//--- �萔�o�b�t�@�p�\����
			enum { NUM_WEIGHTS = 8 };				//�d�݂̐��B
			struct WEIGHT_TABLE
			{
				//--- �V���A���C�Y
				template<class Archive>
				void serialize(Archive &archive) 
				{
					archive(
						CEREAL_NVP(fWeights)
					);
				}
				float fWeights[NUM_WEIGHTS];			//�d�݃e�[�u���B
			};
		private:
			WEIGHT_TABLE m_weight;			//�d�݃e�[�u���B
			ID3D11ShaderResourceView* m_originalTexture = nullptr;	//�I���W�i���e�N�X�`���B
			CRenderTarget m_xBlurRenderTarget;		//���{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
			CRenderTarget m_yBlurRenderTarget;		//�c�{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
			//Sprite m_xBlurSprite;					//���{�P�摜��`�悷�邽�߂̃X�v���C�g�B
			//Sprite m_yBlurSprite;					//�c�{�P�摜��`�悷�邽�߂̃X�v���C�g�B
			DXGI_FORMAT m_eFormat;
			float m_fWidth;
			float m_fHeight;

		private:
			// �����_�����O�^�[�Q�b�g������
			void InitRenderTargets();

			// �X�v���C�g
			void DrawSpriteXBlur();
			void DrawSpriteYBlur();

			// �d�݃e�[�u���X�V
			void UpdateWeightsTable(float blurPower);

		public:
			CGaussianBlur();
			CGaussianBlur(ID3D11ShaderResourceView* originalTexture, DXGI_FORMAT eFormat,
						  float fWidth, float fHeight);
			~CGaussianBlur();

			static HRESULT InitShader();

			void Init(ID3D11ShaderResourceView* originalTexture, DXGI_FORMAT m_eFormat,
					  float m_fWidth,
					  float m_fHeight);

			// �K�E�V�A���u���[��GPU��Ŏ��s
			// �����_�����O�^�[�Q�b�g
			// �u���[�̋����B�l���傫���قǃ{�P�������Ȃ�
			void ExecuteOnGPU(float blurPower, ID3D11ShaderResourceView* tex);

			// ���u���[�e�N�X�`�����擾
			ID3D11ShaderResourceView* GetXBlurTexture()
			{
				return m_xBlurRenderTarget.GetSRV();
			}
			
			// �{�P�e�N�X�`�����擾
			ID3D11ShaderResourceView* GetBokeTexture()
			{
				return m_yBlurRenderTarget.GetSRV();
			}
		
			static void CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma);
		};

	}

}

#endif // !__GAUSSIAN_BLUR_H__
