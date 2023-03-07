//==========================================================
// [volume.h]
//----------------------------------------------------------
// 
//==========================================================

#ifndef __VOLUME_COMPONENT_H__
#define __VOLUME_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <GraphicsSystem/PostProcess/postProcess.h>

namespace MySpace
{
	namespace Game
	{

		class CVolume : public CComponent
		{
#pragma region cereal
			//--- �V���A���C�Y
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

			// *@����PostProcess�擾
			MySpace::Graphics::CPostProcess* GetEffect()const { return m_pPost.get(); }

			// *@�D��x�擾
			const int GetPriority() { return m_nPriority; }

			// *@�D��x�ݒ�
			void SetPriority(const int value) { m_nPriority = value; }

			// *@�K�p����Layer���m�F
			bool IsLayer(const int layerNo);

			// *@�|�X�g�v���Z�X�ݒ�p
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
