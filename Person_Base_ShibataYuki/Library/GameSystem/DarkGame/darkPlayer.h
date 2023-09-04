//==========================================================
// [darkPlayer.h]
//----------------------------------------------------------
// �쐬:2023/05/21
// �X�V:2023/05/21 cpp����
// �X�V:2023/06/08 ���N���X�ύX
// 
//==========================================================

#ifndef __DRAK_PLAYER_H__
#define __DRAK_PLAYER_H__

#include <GameSystem/DarkGame/moveCharacter.h>
#include <GameSystem/DarkGame/lightCollisionRegistry.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CDarkPlayer : public CMoveCharacter
	{
	public:
		//--- �����o�ϐ�
		static constexpr LPCSTR TAG = "Player";	// �擾�p�Ɍ��J����

	private:
		std::weak_ptr<CLightCollisionRegistry> m_pLightCol;

	public:
		//--- �����o�֐�
		CDarkPlayer();
		CDarkPlayer(CGameObject::Ptr ptr);
		~CDarkPlayer();

		void Awake();
		void Update();
		void LateUpdate();


	};

}

#endif // !__DRAK_PLAYER_H__
