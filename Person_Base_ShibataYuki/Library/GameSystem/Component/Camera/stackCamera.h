//==========================================================
// [stackCamera.h]
//---------------------------------------------------------
// 作成：2023/02/14
//---------------------------------------------------------
//
//==========================================================

#ifndef __STACK_CAMERA_COMPONENT_H__
#define __STACK_CAMERA_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Camera/layerCamera.h>
#include <GraphicsSystem/DirectX/GBuffer.h>

namespace MySpace
{
	namespace Game
	{
		class CStackCamera : public CLayerCamera
		{
			//--- シリアライズ
#pragma region _cereal
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("stackCamera", cereal::base_class<CLayerCamera>(this)),
						CEREAL_NVP(m_eMode), CEREAL_NVP(m_aStackCamera)
				); 
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("stackCamera", cereal::base_class<CLayerCamera>(this)),
						CEREAL_NVP(m_eMode), CEREAL_NVP(m_aStackCamera)
				);
			}
#pragma endregion
		public:
			enum class EStackMode
			{
				BASE,		// stack可能状態。MainCameraでなければ意味がない
				OVERLAY		// stack対象状態。Baseにstackできる
			};

		private:
			EStackMode m_eMode;
			std::vector<std::weak_ptr<CStackCamera>> m_aStackCamera;
			std::unique_ptr<MySpace::Graphics::CGBuffer> m_pGBuf;

		public:
			CStackCamera();
			CStackCamera(CGameObject::Ptr owner);
			~CStackCamera();

			// *@stackに追加
			// *@引き数は順番指定
			void Stack(std::weak_ptr<CStackCamera> camera, int idx = -1);
			
			// *@マスク対象か確認
			bool IsMask(const int layer)override final;

			// *@モード取得
			bool IsStackMode(const EStackMode mode) { return m_eMode == mode; }

			// *@layer対象
			const int GetAllTargetLayer();

			// *@設定されたｶﾒﾗの取得
			std::vector<std::weak_ptr<CStackCamera>> GetStackCameras() { return m_aStackCamera; }

			// *@所持するGBuffer
			MySpace::Graphics::CGBuffer* GetGBuffer()const { return m_pGBuf.get(); }
			
			// *@モード取得
			const EStackMode GetStackMode() { return m_eMode; }

			// *@モード設定
			void SetStackMode(const EStackMode mode) { m_eMode = mode; }



#ifdef BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MDOE


		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CStackCamera)

#endif // !__STACK_CAMERA_COMPONENT_H__