//=========================================================
// [debugCamera.h]
// 作成:2022/07/19
// 
//=========================================================

//--- インクルードガード
#ifndef __DEBUG_CAMERA_COMPONENT_H__
#define __DEBUG_CAMERA_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Camera/camera.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CDebugCamera : public CCamera
		{
		private:
			// シリアライズ
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("debugCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("debugCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
		private:
			enum class ECameraMode	// ｶﾒﾗ操作状態
			{
				CAM_MODE_NONE,		// 操作なし
				CAM_MODE_ORBIT,		// 注視点を中心に回転
				CAM_MODE_TRACK,		// 注視点と一緒に移動
				CAM_MODE_DOLLY,		// 注視点に近づいたり遠ざかったり
			};

			ECameraMode m_eMode = ECameraMode::CAM_MODE_NONE;		// カメラ視点
			bool m_bMouse;
			POINT m_oldMousePos;
		public:
			//--- ﾒﾝﾊﾞ関数
			CDebugCamera();
			CDebugCamera(std::shared_ptr<CGameObject> owner);
			~CDebugCamera();

			void Awake();
			void Init();					// 初期化 他コンポーネントの取得などを行う
			void Update();					// 更新
			//void LateUpdate();			// 遅い更新

			void CameraMouseMove(int x, int y);
#ifdef BUILD_MODE

			//virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CDebugCamera)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CCamera, MySpace::CGameCamera)

#endif // !__GAME_CAMERA_H__
