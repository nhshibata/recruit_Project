

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
		class CMyGizmo;
	}
	namespace Game
	{
		class CGameObject;
		class CDebugCamera;
	}
}

namespace MySpace
{
	namespace Debug
	{
		using MySpace::System::CSingleton;
		using MySpace::Game::CGameObject;
		using MySpace::Game::CDebugCamera;
		using MySpace::Debug::CMyGizmo;

		// ImGui管理クラス
		class ImGuiManager : public CAppSystem<ImGuiManager>
		{
			friend class CAppSystem<ImGuiManager>;
			friend class CSingleton<ImGuiManager>;
		public:
			enum EIsHovered
			{
				HOVERED_NONE	= 0,
				HOVERED_WINDOW	= 1 << 0,
				HOVERED_MOUSE	= 1 << 1,
				HOVERED_ITEM	= 1 << 2,
				HOVERED_GIZMO	= 1 << 3
			};
		private:
			enum class EPlayMode
			{
				Debug,
				Release,
			};

			using MapString = std::map<std::string, int>;
			using MapStringPair = std::pair<std::string, int>;

		private:
			EPlayMode m_bPlayMode;			
			bool m_bPause;				
			bool m_bOneFlame;
			bool m_bEditFlg;
			std::shared_ptr<CGameObject> m_pDebugObj;
			std::weak_ptr<CDebugCamera> m_pDebugCamera;
			MapString m_debugMap;

			std::shared_ptr<CInspector> m_pInspector;
			std::shared_ptr<CHierachy> m_pHierarchy;
			std::shared_ptr<CMyGizmo> m_pGizmo;

			EIsHovered m_nHover;
		private:
			//--- メンバ関数
			ImGuiManager();
		public:
			//~ImGuiManager();

			void Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
			void Update();
			void Render();
			void Uninit();

			bool CheckPlayMode();											// 確認
			void DebugStop() { m_bPlayMode = EPlayMode::Release; }			// ImGuiデバッグしない
			void DebugPlay() { m_bPlayMode = EPlayMode::Debug; }				// ImGuiデバッグする

			void Pause();													// ポーズの処理

			inline bool GetFlg() { return m_bEditFlg; }									// フラグ管理クラスの返す
			inline bool GetPause() { return m_bPause; }							// ポーズの有無
			inline void SetPause(bool flg) { m_bPause = flg; }

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

			inline std::shared_ptr<CInspector> GetInspector() { return m_pInspector; }
			inline std::shared_ptr<CHierachy> GetHierarchy() { return m_pHierarchy; }


			inline EIsHovered GetHover() { return m_nHover; };
			inline bool IsHover(EIsHovered hover) { return m_nHover & hover; };
			inline void UpHover(EIsHovered hover)
			{
				m_nHover = static_cast<EIsHovered>(m_nHover | hover);
			}
			inline void DownHover(EIsHovered hover)
			{
				if (m_nHover & hover)
				{
					m_nHover = static_cast<EIsHovered>(m_nHover ^ hover);
				}
			}
		};
	}
}

#endif // BUILD_MODE

#endif // !__IMGUI_MANAGER
