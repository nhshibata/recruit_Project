//=========================================================
// [rectTrabsform.h]
// 作成:2022/06/28
//=========================================================

#ifndef __RECT_TRANSFORM_COMPONENT_H__
#define __RECT_TRANSFORM_COMPONENT_H__

#include <GameSystem/Component/component.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Game
	{
		class CRectTransform;
		using RectTransSharedPtr = std::shared_ptr<CRectTransform>;
		using RectTransWeakPtr = std::weak_ptr<CRectTransform>;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		class CRectTransform : public CComponent
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("rectTransform", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vSize), CEREAL_NVP(m_fAngle)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("rectTransform", cereal::base_class<CBaseComponent>(this)),
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vSize), CEREAL_NVP(m_fAngle)
				);
			}
		private:
			//--- メンバ変数
			Vector2 m_vPos;
			Vector2 m_vSize;
			float m_fAngle = 0.0f;

		public:
			//--- メンバ関数
			CRectTransform() :m_vPos(0.0f, 0.0f), m_vSize(100, 100), m_fAngle(0.0f) 
			{
			};
			CRectTransform(std::shared_ptr<CGameObject> owner);
			~CRectTransform();

			void Init() {};
			void Update() {};

			//--- ゲッター・セッター
			inline Vector2 GetPos() { return m_vPos; }
			inline Vector2 GetSize() { return m_vSize; }
			inline float GetAngle() { return m_fAngle; }
			
			inline void SetPos(Vector2 value) {  m_vPos = value; }
			inline void SetPos(float x, float y) { m_vPos.x = x; m_vPos.y = y; }
			inline void SetSize(Vector2 value) {  m_vSize = value; }
			inline void SetSize(float x, float y) {  m_vSize.x = x; m_vSize.y = y; }
			inline void SetAngle(float value) {  m_fAngle = value; }
			
#ifdef BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CRectTransform)

#endif // !__RECT_TRANSFORM_H__
