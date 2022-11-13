//=========================================================
// �쐬:2022/04/21
// �J�[�\��: ���i�׽
//=========================================================
//--- �C���N���[�h�K�[�h
#ifndef __CURSOR_COMPONENT_H__
#define __CURSOR_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CCursor : public CComponent
		{
		private:
			// �V���A���C�Y
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("cursor", cereal::base_class<CComponent>(this)), CEREAL_NVP(m_nPoint),
					CEREAL_NVP(m_nMax),CEREAL_NVP(m_bAuthority)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("cursor", cereal::base_class<CComponent>(this)), CEREAL_NVP(m_nPoint),
					CEREAL_NVP(m_nMax), CEREAL_NVP(m_bAuthority)
				);
			}

		private:
			//--- �����o�ϐ�
			bool m_bInput;				// �J�[�\�����ړ����Ȃ����p
			int m_nPoint;				// 
			int m_nOldPoint;			// 
			int m_nMax;					// �ő吔

			bool m_bAuthority;			// ���쌠��

		public:
			//--- ���ފ֐�
			CCursor();
			CCursor(std::shared_ptr<CGameObject> owner);
			~CCursor();

			void Init();
			void Update();

			void InputSwitch() { m_bInput ^= true; };

			//--- �Z�b�^�[�E�Q�b�^�[
			int GetPos() { return m_nPoint; };
			int GetOldPos() { return m_nOldPoint; };

			void SetMax(int value) { m_nMax = value; };
			void SetAuthority(bool flg) { m_bAuthority = flg; }
			bool IsAuthority() { return m_bAuthority; }
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CCursor)

#endif // !__CURSOR_H__

