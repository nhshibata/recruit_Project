//=========================================================
// [stl.h]
// 作成:2022/09/13 string,wstring変換用関数追加
// 参考:
//	https://tagsqa.com/detail/74555
//	https://tagsqa.com/detail/23220
//=========================================================

#define NOMINMAX

//--- インクルード部
#include <CoreSystem/Util/stringConvert.h>
#include <locale>
#include <codecvt>
#include <Windows.h>

namespace MySpace
{
	namespace System
	{
		// *@wstringをstringへ変換
		// *@逆は文字化けする(英数字以外)
		// std::wstring -> std::string
		std::string WStringToString(const std::wstring& wszWord)
		{
			/*std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::string ret = converter.to_bytes(wszWord);
			return ret;*/

			int strSize = WideCharToMultiByte(CP_UTF8, 0, wszWord.c_str(), -1, nullptr, 0, nullptr, nullptr);
			std::string ret(strSize, 0);
			WideCharToMultiByte(CP_UTF8, 0, wszWord.c_str(), -1, &ret[0], strSize, nullptr, nullptr);
			return ret;
		}

		// *@stringをwstringへ変換
		// std::string -> std::wstring
		std::wstring StringToWString(const std::string& szWord)
		{
			/*std::string narrow = szWord;
			std::wstring wide;
			wide.resize(std::mbstowcs(nullptr, narrow.c_str(), 0));
			std::mbstowcs(&wide[0], narrow.c_str(), wide.size());
			return wide;*/

			int wstrSize = MultiByteToWideChar(CP_UTF8, 0, szWord.c_str(), -1, nullptr, 0);
			std::wstring ret(wstrSize, 0);
			MultiByteToWideChar(CP_UTF8, 0, szWord.c_str(), -1, &ret[0], wstrSize);
			return ret;
		}

	}
}