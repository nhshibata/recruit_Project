//=========================================================
// [shaderStruct.h]
//---------------------------------------------------------
// �쐬�F2023/02/07 
//---------------------------------------------------------
// �V�F�[�_�[�ɓn�����\����
//=========================================================

#ifndef __SHADER_STRUCT_H__
#define __SHADER_STRUCT_H__

//--- �C���N���[�h��
#include <DirectXMath.h>

namespace MySpace
{
	namespace Graphics
	{
		// �V�F�[�_�ɓn���l
		// b0
		struct SHADER_GLOBAL_WVP
		{
			DirectX::XMMATRIX	mW;			// ���[���h�s��(�]�u�s��)
			DirectX::XMMATRIX	mView;		// �r���[�s��(�]�u�s��)
			DirectX::XMMATRIX	mProj;		// �ˉe�s��(�]�u�s��)
			DirectX::XMMATRIX	mTex;		// �e�N�X�`���s��(�]�u�s��)
		};

		// vs/ps
		// b1
		struct SHADER_GLOBAL_CAMERA_LIGHT
		{
			DirectX::XMVECTOR	vEye;		// ���_���W
			DirectX::XMVECTOR	vLightDir;	// ��������
			DirectX::XMVECTOR	vLa;		// �����F(�A���r�G���g)
			DirectX::XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
			DirectX::XMVECTOR	vLs;		// �����F(�X�y�L����)
		};

		// �}�e���A�� (�V�F�[�_�p)
		struct SHADER_MATERIAL 
		{
			DirectX::XMVECTOR	vAmbient;	// �A���r�G���g�F
			DirectX::XMVECTOR	vDiffuse;	// �f�B�t���[�Y�F
			DirectX::XMVECTOR	vSpecular;	// �X�y�L�����F
			DirectX::XMVECTOR	vEmissive;	// �G�~�b�V�u�F
			DirectX::XMVECTOR	vFlags;		// �e�N�X�`���L��
			void SetData(DirectX::XMFLOAT4 diff,DirectX::XMFLOAT4 ambi,DirectX::XMFLOAT4 spec, DirectX::XMFLOAT4 emi, DirectX::XMFLOAT4 flg, float power,  bool bTex)
			{
				this->vDiffuse = XMLoadFloat4(&diff);
				this->vAmbient = XMVectorSet(ambi.x, ambi.y, ambi.z, bTex ? 1.0f : 0.0f);
				this->vSpecular = XMVectorSet(spec.x, spec.y, spec.z, power);
				this->vEmissive = XMLoadFloat4(&emi);
				this->vFlags = XMLoadFloat4(&flg);
			}
		};

		// b6
		struct SHADER_SUN
		{
			DirectX::XMFLOAT4X4 sunView;
			DirectX::XMFLOAT4X4 sunProj;
		};

	}
}


#endif // !__SHADER_STRUCT_H__



