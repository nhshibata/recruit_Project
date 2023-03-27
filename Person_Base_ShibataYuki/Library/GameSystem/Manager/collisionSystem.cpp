//=====================================================
// [collisionSystem.cpp]
// �쐬:2022/11/9
// �X�V:2023/03/22 �ςȏ����ɂȂ��Ă��̂ŁA�X�b�L���C��
//-----------------------------------------------------
//
//=====================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/collisionSystem.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Collision/collision.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CCollisionSystem::CCollisionSystem()
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CCollisionSystem::~CCollisionSystem()
{
	m_aIntMap.clear();
}

//==========================================================
// Collision�׽���m�̓����蔻����s
// REVIEW: ���P�̗]�n����
//==========================================================
void CCollisionSystem::CollisionCheck()
{
	if (m_aIntMap.size() == 0)
		return;
	
	//--- �����蔻����s�����߰��Ă�I�ʂ���
	COLLISION_VEC list;
	auto it = m_aIntMap.begin();

	for (; it != m_aIntMap.end(); )
	{
		// null�m�F
		if (!(*it).second.lock())
		{
			it = m_aIntMap.erase((it));
			continue;
		}

		// ��Ԋm�F
		if (!(*it).second.lock()->GetOwner()->IsActive())
		{
			++it;
			continue;
		}

		list.push_back((*it).second);	// ����Y���ƈӖ��Ȃ�(��炩��1)
		++it;
	}

	// �ŏ��ȊO
	for (int cnt = 0; cnt < static_cast<int>(list.size()); ++cnt)
	{
		for (int otherCnt = cnt + 1; otherCnt < static_cast<int>(list.size()); ++otherCnt)
		{
			if (list[cnt].lock() == list[otherCnt].lock())
				continue;

			// ���g�̃R���W�������߰��Ă������n��
			list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
		}
	}

	// ���ꂽ�I�u�W�F�N�g���m�F
	// �z�񂩂�̏��O
	for (it = m_aIntMap.begin(); it != m_aIntMap.end();)
	{			
		// Active�m�F
		if (!(*it).second.lock()->IsActive())
		{
			it++;
			continue;
		}

		// ���ꂽ
		(*it).second.lock()->ExitTell();
		it++;
	}

}
