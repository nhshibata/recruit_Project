//=========================================================
// [tag.h]
//--------------------------
// 作成:2022/04/19 (火曜日)
// 更新:2022/09/20 タグの比較に完全一致か部分一致かを判別できるよう変更
// 更新:2022/12/18 タグ管理ｸﾗｽをファイル分け
//--------------------------
// タグ : 部品クラス
//=========================================================

#ifndef __TAG_H__
#define __TAG_H__

//--- インクルード部
#include <GameSystem/Manager/tagManager.h>
#include <string>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		// 基本的なタグ所持ｸﾗｽ
		class CDefaultTagChar
		{
		public:
			static constexpr const char* DEFAULT = "Default";
			static constexpr const char* CAMERA = "MainCamera";
			static constexpr const char* LIGHT = "Light";
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
			
		private:
			//--- メンバ関数
			inline int GetID(std::string name) { return CTagManager::Get()->FindIdx(name); }
		public:
			CTag();
			CTag(std::string name);
			~CTag();

			// *@登録完了 / 必要なし
			// *@なければ生成。どちらにせよ登録される
			bool CreateTag(std::string name) 
			{ 
				bool ret = CTagManager::Get()->CreateRegist(name);
				SetTag(name);
				return ret;
			}

			//--- セッター・ゲッター
			inline std::string GetTag() { return CTagManager::Get()->IDToTag(m_nTagID); }
			// *@※注意 
			// *@事前にCreateTag関数を呼び出す必要あり
			// *@登録、生成されていなければ-1が入る
			inline void SetTag(std::string name) { m_nTagID = GetID(name); }

			// TODO: エラーだしたい
			// *@指定のタグか確認
			// *@return (完全一致:1, 部分一致:2, 一致なし:0)
			int Compare(std::string name)
			{
				if (auto tag = CTagManager::Get()->IDToTag(m_nTagID); name == tag) { return true; }
				else if (tag.find(name) != std::string::npos) { return 2; }
				return false; 
			}

		};
	}
}

#endif  //__TAG_H__