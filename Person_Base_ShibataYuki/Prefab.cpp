#include "Prefab.h"
#include "../../FrameWork/Component/Transform/NodeTransform.h"
#include "../../Main/manager.h"
#include "../Common.h"

#include <cereal/archives/binary.hpp>
#include <fstream>
#include <direct.h>

static const char FileName[] = "Prefab";
static const char PopupPrefab[] = "PopupPrefab";

using namespace Input;

void Prefab::Load(std::string path)
{
	deletePathList.clear();
	prefabComponentList.clear();
	prefabGameObjectList.clear();
	prefabList.clear();

	// �f�[�^�p�̃t�H���_�쐬
	std::string filePathName = path + std::string(FileName);
	_mkdir(filePathName.c_str());

	std::vector<std::string> fileArray;
	// �f�[�^�̖��O�z���ǂݍ���
	fileArray = FileLoader::GetFileListInFolder(path + FileName, "prefab");

	if (fileArray.size() == 0)
		return;

	// ���O�z�񂩂�f�[�^��ǂݍ���
	for (auto & name : fileArray)
	{
		std::ifstream ifs(filePathName + "/" + name, std::ios::binary);
		if (ifs)
		{
			std::shared_ptr<GameObject> inData;
			cereal::BinaryInputArchive i_archive(ifs);
			i_archive(inData);
			RegisterObject(inData);
		}
		ifs.close();
	}
}

void Prefab::Save(std::string path)
{
	std::string filePathName = path + std::string(FileName);

	// �폜���ꂽ�f�[�^�̃t�@�C��������
	for (std::string delPath : deletePathList)
	{
		std::string name = delPath + ".prefab";
		DeleteFile((filePathName + "/" + name).c_str());
	}
	deletePathList.clear();

	// �f�[�^������ۑ�
	for (auto & pre : prefabList)
	{
		std::string name = pre->name + ".prefab";

		std::string fileName = path + FileName + "/" + name;
		std::ofstream ofs(fileName, std::ios::binary);
		{
			cereal::BinaryOutputArchive o_archive(ofs);
			o_archive(cereal::make_nvp(pre->name, pre));
		}
		ofs.close();
	}
}

void Prefab::RegisterObject(std::weak_ptr<GameObject> gameObject)
{
	auto transform = gameObject.lock()->transform.lock();
	auto parent = transform->GetParent();

	Translation3f translation = transform->translation;
	Scaling3f scaling = transform->scaling;
	Quaternion rotate = transform->rotate;
	// ��U�e��ޔ�
	if (!parent.expired())
	{
		transform->parent.reset();
		/*Matrix4 localMat = transform->localMatrix;
		Vector3 pos = localMat.position();
		Vector3 sca = localMat.scale();
		transform->translation = Translation3f(pos.x, pos.y, pos.z);
		transform->scaling = Scaling3f(sca.x, sca.y, sca.z);
		transform->rotate = localMat.rotation();*/
	}

	// ��U�V���A����
	std::stringstream ss;
	{
		cereal::BinaryOutputArchive o_archive(ss);
		o_archive(gameObject.lock());
	}

	// �R�s�[��ޔ������e���ēx�Z�b�g
	if (!parent.expired())
	{
		transform->parent = parent;

		/*transform->translation = translation;
		transform->scaling = scaling;
		transform->rotate = rotate;*/
	}

	// �V���A�����������̂�V�����C���X�^���X����
	std::shared_ptr<GameObject> object;
	{
		cereal::BinaryInputArchive i_archive(ss);
		i_archive(object);

		// �v���n�u�ɓo�^
		prefabList.emplace_back(object);

		// �q�I�u�W�F�N�g���܂ߑS�Ă��C���X�^���X���X�g�ɓo�^
		this->RegisterGameObject(object);
	}
}

void Prefab::RegisterInsertObject(std::weak_ptr<GameObject> gameObject, std::list<std::shared_ptr<GameObject>>::iterator & outItr)
{
	auto transform = gameObject.lock()->transform.lock();
	auto parent = transform->GetParent();

	Translation3f translation = transform->translation;
	Scaling3f scaling = transform->scaling;
	Quaternion rotate = transform->rotate;
	// ��U�e��ޔ�
	if (!parent.expired())
	{
		transform->parent.reset();
		//Matrix4 localMat = transform->worldMatrix;
		//Vector3 pos = localMat.position();
		//Vector3 sca = localMat.scale();
		//transform->translation = Translation3f(pos.x, pos.y, pos.z);
		//transform->scaling = Scaling3f(sca.x, sca.y, sca.z);
		//transform->rotate = localMat.rotation();
	}

	// ��U�V���A����
	std::stringstream ss;
	{
		cereal::BinaryOutputArchive o_archive(ss);
		o_archive(gameObject.lock());
	}

	// �R�s�[��ޔ������e���ēx�Z�b�g
	if (!parent.expired())
	{
		transform->parent = parent;

		//transform->translation = translation;
		//transform->scaling = scaling;
		//transform->rotate = rotate;
	}

	// �V���A�����������̂�V�����C���X�^���X����
	std::shared_ptr<GameObject> object;
	{
		cereal::BinaryInputArchive i_archive(ss);
		i_archive(object);

		// �w��̈ʒu�Ƀv���n�u�ɓo�^
		prefabList.insert(outItr, object);

		// �q�I�u�W�F�N�g���܂ߑS�Ă��C���X�^���X���X�g�ɓo�^
		this->RegisterGameObject(object);
	}
}

void Prefab::DeleteObject(std::shared_ptr<GameObject> gameObject)
{
	deletePathList.emplace_back(gameObject->name);

	// �v���n�u���X�g����폜
	for (auto itr = prefabList.begin(), end = prefabList.end(); itr != end; ++itr)
	{
		if ((*itr) != gameObject) continue;
		prefabList.erase(itr);
		break;
	}

	// �C���X�^���X���X�g����폜
	DeleteGameObject(gameObject);
}

void Prefab::DrawImGui()
{
	const std::string strId = "##Prefab";
	const char popupName1[] = "Popup##PrefabEdit";
	static std::weak_ptr<GameObject> deleteObject;

	ImVec2 winSize = ImGui::GetWindowSize();
	winSize.x -= ImGui::GetCursorPosX();
	winSize.y -= ImGui::GetCursorPosY();
	ImGui::BeginChild("PrefabChild", winSize);

	for (auto prefab : prefabList)
	{
		std::string & name = prefab->name;
		ImGui::Bullet(); ImGui::SameLine();

		bool isSelect = prefab == selectObject.lock();
		if (ImGui::Selectable((name + strId).c_str(), isSelect))
		{
			if (isSelect)
				selectObject.reset();
			else
				selectObject = prefab;
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload(GAMEOBJECT, &prefab, sizeof(prefab));

			ImGui::Text(name.c_str());

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered())
		{
			if (Mouse::IsTrigger(Mouse::Button::Right))
			{
				deleteObject = prefab;
				ImGui::OpenPopup(PopupPrefab);
			}
		}
	}

	if (ImGui::BeginPopup(PopupPrefab))
	{
		if (!deleteObject.expired())
		{
			if (ImGui::MenuItem("Delete"))
			{
				DeleteObject(deleteObject.lock());
				deleteObject.reset();
			}
		}

		ImGui::EndPopup();
	}

	ImGui::EndChild();

	// �v���n�u�o�^
	static std::weak_ptr<GameObject> overwriteObject;
	static std::list<std::shared_ptr<GameObject>>::iterator outItr = prefabList.end();
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(NodeTransform::NODE_TRANSFORM_LIST))
		{
			auto & payload_n = *(std::list<std::weak_ptr<NodeTransform>>*)payload->Data;
			overwriteObject = payload_n.front().lock()->transform.lock()->gameObject;
			// �������O�̃I�u�W�F�N�g�����݂��Ă��邩�m�F
			if (CheckName(overwriteObject, overwriteObject.lock()->name, outItr))
			{
				RegisterObject(overwriteObject);
				overwriteObject.reset();
			}
			else
			{
				ImGui::OpenPopup("Override?");
			}
			
		}
	}

	// �����v���n�u�̖��O����������㏑�����邩�̊m�F
	if (outItr != prefabList.end() && ImGui::BeginPopupModal("Override?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text((*outItr)->name.c_str());
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// �㏑������I�u�W�F�N�g�̈ʒu�ɑ}��
			RegisterInsertObject(overwriteObject, outItr);
			// �㏑���O�̃I�u�W�F�N�g���폜
			DeleteObject(*outItr);

			overwriteObject.reset();
			outItr = prefabList.end();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// �ҏW�E�B���h�E
	bool isOpen = !selectObject.expired();
	if (isOpen)
	{
		if (ImGui::Begin("PrefabEdit", &isOpen))
		{
			std::string beforeName = selectObject.lock()->name;
			selectObject.lock()->DrawImGui();
			if (beforeName != selectObject.lock()->name)
			{
				SetOriginalName(selectObject);	// �I���W�i���̖��O���m�F����

				// �t�@�C���ɂ��ύX���𔽉f
				std::string filePathName = DATA_FOLDER_PATH + std::string(FileName);
				std::string oldName = filePathName + +"/" + beforeName + ".prefab";
				std::string finalName = filePathName + "/" + selectObject.lock()->name + ".prefab";
				rename(oldName.c_str(), finalName.c_str());
			}

			int cnt = 0;
			std::string componentLabel = "##PrefabComponent";
			std::string strId = componentLabel + std::to_string(cnt);
			for (auto & com : selectObject.lock()->components)
			{
				ImGui::Checkbox(("##" + com.lock()->ClassName() + strId).c_str(), &com.lock()->Enable());

				ImGui::SameLine();
				bool isOnMouse = false;
				if (ImGui::TreeNode((com.lock()->ClassName() + strId).c_str()))
				{
					isOnMouse = ImGui::IsItemHovered();

					if ((com.lock()->ClassName().c_str(), ""))
					{
						com.lock()->DrawImGui(cnt);
					}
					ImGui::TreePop();
				}
				else
				{
					isOnMouse = ImGui::IsItemHovered();
				}

				if (isOnMouse)
				{
					if (Mouse::IsTrigger(Mouse::Button::Right))
					{
						ImGui::OpenPopup(popupName1);
						if (typeid(*com.lock()) != typeid(Transform))
							selectComponent = com;
					}
				}

				cnt++;
			}

			if (!selectComponent.expired() && ImGui::BeginPopup(popupName1))
			{
				if (ImGui::MenuItem("Copy##Inspector"))
				{

				}
				if (ImGui::MenuItem("Paste##Inspector"))
				{

				}
				if (ImGui::MenuItem("Remove##Inspector"))
				{
					for (auto itr = selectObject.lock()->components.begin(), end = selectObject.lock()->components.end(); itr != end; ++itr)
					{
						if (itr->lock() != selectComponent.lock()) continue;
						selectObject.lock()->components.erase(itr);
						break;
					}
					DeletePrefabComponentList(selectComponent);
					selectComponent.reset();
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();

		if (!isOpen)
			selectObject.reset();
	}
}

std::weak_ptr<GameObject> Prefab::FindGameObject(std::string name)
{
	for (auto obj : prefabList)
	{
		if (obj->name == name)
		{
			return obj;
		}
	}
	return std::weak_ptr<GameObject>();
}

void Prefab::RegisterGameObject(const std::shared_ptr<GameObject>& object)
{
	// ���X�g�ɓo�^
	prefabGameObjectList.emplace_back(object);
	// �R���|�[�l���g�̓o�^
	for (const std::weak_ptr<Component> & component : object->components)
	{
		prefabComponentList.emplace_back(component);
	}

	// �q�̃I�u�W�F�N�g�����l�ɓo�^
	for (auto & child : object->transform.lock()->GetChilds())
	{
		if (child.lock()->gameObject.expired())
			continue;

		this->RegisterGameObject(child.lock()->gameObject.lock());
	}
}

void Prefab::DeleteGameObject(std::weak_ptr<GameObject> object)
{
	// �q�̃I�u�W�F�N�g�����l�ɍ폜
	for (auto & child : object.lock()->transform.lock()->GetChilds())
	{
		if (child.lock()->gameObject.expired())
			continue;

		this->DeleteGameObject(child.lock()->gameObject);
	}

	for (auto component : object.lock()->components)
		DeletePrefabComponentList(component);

	DeletePrefabObjectList(object);
}

void Prefab::DeletePrefabObjectList(std::weak_ptr<GameObject> object)
{
	for (auto itr = prefabGameObjectList.begin(), end = prefabGameObjectList.end(); itr != end; ++itr)
	{
		if ((*itr) != object.lock()) continue;
		prefabGameObjectList.erase(itr);
		return;
	}
}

void Prefab::DeletePrefabComponentList(std::weak_ptr<Component> component)
{
	for (auto itr = prefabComponentList.begin(), end = prefabComponentList.end(); itr != end; ++itr)
	{
		if ((*itr) != component.lock()) continue;
		prefabComponentList.erase(itr);
		return;
	}
}

void Prefab::SetOriginalName(std::weak_ptr<GameObject> & original)
{
	// �������O�����Ȃ������炻�̂܂܃��^�[��
	if (CheckName(original, original.lock()->name))
		return;

	// �d�����Ȃ��Ȃ�܂ŃJ�E���g�𑝂₷
	int cnt = 1;
	while (true)
	{
		if (CheckName(original, original.lock()->name + "(" + std::to_string(cnt) + ")"))
			break;
		cnt++;
	}
	original.lock()->name += "(" + std::to_string(cnt) + ")";
}

bool Prefab::CheckName(std::weak_ptr<GameObject> & mine, std::string checkName)
{
	for (auto & data : prefabList)
	{
		if (data == mine.lock()) continue;

		if (data->name == checkName)
			return false;
	}
	return true;
}

bool Prefab::CheckName(std::weak_ptr<GameObject>& mine, std::string checkName, std::list<std::shared_ptr<GameObject>>::iterator & outItr)
{
	for (auto itr = prefabList.begin(),end = prefabList.end();itr != end;++itr)
	{
		if (*itr == mine.lock()) continue;

		if ((*itr)->name == checkName)
		{
			outItr = itr;
			return false;
		}
	}
	return true;
}
