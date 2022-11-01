//=========================================================
// [enemyBase.h] 
// 作成:2022/08/16
//---------------------------------------------------------
//=========================================================

#ifndef __ENEMY_BASE_COMPONENT_H__
#define __ENEMY_BASE_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

//--- 前方宣言
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
	//--- メンバ関数
	CEnemyBase() {};
	CEnemyBase(std::shared_ptr<CGameObject> owner);
	~CEnemyBase() {};
	void Awake();
	virtual void Init();
	virtual void Update();

	void SetLife(float hp) { m_nLife = hp; }
	void SetTarget(std::weak_ptr<CCentipede> ptr) { m_pTarget = ptr; }
	void SetManager(std::weak_ptr<CEnemyManager> ptr) { m_pManager = ptr; }

	//--- 当たり判定
	// 処理を変えたいかもしれないため、仮想
	virtual void OnCollisionEnter(CGameObject* other);
	virtual void OnCollisionStay(CGameObject* other);
};
// 移動せずに弾を撃つ
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
	std::weak_ptr<CCentipede> m_pTarget;			// Player: 敵生成時に使用
	std::weak_ptr<CItemManager> m_pItemManager;		// 敵撃破時呼び出し
	std::weak_ptr<CBulletManager> m_pBulletManager;		// 敵撃破時呼び出し

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

	// enemyは管理ｸﾗｽを知っているのでbulletポインタはここから呼び出す
	CBulletManager* GetBullet() { return m_pBulletManager.lock().get(); };
};

#endif //__ENEMY_BASE_COMPONENT_H__