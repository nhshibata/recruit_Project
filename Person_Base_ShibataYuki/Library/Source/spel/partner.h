//=========================================================
// [partner.h]
// �쐬:2022/12/15
//---------------------------------------------------------
// �p�[�g�i�[���׽
//=========================================================

#ifndef __PARTNER_COMPONENT_H__
#define __PARTNER_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <GameSystem/GameObject/gameObject.h>

namespace AI
{
	class CAISystem;
}

namespace Spell
{
	enum class EPreference
	{

	};
	
	// *@��ԊǗ�
	enum class EPartnerState
	{
		NONE = 0,
		ENVY,
		PASS,
		PERFECT,
	};

	// *@�ő�l
	struct STFixedValue
	{
		float fMaxEnvyValue;			// ���i�l
		float fMaxPassValue;			// �����l
		double dMaxInterval;			// ��Ԍp���l
		STFixedValue() {
			fMaxEnvyValue = fMaxPassValue
				= 80.0f;
			dMaxInterval = 0;
		};
		STFixedValue(float envy, float pass, double interval)
		{
			fMaxEnvyValue = envy;
			fMaxPassValue = pass;
			dMaxInterval = interval;
		};
	};

	// *@�p�����[�^�\����
	// *@CAISystem�ɓn�����Ƃ�O��ɍ쐬
	struct STPartnerParam
	{
		EPreference ePreference;	// �D�ݎ��
		EPartnerState eState;
		std::string szName;			// ���O
		int nAtk;					// �U��
		int nAtkSpd;				// ���܂ł̎���
		float fEnvyValue;			// ���i�l
		float fPassValue;			// �����l
		double dInterval;			// ��Ԍp���l
		STFixedValue stFixed;
		STPartnerParam()
			:ePreference(EPreference(0)), eState(EPartnerState::NONE), szName("name"), nAtk(1), nAtkSpd(1)
		{
		};

		STPartnerParam(EPreference pre, std::string name, int atk, int atkspd, STFixedValue st)
			:ePreference(pre),szName(name),nAtk(atk),nAtkSpd(atkspd),
			eState(EPartnerState::NONE)
		{
			fEnvyValue = fPassValue = 0.0f;
			dInterval = 0;
			stFixed = st;
		};
	};

	class CPartner : public CComponent
	{
	private:
		//--- �����o�ϐ�
		STPartnerParam m_Param;
		std::weak_ptr<AI::CAISystem> m_pAiSystem;

	public:
		//--- �����o�֐�
		CPartner() {};
		CPartner(MySpace::Game::CGameObject::Ptr);
		~CPartner() {};

		virtual void Awake();
		virtual void Update();

		//--- �Q�b�^�[�E�Z�b�^�[
		STPartnerParam* GetParam() { return &m_Param; };
	};
}

#endif // !__PARTNER_COMPONENT_H__