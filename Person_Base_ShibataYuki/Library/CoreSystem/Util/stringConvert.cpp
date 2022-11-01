//=========================================================
// [stl.h]
// �쐬:2022/09/13 string,wstring�ϊ��p�֐��ǉ�
// �Q�l:
//	https://tagsqa.com/detail/74555
//	https://tagsqa.com/detail/23220
//=========================================================

#include <CoreSystem/Util/stringConvert.h>

namespace MySpace
{
	namespace System
	{
		// *@wstring��string�֕ϊ�
		// *@�t�͕�����������(�p�����ȊO)
		std::string WStringToString(std::wstring wszWord)
		{
			std::string ret = std::string(wszWord.begin(), wszWord.end());
			return ret;
		}
		// *@string��wstring�֕ϊ�
		std::wstring StringToWString(std::string szWord)
		{
			const size_t WCHARBUF = 100;
			std::wstring wszRet;
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szWord.data(), -1, wszRet.data(), WCHARBUF);
			return wszRet;
		}
	}
}