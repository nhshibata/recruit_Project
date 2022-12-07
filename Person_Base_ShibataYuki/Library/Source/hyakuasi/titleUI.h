//=========================================================
// [titleUI.h] 
// �쐬:2022/08/18
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __TITLE_UI_COMPONENT_H__
#define __TITLE_UI_COMPONENT_H__

//--- �C���N���[�h��
#include <centipede.h>
#include <fadeController.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>


//
class CTitleUI : public CComponent
{
public:

private:
	std::weak_ptr<CPolygonRenderer> m_pPolygon;

	float m_fFadeTime;
public:
	CTitleUI() {};
	CTitleUI(std::shared_ptr<CGameObject> owner);
	~CTitleUI() {};
	void Awake();
	void Init();
	void Update();
};


#endif //__TITLE_UI_COMPONENT_H__
