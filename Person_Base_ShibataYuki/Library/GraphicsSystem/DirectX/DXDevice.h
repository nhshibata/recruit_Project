//=========================================================
// [DXDevice.h] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __DX_DEVICE_H__
#define __DX_DEVICE_H__

#define NOMINMAX
//--- インクルード部
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include <CoreSystem/systemBase.h>

using Microsoft::WRL::ComPtr;

namespace MySpace
{
	namespace Graphics
	{
		//--- 列挙体定義
		enum class EBlendState : int
		{
			BS_NONE = 0,							// 半透明合成無し
			BS_ALPHABLEND,							// 半透明合成
			BS_ADDITIVE,							// 加算合成
			BS_SUBTRACTION,							// 減算合成

			MAX_BLENDSTATE
		};
		enum class ECullMode : int
		{
			CULLMODE_NONE = 0,						// カリングしない
			CULLMODE_CW,							// 前面カリング
			CULLMODE_CCW,							// 背面カリング

			MAX_CULLMODE
		};

		//--- クラス定義
		class CDXDevice : public MySpace::System::CSystemBase
		{
		private:
			//--- メンバ変数
			ComPtr<ID3D11Device>			g_pDevice;				// デバイス
			ComPtr<ID3D11DeviceContext>		g_pDeviceContext;		// デバイス コンテキスト
			ComPtr<IDXGISwapChain> 			g_pSwapChain;			// スワップチェーン

			ComPtr<ID3D11RasterizerState>	g_pRs[(int)ECullMode::MAX_CULLMODE];			// ラスタライザ ステート
			ComPtr<ID3D11BlendState>		g_pBlendState[(int)EBlendState::MAX_BLENDSTATE];// ブレンド ステート
			ComPtr<ID3D11DepthStencilState>	g_pDSS[2];				// Z/ステンシル ステート

			UINT							g_uSyncInterval;		// 垂直同期 (0:無, 1:有)
			D3D_DRIVER_TYPE					m_DriverType;			// ドライバタイプ
			D3D_FEATURE_LEVEL				m_FeatureLevel;			// 機能レベル
			int								m_Width;				// バックバッファＸサイズ
			int								m_Height;				// バックバッファＹサイズ
			std::shared_ptr<D3D11_VIEWPORT> m_viewPort;
			ComPtr<ID3D11SamplerState>		m_SamplerState;
			
			ComPtr<ID3D11RenderTargetView>	g_pRenderTargetView;	// フレームバッファ
			ComPtr<ID3D11Texture2D>			g_pRenderTexture;		// フレームバッファ用メモリ
			ComPtr<ID3D11DepthStencilView>	g_pDepthStencilView;	// Zバッファ
			ComPtr<ID3D11Texture2D>			g_pDepthStencilTexture;	// Zバッファ用メモリ
			ComPtr<ID3D11ShaderResourceView> g_pSRV;

		public:
			//--- メンバ関数
			CDXDevice();
			~CDXDevice() = default;
			
			static CDXDevice* Get();

			HRESULT Init(HWND hWnd, unsigned int Width, unsigned int Height, bool full = false);
			void Uninit();

			// *@デバイス取得
			inline ID3D11Device* GetDevice() { return g_pDevice.Get(); }
			
			// *@デバイス コンテキスト取得
			inline ID3D11DeviceContext* GetDeviceContext() const{ return g_pDeviceContext.Get(); }

			// *@スワップチェイン
			inline IDXGISwapChain* GetSwapChain()const{ return g_pSwapChain.Get(); }

			// *@レンダーターゲット
			inline ID3D11RenderTargetView* GetRenderTargetView()const { return g_pRenderTargetView.Get(); }
			
			// *@レンダーターゲット用テクスチャ取得
			ID3D11Texture2D* GetRenderTexture()const { return g_pRenderTexture.Get(); }

			// *@レンダーターゲット用シェーダーリソース取得
			ID3D11ShaderResourceView* GetSRV() { return g_pSRV.Get(); }

			// *@深度
			inline ID3D11DepthStencilView* GetDepthStencilView() { return g_pDepthStencilView.Get(); }

			// *@深度
			inline ID3D11DepthStencilState* GetDepthStencilState(int no) { return g_pDSS[no].Get(); }

			// *@ラスタイザ
			inline ID3D11RasterizerState* GetRasterizerState(int no) { return g_pRs[no].Get(); }

			// *@ビューポートの高さを取得する
			inline int GetViewPortHeight() const { return m_Height; }

			// *@ビューポートの幅を取得する
			inline int GetViewPortWidth() const { return m_Width; }

			// *@ビューポート取得
			inline D3D11_VIEWPORT* GetViewPort() { return m_viewPort.get(); };


			// *@深度バッファ有効無効制御
			inline void SetZBuffer(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[1].Get(), 0);
			}

			// *@深度バッファ更新有効無効制御
			inline void SetZWrite(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[0].Get(), 0);
			}

			// *@ブレンド ステート設定
			void SetBlendState(int nBlendState)
			{
				if (nBlendState >= 0 && nBlendState < (int)EBlendState::MAX_BLENDSTATE)
				{
					float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					g_pDeviceContext->OMSetBlendState(g_pBlendState[nBlendState].Get(), blendFactor, 0xffffffff);
				}
			}

			// *@ブレンド ステート設定
			void SetBlendState(EBlendState eBlendState)
			{
				if (eBlendState >= EBlendState::BS_NONE && eBlendState < EBlendState::MAX_BLENDSTATE)
				{
					float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					g_pDeviceContext->OMSetBlendState(g_pBlendState[(int)eBlendState].Get(), blendFactor, 0xffffffff);
				}
			}

			// *@カリング設定
			void SetCullMode(int nCullMode)
			{
				if (nCullMode >= 0 && nCullMode < (int)ECullMode::MAX_CULLMODE) 
				{
					g_pDeviceContext->RSSetState(g_pRs[nCullMode].Get());
				}
			}

			// *@描画先の変更
			// *@nullptrで通常に戻す
			void SwitchRender(ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
			{
				GetDeviceContext()->OMSetRenderTargets(
					1,
					pRTV ? &pRTV : g_pRenderTargetView.GetAddressOf(),
					pDSV
				);
			}

		};
	}
}
#endif // !__DEX_DEVICE_H__