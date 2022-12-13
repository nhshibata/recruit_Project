//=========================================================
// [Billboard.h]
// 作成: 2022/07/04
//=========================================================

#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include <GraphicsSystem/Render/mesh.h>

namespace MySpace
{
	namespace Graphics
	{
		class CSpriteAnimation;
	}
}

namespace MySpace
{
	namespace Graphics
	{
		class CBillboard :public CMesh
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(
					cereal::make_nvp("billboard", cereal::base_class<CMesh>(this))
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(
					cereal::make_nvp("billboard", cereal::base_class<CMesh>(this))
				);
			}
		private:
			Matrix4x4 m_mtxTexture;

		public:
			CBillboard();
			~CBillboard();

			HRESULT Init();
			void Update(Vector3 pos, Vector2 size);

			void SetTextureMatrix(XMFLOAT4X4 mtx) { m_mtxTexture = mtx; }
			XMFLOAT4X4 GetTextureMatrix() { return m_mtxTexture; }

			//void Draw(ID3D11ShaderResourceView* pTexture = nullptr, XMFLOAT4X4* mWorld = nullptr) { CMesh::Draw(pTexture, mWorld); };
			//void Draw(ID3D11ShaderResourceView* tex, CSpriteAnimation* sprite = nullptr);
		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Graphics::CBillboard)

#endif // !__BILLBOARD_H__
