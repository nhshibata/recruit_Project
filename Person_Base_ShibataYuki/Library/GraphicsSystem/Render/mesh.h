//=========================================================
// [mesh.h] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __MESH_H__
#define __MESH_H__

//--- インクルード部
//#include <d3d11.h>
#include <GraphicsSystem/Render/vertex.h>
#include <CoreSystem/Util/cerealCommon.h>
#include <CoreSystem/Util/define.h>
#include <CoreSystem/Math/MyMath.h>

namespace MySpace
{
	namespace Graphics
	{
		using namespace MySpace::MyMath;

		class CMeshMaterial 
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(
					CEREAL_NVP(m_Diffuse), CEREAL_NVP(m_Ambient),
					CEREAL_NVP(m_Specular), CEREAL_NVP(m_Emissive),
					CEREAL_NVP(m_Power)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(
					CEREAL_NVP(m_Diffuse), CEREAL_NVP(m_Ambient),
					CEREAL_NVP(m_Specular), CEREAL_NVP(m_Emissive),
					CEREAL_NVP(m_Power)
				);
			}
		public:
			Vector4 m_Diffuse;		// Diffuse color RGBA
			Vector4 m_Ambient;		// Ambient color RGB
			Vector4 m_Specular;	// Specular 'shininess'
			Vector4 m_Emissive;	// Emissive color RGB
			float		m_Power;		// Sharpness if specular highlight
			CMeshMaterial() {};
			CMeshMaterial(Vector4 diff, Vector4 amb, Vector4 spe, Vector4 emi, float pow)
			{
				m_Diffuse = diff;
				m_Ambient = amb;
				m_Specular = spe;
				m_Emissive = emi;
				m_Power = pow;
			}
		};

		class CMesh
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(
					CEREAL_NVP(m_nNumVertex), CEREAL_NVP(m_nNumIndex),
					CEREAL_NVP(m_mWorld), CEREAL_NVP(m_material)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(
					CEREAL_NVP(m_nNumVertex), CEREAL_NVP(m_nNumIndex),
					CEREAL_NVP(m_mWorld), CEREAL_NVP(m_material)
				);
			}
		private:
			ID3D11Buffer* m_pVertexBuffer;	// 頂点バッファインターフェースへのポインタ
			ID3D11Buffer* m_pIndexBuffer;	// インデックスバッファインターフェースへのポインタ
			
			int m_nNumVertex;				// 総頂点数	
			int m_nNumIndex;				// 総インデックス数

			Matrix4x4 m_mWorld;
			CMeshMaterial m_material;		// マテリアル
			
			//XMFLOAT4X4 m_mtxTexture;
			//ID3D11ShaderResourceView* m_pTexture;		// テクスチャ

			static ID3D11Buffer* m_pConstantBuffer[2];	// 定数バッファ
			static ID3D11SamplerState* m_pSamplerState;	// テクスチャ サンプラ
			static ID3D11VertexShader* m_pVertexShader;	// 頂点シェーダ
			static ID3D11InputLayout* m_pInputLayout;	// 頂点フォーマット
			static ID3D11PixelShader* m_pPixelShader;	// ピクセルシェーダ

		public:
			CMesh();
			virtual ~CMesh();

			static HRESULT InitShader();
			static void FinShader();

			HRESULT Init(const VERTEX_3D vertexWk[], int nVertex, int indexWk[], int nIndex);
			virtual void Fin();
			virtual void Draw(ID3D11ShaderResourceView* m_pTexture = nullptr, XMFLOAT4X4* mWorld = nullptr);

			void SetWorld(XMFLOAT4X4* pWorld) { m_mWorld = *pWorld; }
			void SetMaterial(const CMeshMaterial* pMaterial);
			void SetDiffuse(MySpace::MyMath::Vector4 vDiffuse) { m_material.m_Diffuse = vDiffuse; }

			CMeshMaterial* GetMaterial() { return &m_material; }
		};
	}
}

#endif // !__DX_TEXTURE_H__
