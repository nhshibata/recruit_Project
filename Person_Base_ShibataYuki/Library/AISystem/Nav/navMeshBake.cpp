//==========================================================
// [navMeshBake.h]
//----------------
// �쐬:2022/12/05
//----------------
// ��Bake�ɂ͎��Ԃ�������
//==========================================================

//--- �C���N���[�h��
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
void CNavMeshBake::Init()
{
#if DEBUG_POINT_DRAW
	//--- �����b�V��
	m_pDebugSphere = std::make_shared<CSphere>();
	m_pDebugSphere->Init(16, 8, 3.0f);
#endif // DEBUG_POINT_DRAW
}
CNavMeshBake::~CNavMeshBake()
{
#if DEBUG_POINT_DRAW
	// ���b�V���̉��
	m_pDebugSphere.reset();
	for (auto & box : m_aDebugBox)
		box.reset();
#endif // DEBUG_POINT_DRAW
}
void CNavMeshBake::Bake(const float startPosY, const float endPosY)
{
	//--- 3D�I�u�W�F�N�g�̓����蔻��
	CModelRenderer* model = nullptr;
	const int totalGrid = m_nGrid * m_nGrid;
	// ���X�g�擾
	auto aRenderList = MySpace::SceneManager::CSceneManager::Get().GetDrawSystem()->GetList();

	// �O���b�h�ʒu���Ɋm�F
	for (int index = 0; index < totalGrid; ++ index)
	{
		Point point = IndexToPoint(index);
		Vector3 vX, vY;
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
			//if (!model->CollisionLineSegment(endPos, startPos, &vX, &vY))
			if (!model->CollisionLineSegment(startPos, endPos, &vX, &vY))
				continue;
			// ���ɍ��W���i�[�ς݂Ȃ�A��r���A��荂���ʒu����꒼��
			if (m_aNavMap.count(index))
			{
				if (m_aNavMap[index].pos.y > vX.y)
					continue;
			}
			m_aNavMap[index].pos = vX;
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
	{	// �C���f�b�N�X����|�C���g��
		point = IndexToPoint(node.first);
		// ����8�}�X������
		for (y = -1; y < 2; ++y)
		{	
			// �͈͊m�F
			if (IsOutOfPoint(CNavMeshBake::Point(point.x + x, point.y + y)))
				continue;

			for (x = -1; x < 2; ++x)
			{
				if(y == 0 && x == 0)
					continue;
				index = PointToIndex(CNavMeshBake::Point(point.x + x, point.y + y));
				if (!m_aNavMap.count(index) || m_aNavMap[index].close)
					continue;
				// �i�[
				node.second.surrounding.push_back(&m_aNavMap[index]);
			}
		}
	}

}

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
	// ����n�_�Ȃ甲����
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
	memset(pList, 0, sizeof(List)* totalGrid);

	// �X�^�[�g�n�_�̃��X�g�����쐬
	List* ptr = pList + PointToIndex(startPoint);
	ptr->parent = pRoot;
	ptr->point = startPoint;
	ptr->score = 0.0f;

	// �I�[�v�����X�g�֒ǉ�
	std::list<List*> openList;
	openList.push_front(ptr);

	// �I�[�v�����X�g����ɂȂ�܂ł͒T��
	bool isEnd = false;
	while (!openList.empty() && !isEnd)
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

		// 8�����𒲍�
		const int dir[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1},
								{-1, -1},{-1, 1},{1, -1},{1, 1} };
		for (int i = 0; i < 8; ++i)
		{
			// �͈͓����m�F
			Point pt = { minScoreList->point.x + dir[i][0], minScoreList->point.y + dir[i][1] };
			if (pt.x < 0 || m_nGrid <= pt.x || pt.y < 0 || m_nGrid <= pt.y)
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
			if (pt.x == endPoint.x && pt.y == endPoint.y)
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
				ptr->score = op.Operator(PointToPos(pt), &m_aNavMap[idx]);
				openList.push_back(ptr);
			}
		}

		// ���X�g����폜
		openList.remove(minScoreList);
	}

	// ���[�g�̍\�z
	CNavMeshBake::MapRoute route;
	if (!isEnd)
	{
		// ������Ȃ�����
		route.push_back(start);
	}
	else
	{
#define MINUS(a,b)	DirectX::XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z)

		// �������������炳���̂ڂ�
		route.push_back(end);
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

				Vector3 vec = MINUS(routeEnd, routeStart);
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
	CDXDevice& dx = CDXDevice::Get();
	dx.SetBlendState(static_cast<int>(EBlendState::BS_ALPHABLEND));

	// �E�F�C�|�C���g�����\��
	for (auto & point : m_aNavMap)
	{
		/*if (point.second.close)
			continue;*/

			// ���W��ݒ�
		m_mWorld._41 = point.second.pos.x;
		m_mWorld._42 = point.second.pos.y;
		m_mWorld._43 = point.second.pos.z;

		XMVECTOR vCenter = XMLoadFloat3(&m_vCenter);
		XMMATRIX mWorld = XMLoadFloat4x4(&m_mWorld);
		vCenter = XMVector3TransformCoord(vCenter, mWorld);
		mWorld = XMMatrixTranslationFromVector(vCenter);
		XMFLOAT4X4 mW;
		XMStoreFloat4x4(&mW, mWorld);
		m_pDebugSphere->SetWorld(&mW);

		m_pDebugSphere->SetDiffuse(Vector4(0, 0, 1, 0.5f));
		for (auto & routePoint : m_aDebugRoute)
		{
			if (point.first == routePoint)
			{
				m_pDebugSphere->SetDiffuse(
					Vector4(1.0f,
							 0.0f,
							 0.0f,
							 1));
				break;
			}
		}
		
		m_pDebugSphere->Draw();
	}
	// ��Q�����{�b�N�X�\��
	for (auto & obs : m_aDebugBox)
	{
		obs->Draw();
	}
	dx.SetBlendState(static_cast<int>(EBlendState::BS_NONE));
}
#endif // DEBUG_POINT_DRAW

// ���W����}�b�v�ɂ�������W���擾
Vector3 CNavMeshBake::PosToMapPos(Vector3 pos)
{
	Point point = PosToPoint(pos);
	// �͈͊m�F
	if (!IsOutOfPoint(point))
		return pos;

	// �i�[�m�F
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

	//--- �`�惁�b�V��
	std::shared_ptr<CBox> box = std::make_shared<CBox>();
	box->Init(size);
	m_aDebugBox.push_back(box);

#if DEBUG_POINT_DRAW
	XMFLOAT4X4 m_mWorld;
	XMMATRIX mtxWorld, mtxScl, mtxTranslate;
	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(size.x, size.y, size.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(pos.x, pos.y, pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
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
	static float line[2] = { 1000, -1000 };
	if (!ImGui::TreeNode("---NavMesh---"))
		return;
	ImGui::Text("PointNum:%d", (int)m_aNavMap.size());
	ImGui::DragFloat3("Center:", m_vCenter);
	ImGui::DragInt("Grid(x*y)", &m_nGrid);
	ImGui::DragFloat("Margin", &m_fMargin);
	ImGui::DragFloat2("Line[Start,End]", line);
	if (ImGui::Button("PointReset"))
	{
		m_aNavMap.clear();
	}
	ImGui::SameLine();
	if (ImGui::Button("BakeOn"))
	{
		Bake(line[0], line[1]);
	}
	ImGui::TreePop();

}
#endif // BUILD_MODE