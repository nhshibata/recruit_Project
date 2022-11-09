//==========================================================
// [drawSystem.h]
// 派生クラス
//---------------------------
// 2022/06/07 作成 ｸﾗｽ名変更するかも
// 更新:2022/11/09 クラス名変更(DrawManager) -> (drawSystem)
// TODO: 2D用のコンポーネントを受け取る仕様の追加
// TODO: レイヤー分け。通常のモデルとの関係。背景とUI、前景、を分けて描画
//==========================================================

//--- インクルードガード
#ifndef __DRAW_SYSTEM_H__
#define __DRAW_SYSTEM_H__

//--- インクルード部
#include <GameSystem/Manager/mapSystemBase.h>

namespace MySpace
{
	namespace Game
	{
		class CRenderer;
	}
}

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CRenderer;

		//--- クラス定義
		class CDrawSystem : public CMapSystemBase<std::weak_ptr<CRenderer>>
		{
		private:
			// エイリアス
			using RenderWeak = std::vector<std::weak_ptr<CRenderer>>;
			//using RenderSort = std::vector<STDrawSet>;
		private:
			//--- メンバ変数
			bool m_bIsSortNecessary = false;

			RenderWeak m_pDrawSortList;

#if BUILD_MODE
			int m_nSkipCnt;
			int m_nDrawCnt;
#endif // _DEBUG

		private:
			//--- メンバ関数
		public:
			CDrawSystem();
			~CDrawSystem();
			void Update();

			//--- 描画するコンポーネント
			//
			int RegistToSystem(std::weak_ptr<CRenderer> render)
			{
				int ret = CMapSystemBase::RegistToSystem(render);
				m_pDrawSortList.push_back(render);
				return ret;
			}
			// 破棄 overlide
			std::weak_ptr<CRenderer> ExecutSystem(int idx)
			{
				auto release = CMapSystemBase::ExecutSystem(idx).lock();

				// 破棄されたｺﾝﾎﾟｰﾈﾝﾄを探し、整列リストから除外
				for (auto it = m_pDrawSortList.begin(); it != m_pDrawSortList.end();)
				{
					if (release == (*it).lock())
					{
						m_pDrawSortList.erase(it);
						break;
					}
				}
				return release;
			}

			std::vector<std::weak_ptr<CRenderer>> GetList()
			{
				return m_pDrawSortList;
			}

#if BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
#endif