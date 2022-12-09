

#ifndef __IMGUI_MANAGER_H__
#define __IMGUI_MANAGER_H__

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

#pragma region ForwardDeclaration
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
#pragma endregion

namespace MySpace
{
	namespace Debug
	{
		using MySpace::System::CSingleton;
		using MySpace::Game::CGameObject;
		using MySpace::Game::CDebugCamera;
		using MySpace::Debug::CMyGizmo;

		// ImGui管理クラス
		class ImGuiManager : public CSingleton<ImGuiManager>
		{
			friend class CSingleton<ImGuiManager>;
		public:
			//--- 列挙体定義
			// *@マウスの状態を確認するフラグ
			// *@2進数により複数状態を保持する
			enum EIsHovered	
			{
				MAX_HOVERD = 7,
				HOVERED_NONE	= 0,
				HOVERED_WINDOW	= 1 << 0,
				HOVERED_MOUSE	= 1 << 1,
				HOVERED_ITEM	= 1 << 2,
				HOVERED_GIZMO	= 1 << 3,
				HOVERED_DRAG	= 1 << 4,
				HOVERED_INPUT	= 1 << 5,
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
			EPlayMode m_bPlayMode;						// 
			bool m_bPause;								// ポーズ
			bool m_bOneFlame;							// 1フレーム進行
			bool m_bEditFlg;							// 編集フラグ
			std::weak_ptr<CDebugCamera> m_pDebugCamera;	// デバッグ用ｶﾒﾗﾎﾟｲﾝﾀ
			std::shared_ptr<CGameObject> m_pDebugObj;	// デバッグ用ｶﾒﾗﾎﾟｲﾝﾀを保持するオブジェクト(ここで保持しないと破棄される)
			
			MapString m_debugMap;						// デバッグログ用map
			EIsHovered m_eHover;						// マウス等選択中か列挙体(bit)
														
			std::shared_ptr<CInspector> m_pInspector;	// インスペクター
			std::shared_ptr<CHierachy> m_pHierarchy;	// ヒエラルキー
			std::shared_ptr<CMyGizmo> m_pGizmo;			// ギズモ

		private:
			//--- メンバ関数
			ImGuiManager();
			//~ImGuiManager() {};

			void DispLog();

		public:
			void Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
			void Update();
			void Render();
			void Uninit();

			// *@ポーズの処理
			void Pause();													

			// *@確認
			bool CheckPlayMode();											
			// *@ImGuiデバッグ停止
			inline void DebugStop() { m_bPlayMode = EPlayMode::Release; }	
			// *@ImGuiデバッグ開始
			inline void DebugPlay() { m_bPlayMode = EPlayMode::Debug; }		

			//--- ゲッター・セッター
			// *@フラグ管理クラスの返す
			inline bool GetFlg() { return m_bEditFlg; }						
			// *@ポーズの有無
			inline bool GetPause() { return m_bPause; }						
			// *@ポーズ切替
			inline void SetPause(bool flg) { m_bPause = flg; }				

			// *@所持インスペクター取得
			inline std::shared_ptr<CInspector> GetInspector() { return m_pInspector; }
			// *@所持ヒエラルキー取得
			inline std::shared_ptr<CHierachy> GetHierarchy() { return m_pHierarchy; }

			//--- マウス等確認
			// *@マウスの状態取得
			inline EIsHovered GetHover() { return m_eHover; };
			// *@指定の状態か確認
			inline bool IsHover(EIsHovered hover) { return m_eHover & hover; };
			// *@ビットを立てる
			inline void UpHover(EIsHovered hover)
			{
				m_eHover = static_cast<EIsHovered>(m_eHover | hover);
			}
			// *@ビットが立っていたら下す
			inline void DownHover(EIsHovered hover)
			{
				if (m_eHover & hover)
				{
					m_eHover = static_cast<EIsHovered>(m_eHover ^ hover);
				}
			}

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
		};
	}
}

#endif // BUILD_MODE

#endif // !__IMGUI_MANAGER
