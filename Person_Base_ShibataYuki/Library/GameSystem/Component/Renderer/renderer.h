//=========================================================
// [renderer.h] 
// 作成: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __BASE_RENDERER_COMPONENT_H__
#define __BASE_RENDERER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::MyMath;

		class CRenderer : public CComponent
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("baseRender", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bVisible), CEREAL_NVP(m_vColor)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("baseRender", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bVisible), CEREAL_NVP(m_vColor)
				);

				// FIXME: 基底クラスで呼び出せば手間が省ける?
				Init();
			}

		private:
			//--- メンバ変数
			bool m_bVisible;
			Color m_vColor;

			int m_nDrawIdx = -1;

		private:
			//--- メンバ関数
			void RequestDraw();
		public:
			CRenderer() {};
			CRenderer(std::shared_ptr<CGameObject> owner);
			virtual ~CRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- セッター・ゲッター
			_NODISCARD inline Color GetColor() { return m_vColor; }
			_NODISCARD inline XMFLOAT4 GetColor(int num) { return XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a); }
			// *@ｲﾝﾃﾞｯｸｽ取得
			_NODISCARD inline UINT GetIdx() { return m_nDrawIdx; }

			void SetLayer(int value);
			inline void SetColor(Color color) { m_vColor = color; }
			inline void SetVisible(bool value) { m_bVisible = value; }

			// *描画状態確認
			inline bool IsVisible() { return m_bVisible; }

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent<CComponent>, MySpace::CRenderer)

#endif // !__RENDERER_H_
