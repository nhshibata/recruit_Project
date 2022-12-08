#ifndef __AI_MAP_COMPONENT_H__
#define __AI_MAP_COMPONENT_H__

#include "AIMap.h"
#include "navMeshBake.h"

struct AIPoint
{
	bool entry;
};


class AIMapRouteScore : public AIMapOperator
{
public:
	AIMapRouteScore(Vector3 start, Vector3 target)
		: m_start(start), m_target(target)
	{

	}
	float Operator(Vector3 pos, void* data) final
	{
		//AIPoint* point = reinterpret_cast<AIPoint*>(data);
		AI::CNavMeshBake::NodeData* point = reinterpret_cast<AI::CNavMeshBake::NodeData*>(data);

		float nor1, nor2;
		Vector3 vec = MINUS(m_start, pos);
		vec = MULTI_KARIB(vec, 0.05f);
		DirectX::XMVECTOR m_vec1;
		m_vec1 = DirectX::XMLoadFloat3(&vec);
		m_vec1 = DirectX::XMVector3Normalize(m_vec1);
		DirectX::XMStoreFloat(&nor1, m_vec1);

		vec = MINUS(m_target, pos);
		vec = MULTI_KARIB(vec, 0.95f);
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
private:
	Vector3 m_start;
	Vector3 m_target;
};

#endif // __AI_MAP_COMPONENT_H__