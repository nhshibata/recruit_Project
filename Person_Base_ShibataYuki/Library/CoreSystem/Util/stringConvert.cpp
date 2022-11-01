//=========================================================
// [stl.h]
// 作成:2022/09/13 string,wstring変換用関数追加
// 参考:
//	https://tagsqa.com/detail/74555
//	https://tagsqa.com/detail/23220
//=========================================================

#include <CoreSystem/Util/stringConvert.h>

namespace MySpace
{
	namespace System
	{
		// *@wstringをstringへ変換
		// *@逆は文字化けする(英数字以外)
		std::string WStringToString(std::wstring wszWord)
		{
			std::string ret = std::string(wszWord.begin(), wszWord.end());
			return ret;
		}
		// *@stringをwstringへ変換
		std::wstring StringToWString(std::string szWord)
		{
			const size_t WCHARBUF = 100;
			std::wstring wszRet;
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szWord.data(), -1, wszRet.data(), WCHARBUF);
			return wszRet;
		}
	}
}