//=========================================================
// [enemyBase.h] 
// �쐬:2022/08/16
//---------------------------------------------------------
//=========================================================

#ifndef __ENEMY_BASE_COMPONENT_H__
#define __ENEMY_BASE_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

//--- �O���錾
class CCentipede;
class CBulletManager;
class CEnemyManager;
class CEnemyBase : public CComponent
{
private:
	int m_nLife;
	int m_nAtk;
protected:
	float m_fDamageInterval;
	std::weak_ptr<CCentipede> m_pTarget;
	std::weak_ptr<CEnemyManager> m_pManager;


public:
	//--- �����o�֐�
	CEnemyBase() {};
	CEnemyBase(std::shared_ptr<CGameObject> owner);
	~CEnemyBase() {};
	void Awake();
	virtual void Init();
	virtual void Update();

	void SetLife(float hp) { m_nLife = hp; }
	void SetTarget(std::weak_ptr<CCentipede> ptr) { m_pTarget = ptr; }
	void SetManager(std::weak_ptr<CEnemyManager> ptr) { m_pManager = ptr; }

	//--- �����蔻��
	// ������ς�������������Ȃ����߁A���z
	virtual void OnCollisionEnter(CGameObject* other);
	virtual void OnCollisionStay(CGameObject* other);
};
// �ړ������ɒe������
class CBulletEnemy : public CEnemyBase
{
	float m_fFireInterval;

public:
	CBulletEnemy() {};
	CBulletEnemy(std::shared_ptr<CGameObject> owner);
	~CBulletEnemy() {};
	void Update();

	void OnCollisionEnter(CGameObject* other);
	void OnCollisionStay(CGameObject* other);
};

class CItemManager;
class CEnemyManager : public CComponent
{
	using EnemyList = std::vector<std::weak_ptr<CEnemyBase>>;
private:
	std::weak_ptr<CCentipede> m_pTarget;			// Player: �G�������Ɏg�p
	std::weak_ptr<CItemManager> m_pItemManager;		// �G���j���Ăяo��
	std::weak_ptr<CBulletManager> m_pBulletManager;		// �G���j���Ăяo��

	EnemyList m_pEnemyList;
	EnemyList m_pWaitList;
	float m_fCreateInterVal;
	float m_fCreateCnt;
private:

public:
	CEnemyManager() {};
	CEnemyManager(std::shared_ptr<CGameObject> owner);
	~CEnemyManager() {};
	void Awake();
	void Init();
	void Update();

	void Create(Vector3 pos);
	void Create(std::vector<Vector3> pos, int cnt);
	void Standby(std::weak_ptr<CEnemyBase>);
	int CreateNum(float level);

	// enemy�͊Ǘ��׽��m���Ă���̂�bullet�|�C���^�͂�������Ăяo��
	CBulletManager* GetBullet() { return m_pBulletManager.lock().get(); };
};

#endif //__ENEMY_BASE_COMPONENT_H__