//==========================================================
// [imageSwitch.cpp]
//----------------------------------------------------------
// �쐬:2023/06/08
// 
//==========================================================

#include <GameSystem/DarkGame/imageSwitch.h>
#include <GraphicsSystem/Texture/imageResource.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CImageSwitch::CImageSwitch()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CImageSwitch::CImageSwitch(CGameObject::Ptr ptr)
	:CAnimeUI(ptr)
{

}

//==========================================================
// ����������
//==========================================================
void CImageSwitch::Awake()
{
	CAnimeUI::Awake();

	// �摜�p�X
	// �񋓑̂Ɠ������Ԃ�
	LPCSTR imagePath[(int)EDispImage::Max] = {
		"",
	};

	// �摜�̓ǂݍ��݂ƕۑ�
	for (int cnt = 0; cnt < (int)EDispImage::Max; cnt++)
	{
		// �ǂݍ���
		m_pPolygon.lock()->GetSprite()->SetImage(imagePath[cnt]);
		// �߲���̕ۑ�
		m_aImages.push_back(m_pPolygon.lock()->GetSprite()->GetImage());
	}
}

//==========================================================
// ������
//==========================================================
void CImageSwitch::Init()
{

}

//==========================================================
// �X�V
//==========================================================
void CImageSwitch::Update()
{
	CAnimeUI::Update();
}

//==========================================================
// �摜�ؑ�
//==========================================================
void CImageSwitch::SwicthImage(const EDispImage eImage)
{
	auto imagePtr = m_aImages[(int)eImage];
	m_pPolygon.lock()->GetSprite()->SetImagePtr(imagePtr.lock());
}

//==========================================================
// �摜�ؑւɍ��킹�ı�Ұ��݊J�n
//==========================================================
void CImageSwitch::StartAnim(const EDispImage eImage)
{
	CAnimeUI::StartAnim();
	SwicthImage(eImage);
}