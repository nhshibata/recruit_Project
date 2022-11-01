//=========================================================
// [gameUI.h] 
// 作成:2022/08/18
//---------------------------------------------------------
//=========================================================
/*
	描画するもの
	1.長さ
	2.胴体の数
	3.経験値
	4.速度
*/

//--- インクルードガード
#ifndef __GAME_UI_COMPONENT_H__
#define __GAME_UI_COMPONENT_H__

//--- インクルード部
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
