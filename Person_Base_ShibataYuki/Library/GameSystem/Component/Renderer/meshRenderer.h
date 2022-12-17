//=========================================================
// [meshRenderer.h] 
// 作成: 2022/06/27
// 更新: 2022/08/20 描画時に描画するか判定を行うため、バウンディングを追加
// 更新: 2022/12/07 NavMesh用にStaticを追加
//---------------------------------------------------------
// 3D描画
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
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius),CEREAL_NVP(m_nStaticMode)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("meshRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_vCenter), CEREAL_NVP(m_bLightEnable), CEREAL_NVP(m_fBSRadius), CEREAL_NVP(m_nStaticMode)
				);
			}
		public:
			//--- 列挙体定義
			enum class EStaticMode : int // bit管理するなら追加
			{
				NONE = 0,
				NONE_MOVE = 1 << 0,
				DYNAMIC,
				MAX
			};

		private:
			//--- メンバ変数
			Vector3 m_vCenter;				// *@中心座標
			bool m_bLightEnable;			// *@ライト有効フラグ
			float m_fBSRadius = 1;			// *@バウンディングスフィア
			int m_nStaticMode;				// *@static状態
			CMeshMaterial m_MeshMaterial;

		protected:
			void SetInstancing(CMesh* mesh, std::string name = std::string());

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
			inline CMeshMaterial* GetMaterial() { return &m_MeshMaterial; }
			inline int GetStatic() { return m_nStaticMode; }

			inline void SetLightEnable(bool flg) { m_bLightEnable = flg; };
			inline void SetCenter(Vector3 value) { m_vCenter = value; }
			inline void SetBSRadius(const float value) { m_fBSRadius = value; };
			inline void SetStatic(EStaticMode value) { m_nStaticMode = static_cast<int>(value); }

#if BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CMeshRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CMeshRenderer)

#endif // !__RENDERER_H_
