//==========================================================
// [imGuiPackage.h]
// ������L�q���邱�Ƃ̑���ImGui�̃Z�b�g���֐���
//==========================================================

#ifndef __IMGUI_PACKAGE_H__
#define __IMGUI_PACKAGE_H__

//--- �C���N���[�h��
#include <DebugSystem/debug.h>
#if BUILD_MODE

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <CoreSystem/Util/stl.h>
#include <DirectXMath.h>

namespace MySpace
{
	namespace Debug
	{
		// *@���������
		_NODISCARD std::string InputString(std::string text, std::string desc = std::string());

		// *@�R���{�\��
		// *@������: �\�����X�g vector<string>, ���j���[�� string
		std::string DispCombo(std::vector<std::string> list, std::string desc, std::string preview = std::string());

		// *@���j���[�o�[�\��
		std::string DispFileMenuBar(std::string path, std::string desc = std::string(), std::string ext = std::string());
		
		// *@���j���[�o�[�\��
		// *@������: �\�����X�g vector<string>, ���j���[�� string
		std::string DispMenuBar(std::vector<std::string> list, std::string desc = std::string());

		// *@���j���[�o�[�\��(�p�X�悩��t�@�C�������擾���Ă��邽�߁A���ʂ�����)
		// *@Effekseer�p
		std::u16string DispFileMenuBar16(std::string path, std::string desc = std::string(), std::string ext = std::string());
		
		// *@���j���[�o�[�\��
		// *@Effekseer�p
		std::u16string DispFileMenuBar16(std::vector<std::string> name, std::vector<std::u16string> file, std::string desc);

		// *@Pop���j���[�\��and�I��
		// *@-1�͂Ȃɂ��I�����Ă��Ȃ�����
		_NODISCARD int PopupMenu(std::vector<std::string> vec, std::string name, bool open);

		// *@�r�b�g�ŕ����ؑփ{�^��
		_NODISCARD int CreateSelectableForBit(std::vector<std::string> vec, int current, int newLine = 2, float width = 200);

		// *@�r�b�g�m�F�ł���萔��񋓑̗p
		_NODISCARD int CreateRadioForBit(std::vector<std::string> vec, int current);

		// *@���W�I�{�^���\��
		_NODISCARD int CreateRadio(std::vector<std::string> vec, int current);

#pragma region DRAG_AND_DROP

		// *@Source�֐��̑��������Ɠ���ɂ���(�萔�ȂǂŊǗ����ׂ�)
		// *@�h���b�v��
		template<class T>
		T* DragDropTarget(std::string name)
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(name.c_str()))
				{
					IM_ASSERT(payload->DataSize == sizeof(T));
					/*T payload_type = *(const T*)payload->Data;
					return &payload_type;*/
					if (auto ptr = (T*)payload->Data; !ptr)
						return nullptr;
					return (T*)payload->Data;
				}
				ImGui::EndDragDropTarget();
			}
			return nullptr;
		}

		// *@Target�֐��̑��������Ɠ���ɂ���(�萔�ȂǂŊǗ����ׂ�)
		// *@�h���b�O��
		template<class T>
		bool DragDropSource(std::string name, std::string moveName, T type)
		{
			if (ImGui::BeginDragDropSource())
			{
				const ImGuiPayload* payload = ImGui::GetDragDropPayload();
				ImGui::SetDragDropPayload(name.c_str(), &type, sizeof(T));
				// �h���b�O���̂��̖̂��O
				ImGui::Text(std::string("Move Parent ==>>" + moveName).c_str());
				ImGui::EndDragDropSource();
				return true;
			}
			return false;
		}

#pragma endregion

	}
}

#endif // !BUILD_MODE

#endif // !__IMGUI_PACKAGE