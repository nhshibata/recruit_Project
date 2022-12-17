//=========================================================
// 作成:2022/04/21
// カーソル: 部品ｸﾗｽ
// unityでカーソルを管理している方法と同じ
//=========================================================

//--- インクルード部
#include <GameSystem/Component/cursor.h>
#include <GameSystem/GameObject/gameObject.h>
#include <CoreSystem/Input/input.h>

using namespace MySpace::Game;

// コンストラクタ
CCursor::CCursor()
	:m_bInput(false),m_nMax(1),m_nPoint(0),m_nOldPoint(0), m_bAuthority(false)
{
}
// 引数付きコンストラクタ
CCursor::CCursor(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bInput(false), m_nMax(1), m_nPoint(0), m_nOldPoint(0), m_bAuthority(false)
{
}
// デストラクタ
CCursor::~CCursor()
{

}
// 初期化
void CCursor::Init()
{

}
// 更新
void CCursor::Update()
{
	if (m_nPoint != m_nOldPoint)
	{
		m_nOldPoint = m_nPoint;
	}
}