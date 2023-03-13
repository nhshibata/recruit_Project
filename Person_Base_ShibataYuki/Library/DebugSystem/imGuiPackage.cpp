//==========================================================
// [imGuiPackage.cpp]
// 複数回記述することのあるImGuiのセットを関数化
//==========================================================

//--- インクルード部
#include <DebugSystem/imGuiPackage.h>
#include <CoreSystem/File/filePath.h>
#if BUILD_MODE

using namespace MySpace::System;
// 名前空間内の関数の定義は名前空間で行わなければエラーが起きる
// ｸﾗｽと違いスコープが明確ではないから？
namespace MySpace
{
	namespace Debug
	{
		//==========================================================
		// カーソルX位置設定
		//==========================================================
		void SetControlPosX(float offset)
		{
			ImGui::SetCursorPosX(Debug::CONTROL_POS_X + offset);
		}

		//==========================================================
		// Textを表示し、Samelineを呼び出し、Cursor位置を設定する
		//==========================================================
		void SetTextAndAligned(std::string text, float offset)
		{
			ImGui::Text(text.c_str());
			ImGui::SameLine();
			SetControlPosX(offset);
		}

#pragma region STRING
		//==========================================================
		// 文字列入力
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
		// vector型のリストから表示し、選択された際にそのstringを返す
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
		// vector型のリストから表示し、選択された際にインデックスを返す
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
		// 指定パスのフォルダ内のファイル名を表示する
		//==========================================================
		std::string DispFileMenuBar(std::string path, std::string desc, std::string ext)
		{
			CFilePath* file = new CFilePath;
			std::string ret = MySpace::Debug::DispMenuBar(file->GetAllFileName(path, ext), desc);
			delete file;

			return ret;
		}

		//==========================================================
		// メニューバーの表示
		// 表示されたアイテムが選択された場合は文字列取得
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
		// メニューバーの表示(Effekseer用)
		// パス先からファイル名を読み込み表示
		// 表示されたアイテムが選択された場合は文字列取得
		//==========================================================
		std::u16string DispFileMenuBar16(std::string path, std::string desc, std::string ext)
		{
			std::u16string ret = std::u16string();
			CFilePath* filePath = new CFilePath();
			//--- パス内のファイルを全取得
			auto DispList = filePath->GetAllFileName(path, ext);		// 表示用
			auto NameList = filePath->GetAllFileNameTo16(path, ext);	// 戻り値用
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
		// メニューバーの表示
		// 受け取ったリストから表示
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
		// ポップアップ表示
		//==========================================================
		// -1はなにも選択していない判定
		int PopupMenu(std::vector<std::string> vec, std::string name)
		{	
			int cnt = -1;
			if (ImGui::BeginPopupContextItem(name.c_str()))
			{
				for (cnt = 0; cnt < vec.size(); cnt++)
				{
					// 入力があれば抜ける
					if (ImGui::MenuItem(vec[cnt].c_str()))
					{
						break;
					}
				}
				ImGui::EndPopup();
			}
			// 入力された番号を返す
			return cnt == vec.size() ? -1 : cnt;
		}
#pragma endregion


#pragma region BUTTON
		//==========================================================
		// ラジオボタン表示
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
		// ラジオボタン表示
		// ビット確認できる定数や列挙体用
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
		// ビットで複数切替ボタン
		//==========================================================
		int CreateSelectableForBit(std::vector<std::string> vec, int current, int newLine, float width)
		{
			ImVec2 size = ImVec2(width, 0);
			// 初期化 & ダブルクリック
			if (ImGui::Selectable(vec[0].c_str(), 0 == current, ImGuiSelectableFlags_AllowDoubleClick, size))
				if (ImGui::IsMouseDoubleClicked(0))
					return 0;

			for (int cnt = 0; cnt < vec.size() - 1; cnt++)
			{
#pragma warning(suppress: 26451)
				if (ImGui::Selectable(vec[cnt + 1].c_str(), (1 << cnt) | current, ImGuiSelectableFlags_::ImGuiSelectableFlags_None, size))
				{
					// 排他
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