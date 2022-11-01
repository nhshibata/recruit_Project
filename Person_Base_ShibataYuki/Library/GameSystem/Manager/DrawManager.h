//==========================================================
// [DrawManager.h]
// 派生クラス
// 2022/06/07 作成
// ｸﾗｽ名変更するかも
// TODO: 2D用のコンポーネントを受け取る仕様の追加
// TODO: レイヤー分け。通常のモデルとの関係。背景とUI、前景、を分けて描画
//==========================================================

//--- インクルードガード
#ifndef __DRAW_MANAGER_H__
#define __DRAW_MANAGER_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CDrawManager
		{
		private:
			//struct ST_RenderData
			//{
			//	std::weak_ptr<CRenderer> pRender;
			//	//UINT uLayer;
			//	/*ST_RenderData(std::weak_ptr<CRenderer> render, UINT layer)
			//		:pRender(render),uLayer(layer)
			//	{}
			//	static std::shared_ptr<ST_RenderData> Make(std::weak_ptr<CRenderer> render, UINT layer)
			//	{
			//		return std::make_shared<ST_RenderData>(render, layer);
			//	}*/
			//};
			// エイリアス
			using RenderList = std::vector<std::weak_ptr<CRenderer> >;
			using RenderVec = std::vector<std::weak_ptr<CRenderer>>;
			using RenderWeak = std::vector<std::weak_ptr<CRenderer>>;
			//using RenderSort = std::vector<STDrawSet>;
		private:
			//SpriteRenderList m_SpriteList;
			bool m_bIsSort;
			//RenderList m_ObjectList;

			RenderVec m_pObjectList;
			RenderWeak m_pSortList;
		private:
			void EraseDraw()
			{
				for (auto it = m_pSortList.begin(); it != m_pSortList.end();)
				{
					if (!(*it).lock())
					{
						it = m_pSortList.erase(it);
						continue;
					}
					++it;
				}
			}

		public:
			CDrawManager();
			~CDrawManager();
			void Update();

			//--- 描画するコンポーネントの設定
			
			//inline void SetDraw(std::weak_ptr<CRenderer> com) { m_ObjectList.push_back(com); m_bIsSort = true; }
			inline UINT SetDraw(std::weak_ptr<CRenderer> com, int idx) 
			{
				int Idx = static_cast<int>(m_pObjectList.size());
				m_pObjectList.push_back(com);
				m_pSortList.push_back(m_pObjectList.back());
				m_bIsSort = true; 
				return Idx; 
			}
			// vector型
			bool ReleaseDraw(UINT idx)
			{
				if (static_cast<int>(m_pObjectList.size()) == 0 )
					return false;
				
				// 監視を切る
				m_pObjectList[idx].reset();

				// 入れ替えられるか確認
				if (static_cast<int>(m_pObjectList.size()) <= 1)
				{
					m_pObjectList.clear();
					return false;
				}
				// 最後尾なら除外
				if (idx == static_cast<int>(m_pObjectList.size() - 1))
				{
					m_pObjectList.pop_back();
					return true;
				}
				// indexの置き換え
				m_pObjectList[idx] = m_pObjectList.back();
				m_pObjectList.back().lock()->SetIdx(idx);
				m_pObjectList.pop_back();
				return true;
			}

			std::vector<std::weak_ptr<CRenderer>> GetList()
			{
				EraseDraw();
				return m_pObjectList;
			}
			// @描画リストの受け取り
			void DrawPass(std::vector<std::weak_ptr<CRenderer>> list)
			{
				for (int cnt = 0; cnt < static_cast<int>(list.size()); ++cnt)
				{
					if (!list[cnt].lock())
						continue;
					int idx = SetDraw(list[cnt], 0);
					m_pObjectList.back().lock()->SetIdx(idx);
				}
				m_bIsSort = true;
			}

			void ImGuiDebug();
		};
	}
}
#endif