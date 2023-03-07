//=========================================================
// [drawSystem.h]
// 派生クラス
//---------------------------------------------------------
// 
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __DRAW_SYSTEM_BASE_H__
#define __DRAW_SYSTEM_BASE_H__

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
		class CDrawSystemBase : public CMapSystemBase<std::weak_ptr<CRenderer>>
		{
			friend class CRenderer;
			friend class CPolygonRenderer;
		protected:
			struct STMeshData
			{
				CMesh* pMesh;
				std::vector<MySpace::Graphics::RENDER_DATA> aData;
			};

			//--- エイリアス
			using PolygonRenderWeakList = std::vector<std::weak_ptr<CPolygonRenderer>>;
			using InstancingMap = std::map<std::string, std::vector<MySpace::Graphics::RENDER_DATA>>;
			using InstancingMeshMap = std::map<std::string, STMeshData>;

		protected:
			//--- メンバ変数
			bool m_bIsSortNecessary;							// 整列実行フラグ
			PolygonRenderWeakList m_aPolygonList;				// 管理しているmapをソートした結果を入れる
			InstancingMap m_aInstancingModelMap;				// インスタンシング描画格納用
			InstancingMeshMap m_aInstancingMeshMap;				// インスタンシング描画格納用
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
			std::map<std::string, STDebugMeshData> m_aDebugMeshMap;		// インスタンシング描画格納用
#endif // _DEBUG

		protected:
			//--- メンバ関数
			// *@ポリゴン整列
			void Sort();
			// *@整列ﾌﾗｸﾞ
			inline void SortOn() { m_bIsSortNecessary = true; }
			// *@3Dインスタンシング影描画
			void Draw3DShadow();
			// *@3Dインスタンシング描画
			virtual void Draw3D();

		protected:
			const std::vector<std::string> GetPSVSName(const std::string name);

		public:
			CDrawSystemBase();
			virtual ~CDrawSystemBase();

			void Init();
			virtual void Update();

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
			void SetInstanchingModel(std::string name, std::string ps, std::string vs, MySpace::Graphics::RENDER_DATA data);

			// *@インスタンシング描画のために情報を格納する
			void SetInstanchingMesh(std::string name, std::string ps, std::string vs, MySpace::Graphics::RENDER_DATA data, CMesh* mesh);

			// *@所持リスト
			_NODISCARD std::vector<std::weak_ptr<CRenderer>> GetList();

#if BUILD_MODE
			virtual void ImGuiDebug();
			// *@インスタンシング描画のために情報を格納する
			virtual void SetDebugMesh(std::string name, DirectX::XMFLOAT4X4 mtx, CMesh* mesh) = 0;
#endif // BUILD_MODE

		};

	}
}

#endif