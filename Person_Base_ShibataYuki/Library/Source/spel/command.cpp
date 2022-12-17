//=========================================================
// [command.cpp]
// �쐬:2022/09/21
//---------------------------------------------------------
// ���͊m�F�������L�[���m�F���A�i�[����
// ���Ԍo�߂Ń��Z�b�g
// ���̺��߰��Ă��擾�A���Z�b�g�����s��
//=========================================================

//--- �C���N���[�h��
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

#if BUILD_MODE	// ������Ƃ��ē��͏����m�F����
	std::wstring text, wText;
	for (auto & nKey : m_nInputList)
	{	// empty?
		wText = testText.lock()->VKeyToWString(nKey);
		text += wText;
	}
	testText.lock()->SetTextWString(text);
#endif // !_DEBUG

	// ���݃t���[���̓��͂��擾
	for (auto &key : m_nInputList)
	{	
		if (CInput::GetKeyTrigger(key))// �ضް�݂̂��m�F
		{
			// ���͂��������̂Ŋi�[�A�C���^�[�o����������
			m_nInputBuffer.push_back(key);
			InputInterval.accessor = 0.0f;
		}
	}

	// �C���^�[�o���X�V&�m�F
	InputInterval.accessor += CFps::Get()->DeltaTime();
	if (InputInterval.accessor > InputMaxInterval.accessor)
	{
		InputInterval.accessor = 0.0f;
		m_nInputList.clear();
		// TODO: ���Z�b�gSE�Đ�
	}
}
void CCommandStock::FixedUpdate()
{

}