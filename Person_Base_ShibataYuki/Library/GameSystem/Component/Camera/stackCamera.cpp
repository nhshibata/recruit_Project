//==========================================================
// [stackCamera.cpp]
//---------------------------------------------------------
// �쐬�F2023/02/14
//---------------------------------------------------------
// ��ׂɶ�ׂ�ݒ肷�邽�߂̃N���X
// draw����Base��ׂ��珇�ɕ`�悳����
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/stackCamera.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;
using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CStackCamera::CStackCamera()
{
	m_pGBuf = std::make_unique<CGBuffer>();	
}

//==========================================================
// �����t���R���X�g���N�^
//==========================================================
CStackCamera::CStackCamera(CGameObject::Ptr owner)
	:CLayerCamera(owner),
	m_eMode(EStackMode::BASE)
{
	m_pGBuf = std::make_unique<CGBuffer>();	
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CStackCamera::~CStackCamera()
{

}

//==========================================================
// stack�ǉ�
//==========================================================
void CStackCamera::Stack(std::weak_ptr<CStackCamera> camera, int idx)
{
	// overlay�ł͂Ȃ�
	if (!camera.lock()->IsStackMode(EStackMode::OVERLAY))
	{
		return;
	}

	for (auto & over : m_aStackCamera)
	{
		if (over.lock().get() == camera.lock().get())
			return;
	}

	//--- �ǉ�����
	// ���Ԗ��w��A���邢�̓T�C�Y�𒴂��Ă���
	if (idx == -1 || idx >= m_aStackCamera.size())
	{
		m_aStackCamera.push_back(camera);
		return;
	}

	//--- idx�����Ɋi�[�ς�
	if (auto work = m_aStackCamera[idx]; work.lock())
	{
		// �T�C�Y����
		m_aStackCamera.resize(m_aStackCamera.size() + 1);
		// ��납�珇�ɋl�߂�
		for (int cnt = (int)m_aStackCamera.size() - 1; 0 < cnt; --cnt)
		{
			if(cnt == idx)
				m_aStackCamera[cnt] = camera;

			m_aStackCamera[cnt] = m_aStackCamera[cnt-1];
		}
	}
}

//==========================================================
// �}�X�N�Ώۂ��m�F
//==========================================================
bool CStackCamera::IsMask(const int layer)const
{
	// ���̎��_�őΏۂȂ�I��
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
// layer�Ώ�
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

	//--- ���[�h�ݒ�
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
		// �I��\��
		ImGui::Selectable(name.c_str());

		//--- �\�[�X�ݒ�
		Debug::DragDropSource<std::weak_ptr<CStackCamera>>(szSelectName, name.c_str(), cameraSource);

		//--- �^�[�Q�b�g�A�^�b�`
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

	//--- �ǉ�����
	auto aCamera = CCamera::GetAllCamera();
	std::vector<std::string> aNameVector;
	for (auto camera : aCamera)
	{
		// CStackCamera�̂�
		if(auto stackCamera = camera->BaseToDerived<CStackCamera>(); stackCamera)
			if(stackCamera->GetStackMode() == EStackMode::OVERLAY)
				aNameVector.push_back(camera->GetOwner()->GetName());
	}

	//--- �R���{�\��
	Debug::SetTextAndAligned("Stack");
	if (auto addName = Debug::DispComboSelect(aNameVector, "##Stack", ""); !addName.empty())
	{
		// �I�����ǉ�
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

	// ���N���X�Ăяo��
	CLayerCamera::ImGuiDebug();

}

#endif // BUILD_MDOE