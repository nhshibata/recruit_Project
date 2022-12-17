//=========================================================
// [gameCamera.h]
// 作成:2022/07/19
// 
//=========================================================

//--- インクルードガード
#ifndef __GAME_CAMERA_COMPONENT_H__
#define __GAME_CAMERA_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Camera/camera.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CGameCamera : public CCamera
		{
		private:
			// シリアライズ
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("gameCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("gameCamera", cereal::base_class<CCamera>(this))

					/*CEREAL_NVP(VIEW_ASPECT),CEREAL_NVP(m_pSky), CEREAL_NVP(m_pCamera)*/
				);
			}
		public:
			//--- ﾒﾝﾊﾞ関数
			CGameCamera();
			CGameCamera(std::shared_ptr<CGameObject> owner);
			~CGameCamera();

			void Awake();
			void Init();					// 初期化 他コンポーネントの取得などを行う
			void Update();					// 更新
			//void LateUpdate();			// 遅い更新

			//virtual void ImGuiDebug();
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CGameCamera)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CCamera, MySpace::CGameCamera)

#endif // !__GAME_CAMERA_H__
