//=========================================================
// [myCamera.h] 
// �쐬:2022/09/21
//---------------------------------------------------------
//=========================================================

#ifndef __GAME_CAMERA_COMPONENT_H__
#define __GAME_CAMERA_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/camera.h>

namespace Spell
{
	//--- �N���X��`
	class CGameCamera : public CCamera
	{
	private:
		float m_fOffset;
		float m_fShakeTime;

	public:
		CGameCamera() {};
		CGameCamera(std::shared_ptr<CGameObject> owner) :CCamera(owner){};
		~CGameCamera() {};

		void Awake();
		void Init();
		void Update();

		inline void SetOffset(float value) { m_fOffset = value; }
		inline float GetOffset() { return m_fOffset; }
	};
}
#endif // !__GAME_CAMERA_COMPONENT_H__
