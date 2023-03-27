//=========================================================
// [instancingData.h]
//---------------------------------------------------------
// �쐬:2023/02/05
//---------------------------------------------------------
// �C���X�^���V���O�ɕK�v�ȍ\����
// mesh, model�ɕK�v�ȏ�񂪓�������
// �s�񂾂��łȂ��A�}�e���A�����e���ݒ�
//=========================================================

#ifndef __INSTANCING_DATA_H__
#define __INSTANCING_DATA_H__

//--- �C���N���[�h��
#include <CoreSystem/Math/MyMath.h>
#include <vector>

#define MAX_WORLD_MATRIX	400

namespace MySpace
{
	namespace Graphics
	{
		//constexpr const char* CB_NAME_INSTANCE_MATRIX = "InstancingMatrix";
		//constexpr const char* CB_NAME_INSTANCE_MATERIAL = "InstancingMaterial";
		//using INSTANCHING_DATA = DirectX::XMFLOAT4X4;


		struct RENDER_DATA_BASE
		{
			XMVECTOR vAmbient;	// �A���r�G���g�F
			XMVECTOR vDiffuse;	// �f�B�t���[�Y�F
			XMVECTOR vSpecular;	// �X�y�L�����F
			XMVECTOR vEmissive;	// �G�~�b�V�u�F
			XMVECTOR vFlags;	// �e�N�X�`���L��
			RENDER_DATA_BASE()
			{};
			RENDER_DATA_BASE(const RENDER_DATA_BASE & copy)
			{
				this->vAmbient = copy.vAmbient;
				this->vDiffuse = copy.vDiffuse;
				this->vSpecular = copy.vSpecular;
				this->vEmissive = copy.vEmissive;
				this->vFlags = copy.vFlags;
			}
		};

		struct RENDER_DATA : public RENDER_DATA_BASE
		{
			XMMATRIX mWorld;
			
			//--- �R���X�g���N�^
			RENDER_DATA() 
			{
				mWorld = XMMatrixIdentity();
			};

			RENDER_DATA(MyMath::Matrix4x4 world, MyMath::Vector4 vAmbi, MyMath::Vector4 vDiff,
					 MyMath::Vector4 vSpec, MyMath::Vector4 vEmi, DirectX::XMUINT4 vFlag = DirectX::XMUINT4(0, 0, 0, 0))
			{
				// �V�F�[�_�[�Ɉ����n�����߁A�]�u����
				this->mWorld = XMMatrixTranspose(XMLoadFloat4x4(&world));
				this->vAmbient = XMLoadFloat4(&vAmbi);
				this->vDiffuse = XMLoadFloat4(&vDiff);
				this->vSpecular = XMLoadFloat4(&vSpec);
				this->vEmissive = XMLoadFloat4(&vEmi);
				this->vFlags = XMLoadUInt4(&vFlag);
			}

			RENDER_DATA(const RENDER_DATA& copy)
			{
				this->mWorld = copy.mWorld;
				this->vAmbient = copy.vAmbient;
				this->vDiffuse = copy.vDiffuse;
				this->vSpecular = copy.vSpecular;
				this->vEmissive = copy.vEmissive;
				this->vFlags = copy.vFlags;
			}
		};
		using INSTANCE_LIST = std::vector<RENDER_DATA>;

		struct INSTANCE_MATRIX
		{
			XMMATRIX mWorld[MAX_WORLD_MATRIX];
		};

		// �n���悤
		struct INSTANCHING_MATERIAL
		{
			RENDER_DATA_BASE renderData[MAX_WORLD_MATRIX];
		};

	}

}

using MySpace::Graphics::INSTANCHING_MATERIAL;

#endif // !__INSTANCING_DATA_H__
