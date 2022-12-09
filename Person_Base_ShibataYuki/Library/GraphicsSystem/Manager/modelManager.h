//=========================================================
// [imageResourceManager.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================


//--- �C���N���[�h�K�[�h
#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

//--- �C���N���[�h��
#define NOMINMAX

#include <CoreSystem/Singleton.h>
#include <GraphicsSystem/Manager/resourceMap.h>
#include <GraphicsSystem/Render/AssimpModel.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <map>

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

		public:
			//~CImageResourceManager();

			HRESULT Init();
			void Uninit();
			
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();

			// *@���f���擾
			// *@�Ȃ���Γǂݍ���
			ModelSharedPtr GetModel(std::string name) 
			{ 
				if (auto it = m_ResourceMap.find(name); it == m_ResourceMap.end()) {
					if (!Load(name)) 
					{ 
						return ModelSharedPtr(); 
					} 
				}
				return m_ResourceMap[name];
			};

			// *@���f���̎g�p���擾
			int GetModelCnt(std::string name)
			{
				if (m_ResourceMap.count(name))
				{
					return m_ResourceMap[name].use_count();
				}
				return 0;
			}

		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_