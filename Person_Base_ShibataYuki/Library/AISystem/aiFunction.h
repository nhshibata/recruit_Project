//========================================================
// [aiFunction.h]
//---------------------------
// 2022/12/19
//---------------------------
//
//========================================================

#ifndef __AI_FUNCTION_H__
#define __AI_FUNCTION_H__

//--- �C���N���[�h��
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
	// *@ ��̃x�N�g�����Ȃ��p��angle�����ǂ���
	// *@ ��l������: �x�擾
	bool SearchAngle(MySpace::Game::CTransform* owner, MySpace::Game::CTransform* other, float angle, bool degree = true);

	// *@ �����̎擾
	// *@ ��������: ���g
	// *@ ��������: �ڕW
	float GetDistance(MySpace::MyMath::Vector3 posA, MySpace::MyMath::Vector3 posB);

	// *@���炩�ɒl��ω�������
	MySpace::MyMath::Vector3 Smoothly(MySpace::MyMath::Vector3 pos, MySpace::MyMath::Vector3 destPos, float per = 0.9f);
}

#endif // !__AI_FUNCTION_H__

