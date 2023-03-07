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

}

//==========================================================
// 引数付きコンストラクタ
//==========================================================
CStackCamera::CStackCamera(CGameObject::Ptr owner)
	:CLayerCamera(owner),m_eMode(EStackMode::BASE)
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

#ifdef BUILD_MODE

void CStackCamera::ImGuiDebug()
{
	static const char* szSelectName = "Stack Select";
	const std::vector<std::string> aModeName = {
		"BASE",
		"OVERLAY",
	};

	//--- モード設定
	if (auto select = Debug::DispComboSelect(aModeName, "Mode", int(m_eMode)); select != int(m_eMode))
	{
		m_eMode = static_cast<EStackMode>(select);
	}

	ImGui::BeginListBox("Stack order");
	for (auto & cameraSource : m_aStackCamera)
	{
		// 選択表示
		ImGui::Selectable(cameraSource.lock()->GetName().c_str());

		//--- ソース設定
		Debug::DragDropSource<std::weak_ptr<CStackCamera>>(szSelectName, cameraSource.lock()->GetName(), cameraSource);

		//--- ターゲットアタッチ
		if (auto select = Debug::DragDropTarget<std::weak_ptr<CStackCamera>>(szSelectName); select)
		{
			std::vector<std::weak_ptr<CStackCamera>>::iterator targetIt;
			std::vector<std::weak_ptr<CStackCamera>>::iterator sourceIt;

			for (auto it = m_aStackCamera.begin(); it != m_aStackCamera.end() ; ++it)
			{
				if (select->lock() == (*it).lock())
				{
					targetIt = it;
				}
				if (cameraSource.lock() == (*it).lock())
				{
					sourceIt = it;
				}
			}

			std::swap(targetIt, sourceIt);
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
		if(camera->BaseToDerived<CStackCamera>())
			aNameVector.push_back(camera->GetName());
	}
	//--- コンボ表示
	if (auto addName = Debug::DispComboSelect(aNameVector, "Add Camera", ""); !addName.empty())
	{
		// 選択時追加
		for (auto camera : aCamera)
		{
			if (camera->GetName() == addName)
			{
				Stack(camera->BaseToDerived<CStackCamera>());
				break;
			}
		}
	}

	m_pGBuf->ImGuiDebug();

	// 基底クラス呼び出し
	CLayerCamera::ImGuiDebug();

}

#endif // BUILD_MDOE