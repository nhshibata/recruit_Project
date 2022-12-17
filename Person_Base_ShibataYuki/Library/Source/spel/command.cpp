//=========================================================
// [command.cpp]
// 作成:2022/09/21
//---------------------------------------------------------
// 入力確認したいキーを確認し、格納する
// 時間経過でリセット
// 他のｺﾝﾎﾟｰﾈﾝﾄが取得、リセット等を行う
//=========================================================

//--- インクルード部
#include <command.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <CoreSystem/Time/fps.h>

using namespace Spell;

#define _DISP_KEY	(1)
#if _DISP_KEY
std::weak_ptr<CTextRenderer> testText;
#endif

void CCommandStock::Awake()
{
#if _DISP_KEY
	testText = this->AddComponent<CTextRenderer>();
	testText.lock()->GetRectTransform()->SetPos({ -CScreen::GetWidth() / 3, CScreen::GetHeight() / 3 });
#endif // _DEBUG
	
	m_nInputList.push_back(VK_U);
	m_nInputList.push_back(VK_I);
	m_nInputList.push_back(VK_O);
	m_nInputList.push_back(VK_P);
}
void CCommandStock::Init()
{

}
void CCommandStock::Update()
{
	if (InputInterval.accessor <= 0)
		return;

#if BUILD_MODE	// 文字列として入力情報を確認する
	std::wstring text, wText;
	for (auto & nKey : m_nInputList)
	{	// empty?
		wText = testText.lock()->VKeyToWString(nKey);
		text += wText;
	}
	testText.lock()->SetTextWString(text);
#endif // !_DEBUG

	// 現在フレームの入力を取得
	for (auto &key : m_nInputList)
	{	
		if (CInput::GetKeyTrigger(key))// ﾄﾘｶﾞｰのみを確認
		{
			// 入力があったので格納、インターバルを初期化
			m_nInputBuffer.push_back(key);
			InputInterval.accessor = 0.0f;
		}
	}

	// インターバル更新&確認
	InputInterval.accessor += CFps::Get()->DeltaTime();
	if (InputInterval.accessor > InputMaxInterval.accessor)
	{
		InputInterval.accessor = 0.0f;
		m_nInputList.clear();
		// TODO: リセットSE再生
	}
}
void CCommandStock::FixedUpdate()
{

}