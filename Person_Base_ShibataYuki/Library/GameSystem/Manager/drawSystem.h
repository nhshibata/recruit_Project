//=========================================================
// [drawSystem.h]
// 派生クラス
//---------------------------------------------------------
// 作成:2022/06/07 ｸﾗｽ名変更するかも
// 更新:2022/11/09 クラス名変更(DrawManager) -> (drawSystem)
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __DRAW_SYSTEM_H__
#define __DRAW_SYSTEM_H__

//--- インクルード部
#include <GameSystem/Manager/mapSystemBase.h>
#include <GameSystem/Shader/depthShadow.h>
#include <GraphicsSystem/Shader/instancingData.h>

#include <DirectXMath.h>

namespace MySpace
{
	namespace Game
	{
		class CRenderer;
		class CPolygonRenderer;
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
			friend class CRenderer;
			friend class CPolygonRenderer;
		private:
			struct STMeshData
			{
				CMesh* pMesh;
				std::vector<MySpace::Graphics::RENDER_DATA> aData;
			};

			//--- エイリアス
			using PolygonRenderWeakList = std::vector<std::weak_ptr<CPolygonRenderer>>;
			using InstancingMap = std::map<std::string, std::vector<MySpace::Graphics::RENDER_DATA>>;
			using InstancingMeshMap = std::map<std::string, STMeshData>;
			
		private:
			//--- メンバ変数
			bool m_bIsSortNecessary;					// 整列実行フラグ
			PolygonRenderWeakList m_aPolygonList;		// 管理しているmapをソートした結果を入れる
			InstancingMap m_aInstancingModelMap;		// インスタンシング描画格納用
			InstancingMeshMap m_aInstancingMeshMap;		// インスタンシング描画格納用
			std::unique_ptr<Game::CDepthShadow> m_pDepthShadow;	// 深度書き込み用

#if BUILD_MODE
			struct STDebugMeshData
			{
				CMesh* pMesh;
				std::vector<DirectX::XMFLOAT4X4> mtx;
			};
			// 確認用変数
			int m_nSkipCnt;
			int m_nDrawCnt;
			int m_nInstancingCnt;
			bool m_bFrustum;
			bool m_bShadowView;
			std::map<std::string,STDebugMeshData> m_aDebugMeshMap;		// インスタンシング描画格納用
#endif // _DEBUG

		private:
			//--- メンバ関数
			// *@ポリゴン整列
			void Sort();
			// *@整列ﾌﾗｸﾞ
			inline void SortOn() { m_bIsSortNecessary = true; }
			// *@3Dインスタンシング影描画
			void Draw3DShadow();
			// *@3Dインスタンシング描画
			void Draw3D();
			
		public:
			CDrawSystem();
			~CDrawSystem();

			void Init();
			void Update();

			//--- 描画するコンポーネント
			// *@登録 override
			_NODISCARD int RegistToSystem(std::weak_ptr<CRenderer> render)
			{
				int ret = CMapSystemBase::RegistToSystem(render);
				return ret;
			}
			
			// *@登録
			_NODISCARD int PolygonRegist(std::weak_ptr<CPolygonRenderer> render);

			// *@破棄 overlide
			std::weak_ptr<CRenderer> ExecutSystem(int idx);

			// *@インスタンシング描画のために情報を格納する
			inline void SetInstanchingModel(std::string name, MySpace::Graphics::RENDER_DATA data)
			{
				m_aInstancingModelMap[name].push_back(data);
			}
			
			// *@インスタンシング描画のために情報を格納する
			void SetInstanchingMesh(std::string name, MySpace::Graphics::RENDER_DATA data, CMesh* mesh);

			// *@所持リスト
			_NODISCARD inline std::vector<std::weak_ptr<CRenderer>> GetList()
			{
				std::vector<std::weak_ptr<CRenderer>> ret;
				for (auto & it : m_aIntMap)
				{
					ret.push_back(it.second);
				}
				return ret;
			}

#if BUILD_MODE

			void ImGuiDebug();
			// *@インスタンシング描画のために情報を格納する
			void SetDebugMesh(std::string name, DirectX::XMFLOAT4X4 mtx, CMesh* mesh);
#endif // BUILD_MODE

		};


#define POST_TEST	0
#if POST_TEST

		class CDrawSystemMap
		{
		private:
			std::unordered_map<int, CDrawSystem> m_aLayerMap;

		public:
			CDrawSystemMap();
			~CDrawSystemMap();

			void Update();

			// *@登録
			int Regist(std::weak_ptr<CRenderer> add, int layer) 
			{
				return m_aLayerMap[layer].RegistToSystem(add); 
			}

			// *@除外
			bool Remove(int layer, int idx)
			{
				auto res = m_aLayerMap[layer].ExecutSystem(idx);
				return (res.lock() ? false : true);
			}

			// *@レイヤー移動
			int Move(int oldLayer, int nextLayer, int idx)
			{
				// ﾎﾟｲﾝﾀ取得
				auto render = m_aLayerMap[oldLayer].IDToData(idx);
				// 現在layer除外
				if (Remove(oldLayer, idx))
				{
					// 新規登録
					return Regist(render.lock(), nextLayer);
				}
			}

		};

#endif // POST

	}
}

#endif