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
		//--- �N���X��`
		class CModelManager : public CSingleton<CModelManager>, public CResourceMap<std::string, ModelSharedPtr>
		{
			friend class CSingleton<CModelManager>;
		private:
			//--- �����o�֐�
			CModelManager();
			~CModelManager() = default;
		public:
			HRESULT Init();
			void Uninit();
			
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();
			int SceneUnload();

			// *@���f���擾
			// *@�Ȃ���Γǂݍ���
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

			// *@���f���̎g�p���擾
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