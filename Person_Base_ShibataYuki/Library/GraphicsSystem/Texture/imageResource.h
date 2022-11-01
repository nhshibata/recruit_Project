//=========================================================
// [imageResource.h] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __IMAGE_RESOURCE_H__
#define __IMAGE_RESOURCE_H__


//--- インクルード部
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <string>

//--- エイリアス
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
		//--- 前方参照

		//--- クラス定義
		class CImageResource
		{
		private:
			ID3D11ShaderResourceView* srv;
			ID3D11Resource* res;
		public:
			CImageResource();
			~CImageResource();

			// ゲッター・セッター
			inline ID3D11ShaderResourceView* GetSRV() const { return srv; }
			inline ID3D11Resource* GetRES() const { return res; }

			bool Load(std::string _fileName);
			void Unload();
		};
	}
}
#endif // !__IMAGE_RESOURCE_H__
