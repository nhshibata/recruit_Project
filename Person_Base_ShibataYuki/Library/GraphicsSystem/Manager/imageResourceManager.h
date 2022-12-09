//=========================================================
// [imageResourceManager.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================


//--- インクルードガード
#ifndef __IMAGE_RESOURCE_MANAGER_H__
#define __IMAGE_RESOURCE_MANAGER_H__

//--- インクルード部
#include <CoreSystem/Singleton.h>
#include <GraphicsSystem/Manager/resourceMap.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GraphicsSystem/Texture/imageResource.h>

namespace MySpace
{
	namespace Graphics
	{
		//--- クラス定義
		class CImageResourceManager : public CSingleton<CImageResourceManager>, public CResourceMap<std::string, ImageSharedPtr>
		{
			friend class CSingleton<CImageResourceManager>;
		private:
			using IMAGE_MAP = std::map<std::string, ImageSharedPtr>;
			using IMAGE_PAIR = std::pair<std::string, ImageSharedPtr>;

		protected:
			//--- メンバ変数
			CImageResourceManager();

		public:
			//--- メンバ関数

			void Uninit();
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();

			ImageSharedPtr GetResource(std::string name);
		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_