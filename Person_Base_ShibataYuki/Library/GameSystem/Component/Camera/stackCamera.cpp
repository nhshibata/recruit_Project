//==========================================================
// [stackCamera.cpp]
//---------------------------------------------------------
// �쐬�F2023/02/14
// �X�V�F2023/03/19 stack�̏��O���Ȃ������̂Œǉ��B���ʂȏ������������̂ŏC��
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
	m_pGBuf.reset();

}

//==========================================================
// stack�ǉ�
//==========================================================
void CStackCamera::Stack(std::weak_ptr<CStackCamera> camera, int idx)
{
	// overlay�ł͂Ȃ�
	if (!camera.lock()->IsStackMode(EStackMode::OVERLAY))
		return;

	// �i�[����Ă��邩�m�F
	for (auto & over : m_aStackCamera)
	{
		if (over.lock().get() == camera.lock().get())
			return;
	}

	//--- �ǉ�����
	// ���Ԗ��w��A���邢�͎w��ʒu���T�C�Y�𒴂��Ă���
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

			m_aStackCamera[cnt] = m_aStackCamera[cnt - 1];
		}
	}
}

//==========================================================
// �}�X�N�Ώۂ��m�F
//==========================================================
bool CStackCamera::IsMask(const int layer)
{
	// ���̎��_�őΏۂȂ�I��
	if (CLayerCamera::IsMask(layer))
		return true;

	for (auto it = m_aStackCamera.begin(); it != m_aStackCamera.end(); ++it)
	{
		// �K�؂ł͂Ȃ����A�Ď����؂�Ă��邩�m�F���A���O
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
// layer�Ώ�
//==========================================================
const int CStackCamera::GetAllTargetLayer()
{
	int layer = m_nLayerMask;

	for (auto it = m_aStackCamera.begin(); it != m_aStackCamera.end(); ++it)
	{
		// �K�؂ł͂Ȃ����A�Ď����؂�Ă��邩�m�F���A���O
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

	//--- ���[�h�ݒ�
	Debug::SetTextAndAligned("Camera Type");
	if (auto select = Debug::DispComboSelect(aModeName, "##Camera Type", int(m_eMode)); select != int(m_eMode))
	{
		m_eMode = static_cast<EStackMode>(select);
	}

	// base�̂�
	// FIXME:�l�X�g���[��
	if (m_eMode == EStackMode::BASE)
	{
		Debug::SetTextAndAligned("Stack Order");
		if (ImGui::BeginListBox("##Stack Order"))
		{
			for (int cameraCnt = 0; cameraCnt < m_aStackCamera.size(); ++cameraCnt)
			{
				auto cameraSource = m_aStackCamera[cameraCnt];
				std::string name = cameraSource.lock()->GetOwner()->GetName().c_str();
				
				// �I��\��
				if (ImGui::Selectable(name.c_str(), nStackSelect == cameraCnt))
				{
					nStackSelect = cameraCnt;
				}

				//--- �\�[�X�ݒ�
				Debug::DragDropSource<std::weak_ptr<CStackCamera>>(szSelectName, name.c_str(), cameraSource);

				//--- �^�[�Q�b�g�A�^�b�`
				// �ݒ�ƃA�^�b�`������
				if (auto select = Debug::DragDropTarget<std::weak_ptr<CStackCamera>>(szSelectName); select)
				{
					//--- ����ւ�����
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

		// ���O����
		Debug::SetTextAndAligned("Remove");
		if (ImGui::Button("-"))
		{
			if(nStackSelect != -1)
				m_aStackCamera.erase(m_aStackCamera.begin() + nStackSelect);
		}

		//--- �ǉ�����
		auto aCamera = CCamera::GetAllCamera();		
		Debug::SetTextAndAligned("Add Stack");
		if (ImGui::BeginCombo("##Stack", ""))
		{
			for (auto camera : aCamera)
			{
				// CStackCamera�̂�
				auto stackCamera = camera->BaseToDerived<CStackCamera>();
				if (!stackCamera)
					continue;
				if (stackCamera->GetStackMode() != EStackMode::OVERLAY)
					continue;

				if (bool bg = ImGui::Selectable(camera->GetOwner()->GetName().c_str(), &bg); bg)
				{
					//--- �I�����ǉ�
					Stack(stackCamera);
					break;
				}
			}
			ImGui::EndCombo();
		}
	
	}

	m_pGBuf->ImGuiDebug();

	ImGui::Separator();

	// ���N���X�Ăяo��
	CLayerCamera::ImGuiDebug();

}

#endif // BUILD_MDOE