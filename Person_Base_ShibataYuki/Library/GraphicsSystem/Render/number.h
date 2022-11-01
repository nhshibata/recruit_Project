//=========================================================
// 作成:2022/04/24
// 数字表示 : 基底ｸﾗｽ
//=========================================================
#ifndef __NUMBER_H__
#define __NUMBER_H__

//---インクルード部
#include <GraphicsSystem/Render/polygon.h>
#include <vector>
#include <DirectXTex.h>

//---定数定義
#define NUMBER		CNumber::Get()

namespace MySpace
{
	namespace Graphics
	{
		using namespace DirectX;

		//---クラス定義
		class CNumber
		{
		private:
			typedef struct
			{
				XMFLOAT2 pos;
				unsigned Number;
				int width;
				XMFLOAT2 size;
			}STNumberData;
		private:
			ID3D11ShaderResourceView* m_pTexture;
			static CNumber* m_pInstance;
			std::vector<STNumberData> m_numberData;
		public:
			CNumber();
			~CNumber();
			void Set(XMFLOAT2 vPos,		// 表示位置(左上)
				unsigned uNumber,			// 表示数値
				int nWidth,					// 表示桁数
				float fSizeX,				// 1文字の幅
				float fSizeY);				// 1文字の高さ
			void Update();
			void Draw();
			// 
			static void Start() { m_pInstance = new CNumber(); };
			static void End() { delete m_pInstance; };
			static CNumber* Get() { return m_pInstance; };
		};

	}
}

#endif // !__NUMBER_H__
