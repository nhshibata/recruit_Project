//==========================================================
// [navMeshBake.h]
//----------------------------------------------------------
// 作成:2022/12/05		ポイント生成成功（デバッグ用の表示も
// 更新:2022/12/06		経路探索(A*)でルート構築成功,障害物設定も確認
// 更新:2022/12/07		就職作品への移植
//----------------------------------------------------------
// ナビメッシュ構築用ｸﾗｽ
// 管理も行っているため、命名ミスったかも...
//==========================================================

#ifndef __NAV_MESH_BAKE_H__
#define __NAV_MESH_BAKE_H__

//--- インクルード部
#include <unordered_map>
#include <vector>
#include <memory>
#include <CoreSystem/Math/MyMath.h>

//--- 定数定義
#define DEBUG_POINT_DRAW		_DEBUG | true		// *@ポイント表示用定数


#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Graphics
	{
		class CSphere;
		class CBox;
	}
}
class AIMapOperator;
#pragma endregion


namespace AI
{
	using namespace MySpace::MyMath;
	using MySpace::Graphics::CSphere;
	using MySpace::Graphics::CBox;

	// *@ナビメッシュ構築用ｸﾗｽ
	class CNavMeshBake
	{
	protected:
		//--- 構造体定義
		struct Obstacle		// 障害物ﾃﾞｰﾀ
		{
			Vector3 pos;
			Vector3 size;
			Obstacle(Vector3 p, Vector3 s) { pos = p; size = s; }
		};
		struct Point		// 配列管理用(インデックス,添字などに使う)
		{
			int x;	
			int y;
			// コンストラクタ
			Point() { x = y = 0; }
			Point(int x1, int y1) { x = x1; y = y1; }
			// 演算子のオーバーロード
			bool operator==(Point other) { return x == other.x && y == other.y; }
			bool operator!=(Point other) { return !(*this == other); }
		};
	public:
		struct NodeData		// ノードが持つﾃﾞｰﾀ
		{
			Vector3 pos;
			bool close = false;
			std::vector<int> surrounding;
			std::map<int, float> aHeightMap;
		};

	private:
		//--- エイリアス
		using NavMap = std::unordered_map<int, NodeData>;		// 作成したノード情報格納(Keyにint型を用いる)
	public:
		using MapRoute = std::vector<Vector3>;					// マップルート引き渡し用
		using Obstacles = std::vector<Obstacle>;				// 障害物ﾃﾞｰﾀ格納

	protected:
		//--- メンバ変数
		NavMap m_aNavMap;						// Nav情報
		Obstacles m_aObstacle;					// 障害物情報
		Vector3 m_vCenter = Vector3(0,0,0);		// 中心座標
		float m_fMargin;						// グリッド毎の差
		int m_nGrid;							// グリッド縦横

	protected:
		//--- ﾒﾝﾊﾞ関数
		// *@障害物追加
		void AddScore(const Vector3& pos, const Vector3& size);

	public:
		CNavMeshBake();
		CNavMeshBake(Vector3 pos, int grid, float margin);
		virtual ~CNavMeshBake();

		// *@初期化関数
		void Init();

		// *@ポイント情報生成呼び出し
		// *@引き数:線分の始点, 終点, 歩ける高さ
		virtual void Bake(const float startPos, const float endPos);


		// *@グリッド外判定
		inline bool IsOutOfPoint(Point index)const
		{
			return index.x < 0 || index.y < 0 || m_nGrid <= index.x || m_nGrid <= index.y;
		}

		// *@インデックスからxy取得
		inline Point IndexToPoint(int index) { return Point{ index % m_nGrid, index / m_nGrid }; }
		
		// *@座標からxyへ
		Point PosToPoint(const Vector3 pos)
		{
			float startX = m_vCenter.x - m_nGrid * m_fMargin * 0.5f; // m_margin * (m_grid - 1) * 0.5f - m_margin * 0.5f;
			float startZ = m_vCenter.z - m_nGrid * m_fMargin * 0.5f;
			return Point{
				static_cast<int>((pos.x - startX) / m_fMargin),
				static_cast<int>((pos.z - startZ) / m_fMargin)
			};
		}

		// *@xyからインデックス変換
		inline int PointToIndex(Point point)const { return point.y * m_nGrid + point.x; }

		// *@xyから座標変換
		Vector3 PointToPos(Point point)
		{
			float startX = m_vCenter.x - (m_nGrid - 1) * m_fMargin * 0.5f;
			float startZ = m_vCenter.z - (m_nGrid - 1) * m_fMargin * 0.5f;
			return Vector3(
				startX + point.x * m_fMargin,
				m_vCenter.y,
				startZ + point.y * m_fMargin
			);
		}

		// *@座標からマップの座標取得
		Vector3 PosToMapPos(Vector3 pos);

		// *@マップﾃﾞｰﾀからルートを構築
		MapRoute GetRoute(Vector3 start, Vector3 end, AIMapOperator&& op);

		//--- ゲッター・セッター

		// *@中心座標
		inline void SetPos(Vector3 pos) { m_vCenter = pos; }

		// *@マージン(幅)
		inline void SetMargin(float value) { m_fMargin = value; }

		// *@障害物設置
		void SetObstacle(Vector3 pos, Vector3 size) { m_aObstacle.push_back(Obstacle(pos, size)); }

#if BUILD_MODE

		void ImGuiDebug();

#endif // 0
		

#if DEBUG_POINT_DRAW
	protected:
		std::shared_ptr<CSphere> m_pDebugSphere;
		std::vector<std::shared_ptr<CBox>> m_aDebugBox;
		std::vector<int> m_aDebugRoute;
		Vector3 m_vDebugStart;
		Vector3 m_vDebugEnd;
		float m_fDebugHeight = (1.0f);
		Vector2 m_vLine = Vector2(1000, -1000);

	private:
		bool IsRoute(int idx);

	public:
		// *@ポイント描画
		void Draw();
#endif // DEBUG_POINT_DRAW

	};
}

#endif // !__NAV_MESH_BAKE_H__