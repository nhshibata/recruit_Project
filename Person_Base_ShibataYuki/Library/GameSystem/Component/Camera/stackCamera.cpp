//==========================================================
// [stackCamera.cpp]
//---------------------------------------------------------
// 作成：2023/02/14
// 更新：2023/03/19 stackの除外がなかったので追加。無駄な処理があったので修正
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
	m_pGBuf.reset();

}

//==========================================================
// stack追加
//==========================================================
void CStackCamera::Stack(std::weak_ptr<CStackCamera> camera, int idx)
{
	// overlayではない
	if (!camera.lock()->IsStackMode(EStackMode::OVERLAY))
		return;

	// 格納されているか確認
	for (auto & over : m_aStackCamera)
	{
		if (over.lock().get() == camera.lock().get())
			return;
	}

	//--- 追加処理
	// 順番未指定、あるいは指定位置がサイズを超えている
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

			m_aStackCamera[cnt] = m_aStackCamera[cnt - 1];
		}
	}
}

//==========================================================
// マスク対象か確認
//==========================================================
bool CStackCamera::IsMask(const int layer)
{
	// この時点で対象なら終了
	if (CLayerCamera::IsMask(layer))
		return true;

	for (auto it = m_aStackCamera.begin(); it != m_aStackCamera.end(); ++it)
	{
		// 適切ではないが、監視が切れているか確認し、除外
		if (!(*it).lock())
		{
			it = m_aStackCamera.erase(it);
			continue;
		}
		if ((*it).lock()->IsMask(layer))
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

	for (auto it = m_aStackCamera.begin(); it != m_aStackCamera.end(); ++it)
	{
		// 適切ではないが、監視が切れているか確認し、除外
		if (!(*it).lock())
		{
			it = m_aStackCamera.erase(it);
			continue;
		}

		layer |= (*it).lock()->GetMask();
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
	static int nStackSelect = -1;

	//--- モード設定
	Debug::SetTextAndAligned("Camera Type");
	if (auto select = Debug::DispComboSelect(aModeName, "##Camera Type", int(m_eMode)); select != int(m_eMode))
	{
		m_eMode = static_cast<EStackMode>(select);
	}

	// baseのみ
	// FIXME:ネストが深い
	if (m_eMode == EStackMode::BASE)
	{
		Debug::SetTextAndAligned("Stack Order");
		if (ImGui::BeginListBox("##Stack Order"))
		{
			for (int cameraCnt = 0; cameraCnt < m_aStackCamera.size(); ++cameraCnt)
			{
				auto cameraSource = m_aStackCamera[cameraCnt];
				std::string name = cameraSource.lock()->GetOwner()->GetName().c_str();
				
				// 選択表示
				if (ImGui::Selectable(name.c_str(), nStackSelect == cameraCnt))
				{
					nStackSelect = cameraCnt;
				}

				//--- ソース設定
				Debug::DragDropSource<std::weak_ptr<CStackCamera>>(szSelectName, name.c_str(), cameraSource);

				//--- ターゲットアタッチ
				// 設定とアタッチ時処理
				if (auto select = Debug::DragDropTarget<std::weak_ptr<CStackCamera>>(szSelectName); select)
				{
					//--- 入れ替え処理
					int targetIdx = -1;
					int sourceIdx = -1;
					for (int cnt = 0; cnt < m_aStackCamera.size(); ++cnt)
					{
						auto pCamera = m_aStackCamera[cnt].lock().get();
						if (select->lock().get() == pCamera)
						{
							targetIdx = cnt;
						}
						if (cameraSource.lock().get() == pCamera)
						{
							sourceIdx = cnt;
						}
						if (targetIdx != -1 && sourceIdx != -1)
							break;
					}

					auto pWork = m_aStackCamera[targetIdx];
					m_aStackCamera[targetIdx] = m_aStackCamera[sourceIdx];
					m_aStackCamera[sourceIdx] = pWork;
					break;
				}
			}
			ImGui::EndListBox();
		}

		// 除外処理
		Debug::SetTextAndAligned("Remove");
		if (ImGui::Button("-"))
		{
			if(nStackSelect != -1)
				m_aStackCamera.erase(m_aStackCamera.begin() + nStackSelect);
		}

		//--- 追加処理
		auto aCamera = CCamera::GetAllCamera();		
		Debug::SetTextAndAligned("Add Stack");
		if (ImGui::BeginCombo("##Stack", ""))
		{
			for (auto camera : aCamera)
			{
				// CStackCameraのみ
				auto stackCamera = camera->BaseToDerived<CStackCamera>();
				if (!stackCamera)
					continue;
				if (stackCamera->GetStackMode() != EStackMode::OVERLAY)
					continue;

				if (bool bg = ImGui::Selectable(camera->GetOwner()->GetName().c_str(), &bg); bg)
				{
					//--- 選択時追加
					Stack(stackCamera);
					break;
				}
			}
			ImGui::EndCombo();
		}
	
	}

	m_pGBuf->ImGuiDebug();

	ImGui::Separator();

	// 基底クラス呼び出し
	CLayerCamera::ImGuiDebug();

}

#endif // BUILD_MDOE