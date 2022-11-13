//=========================================================
// [meshRenderer.h] 
// 作成: 2022/06/27
// 作成: 2022/08/20 描画時に描画するか判定を行うため、バウンディングを追加
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __MESH_RENDERER_COMPONENT_H__
#define __MESH_RENDERER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/renderer.h>
#include <GraphicsSystem/Render/mesh.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		class CMeshRenderer : public CRenderer
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius)
				);
			}
		private:
			//--- メンバ変数
			Vector3 m_vCenter;			// *@
			bool m_bLightEnable;		// *@
			float m_fBSRadius = 1;		// *@バウンディングスフィア
		
		public:
			//--- メンバ関数
			CMeshRenderer();
			CMeshRenderer(std::shared_ptr<CGameObject> owner);
			virtual ~CMeshRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- ゲッター・セッター
			inline bool GetLightEnable() { return m_bLightEnable; };
			inline Vector3 GetCenter() { return m_vCenter; }
			Vector3 GetCenter(int n);
			inline float GetBSRadius()const { return m_fBSRadius; };		// *@バウンディングスフィア

			inline void SetLightEnable(bool flg) { m_bLightEnable = flg; };
			inline void SetCenter(Vector3 value) { m_vCenter = value; }
			inline void SetBSRadius(const float value) { m_fBSRadius = value; };

			// over ride
			virtual void SetMaterial(CMeshMaterial mat) {};
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CMeshRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CMeshRenderer)

#endif // !__RENDERER_H_
