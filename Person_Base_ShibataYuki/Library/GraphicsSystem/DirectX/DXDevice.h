//=========================================================
// [DXDevice.h] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DEX_DEVICE_H__
#define __DEX_DEVICE_H__

//--- �C���N���[�h��
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
			BS_NONE = 0,							// ��������������
			BS_ALPHABLEND,							// ����������
			BS_ADDITIVE,							// ���Z����
			BS_SUBTRACTION,							// ���Z����

			MAX_BLENDSTATE
		};
		enum class ECullMode : int
		{
			CULLMODE_NONE = 0,						// �J�����O���Ȃ�
			CULLMODE_CW,							// �O�ʃJ�����O
			CULLMODE_CCW,							// �w�ʃJ�����O

			MAX_CULLMODE
		};

		//--- �N���X��`
		class CDXDevice : public CAppSystem<CDXDevice>
		{

			friend class CSingleton<CDXDevice>; // Singleton �ł̃C���X�^���X�쐬�͋���
			friend class CAppSystem<CDXDevice>; // Singleton �ł̃C���X�^���X�쐬�͋���
		private:
			ComPtr<ID3D11Device>			g_pDevice;				// �f�o�C�X
			ComPtr<ID3D11DeviceContext>		g_pDeviceContext;		// �f�o�C�X �R���e�L�X�g
			ComPtr<IDXGISwapChain> 			g_pSwapChain;			// �X���b�v�`�F�[��
			ComPtr<ID3D11RenderTargetView>	g_pRenderTargetView;	// �t���[���o�b�t�@
			ComPtr<ID3D11Texture2D>			g_pDepthStencilTexture;	// Z�o�b�t�@�p������
			ComPtr<ID3D11DepthStencilView>	g_pDepthStencilView;	// Z�o�b�t�@
			ComPtr<ID3D11RasterizerState>	g_pRs[(int)ECullMode::MAX_CULLMODE];	// ���X�^���C�U �X�e�[�g
			ComPtr<ID3D11BlendState>		g_pBlendState[(int)EBlendState::MAX_BLENDSTATE];// �u�����h �X�e�[�g
			ComPtr<ID3D11DepthStencilState>	g_pDSS[2];				// Z/�X�e���V�� �X�e�[�g

			UINT							g_uSyncInterval = 0;	// �������� (0:��, 1:�L)
			D3D_DRIVER_TYPE					m_DriverType;				// �h���C�o�^�C�v
			D3D_FEATURE_LEVEL				m_FeatureLevel;				// �@�\���x��
			int								m_Width = 0;				// �o�b�N�o�b�t�@�w�T�C�Y
			int								m_Height = 0;				// �o�b�N�o�b�t�@�x�T�C�Y
			std::shared_ptr<D3D11_VIEWPORT> m_viewPort;
			ComPtr<ID3D11SamplerState>		m_SamplerState;

		public:
			
			HRESULT CreateBackBuffer(unsigned int Width, unsigned int Height);
			HRESULT Init(HWND hWnd, unsigned int Width, unsigned int Height, bool full = false);
			void Uninit();

			// �f�o�C�X�擾
			inline ID3D11Device* GetDevice() { return g_pDevice.Get(); }
			
			// �f�o�C�X �R���e�L�X�g�擾
			inline ID3D11DeviceContext* GetDeviceContext() { return g_pDeviceContext.Get(); }

			inline IDXGISwapChain* GetSwapChain() { return g_pSwapChain.Get(); }

			inline ID3D11RenderTargetView* GetRenderTargetView() { return g_pRenderTargetView.Get(); }

			inline ID3D11DepthStencilView* GetDepthStencilView() { return g_pDepthStencilView.Get(); }

			inline ID3D11DepthStencilState* GetDepthStencilState(int no) { return g_pDSS[no].Get(); }

			inline ID3D11RasterizerState* GetRasterizerState(int no) { return g_pRs[no].Get(); }

			// �r���[�|�[�g�̍������擾����
			inline int GetViewPortHeight() const { return m_Height; }

			// �r���[�|�[�g�̕����擾����
			inline int GetViewPortWidth() const { return m_Width; }

			inline D3D11_VIEWPORT* GetViewPort() { return m_viewPort.get(); };

			// �[�x�o�b�t�@�L����������
			inline void SetZBuffer(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[1].Get(), 0);
			}
			// �[�x�o�b�t�@�X�V�L����������
			inline void SetZWrite(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[0].Get(), 0);
			}
			// �u�����h �X�e�[�g�ݒ�
			inline void SetBlendState(int nBlendState)
			{
				if (nBlendState >= 0 && nBlendState < (int)EBlendState::MAX_BLENDSTATE) 
				{
					float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					g_pDeviceContext->OMSetBlendState(g_pBlendState[nBlendState].Get(), blendFactor, 0xffffffff);
				}
			}
			// �J�����O�ݒ�
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