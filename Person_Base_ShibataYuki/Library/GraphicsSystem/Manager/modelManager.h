//=========================================================
// [imageResourceManager.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#define NOMINMAX

//--- インクルード部
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
		//--- クラス定義
		class CModelManager : public CResourceMap<std::string, ModelSharedPtr>
		{	
		public:
			//--- メンバ関数
			CModelManager();
			~CModelManager();
			
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();
			int SceneUnload();

			// *@モデル取得
			// *@なければ読み込み
			ModelSharedPtr GetModel(std::string name);

			// *@モデルの使用数取得
			int GetModelCnt(std::string name);

		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_