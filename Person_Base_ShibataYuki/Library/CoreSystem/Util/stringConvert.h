//=========================================================
// [stl.h]
// �쐬:2022/09/13 string,wstring�ϊ��p�֐��ǉ�
// �Q�l:
//	https://tagsqa.com/detail/74555
//	https://tagsqa.com/detail/23220
//=========================================================

#ifndef __STRING_CONVERT_H__
#define __STRING_CONVERT_H__

//--- �C���N���[�h��
//#include <CoreSystem/Util/stl.h>
#include <string>
#include <d3d11.h>

// FIXME:�ړ����邩��?
namespace MySpace
{
	namespace System
	{
		// *@wstring��string�֕ϊ�
		// *@�t�͕�����������(�p�����ȊO)
		std::string WStringToString(const std::wstring& wszWord);
		// *@string��wstring�֕ϊ�
		std::wstring StringToWString(const std::string& szWord);
	}
}

#endif // !__STRING_CONVERT__
