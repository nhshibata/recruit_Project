//=========================================================
// [polygon.h] 
// �쐬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __POLYGON_H__
#define __POLYGON_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Render/vertex.h>
#include <CoreSystem/Math/MyMath.h>

#define	NUM_VERTEX		(4)					// ���_��
#define	NUM_POLYGON		(2)					// �|���S����

namespace MySpace
{
	namespace Graphics
	{
		using namespace MySpace::MyMath;

		class CPolygon
		{
		private:
			//--- �����o�ϐ�
			static ID3D11ShaderResourceView* m_pTexture;	// �e�N�X�`���ւ̃|�C���^

			static VERTEX_2D m_vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

			static Vector3 m_vPos;							// �|���S���̈ړ���
			static Vector3 m_vAngle;						// �|���S���̉�]��
			static Vector3 m_vScale;						// �|���S���̃T�C�Y
			static Color m_vColor;						// �|���S���̒��_�J���[
			static bool m_bInvalidate;						// ���_�f�[�^�X�V�t���O

			static XMFLOAT2 m_vPosTexFrame;					// UV���W
			static XMFLOAT2 m_vSizeTexFrame;				// �e�N�X�`�����o�T�C�Y

			static ID3D11Buffer* m_pConstantBuffer;			// �萔�o�b�t�@
			static ID3D11Buffer* m_pVertexBuffer;			// ���_�o�b�t�@
			static ID3D11SamplerState* m_pSamplerState;		// �e�N�X�`�� �T���v��
			static ID3D11VertexShader* m_pVertexShader;		// ���_�V�F�[�_
			static ID3D11InputLayout* m_pInputLayout;		// ���_�t�H�[�}�b�g
			static ID3D11PixelShader* m_pPixelShader;		// �s�N�Z���V�F�[�_

			static XMFLOAT4X4 m_mProj;						// �ˉe�ϊ��s��
			static XMFLOAT4X4 m_mView;						// �r���[�ϊ��s��
			static XMFLOAT4X4 m_mWorld;						// ���[���h�ϊ��s��
			static XMFLOAT4X4 m_mTex;						// �e�N�X�`���ϊ��s��

		private:

			//--- �����o�֐�
			static HRESULT MakeVertex(ID3D11Device* pDevice);
			static void SetVertex(void);

		public:
			static HRESULT Init(ID3D11Device* pDevice);
			static void Fin();
			static void Draw(ID3D11DeviceContext* pDeviceContext);

			// *@�e�N�X�`���̐ݒ�
			static inline void SetTexture(ID3D11ShaderResourceView* pTexture)
			{
				m_pTexture = pTexture;
				m_mTex._44 = (m_pTexture) ? 1.0f : 0.0f;
			}
			// *@�\�����W�̐ݒ�
			static inline  void SetPos(float fX, float fY) { m_vPos.x = fX; m_vPos.y = fY; }
			// *@�\�����W�̐ݒ�
			static inline  void SetPos(Vector2 pos) { m_vPos.x = pos.x; m_vPos.y = pos.y; }

			// *@�\���T�C�Y�̐ݒ�
			static inline void SetSize(float fScaleX, float fScaleY) { m_vScale.x = fScaleX; m_vScale.y = fScaleY; }
			// *@�\���T�C�Y�̐ݒ�
			static inline void SetSize(Vector2 size) { m_vScale.x = size.x; m_vScale.y = size.y; }

			// *@�\���p�x�̐ݒ�(�P��:�x)
			static inline void SetAngle(float fAngle) { m_vAngle.z = fAngle; }

			// *@����e�N�X�`�����W�̐ݒ� (0.0��fU��1.0, 0.0��fV��1.0)
			static inline void SetUV(float fU, float fV) { m_vPosTexFrame.x = fU; m_vPosTexFrame.y = fV; }
			static inline void SetUV(XMFLOAT2 uv) { m_vPosTexFrame = uv; }

			// *@�e�N�X�`���t���[���T�C�Y�̐ݒ� (0.0��fWidth��1.0, 0.0��fHeight��1.0)
			static inline void SetFrameSize(float fWidth, float fHeight) { m_vSizeTexFrame.x = fWidth; m_vSizeTexFrame.y = fHeight; }
			// *@�e�N�X�`���t���[���T�C�Y�̐ݒ� (0.0��fWidth��1.0, 0.0��fHeight��1.0)
			static inline void SetFrameSize(XMFLOAT2 size) { m_vSizeTexFrame = size; }

			// *@���_�J���[�̐ݒ�
			static inline void SetColor(float fRed, float fGreen, float fBlue) { SetColor(XMFLOAT3(fRed, fGreen, fBlue)); }
			// *@���_�J���[�̐ݒ�
			static inline void SetColor(XMFLOAT3 vColor)
			{
				if (m_vColor != vColor)
				{
					m_vColor = vColor;
					m_bInvalidate = true;
				}
			}

			// *@�s�����x�̐ݒ�
			static inline void SetAlpha(float fAlpha)
			{
				if (fAlpha == m_vColor.a)
					return;
				m_vColor.a = fAlpha;
				m_bInvalidate = true;
			}

			// *@���_�J���[�̐ݒ�
			static inline void SetColor(float fRed, float fGreen, float fBlue, float fAlpha)
			{
				SetColor(XMFLOAT4(fRed, fGreen, fBlue, fAlpha));
			}
			// *@���_�J���[�̐ݒ�
			static inline void SetColor(XMFLOAT4 vColor)
			{
				if (m_vColor != vColor)
				{
					m_vColor = vColor;
					m_bInvalidate = true;
				}
			}

		};

	}
}

#endif // !__POLYGON_H__
