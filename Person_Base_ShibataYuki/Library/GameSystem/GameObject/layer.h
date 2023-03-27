//=========================================================
// [layer.h]
//---------------------------------------------------------
// 作成:2022/04/19
// 更新:2023/02/14 ビットに対応できるよう変更
//---------------------------------------------------------
// レイヤーｸﾗｽ : 部品ｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __LAYER_H__
#define __LAYER_H__

//--- インクルード部
#include <CoreSystem/Util/cerealCommon.h>
#include <string>

//--- 定数定義
#ifndef NAME_TO
#define NAME_TO(name)		#name
#endif // NAME_TO

#define MAP_SET(key, value)	static_cast<int>(key), NAME_TO(value)

namespace MySpace
{
	namespace Game
	{
		class CLayer
		{
		private:
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(m_nLayer));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_nLayer));
			}
		private:
			//--- メンバ変数
			int m_nLayer;

			// 登録用静的メンバ変数
			static std::map<int, std::string> m_aLayerMap;

		public:
			//--- メンバ関数
			CLayer();
			CLayer(int layer);
			~CLayer();

			//--- ゲッター・セッター
			inline int GetLayer() { return m_nLayer; }
			inline std::string GetName() { return NumberToName(m_nLayer); }
			void SetLayer(int layer);
			void SetLayer(std::string layer);

			// *@静的メンバ関数
			// *@新規登録
			static bool Regist(int no, std::string registName);
			// *@静的メンバ関数
			// *@名前を数字に
			static int GetNumber(std::string name);
			// *@静的メンバ関数
			// *@数字を名前に
			static std::string NumberToName(int No);
			// *@静的メンバ関数
			// *@数字を二進数変換
			static int NumberToBit(int no);
			// *@静的メンバ関数
			// *@ファイル読み込み
			static void LoadSystem();
			// *@静的メンバ関数
			// *@ファイル書き出し
			static void SaveSystem();

#ifdef BUILD_MODE
			static std::vector<std::string> GetNameList(bool bId = false);
			static void ImGuiLayer(bool& disp);
			static int ImGuiSetLayerList(int bit);
#endif // BUILD_MODE


		};


	}
}

#endif // !__LAYER_H__
