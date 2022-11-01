//=========================================================
// [gameobjectManager.cpp]
// �쐬:2022/06/07
//---------------------------------------------------------
// �Q�[���I�u�W�F�N�g���Ǘ�����׽
// �Ǘ��݂̂Ő����͑��̸׽�ōs��
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/Component/Collision/collision.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/camera.h>


#include <GameSystem/Manager/sceneManager.h>

using namespace MySpace::Game;
//using namespace MySpace;

// �R���X�g���N�^
CGameObjectManager::CGameObjectManager()
{
	
}
// �f�X�g���N�^
CGameObjectManager::~CGameObjectManager()
{
}
// ������
void CGameObjectManager::Init()
{
	for (auto & obj : m_objMgr)
	{
		obj->Init();
	}
}
// ���
void CGameObjectManager::Uninit()
{
	// �S�ď��O
	for (auto & obj : m_objMgr)
	{
		obj.reset();
	}
	m_objMgr.clear();
	m_pCollisionComponent.clear();
}
// �X�V
void CGameObjectManager::Update()
{
	WeakList pActiveObj(0);		// �A�N�e�B�u�ȃI�u�W�F�N�g���i�[
	WeakList pDestoroyObj(0);	// �j���I�u�W�F�N�g���i�[

	// �I�u�W�F�N�g�X�V
	// ��Ԋm�F�i���͎��̃t���[���܂ł͔j������Ȃ��I�u�W�F�N�g�����邱��?)
	for (auto & obj : m_objMgr)
	{
		// ��Ԃɂ�蕪��
		switch (obj->GetState())
		{
		case CGameObject::ACTIVE:				// �ʏ�̍X�V
			// �g�����X�t�H�[���̍X�V
			//(*it).get()->GetTransform()->Update();

			// �A�N�e�B�u�ȃI�u�W�F�N�g���i�[
			pActiveObj.push_back(obj);
			break;
		case CGameObject::WAIT:					// �ҋ@
			break;
		case CGameObject::DESTROY:				// �폜�I�u�W�F�N�g
			// �i�[
			pDestoroyObj.push_back(obj);
			break;
		case CGameObject::TAKEOVER:				// �����p���҂�
			break;
		case CGameObject::MAX_OBJECT_STATE:		// �ő吔
			break;
		default:
			break;
		}
	}

	// �����蔻��m�F
	CollisionCheck();

	// Tween�̍X�V(���Ԍ���)
	CTweenManager::Get().Update();

	//--- �A�N�e�B�u�̂��̂���
	// �R���|�[�l���g�̍X�V
	for (auto & obj : pActiveObj)
	{
		// component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
		if (MySpace::SceneManager::CSceneManager::Get().Escape())
			return;
#ifdef _DEBUG

		[[maybe_unused]]auto name = obj.lock()->GetName();
#endif // _DEBUG

		obj.lock()->Update();
	}
	
	// �Ō�̕��ɍX�V���������̂��X�V
	for (auto & obj : pActiveObj)
	{
		// component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
		if (MySpace::SceneManager::CSceneManager::Get().Escape())
			return;
		obj.lock()->LateUpdate();
	}

	// �I�u�W�F�N�g�̔j��
	for (auto & obj : pDestoroyObj)
	{	// FIXME: �폜�����̂�?
		DestroyObject(obj);
		obj.reset();
	}

	// �ǉ��I�u�W�F�N�g�̊m�F�A�ǉ�
	ObjectListUpdate();
	
	// �z��̃��Z�b�g
	pActiveObj.clear();
	pDestoroyObj.clear();
}
void CGameObjectManager::FixedUpdate()
{
	// �I�u�W�F�N�g�X�V
	// �R���|�[�l���g�̍X�V
	for (auto & obj : m_objMgr)
	{
		// component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
		if (MySpace::SceneManager::CSceneManager::Get().Escape())
			return;

		// ��Ԃɂ�蕪��
		if (obj->GetState() == CGameObject::ACTIVE) 
		{
			obj->FixedUpdate();
		}
	}
}
// REVIEW: ���P�̗]�n����
void CGameObjectManager::CollisionCheck()
{
	if (m_pCollisionComponent.size() == 0)
		return;
#if 1
	//--- �����蔻����s�����߰��Ă�I�ʂ���
	COLLISION_VEC list;
	auto first_col = m_pCollisionComponent.front();
	// �ŏ���null�Ȃ�T��
	if (!first_col.lock())
	{
		for (auto it = m_pCollisionComponent.begin(); it != m_pCollisionComponent.end(); ++it)
		{
			if ((*it).lock())
				first_col = (*it);
		}
	}
	list.push_back(first_col);

	for (auto it = m_pCollisionComponent.begin(); it != m_pCollisionComponent.end(); ++it)
	{
		// ����̃I�u�W�F�N�g�͔��肵�Ȃ�
		if (first_col.lock() == (*it).lock())
			continue;
		// �j������Ă��Ȃ����m�F
		if (!(*it).lock()->GetOwner()->IsActive() || !(*it).lock())
		{			
			continue;
		}
#ifdef _DEBUG
		auto name = first_col.lock()->GetOwner()->GetName();
		auto flg = first_col.lock()->GetOwner()->GetState();
		auto other_name = (*it).lock()->GetOwner()->GetName();
		auto other_flg = (*it).lock()->GetOwner()->GetState();
		// ���g�̃R���W�������߰��Ă������n��
		bool res = first_col.lock()->HitCheckPtr((*it).lock().get());
		list.push_back((*it));
#else
		// ���g�̃R���W�������߰��Ă������n��
		first_col.lock()->HitCheckPtr((*it).lock().get());
		list.push_back((*it));	// ����Y���ƈӖ��Ȃ�(��炩��1)
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
	for (int cnt = 0; cnt < static_cast<int>(m_pCollisionComponent.size()); ++cnt)
	{
		if (!m_pCollisionComponent[cnt].lock()->IsActive())
			continue;
		if (!m_pCollisionComponent[cnt].lock())
		{	// �j��(�Ō������)
			m_pCollisionComponent[cnt] = m_pCollisionComponent.back();
			m_pCollisionComponent.pop_back();
			--cnt;	// �d�؂蒼��
			continue;
		}
		m_pCollisionComponent[cnt].lock()->ExitTell();
	}

	return;
#endif // 0

#pragma region OLD
	for (auto & col : m_pCollisionComponent)
	{
		// �����Ԃ��m�F
		if (!col.lock())
			continue;
		// �����Ԃ��m�F
		if (!col.lock()->GetOwner()->IsActive())
			continue;

		for (auto & other : m_pCollisionComponent)
		{
			// ����̃I�u�W�F�N�g�͔��肵�Ȃ�
			// �j������Ă��Ȃ����m�F
			if (col.lock() == other.lock() || !other.lock()->GetOwner()->IsActive() || !other.lock())
			{
				continue;
			}
#ifdef _DEBUG
			auto name = col.lock()->GetOwner()->GetName();
			auto flg = col.lock()->GetOwner()->GetState();
			auto other_name = other.lock()->GetOwner()->GetName();
			auto other_flg = other.lock()->GetOwner()->GetState();
			// ���g�̃R���W�������߰��Ă������n��
			bool res = col.lock()->HitCheckPtr(other.lock().get());
#else
			// ���g�̃R���W�������߰��Ă������n��
			col.lock()->HitCheckPtr(other.lock().get());
#endif // _DEBUG
		}// other
	}// this

	// ���ꂽ�I�u�W�F�N�g���m�F
	// �z�񂩂�̏��O
	for (int cnt = 0; cnt < static_cast<int>(m_pCollisionComponent.size()); ++cnt)
	{	
		if (!m_pCollisionComponent[cnt].lock()->IsActive())
			continue;
		if (!m_pCollisionComponent[cnt].lock())
		{	// �j��(�Ō������)
			m_pCollisionComponent[cnt] = m_pCollisionComponent.back();
			m_pCollisionComponent.pop_back();
			--cnt;	// �d�؂蒼��
			continue;
		}
		m_pCollisionComponent[cnt].lock()->ExitTell();
	}
	// �z������
	//m_pCollisionComponent.clear();
#pragma endregion
}
// �I�u�W�F�N�g�̒ǉ�
bool CGameObjectManager::ObjectListUpdate()
{
	// �ǉ��I�u�W�F�N�g����łȂ���
	// �r���ǉ��̃I�u�W�F������ꍇ�̂���while
	while(m_addObjList.size() != 0)
	{
		auto addList = m_addObjList;	// �󂯎��
		m_addObjList.clear();			// �z������

		// �i�[���A�������Ăяo��
		for (auto & addObj : addList)
		{
			if (addObj)
			{
				addObj->Awake();
				SetGameObject(addObj);
			}
		}
		for (auto & addObj : addList)
		{
#ifdef _DEBUG
			auto name = addObj->GetName();
#endif // !_DEBUG

			if (addObj)
			{
				addObj->Init();
			}
		}
	}
	
	return false;
}

// �`��ɕK�v�ȃI�u�W�F�N�g�̍쐬�Z�b�g
void CGameObjectManager::CreateBasicObject()
{
	std::shared_ptr<CGameObject> pObj = CreateGameObject();
	// �K�{
	// ���
	pObj->AddComponent<CCamera>();
	// ���C�g
	pObj = CreateGameObject();
	pObj->AddComponent<CDirectionalLight>();
}