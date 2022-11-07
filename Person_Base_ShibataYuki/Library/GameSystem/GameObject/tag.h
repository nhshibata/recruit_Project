//=========================================================
// 作成:2022/04/19 (火曜日)
// 更新:2022/09/20 タグの比較に完全一致か部分一致かを判別できるよう変更
// タグ : 部品クラス
//=========================================================
#ifndef __TAG_H__
#define __TAG_H__

//--- インクルード部
#include <string>
#include <vector>
#include <CoreSystem/Util/cerealCommon.h>
#include <CoreSystem/system.h>

namespace MySpace
{
	namespace Game
	{
		using MySpace::System::CAppSystem;

		// 基本的なタグ所持ｸﾗｽ
		class CDefaultTagChar
		{
		public:
			static constexpr const char* DEFAULT = "Default";
			static constexpr const char* CAMERA = "MainCamera";
			static constexpr const char* LIGHT = "Light";
		};

		// タグの全体管理
		class CTagName : public CAppSystem<CTagName>
		{
			friend class CAppSystem<CTagName>;
			friend class CSingleton<CTagName>;
		private:
			std::vector<std::string> m_tagVec;
		private:
			CTagName();
			~CTagName();

		public:
			void LoadSystem();
			void SaveSystem();

			std::vector<std::string> GetList() 
			{
				return m_tagVec; 
			}

			int Find(std::string name)
			{
				int id = 0;
				for (std::vector<std::string>::iterator it = m_tagVec.begin(); it != m_tagVec.end(); ++it, ++id)
				{
					if ((*it) == name) { return id; }
				}
				return -1;
			}

			// 登録完了 / 必要なし
			bool CreateTag(std::string name) 
			{
				if (Find(name) == -1) { m_tagVec.push_back(name); return true; }
				return false;
			}

			std::string GetTag(int id)
			{
				if (m_tagVec.size() <= id) { return "null"; }return m_tagVec[id];
			}
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
			// 代入
			inline int GetID(std::string name) { return CTagName::Get().Find(name); }
		public:
			CTag();
			CTag(std::string name);
			~CTag();

			// 登録完了 / 必要なし
			// なければ生成。どちらにせよ登録される
			bool CreateTag(std::string name) 
			{ 
				bool ret = CTagName::Get().CreateTag(name);
				SetTag(name);
				return ret;
			}

			//--- セッター・ゲッター
			inline std::string GetTag() { return CTagName::Get().GetTag(m_nTagID); }
			// ※注意 
			// 事前にCreateTag関数を呼び出す必要あり
			// 登録、生成されていなければ-1が入る
			inline void SetTag(std::string name) { m_nTagID = GetID(name); }

			// TODO: エラーだしたい
			// *@指定のタグか確認
			// *@return (完全一致:1, 部分一致:2, 一致なし:0)
			int Compare(std::string name)
			{
				if (auto tag = CTagName::Get().GetTag(m_nTagID); name == tag) { return true; }
				else if (tag.find(name) != std::string::npos) { return 2; }
				return false; 
			}

		};
	}
}

#endif  //__TAG_H__