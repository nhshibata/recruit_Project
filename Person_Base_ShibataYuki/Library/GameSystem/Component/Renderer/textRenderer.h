//=========================================================
// [textRenerer.h] 
// 作成: 2022/08/04
//---------------------------------------------------------
// polygonを通して文字描画を行う
//=========================================================

//--- インクルードガード
#ifndef __TEXT_RENERER_H__
#define __TEXT_RENERER_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <CoreSystem/Util/stringConvert.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		//--- クラス定義
		class CTextRenderer : public CRenderer
		{
		private:
			// シリアライズ
#pragma region CEREAL
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				std::string text = WStringToString(m_Text);	// wstringを保存するためstringとして変換
				archive(cereal::make_nvp("TextRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_fOffset), CEREAL_NVP(m_bVerticalWrit),
					CEREAL_NVP(text)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				std::string text; // 文字列受け取り
				archive(cereal::make_nvp("TextRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_fOffset), CEREAL_NVP(m_bVerticalWrit),
					CEREAL_NVP(text)
				);
				// 受け取った文字列をワイド文字に変換
				m_Text = StringToWString(text);
			}
#pragma endregion

		private:
			//--- メンバ変数
			RectTransWeakPtr m_pRectTransform;					// 描画情報
			std::vector<CFontTexture::STCharaData> m_pTexList;	// ○○番目の文字取得などがしやすいvector
			
			std::wstring m_Font;		// 読み込みフォント名
			std::wstring m_Text;		// 文字列
			size_t m_uOldTextSize;		// 文字列サイズ
			float m_fOffset;			// 文字列間隔
			bool m_bVerticalWrit;		// 縦書きか否か

		public:
			//--- メンバ関数
			CTextRenderer() {};
			CTextRenderer(std::shared_ptr<CGameObject> owner);
			~CTextRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- ゲッター・セッター
			RectTransSharedPtr GetRectTransform() { return m_pRectTransform.lock(); }

			// *@font設定
			inline void SetFont(std::wstring text) { m_Font = text; }
			// *@テキストに何を表示させるか
			// *@ワイド文字
			inline void SetTextWString(std::wstring text) { m_Text = text; m_pTexList = CFontTexture::Get().GetString(m_Text, m_Font);}

			// *@文字間距離の設定
			inline void SetOffSet(float offset) { m_fOffset = offset; }

			// *@縦描画フラグ
			inline void Vertical(bool flg) { m_bVerticalWrit = flg; }

#ifdef BUILD_MODE

			virtual void ImGuiDebug();
			// *@仮想キーをwstringとして取得
			std::wstring VKeyToWString(int nKey);

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CTextRenderer)

#endif //__TEXT_RENERER_H__