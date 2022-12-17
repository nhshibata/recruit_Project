//=====================================================
// [collisionSystem.cpp]
// �쐬:2022/11/9
//----------------------------
//
//=====================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/collisionSystem.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Collision/collision.h>

using namespace MySpace::Game;

CCollisionSystem::CCollisionSystem()
{
}
CCollisionSystem::~CCollisionSystem()
{
	m_aIntMap.clear();
}
// REVIEW: ���P�̗]�n����
void CCollisionSystem::CollisionCheck()
{
	if (m_aIntMap.size() == 0)
		return;
	
	//--- �����蔻����s�����߰��Ă�I�ʂ���
	COLLISION_VEC list;
	auto it = m_aIntMap.begin();
	auto first_col = (*it).second;

	// �ŏ���null�Ȃ�T��
	if (!first_col.lock())
	{
		for (it = m_aIntMap.begin(); it != m_aIntMap.end(); ++it)
		{
			if ((*it).second.lock())
				first_col = (*it).second;
		}
	}
	list.push_back(first_col);

	for (auto it = m_aIntMap.begin(); it != m_aIntMap.end(); ++it)
	{
		// ����̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (first_col.lock() == (*it).second.lock())
			continue;

		// �j������Ă��Ȃ����m�F
		if (!(*it).second.lock()->GetOwner()->IsActive() || !(*it).second.lock())
		{
			continue;
		}
#ifdef _DEBUG
		auto name = first_col.lock()->GetOwner()->GetName();
		auto flg = first_col.lock()->GetOwner()->GetState();
		auto other_name = (*it).second.lock()->GetOwner()->GetName();
		auto other_flg = (*it).second.lock()->GetOwner()->GetState();
		// ���g�̃R���W�������߰��Ă������n��
		bool res = first_col.lock()->HitCheckPtr((*it).second.lock().get());
		list.push_back((*it).second);
#else
		// ���g�̃R���W�������߰��Ă������n��
		first_col.lock()->HitCheckPtr((*it).second.lock().get());
		list.push_back((*it).second);	// ����Y���ƈӖ��Ȃ�(��炩��1)
#endif

	}
	// �ŏ��ȊO
	for (int cnt = 1; cnt < static_cast<int>(list.size()); ++cnt)
	{
		for (int otherCnt = 0; otherCnt < static_cast<int>(list.size()); ++otherCnt)
		{
			if (list[cnt].lock() == list[otherCnt].lock())
				continue;
#ifdef _DEBUG
			auto name = list[cnt].lock()->GetOwner()->GetName();
			auto flg = list[cnt].lock()->GetOwner()->GetState();
			auto other_name = list[otherCnt].lock()->GetOwner()->GetName();
			auto other_flg = list[otherCnt].lock()->GetOwner()->GetState();
			// ���g�̃R���W�������߰��Ă������n��
			bool res = list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
#else
			// ���g�̃R���W�������߰��Ă������n��
			list[cnt].lock()->HitCheckPtr(list[otherCnt].lock().get());
#endif // _DEBUG
		}
	}

	// ���ꂽ�I�u�W�F�N�g���m�F
	// �z�񂩂�̏��O
	for (it = m_aIntMap.begin(); it != m_aIntMap.end();)
	{	
		// null�m�F
		if (!(*it).second.lock())
		{
			it = m_aIntMap.erase((it));
			continue;
		}
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
