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
#include <CoreSystem/Singleton.h>
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
		class CModelManager : public CSingleton<CModelManager>, public CResourceMap<std::string, ModelSharedPtr>
		{
			friend class CSingleton<CModelManager>;
		private:
			//--- メンバ関数
			CModelManager();
			~CModelManager() = default;
		public:
			HRESULT Init();
			void Uninit();
			
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();
			int SceneUnload();

			// *@モデル取得
			// *@なければ読み込み
			ModelSharedPtr GetModel(std::string name) 
			{ 
				if (auto it = m_aResourceMap.find(name); it == m_aResourceMap.end()) {
					if (!Load(name)) 
					{ 
						return ModelSharedPtr(); 
					} 
				}
				return m_aResourceMap[name];
			};

			// *@モデルの使用数取得
			inline int GetModelCnt(std::string name)
			{
				if (m_aResourceMap.count(name))
				{
					return m_aResourceMap[name].use_count();
				}
				return 0;
			}

		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_