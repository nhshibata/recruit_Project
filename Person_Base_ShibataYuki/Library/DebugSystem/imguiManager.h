

#ifndef __IMGUI_MANAGER_H__
#define __IMGUI_MANAGER_H__

#include <DebugSystem/debug.h>

#if BUILD_MODE

#include <cctype>
#include <stdio.h>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/define.h>
#include <CoreSystem/system.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/imstb_rectpack.h>
#include <ImGui/imstb_textedit.h>
#include <ImGui/imstb_truetype.h>


#include "D3D11.h"

namespace MySpace
{
	namespace Debug
	{
		class CInspector;
		class CHierachy;
	}
	namespace Game
	{
		class CDebugCamera;
	}
}

namespace MySpace
{
	namespace Debug
	{
		using MySpace::System::CSingleton;
		using MySpace::Game::CDebugCamera;

		// ImGui�Ǘ��N���X
		class ImGuiManager : public CAppSystem<ImGuiManager>
		{
			friend class CAppSystem<ImGuiManager>;
			friend class CSingleton<ImGuiManager>;
		private:
			enum class EPlayMode
			{
				Debug,
				Release,
			};

			using MapString = std::map<std::string, int>;
			using MapStringPair = std::pair<std::string, int>;

		private:
			EPlayMode	m_bPlayMode;			
			bool m_bPause;				
			bool m_bOneFlame;
			bool m_flg;
			std::weak_ptr<CDebugCamera> m_pDebugCamera;
			MapString m_debugMap;

			std::shared_ptr<CInspector> m_pInspector;
			std::shared_ptr<CHierachy> m_pHierarchy;
			
		private:
			//--- �����o�֐�
			ImGuiManager();
		public:
			//~ImGuiManager();

			void Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
			void Update();
			void Render();
			void Uninit();

			bool CheckPlayMode();											// �m�F
			void DebugStop() { m_bPlayMode = EPlayMode::Release; }			// ImGui�f�o�b�O���Ȃ�
			void DebugPlay() { m_bPlayMode = EPlayMode::Debug; }				// ImGui�f�o�b�O����

			void Pause();													// �|�[�Y�̏���

			bool GetFlg() { return m_flg; }									// �t���O�Ǘ��N���X�̕Ԃ�
			bool GetPause() { return m_bPause; }							// �|�[�Y�̗L��
			void SetPause(bool flg) { m_bPause = flg; }

			void DebugLog(std::string log) 
			{
				auto it = m_debugMap.find(log);
				if (it != m_debugMap.end())
				{
					it->second = it->second + 1;
					return;
				}
				m_debugMap.insert(MapStringPair(log, 0));
			};
			void DispLog();

			std::shared_ptr<CInspector> GetInspector() { return m_pInspector; }
			std::shared_ptr<CHierachy> GetHierarchy() { return m_pHierarchy; }
		};
	}
}

#endif // BUILD_MODE

#endif // !__IMGUI_MANAGER
