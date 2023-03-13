//=========================================================
// [imageResource.h] 
// �쐬:2022/06/19
//---------------------------------------------------------
// ø����ێ��׽
// 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __IMAGE_RESOURCE_H__
#define __IMAGE_RESOURCE_H__

//--- �C���N���[�h��
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <string>

//--- �G�C���A�X
namespace MySpace
{
	namespace Graphics
	{
		class CImageResource;
		using ImageSharedPtr = std::shared_ptr<CImageResource>;
		using ImageWeakPtr = std::weak_ptr<CImageResource>;
	}
}
	
namespace MySpace
{
	namespace Graphics
	{
		//--- �O���Q��

		//--- �N���X��`
		class CImageResource
		{
		private:
		protected:
			//--- �����o�ϐ�
			ID3D11ShaderResourceView* m_pSRV;
			ID3D11Texture2D* m_pTex;
			//ID3D11Resource* res;

		public:
			//--- �����o�֐�
			CImageResource();
			~CImageResource();

			// �Q�b�^�[�E�Z�b�^�[
			// *@ø����ޭ��擾
			inline ID3D11ShaderResourceView* GetSRV() const { return m_pSRV; }
			// *@ø����擾
			inline ID3D11Texture2D* GetTexter() const { return m_pTex; }

			bool Load(std::string _fileName);
			void Unload();
		};
	}
}
#endif // !__IMAGE_RESOURCE_H__
