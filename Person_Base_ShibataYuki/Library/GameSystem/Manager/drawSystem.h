//==========================================================
// [drawSystem.h]
// 派生クラス
//---------------------------
// 作成:2022/06/07 ｸﾗｽ名変更するかも
// 更新:2022/11/09 クラス名変更(DrawManager) -> (drawSystem)
// TODO: 2D用のコンポーネントを受け取る仕様の追加
// TODO: レイヤー分け。通常のモデルとの関係。背景とUI、前景、を分けて描画
//==========================================================

//--- インクルードガード
#ifndef __DRAW_SYSTEM_H__
#define __DRAW_SYSTEM_H__

//--- インクルード部
#include <GameSystem/Manager/mapSystemBase.h>
#include <DirectXMath.h>

namespace MySpace
{
	namespace Game
	{
		class CRenderer;
	}
	namespace Graphics
	{
		class CMesh;
	}
}

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CRenderer;
		using MySpace::Graphics::CMesh;

		//--- クラス定義
		class CDrawSystem : public CMapSystemBase<std::weak_ptr<CRenderer>>
		{
		private:
			// エイリアス
			using RenderWeak = std::vector<std::weak_ptr<CRenderer>>;
			using InstancingMap = std::map<std::string, std::vector<DirectX::XMFLOAT4X4>>;
			using InstancingMeshMap = std::map<std::string, std::vector<CMesh*>>;

		private:
			//--- メンバ変数
			bool m_bIsSortNecessary = false;
			RenderWeak m_pDrawSortList;			// 管理しているmapをソートした結果を入れる変数
			InstancingMap m_aInstancingModelMap;
			InstancingMeshMap m_aInstancingMesh;

#if BUILD_MODE
			// 確認用変数
			int m_nSkipCnt;
			int m_nDrawCnt;
			int m_nInstancingCnt;
			bool m_bFrustum = true;
#endif // _DEBUG

		private:
			//--- メンバ関数
		public:
			CDrawSystem();
			~CDrawSystem();
			void Update();

			//--- 描画するコンポーネント
			// *@登録 override
			_NODISCARD int RegistToSystem(std::weak_ptr<CRenderer> render)
			{
				int ret = CMapSystemBase::RegistToSystem(render);
				m_pDrawSortList.push_back(render);
				return ret;
			}
			// *@破棄 overlide
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

			// *@インスタンシング描画のために情報を格納する
			inline void SetInstanchingModel(std::string name, DirectX::XMFLOAT4X4 mtx)
			{
				m_aInstancingModelMap[name].push_back(mtx);
			}
			
			// *@インスタンシング描画のために情報を格納する
			inline void SetInstanchingMesh(std::string name, CMesh* mesh)
			{
				m_aInstancingMesh[name].push_back(mesh);
			}

			// *@
			_NODISCARD inline std::vector<std::weak_ptr<CRenderer>> GetList()
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