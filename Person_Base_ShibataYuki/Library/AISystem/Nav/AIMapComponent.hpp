#ifndef __AI_MAP_COMPONENT_H__
#define __AI_MAP_COMPONENT_H__

//--- インクルード部
#include <AISystem/Nav/AIMap.h>
#include <AISystem/Nav/navMeshBake.h>

struct AIPoint
{
	bool entry;
};

class AIMapRouteScore : public AIMapOperator
{
private:
	//--- メンバ変数
	Vector3 m_start;
	Vector3 m_target;

public:
	//--- メンバ関数
	AIMapRouteScore(Vector3 start, Vector3 target)
		: m_start(start), m_target(target)
	{
	}

	float Operator(Vector3 pos, void* data) final
	{
		//AIPoint* point = reinterpret_cast<AIPoint*>(data);
		// キャスト
		AI::CNavMeshBake::NodeData* point = reinterpret_cast<AI::CNavMeshBake::NodeData*>(data);

		float nor1, nor2;
		Vector3 vec = m_start - pos;
		vec = vec * 0.05f;
		DirectX::XMVECTOR m_vec1;
		m_vec1 = DirectX::XMLoadFloat3(&vec);
		m_vec1 = DirectX::XMVector3Normalize(m_vec1);
		DirectX::XMStoreFloat(&nor1, m_vec1);

		vec = m_target - pos;
		vec = vec * 0.95f;
		DirectX::XMVECTOR m_vec2;
		m_vec2 = DirectX::XMLoadFloat3(&vec);
		m_vec2 = DirectX::XMVector3Normalize(m_vec2);
		DirectX::XMStoreFloat(&nor2, m_vec2);

		float score[] = {
			//((m_start - pos) * 0.05f).Length(),
			//((m_target - pos) * 0.95f).Length(),
			nor1,
			nor2,
			//point->entry ? 0.0f : 100.0f
			!point->close ? 0.0f : 100.0f
		};
		return score[0] + score[1] + score[2];
	}
};

#endif // __AI_MAP_COMPONENT_H__