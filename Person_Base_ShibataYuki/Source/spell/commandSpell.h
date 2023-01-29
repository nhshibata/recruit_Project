//=========================================================
// [command.h] 
// �쐬:2022/09/21
//---------------------------------------------------------
// union���g���Ă݂������������ȋC������
//=========================================================

#ifndef __COMMAND_SPELL_COMPONENT_H__
#define __COMMAND_SPELL_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <CoreSystem/property.h>
//#include <command.h>

namespace Spell
{
	using namespace MySpace::Game;

	enum class EAttributte
	{
		RED = 0,
		BLUE,
		GREEN,
	};
	enum class EType 
	{
		ATK = 0,
		DEF,
		RECOVERY
	};

	struct STSpell // CSV�ȂǂŊǗ�����?
	{
		using Succes = std::vector<bool>;
		using Command = std::vector<int>;

		//--- �����o�ϐ�	
		std::string			spellName;		// ���O
		EAttributte			eAttribute;		// ����(�񋓌^)
		EType				eType;
		int					nValue;			// ���(���p��)
		Succes	succes;
		Command	command;					// �L�[�i�[(0~)

		//--- �����o�֐�
		STSpell() :spellName(std::string()), eAttribute(EAttributte::BLUE), eType(EType::ATK), command(0) {};
		STSpell(std::string sz, EAttributte e, EType type, std::vector<int> n = std::vector<int>())
			:spellName(sz), eAttribute(e),eType(type), command(n)
		{};
		
		void Set(std::vector<int> value) { command = value; }
		void Set(std::vector<bool> value) { succes = value; }
		void SetType(EType type, int n) { nValue = n; eType = type; }
		void Create(std::string sz, EAttributte eAtt, EType type, int value, std::vector<int> vec)
		{
			this->spellName = sz;
			this->eAttribute = eAtt;
			Set(vec);
			SetType(type, value);
		}
		bool LoadString(std::vector<std::string> file)
		{
			spellName = file[0];
			eAttribute = static_cast<EAttributte>(std::atoi(file[1].c_str()));
			eType = static_cast<EType>(std::atoi(file[2].c_str()));
			nValue = std::atoi(file[3].c_str());
			std::vector<int> set;
			for (int cnt = 4; cnt < file.size(); ++cnt)
			{
				set.push_back(std::atoi(file[cnt].c_str()));
			}
			Set(set);
		}
	};

	// *@���͂��L�^����׽
	class CCommandSpell : public MySpace::Game::CComponent
	{
	private:
		//--- �����o�ϐ�
		CCommandStock* m_pStock;
		std::map<int, STSpell> m_SpellMap;	// �񋓑̂ŊǗ��ł���悤��
		STSpell* m_pMagic;
	public:
		CProperty<int> Considered;	 // �݂Ȃ�
		CProperty<bool> Execution;	// ���s�t���O

	private:
		//--- �����o�֐�
		_NODISCARD int SpellCheck(std::vector<int> list, std::vector<int> check);
		_NODISCARD std::vector<bool> SpellCheckConsidered(std::vector<int> list, std::vector<int> check);
	public:
		CCommandSpell() : m_pMagic(nullptr), m_pStock(nullptr) {};
		CCommandSpell(std::shared_ptr<CGameObject> owner) :MySpace::Game::CComponent(owner)
		{};
		~CCommandSpell() {};

		void Awake();
		void Init();
		void Update();
		void FixedUpdate();

		// *@�o�^
		// *@id�w��(�񋓌^�Ȃǎg���Ƃ��Ȃ�)
		inline int Regist(STSpell st, int id = 0)
		{
			STSpell s = st;
			if (id == 0)
				id = static_cast<int>(m_SpellMap.size());
			m_SpellMap.insert(std::make_pair(id, s));
		}

		// *@���͏�񏉊���
		inline void Reset(int id) { m_SpellMap[id].command.clear(); }

		// *@�ް��擾�p
		// *@null�m�F�K�{
		inline STSpell* Present() { return m_pMagic; }

		// *@�󂯎��������ް���j������
		// *@�ް����Ȃ��̂ɌĂяo������false���Ԃ�
		inline bool DestroyPresent()
		{
			if (m_pMagic) { delete m_pMagic; m_pMagic = nullptr; return true; } 
			return false;
		}
	};
}
#endif // !__COMMAND_COMPONENT_H__
