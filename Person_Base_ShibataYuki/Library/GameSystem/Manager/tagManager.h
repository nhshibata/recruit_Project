//=========================================================
// [tagManager.h]
//--------------------
// 作成:2022/12/18 tag.hから分割
//--------------------
//=========================================================

#ifndef __TAG_MANAGER_H__
#define __TAG_MANAGER_H__

//--- インクルード部
#include <string>
#include <vector>
#include <CoreSystem/Singleton.h>

namespace MySpace
{
	namespace Game
	{
		// タグの全体管理
		class CTagManager : public CSingleton<CTagManager>
		{
			friend class CSingleton<CTagManager>;
		private:
			//--- メンバ変数
			static inline std::vector<std::string> m_aTagName;
			//std::vector<std::string> m_aTagName;

		private:
			//--- メンバ関数
			CTagManager();
			~CTagManager();
		public:
			void LoadSystem();
			void SaveSystem();
			inline std::vector<std::string> GetNameList() 
			{
				return m_aTagName; 
			}

			// *@インデックス取得
			_NODISCARD int FindIdx(std::string name)
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
			bool CreateRegist(std::string name) 
			{
				if (FindIdx(name) == -1) 
				{
					m_aTagName.push_back(name);
					return true;
				}
				return false;
			}

			// *@idからstring取得
			std::string IDToTag(int id)
			{
				if (m_aTagName.size() <= id) 
				{
					return "null";
				}
				return m_aTagName[id];
			}
		};

	}
}

#endif  //!__TAG_MANAGER_H__