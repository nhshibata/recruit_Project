//=========================================================
// [imageResourceManager.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================


//--- �C���N���[�h�K�[�h
#ifndef __IMAGE_RESOURCE_MANAGER_H__
#define __IMAGE_RESOURCE_MANAGER_H__

//--- �C���N���[�h��
#include <CoreSystem/Singleton.h>
#include <GraphicsSystem/Manager/resourceMap.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Texture/imageResource.h>

namespace MySpace
{
	namespace Graphics
	{
		//--- �N���X��`
		class CImageResourceManager : public CSingleton<CImageResourceManager>, public CResourceMap<std::string, ImageSharedPtr>
		{
			friend class CSingleton<CImageResourceManager>;
		private:
			using IMAGE_MAP = std::map<std::string, ImageSharedPtr>;
			using IMAGE_PAIR = std::pair<std::string, ImageSharedPtr>;

		protected:
			//--- �����o�ϐ�
			CImageResourceManager();

		public:
			//--- �����o�֐�

			void Uninit();
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();

			ImageSharedPtr GetResource(std::string name);
		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_