//=========================================================
// [sphere.h]
// 作成: 2022/07/04
//=========================================================

#ifndef __SPHERE_MESH_H__
#define __SPHERE_MESH_H__

#include <GraphicsSystem/Render/mesh.h>

namespace MySpace
{
	namespace Graphics
	{
		class CSphere : public CMesh
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(
					cereal::make_nvp("sphereMesh", cereal::base_class<CMesh>(this))
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(
					cereal::make_nvp("sphereMesh", cereal::base_class<CMesh>(this))
				);
			}

		public:
			//--- メンバ関数
			CSphere();
			virtual ~CSphere();

			HRESULT Init(int nSlice = 16, int nStack = 8, float fRadius = 0.5f);
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CSphere)

#endif // !__SPHERE_MESH_H__
