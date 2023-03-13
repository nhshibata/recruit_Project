//=========================================================
// [imGuiContextMenu.cpp] 
// �쐬:2023/03/13
//---------------------------------------------------------
// �R���e�L�X�g���j���[�p
//=========================================================

//--- �C���N���[�h��
#include <DebugSystem/imGuiContextMenu.h>
#include <DebugSystem/imGuiPackage.h>

#include <GameSystem/components.h>
#include <CoreSystem/File/cerealize.h>

namespace MySpace
{

	namespace Debug
	{
		void PopUpGameObjectMenu(MySpace::Game::CGameObject* pObj)
		{
			static std::vector<std::string> menuVec = {
				u8"ParentDissolved(�e�q�֌W����)",
				u8"Copy",
				u8"Paste",
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
						sirial.OutputFile(pObj->GetName(), GAME_COPY, obj);
					}
					break;
				}
				case 2:
				{
					// �ꎞ�I�ȃI�u�W�F�N�g����
					if (auto work = std::make_shared<CGameObject>(); work)
					{
						CCerealize<std::shared_ptr<CGameObject>> sirial;
						// �ް��ǂݍ���
						work = sirial.InputFile(GAME_COPY);

						// �V�����I�u�W�F�N�g����
						auto newObj = CGameObject::CreateObject();
						// �ǂ݂��܂ꂽ�R���|�[�l���g�̎󂯓n��
						auto comList = work->GetComponentList();
						newObj.lock()->SetName(newObj.lock()->GetName() + "_Clone");
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
					}
					break;
				}
			}

		}

	}

}
