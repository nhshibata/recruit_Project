//=========================================================
// [DXDevice.h] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __DEX_DEVICE_H__
#define __DEX_DEVICE_H__

//--- インクルード部
#define NOMINMAX
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>

#include <CoreSystem/system.h>

using Microsoft::WRL::ComPtr;
using MySpace::System::CSingleton;

namespace MySpace
{
	namespace Graphics
	{
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
		class CDXDevice : public CAppSystem<CDXDevice>
		{

			friend class CSingleton<CDXDevice>; // Singleton でのインスタンス作成は許可
			friend class CAppSystem<CDXDevice>; // Singleton でのインスタンス作成は許可
		private:
			ComPtr<ID3D11Device>			g_pDevice;				// デバイス
			ComPtr<ID3D11DeviceContext>		g_pDeviceContext;		// デバイス コンテキスト
			ComPtr<IDXGISwapChain> 			g_pSwapChain;			// スワップチェーン
			ComPtr<ID3D11RenderTargetView>	g_pRenderTargetView;	// フレームバッファ
			ComPtr<ID3D11Texture2D>			g_pDepthStencilTexture;	// Zバッファ用メモリ
			ComPtr<ID3D11DepthStencilView>	g_pDepthStencilView;	// Zバッファ
			ComPtr<ID3D11RasterizerState>	g_pRs[(int)ECullMode::MAX_CULLMODE];	// ラスタライザ ステート
			ComPtr<ID3D11BlendState>		g_pBlendState[(int)EBlendState::MAX_BLENDSTATE];// ブレンド ステート
			ComPtr<ID3D11DepthStencilState>	g_pDSS[2];				// Z/ステンシル ステート

			UINT							g_uSyncInterval = 0;	// 垂直同期 (0:無, 1:有)
			D3D_DRIVER_TYPE					m_DriverType;				// ドライバタイプ
			D3D_FEATURE_LEVEL				m_FeatureLevel;				// 機能レベル
			int								m_Width = 0;				// バックバッファＸサイズ
			int								m_Height = 0;				// バックバッファＹサイズ
			std::shared_ptr<D3D11_VIEWPORT> m_viewPort;
			ComPtr<ID3D11SamplerState>		m_SamplerState;

		public:
			
			HRESULT CreateBackBuffer(unsigned int Width, unsigned int Height);
			HRESULT Init(HWND hWnd, unsigned int Width, unsigned int Height, bool full = false);
			void Uninit();

			// デバイス取得
			inline ID3D11Device* GetDevice() { return g_pDevice.Get(); }
			
			// デバイス コンテキスト取得
			inline ID3D11DeviceContext* GetDeviceContext() { return g_pDeviceContext.Get(); }

			inline IDXGISwapChain* GetSwapChain() { return g_pSwapChain.Get(); }

			inline ID3D11RenderTargetView* GetRenderTargetView() { return g_pRenderTargetView.Get(); }

			inline ID3D11DepthStencilView* GetDepthStencilView() { return g_pDepthStencilView.Get(); }

			inline ID3D11DepthStencilState* GetDepthStencilState(int no) { return g_pDSS[no].Get(); }

			inline ID3D11RasterizerState* GetRasterizerState(int no) { return g_pRs[no].Get(); }

			// ビューポートの高さを取得する
			inline int GetViewPortHeight() const { return m_Height; }

			// ビューポートの幅を取得する
			inline int GetViewPortWidth() const { return m_Width; }

			inline D3D11_VIEWPORT* GetViewPort() { return m_viewPort.get(); };

			// 深度バッファ有効無効制御
			inline void SetZBuffer(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[1].Get(), 0);
			}
			// 深度バッファ更新有効無効制御
			inline void SetZWrite(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[0].Get(), 0);
			}
			// ブレンド ステート設定
			inline void SetBlendState(int nBlendState)
			{
				if (nBlendState >= 0 && nBlendState < (int)EBlendState::MAX_BLENDSTATE) 
				{
					float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					g_pDeviceContext->OMSetBlendState(g_pBlendState[nBlendState].Get(), blendFactor, 0xffffffff);
				}
			}
			// カリング設定
			inline void SetCullMode(int nCullMode)
			{
				if (nCullMode >= 0 && nCullMode < (int)ECullMode::MAX_CULLMODE) 
				{
					g_pDeviceContext->RSSetState(g_pRs[nCullMode].Get());
				}
			}
		};
	}
}
#endif // !__DEX_DEVICE_H__