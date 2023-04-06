//========================================================
// [AISystem.h]
//------------------
// �쐬:2022/11/22
//------------------
// ��{�I�ȃr�w�C�r�A
// �m�[�h�̒ǉ����ŗD��x�����܂�
// �m�[�h�̃m�[�h���Ǝq�m�[�h���D�悳���
// Node�ɂ�Condition�ƂȂ������ǉ�����
//�i��������ꍇ�A�S�Ă̏����𖞂����Ă��Ȃ���΃m�[�h�͎��s����Ȃ��j
//========================================================

#ifndef __AI_SYSTEM_H__
#define __AI_SYSTEM_H__

//--- �C���N���[�h��
#include <map>
#include <memory>
#include <AISystem/Node/aiNode.h>
#include <GameSYstem/Component/component.h>

//--- �O���錾
namespace MySpace
{
	namespace Game
	{
		class CGameObject;
	}
}

namespace AI
{
	class CAINode;
}
	
namespace AI
{
	using MySpace::Game::CGameObject;
	
	class CAISystem : public CComponent
	{
#pragma region Serialize
	private:
		//--- �V���A���C�Y
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("AISystemComponent", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_pRoot), CEREAL_NVP(m_Nodes)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("AISystemComponent", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_pRoot), CEREAL_NVP(m_Nodes)
			);
		}
#pragma endregion
	private:
		//--- �G�C���A�X
		using NodePtr = std::shared_ptr<CAINode>;
		using NodeMap = std::map<int, NodePtr>;
		
	protected:
		//--- �����o�ϐ�
		NodePtr m_pRoot;			// �擪�m�[�h
		NodeMap m_Nodes;			// �m�[�h���ꊇ�ŊǗ�����
		void* m_pData;				// (����̐ӔC������)

	public:
		//--- �����o�֐�
		CAISystem();
		CAISystem(std::shared_ptr<CGameObject> ptr);
		~CAISystem();
		
		// *@�������Ăяo��
		virtual void Awake() {};
		// *@������
		virtual void Init();

		void Update();

		// *@�m�[�h�̍X�V
		// *@�߂�l�̓m�[�h�̌��ʂ�Ԃ�(�l�͗񋓑̂ł���CAINode::EStatus�)
		virtual int NodeUpdate();

		// *@�����߲���ϐ���void*����T�^��cast����
		template<class T>
		inline T* ConvertData()
		{
			T* ret = reinterpret_cast<T*>(m_pData);
			return ret;
		}

		//--- �m�[�h�֘A
		// *@ �m�[�h�ǉ�
		CAISystem::NodePtr AddNode(NodePtr node);

		// *@�m�[�h�����֐�
		// *@������ID�ԍ��������s���A�U�蕪����
		template<class T>
		_NODISCARD std::shared_ptr<T> MakeNode()
		{
			int idx = -1;
			do{
				idx = rand() % 1000;
				if (!m_Nodes.count(idx))
					break;
			} while (true);

			auto node = std::make_shared<T>();
			node->SetIndex(idx);
			AddNode(node);
			return node;
		}

		//--- �Q�b�^�[�E�Z�b�^�[

		// *@�m�[�h�擾
		// *@������:�U�蕪��ꂽ��ӂȃC���f�b�N�X�ԍ�
		// *@�o�^����Ă��Ȃ��C���f�b�N�X���n���ꂽ�ꍇ�A���SP��Ԃ�
		inline NodePtr GetNode(int idx) { if (!m_Nodes.count(idx)) { return NodePtr(); } return m_Nodes[idx]; }

		// *@�o�^�����C���f�b�N�X����SP�擾
		inline NodePtr GetSmartPtr(int idx) { if (!m_Nodes.count(idx))NodePtr(); return m_Nodes[idx]; }

		// *@�^����m�[�h���擾
		template<class T>
		_NODISCARD std::shared_ptr<T> GetNode()
		{
			std::shared_ptr<T> ret = std::shared_ptr<T>();
			for (auto & node : m_Nodes)
			{
				ret = std::dynamic_pointer_cast<T>(node);
				if (ret)
					return ret;
			}
			return std::shared_ptr<T>();
		}

		// *@���s�m�[�h�̐ݒ�
		inline void SetRoot(NodePtr node) { m_pRoot = node; }

		inline void SetData(void* data) { m_pData = data; }

#if _DEBUG
	public:
		int m_nNodeCnt = 0;
#endif // _DEBUG

	};
}

CEREAL_REGISTER_TYPE(AI::CAISystem)

#endif // !__AI_SYSTEM_H__
