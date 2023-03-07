//=========================================================
// [stl.h]
// 作成:2022/09/13 string,wstring変換用関数追加
// 参考:
//	https://tagsqa.com/detail/74555
//	https://tagsqa.com/detail/23220
//=========================================================

#ifndef __STRING_CONVERT_H__
#define __STRING_CONVERT_H__

//--- インクルード部
//#include <CoreSystem/Util/stl.h>
#include <string>
#include <d3d11.h>

// FIXME:移動するかも?
namespace MySpace
{
	namespace System
	{
		// *@wstringをstringへ変換
		// *@逆は文字化けする(英数字以外)
		std::string WStringToString(const std::wstring& wszWord);
		// *@stringをwstringへ変換
		std::wstring StringToWString(const std::string& szWord);
	}
}

#endif // !__STRING_CONVERT__
