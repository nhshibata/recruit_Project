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
#include <CoreSystem/Util/define.h>

namespace MySpace
{
	namespace Graphics
	{
		namespace Slot
		{
			enum EConstantBufSlot
			{
				// �萔���ȗ� = �X���b�g�ԍ�
				CB_WVP = 0,
				CB_CAMERA_LIGHT,
				CB_MATERIAL,
				CB_MODEL_BONE,
				CB_INSTANCE_MATRIX,
				CB_INSTANCE_MATERIAL,
				CB_SUN_VP = 6,
				CB_RATE = 7,
				CB_SCREEN_SIZE = 8,
				CB_BLUR = 9,

				CB_MESH_MATERIAL = 11,
			};

			enum ETextureSlot
			{
				TS_DEFAULT = 0,
				TS_EMISSIVE,
				TS_TRANSPARENT,
				TS_SPECULAR,
				TS_SHADOW = 4,
				TS_RAMP = 5,
				TS_COLOR = 6,
				TS_NORMAL = 7,
				TS_WORLD = 8,
				TS_DEPTH = 9,
			};
		}

		class CHLSLFile
		{
		public:
			static std::string GetCSO(const LPCSTR name)
			{
				std::string ret;
				ret = FORDER_DIR(Data/shader/);
				ret += name;
				ret += ".cso";
				return ret;
			}
		};

		class CPixelName : public CHLSLFile
		{
		public:
			static inline const LPCSTR sz2D =			"PS_2D";
			static inline const LPCSTR szPixel =		"Pixel";
			static inline const LPCSTR szAssimpPixel =	"AssimpPixel";
			static inline const LPCSTR szDefaultMesh =	"PS_Mesh";
			static inline const LPCSTR szDefaultAssimp ="PS_Assimp";
			static inline const LPCSTR szDepthWrite =	"PS_DepthWrite";
			static inline const LPCSTR szAssimpToon =	"PS_AssimpToon";
			static inline const LPCSTR szMeshToon =		"PS_MeshToon";
			static inline const LPCSTR szMeshNega =		"PS_MeshNega";
			static inline const LPCSTR szAssimoNega =	"PS_AssimpNega";
			static inline const LPCSTR szGBuffer =		"PS_GBuffer";
			static inline const LPCSTR szLuminance =	"PS_Luminance";
			static inline const LPCSTR szColorTexture =	"PS_ColorTexture";
			static inline const LPCSTR szNegative =		"PS_Negative";
			static inline const LPCSTR szOutline =		"PS_Outline";
			static inline const LPCSTR szMonochrome =	"PS_Monochrome";
			static inline const LPCSTR szBlur =			"PS_Blur";
		};

		class CVertexName : public CHLSLFile
		{
		public:
			static inline const LPCSTR sz2D =			"VS_2D";
			static inline const LPCSTR szVertex =		"Vertex";
			static inline const LPCSTR szAssimpVertex =	"AssimpVertex";
			static inline const LPCSTR szDefaultMesh =	"VS_Mesh";
			static inline const LPCSTR szDefaultAssimp ="VS_Assimp";
			static inline const LPCSTR szDepthWrite =	"VS_DepthWrite";
			static inline const LPCSTR szXBlur =		"VS_XBlur";
			static inline const LPCSTR szYBlur =		"VS_YBlur";
		};

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

		struct SHADER_RATE
		{
			float nega;
			float mono;
			float dummy;
			float dummy_one;
		};

	}
}


#endif // !__SHADER_STRUCT_H__



