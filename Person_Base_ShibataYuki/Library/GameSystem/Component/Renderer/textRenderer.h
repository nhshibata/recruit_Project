//=========================================================
// [textRenerer.h] 
//---------------------------------------------------------
// 作成:2022/08/04
// 更新:2023/01/13 CRendererからCPolygonRendererへ継承元を変更
//				(ポリゴン使ってるのに継承しないのは...?)
//---------------------------------------------------------
// polygonを通して文字描画を行う
//=========================================================

//--- インクルードガード
#ifndef __TEXT_RENERER_H__
#define __TEXT_RENERER_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <CoreSystem/Util/stringConvert.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		//--- クラス定義
		class CTextRenderer : public CPolygonRenderer
		{
		private:
			// シリアライズ
#pragma region CEREAL
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				std::string text = WStringToString(m_Text);	// wstringを保存するためstringとして変換
				archive(cereal::make_nvp("TextRender", cereal::base_class<CPolygonRenderer>(this)),
					CEREAL_NVP(m_fOffset), CEREAL_NVP(m_bVerticalWrit),
					CEREAL_NVP(text)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				std::string text; // 文字列受け取り
				archive(cereal::make_nvp("TextRender", cereal::base_class<CPolygonRenderer>(this)),
					CEREAL_NVP(m_fOffset), CEREAL_NVP(m_bVerticalWrit),
					CEREAL_NVP(text)
				);
				// 受け取った文字列をワイド文字に変換
				m_Text = StringToWString(text);
			}
#pragma endregion

		private:
			//--- メンバ変数
			RectTransWeakPtr m_pRectTransform;
			std::vector<CFontTexture::STCharaData> m_aTexList;	// ○○番目の文字取得などがしやすいvector
			
			std::wstring m_Font;		// 読み込みフォント名
			std::wstring m_Text;		// 文字列
			size_t m_uOldTextSize;		// 文字列サイズ
			float m_fOffset;			// 文字列間隔
			bool m_bVerticalWrit;		// 縦書きか否か

		public:
			//--- メンバ関数
			CTextRenderer()
				:m_Font(std::wstring()), m_Text(std::wstring()), m_uOldTextSize(0), m_fOffset(0.0f), m_bVerticalWrit(false)
			{};
			CTextRenderer(std::shared_ptr<CGameObject> owner);
			~CTextRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- ゲッター・セッター
			// *@font設定
			inline void SetFont(std::wstring text) { m_Font = text; }
			
			// *@テキストに何を表示させるか
			// *@ワイド文字指定
			void SetTextWString(std::wstring text);

			// *@文字間距離の設定
			inline void SetOffSet(float offset) { m_fOffset = offset; }

			// *@縦描画フラグ
			inline void Vertical(bool flg) { m_bVerticalWrit = flg; }

#ifdef BUILD_MODE
		private:
			static inline int m_nBulidCreateNum;
		public:
			virtual void ImGuiDebug();
			// *@仮想キーをwstringとして取得
			std::wstring VKeyToWString(int nKey);

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CTextRenderer)

#endif //__TEXT_RENERER_H__