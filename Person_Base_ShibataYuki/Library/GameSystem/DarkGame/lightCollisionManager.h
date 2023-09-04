//==========================================================
// [lightCollisionManager.h]
//----------------------------------------------------------
// �쐬:2023/06/01
//----------------------------------------------------------
// ���C�g�͈̔͊m�F�N���X���Ǘ�����
//==========================================================

#ifndef __LIGHT_COLLISION_MANAGER_H__
#define __LIGHT_COLLISION_MANAGER_H__

#include <GameSystem/Component/component.h>

#pragma region ForwardDeclaration

namespace DarkGame 
{
	class CLightCollision;
}

#pragma endregion


namespace DarkGame
{
	using namespace MySpace::Game;

	class CLightCollisionManager : public CComponent
	{
	private:
		std::vector<CLightCollision*> m_aLights; // �V�[�����ɐ�������\��͂Ȃ��̂ŁA���I�z��

	public:
		static constexpr LPCSTR TAG = "LightManager";

	public:
		//--- �����o�֐�
		CLightCollisionManager();
		CLightCollisionManager(CGameObject::Ptr ptr);
		virtual ~CLightCollisionManager();

		void Awake();
		void Init();
		void Update();

		// *@�ǉ�
		void AddList(CLightCollision* add) { m_aLights.push_back(add); }

	};
}

#endif // !__LIGHT_COLLISION_MANAGER_H__
