//==========================================================
// [volume.h]
//----------------------------------------------------------
// 
//==========================================================

#ifndef __VOLUME_COMPONENT_H__
#define __VOLUME_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GraphicsSystem/PostProcess/postProcess.h>

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
						/*CEREAL_NVP(m_pPost),*/ CEREAL_NVP(m_nPriority), CEREAL_NVP(m_nID)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("VolumeComponent", cereal::base_class<CComponent>(this)),
						/*CEREAL_NVP(m_pPost),*/ CEREAL_NVP(m_nPriority), CEREAL_NVP(m_nID)
				);
			}
#pragma endregion

		private:
			std::unique_ptr<MySpace::Graphics::CPostProcess> m_pPost;
			int m_nPriority;
			int m_nID;

		public:
			CVolume();
			CVolume(CGameObject::Ptr owner);
			~CVolume();

			void Awake();
			void Init();

			// *@所持PostProcess取得
			MySpace::Graphics::CPostProcess* GetEffect()const { return m_pPost.get(); }

			// *@優先度取得
			const int GetPriority() { return m_nPriority; }

			// *@優先度設定
			void SetPriority(const int value) { m_nPriority = value; }

			// *@適用するLayerか確認
			bool IsLayer(const int layerNo);

			// *@ポストプロセス設定用
			template <class T>
			void SetPostProcess()
			{
				if (m_pPost)
					m_pPost.reset();
				m_pPost = std::make_unique<T>();
			}

#if BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

#endif // !__VOLUME_COMPONENT_H__
