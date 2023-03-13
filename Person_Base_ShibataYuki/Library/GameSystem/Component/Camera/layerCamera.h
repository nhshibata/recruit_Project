//==========================================================
// [layerCamera.h]
//---------------------------------------------------------
// 作成:2023/02/14 
//---------------------------------------------------------
// レイヤーマスクを実装したｶﾒﾗ
//==========================================================

#ifndef __LAYER_CAMERA_COMPONENT_H__
#define __LAYER_CAMERA_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Camera/camera.h>

namespace MySpace
{
	namespace Game
	{

		class CLayerCamera : public CCamera
		{
			//--- シリアライズ
#pragma region _cereal
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("layerCamera", cereal::base_class<CCamera>(this)),
						CEREAL_NVP(m_nLayerMask)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("layerCamera", cereal::base_class<CCamera>(this)),
						CEREAL_NVP(m_nLayerMask)
				);
			}
#pragma endregion
		protected:
			int m_nLayerMask; // 描画するLayer管理用

		public:
			CLayerCamera();
			CLayerCamera(std::shared_ptr<CGameObject> owner);
			~CLayerCamera();

			// *@所持マスク
			inline const int GetMask() const{ return m_nLayerMask; }
			// *@マスク対象か確認
			virtual bool IsMask(const int layer)const override;
			// *@マスク設定
			void SetMask(const int mask);
			// *@マスク設定追加
			void AddMask(const int laerNo);
			// *@マスク設定解除
			void ReleaseMask(const int layerNo);

#ifdef BUILD_MODE
			virtual void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CLayerCamera)

#endif // !__LAYER_CAMERA_COMPONENT_H__

