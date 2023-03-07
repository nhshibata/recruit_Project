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

}

//==========================================================
// �����t���R���X�g���N�^
//==========================================================
CStackCamera::CStackCamera(CGameObject::Ptr owner)
	:CLayerCamera(owner),m_eMode(EStackMode::BASE)
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

#ifdef BUILD_MODE

void CStackCamera::ImGuiDebug()
{
	static const char* szSelectName = "Stack Select";
	const std::vector<std::string> aModeName = {
		"BASE",
		"OVERLAY",
	};

	//--- ���[�h�ݒ�
	if (auto select = Debug::DispComboSelect(aModeName, "Mode", int(m_eMode)); select != int(m_eMode))
	{
		m_eMode = static_cast<EStackMode>(select);
	}

	ImGui::BeginListBox("Stack order");
	for (auto & cameraSource : m_aStackCamera)
	{
		// �I��\��
		ImGui::Selectable(cameraSource.lock()->GetName().c_str());

		//--- �\�[�X�ݒ�
		Debug::DragDropSource<std::weak_ptr<CStackCamera>>(szSelectName, cameraSource.lock()->GetName(), cameraSource);

		//--- �^�[�Q�b�g�A�^�b�`
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

	//--- �ǉ�����
	auto aCamera = CCamera::GetAllCamera();
	std::vector<std::string> aNameVector;
	for (auto camera : aCamera)
	{
		// CStackCamera�̂�
		if(camera->BaseToDerived<CStackCamera>())
			aNameVector.push_back(camera->GetName());
	}
	//--- �R���{�\��
	if (auto addName = Debug::DispComboSelect(aNameVector, "Add Camera", ""); !addName.empty())
	{
		// �I�����ǉ�
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

	// ���N���X�Ăяo��
	CLayerCamera::ImGuiDebug();

}

#endif // BUILD_MDOE