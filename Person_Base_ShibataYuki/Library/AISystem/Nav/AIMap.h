#ifndef __AI_MAP_H__
#define __AI_MAP_H__

#include <memory>
#include <vector>
#include <CoreSystem/Math/MyMath.h>

#define MINUS(a,b)			DirectX::XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z)
#define MULTI_KARI(a,b)		DirectX::XMFLOAT3(a.x * b.x, a.y * b.y, a.z * b.z)
#define MULTI_KARIB(a,b)	DirectX::XMFLOAT3(a.x * b, a.y * b, a.z * b)

namespace AI
{
	class CNavMeshBake;
}
using namespace MySpace::MyMath;

class AIMapOperator
{
	friend class AIMap;
	friend class AI::CNavMeshBake;
public:
	AIMapOperator() {}
	~AIMapOperator() {}
protected:
	virtual float Operator(Vector3 pos, void* data) = 0;

};

class AIMap
{
public:
	using Route = std::vector<Vector3>;
private:
	struct Point
	{
		int x;
		int y;
	};

public:
	AIMap(int dataSize, int grid);
	AIMap(int dataSize, int grid, AIMapOperator&& op);
	virtual ~AIMap();

	void SetPos(const Vector3& pos);
	void SetMargin(float margin);


	void AddScore(const Vector3& pos, const Vector3& size, AIMapOperator&& op);
	Vector3 SearchTarget(const Vector3 start, AIMapOperator&& op);
	Route SearchRoute(const Vector3 start, const Vector3 target, AIMapOperator&& op);

#ifdef _DEBUG
	void Draw(AIMapOperator&& op);
#endif

private:
	bool IsOutOfPoint(Point index);
	Point IndexToPoint(int index);
	Point PosToPoint(const Vector3 pos);
	int PointToIndex(Point point);
	Vector3 PointToPos(Point point);

private:
	char* m_pData;
	int m_stride;
	int m_grid;

	Vector3 m_pos;
	float m_margin;
};

#endif // __AI_MAP_H__