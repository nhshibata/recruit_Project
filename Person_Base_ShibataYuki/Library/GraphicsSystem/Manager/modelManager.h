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

#include <CoreSystem/system.h>
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
		//enum class EModelType
		//{
		//	MODEL_PLAYER = 0,	// �v���C���[ ���f��
		//	MODEL_SKY,			// �X�J�C�h�[��
		//	MODEL_LAND,			// �n�ʃ��f��
		//	NONE,
		//	MAX,
		//};
		//--- �O���Q��

		//--- �N���X��`
		class CModelManager : public CAppSystem<CModelManager>, public CResourceMap<std::string, ModelSharedPtr>
		{
			friend class CAppSystem<CModelManager>;
			friend class CSingleton<CModelManager>;
		private:
			
			//ModelSharedPtr m_pModels[int(EModelType::MAX)];
			//std::map<std::string, ModelSharedPtr> m_pMapModels;

			CModelManager();
		public:
			//~CImageResourceManager();

			HRESULT Init();
			void Uninit();
			
			bool Load(std::string name);
			bool Unload(std::string name);
			void UnloadAll();

			// ���f���擾
			// �Ȃ���Γǂݍ���
			ModelSharedPtr GetModel(std::string name) 
			{ 
				if (auto it = m_ResourceMap.find(name); it == m_ResourceMap.end()) {
					if (!Load(name)) 
					{ return ModelSharedPtr(); } }

				return m_ResourceMap[name]; 
			};
		};

	}
}

#endif // !__IMAGE_RESOURCE_MANAGER_H_