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

namespace MySpace
{
	namespace Debug
	{
		// 文字列入力
		std::string InputString(std::string text, std::string desc = std::string());
		// メニューバー表示
		std::string DispFileMenuBar(std::string path, std::string desc = std::string(), std::string ext = std::string());
		// *引き数: 表示リスト vector<string>, メニュー名 string
		std::string DispMenuBar(std::vector<std::string> list, std::string desc = std::string());
		// Effekseer用
		std::u16string DispFileMenuBar16(std::string path, std::string desc = std::string(), std::string ext = std::string());
		std::u16string DispFileMenuBar16(std::vector<std::string> name, std::vector<std::u16string> file, std::string desc);

		DirectX::XMFLOAT3 InputInt(DirectX::XMFLOAT3 value);
		DirectX::XMFLOAT2 InputInt(DirectX::XMFLOAT2 value);

		//DirectX::XMFLOAT3 InputFloat(DirectX::XMFLOAT3 value);
	}
}
#endif // #ifdef BUILD_MODE

#endif // !__IMGUI_PACKAGE