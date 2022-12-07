//========================================================
// [aiNode.h]
//-------------------
// �쐬:2022/11/22
//-------------------
// AISystem�ɊǗ������m�[�h�B�̊��N���X
// �����A�����AISystem�����Ȃ���΂Ȃ�Ȃ�
//========================================================

#ifndef __AI_NODE_H__
#define __AI_NODE_H__

//--- �C���N���[�h��
#include <memory>

//--- �萔��`
// AID: AIDefine
#define AID_DEFINITION_ACTION					CAINode::EStatus Execute(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_ACTION(type)			CAINode::EStatus type##::Execute(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_AWAKE				void OnAwake(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_START				void OnStart(AI::CAISystem* sys)

namespace AI
{
	//--- �O���錾
	class CAISystem;

	class CAINode
	{
		friend class CAISystem;
	public:
		//--- �񋓑�
		// *@�m�[�h���
		enum class EStatus : int
		{
			NONE_ACTIVE = 0,	// ���s��ԂłȂ�
			SUCCES,				// ����
			FAILURE,			// ���s
			RUNNING,			// ���s��
			//COMPLETED,			// ����
			MAX,
		};
		//--- �G�C���A�X
		using Ptr = std::shared_ptr<CAINode>;

	private:
		//--- �����o�ϐ�
		EStatus m_eStatus = EStatus::NONE_ACTIVE;
		int m_nIndex;

	protected:
		//--- �����o�֐�
		// *@��Ԑݒ�
		// *@�ݒ肵����Ԃ�Ԃ�
		inline CAINode::EStatus SetStatus(const EStatus e) { m_eStatus = e; return m_eStatus; }
		inline void SetIndex(const int idx) { m_nIndex = idx; }

	public:

		// *@�N��������
		virtual void OnAwake(CAISystem* sys) {};

		// *@�J�n������
		virtual void OnStart(CAISystem* sys) {};

		//// *@�X�V����
		//virtual void OnUpdate(CAISystem* sys) = 0;

		//// *@�I������
		//virtual void OnEnd(CAISystem* sys) = 0;

		// *@�m�[�h���s�֐�
		virtual CAINode::EStatus Execute(CAISystem* sys) = 0;

		//--- �Q�b�^�[�E�Z�b�^�[
		// *@�o�^�C���f�b�N�X�擾
		int GetIndex()const { return m_nIndex; }

		// *@��Ԏ擾
		CAINode::EStatus GetStatus()const { return m_eStatus; }

		// *@��è�ޏ�Ԋm�F
		bool IsActive()const { return m_eStatus != EStatus::NONE_ACTIVE; }
	};
}
#endif // DEBUG
