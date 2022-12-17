//=========================================================
// [box.h]
// �쐬: 2022/07/04
//=========================================================

#ifndef __BOX_MESH_H__
#define __BOX_MESH_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Render/mesh.h>

namespace MySpace
{	
	namespace Graphics
	{
		class CBox :public CMesh
		{
		private:
			//--- �V���A���C�Y
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
			//--- �����o�֐�
			CBox();
			virtual ~CBox();

			HRESULT Init(XMFLOAT3 vBBox);
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Graphics::CBox)

#endif // !__BOX_MESH_H__
