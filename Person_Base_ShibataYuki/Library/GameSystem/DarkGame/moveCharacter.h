//==========================================================
// [moveCharacter.h]
//----------------------------------------------------------
// �쐬:2023/06/08
// �X�V:2023/
// 
//==========================================================

#ifndef __DRAK_MOVE_CHARACTER_H__
#define __DRAK_MOVE_CHARACTER_H__

#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Light/pointLight.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CMoveCharacter : public CComponent
	{
	private: 
		bool m_bMove;
	protected:
		//--- �����o�ϐ�
		std::weak_ptr<CPointLight> m_pPointLight;

	public:
		//--- �����o�֐�
		CMoveCharacter();
		CMoveCharacter(CGameObject::Ptr ptr);
		virtual ~CMoveCharacter();

		void Awake();
		void Update();

		void SetMoving(bool flg) { m_bMove = flg; }
		const bool IsMoving()const { return m_bMove; }

		// *@�������C�g�̎擾
		CPointLight* GetLight()const { return m_pPointLight.lock().get(); }
	};

}

#endif // !__DRAK_MOVE_CHARACTER_H__