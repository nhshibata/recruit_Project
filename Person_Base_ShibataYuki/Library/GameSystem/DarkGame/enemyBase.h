//==========================================================
// [enemyBase.h]
//----------------------------------------------------------
// �쐬:2023/05/19 
// �X�V:2023/06/08 ���N���X��ύX
// 
//==========================================================

#ifndef __DARK_ENEMY_BASE_H__
#define __DARK_ENEMY_BASE_H__

#include <GameSystem/DarkGame/moveCharacter.h>
#include <GameSystem/DarkGame/lightCollisionCondition.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CEnemyBase : public CMoveCharacter
	{
	public :
		static constexpr LPCSTR TAG = "Enemy";	// �擾�p�Ɍ��J����

	private:
		std::weak_ptr<CLightCollisionCondition> m_pLightCol;

	public :
		//--- �����o�֐�
		CEnemyBase();
		CEnemyBase(CGameObject::Ptr ptr);
		virtual ~CEnemyBase();
		
		virtual void Awake();
		void Update();
		virtual void LateUpdate();

		// *@�͈͊m�F
		bool IsRange();

		// *@����������
		virtual void DiscoveryEffect();
	};

}

#endif // !__DARK_ENEMY_BASE_H__
