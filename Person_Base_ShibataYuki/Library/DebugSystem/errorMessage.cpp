//=========================================================
// [errorMessage.h]
//---------------------------------------------------------
// �쐬:2023/03/16 
//---------------------------------------------------------
// �G���[���b�Z�[�W�p�N���X
//=========================================================

//--- �C���N���[�h��
#include <DebugSystem/errorMessage.h>

using namespace MySpace::Debug;

//=========================================================
// HRESULT����͂��ĕ������Ԃ�
//=========================================================
std::string CErrorMessage::GetErrorDescription(HRESULT hr) 
{
	char buf[512] = { 0 };
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, 0, buf, 511, NULL);
	return buf;
}

//=========================================================
// HRESULT����͂���MessageBox��\������
//=========================================================
void CErrorMessage::DispErrorHandle(HRESULT hr, std::string title)
{
	auto desc = GetErrorDescription(hr);
	title = title.empty() ? "HRESULT Error" : title;
	DispMessage(desc, title);
}

//=========================================================
// MessageBox��\������
//=========================================================
void CErrorMessage::DispMessage(std::string message, std::string title, int type)
{
	MessageBox(NULL, _T(message.c_str()), _T(title.c_str()), type);
}

