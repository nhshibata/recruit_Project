//=========================================================
// [imageResourceManager.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#define NOMINMAX

//--- �C���N���[�h��
#include <GraphicsSystem/Manager/resourceMap.h>
#include <GraphicsSystem/Render/AssimpModel.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

namespace MySpace
{
	namespace Graphics
	{
		using ModelSharedPtr = std::shared_ptr<CAssimpModel>;
		using ModelWeakPtr = std::weak_ptr<CAssimpModel>;
	}
}

namespace MySpace
{
	namespace Graphics
	{
		//--- �N���X��`
		class CModelManager : public CResourceMap<std::string, ModelSharedPtr>
		{	
		public:
			//--- �����o�֐�
			CModelManager();
			~CModelManager();
			
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();
			int SceneUnload();

			// *@���f���擾
			// *@�Ȃ���Γǂݍ���
			ModelSharedPtr GetModel(std::string name);

			// *@���f���̎g�p���擾
			int GetModelCnt(std::string name);

		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_