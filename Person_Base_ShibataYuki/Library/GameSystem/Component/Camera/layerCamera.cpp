//==========================================================
// [layerCamera.cpp]
//---------------------------------------------------------
// 作成:2023/02/14 
//---------------------------------------------------------
// レイヤーマスクを実装したｶﾒﾗ
//==========================================================

//--- インクルード部
#include <GameSystem/Component/Camera/layerCamera.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CLayerCamera::CLayerCamera()
{
	// 初期値として、全てのビットを1にする
	m_nLayerMask = (1u << 31) - 1;
}

//==========================================================
// 引数付きコンストラクタ
//==========================================================
CLayerCamera::CLayerCamera(std::shared_ptr<CGameObject> owner)
	:CCamera(owner), m_nLayerMask(0)
{
	// 初期値として、全てのビットを1にする
	m_nLayerMask = (1u << 31) - 1;
}

CLayerCamera::~CLayerCamera()
{
}

//==========================================================
// マスク対象か確認
//==========================================================
bool CLayerCamera::IsMask(const int layer)const
{
	return m_nLayerMask & (1 << layer);
}

//==========================================================
// マスク設定
//==========================================================
void CLayerCamera::SetMask(const int mask)
{
	m_nLayerMask = mask;
}

//==========================================================
// マスク設定追加
//==========================================================
void CLayerCamera::AddMask(const int layerNo)
{
	m_nLayerMask |= 1 << layerNo;
}

//==========================================================
// マスク設定解除
//==========================================================
void CLayerCamera::ReleaseMask(const int layerNo)
{
	// ビットを0にする
	// ~ ビット反転
	m_nLayerMask &= ~(1 << layerNo);
}

#ifdef BUILD_MODE

//==========================================================
// 表示
//==========================================================
void CLayerCamera::ImGuiDebug()
{

	// LayerMask設定
	m_nLayerMask = CLayer::ImGuiSetLayerList(m_nLayerMask);

	ImGui::Separator();

	CCamera::ImGuiDebug();
}

#endif // BUILD_MODE
