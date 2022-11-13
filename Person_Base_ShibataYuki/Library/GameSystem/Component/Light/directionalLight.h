//=========================================================
// �쐬:2022/06/27
// 
// 
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DIRECTIONAL_LIGHT_COMPONENT_H__
#define __DIRECTIONAL_LIGHT_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Light/light.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CDirectionalLight : public CLight
		{
		private:
			// �V���A���C�Y
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("directionalLight", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_diffuse), CEREAL_NVP(m_ambient), CEREAL_NVP(m_specular), CEREAL_NVP(m_direction)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("directionalLight", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_diffuse), CEREAL_NVP(m_ambient), CEREAL_NVP(m_specular), CEREAL_NVP(m_direction)
				);
			}

		private:
			//--- �����o�ϐ�
			Vector4 m_diffuse;
			Vector4 m_ambient;
			Vector4 m_specular;
			Vector3 m_direction;

		public:
			//--- ���ފ֐�
			CDirectionalLight() {};
			CDirectionalLight(std::shared_ptr<CGameObject> owner);
			~CDirectionalLight();

			void Init();				// ������ ���R���|�[�l���g�̎擾�Ȃǂ��s��

			inline XMFLOAT4& GetDiffuse() { return m_diffuse; }
			inline XMFLOAT4& GetAmbient() { return m_ambient; }
			inline XMFLOAT4& GetSpecular() { return m_specular; }
			XMFLOAT3& GetDir();

#ifdef BUILD_MODE

			// �Z�b�^�[�E�Q�b�^�[
			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CDirectionalLight)

#endif // !__PLAYER_H__
