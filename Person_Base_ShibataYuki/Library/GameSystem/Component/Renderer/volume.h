//==========================================================
// [volume.h]
//----------------------------------------------------------
// �쐬:2023/03/06
// �X�V:2023/03/12 Renderer�L���b�V���@�\�ǉ�
//----------------------------------------------------------
// 
//==========================================================

#ifndef __VOLUME_COMPONENT_H__
#define __VOLUME_COMPONENT_H__

//--- �C���N���[�h��
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
			//--- �V���A���C�Y
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
			std::unordered_map<int, std::vector<int>> m_aRenderIDCash;  // �L���b�V���p

		public:
			CVolume();
			CVolume(CGameObject::Ptr owner);
			~CVolume();

			void Awake();
			void Init();

			// *@�`��ID�ǉ�
			void AddRendererID(const int nID);

			void ResetRenderCash();

			std::vector<int> GetRenderCash()const;

			// *@����PostProcess�擾
			MySpace::Graphics::CPostProcess* GetEffect()const { return m_pPost.get(); }

			// *@�D��x�擾
			const int GetPriority() { return m_nPriority; }

			// *@�D��x�ݒ�
			void SetPriority(const int value) { m_nPriority = value; }

			// *@�|�X�g�v���Z�X�ݒ�p
			template <class T>
			void SetPostProcess()
			{
				if (m_pPost)
					m_pPost.reset();
				m_pPost = std::make_unique<T>();
			}

			// *@�K�p����Layer���m�F
			bool IsLayer(const int layerNo);

#if BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MODE

		};

	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CVolume)

#endif // !__VOLUME_COMPONENT_H__
