//=========================================================
// [resultUI.h] 
// 作成:2022/08/18
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __RESULT_UI_COMPONENT_H__
#define __RESULT_UI_COMPONENT_H__

//--- インクルード部
#include <centipede.h>
#include <fadeController.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>

//
class CResultUI : public CComponent
{
public:

private:
	std::weak_ptr<CTextRenderer> m_pResultText;
	std::weak_ptr<CTextRenderer> m_pButtonText;
	std::weak_ptr<CTextRenderer> m_pLengthText;
	int m_nDispNum = 0;

public:
	CResultUI() {};
	CResultUI(std::shared_ptr<CGameObject> owner) :CComponent(owner) {};
	~CResultUI() {};
	void Awake();
	void Init();
	void Update();
	void SetNum(int n) { m_nDispNum = n; }
};


#endif __RESULT_UI_COMPONENT_H__