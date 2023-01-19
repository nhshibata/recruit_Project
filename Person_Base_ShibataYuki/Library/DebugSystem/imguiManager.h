//=========================================================
// [imGuiManager.h]
// �쐬:2022/07/10
//---------------------
// ImGui�Ǘ��׽
//=========================================================

#ifndef __IMGUI_MANAGER_H__
#define __IMGUI_MANAGER_H__

#if BUILD_MODE

//--- �C���N���[�h��
#include <cctype>
#include <stdio.h>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/define.h>

#include <CoreSystem/systemBase.h>

#include <ImGui/imgui.h>
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
	namespace Graphics
	{
		class CRenderTarget;
		class CDepthStencil;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Debug
	{
		using MySpace::Game::CGameObject;
		using MySpace::Game::CDebugCamera;
		using MySpace::Debug::CMyGizmo;
		using MySpace::Graphics::CRenderTarget;
		using MySpace::Graphics::CDepthStencil;

		// ImGui�Ǘ��N���X
		class ImGuiManager : public MySpace::System::CSystemBase
		{
		public:
			//--- �񋓑̒�`
			// *@�}�E�X�̏�Ԃ��m�F����t���O
			// *@2�i���ɂ�蕡����Ԃ�ێ�����
			enum EMouseHovered	
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

			//--- �G�C���A�X
			using MapString = std::map<std::string, int>;
			using MapStringPair = std::pair<std::string, int>;

		private:
			EPlayMode m_bPlayMode;						// 
			bool m_bPause;								// �|�[�Y
			bool m_bOneFlame;							// 1�t���[���i�s
			bool m_bEditFlg;							// �ҏW�t���O
			bool m_bGridDisp;

			EMouseHovered m_eHover;						// �}�E�X���I�𒆂��񋓑�(bit)
			bool m_bSceneRender;						// �V�[�������_�[�t���O
			MapString m_aDebugMap;						// �f�o�b�O���O�pmap
			
			std::shared_ptr<CGameObject> m_pDebugObj;	// �f�o�b�O�p����߲����ێ�����I�u�W�F�N�g(�����ŕێ����Ȃ��Ɣj�������)
			std::weak_ptr<CDebugCamera> m_pDebugCamera;	// �f�o�b�O�p����߲��
														
			std::shared_ptr<CInspector> m_pInspector;	// �C���X�y�N�^�[
			std::shared_ptr<CHierachy> m_pHierarchy;	// �q�G�����L�[
			std::shared_ptr<CMyGizmo> m_pGizmo;			// �M�Y��
			std::shared_ptr<CRenderTarget> m_pRT;		// �����_�[�^�[�Q�b�g
			std::shared_ptr<CDepthStencil> m_pDS;		// �f�v�X�X�e���V��

		private:
			//--- �����o�֐�
			// *@���O�\��
			void DispLog();
			// *@�|�[�Y����
			void Pause();													

		public:
			ImGuiManager();
			~ImGuiManager() = default;

			static ImGuiManager* Get();

			HRESULT Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
			void Update();
			void Render();
			void Uninit();

			// *@�m�F
			bool CheckPlayMode();											
			// *@ImGui�f�o�b�O��~
			inline void DebugStop() { m_bPlayMode = EPlayMode::Release; }	
			// *@ImGui�f�o�b�O�J�n
			inline void DebugPlay() { m_bPlayMode = EPlayMode::Debug; }		

			//--- �Q�b�^�[�E�Z�b�^�[
			// *@�t���O�Ǘ��N���X�̕Ԃ�
			inline bool GetFlg() { return m_bEditFlg; }						
			// *@�|�[�Y�̗L��
			inline bool GetPause() { return m_bPause; }						
			// *@�|�[�Y�ؑ�
			inline void SetPause(bool flg) { m_bPause = flg; }				

			// *@�����C���X�y�N�^�[�擾
			inline std::shared_ptr<CInspector> GetInspector() { return m_pInspector; }
			// *@�����q�G�����L�[�擾
			inline std::shared_ptr<CHierachy> GetHierarchy() { return m_pHierarchy; }

			//--- �}�E�X���m�F
			// *@�}�E�X�̏�Ԏ擾
			inline EMouseHovered GetHover() { return m_eHover; };
			
			// *@�w��̏�Ԃ��m�F
			inline bool IsHover(EMouseHovered hover) { return m_eHover & hover; };
			
			// *@�r�b�g�𗧂Ă�
			inline void UpHover(EMouseHovered hover)
			{
				m_eHover = static_cast<EMouseHovered>(m_eHover | hover);
			}

			// *@�r�b�g�������Ă����牺��
			inline void DownHover(EMouseHovered hover)
			{
				if (m_eHover & hover)
				{
					m_eHover = static_cast<EMouseHovered>(m_eHover ^ hover);
				}
			}

			// *@�}�E�X��Ԏ擾�p�֐��Z�b�g
			void HoverStateSet();

			// *@���O�\��
			void DebugLog(std::string log)
			{
				auto it = m_aDebugMap.find(log);
				if (it != m_aDebugMap.end())
				{
					it->second = it->second + 1;
					return;
				}
				m_aDebugMap.insert(MapStringPair(log, 0));
			};

			//--- �����_�[�^�[�Q�b�g�֘A
			ID3D11RenderTargetView* GetRTV();
			ID3D11DepthStencilView* GetDSV();
			void SceneRender();
			void SceneRenderClear();
			void SceneGizmo();
			bool IsSceneRender() { return m_bSceneRender; }

		};
	}
}

#endif // BUILD_MODE

#endif // !__IMGUI_MANAGER
