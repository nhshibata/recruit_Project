//==========================================================
// [navMeshBake.h]
//----------------
// 作成:2022/12/05
//----------------
// ※Bakeには時間がかかる
//==========================================================

//--- インクルード部
#include <AISystem/Nav/navMeshBake.h>
#include <AISystem/Nav/AIMapComponent.hpp>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GraphicsSystem/Render/Sphere.h>
#include <GraphicsSystem/Render/box.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <ImGui/imgui.h>

using namespace AI;
using namespace MySpace::Game;
using namespace MySpace::Graphics;
using namespace MySpace::Debug;

CNavMeshBake::CNavMeshBake() 
	:CNavMeshBake(Vector3(0, 0, 0), 20, 10.0f)
{
}
CNavMeshBake::CNavMeshBake(Vector3 pos, int grid, float margin)
	:m_vCenter(pos),m_nGrid(grid),m_fMargin(margin)
{
}
CNavMeshBake::~CNavMeshBake()
{
#if DEBUG_POINT_DRAW
	// メッシュの解放
	m_pDebugSphere.reset();
	for (auto & box : m_aDebugBox)
		box.reset();
#endif // DEBUG_POINT_DRAW
}

void CNavMeshBake::Init()
{
#if DEBUG_POINT_DRAW
	//--- 球メッシュ
	m_pDebugSphere = std::make_shared<CSphere>();
	m_pDebugSphere->Init(16, 8, 3.0f);
#endif // DEBUG_POINT_DRAW
}

void CNavMeshBake::Bake(const float startPosY, const float endPosY, const float wayHeight)
{
	// 3Dオブジェクト当たり判定用
	CModelRenderer* model = nullptr;
	const int totalGrid = m_nGrid * m_nGrid;
	// リスト取得
	// 外見のあるもの=当たり判定のあるものとする
	auto aRenderList = MySpace::SceneManager::CSceneManager::Get()->GetDrawSystem()->GetList();

	//--- グリッド位置毎に確認
	for (int index = 0; index < totalGrid; ++ index)
	{
		Point point = IndexToPoint(index);
		Vector3 vX, vY;
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
			//if (!model->CollisionLineSegment(endPos, startPos, &vX, &vY))
			if (!model->CollisionLineSegment(startPos, endPos, &vX, &vY))
				continue;
			// 既に座標を格納済みなら、比較し、より高い位置を入れ直す
			if (m_aNavMap.count(index))
			{
				if (m_aNavMap[index].pos.y > vX.y)
					continue;
			}
			m_aNavMap[index].pos = vX;
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
				if(y == 0 && x == 0)
					continue;

				// 範囲確認
				if (IsOutOfPoint(CNavMeshBake::Point(point.x + x, point.y + y)))
					continue;

				index = PointToIndex(CNavMeshBake::Point(point.x + x, point.y + y));

				// 存在していない or 閉じている場合
				if (!m_aNavMap.count(index) || m_aNavMap[index].close)
					continue;
				
				// 高さの確認(差が高さよりも大きい場合)
				if (abs(m_aNavMap[index].pos.y - node.second.pos.y) > wayHeight)
					continue;

				// 格納
				node.second.surrounding.push_back(index);
			}
		}
	}

}

CNavMeshBake::MapRoute CNavMeshBake::GetRoute(Vector3 start, Vector3 end, float height)
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
		pList[i].pNode = nullptr;
		if (!m_aNavMap.count(i))
			continue;
		pList[i].pNode = &m_aNavMap[i];
		pList[i].dist = 0.0f;
		pList[i].step = 0.0f;
		pList[i].weight = 0.0f;
	}

	// スタート地点のリスト情報を作成
	List* ptr = pList + PointToIndex(startPoint);
	ptr->parent = pRoot;

	// オープンリストへ追加
	std::list<List*> openList;
	openList.clear();
	openList.push_front(ptr);

	bool isEnd = false;
	//--- 8方向を調査
	while (ptr->pNode->pos != end && !isEnd)
	{
		// 繋がっているノードを探索
		for (auto & index : ptr->pNode->surrounding)
		{
			List* node = pList + index;
			if (node->parent)
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
			auto it = openList.begin();
			for (int cnt = 0; cnt < openList.size(); ++cnt, ++it)
			{
				if ((*it)->weight > node->weight)
				{
					openList.insert(it, node);
					add = true;
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
		if (ptr->pNode->pos == end)
			isEnd = true;
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
				DirectX::XMVECTOR m_vec1;
				//DirectX::XMStoreFloat3(&vec, m_vec1);
				m_vec1 = DirectX::XMLoadFloat3(&vec);
				m_vec1 = DirectX::XMVector3Normalize(m_vec1);

				vec = routeTarget - routeEnd;
				DirectX::XMVECTOR m_vec2;
				//DirectX::XMStoreFloat3(&vec, m_vec2);
				m_vec2 = DirectX::XMLoadFloat3(&vec);
				m_vec2 = DirectX::XMVector3Normalize(m_vec2);

				float dot;
				DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(m_vec1, m_vec2));

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

float CNavMeshBake::CalcDist(Vector3 a, Vector3 b)
{
	Vector3 vec = b - a;
	return vec.Length();
}
float CNavMeshBake::CalcStepWeight(Vector3 a, Vector3 b, float height)
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

//*@ ルートを構築
CNavMeshBake::MapRoute CNavMeshBake::GetRoute(Vector3 start, Vector3 end, AIMapOperator&& op)
{
	struct List
	{
		List* parent;
		float score;
		Point point;
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
	memset(pList, 0, sizeof(List)* totalGrid);

	// スタート地点のリスト情報を作成
	List* ptr = pList + PointToIndex(startPoint);
	ptr->parent = pRoot;
	ptr->point = startPoint;
	ptr->score = 0.0f;

	// オープンリストへ追加
	std::list<List*> openList;
	openList.push_front(ptr);

	// オープンリストが空になるまでは探索
	bool isEnd = false;
	while (!openList.empty() && !isEnd)
	{
		// オープンリストの中から一番スコアが低いものを選択
		std::list<List*>::iterator it = openList.begin();
		List* minScoreList = *it;
		float score = (*it)->score;
		while (it != openList.end())
		{
			if ((*it)->score < score)
			{
				minScoreList = *it;
				score = (*it)->score;
			}
			++it;
		}

		//--- 8方向を調査
		const int dir[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1},
								{-1, -1},{-1, 1},{1, -1},{1, 1} };
		for (int i = 0; i < 8; ++i)
		{
			// 範囲内か確認
			Point pt = { minScoreList->point.x + dir[i][0], minScoreList->point.y + dir[i][1] };
			if (pt.x < 0 || m_nGrid <= pt.x || pt.y < 0 || m_nGrid <= pt.y)
				continue;

			// 親が設定されている == 開き済み
			int idx = PointToIndex(pt);
			ptr = pList + idx;
			if (ptr->parent != nullptr)
				continue;

			// ゴールに到達したか
			if (pt.x == endPoint.x && pt.y == endPoint.y)
			{
				ptr->parent = minScoreList;
				isEnd = true;
				break;
			}
			else
			{
				// 新しくオープンリストへ追加
				ptr->parent = minScoreList;
				ptr->point = pt;
				ptr->score = op.Operator(PointToPos(pt), &m_aNavMap[idx]);
				openList.push_back(ptr);
			}
		}

		// リストから削除
		openList.remove(minScoreList);
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
			Vector3 routePos = PointToPos(ptr->point);
			// 後ろ二つの進行方向
			if (route.size() >= 2)
			{
				Vector3 routeTarget = routePos;
				Vector3 routeStart = route[route.size() - 2];
				Vector3 routeEnd = route[route.size() - 1];

				Vector3 vec = routeEnd - routeStart;
				DirectX::XMVECTOR m_vec1;
				//DirectX::XMStoreFloat3(&vec, m_vec1);
				m_vec1 = DirectX::XMLoadFloat3(&vec);
				m_vec1 = DirectX::XMVector3Normalize(m_vec1);

				vec = routeTarget - routeEnd;
				DirectX::XMVECTOR m_vec2;
				//DirectX::XMStoreFloat3(&vec, m_vec2);
				m_vec2 = DirectX::XMLoadFloat3(&vec);
				m_vec2 = DirectX::XMVector3Normalize(m_vec2);

				float dot;
				DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(m_vec1, m_vec2));

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

#if DEBUG_POINT_DRAW
void CNavMeshBake::Draw()
{
	XMFLOAT4X4 m_mWorld;
	XMStoreFloat4x4(&m_mWorld, XMMatrixIdentity());
	CDXDevice* dx = CDXDevice::Get();
	dx->SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	// ウェイポイントを球表示
	std::vector< XMFLOAT4X4> instancing;
	std::vector< XMFLOAT4X4> instancingRoute;
	instancing.clear();
	instancingRoute.clear();
	m_pDebugSphere->SetDiffuse(Vector4(0, 0, 1, 0.5f));
	for (auto & point : m_aNavMap)
	{
		/*if (point.second.close)
			continue;*/

			// 座標を設定
		m_mWorld._41 = point.second.pos.x;
		m_mWorld._42 = point.second.pos.y;
		m_mWorld._43 = point.second.pos.z;

		XMVECTOR vCenter = XMLoadFloat3(&m_vCenter);
		XMMATRIX mWorld = XMLoadFloat4x4(&m_mWorld);
		vCenter = XMVector3TransformCoord(vCenter, mWorld);
		mWorld = XMMatrixTranslationFromVector(vCenter);
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, mWorld);

		bool routePoint = false;
		for (auto & routePoint : m_aDebugRoute)
		{
			if (point.first == routePoint)
			{
				instancingRoute.push_back(mW);
				routePoint = true;
				break;
			}
		}
		if(!routePoint)
			instancing.push_back(mW);
		
	}
	if (instancing.size() != 0)
	{
		m_pDebugSphere->DrawInstancing(instancing);
		m_pDebugSphere->SetDiffuse(Vector4(1.0f, 0.0f, 0.0f, 1));
		m_pDebugSphere->DrawInstancing(instancingRoute);
	}

	// 障害物をボックス表示
	for (auto & obs : m_aDebugBox)
	{
		obs->Draw();
	}
	dx->SetBlendState(static_cast<int>(EBlendState::BS_NONE));
}
#endif // DEBUG_POINT_DRAW

// 座標からマップにおける座標を取得
Vector3 CNavMeshBake::PosToMapPos(Vector3 pos)
{
	Point point = PosToPoint(pos);
	// 範囲確認
	if (!IsOutOfPoint(point))
		return pos;

	// 格納確認
	int idx = PointToIndex(point);
	if (!m_aNavMap.count(idx))
		return pos;

	return m_aNavMap[idx].pos;
}

void CNavMeshBake::AddScore(const Vector3& pos, const Vector3& size)
{
	Vector3 halfSize(size.x * 0.5f, 0.0f, size.z * 0.5f);
	Point startPoint = PosToPoint(Vector3(pos.x - halfSize.x, 0.0f, pos.z - halfSize.z));
	Point endPoint = PosToPoint(Vector3(pos.x + halfSize.x, 0.0f, pos.z + halfSize.z));
	Point point = startPoint;
	for (; point.y <= endPoint.y; ++point.y)
	{
		if (IsOutOfPoint(point)) { continue; }
		for (point.x = startPoint.x; point.x <= endPoint.x; ++point.x)
		{
			if (IsOutOfPoint(point)) { continue; }
			m_aNavMap[PointToIndex(point)].close = true;
		}
	}

	//--- 描画メッシュ
	std::shared_ptr<CBox> box = std::make_shared<CBox>();
	box->Init(size);
	m_aDebugBox.push_back(box);

#if DEBUG_POINT_DRAW
	XMFLOAT4X4 m_mWorld;
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(size.x, size.y, size.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&m_mWorld, mtxWorld);

	XMVECTOR vCenter = XMLoadFloat3(&m_vCenter);
	XMMATRIX mWorld = XMLoadFloat4x4(&m_mWorld);
	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	box->SetWorld(&mW);
	box->SetDiffuse(Vector4(1, 0, 0, 0.3f));
#endif // _DEBUG
}

#if BUILD_MODE
void CNavMeshBake::ImGuiDebug()
{

	ImGui::Text("PointNum:%d", (int)m_aNavMap.size());
	ImGui::DragFloat3("Center:", m_vCenter);
	ImGui::DragInt("Grid(x*y)", &m_nGrid);
	ImGui::DragFloat("Margin", &m_fMargin);
	ImGui::DragFloat2("Line[Start,End]", m_vLine);
	if (ImGui::Button("PointReset"))
	{
		m_aNavMap.clear();
	}
	ImGui::SameLine();
	if (ImGui::Button("BakeOn"))
	{
		Bake(m_vLine.x, m_vLine.y);
	}
	if (ImGui::Button("BakeOnHeight"))
	{
		Bake(m_vLine.x, m_vLine.y, 1);
	}
	
	ImGui::DragFloat3("startPos", (float*)m_vDebugStart);
	ImGui::DragFloat3("endPos", (float*)m_vDebugEnd);
	if (ImGui::Button("Debug"))
	{
		GetRoute(m_vDebugStart, m_vDebugEnd, 1.0f);
	}

}
#endif // BUILD_MODE