//==========================================================
// [imageSwitch.cpp]
//----------------------------------------------------------
// 作成:2023/06/08
// 
//==========================================================

#include <GameSystem/DarkGame/imageSwitch.h>
#include <GraphicsSystem/Texture/imageResource.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CImageSwitch::CImageSwitch()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CImageSwitch::CImageSwitch(CGameObject::Ptr ptr)
	:CAnimeUI(ptr)
{

}

//==========================================================
// 生成時処理
//==========================================================
void CImageSwitch::Awake()
{
	CAnimeUI::Awake();

	// 画像パス
	// 列挙体と同じ順番に
	LPCSTR imagePath[(int)EDispImage::Max] = {
		"",
	};

	// 画像の読み込みと保存
	for (int cnt = 0; cnt < (int)EDispImage::Max; cnt++)
	{
		// 読み込み
		m_pPolygon.lock()->GetSprite()->SetImage(imagePath[cnt]);
		// ﾎﾟｲﾝﾀの保存
		m_aImages.push_back(m_pPolygon.lock()->GetSprite()->GetImage());
	}
}

//==========================================================
// 初期化
//==========================================================
void CImageSwitch::Init()
{

}

//==========================================================
// 更新
//==========================================================
void CImageSwitch::Update()
{
	CAnimeUI::Update();
}

//==========================================================
// 画像切替
//==========================================================
void CImageSwitch::SwicthImage(const EDispImage eImage)
{
	auto imagePtr = m_aImages[(int)eImage];
	m_pPolygon.lock()->GetSprite()->SetImagePtr(imagePtr.lock());
}

//==========================================================
// 画像切替に合わせてｱﾆﾒｰｼｮﾝ開始
//==========================================================
void CImageSwitch::StartAnim(const EDispImage eImage)
{
	CAnimeUI::StartAnim();
	SwicthImage(eImage);
}