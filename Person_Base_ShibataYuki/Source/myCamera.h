//=========================================================
// [myCamera.h] 
// �쐬:2022/08/18
//---------------------------------------------------------
//=========================================================

#ifndef __MY_CAMERA_COMPONENT_H__
#define __MY_CAMERA_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/camera.h>


//--- �N���X��`
class CCentipede;
class CMyCamera : public CCamera
{
private:
	std::weak_ptr<CCentipede> m_pTarget;
	float m_rotation;
	float m_rotationValue;
	float m_distance;
	float m_fOffset;

public:
	CMyCamera() {};
	CMyCamera(std::shared_ptr<CGameObject> owner);
	~CMyCamera() {};

	void Awake();
	void Init();
	void Update();
	void FindPlayer()
	{
		if (auto obj = CGameObject::FindGameObjectWithTag("Player").lock(); obj)
			m_pTarget = obj->GetComponent<CCentipede>();
	}
	inline void SetOffset(float value) { m_fOffset = value; }
	inline float GetOffset() { return m_fOffset; }
};

#endif // !__MY_CAMERA_COMPONENT_H__
