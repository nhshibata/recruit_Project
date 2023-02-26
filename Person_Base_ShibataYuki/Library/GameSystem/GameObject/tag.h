//=========================================================
// [tag.h]
//--------------------------
// 作成:2022/04/19 (火曜日)
// 更新:2022/09/20 タグの比較に完全一致か部分一致かを判別できるよう変更
// 更新:2022/12/18 タグ管理ｸﾗｽをファイル分け
// 更新:2022/12/22 静的メンバ変数でリスト管理に変更
//--------------------------
// タグ : 部品クラス
//=========================================================

#ifndef __TAG_H__
#define __TAG_H__

//--- インクルード部
#include <string>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		// 基本的なタグ所持ｸﾗｽ
		class CTagDefault
		{
		public:
			static constexpr const char* DEFAULT =		"Default";
			static constexpr const char* MAIN_CAMERA =	"MainCamera";
			static constexpr const char* SUB_CAMERA =	"SubCamera";
			static constexpr const char* LIGHT =		"Light";
		};

		//--- クラス定義
		class CTag
		{
		private:
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				//archive(cereal::make_nvp(GetTag(), m_nTagID));
				archive(CEREAL_NVP(m_nTagID));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_nTagID));
			}
		private:
			//--- メンバ変数
			int m_nTagID = 0;
			static inline std::vector<std::string> m_aTagName;
			
		private:
			//--- メンバ関数

			// *@インデックス取得
			_NODISCARD static int FindIdx(std::string name)
			{
				int id = 0;
				for (std::vector<std::string>::iterator it = m_aTagName.begin(); it != m_aTagName.end(); ++it, ++id)
				{
					if ((*it) == name)
						return id;
				}
				return -1;
			}

			// *@タグ生成(登録)
			// *@登録完了 / 必要なし
			static bool CreateRegist(std::string name)
			{
				if (FindIdx(name) == -1)
				{
					m_aTagName.push_back(name);
					return true;
				}
				return false;
			}

			// *@idからstring取得
			static std::string IDToTag(int id)
			{
				if (id < 0 || m_aTagName.size() <= id)
				{
					return "Unregistered";
				}
				return m_aTagName[id];
			}

		public:
			CTag();
			CTag(std::string name);
			~CTag();

			// *@登録完了 / 必要なし
			// *@なければ生成
			static bool CreateTag(std::string name) 
			{ 
				bool ret = CreateRegist(name);
				return ret;
			}

			//--- セッター・ゲッター
			inline std::string GetTag() { return IDToTag(m_nTagID); }

			// *@※注意 
			// *@事前にCreateTag関数を呼び出す必要あり
			// *@登録、生成されていなければ-1が入る
			inline void SetTag(std::string name) { m_nTagID = FindIdx(name); }

			// TODO: エラーだしたい
			// *@指定のタグか確認
			// *@return (完全一致:1, 部分一致:2, 一致なし:0)
			int Compare(std::string name)
			{
				if (auto tag = IDToTag(m_nTagID); name == tag) { return 1; }
				else if (tag.find(name) != std::string::npos) { return 2; }
				return 0; 
			}

			// *@リスト取得
			static inline std::vector<std::string> GetNameList() { return m_aTagName; }

			// *@ファイル保存
			static void SaveSystem();

			// *@ファイル読み込み
			static void LoadSystem();

#ifdef BUILD_MODE
			// *@ImGui表示
			static void ImGuiTag(bool& disp);
#endif // BUILD_MODE

		};
	}
}

#endif  //__TAG_H__