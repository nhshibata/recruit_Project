//=========================================================
// [bulletBase.h] 
// �쐬:2022/08/16
//---------------------------------------------------------
//=========================================================

#ifndef __BULLET_BASE_COMPONENT_H__
#define __BULLET_BASE_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

class CBulletManager;
class CBulletBase : public CComponent
{
private:
	std::weak_ptr< CBulletManager> m_pMgr;
	int m_nAtk;
	Vector3 m_vVel;
public:
	CBulletBase() {};
	CBulletBase(std::shared_ptr<CGameObject> owner);
	~CBulletBase() {};
	void Awake();
	void Init();
	void Update();

	// �Q�b�^�[�E�Z�b�^�[
	void SetMgr(std::weak_ptr< CBulletManager> pMgr) { m_pMgr = pMgr; }
	void SetAtk(int atk) { m_nAtk = atk; };
	void SetVel(Vector3 value) { m_vVel = value; }
	int GetAtk() { return m_nAtk; };
	Vector3 GetVesl() { return m_vVel; }


	void OnCollisionEnter(CGameObject* other);
	void OnCollisionStay(CGameObject* other);
};

class CBulletManager : public CComponent
{
public:
	static inline constexpr const char* TAG_NAME = "BulletManager";

private:
	std::vector<std::weak_ptr<CBulletBase>> m_pBulletList;
	std::vector<std::weak_ptr<CBulletBase>> m_pStandby;

public:
	CBulletManager() {};
	CBulletManager(std::shared_ptr<CGameObject> owner);
	~CBulletManager() {};

	void Awake();
	void Init();
	void Update();

	void Standby(std::weak_ptr<CBulletBase> ptr);
	void Create(Vector3 pos, Vector3 vel);
};

#endif //__BULLET_BASE_COMPONENT_H__
