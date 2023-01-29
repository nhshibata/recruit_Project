//=========================================================
// [myCamera.h] 
// 作成:2022/09/21
//---------------------------------------------------------
//=========================================================

#ifndef __SPELL_GAME_CAMERA_COMPONENT_H__
#define __SPELL_GAME_CAMERA_COMPONENT_H__

//--- インクルード部
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Camera/camera.h>

namespace Spell
{
	using namespace MySpace::Game;

	//--- クラス定義
	class CGameCamera : public CCamera
	{
	private:
		float m_fOffset;
		float m_fShakeTime;

	public:
		CGameCamera()
			:m_fShakeTime(0), m_fOffset(0) 
		{};
		CGameCamera(std::shared_ptr<CGameObject> owner)
			:CCamera(owner), m_fShakeTime(0), m_fOffset(0)
		{};
		~CGameCamera()
		{};

		void Awake();
		void Init();
		void Update();

		inline void SetOffset(float value) { m_fOffset = value; }
		inline float GetOffset() { return m_fOffset; }
	};
}

#endif // !__SPELL_GAME_CAMERA_COMPONENT_H__
