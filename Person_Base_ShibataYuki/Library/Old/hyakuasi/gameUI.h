//=========================================================
// [gameUI.h] 
// �쐬:2022/08/18
//---------------------------------------------------------
//=========================================================
/*
	�`�悷�����
	1.����
	2.���̂̐�
	3.�o���l
	4.���x
*/

//--- �C���N���[�h�K�[�h
#ifndef __GAME_UI_COMPONENT_H__
#define __GAME_UI_COMPONENT_H__

//--- �C���N���[�h��
#include <centipede.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <vector>

class CGameUI : public CComponent
{
private:
	enum class E_UI
	{
		LENGTH,
		BODY,
		EXP,
		SPEED,
	};
private:
	std::weak_ptr<CCentipede> m_pHead;
	std::vector<std::weak_ptr<CPolygonRenderer>> m_pLength;
	std::weak_ptr<CPolygonRenderer> m_pGauge;
	std::weak_ptr<CTextRenderer> m_pNumber;
	int m_nPrevLife = 0;

public:
	CGameUI() {};
	CGameUI(std::shared_ptr<CGameObject> owner) : CComponent(owner) {};
	~CGameUI() {};

	void Awake();
	void Init();
	void Update();
};

#endif // !__GAME_UI_COMPONENT_H__
