//=========================================================
// [systemManager.h]
// �쐬�F2022/09/20
//----------------------
// �V���O���g�������Ƃ����׽���Ǘ�����
//=========================================================

#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__

//--- �C���N���[�h��
#include <map>
#include <vector>

//--- �N���X��`
namespace MySpace
{
	namespace System
	{
		enum class ESystems // �Y��Ȃ����߂Ɖ�����Ɋy�A�ȊO���܂�Ӗ����Ȃ�?
		{
			DEVICE,
			SCENE,
			TWEEN,
			TAG,
			TYPE_SAVE,
			FUNC_PTR,
			IMAGE,
			MODEL,
			EFFEKSEER,
			FONT,
			IMGUI,
		};

		class CSystemManager {
		private:
			std::map<ESystems, void*> m_pMap;
		public:
			void* SetSystem(ESystems system, void* ptr) { m_pMap.insert(std::make_pair(system, ptr)); return ptr; }
			_NODISCARD void* GetSystem(ESystems system)
			{
				if (m_pMap.at(system))
					return m_pMap[system];
				return nullptr;
			}
			template <class T>
			T* GetSystem(ESystems system, bool f)
			{
				if (m_pMap.at(system))
					return reinterpret_cast<T*>(m_pMap[system]);
				return nullptr;
			}
			template <class T>
			T* GetSystem()
			{
				//static_assert(std::is_base_of<CAppSystem, T> == false, ("not AppSystem"));
				for (auto &ptr : m_pMap)
				{
					if (auto system = dynamic_cast<T>(ptr); system)
					{
						//return reinterpret_cast<T*>(m_pMap);
						return system;
					}
				}
				return nullptr;
			}
			// �V�X�e���̉��
			void DisposeSystem(ESystems system)
			{
				m_pMap.erase(system);
			}
			void UninitAll()
			{
				m_pMap.clear();
			}
			std::vector<void*> GetAll()
			{
				std::vector<void*> ret;
				for (auto & system : m_pMap)
				{
					ret.push_back(system.second);
				}
				return ret;
			}
		};
	}
}
#endif // !__SYSTEM_MANAGER_H__


