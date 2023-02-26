//=========================================================
// [stl.h]
// �쐬:2022/09/13 string,wstring�ϊ��p�֐��ǉ�
// �Q�l:
//	https://tagsqa.com/detail/74555
//	https://tagsqa.com/detail/23220
//=========================================================

#include <CoreSystem/Util/stringConvert.h>
#include <locale>
#include <codecvt>

namespace MySpace
{
	namespace System
	{
		// *@wstring��string�֕ϊ�
		// *@�t�͕�����������(�p�����ȊO)
		std::string WStringToString(std::wstring wszWord)
		{
			// std::wstring -> std::string
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::string ret = converter.to_bytes(wszWord);
			return ret;

			/*std::string ret = std::string(wszWord.begin(), wszWord.end());
			return ret;*/
		}
		// *@string��wstring�֕ϊ�
		std::wstring StringToWString(std::string szWord)
		{
			// std::string -> std::wstring
			std::string narrow = szWord;
			std::wstring wide;
			wide.resize(std::mbstowcs(nullptr, narrow.c_str(), 0));
			std::mbstowcs(&wide[0], narrow.c_str(), wide.size());
			return wide;

			/*const size_t WCHARBUF = 100;
			std::wstring wszRet;
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szWord.data(), -1, wszRet.data(), WCHARBUF);
			return wszRet;*/
		}
	}
}