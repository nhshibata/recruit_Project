//=========================================================
// [FontTexture.h] 
// 作成: 2022/08/04
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __FONT_TEXTURE_H__
#define __FONT_TEXTURE_H__

//--- インクルード部
#include <d3d11.h>
#include <CoreSystem/system.h>
#include <CoreSystem/Util/stl.h>

namespace MySpace
{
	namespace Graphics
	{
		class CFontTexture : public CAppSystem< CFontTexture>
		{
			friend class Application;
			friend class CAppSystem<CFontTexture>;

		public:
			//--- 構造体定義
			struct STCharaData
			{
				ID3D11ShaderResourceView* pTex;
				int Width;
				int Height;
				int BMPWidth;
				int BMPHeight;
				int OffSetX;
				int OffSetY;
			};
		private:
			// ｴｲﾘｱｽ
			using CharTexMap = std::map<LPCWSTR, STCharaData>;		// なんの文字か、どんなパラメータか
			using CharTexPair = std::pair<LPCWSTR, STCharaData>;
			using FontMap = std::map<std::wstring, CharTexMap>;		// フォント
			using FontPair = std::pair<std::wstring, CharTexMap>;
		private:
			HDC m_hdc;					// 
			HFONT m_hFont;				// 
			HFONT m_hOldFont;			// 

			FontMap m_StringMap;		// 格納用
			std::wstring m_FontName;	// 現在保持フォント名
		
		public:
			//--- メンバ関数
			CFontTexture();
			~CFontTexture();

			HRESULT Init();
			void Uninit();

			void DesignFont(std::string fontPath, std::wstring fontName);	// フォント変更用 ※変更にはてきしていない状態

			// 文字ﾃｸｽﾁｬの作成(空ならＭＳ Ｐ明朝)
			// *@引き数:wstring L""
			CFontTexture::STCharaData CreateTex(LPCWSTR character, std::wstring fontName = std::wstring());
			// 文字列を受け取り、ﾃｸｽﾁｬを返す
			std::vector<CFontTexture::STCharaData> GetString(std::wstring text, std::wstring fontName = std::wstring());	

		};
	}
}

#endif //__FONT_TEXTURE_H__