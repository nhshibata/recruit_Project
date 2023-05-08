//=========================================================
// [light.h]
// �쐬:2022/06/27
// 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __LIGHT_COMPONENT_H__
#define __LIGHT_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CLight : public CComponent
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("light", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bEnable)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("light", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_bEnable)
				);
			}
		private:
			bool m_bEnable;			// ���C�e�B���O�L��/����
			int m_nSystemIdx;
			static inline CLight* m_pMainLight = nullptr;

		public:
			//--- ���ފ֐�
			CLight();
			CLight(std::shared_ptr<CGameObject> owner);
			~CLight();

			void OnLoad();
			virtual void Awake();			// ������ ���R���|�[�l���g�̎擾�Ȃǂ��s��
			//void LateUpdate();			// �x���X�V

			inline void SetEnable(bool bEnable = true) { m_bEnable = bEnable; }
			inline void SetDisable(bool bDisable = true) { m_bEnable = !bDisable; }
			inline bool IsEnable() { return m_bEnable; }

			static CLight* GetMain();
			static void Set(CLight* pLight = nullptr);

#ifdef BUILD_MODE
			// �Z�b�^�[�E�Q�b�^�[
			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CLight)

#endif // !__PLAYER_H__
