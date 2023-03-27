//=========================================================
// [shaderStruct.h]
//---------------------------------------------------------
// 作成：2023/02/07 
//---------------------------------------------------------
// シェーダーに渡す情報構造体
//=========================================================

#ifndef __SHADER_STRUCT_H__
#define __SHADER_STRUCT_H__

//--- インクルード部
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
				// 定数名簡略 = スロット番号
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

		// シェーダに渡す値
		// b0
		struct SHADER_GLOBAL_WVP
		{
			DirectX::XMMATRIX	mW;			// ワールド行列(転置行列)
			DirectX::XMMATRIX	mView;		// ビュー行列(転置行列)
			DirectX::XMMATRIX	mProj;		// 射影行列(転置行列)
			DirectX::XMMATRIX	mTex;		// テクスチャ行列(転置行列)
		};

		// vs/ps
		// b1
		struct SHADER_GLOBAL_CAMERA_LIGHT
		{
			DirectX::XMVECTOR	vEye;		// 視点座標
			DirectX::XMVECTOR	vLightDir;	// 光源方向
			DirectX::XMVECTOR	vLa;		// 光源色(アンビエント)
			DirectX::XMVECTOR	vLd;		// 光源色(ディフューズ)
			DirectX::XMVECTOR	vLs;		// 光源色(スペキュラ)
		};

		// マテリアル (シェーダ用)
		struct SHADER_MATERIAL 
		{
			DirectX::XMVECTOR	vAmbient;	// アンビエント色
			DirectX::XMVECTOR	vDiffuse;	// ディフューズ色
			DirectX::XMVECTOR	vSpecular;	// スペキュラ色
			DirectX::XMVECTOR	vEmissive;	// エミッシブ色
			DirectX::XMVECTOR	vFlags;		// テクスチャ有無
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



