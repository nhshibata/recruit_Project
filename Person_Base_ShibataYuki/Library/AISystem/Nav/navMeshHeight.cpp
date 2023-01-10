//==========================================================
// [navMeshHeight.cpp]
//----------------
// 作成:2022/12/28
//----------------
// 高さ用
// ひとまず分割
//==========================================================

//--- インクルード部
#include <AISystem/Nav/navMeshHeight.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>

using namespace AI;
using namespace MySpace::Game;
using namespace MySpace::Graphics;
using namespace MySpace::Debug;


CNavMeshHeight::CNavMeshHeight()
{

}

CNavMeshHeight::CNavMeshHeight(Vector3 pos, int grid, float margin, float height)
	:CNavMeshBake(pos, grid, margin)
{

}

CNavMeshHeight::~CNavMeshHeight()
{

}

//==========================================================
// ウェイポイントの取得
//==========================================================
void CNavMeshHeight::Bake(const float startPosY, const float endPosY)
{
	// 3Dオブジェクト当たり判定用
	CModelRenderer* model = nullptr;
	const int totalGrid = m_nGrid * m_nGrid;
	// リスト取得
	// 外見のあるもの=当たり判定のあるものとする
	auto aRenderList = MySpace::SceneManager::CSceneManager::Get().GetDrawSystem()->GetList();

	//--- グリッド位置毎に確認
	for (int index = 0; index < totalGrid; ++index)
	{
		Point point = IndexToPoint(index);
		//Vector3 vX, vY;
		// 線分の始点、終点
		Vector3 startPos = PointToPos(point);
		Vector3 endPos = startPos;
		startPos.y = startPosY;
		endPos.y = endPosY;

		// 当たり判定を行い、当たった場合は相手座標をリストへ格納
		// 当たるものがなかった場合は格納されない
		for (auto & render : aRenderList)
		{
			if (model = dynamic_cast<CModelRenderer*>(render.lock().get()); !model)
				continue;
			if (model->GetStatic() == 0)
				continue;
			// 当たり判定
			//if (!model->CollisionLineSegment(startPos, endPos, &vX, &vY))
			std::vector<Vector3> pos;
			if (!model->CollisionLineSegment(startPos, endPos, &pos))
				continue;

			// 当たったものが一つ以上なら
			if (pos.size() > 1)
			{
				// 高さ順に整列
				std::sort(pos.begin(), pos.end(), [](auto const& a, auto const& b)->bool
				{
					return a.y > b.y;
				});
				// 高さ別に分けて格納
				for (int i = 0; i < pos.size(); i++)
				{
					int idx = int(pos[i].y / m_fHeight);
					m_aNavMap[index].aHeightMap[idx] = pos[i].y;
				}
			}
			// 先頭を渡す
			m_aNavMap[index].pos = pos.front();
		}
	}
	//--- 通行不可エリアの取得
	for (auto & obs : m_aObstacle)
	{
		AddScore(obs.pos, obs.size);
	}
	//ノード終了

	//--- 隣り合うノードを繋げる
	Point point;
	int x, y, index;
	x = y = index = 0;
	for (auto & node : m_aNavMap)
	{
		// インデックスからポイントに
		point = IndexToPoint(node.first);
		// 周囲8マスを検索
		for (y = -1; y < 2; ++y)
		{
			for (x = -1; x < 2; ++x)
			{
				// 自分は判定しない
				if (y == 0 && x == 0)
					continue;

				// 範囲確認
				if (IsOutOfPoint(CNavMeshBake::Point(point.x + x, point.y + y)))
					continue;

				index = PointToIndex(CNavMeshBake::Point(point.x + x, point.y + y));

				// 存在していない or 閉じている場合
				if (!m_aNavMap.count(index) || m_aNavMap[index].close)
					continue;

				// 格納
				node.second.surrounding.push_back(index);
			}
		}
	}

}

//==========================================================
// ルート構築
//==========================================================
CNavMeshBake::MapRoute CNavMeshHeight::GetRoute(Vector3 start, Vector3 end, float height)
{
	struct List
	{
		List* parent;
		float weight;
		float dist;
		float step;
		NodeData* pNode;
	};
	List* const pRoot = reinterpret_cast<List*>(-1);

	Point startPoint = PosToPoint(start);
	Point endPoint = PosToPoint(end);
	//--- 同一地点なら抜ける
	if (startPoint == endPoint)
	{
		CNavMeshBake::MapRoute ret;
		ret.push_back(start);
		ret.push_back(end);
		return ret;
	}

	//--- 経路探索
	// マップ初期化
	int totalGrid = m_nGrid * m_nGrid;
	List* pList = new List[totalGrid];
	for (int i = 0; i < totalGrid; i++)
	{
		pList[i].parent = nullptr;
		pList[i].pNode = nullptr;
		if (!m_aNavMap.count(i))
			continue;
		pList[i].pNode = &m_aNavMap[i];
		pList[i].dist = 0.0f;
		pList[i].step = 0.0f;
		pList[i].weight = 0.0f;
	}

	// スタート地点のリスト情報を作成
	int index = PointToIndex(startPoint);
	List* ptr = &pList[index];
	ptr->parent = pRoot;
	bool bb = m_aNavMap.count(index);

	if (!ptr->pNode)
	{
		CNavMeshBake::MapRoute ret;
		ret.push_back(start);
		ret.push_back(end);
		return ret;
	}

	// オープンリストへ追加
	std::list<List*> openList;
	openList.clear();
	openList.push_front(ptr);

	bool isEnd = false;
	//--- 8方向を調査
	while (PosToPoint(ptr->pNode->pos) != endPoint && !isEnd)
	{
		// 繋がっているノードを探索
		for (auto & index : ptr->pNode->surrounding)
		{
			List* node = &pList[index];
			if (node->parent || !node->pNode)
				continue;

			// 移動コストを計算。平面のA*であれば通常は 1 で問題なし  
			float weight = CalcStepWeight(ptr->pNode->pos, node->pNode->pos, height);
			if (weight < 0)
				continue; // 0以下を壁とみなし探索を飛ばす  
			node->step = ptr->step + weight;
			node->dist = CalcDist(node->pNode->pos, end);
			node->weight = node->step + node->dist;

			node->parent = ptr;

			// ウェイトに沿って挿入
			bool add = false;
			for (auto it = openList.begin(); it != openList.end(); ++it)
			{
				if ((*it)->weight > node->weight)
				{
					openList.insert(it, node);
					add = true;
					break;
				}
			}
			if (!add)
				openList.push_back(node);
		}

		// 探索終了したので除外
		openList.remove(ptr);

		// 探索できるルートなし
		if (openList.size() == 0)
			break;

		// 次へ
		ptr = openList.front();

		// 見つかったので抜ける
		if (PosToPoint(ptr->pNode->pos) == endPoint)
		{

			isEnd = true;
		}
	}

	//--- ルートの構築
	CNavMeshBake::MapRoute route;
	if (!isEnd)
	{
		// 見つからなかった
		route.push_back(start);
	}
	else
	{
		// 見つかった箇所からさかのぼる
		route.push_back(end);
		ptr = ptr->parent;
		while (ptr->parent != pRoot)
		{
			Vector3 routePos = ptr->pNode->pos;
			// 後ろ二つの進行方向
			if (route.size() >= 2)
			{
				Vector3 routeTarget = routePos;
				Vector3 routeStart = route[route.size() - 2];
				Vector3 routeEnd = route[route.size() - 1];

				Vector3 vec = routeEnd - routeStart;
				vec.Normalize();

				Vector3 vec2 = routeTarget - routeEnd;
				vec2.Normalize();

				float dot;
				DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&vec), DirectX::XMLoadFloat3(&vec2)));

				if (dot >= 0.95f) // acos
				{
					route.back() = routePos;
				}
				else
				{
					route.push_back(routePos);
				}
			}
			else
			{
				route.push_back(routePos);
			}
			ptr = ptr->parent;
		}
		// 始点を入れ、入れ替え
		route.push_back(start);
		std::reverse(route.begin(), route.end());
	}

#if	DEBUG_POINT_DRAW
	m_aDebugRoute.clear();
	for (auto & point : route)
	{
		m_aDebugRoute.push_back(PointToIndex(PosToPoint(point)));
	}
#endif // 

	delete[] pList;
	return route;
}

//==========================================================
// 距離取得
//==========================================================
float CNavMeshHeight::CalcDist(Vector3 a, Vector3 b)
{
	Vector3 vec = b - a;
	return vec.Length();
}

//==========================================================
// 重み取得
//==========================================================
float CNavMeshHeight::CalcStepWeight(Vector3 a, Vector3 b, float height)
{
	// 高さが同一ならば移動コストは通常通り  
	if (a.y == b.y)
		return 1.0f;

	// 上り  
	if (a.y < b.y)
	{
		int sh = static_cast<int>(b.y - a.y);
		if (sh > height) return -1;
		return 1 + sh * height; // 高さ分コストを加える  
	}
	// 下り  
	if (a.y > b.y)
	{
		int sh = static_cast<int>(a.y - b.y);
		if (sh > height) return -1;
		return 1; // 下りは同一コストとする  
	}
	return -1;
}
