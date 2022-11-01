//=========================================================
// [command.h] 
// �쐬:2022/09/21
//---------------------------------------------------------
//=========================================================

#ifndef __COMMAND_COMPONENT_H__
#define __COMMAND_COMPONENT_H__

#include <GameSystem/Component/component.h>
#include <CoreSystem/Input/input.h>
#include <CoreSystem/property.h>

namespace Spell
{
	// *@���͂��L�^����׽
	class CCommandStock : public CComponent
	{
	private:
		//--- �����o�ϐ�
		std::vector<int> m_nInputList;		// ���͊m�F���X�g
		std::vector<int> m_nInputBuffer;	// ���͂��L�^���郊�X�g
	
	public:
		CProperty<float> InputInterval;
		CProperty<float> InputMaxInterval;

	public:
		//--- �����o�֐�
		CCommandStock() {};
		CCommandStock(std::shared_ptr<CGameObject> owner):CComponent(owner)
		{};
		~CCommandStock() {};

		void Awake();
		void Init();
		void Update();
		void FixedUpdate();

		// *@���͏�񏉊���
		inline void CommandReset() { m_nInputBuffer.clear(); }
		inline auto GetBuffer() { return m_nInputBuffer; }
		inline bool SetKey(int key) {
			if (std::find(m_nInputList.begin(), m_nInputList.end(), key) == m_nInputList.end())
			{
				m_nInputList.push_back(key);
				return true;
			}
			return false;
		}
	};
}
#endif // !__COMMAND_COMPONENT_H__
