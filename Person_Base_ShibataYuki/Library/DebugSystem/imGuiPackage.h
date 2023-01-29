//==========================================================
// [imGuiPackage.h]
// 複数回記述することの多いImGuiのセットを関数化
//==========================================================

#ifndef __IMGUI_PACKAGE_H__
#define __IMGUI_PACKAGE_H__

//--- インクルード部
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
		// *@文字列入力
		_NODISCARD std::string InputString(std::string text, std::string desc = std::string());

		// *@コンボ表示
		// *@引き数: 表示リスト vector<string>, メニュー名 string
		std::string DispCombo(std::vector<std::string> list, std::string desc, std::string preview = std::string());

		// *@メニューバー表示
		std::string DispFileMenuBar(std::string path, std::string desc = std::string(), std::string ext = std::string());
		
		// *@メニューバー表示
		// *@引き数: 表示リスト vector<string>, メニュー名 string
		std::string DispMenuBar(std::vector<std::string> list, std::string desc = std::string());

		// *@メニューバー表示(パス先からファイル名を取得しているため、無駄がある)
		// *@Effekseer用
		std::u16string DispFileMenuBar16(std::string path, std::string desc = std::string(), std::string ext = std::string());
		
		// *@メニューバー表示
		// *@Effekseer用
		std::u16string DispFileMenuBar16(std::vector<std::string> name, std::vector<std::u16string> file, std::string desc);

		// *@Popメニュー表示and選択
		// *@-1はなにも選択していない判定
		_NODISCARD int PopupMenu(std::vector<std::string> vec, std::string name, bool open);

		// *@ビットで複数切替ボタン
		_NODISCARD int CreateSelectableForBit(std::vector<std::string> vec, int current, int newLine = 2, float width = 200);

		// *@ビット確認できる定数や列挙体用
		_NODISCARD int CreateRadioForBit(std::vector<std::string> vec, int current);

		// *@ラジオボタン表示
		_NODISCARD int CreateRadio(std::vector<std::string> vec, int current);

#pragma region DRAG_AND_DROP

		// *@Source関数の第一引き数と同一にする(定数などで管理すべき)
		// *@ドロップ先
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

		// *@Target関数の第一引き数と同一にする(定数などで管理すべき)
		// *@ドラッグ元
		template<class T>
		bool DragDropSource(std::string name, std::string moveName, T type)
		{
			if (ImGui::BeginDragDropSource())
			{
				const ImGuiPayload* payload = ImGui::GetDragDropPayload();
				ImGui::SetDragDropPayload(name.c_str(), &type, sizeof(T));
				// ドラッグ中のものの名前
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