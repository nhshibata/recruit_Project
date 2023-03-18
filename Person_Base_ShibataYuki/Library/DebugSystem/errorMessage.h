//=========================================================
// [errorMessage.h]
//---------------------------------------------------------
// �쐬:2023/03/16 
//---------------------------------------------------------
// �G���[���b�Z�[�W�p�N���X
//=========================================================

#ifndef __ERROR_MESSAGE_H__
#define __ERROR_MESSAGE_H__

//--- �C���N���[�h��
#include <iostream>
#include <string>
#include <Windows.h>
#include <D3D11.h>
#include <tchar.h>

namespace MySpace
{
	namespace Debug
	{
		class CErrorMessage
		{
		private:

		public:
			// *@HRESULT����͂��ĕ������Ԃ�
			static std::string GetErrorDescription(HRESULT hr);

			// *@HRESULT����͂���MessageBox��\������
			static void DispErrorHandle(HRESULT hr, std::string title = std::string());
			
			// *@MessageBox��\������
			static void DispMessage(std::string message, std::string title, int type = MB_OK);

		};
	}
}

#endif // !__ERROR_MESSAGE_H__
