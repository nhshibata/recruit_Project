//=========================================================
// [partner.h]
// 作成:2022/12/15
//---------------------------------------------------------
// パートナー基底ｸﾗｽ
//=========================================================

#ifndef __PARTNER_COMPONENT_H__
#define __PARTNER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GameSystem/GameObject/gameObject.h>
#include <spellPlayer.h>

namespace AI
{
	class CAISystem;
}

namespace Spell
{

	class CPartnerSupport
	{
	private:
		CGameObject::PtrWeak m_pTarget;
		int m_aPos[3*3];
	public:
		CPartnerSupport();
		~CPartnerSupport();

		int GetPos(int , Vector3&);
		const int Bind(int idx);
		void Release(const int idx);
		CGameObject::Ptr GetTarget() { return m_pTarget.lock(); }
	};

	enum class EPreference
	{

	};
	
	// 状態管理
	enum class EPartnerState
	{
		NONE = 0,
		ENVY,
		PASS,
		PERFECT,
	};

	// 現在値と最大値管理用
	struct STSetValue
	{
		float fMax;
		float fValue;
	};

	// *@最大値
	struct STFixedValue
	{
		float fMaxEnvyValue;			// 嫉妬値
		float fMaxPassValue;			// 遠慮値
		double dMaxInterval;			// 状態継続値
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

	// *@パラメータ構造体
	// *@CAISystemに渡すことを前提に作成
	struct STPartnerParam
	{
		EPreference ePreference;	// 好み種類
		EPartnerState eState;
		std::string szName;			// 名前
		int nAtk;					// 攻撃
		int nAtkSpd;				// 次までの時間
		float fEnvyValue;			// 嫉妬値
		float fPassValue;			// 遠慮値
		double dInterval;			// 状態継続値
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

	struct STPartnerParamB
	{
		int nPosIdx;
		float fMove;
		float fDistance;
		STSetValue fStopTime;
		std::weak_ptr<CPartnerSupport> m_pSuppo;

		void SetSupport(std::shared_ptr<CPartnerSupport> ptr) { m_pSuppo = ptr; }
		std::shared_ptr<CPartnerSupport> GetSupport() { return m_pSuppo.lock(); }
	};

	class CPartner : public CComponent
	{
	private:
		//--- メンバ変数
		STPartnerParam m_Param;
		std::weak_ptr<AI::CAISystem> m_pAiSystem;

	public:
		//--- メンバ関数
		CPartner() {};
		CPartner(MySpace::Game::CGameObject::Ptr);
		~CPartner() {};

		virtual void Awake();
		virtual void Update();

		//--- ゲッター・セッター
		STPartnerParam* GetParam() { return &m_Param; }

	};
}

#endif // !__PARTNER_COMPONENT_H__