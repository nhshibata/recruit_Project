//==========================================================
// [navMeshHeight.cpp]
//----------------
// �쐬:2022/12/28
//----------------
// �����p
// �ЂƂ܂�����
//==========================================================

//--- �C���N���[�h��
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
// �E�F�C�|�C���g�̎擾
//==========================================================
void CNavMeshHeight::Bake(const float startPosY, const float endPosY)
{
	// 3D�I�u�W�F�N�g�����蔻��p
	CModelRenderer* model = nullptr;
	const int totalGrid = m_nGrid * m_nGrid;
	// ���X�g�擾
	// �O���̂������=�����蔻��̂�����̂Ƃ���
	auto aRenderList = MySpace::SceneManager::CSceneManager::Get().GetDrawSystem()->GetList();

	//--- �O���b�h�ʒu���Ɋm�F
	for (int index = 0; index < totalGrid; ++index)
	{
		Point point = IndexToPoint(index);
		//Vector3 vX, vY;
		// �����̎n�_�A�I�_
		Vector3 startPos = PointToPos(point);
		Vector3 endPos = startPos;
		startPos.y = startPosY;
		endPos.y = endPosY;

		// �����蔻����s���A���������ꍇ�͑�����W�����X�g�֊i�[
		// ��������̂��Ȃ������ꍇ�͊i�[����Ȃ�
		for (auto & render : aRenderList)
		{
			if (model = dynamic_cast<CModelRenderer*>(render.lock().get()); !model)
				continue;
			if (model->GetStatic() == 0)
				continue;
			// �����蔻��
			//if (!model->CollisionLineSegment(startPos, endPos, &vX, &vY))
			std::vector<Vector3> pos;
			if (!model->CollisionLineSegment(startPos, endPos, &pos))
				continue;

			// �����������̂���ȏ�Ȃ�
			if (pos.size() > 1)
			{
				// �������ɐ���
				std::sort(pos.begin(), pos.end(), [](auto const& a, auto const& b)->bool
				{
					return a.y > b.y;
				});
				// �����ʂɕ����Ċi�[
				for (int i = 0; i < pos.size(); i++)
				{
					int idx = int(pos[i].y / m_fHeight);
					m_aNavMap[index].aHeightMap[idx] = pos[i].y;
				}
			}
			// �擪��n��
			m_aNavMap[index].pos = pos.front();
		}
	}
	//--- �ʍs�s�G���A�̎擾
	for (auto & obs : m_aObstacle)
	{
		AddScore(obs.pos, obs.size);
	}
	//�m�[�h�I��

	//--- �ׂ荇���m�[�h���q����
	Point point;
	int x, y, index;
	x = y = index = 0;
	for (auto & node : m_aNavMap)
	{
		// �C���f�b�N�X����|�C���g��
		point = IndexToPoint(node.first);
		// ����8�}�X������
		for (y = -1; y < 2; ++y)
		{
			for (x = -1; x < 2; ++x)
			{
				// �����͔��肵�Ȃ�
				if (y == 0 && x == 0)
					continue;

				// �͈͊m�F
				if (IsOutOfPoint(CNavMeshBake::Point(point.x + x, point.y + y)))
					continue;

				index = PointToIndex(CNavMeshBake::Point(point.x + x, point.y + y));

				// ���݂��Ă��Ȃ� or ���Ă���ꍇ
				if (!m_aNavMap.count(index) || m_aNavMap[index].close)
					continue;

				// �i�[
				node.second.surrounding.push_back(index);
			}
		}
	}

}

//==========================================================
// ���[�g�\�z
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
	//--- ����n�_�Ȃ甲����
	if (startPoint == endPoint)
	{
		CNavMeshBake::MapRoute ret;
		ret.push_back(start);
		ret.push_back(end);
		return ret;
	}

	//--- �o�H�T��
	// �}�b�v������
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

	// �X�^�[�g�n�_�̃��X�g�����쐬
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

	// �I�[�v�����X�g�֒ǉ�
	std::list<List*> openList;
	openList.clear();
	openList.push_front(ptr);

	bool isEnd = false;
	//--- 8�����𒲍�
	while (PosToPoint(ptr->pNode->pos) != endPoint && !isEnd)
	{
		// �q�����Ă���m�[�h��T��
		for (auto & index : ptr->pNode->surrounding)
		{
			List* node = &pList[index];
			if (node->parent || !node->pNode)
				continue;

			// �ړ��R�X�g���v�Z�B���ʂ�A*�ł���Βʏ�� 1 �Ŗ��Ȃ�  
			float weight = CalcStepWeight(ptr->pNode->pos, node->pNode->pos, height);
			if (weight < 0)
				continue; // 0�ȉ���ǂƂ݂Ȃ��T�����΂�  
			node->step = ptr->step + weight;
			node->dist = CalcDist(node->pNode->pos, end);
			node->weight = node->step + node->dist;

			node->parent = ptr;

			// �E�F�C�g�ɉ����đ}��
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

		// �T���I�������̂ŏ��O
		openList.remove(ptr);

		// �T���ł��郋�[�g�Ȃ�
		if (openList.size() == 0)
			break;

		// ����
		ptr = openList.front();

		// ���������̂Ŕ�����
		if (PosToPoint(ptr->pNode->pos) == endPoint)
		{

			isEnd = true;
		}
	}

	//--- ���[�g�̍\�z
	CNavMeshBake::MapRoute route;
	if (!isEnd)
	{
		// ������Ȃ�����
		route.push_back(start);
	}
	else
	{
		// ���������ӏ����炳���̂ڂ�
		route.push_back(end);
		ptr = ptr->parent;
		while (ptr->parent != pRoot)
		{
			Vector3 routePos = ptr->pNode->pos;
			// ����̐i�s����
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
		// �n�_�����A����ւ�
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
// �����擾
//==========================================================
float CNavMeshHeight::CalcDist(Vector3 a, Vector3 b)
{
	Vector3 vec = b - a;
	return vec.Length();
}

//==========================================================
// �d�ݎ擾
//==========================================================
float CNavMeshHeight::CalcStepWeight(Vector3 a, Vector3 b, float height)
{
	// ����������Ȃ�Έړ��R�X�g�͒ʏ�ʂ�  
	if (a.y == b.y)
		return 1.0f;

	// ���  
	if (a.y < b.y)
	{
		int sh = static_cast<int>(b.y - a.y);
		if (sh > height) return -1;
		return 1 + sh * height; // �������R�X�g��������  
	}
	// ����  
	if (a.y > b.y)
	{
		int sh = static_cast<int>(a.y - b.y);
		if (sh > height) return -1;
		return 1; // ����͓���R�X�g�Ƃ���  
	}
	return -1;
}
