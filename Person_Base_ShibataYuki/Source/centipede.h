//=========================================================
// [centipede.h] 
// �쐬:2022/08/16
//---------------------------------------------------------
//=========================================================

#ifndef __CENTIPEDE_COMPONENT_H__
#define __CENTIPEDE_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <vector>

//--- �N���X��`
class CEffect //: public CPolygonRenderer
{
private:
	struct STParam
	{
		Vector3 pos;
		Color color;
	};
	STParam m_EffectParam;
public:
};

namespace MySpace
{
	namespace Game 
	{
		class CSphereCollision;
	}
}
class CCentipede;
class CCentipedeBody : public CComponent
{
public: 
	static constexpr const char* TAG_NORMAL = "Player";
	static constexpr const char* TAG_DAMAGE = "PlayerDamage";
private:
	std::weak_ptr<CCentipede> m_pHead;
	std::weak_ptr<CModelRenderer> m_pModel;
	std::weak_ptr<CSphereCollision> m_pSphereCol;
	float m_fLife;
	int m_nID = 0;
	Vector3 m_vTargetPos;
	Vector3 m_vVel;

public:
	CCentipedeBody() {};
	CCentipedeBody(std::shared_ptr<CGameObject> owner);
	~CCentipedeBody() {};

	void Awake();
	void Init();
	void Update();


	void SetTargetPos(Vector3 value) { m_vTargetPos = value; };
	void SetSize(Vector3 value) { Transform()->SetScale(value); };
	void SetHeadPtr(std::weak_ptr<CCentipede> pHead) { m_pHead = pHead; }
	void SetID(int id) { m_nID = id; }
	void SetLife(float hp);

	void OnCollisionEnter(CGameObject* other);
	void OnCollisionStay(CGameObject* other);
	void OnTriggerStay(CGameObject* other);
	void OnTriggerEnter(CGameObject* other);
	//void ImGuiDebug();
};
// ���������׽�i��̂݁j
class CCentipede : public CComponent
{
public:
	struct ST_Param
	{
		Vector3 pos;
		Vector3 rot;
		ST_Param() :pos(0,0,0), rot(0,0,0) {}
		ST_Param(Vector3 p, Vector3 r) :pos(p), rot(r) {}
	};
private:
	std::weak_ptr<CModelRenderer> m_pModelCom;		// ����
	ModelWeakPtr m_pBodyModel;						// ����
	ModelWeakPtr m_pTailModel;						// ��
	Vector3 m_vUpdatePoint;							// ���W�X�V���W
	float m_fSize;									// �T�C�Y
	float m_fLife;									// �̗�
	float m_fMaxLife;								// �̗�
	float m_fGrowGauge;								// �Q�[�W
	float m_fNextGrowGauge;								// �Q�[�W
	float m_fMaxGrowGauge;							// �Q�[�W
	float m_fMoveSpeed;								// ������

	std::list<ST_Param> m_vBodyParam;
	std::vector<std::weak_ptr<CCentipedeBody>> m_pBodyList;
	std::vector<std::weak_ptr<CCentipedeBody>> m_pStanbyList;

private:
	//--- �����o�֐�
	void PosUpdate();
public:
	CCentipede() {};
	CCentipede(std::shared_ptr<CGameObject> owner);
	~CCentipede() {};

	void Awake();
	void Init();
	void Update();

	/*void OnCollisionEnter(CGameObject* other);
	void OnCollisionStay(CGameObject* other);
	void OnTriggerStay(CGameObject* other);
	void OnTriggerEnter(CGameObject* other);*/

	void Growth();			// ����
	void SetStanby(std::weak_ptr<CCentipedeBody> body);
	void SetMaxLife(float value) { m_fMaxLife = value; }
	void AddGauge(float value) { m_fGrowGauge += value; }

	//--- �Q�b�^�[�E�Z�b�^�[
	inline float& GetSpeed() { return m_fMoveSpeed; }
	inline float& GetGrowGauge() { return m_fGrowGauge; }
	inline float& GetMaxGrowGauge() { return m_fMaxGrowGauge; }
	inline float& GetLife() { return m_fLife; }
	inline float& GetMaxLife() { return m_fMaxLife; }
	inline float& GetSize() { return m_fSize; }
	inline int GetLength() { return static_cast<int>(m_vBodyParam.size()); }

#ifdef BUILD_MODE
	
	void ImGuiDebug();

#endif // DEBUG

};


#endif // !__CENTIPEDE_COMPONENT_H__

