//=========================================================
// [ground.h] 
// 作成:2022/09/07
//---------------------------------------------------------
//=========================================================

#ifndef __GROUND_COMPONENT_H__
#define __GROUND_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>

class CGround : public CComponent
{
public:
	CGround() {};
	CGround(std::shared_ptr<CGameObject> owner) :CComponent(owner) {};
	~CGround() {};

	void Awake();
	void Init();
	//void Update();

	void OnCollisionEnter(CGameObject* other);
	void OnCollisionStay(CGameObject* other);
	void OnCollisionExit(CGameObject* other);
};
class CGroundManager : public CComponent
{
private:
	ModelWeakPtr m_pModelPtr;
	
public:
	CGroundManager() {};
	CGroundManager(std::shared_ptr<CGameObject> owner) :CComponent(owner) {};
	~CGroundManager() {};

	void Awake();
	void Update();
};

#endif __GROUND_COMPONENT_H__