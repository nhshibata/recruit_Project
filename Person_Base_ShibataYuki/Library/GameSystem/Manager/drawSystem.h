//=========================================================
// [drawSystem.h]
// 派生クラス
//---------------------------------------------------------
// 作成:2022/06/07 ｸﾗｽ名変更するかも
// 更新:2022/11/09 クラス名変更(DrawManager) -> (drawSystem)
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __DRAW_SYSTEM_H__
#define __DRAW_SYSTEM_H__

//--- インクルード部
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

		//--- クラス定義
		class CDrawSystem : public CDrawSystemBase
		{
			friend class CRenderer;
			friend class CPolygonRenderer;
		
		private:
			//--- メンバ変数
			Game::CVolumeManager m_VolumeMgr;	// Volume管理

		private:
			//--- メンバ関数
			// *@stackCameraによる描画
			void GBufferDraw(bool gb, std::function<bool(int)> func = nullptr);
			// *@3Dインスタンシング描画
			void Draw3D()override;
			// *@描画対象の確認
			// *@Layer,Stack用
			void CheckRenderedObjectsIn3D()override;

		public:
			CDrawSystem();
			~CDrawSystem();

			void Update();

			// *@Volume管理クラスの取得
			inline CVolumeManager* GetVolumeManager() { return &m_VolumeMgr; }

#if BUILD_MODE
			//void ImGuiDebug();
			
#endif // BUILD_MODE

		};

	}
}

#endif