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

namespace MySpace
{
	namespace Graphics
	{
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

	}
}


#endif // !__SHADER_STRUCT_H__



