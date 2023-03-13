//==========================================================
// [stackCamera.cpp]
//---------------------------------------------------------
// 作成：2023/02/14
//---------------------------------------------------------
// ｶﾒﾗにｶﾒﾗを設定するためのクラス
// drawからBaseｶﾒﾗから順に描画させる
//==========================================================

//--- インクルード部
#include <GameSystem/Component/Camera/stackCamera.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CStackCamera::CStackCamera()
{
	m_pGBuf = std::make_unique<CGBuffer>();	
}

//==========================================================
// 引数付きコンストラクタ
//==========================================================
CStackCamera::CStackCamera(CGameObject::Ptr owner)
	:CLayerCamera(owner),
	m_eMode(EStackMode::BASE)
{
	m_pGBuf = std::make_unique<CGBuffer>();	
}

//==========================================================
// デストラクタ
//==========================================================
CStackCamera::~CStackCamera()
{

}

//==========================================================
// stack追加
//==========================================================
void CStackCamera::Stack(std::weak_ptr<CStackCamera> camera, int idx)
{
	// overlayではない
	if (!camera.lock()->IsStackMode(EStackMode::OVERLAY))
	{
		return;
	}

	for (auto & over : m_aStackCamera)
	{
		if (over.lock().get() == camera.lock().get())
			return;
	}

	//--- 追加処理
	// 順番未指定、あるいはサイズを超えている
	if (idx == -1 || idx >= m_aStackCamera.size())
	{
		m_aStackCamera.push_back(camera);
		return;
	}

	//--- idxが既に格納済み
	if (auto work = m_aStackCamera[idx]; work.lock())
	{
		// サイズ調整
		m_aStackCamera.resize(m_aStackCamera.size() + 1);
		// 後ろから順に詰める
		for (int cnt = (int)m_aStackCamera.size() - 1; 0 < cnt; --cnt)
		{
			if(cnt == idx)
				m_aStackCamera[cnt] = camera;

			m_aStackCamera[cnt] = m_aStackCamera[cnt-1];
		}
	}
}

//==========================================================
// マスク対象か確認
//==========================================================
bool CStackCamera::IsMask(const int layer)const
{
	// この時点で対象なら終了
	if (CLayerCamera::IsMask(layer))
		return true;

	for (auto & child : m_aStackCamera)
	{
		if (child.lock()->IsMask(layer))
			return true;
	}

	return false;
}

//==========================================================
// layer対象
//==========================================================
const int CStackCamera::GetAllTargetLayer()
{
	int layer = m_nLayerMask;

	for (auto & child : m_aStackCamera)
	{
		layer |= child.lock()->GetMask();
	}
	return layer;
}


#ifdef BUILD_MODE

void CStackCamera::ImGuiDebug()
{
	static const char* szSelectName = "StackSelect";
	const std::vector<std::string> aModeName = {
		"BASE",
		"OVERLAY",
	};

	//--- モード設定
	Debug::SetTextAndAligned("Mode");
	if (auto select = Debug::DispComboSelect(aModeName, "##Mode", int(m_eMode)); select != int(m_eMode))
	{
		m_eMode = static_cast<EStackMode>(select);
	}

	Debug::SetTextAndAligned("Stack Order");
	ImGui::BeginListBox("##Stack Order");
	for (auto & cameraSource : m_aStackCamera)
	{
		std::string name = cameraSource.lock()->GetOwner()->GetName().c_str();
		// 選択表示
		ImGui::Selectable(name.c_str());

		//--- ソース設定
		Debug::DragDropSource<std::weak_ptr<CStackCamera>>(szSelectName, name.c_str(), cameraSource);

		//--- ターゲットアタッチ
		if (auto select = Debug::DragDropTarget<std::weak_ptr<CStackCamera>>(szSelectName); select)
		{
			int targetIt = -1;
			int sourceIt = -1;

			for (int cnt = 0; cnt < m_aStackCamera.size(); ++cnt)
			{
				auto pCamera = m_aStackCamera[cnt].lock().get();
				if (select->lock().get() == pCamera)
				{
					targetIt = cnt;
				}
				if (cameraSource.lock().get() == pCamera)
				{
					sourceIt = cnt;
				}
				if (targetIt != -1 && sourceIt != -1)
					break;
			}

			auto pWork = m_aStackCamera[targetIt];
			m_aStackCamera[targetIt] = m_aStackCamera[sourceIt];
			m_aStackCamera[sourceIt] = pWork;
			break;
		}
	}
	ImGui::EndListBox();

	//--- 追加処理
	auto aCamera = CCamera::GetAllCamera();
	std::vector<std::string> aNameVector;
	for (auto camera : aCamera)
	{
		// CStackCameraのみ
		if(auto stackCamera = camera->BaseToDerived<CStackCamera>(); stackCamera)
			if(stackCamera->GetStackMode() == EStackMode::OVERLAY)
				aNameVector.push_back(camera->GetOwner()->GetName());
	}

	//--- コンボ表示
	Debug::SetTextAndAligned("Stack");
	if (auto addName = Debug::DispComboSelect(aNameVector, "##Stack", ""); !addName.empty())
	{
		// 選択時追加
		for (auto camera : aCamera)
		{
			if (camera->GetOwner()->GetName() == addName)
			{
				Stack(camera->BaseToDerived<CStackCamera>());
				break;
			}
		}
	}

	m_pGBuf->ImGuiDebug();

	ImGui::Separator();

	// 基底クラス呼び出し
	CLayerCamera::ImGuiDebug();

}

#endif // BUILD_MDOE