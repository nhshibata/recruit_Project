//=========================================================
// [box.h]
// 作成: 2022/07/04
//=========================================================

#ifndef __BOX_MESH_H__
#define __BOX_MESH_H__

//--- インクルード部
#include <GraphicsSystem/Render/mesh.h>

namespace MySpace
{	
	namespace Graphics
	{
		class CBox :public CMesh
		{
		private:
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(
					cereal::make_nvp("boxMesh", cereal::base_class<CMesh>(this))
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(
					cereal::make_nvp("boxMesh", cereal::base_class<CMesh>(this))
				);
			}
		public:
			//--- メンバ関数
			CBox();
			virtual ~CBox();

			HRESULT Init(XMFLOAT3 vBBox);
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CBox)

#endif // !__BOX_MESH_H__
