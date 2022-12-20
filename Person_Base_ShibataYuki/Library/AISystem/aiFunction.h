//========================================================
// [aiFunction.h]
//---------------------------
// 2022/12/19
//---------------------------
//
//========================================================

#ifndef __AI_FUNCTION_H__
#define __AI_FUNCTION_H__

//--- インクルード部
#include <CoreSystem/Math/MyMath.h>

namespace MySpace
{
	namespace Game
	{
		class CGameObject;
		class CTransform;
	}
}

namespace AI
{
	// *@ 二つのベクトルをなす角がangle内かどうか
	// *@ 第四引き数: 度取得
	bool SearchAngle(MySpace::Game::CTransform* owner, MySpace::Game::CTransform* other, float angle, bool degree = true);

	// *@ 距離の取得
	// *@ 第一引き数: 自身
	// *@ 第二引き数: 目標
	float GetDistance(MySpace::MyMath::Vector3 posA, MySpace::MyMath::Vector3 posB);

	// *@滑らかに値を変化させる
	MySpace::MyMath::Vector3 Smoothly(MySpace::MyMath::Vector3 pos, MySpace::MyMath::Vector3 destPos, float per = 0.9f);
}

#endif // !__AI_FUNCTION_H__

