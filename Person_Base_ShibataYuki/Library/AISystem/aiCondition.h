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
#include <CoreSystem/Util/cerealCommon.h>

namespace AI
{

#pragma region ForwardDeclaration
	class CAISystem;
#pragma endregion

	class CAICondition
	{
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(CEREAL_NVP(m_Functions)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(CEREAL_NVP(m_Functions)
			);
		}
#pragma endregion
	public:
		//--- �G�C���A�X
		// *@�֐��|�C���^
		// *@�֐����͏�Ԋm�F���s�����̂��]�܂���
		//using FuncPtr = bool(*)(CAISystem*);
		using FuncPtr = std::function<bool(CAISystem*)>;

		// *@ shared�߲���p
		using Ptr = std::shared_ptr<CAICondition>;

	private:
		//--- ���ޕϐ�
		std::list<FuncPtr> m_Functions;

	public:
		//--- �����o�֐�

		// *@���s�ł��邩�֐��m�F
		// *@��������ꍇ�A�S�Ă̏����𖞂����Ȃ���΂Ȃ�Ȃ�
		bool IsTransition(CAISystem* sys)const
		{
			for (auto & func : m_Functions)
				if (!func(sys))
					return false;
			return true;
		}

		// *@�m�F�����ǉ�
		// *@������: bool(*)(CAISystem*)
		inline void AddFunc(FuncPtr ptr)
		{
			m_Functions.push_back(std::move(ptr));
		}

		// *@�m�F�����j��
		void RemoveFunc(FuncPtr ptr)
		{
			for (auto it = m_Functions.begin(); it != m_Functions.end(); ++it)
			{
				// ������߲����T��
				if ((*it).target<bool(CAISystem*)>() == ptr.target<bool(CAISystem*)>())
				{
					m_Functions.erase(it);
					return;
				}
			}
		}

		// *@������
		inline void Uninit()
		{
			m_Functions.clear();
		}

	};
}

#endif // !__CONDITION_H__
