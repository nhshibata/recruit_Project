//=========================================================
// [hirarchy.cpp] 
// �쐬:2022/07/10
// �X�V:2022/11/12 Drag&Drop�Őe�q�֌W
//---------------------------------------------------------
// GameObject��\���A���삷��
//=========================================================

//--- �C���N���[�h��
#include <DebugSystem/hierarchy.h>
#if BUILD_MODE

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <DebugSystem/imguiManager.h>
#include <DebugSystem/inspector.h>
#include <DebugSystem/imGuiPackage.h>
#include <DebugSystem/imGuiContextMenu.h>

#include <GameSystem/Scene/scene.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Camera/debugCamera.h>
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>
#include <GameSystem/Component/Renderer/boxRenderer.h>
#include <GameSystem/Component/Renderer/effekseerRenderer.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Manager/drawSystem.h>

#include <CoreSystem/File/filePath.h>
#include <CoreSystem/Input/input.h>

#include <Application/Application.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

using namespace MySpace::System;
using namespace MySpace::Debug;
using namespace MySpace::SceneManager;
using namespace MySpace::Game;

//--- �񋓑̐錾
// ���̃\�[�X���ł����g���Ȃ��A�}�W�b�N�i���o�[�����p
enum class ECreateObjType
{
	EMPTY,
	MODEL,
	STATIC_MODEL,
	BILLBOARD,
	SPHERE,
	BOX,
	POLYGON,
	TEXT,
	MAX
};

//==========================================================
// �R���X�g���N�^
//==========================================================
CHierachy::CHierachy()
	:m_bLoadSaveWindow(false)
{
	m_strSavePath.clear();
	m_strLoadPath = FORDER_DIR(Data/scene);
	LoadScenePathList();
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CHierachy::~CHierachy()
{
}

//==========================================================
// �X�V
//==========================================================
void CHierachy::Update(ImGuiManager* manager)
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	auto screenSize = CScreen::GetSize();
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(screenSize.x * 0.2f, screenSize.y * 0.6f), ImGuiCond_Once);
	bool flg = true;
	ImGui::Begin("Hierarchy", &flg, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

	// �V�[�������݂��Ă��Ȃ���Ώ������Ȃ�
	if (!CSceneManager::Get()->GetActiveScene())
		return;

	manager->HoverStateSet();

	//--- ���j���[�o�[
	if (ImGui::BeginMenuBar())
	{
		//--- �V�[��
		if (ImGui::BeginMenu("Scene/File"))
		{
			if (ImGui::MenuItem("New empty Scene"))
			{
				CSceneManager::Get()->CreateNewScene<CScene>();
				//auto newScene = CSceneManager::Get()->CreateNewScene<CScene>();
				//CSceneManager::Get()->SetActiveScene(CSceneManager::Get()->GetSceneByName(newScene->GetSceneName()));
			}
			if (ImGui::MenuItem("New Scene"))
			{
				auto newScene = CSceneManager::Get()->CreateNewScene<CScene>();
				newScene->CreateEmptyScene();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Load/Save"))
			{
				m_bLoadSaveWindow = !m_bLoadSaveWindow;
			}
			ImGui::EndMenu();
		}

		//--- �I�u�W�F�N�g����
		if (ImGui::BeginMenu("New GameObject"))
		{
			if (ImGui::MenuItem("Empty"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::EMPTY));
			if (ImGui::MenuItem("Model"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::MODEL));
			if (ImGui::MenuItem("StaticModel"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::STATIC_MODEL));
			if (ImGui::MenuItem("Billboard"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::BILLBOARD));
			if (ImGui::MenuItem("Sphere"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::SPHERE));
			if (ImGui::MenuItem("Box"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::BOX));
			if (ImGui::MenuItem("Polygon"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::POLYGON));
			if (ImGui::MenuItem("Text"))
				manager->GetInspector()->SetSelectGameObject(CreateObject((int)ECreateObjType::TEXT));
			if (ImGui::MenuItem("Objects "))
			{
				m_CreateParam.bDisp ^= true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// �����X�V
	DispSearch();

	// �I�u�W�F�N�g�E�B���h�E
	CreateObjectsWindow();

	//--- GameObject�\��
	{
		auto objList = CSceneManager::Get()->GetActiveScene()->GetObjManager()->GetList();
		int objCnt = 0;
		// �Q�[���I�u�W�F�N�g���̕\���Ǝq�v�f�̕\��
		for (const auto & object : objList)
		{
			// ������ON�ɂȂ��Ă��鎞
			if (m_Search.bSearchCriteria)
			{	// ���������ƈ�v���邩���肵�A��v���Ȃ��Ȃ�Ύ���
				if (!DispCheck(object.get()))
					continue;
			}

			// �e�v�f�̊m�F�A�e���q��\������̂Ŏ���
			if (object->GetTransform()->GetParent().lock())
				continue;

			// ���O������̏ꍇ�AImGui�ɔF������Ȃ��̂ŁAID��^����
			auto name = std::to_string(objCnt) + ":" + object->GetName();


			// �I���{�^���A�E�B���h�E�\��
			if (ImGui::Button(name.c_str()))
			{
				manager->GetInspector()->SetSelectGameObject(object);
				break;
			}
			Debug::PopUpGameObjectMenu(object.get());

			// �h���b�O�ݒ�
			//if (ImGuiManager::Get()->GetInspector()->GetSelectObject().lock() == object)
			{
				DragDropSource<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ, name.c_str(), object);
			}

			// �h���b�O&�h���b�v���ꂽ�Ƃ�
			if (auto select = DragDropTarget<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ); select)
				object->GetTransform()->AddChild(select->lock()->GetComponent<CTransform>());

			//--- �q�v�f�̕\��
			DispChild(manager, object);

			++objCnt;
		}
	}
	ImGui::End();

	//--- �Z�[�u���[�h
	if (m_bLoadSaveWindow)
		DispSaveLoadMenu();

	//--- �N���b�N
	if(manager->GetHover() == ImGuiManager::EMouseHovered::HOVERED_NONE)
	{
		if (CInput::GetMouseTrigger(MOUSEBUTTON_L))
		{
			ClickSelect(manager);
		}
	}


	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

//==========================================================
// �V�[���t�@�C��
//==========================================================
void CHierachy::LoadScenePathList()
{
	CFilePath path;
	m_aScenePathList = path.GetFileName(m_strLoadPath);
}

//==========================================================
// Scene�ۑ��ǂݍ��݃��j���[�\��
//==========================================================
void CHierachy::DispSaveLoadMenu()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

	if (ImGui::Begin("Load", &m_bLoadSaveWindow, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("SceneList"))
			{
				for (auto it = m_aScenePathList.begin(); it < m_aScenePathList.end(); ++it)
				{
					auto name = (*it).c_str();
					if (ImGui::MenuItem(name))
					{
						m_strLoadPath = name;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::Button("Scene Path Reload"))
		{
			LoadScenePathList();
		}

		m_strLoadPath = InputString(m_strLoadPath, "LoadFile");

		if (ImGui::Button("Load"))
		{
			if (CSceneManager::Get()->LoadScene(m_strLoadPath))
				m_bLoadSaveWindow = false;
		}
		ImGui::Separator();


		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);

		m_strSavePath = InputString(m_strSavePath, "SaveFile");
		if (ImGui::Button("Save"))
		{
			CSceneManager::Get()->SaveScene(m_strSavePath);
		}
		ImGui::Separator();
		if (ImGui::Button("LoadScene OverWrite"))
		{
			CSceneManager::Get()->SaveScene(m_strLoadPath);
		}
		if (ImGui::Button("SaveScene OverWrite"))
		{
			CSceneManager::Get()->SaveScene(m_strSavePath);
		}

		ImGui::End();
	}
}

#pragma region GAME_OBJECT

//==========================================================
// �q�I�u�W�F�N�g�\��
//==========================================================
void CHierachy::DispChild(ImGuiManager* manager, std::weak_ptr<MySpace::Game::CGameObject> object)
{
	bool select = false;

	ImGui::SameLine();

	// �ʒu�ݒ�
	SetControlPosX(20);
	ImGui::Text("- child -");
	ImGui::SameLine();
	SetControlPosX(60);

	// �q�̕\��
	if (ImGui::TreeNode(std::string(object.lock()->GetName() + "##- child -").c_str()))
	{
		// �J�������A�Ȃ�
		if (object.lock()->GetTransform()->GetChildCount() == 0)
		{
			ImGui::Text("none");
			ImGui::TreePop();
			return;
		}

		for (auto cnt = 0; cnt < object.lock()->GetTransform()->GetChildCount(); ++cnt)
		{
			// nullptr�m�F
			if (auto child = object.lock()->GetTransform()->GetChild(cnt).lock()->GetOwner(0); child.lock())
			{
				// NOTE:???�悭�킩��Ȃ����A��������ƂȂ����\�����Y��ɂȂ�
				std::string ownname = child.lock()->GetName();
				const char* childName = ownname.c_str();

				// inspector�I��
				if (select = ImGui::Button(childName); select)
				{
					manager->GetInspector()->SetSelectGameObject(child);
				}
				// �E�N���b�N�\��
				Debug::PopUpGameObjectMenu(child.lock().get());

				// �h���b�O�ݒ�
				//if (ImGuiManager::Get()->GetInspector()->GetSelectObject().lock() == object.lock())
				{
					DragDropSource<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ, child.lock()->GetName(), child);
				}

				// �h���b�O&�h���b�v���ꂽ�Ƃ�
				if (auto selectObj = DragDropTarget<CGameObject::PtrWeak>(CHierachy::DESC_SELECT_OBJ); selectObj)
					child.lock()->GetTransform()->AddChild(selectObj->lock()->GetComponent<CTransform>());

				//--- �q�v�f���X�ɕ\��(�ċA)
				DispChild(manager, child.lock());

				// �I������Ă����甲����
				if (select)
					break;
			}
		}

		ImGui::TreePop();
	}
}
#pragma endregion

#pragma region SEARCH

//==========================================================
// �\���I�u�W�F�N�g�ؑ�
//==========================================================
void CHierachy::DispSearch()
{
	// for���ϐ�
	static const char* szDisp[static_cast<int>(ESearchTerms::MAX)] = {
		"Name",
		"Tag",
		"Component",
		"sActive",
		"sStop"
	};

	if (ImGui::Button(m_Search.bSearchCriteria ? "Search:ON" : "Search:OFF"))
	{
		m_Search.bSearchCriteria ^= true;
	}
	ImGui::SameLine();

	//--- ����
	m_Search.inputName = InputString(m_Search.inputName, "##Search");

	// �����I��
	ImGui::Text("Condition");
	for (int cnt = 0; cnt < static_cast<int>(ESearchTerms::MAX); ++cnt)
	{
		ImGui::SameLine();
		ImGui::RadioButton(szDisp[cnt], (int*)&m_Search.eTerms, cnt);
	}
	ImGui::Separator();
}

//==========================================================
// �\������Ώۊm�F
//==========================================================
bool CHierachy::DispCheck(CGameObject* obj)
{
	switch (m_Search.eTerms)
	{
		case MySpace::Debug::CHierachy::ESearchTerms::OBJ_NAME:		// ������ꕔ��v�ł���
			if (obj->GetName().find(m_Search.inputName) != std::string::npos)
				return true;
			break;
		case MySpace::Debug::CHierachy::ESearchTerms::TAG:			// tag��r
			return obj->GetTagPtr()->Compare(m_Search.inputName);

		case MySpace::Debug::CHierachy::ESearchTerms::COMPONENT:
		{
			auto comList = obj->GetComponentList();					// ������ꕔ��v�ł���
			for (const auto & com : comList)
			{
				if (com->GetName().find(m_Search.inputName) != std::string::npos)
					return true;
			}
			break;
		}
		case MySpace::Debug::CHierachy::ESearchTerms::STATE_ACTIVE:
			return obj->GetState() == CGameObject::E_ObjectState::ACTIVE;

		case MySpace::Debug::CHierachy::ESearchTerms::STATE_STOP:
			return obj->GetState() == CGameObject::E_ObjectState::STOP;
		
		default:
			break;
	}
	return false;
}
#pragma endregion

//==========================================================
// �I�u�W�F�N�g�ꊇ����
//==========================================================
void CHierachy::CreateObjectsWindow()
{
	static std::vector<std::string> aSelectType = {
		"Empty",
		"Model",
		"StaticModel",
		"Billboard",
		"Sphere",
		"Box",
		"Polygon",
		"Text",
	};
	if (!m_CreateParam.bDisp)
		return;
	ImGui::SetNextWindowSize(ImVec2(CScreen::GetWidth()/3, CScreen::GetHeight()/3), ImGuiCond_::ImGuiCond_Once);
	ImGui::Begin("Objects Create", &m_CreateParam.bDisp);

	// type�I��
	m_CreateParam.nObjType = Debug::DispComboSelect(aSelectType, "Type", m_CreateParam.nObjType);

	Debug::SetTextAndAligned("ObjectName");
	m_CreateParam.strName = Debug::InputString(m_CreateParam.strName, m_CreateParam.strName);
	
	Debug::SetTextAndAligned("Duplicate?");
	ImGui::Checkbox("##Duplicate", &m_CreateParam.bCopy);

	Debug::SetTextAndAligned(u8"Grid (? �~ ?)");
	ImGui::DragInt("##Grid", &m_CreateParam.nGrid);

	Debug::SetTextAndAligned("Margin");
	ImGui::DragFloat("##Margin", &m_CreateParam.fMargin);

	Debug::SetTextAndAligned("Center");
	ImGui::DragFloat3("##Center", (float*)&m_CreateParam.vCenter);

	// ���s
	Debug::SetTextAndAligned("Create");
	if (ImGui::Button("Generate"))
	{
		for (int gridCnt = 0; gridCnt < m_CreateParam.nGrid * m_CreateParam.nGrid; ++gridCnt)
		{
			CGameObject::Ptr pObj;
			//--- �I�u�W�F�N�g��߰��
			if (m_CreateParam.bCopy)
			{
				pObj = Debug::CopyGameObject();
				pObj->SetName(pObj->GetName() + std::to_string(gridCnt));
			}
			else
				pObj = CreateObject(m_CreateParam.nObjType);

			//--- ���W�ݒ�
			float col = float(gridCnt % m_CreateParam.nGrid);
			float row = float(gridCnt / m_CreateParam.nGrid);
			auto newPos = m_CreateParam.vCenter;


			float offsetX = (col - (m_CreateParam.nGrid - 1) * 0.5f) * m_CreateParam.fMargin;
			float offsetZ = (row - (m_CreateParam.nGrid - 1) * 0.5f) * m_CreateParam.fMargin;

			newPos.x += offsetX;
			newPos.z += offsetZ;
			pObj->GetTransform()->SetPos(newPos);

			if (!m_CreateParam.strName.empty())
				pObj->SetName(m_CreateParam.strName + "_" + std::to_string(gridCnt));

		}
	}

	ImGui::End();
}

//==========================================================
// �I�u�W�F�N�g����
//==========================================================
CGameObject::Ptr CHierachy::CreateObject(const int No, std::shared_ptr<MySpace::Game::CGameObject> copy)
{
	CGameObject::Ptr obj;

	//--- ��߰���邩�m�F
	if (copy)
		obj = CGameObject::CopyObject(copy).lock();
	else
		obj = CGameObject::CreateObject().lock();

	//--- �񋓑̂ɕϊ�
	ECreateObjType eType = (ECreateObjType)No;

	switch (eType)
	{
		case ECreateObjType::EMPTY:
			break;
		case ECreateObjType::MODEL:
			obj->AddComponent<CModelRenderer>();
			break;
		case ECreateObjType::STATIC_MODEL:
		{
			auto render = obj->AddComponent<CModelRenderer>();
			render->SetStatic(CMeshRenderer::EStaticMode::STATIC);
			break;
		}
		case ECreateObjType::BILLBOARD:
			obj->AddComponent<Game::CBillboardRenderer>();
			break;
		case ECreateObjType::SPHERE:
			obj->AddComponent<Game::CSphereRenderer>();
			break;
		case ECreateObjType::BOX:
			obj->AddComponent<Game::CBoxRenderer>();
			break;
		case ECreateObjType::POLYGON:
			obj->AddComponent<Game::CPolygonRenderer>();
			break;
		case ECreateObjType::TEXT:
			obj->AddComponent<Game::CTextRenderer>();
			break;
		case ECreateObjType::MAX:
			break;
		default:
			break;
	}

	return obj;
}

// *@�N���b�N�ŃI�u�W�F�N�g��I��
void CHierachy::ClickSelect(MySpace::Debug::ImGuiManager* manager)
{
	// �}�E�X���W
	CCamera* pCam = CCamera::GetMain();
	XMFLOAT3 E = pCam->Transform()->GetPos();
	const auto& mousePos = Input::Mouse::GetPosition();

#if 1
	// �}�E�X�J�[�\���̈ʒu�����[���h���W�ɕϊ�
	const auto& g_vp = Application::Get()->GetSystem<CDXDevice>()->GetViewPort();
	XMFLOAT3 mouseP;
	XMStoreFloat3(&mouseP, XMVector3Unproject(XMVectorSet(mousePos.x, mousePos.y, 0.0f, 1.0f),
										 g_vp->TopLeftX, g_vp->TopLeftY, g_vp->Width, g_vp->Height, g_vp->MinDepth, g_vp->MaxDepth,
										 XMLoadFloat4x4(&pCam->GetProjMatrix()), XMLoadFloat4x4(&pCam->GetViewMatrix()),
										 XMMatrixIdentity()));

	// �I�u�W�F�N�g���X�g���擾
	const auto& list = CSceneManager::Get()->GetDrawSystem()->GetList();

	// ����͈͂𒲐����邽�߂̃X�P�[���W��
	const float scale = 100.0f * 0.5f;

	// �I�u�W�F�N�g���X�g�𑖍����āA�}�E�X�J�[�\���̈ʒu�ɂ���I�u�W�F�N�g����������
	int selectedIndex = -1;
	float minDistance = std::numeric_limits<float>::max();
	for (int i = 0; i < list.size(); ++i)
	{
		const auto& render = list[i].lock()->BaseToDerived<CMeshRenderer>();
		if (!render) 
			continue;

		// �I�u�W�F�N�g�̒��S���W���擾
		XMFLOAT3 centerPos = render->Transform()->GetPos();

		// �I�u�W�F�N�g�̊g�嗦���擾
		XMFLOAT3 scaleVec = render->Transform()->GetScale();

		// ���蔼�a���v�Z
		float radius = std::max(std::max(scaleVec.x, scaleVec.y), scaleVec.z);
		radius = render->GetBSRadius() * scale;

		// �}�E�X�J�[�\���ƃI�u�W�F�N�g���S�_�Ƃ̋������v�Z
		float distance = sqrtf(powf(centerPos.x - mouseP.x, 2) + powf(centerPos.y - mouseP.y, 2) + powf(centerPos.z - mouseP.z, 2));

		// ���s���̒l���v�Z
		XMVECTOR depthVec = XMVector3Length(XMLoadFloat3(&centerPos) - XMLoadFloat3(&E));
		float depth = XMVectorGetX(depthVec);

		// ���蔼�a�ȉ����A�}�E�X�J�[�\���ƃI�u�W�F�N�g���S�_�Ƃ̋������ŏ��l�����������ꍇ�A���̃I�u�W�F�N�g��I������
		if (distance <= radius && depth < minDistance)
		{
			if (render->GetOwner()->GetTag() == "MainCamera")
				continue;

			// �y�`�F�b�N�֌W���[�N�X�V
			selectedIndex = i;		// �I�𒆂̃I�u�W�F�N�g�C���f�b�N�X��ۑ�
			minDistance = depth;    // �I�𒆂̃I�u�W�F�N�g�̉��s����ۑ�
		}
	}

	// �q�b�g���Ă����
	if (selectedIndex != -1)
	{
		auto bj = list[selectedIndex].lock()->GetOwner(0);
		manager->GetInspector()->SetSelectGameObject(bj);
	}

#elif 3

	//const POINT& mousePos = *CInput::GetMousePosition();
	const auto& mousePos = Input::Mouse::GetPosition();
	const auto& g_vp = Application::Get()->GetSystem<CDXDevice>()->GetViewPort();
	const auto& list = CSceneManager::Get()->GetDrawSystem()->GetList();
	int Zidx = -1;
	float HitZDepth = 9999.0f;

	for (int cnt = 0; cnt < list.size(); cnt++)
	{
		const auto& render = list[cnt].lock();

		const XMFLOAT4X4& worldMatrix = render->Transform()->GetWorldMatrix();

		// �I�u�W�F�N�g�̒��S���W���擾
		XMVECTOR centerPos = XMLoadFloat3(&render->Transform()->GetPos());

		// �I�u�W�F�N�g�̊g�嗦���擾
		XMVECTOR scaleVec = XMLoadFloat3(&render->Transform()->GetScale());

		// �����蔻��̊�_���I�u�W�F�N�g���S���W����I�u�W�F�N�g�̊g�嗦�ɉ����Ĉړ�������
		centerPos = XMVector3Transform(centerPos, XMLoadFloat4x4(&worldMatrix));

		// �}�E�X�J�[�\���̈ʒu�����[���h���W�ɕϊ�
		XMFLOAT3 mouseP;
		XMStoreFloat3(&mouseP, XMVector3Unproject(XMVectorSet(mousePos.x, mousePos.y, 0.0f, 1.0f),
											 g_vp->TopLeftX, g_vp->TopLeftY, g_vp->Width, g_vp->Height, g_vp->MinDepth, g_vp->MaxDepth,
											 XMLoadFloat4x4(&pCam->GetProjMatrix()), XMLoadFloat4x4(&pCam->GetViewMatrix()),
											 XMMatrixIdentity()));

		// �����蔻��̈�̔��a���v�Z
		float radius = XMVectorGetX(XMVector3Length(scaleVec));

		// �����蔻��
		XMFLOAT3 center;
		XMStoreFloat3(&center, centerPos);
		float distance = sqrtf(powf(center.x - mouseP.x, 2) + powf(center.y - mouseP.y, 2) + powf(center.z - mouseP.z, 2));

		// ���s���̒l���v�Z
		XMVECTOR depthVec = XMVector3Length(centerPos - XMLoadFloat3(&E));
		float depth = XMVectorGetX(depthVec);

		if (distance <= radius && depth < HitZDepth)
		{
			if (render->GetOwner()->GetTag() == "MainCamera")
				continue;

			// ������
			// �y�`�F�b�N�֌W���[�N�X�V
			Zidx = cnt;				// �I�𒆂̃I�u�W�F�N�g�C���f�b�N�X��ۑ�
			HitZDepth = depth;    // �I�𒆂̃I�u�W�F�N�g�̉��s����ۑ�
		}
	}

	// �q�b�g���Ă����
	if (Zidx != -1)
	{
		auto bj = list[Zidx].lock()->GetOwner(0);
		manager->GetInspector()->SetSelectGameObject(bj);
	}
#endif // 0

}

#endif // !BUILD_MODE
