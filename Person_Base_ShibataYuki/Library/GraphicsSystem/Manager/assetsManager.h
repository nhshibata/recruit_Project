//========================================================
// [assetsManager.h]
//-------------------------
// �쐬:2022/12/22 
//-------------------------
// �΂�΂�̑f�ފ֌W����Z�߂�
//========================================================

#ifndef __ASSETSMANAGER_H___
#define __ASSETSMANAGER_H___

//--- �C���N���[�h��
#include <memory>
#include <CoreSystem/systemBase.h>

#pragma region ForwardDeclaration

class Application;

#pragma endregion

namespace MySpace
{
	namespace Graphics
	{
		class CImageResourceManager;
		class CFontTexture;
		class CModelManager;
		class CEffekseer;
		class CShaderManager;

		class CAssetsManager : public MySpace::System::CSystemBase
		{
		private:
			//--- �����o�ϐ�
			Application* m_pApp;

			std::shared_ptr<CImageResourceManager> m_pImage;
			std::shared_ptr<CFontTexture> m_pFont;
			std::shared_ptr<CModelManager> m_pModel;
			std::shared_ptr<CEffekseer> m_pEffekseer;
			std::shared_ptr<CShaderManager> m_pShader;

		public:
			//--- �����o�֐�
			CAssetsManager();
			~CAssetsManager();

			HRESULT Init(Application* app);

			// *@�X�V
			void Update();

			// *@�V�[���j�����ɌĂяo���A�s�v�Ȃ��̂��������
			void SceneReset();

			//--- �Q�b�^�[�E�Z�b�^�[
			_NODISCARD CImageResourceManager* GetImageManager()const	{ return m_pImage.get(); }
			_NODISCARD CFontTexture* GetFont()const						{ return m_pFont.get(); }
			_NODISCARD CModelManager* GetModelManager()const			{ return m_pModel.get(); }
			_NODISCARD CEffekseer* GetEffekseer()const					{ return m_pEffekseer.get(); }
			_NODISCARD CShaderManager* GetShaderManager()const			{ return m_pShader.get(); }

		};

	}
}

#endif // !__ASSETSMANAGER_H___

