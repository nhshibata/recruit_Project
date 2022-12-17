//========================================================
// [aiAgent.h]
//------------------
// 作成:2022/11/24
//------------------
// UnityのAgentっぽく作ろうとしたもの
// 移動専門のAI
// 
// TODO:移動できない地点などがある場合、進めなくなってしまう(移動できるかどうかMapなど用意する?)
// 移動できない場所だと判断する処理を追加する?
//========================================================

#ifndef __AI_AGENT_H__
#define __AI_AGENT_H__

//--- インクルード部
#include <AISystem/System/aiSystem.h>
#include <CoreSystem/Math/MyMath.h>

using namespace MySpace::MyMath;

namespace AI
{	
	class CAIAgent :public CAISystem
	{
	private:
		//--- メンバ変数
		Vector3 m_vTarget;			// 目標座標
		float m_fDistans;			// 現在の目標との距離
		float m_fLimitDistans;		// 目標までの距離
		bool m_bIsStoping;			// 目標まで自動で移動するかのフラグ

	public:
		//--- メンバ関数
		CAIAgent(std::shared_ptr<CGameObject> owner);
		~CAIAgent();
		
		int NodeUpdate();

		//--- ゲッター・セッター
		Vector3 GetTarget() { return m_vTarget; }
		inline float GetDistance() { return m_fDistans; }
		inline float GetLimitDistance() { return m_fLimitDistans; }

		inline void SetTarget(Vector3 pos) { m_vTarget = pos; }
		//inline void SetDistance(float value) { m_fDistans = value; }
		inline void SetLimitDistance(float value) { m_fLimitDistans = value; }
		inline void SetStoping(bool state) { m_bIsStoping = state; }
		
		inline bool IsStoping() { return m_bIsStoping; }
	};
}
#endif // !__AI_AGENT_H__
