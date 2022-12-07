//=========================================================
// [mesh.h] 
// �쐬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __MESH_H__
#define __MESH_H__

//--- �C���N���[�h��
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
			// �V���A���C�Y
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
			// �V���A���C�Y
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
			ID3D11Buffer* m_pVertexBuffer;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			ID3D11Buffer* m_pIndexBuffer;	// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			
			int m_nNumVertex;				// �����_��	
			int m_nNumIndex;				// ���C���f�b�N�X��

			Matrix4x4 m_mWorld;
			CMeshMaterial m_material;		// �}�e���A��
			
			//XMFLOAT4X4 m_mtxTexture;
			//ID3D11ShaderResourceView* m_pTexture;		// �e�N�X�`��

			static ID3D11Buffer* m_pConstantBuffer[2];	// �萔�o�b�t�@
			static ID3D11SamplerState* m_pSamplerState;	// �e�N�X�`�� �T���v��
			static ID3D11VertexShader* m_pVertexShader;	// ���_�V�F�[�_
			static ID3D11InputLayout* m_pInputLayout;	// ���_�t�H�[�}�b�g
			static ID3D11PixelShader* m_pPixelShader;	// �s�N�Z���V�F�[�_

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
