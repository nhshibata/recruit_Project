//=========================================================
// [imageResourceManager.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __IMAGE_RESOURCE_MANAGER_H__
#define __IMAGE_RESOURCE_MANAGER_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Manager/resourceMap.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Texture/imageResource.h>

namespace MySpace
{
	namespace Graphics
	{
		//--- �N���X��`
		class CImageResourceManager : public CResourceMap<std::string, ImageSharedPtr>
		{
		private:
			using IMAGE_MAP = std::map<std::string, ImageSharedPtr>;
			using IMAGE_PAIR = std::pair<std::string, ImageSharedPtr>;

		public:
			//--- �����o�֐�
			CImageResourceManager();
			~CImageResourceManager();

			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();
			int SceneUnload();

			// *@���\�[�X�擾
			ImageSharedPtr GetResource(std::string name);
		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_