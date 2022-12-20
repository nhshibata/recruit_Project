//========================================================
// [aiFunction.h]
//---------------------------
// 2022/12/19
//---------------------------
//
//========================================================

//--- インクルード部
#include <AISystem/aiFunction.h>
#include <GameSystem/Component/Transform/transform.h>

namespace AI
{
	// *@二点間から角度を求め、角度内か判定
	// *@参考: http://www.sousakuba.com/Programming/gs_two_vector_angle.html
	bool SearchAngle(MySpace::Game::CTransform* owner, MySpace::Game::CTransform* other, float searchAngle, bool degree)
	{
		auto fA = other->GetPos().Length();
		auto fB = owner->GetPos().Length();
		float cosSita = Vector3::Dot(other->GetPos(), owner->GetPos()) / (fA * fB);
		float angle = acosf(cosSita);
	
		if(degree)
			angle = angle * 180.0f / DirectX::XM_PI;
		
		return (searchAngle >= angle);
	}


	// 距離の判定
	inline float GetDistance(Vector3 posA, Vector3 posB)
	{
		return sqrtf(pow(posB.x - posA.x, 2) +
					 pow(posB.y - posA.y, 2) +
					 pow(posB.z - posA.z, 2));
	}

	Vector3 Smoothly(Vector3 pos, Vector3 destPos, float per)
	{
		return (pos * per + destPos * (1.0f - per));
	}
}
