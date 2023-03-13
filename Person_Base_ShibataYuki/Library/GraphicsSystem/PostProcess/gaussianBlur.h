//==========================================================
// [gaussianBlur.h]
//---------------------------------------------------------
// 作成:2023/02/15 
//---------------------------------------------------------
// シェーダーの魔導書そのまま
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
			//--- シリアライズ
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
			//--- 定数バッファ用構造体
			enum { NUM_WEIGHTS = 8 };				//重みの数。
			struct WEIGHT_TABLE
			{
				//--- シリアライズ
				template<class Archive>
				void serialize(Archive &archive) 
				{
					archive(
						CEREAL_NVP(fWeights)
					);
				}
				float fWeights[NUM_WEIGHTS];			//重みテーブル。
			};
		private:
			WEIGHT_TABLE m_weight;			//重みテーブル。
			ID3D11ShaderResourceView* m_originalTexture = nullptr;	//オリジナルテクスチャ。
			CRenderTarget m_xBlurRenderTarget;		//横ボケ画像を描画するレンダリングターゲット。
			CRenderTarget m_yBlurRenderTarget;		//縦ボケ画像を描画するレンダリングターゲット。
			//Sprite m_xBlurSprite;					//横ボケ画像を描画するためのスプライト。
			//Sprite m_yBlurSprite;					//縦ボケ画像を描画するためのスプライト。
			DXGI_FORMAT m_eFormat;
			float m_fWidth;
			float m_fHeight;

		private:
			// レンダリングターゲット初期化
			void InitRenderTargets();

			// スプライト
			void DrawSpriteXBlur();
			void DrawSpriteYBlur();

			// 重みテーブル更新
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

			// ガウシアンブラーをGPU上で実行
			// レンダリングターゲット
			// ブラーの強さ。値が大きいほどボケが強くなる
			void ExecuteOnGPU(float blurPower, ID3D11ShaderResourceView* tex);

			// 横ブラーテクスチャを取得
			ID3D11ShaderResourceView* GetXBlurTexture()
			{
				return m_xBlurRenderTarget.GetSRV();
			}
			
			// ボケテクスチャを取得
			ID3D11ShaderResourceView* GetBokeTexture()
			{
				return m_yBlurRenderTarget.GetSRV();
			}
		
			static void CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma);
		};

	}

}

#endif // !__GAUSSIAN_BLUR_H__
