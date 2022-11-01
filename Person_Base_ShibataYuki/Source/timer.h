//=========================================================
// [timer.h] 
// 作成:2022/09/08
//---------------------------------------------------------
//=========================================================

#ifndef __TIMER_COMPONENT_H__
#define __TIMER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/textRenderer.h>


class CTimer : public CComponent
{
public:
	static inline constexpr const char* TAG_NAME = "Timer";
private:
	std::weak_ptr<CTextRenderer> m_pText;
	float m_fTime = 120;
	//float m_fMaxTime;

public:
	CTimer() {};
	CTimer(std::shared_ptr<CGameObject> owner)
		:CComponent(owner){}
	~CTimer() {};

	void Awake();
	void Update();

	void SetTime(float max) { m_fTime = max; };
	float GetTime() { return m_fTime; }
};

#endif //!__TIMER_COMPONENT_H__