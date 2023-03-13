//=========================================================
// [imageResource.h] 
// 作成:2022/06/19
//---------------------------------------------------------
// ﾃｸｽﾁｬ保持ｸﾗｽ
// 
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
		protected:
			//--- メンバ変数
			ID3D11ShaderResourceView* m_pSRV;
			ID3D11Texture2D* m_pTex;
			//ID3D11Resource* res;

		public:
			//--- メンバ関数
			CImageResource();
			~CImageResource();

			// ゲッター・セッター
			// *@ﾃｸｽﾁｬﾋﾞｭｰ取得
			inline ID3D11ShaderResourceView* GetSRV() const { return m_pSRV; }
			// *@ﾃｸｽﾁｬ取得
			inline ID3D11Texture2D* GetTexter() const { return m_pTex; }

			bool Load(std::string _fileName);
			void Unload();
		};
	}
}
#endif // !__IMAGE_RESOURCE_H__
