//==========================================================
// [imGuiPackage.cpp]
// 複数回記述することの多いImGuiのセットを関数化
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
		std::string InputString(std::string text, std::string desc)
		{
			char name[256];
			strcpy_s(name, text.c_str());
			ImGui::InputText(desc.c_str(), name, 256);
			return name;
		}

		std::string DispFileMenuBar(std::string path, std::string desc, std::string ext)
		{
			CFilePath file;
			std::string ret = MySpace::Debug::DispMenuBar(file.GetAllFileName(path, ext), desc);
			return ret;
		}

		// メニューバーの表示
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

		// メニューバーの表示
		std::u16string DispFileMenuBar16(std::string path, std::string desc, std::string ext)
		{
			CFilePath filePath;
			std::u16string ret = std::u16string();
			auto DispList = filePath.GetAllFileName(path, ext);		// 表示用
			auto NameList = filePath.GetAllFileNameTo16(path, ext);	// 戻り値用
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
			return ret;
		}
		
		// メニューバーの表示
		std::u16string DispFileMenuBar16(std::vector<std::string> name,std::vector<std::u16string> file, std::string desc)
		{
			int cnt;
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
			if (cnt >= file.size())
				return file[cnt];
			return std::u16string();
		}

		DirectX::XMFLOAT3 InputInt(DirectX::XMFLOAT3 value)
		{
			DirectX::XMINT3 input = {
				static_cast<int>(value.x),
				static_cast<int>(value.y),
				static_cast<int>(value.z)
			};
			ImGui::InputInt(u8"x", &input.x);
			ImGui::InputInt(u8"y", &input.y);
			ImGui::InputInt(u8"z", &input.z);

			return DirectX::XMFLOAT3(
				static_cast<float>(input.x),
				static_cast<float>(input.y),
				static_cast<float>(input.z));
		}
		
		DirectX::XMFLOAT2 InputInt(DirectX::XMFLOAT2 value)
		{
			DirectX::XMINT2 input = {
				static_cast<int>(value.x),
				static_cast<int>(value.y)
			};
			ImGui::InputInt(u8"x", &input.x);
			ImGui::InputInt(u8"y", &input.y);

			return DirectX::XMFLOAT2(
				static_cast<float>(input.x),
				static_cast<float>(input.y)
				);
		}
	
		/*DirectX::XMFLOAT3 InputFloat(DirectX::XMFLOAT3 value)
		{

		}*/
	}
}
#endif