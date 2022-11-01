//=========================================================
// [imageResource.h] 
// �쐬:2022/06/19
//---------------------------------------------------------
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
			ID3D11ShaderResourceView* srv;
			ID3D11Resource* res;
		public:
			CImageResource();
			~CImageResource();

			// �Q�b�^�[�E�Z�b�^�[
			inline ID3D11ShaderResourceView* GetSRV() const { return srv; }
			inline ID3D11Resource* GetRES() const { return res; }

			bool Load(std::string _fileName);
			void Unload();
		};
	}
}
#endif // !__IMAGE_RESOURCE_H__
