//========================================================
// [aiAgent.h]
//------------------
// �쐬:2022/11/24
//------------------
// Unity��Agent���ۂ���낤�Ƃ�������
// �ړ�����AI
// 
// TODO:�ړ��ł��Ȃ��n�_�Ȃǂ�����ꍇ�A�i�߂Ȃ��Ȃ��Ă��܂�(�ړ��ł��邩�ǂ���Map�ȂǗp�ӂ���?)
// �ړ��ł��Ȃ��ꏊ���Ɣ��f���鏈����ǉ�����?
//========================================================

#ifndef __AI_AGENT_H__
#define __AI_AGENT_H__

//--- �C���N���[�h��
#include <AISystem/System/aiSystem.h>
#include <CoreSystem/Math/MyMath.h>

using namespace MySpace::MyMath;

namespace AI
{	
	class CAIAgent :public CAISystem
	{
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("AIAgentComponent", cereal::base_class<CAISystem>(this)),
					CEREAL_NVP(m_vTarget), CEREAL_NVP(m_fDistans), 
					CEREAL_NVP(m_fLimitDistans), CEREAL_NVP(m_bIsStoping)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("AIAgentComponent", cereal::base_class<CAISystem>(this)),
					CEREAL_NVP(m_vTarget), CEREAL_NVP(m_fDistans),
					CEREAL_NVP(m_fLimitDistans), CEREAL_NVP(m_bIsStoping)
			);
		}
#pragma endregion
	private:
		//--- �����o�ϐ�
		Vector3 m_vTarget;			// �ڕW���W
		float m_fDistans;			// ���݂̖ڕW�Ƃ̋���
		float m_fLimitDistans;		// �ڕW�܂ł̋���
		bool m_bIsStoping;			// �ڕW�܂Ŏ����ňړ����邩�̃t���O

	public:
		//--- �����o�֐�
		CAIAgent();
		CAIAgent(std::shared_ptr<CGameObject> owner);
		~CAIAgent();
		
		// *@�������Ăяo��
		virtual void Awake() {};

		int NodeUpdate();

		//--- �Q�b�^�[�E�Z�b�^�[
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

CEREAL_REGISTER_TYPE(AI::CAIAgent)

#endif // !__AI_AGENT_H__
