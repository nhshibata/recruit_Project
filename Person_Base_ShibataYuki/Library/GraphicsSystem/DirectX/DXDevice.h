//=========================================================
// [DXDevice.h] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DX_DEVICE_H__
#define __DX_DEVICE_H__

#define NOMINMAX
//--- �C���N���[�h��
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include <CoreSystem/systemBase.h>

using Microsoft::WRL::ComPtr;

namespace MySpace
{
	namespace Graphics
	{
		//--- �񋓑̒�`
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
		class CDXDevice : public MySpace::System::CSystemBase
		{
		private:
			//--- �����o�ϐ�
			ComPtr<ID3D11Device>			g_pDevice;				// �f�o�C�X
			ComPtr<ID3D11DeviceContext>		g_pDeviceContext;		// �f�o�C�X �R���e�L�X�g
			ComPtr<IDXGISwapChain> 			g_pSwapChain;			// �X���b�v�`�F�[��

			ComPtr<ID3D11RasterizerState>	g_pRs[(int)ECullMode::MAX_CULLMODE];			// ���X�^���C�U �X�e�[�g
			ComPtr<ID3D11BlendState>		g_pBlendState[(int)EBlendState::MAX_BLENDSTATE];// �u�����h �X�e�[�g
			ComPtr<ID3D11DepthStencilState>	g_pDSS[2];				// Z/�X�e���V�� �X�e�[�g

			UINT							g_uSyncInterval;		// �������� (0:��, 1:�L)
			D3D_DRIVER_TYPE					m_DriverType;			// �h���C�o�^�C�v
			D3D_FEATURE_LEVEL				m_FeatureLevel;			// �@�\���x��
			int								m_Width;				// �o�b�N�o�b�t�@�w�T�C�Y
			int								m_Height;				// �o�b�N�o�b�t�@�x�T�C�Y
			std::shared_ptr<D3D11_VIEWPORT> m_viewPort;
			ComPtr<ID3D11SamplerState>		m_SamplerState;
			
			ComPtr<ID3D11RenderTargetView>	g_pRenderTargetView;	// �t���[���o�b�t�@
			ComPtr<ID3D11Texture2D>			g_pRenderTexture;		// �t���[���o�b�t�@�p������
			ComPtr<ID3D11DepthStencilView>	g_pDepthStencilView;	// Z�o�b�t�@
			ComPtr<ID3D11Texture2D>			g_pDepthStencilTexture;	// Z�o�b�t�@�p������
			ComPtr<ID3D11ShaderResourceView> g_pSRV;

		public:
			//--- �����o�֐�
			CDXDevice();
			~CDXDevice() = default;
			
			static CDXDevice* Get();

			HRESULT Init(HWND hWnd, unsigned int Width, unsigned int Height, bool full = false);
			void Uninit();

			// *@�f�o�C�X�擾
			inline ID3D11Device* GetDevice() { return g_pDevice.Get(); }
			
			// *@�f�o�C�X �R���e�L�X�g�擾
			inline ID3D11DeviceContext* GetDeviceContext() const{ return g_pDeviceContext.Get(); }

			// *@�X���b�v�`�F�C��
			inline IDXGISwapChain* GetSwapChain()const{ return g_pSwapChain.Get(); }

			// *@�����_�[�^�[�Q�b�g
			inline ID3D11RenderTargetView* GetRenderTargetView()const { return g_pRenderTargetView.Get(); }
			
			// *@�����_�[�^�[�Q�b�g�p�e�N�X�`���擾
			ID3D11Texture2D* GetRenderTexture()const { return g_pRenderTexture.Get(); }

			// *@�����_�[�^�[�Q�b�g�p�V�F�[�_�[���\�[�X�擾
			ID3D11ShaderResourceView* GetSRV() { return g_pSRV.Get(); }

			// *@�[�x
			inline ID3D11DepthStencilView* GetDepthStencilView() { return g_pDepthStencilView.Get(); }

			// *@�[�x
			inline ID3D11DepthStencilState* GetDepthStencilState(int no) { return g_pDSS[no].Get(); }

			// *@���X�^�C�U
			inline ID3D11RasterizerState* GetRasterizerState(int no) { return g_pRs[no].Get(); }

			// *@�r���[�|�[�g�̍������擾����
			inline int GetViewPortHeight() const { return m_Height; }

			// *@�r���[�|�[�g�̕����擾����
			inline int GetViewPortWidth() const { return m_Width; }

			// *@�r���[�|�[�g�擾
			inline D3D11_VIEWPORT* GetViewPort() { return m_viewPort.get(); };


			// *@�[�x�o�b�t�@�L����������
			inline void SetZBuffer(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[1].Get(), 0);
			}

			// *@�[�x�o�b�t�@�X�V�L����������
			inline void SetZWrite(bool bEnable)
			{
				g_pDeviceContext->OMSetDepthStencilState((bEnable) ? nullptr : g_pDSS[0].Get(), 0);
			}

			// *@�u�����h �X�e�[�g�ݒ�
			void SetBlendState(int nBlendState)
			{
				if (nBlendState >= 0 && nBlendState < (int)EBlendState::MAX_BLENDSTATE)
				{
					float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					g_pDeviceContext->OMSetBlendState(g_pBlendState[nBlendState].Get(), blendFactor, 0xffffffff);
				}
			}

			// *@�u�����h �X�e�[�g�ݒ�
			void SetBlendState(EBlendState eBlendState)
			{
				if (eBlendState >= EBlendState::BS_NONE && eBlendState < EBlendState::MAX_BLENDSTATE)
				{
					float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					g_pDeviceContext->OMSetBlendState(g_pBlendState[(int)eBlendState].Get(), blendFactor, 0xffffffff);
				}
			}

			// *@�J�����O�ݒ�
			void SetCullMode(int nCullMode)
			{
				if (nCullMode >= 0 && nCullMode < (int)ECullMode::MAX_CULLMODE) 
				{
					g_pDeviceContext->RSSetState(g_pRs[nCullMode].Get());
				}
			}

			// *@�`���̕ύX
			// *@nullptr�Œʏ�ɖ߂�
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