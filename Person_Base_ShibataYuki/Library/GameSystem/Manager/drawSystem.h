//=========================================================
// [drawSystem.h]
// �h���N���X
//---------------------------------------------------------
// �쐬:2022/06/07 �׽���ύX���邩��
// �X�V:2022/11/09 �N���X���ύX(DrawManager) -> (drawSystem)
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DRAW_SYSTEM_H__
#define __DRAW_SYSTEM_H__

//--- �C���N���[�h��
#include <GameSystem/Manager/drawSystemBase.h>
#include <GameSystem/Shader/depthShadow.h>
#include <GameSystem/Manager/volumeManager.h>

#include <DirectXMath.h>

namespace MySpace
{
	namespace Game
	{
		class CRenderer;
		class CPolygonRenderer;
	}
	namespace Graphics
	{
		class CMesh;
	}
}

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CRenderer;
		using MySpace::Graphics::CMesh;

		//--- �N���X��`
		class CDrawSystem : public CDrawSystemBase
		{
			friend class CRenderer;
			friend class CPolygonRenderer;
		
		private:
			//--- �����o�ϐ�
			Game::CVolumeManager m_VolumeMgr;	// Volume�Ǘ�

		private:
			//--- �����o�֐�
			// *@stackCamera�ɂ��`��
			void GBufferDraw(bool gb, std::function<bool(int)> func = nullptr);
			// *@3D�C���X�^���V���O�`��
			void Draw3D()override;
			// *@�`��Ώۂ̊m�F
			// *@Layer,Stack�p
			void CheckRenderedObjectsIn3D()override;

		public:
			CDrawSystem();
			~CDrawSystem();

			void Update();

			// *@Volume�̎擾
			inline CVolumeManager* GetVolumeManager() { return &m_VolumeMgr; }

#if BUILD_MODE
			//void ImGuiDebug();
			
#endif // BUILD_MODE

		};

	}
}

#endif