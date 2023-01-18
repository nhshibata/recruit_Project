#include "AIMap.h"
#include <list>

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
AIMap::AIMap(int dataSize, int grid)
	: m_pData(nullptr)
	, m_stride(dataSize)
	, m_grid(grid)
	, m_pos(0.0f, 0.0f, 0.0f)
	, m_margin(1.0f)
{
	int totalSize = m_stride * m_grid * m_grid;
	m_pData = new char[totalSize];
	memset(m_pData, 0, totalSize);
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
AIMap::AIMap(int dataSize, int grid, AIMapOperator&& op)
	: AIMap(dataSize, grid)
{
	int totalGrid = m_grid * m_grid;
	for (int i = 0; i < totalGrid; ++i)
	{
#pragma warning(suppress: 26451)
		op.Operator(PointToPos(IndexToPoint(i)), m_pData + m_stride * i);
	}
}

//==========================================================
// �f�X�g���N�^
//==========================================================
AIMap::~AIMap()
{
	delete[] m_pData;
}

//==========================================================
// ���S���W
//==========================================================
void AIMap::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

//==========================================================
// �|�C���g�Ԃ̍�
//==========================================================
void AIMap::SetMargin(float margin)
{
	m_margin = margin;
}

//==========================================================
// ��Q���ݒ�
//==========================================================
void AIMap::AddScore(const Vector3& pos, const Vector3& size, AIMapOperator&& op)
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
#pragma warning(suppress: 26451)
			op.Operator(PointToPos(point), m_pData + m_stride * PointToIndex(point));
		}
	}
}

//==========================================================
// �ł�����
//==========================================================
Vector3 AIMap::SearchTarget(const Vector3 start, AIMapOperator&& op)
{
	int grid = m_grid * m_grid;
	float maxScore = 0.0f;
	Vector3 maxPos = m_pos;
	for (int i = 0; i < grid; ++i)
	{
		Vector3 pos = PointToPos(IndexToPoint(i));
#pragma warning(suppress: 26451)
		float score = op.Operator(pos, m_pData + m_stride * i );
		if (score > maxScore)
		{
			maxScore = score;
			maxPos = pos;
		}
	}
	return maxPos;
}

//==========================================================
// �ڕW���W�܂ł̃��[�g�擾
// �Ȃ���΃X�^�[�g�n�_��
//==========================================================
AIMap::Route AIMap::SearchRoute(const Vector3 start, const Vector3 target, AIMapOperator&& op)
{
	struct List
	{
		List* parent;
		float score;
		Point point;
	};
	List* const pRoot = reinterpret_cast<List*>(-1);

	// �X�^�[�g�ƃ^�[�Q�b�g�������ʒu�Ȃ瑦�I��
	Point startPt = PosToPoint(start);
	Point targetPt = PosToPoint(target);
	if (startPt.x == targetPt.x && startPt.y == targetPt.y)
	{
		Route route;
		route.push_back(start);
		route.push_back(target);
		return route;
	}

	// �}�b�v������
	int totalGrid = m_grid * m_grid;
	List* pList = new List[totalGrid];
	memset(pList, 0, sizeof(List)* totalGrid);

	// �X�^�[�g�n�_�̃��X�g�����쐬
	List* ptr = pList + PointToIndex(startPt);
	ptr->parent = pRoot;
	ptr->point = startPt;
	ptr->score = 0.0f;

	// �I�[�v�����X�g�֒ǉ�
	std::list<List*> openList;
	openList.push_front(ptr);

	// �I�[�v�����X�g����ɂȂ�܂ł͒T��
	bool isEnd = false;
	while(!openList.empty() && !isEnd)
	{
		// �I�[�v�����X�g�̒������ԃX�R�A���Ⴂ���̂�I��
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

		// 4�����𒲍�
		const int dir[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0,1} };
		for (int i = 0; i < 4; ++i)
		{
			// �͈͓����m�F
			Point pt = { minScoreList->point.x + dir[i][0], minScoreList->point.y + dir[i][1] };
			if (pt.x < 0 || m_grid <= pt.x || pt.y < 0 || m_grid <= pt.y)
			{
				continue;
			}

			// �e���ݒ肳��Ă��� == �J���ς�
			int idx = PointToIndex(pt);
			ptr = pList + idx;
			if (ptr->parent != nullptr)
			{
				continue;
			}

			// �S�[���ɓ��B������
			if (pt.x == targetPt.x && pt.y == targetPt.y)
			{
				ptr->parent = minScoreList;
				isEnd = true;
				break;
			}
			else
			{
				// �V�����I�[�v�����X�g�֒ǉ�
				ptr->parent = minScoreList;
				ptr->point = pt;
#pragma warning(suppress: 26451)
				ptr->score = op.Operator(PointToPos(pt), m_pData + m_stride * idx );
				openList.push_back(ptr);
			}
		}

		// ���X�g����폜
		openList.remove(minScoreList);
	}

	// ���[�g�̍\�z
	Route route;
	if (!isEnd)
	{
		// ������Ȃ�����
		route.push_back(start);
	}
	else
	{
		// �������������炳���̂ڂ�
		route.push_back(target);
		ptr = ptr->parent;
		while (ptr->parent != pRoot)
		{
			Vector3 routePos = PointToPos(ptr->point);
			// ����̐i�s����
			if (route.size() >= 2)
			{
				Vector3 routeTarget = routePos;
				Vector3 routeStart = route[route.size() - 2];
				Vector3 routeEnd = route[route.size() - 1];
				
				Vector3 vec = MINUS(routeEnd ,routeStart);
				DirectX::XMVECTOR m_vec1;
				//DirectX::XMStoreFloat3(&vec, m_vec1);
				m_vec1 = DirectX::XMLoadFloat3(&vec);
				m_vec1 = DirectX::XMVector3Normalize(m_vec1);
				
				vec = MINUS(routeTarget, routeEnd);
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
		route.push_back(start);
		std::reverse(route.begin(), route.end());
	}


	delete[] pList;
	return route;
}

#ifdef _DEBUG
void AIMap::Draw(AIMapOperator&& op)
{
	int grid = m_grid * m_grid;
	for (int i = 0; i < grid; ++i)
	{
		op.Operator(PointToPos(IndexToPoint(i)), m_pData + m_stride * i);
	}
}
#endif

//==========================================================
// �͈͓��m�F
//==========================================================
bool AIMap::IsOutOfPoint(Point index)
{
	return index.x < 0 || index.y < 0 ||
		m_grid <= index.x || m_grid <= index.y;
}

//==========================================================
// �C���f�b�N�X����񎟌��z��֕ϊ�
//==========================================================
AIMap::Point AIMap::IndexToPoint(int index)
{
	return Point{ index % m_grid, index / m_grid };
}

//==========================================================
// ���W����񎟌��z��֕ϊ�
//==========================================================
AIMap::Point AIMap::PosToPoint(const Vector3 pos)
{
	float startX = m_pos.x - m_grid * m_margin * 0.5f; // m_margin * (m_grid - 1) * 0.5f - m_margin * 0.5f;
	float startZ = m_pos.z - m_grid * m_margin * 0.5f;
	return Point{
		static_cast<int>((pos.x - startX) / m_margin),
		static_cast<int>((pos.z - startZ) / m_margin)
	};
}

//==========================================================
// �񎟌��z�񂩂�C���f�b�N�X�֕ϊ�
//==========================================================
int AIMap::PointToIndex(Point point)
{
	return point.y * m_grid + point.x;
}

//==========================================================
// �񎟌��z�񂩂���W�֕ϊ�
//==========================================================
Vector3 AIMap::PointToPos(Point point)
{
	float startX = m_pos.x - (m_grid - 1) * m_margin * 0.5f;
	float startZ = m_pos.z - (m_grid - 1) * m_margin * 0.5f;
	return Vector3(
		startX + point.x * m_margin,
		m_pos.y,
		startZ + point.y * m_margin
	);
}