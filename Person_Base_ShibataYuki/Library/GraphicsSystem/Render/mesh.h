//=========================================================
// [mesh.h] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __MESH_H__
#define __MESH_H__

//--- インクルード部
#include <GraphicsSystem/Render/vertex.h>
#include <GraphicsSystem/Shader/instancingData.h>

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
			//--- シリアライズ
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
			//--- メンバ変数
			Vector4 m_Diffuse;		// Diffuse color RGBA
			Vector4 m_Ambient;		// Ambient color RGB
			Vector4 m_Specular;		// Specular 'shininess'
			Vector4 m_Emissive;		// Emissive color RGB
			float m_Power;		// Sharpness if specular highlight

		public:
			//--- メンバ関数
			CMeshMaterial():m_Diffuse(1,1,1,1), m_Ambient(1,1,1,0),m_Power(0){};
			CMeshMaterial(Vector4 diff, Vector4 amb, Vector4 spe, Vector4 emi, float pow)
			{
				m_Diffuse = diff;
				m_Ambient = amb;
				m_Specular = spe;
				m_Emissive = emi;
				m_Power = pow;
			}
			float GetFloat()
			{
				return m_Diffuse.x + m_Diffuse.y + m_Diffuse.z + m_Diffuse.w +
					m_Ambient.x + m_Ambient.y + m_Ambient.z + m_Ambient.w +
					m_Specular.x + m_Specular.y + m_Specular.z + m_Specular.w +
					m_Emissive.x + m_Emissive.y + m_Emissive.z + m_Emissive.w;
			}
		};

		class CMesh
		{
		private:
			//--- シリアライズ
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
			//--- メンバ変数
			ID3D11Buffer* m_pVertexBuffer;	// 頂点バッファインターフェースへのポインタ
			ID3D11Buffer* m_pIndexBuffer;	// インデックスバッファインターフェースへのポインタ
			
			int m_nNumVertex;				// 総頂点数	
			int m_nNumIndex;				// 総インデックス数

			Matrix4x4 m_mWorld;
			CMeshMaterial m_material;		// マテリアル
			
			//XMFLOAT4X4 m_mtxTexture;
			//ID3D11ShaderResourceView* m_pTexture;		// テクスチャ

			//static ID3D11Buffer* m_pConstantBuffer[2];	// 定数バッファ
			static ID3D11SamplerState* m_pSamplerState;	// テクスチャ サンプラ

			//static ID3D11VertexShader* m_pVertexShader;	// 頂点シェーダ
			//static ID3D11InputLayout* m_pInputLayout;	// 頂点フォーマット
			//static ID3D11PixelShader* m_pPixelShader;	// ピクセルシェーダ

			//static inline ID3D11VertexShader* m_pInstancingVS;	// 頂点シェーダ
			//static inline ID3D11PixelShader* m_pShadowPS;		// ピクセルシェーダ
			//static inline ID3D11InputLayout* m_pShadowIL;		// 頂点フォーマット
			//static inline ID3D11Buffer* m_pConstantBufferI;

		public:
			//--- メンバ関数
			CMesh();
			virtual ~CMesh();

			//--- 静的メンバ関数
			static HRESULT InitShader();
			static void FinShader();

			// *@初期化
			HRESULT Init(const VERTEX_3D vertexWk[], int nVertex, int indexWk[], int nIndex);
			
			// *@終了
			virtual void Fin();

			// *@描画
			virtual void Draw(ID3D11ShaderResourceView* m_pTexture = nullptr, XMFLOAT4X4* mWorld = nullptr);
			
			// *@インスタンシング描画
			// *@第二引数falseでdefaultShader off
			void DrawInstancing(const std::vector<RENDER_DATA>& aMesh, const bool& defaultShader = true,
								ID3D11ShaderResourceView* m_pTexture = nullptr, XMFLOAT4X4* mWorld = nullptr);
			
			// *@インスタンシング描画
			// *@第二引数falseでdefaultShader off
			void DrawInstancing(const std::vector<DirectX::XMFLOAT4X4>& aMesh, 
								D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
								const bool& defaultShader = true,
								ID3D11ShaderResourceView* m_pTexture = nullptr,XMFLOAT4X4* mWorld = nullptr);
			
			// *@インスタンシング描画
			void DrawInstancing(const std::vector<RENDER_DATA>& aData, D3D11_PRIMITIVE_TOPOLOGY eTopology,
								ID3D11Buffer* vertexS, ID3D11Buffer* indexS,
								ID3D11ShaderResourceView* m_pTexture = nullptr, XMFLOAT4X4* mWorld = nullptr);
			
			//--- ゲッター・セッター
			inline CMeshMaterial* GetMaterial() { return &m_material; }
			inline int GetIndexNum()const { return m_nNumIndex; }
			inline Matrix4x4 GetWorld() { return m_mWorld; }

			inline void SetWorld(XMFLOAT4X4* pWorld) { m_mWorld = *pWorld; }
			void SetMaterial(const CMeshMaterial* pMaterial);
			inline void SetDiffuse(MySpace::MyMath::Vector4 vDiffuse) { m_material.m_Diffuse = vDiffuse; }

		};
	}
}

#endif // !__DX_TEXTURE_H__
