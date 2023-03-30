//=========================================================
// [imGuiContextMenu.cpp] 
// �쐬:2023/03/13
//---------------------------------------------------------
// �R���e�L�X�g���j���[�p
// �t�@�C�����~�X��������
//=========================================================

//--- �C���N���[�h��
#include <DebugSystem/imGuiContextMenu.h>
#include <DebugSystem/imGuiPackage.h>

#include <GameSystem/components.h>
#include <CoreSystem/File/cerealize.h>
#include <CoreSystem/File/filePath.h>

namespace MySpace
{

	namespace Debug
	{
		// �悭�Ȃ����A�b��
		static std::string selectPrefab;

		//=========================================================
		// �R���e�L�X�g���j���[�\��
		// FIXME:�l�X�g���[��
		//=========================================================
		void PopUpGameObjectMenu(MySpace::Game::CGameObject* pObj)
		{
			static std::vector<std::string> menuVec = {
				u8"ParentDissolved(�e�q�֌W����)",
				u8"Copy",
				u8"Paste",
				u8"Prefab��",
				u8"Prefab Paste",
				u8"Close",
			};

			int res = Debug::PopupMenu(menuVec, pObj->GetName());
			switch (res)
			{
				case 0:
				{
					// �e�q�֌W����
					auto trans = pObj->GetTransform();
					if (auto parent = trans->GetParent(); parent.lock())
					{
						trans->ParentDissolved();
					}
					break;
				}
				case 1:
				{
					// �V���A���C�Y�����Ė��O,�߲���Ȃǂ����㏑������΂ł���?
					// �ł���
					// �V���A���C�Y�N���X�쐬
					CCerealize<std::shared_ptr<CGameObject>> sirial;
					{
						// �ް����O���ۑ�
						auto obj = pObj->GetPtr().lock();
						sirial.OutputFile(pObj->GetName(), COPY_DATA_GAME_OBJECT_PATH, obj);
					}
					break;
				}
				case 2:
				{
					CopyGameObject();
					break;
				}
				case 3:
				{
					// �V���A���C�Y�����Ė��O,�߲���Ȃǂ����㏑������΂ł���?
					// �ł���
					// �V���A���C�Y�N���X�쐬
					CCerealize<std::shared_ptr<CGameObject>> sirial;
					{
						std::string fileName = PREFAB_FOLDER_PATH;
						fileName += pObj->GetName();
						fileName += ".prefab";

						// �ް����O���ۑ�
						auto obj = pObj->GetPtr().lock();
						sirial.OutputFile(pObj->GetName(), fileName, obj);
					}
					break;
				}
				case 4:
				{
					PrefabPaste();
					break;
				}
			}
		}

		//=========================================================
		// �I�u�W�F�N�g�̺�߰
		//=========================================================
		std::shared_ptr<MySpace::Game::CGameObject> CopyGameObject()
		{
			// �ꎞ�I�ȃI�u�W�F�N�g����
			auto work = std::make_shared<CGameObject>();
			
			CCerealize<std::shared_ptr<CGameObject>> sirial;
			// �ް��ǂݍ���
			work = sirial.InputFile(COPY_DATA_GAME_OBJECT_PATH);
			if (!work)
				return work;

			// �V�����I�u�W�F�N�g����
			auto newObj = CGameObject::CreateObject();
			// ���ݒ�
			newObj.lock()->SetName(work->GetName() + "_Clone");
			newObj.lock()->SetObjTag(work->GetTag());
			newObj.lock()->SetLayer(work->GetLayer());
			newObj.lock()->Uninit();

			// �ǂ݂��܂ꂽ�R���|�[�l���g�̎󂯓n��
			auto comList = work->GetComponentList();
			for (auto & com : comList)
			{
				newObj.lock()->SetComponent(com);

				//--- �`��Ɠ����蔻��N���X�͗v������K�v�����邽�߁AInit���Ăяo��
				// NOTE: ����I�Ȃ��̂Ŕėp���Ɍ�����B�����ǂ��Ȃ̂�
				if (com->GetName().find("Renderer") != std::string::npos ||
					com->GetName().find("Collision") != std::string::npos)
				{
					com->Awake();
					com->Init();
				}
			}
			// �I�u�W�F�N�g�j��
			work.reset();

			return newObj.lock();
		}

		//==========================================================
		// �t�@�C���I��
		//==========================================================
		void PrefabSelect()
		{
			static std::vector<std::string> aPrefab;

			Debug::SetTextAndAligned("Prefab Reload");
			if (aPrefab.size() == 0 || ImGui::Button("Prefab Reload"))
			{
				MySpace::System::CFilePath file;
				aPrefab = file.GetAllFileName(PREFAB_FOLDER_PATH, ".prefab");
			}

			// GUI�\��
			Debug::SetTextAndAligned("Prefab");
			selectPrefab = MySpace::Debug::DispComboSelect(aPrefab, "##Prefab", selectPrefab);
		}

		//=========================================================
		// prefab��߰
		//=========================================================
		std::shared_ptr<MySpace::Game::CGameObject> PrefabPaste()
		{
			if (selectPrefab.empty())
				return std::shared_ptr<CGameObject>();
			
			// �ꎞ�I�ȃI�u�W�F�N�g����
			auto work = std::make_shared<CGameObject>();

			CCerealize<std::shared_ptr<CGameObject>> sirial;
			// �ް��ǂݍ���
			work = sirial.InputFile(selectPrefab.c_str());
			if (!work)
				return work;

			// �V�����I�u�W�F�N�g����
			auto newObj = CGameObject::CreateObject();
			// ���ݒ�
			newObj.lock()->SetName(work->GetName());
			newObj.lock()->SetObjTag(work->GetTag());
			newObj.lock()->SetLayer(work->GetLayer());
			// �ǂ݂��܂ꂽ�R���|�[�l���g�̎󂯓n��
			auto comList = work->GetComponentList();
			for (auto & com : comList)
			{
				newObj.lock()->SetComponent(com);
				//--- �`��Ɠ����蔻��N���X�͗v������K�v�����邽�߁AInit���Ăяo��
				// NOTE: ����I�Ȃ��̂Ŕėp���Ɍ�����B�����ǂ��Ȃ̂�
				if (com->GetName().find("Renderer") != std::string::npos ||
					com->GetName().find("Collision") != std::string::npos)
				{
					com->Awake();
					com->Init();
				}
			}
			// �I�u�W�F�N�g�j��
			work.reset();

			return newObj.lock();
		}
	}

}
