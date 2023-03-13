//==========================================================
// [imGuiPackage.cpp]
// ������L�q���邱�Ƃ̂���ImGui�̃Z�b�g���֐���
//==========================================================

//--- �C���N���[�h��
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>
#if BUILD_MODE

using namespace MySpace::System;
// ���O��ԓ��̊֐��̒�`�͖��O��Ԃōs��Ȃ���΃G���[���N����
// �׽�ƈႢ�X�R�[�v�����m�ł͂Ȃ�����H
namespace MySpace
{
	namespace Debug
	{
		//==========================================================
		// �J�[�\��X�ʒu�ݒ�
		//==========================================================
		void SetControlPosX(float offset)
		{
			ImGui::SetCursorPosX(Debug::CONTROL_POS_X + offset);
		}

		//==========================================================
		// Text��\�����ASameline���Ăяo���ACursor�ʒu��ݒ肷��
		//==========================================================
		void SetTextAndAligned(std::string text, float offset)
		{
			ImGui::Text(text.c_str());
			ImGui::SameLine();
			SetControlPosX(offset);
		}

#pragma region STRING
		//==========================================================
		// ���������
		//==========================================================
		std::string InputString(std::string text, std::string desc)
		{
			char name[56];
			strcpy_s(name, text.c_str());
			if (ImGui::InputText(desc.c_str(), name, 56))
			{

			}
			return name;
		}
#pragma endregion

#pragma region DISP_FILE
		//==========================================================
		// vector�^�̃��X�g����\�����A�I�����ꂽ�ۂɂ���string��Ԃ�
		//==========================================================
		std::string DispComboSelect(std::vector<std::string> list, std::string desc, std::string preview)
		{
			std::string ret = std::string();
			if (ImGui::BeginCombo(desc.c_str(), preview.c_str()))
			{
				for (int cnt = 0; cnt < list.size(); cnt++)
				{
					if (bool bg = ImGui::Selectable(list[cnt].c_str(), &bg); bg)
					{
						ret = list[cnt];
						break;
					}
				}
				ImGui::EndCombo();
			}
			return ret;
		}

		//==========================================================
		// vector�^�̃��X�g����\�����A�I�����ꂽ�ۂɃC���f�b�N�X��Ԃ�
		//==========================================================
		int DispComboSelect(std::vector<std::string> list, std::string desc, int idx)
		{
			std::string preview = (idx < 0) ? "" : list[idx];
			if (ImGui::BeginCombo(desc.c_str(), preview.c_str()))
			{
				for (int cnt = 0; cnt < list.size(); cnt++)
				{
					if (bool bg = ImGui::Selectable(list[cnt].c_str(), &bg); bg)
					{
						idx = cnt;
						break;
					}
				}
				ImGui::EndCombo();
			}
			return idx;
		}

		//==========================================================
		// �w��p�X�̃t�H���_���̃t�@�C������\������
		//==========================================================
		std::string DispFileMenuBar(std::string path, std::string desc, std::string ext)
		{
			CFilePath* file = new CFilePath;
			std::string ret = MySpace::Debug::DispMenuBar(file->GetAllFileName(path, ext), desc);
			delete file;

			return ret;
		}

		//==========================================================
		// ���j���[�o�[�̕\��
		// �\�����ꂽ�A�C�e�����I�����ꂽ�ꍇ�͕�����擾
		//==========================================================
		std::string DispMenuBar(std::vector<std::string> list, std::string desc)
		{
			std::string ret = std::string();
			auto NameList = list;
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu(desc.c_str()))
				{
					for (auto & fileName : NameList)
					{
						if (ImGui::MenuItem(fileName.c_str()))
						{
							ret = fileName;
							break;
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			return ret;
		}

		//==========================================================
		// ���j���[�o�[�̕\��(Effekseer�p)
		// �p�X�悩��t�@�C������ǂݍ��ݕ\��
		// �\�����ꂽ�A�C�e�����I�����ꂽ�ꍇ�͕�����擾
		//==========================================================
		std::u16string DispFileMenuBar16(std::string path, std::string desc, std::string ext)
		{
			std::u16string ret = std::u16string();
			CFilePath* filePath = new CFilePath();
			//--- �p�X���̃t�@�C����S�擾
			auto DispList = filePath->GetAllFileName(path, ext);		// �\���p
			auto NameList = filePath->GetAllFileNameTo16(path, ext);	// �߂�l�p
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu(desc.c_str()))
				{
					for (int cnt = 0; cnt < DispList.size(); ++cnt)
					{
						if (ImGui::MenuItem(DispList[cnt].c_str()))
						{
							ret = NameList[cnt];
							break;
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			delete filePath;
			return ret;
		}
		
		//==========================================================
		// ���j���[�o�[�̕\��
		// �󂯎�������X�g����\��
		//==========================================================
		std::u16string DispFileMenuBar16(std::vector<std::string> name,std::vector<std::u16string> file, std::string desc)
		{
			int cnt = 0;
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu(desc.c_str()))
				{
					for (cnt = 0; cnt < name.size(); ++cnt)
					{
						if (ImGui::MenuItem(name[cnt].c_str()))
						{
							break;
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			if (cnt < file.size())
				return file[cnt];
			return std::u16string();
		}
#pragma endregion

#pragma region POP_MENU
		
		//==========================================================
		// �|�b�v�A�b�v�\��
		//==========================================================
		// -1�͂Ȃɂ��I�����Ă��Ȃ�����
		int PopupMenu(std::vector<std::string> vec, std::string name)
		{	
			int cnt = -1;
			if (ImGui::BeginPopupContextItem(name.c_str()))
			{
				for (cnt = 0; cnt < vec.size(); cnt++)
				{
					// ���͂�����Δ�����
					if (ImGui::MenuItem(vec[cnt].c_str()))
					{
						break;
					}
				}
				ImGui::EndPopup();
			}
			// ���͂��ꂽ�ԍ���Ԃ�
			return cnt == vec.size() ? -1 : cnt;
		}
#pragma endregion


#pragma region BUTTON
		//==========================================================
		// ���W�I�{�^���\��
		//==========================================================
		int CreateRadio(std::vector<std::string> vec, int current)
		{
			for (int cnt = 0; cnt < vec.size(); cnt++)
			{
				if (ImGui::RadioButton(vec[cnt].c_str(), cnt == current))
				{
					return cnt;
				}
			}
			return current;
		}


		//==========================================================
		// ���W�I�{�^���\��
		// �r�b�g�m�F�ł���萔��񋓑̗p
		//==========================================================
		int CreateRadioForBit(std::vector<std::string> vec, int current)
		{
			if (ImGui::RadioButton(vec[0].c_str(), 0 == current))
				return 0;

			for (int cnt = 0; cnt < vec.size() - 1; cnt++)
			{
#pragma warning(suppress: 26451)
				if (ImGui::RadioButton(vec[cnt + 1].c_str(), (1 << cnt) == current))
				{
					return  (1 << cnt);
				}
			}
			return current;
		}

		//==========================================================
		// �r�b�g�ŕ����ؑփ{�^��
		//==========================================================
		int CreateSelectableForBit(std::vector<std::string> vec, int current, int newLine, float width)
		{
			ImVec2 size = ImVec2(width, 0);
			// ������ & �_�u���N���b�N
			if (ImGui::Selectable(vec[0].c_str(), 0 == current, ImGuiSelectableFlags_AllowDoubleClick, size))
				if (ImGui::IsMouseDoubleClicked(0))
					return 0;

			for (int cnt = 0; cnt < vec.size() - 1; cnt++)
			{
#pragma warning(suppress: 26451)
				if (ImGui::Selectable(vec[cnt + 1].c_str(), (1 << cnt) | current, ImGuiSelectableFlags_::ImGuiSelectableFlags_None, size))
				{
					// �r��
					current ^= (1 << cnt);
				}
				if ((cnt) % newLine == 0)
					ImGui::SameLine();
			}
			return current;
		}

#pragma endregion

	}
}
#endif