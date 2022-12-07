//=========================================================
// [modelRenderer.h] 
// 作成: 2022/06/27
// 更新: 2022/07/04 実装
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __MODEL_RENDERER_COMPONENT_H__
#define __MODEL_RENDERER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GraphicsSystem/Manager/modelManager.h>

#define MODEL_PATH				FORDER_DIR(Data/model/)
#define MODEL_PATH2(name)		FORDER_DIR(Data/model/name)
#define CHARACTER_PATH(name)	FORDER_DIR(Data/model/character/name)

namespace MySpace
{
	namespace Game
	{
		class CModelRenderer : public CMeshRenderer
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("modelRender", cereal::base_class<CMeshRenderer>(this)),
					/*CEREAL_NVP(m_pModelCom),*/ CEREAL_NVP(m_modelName)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("modelRender", cereal::base_class<CMeshRenderer>(this)),
					/*CEREAL_NVP(m_pModelCom),*/ CEREAL_NVP(m_modelName)
				);
			}
		public:
			using PtrWeak = std::weak_ptr<CModelRenderer>;
		private:
			ModelWeakPtr m_pModel;
			std::string m_modelName;
			UINT m_nVertex;				// 頂点数
			TAssimpVertex* m_pVertex;	// 頂点配列
			UINT m_nIndex;				// インデックス数
			UINT* m_pIndex;				// インデックス配列

		private:
			void InitVertexArray();
			void FinVertexArray();

		public:
			CModelRenderer() {};
			CModelRenderer(std::shared_ptr<CGameObject> owner);
			~CModelRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();
			bool Draw(int);

			// レイとの当たり判定
			bool CollisionRay(XMFLOAT3 vP0, XMFLOAT3 vW, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);

			// 線分との当たり判定
			bool CollisionLineSegment(XMFLOAT3 vP0, XMFLOAT3 vP1, XMFLOAT3* pX = nullptr, XMFLOAT3* pN = nullptr);

			//--- ゲッター・セッター
			inline void SetModel(ModelWeakPtr model) { m_pModel = model; /*m_modelName = model.lock()->GetFileName();*/ }
			void SetModel(std::string name);
			
			inline ModelWeakPtr GetModel() { return m_pModel; }
			inline std::string GetModelName() { return m_modelName; }

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CModelRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent, MySpace::CModelRenderer)

#endif // !__MODEL_RENDERER_H__
