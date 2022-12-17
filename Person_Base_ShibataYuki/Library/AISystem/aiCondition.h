//========================================================
// [aiCondition.h]
//------------------
// �쐬:2022/11/22
//========================================================

#ifndef __AI_CONDITION_H__
#define __AI_CONDITION_H__

//--- �C���N���[�h��
#include <functional>
#include <vector>

namespace AI
{

#pragma region ForwardDeclaration
	class CAISystem;
#pragma endregion

	class CAICondition
	{
	public:
		//--- �G�C���A�X
		// *@�֐��|�C���^
		// *@�֐����͏�Ԋm�F���s�����̂��]�܂���
		using FuncPtr = bool(*)(CAISystem*);
		// *@ shared�߲���p
		using Ptr = std::shared_ptr<CAICondition>;

	private:
		using STLFunc = std::function<bool(CAISystem*)>;

	private:
		//--- ���ޕϐ�
		std::vector<STLFunc> m_Functions;

	public:
		//--- �����o�֐�

		// *@���s�ł��邩�֐��m�F
		// *@��������ꍇ�A�S�Ă̏����𖞂����Ȃ���΂Ȃ�Ȃ�
		bool IsTransition(CAISystem* ptr)
		{
			for (auto & func : m_Functions)
				if (!func(ptr))
					return false;
			return true;
		}

		// *@�m�F�����ǉ�
		// *@������: bool(*)(CAISystem*)
		inline void AddFunc(FuncPtr ptr)
		{
			m_Functions.push_back(ptr);
			//m_Functions.push_back(std::bind(ptr));
		}

		// *@������
		inline void Uninit()
		{
			m_Functions.clear();
		}

	};
}

#endif // !__CONDITION_H__
