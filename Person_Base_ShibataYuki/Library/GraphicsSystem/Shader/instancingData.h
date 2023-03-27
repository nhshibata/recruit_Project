//=========================================================
// [instancingData.h]
//---------------------------------------------------------
// 作成:2023/02/05
//---------------------------------------------------------
// インスタンシングに必要な構造体
// mesh, modelに必要な情報が同じため
// 行列だけでなく、マテリアルも各自設定
//=========================================================

#ifndef __INSTANCING_DATA_H__
#define __INSTANCING_DATA_H__

//--- インクルード部
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
			XMVECTOR vAmbient;	// アンビエント色
			XMVECTOR vDiffuse;	// ディフューズ色
			XMVECTOR vSpecular;	// スペキュラ色
			XMVECTOR vEmissive;	// エミッシブ色
			XMVECTOR vFlags;	// テクスチャ有無
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
			
			//--- コンストラクタ
			RENDER_DATA() 
			{
				mWorld = XMMatrixIdentity();
			};

			RENDER_DATA(MyMath::Matrix4x4 world, MyMath::Vector4 vAmbi, MyMath::Vector4 vDiff,
					 MyMath::Vector4 vSpec, MyMath::Vector4 vEmi, DirectX::XMUINT4 vFlag = DirectX::XMUINT4(0, 0, 0, 0))
			{
				// シェーダーに引き渡すため、転置する
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

		// 渡すよう
		struct INSTANCHING_MATERIAL
		{
			RENDER_DATA_BASE renderData[MAX_WORLD_MATRIX];
		};

	}

}

using MySpace::Graphics::INSTANCHING_MATERIAL;

#endif // !__INSTANCING_DATA_H__
