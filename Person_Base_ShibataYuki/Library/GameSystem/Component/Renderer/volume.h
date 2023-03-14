//==========================================================
// [volume.h]
//----------------------------------------------------------
// 作成:2023/03/06
// 更新:2023/03/12 Rendererキャッシュ機能追加
//----------------------------------------------------------
// 
//==========================================================

#ifndef __VOLUME_COMPONENT_H__
#define __VOLUME_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GraphicsSystem/PostProcess/postProcess.h>

#include <GraphicsSystem/PostProcess/bloom.h>
#include <GraphicsSystem/PostProcess/negative.h>
#include <GraphicsSystem/PostProcess/monochrome.h>
#include <GraphicsSystem/PostProcess/outline.h>

namespace MySpace
{
	namespace Game
	{

		class CVolume : public CComponent
		{
#pragma region cereal
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("VolumeComponent", cereal::base_class<CComponent>(this)),
						CEREAL_NVP(m_pPost), CEREAL_NVP(m_nPriority)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("VolumeComponent", cereal::base_class<CComponent>(this)),
						CEREAL_NVP(m_pPost), CEREAL_NVP(m_nPriority)
				);
			}
#pragma endregion

		private:
			std::unique_ptr<MySpace::Graphics::CPostProcess> m_pPost;
			int m_nPriority;
			int m_nID;
			std::unordered_map<int, std::vector<int>> m_aRenderIDCash;  // キャッシュ用

		public:
			CVolume();
			CVolume(CGameObject::Ptr owner);
			~CVolume();

			void Awake();
			void Init();

			// *@描画ID追加
			void AddRendererID(const int nID);

			void ResetRenderCash();

			std::vector<int> GetRenderCash()const;

			// *@所持PostProcess取得
			MySpace::Graphics::CPostProcess* GetEffect()const { return m_pPost.get(); }

			// *@優先度取得
			const int GetPriority() { return m_nPriority; }

			// *@優先度設定
			void SetPriority(const int value) { m_nPriority = value; }

			// *@ポストプロセス設定用
			template <class T>
			void SetPostProcess()
			{
				if (m_pPost)
					m_pPost.reset();
				m_pPost = std::make_unique<T>();
			}

			// *@適用するLayerか確認
			bool IsLayer(const int layerNo);

#if BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CVolume)

#endif // !__VOLUME_COMPONENT_H__
