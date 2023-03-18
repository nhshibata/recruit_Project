//=========================================================
// [errorMessage.h]
//---------------------------------------------------------
// 作成:2023/03/16 
//---------------------------------------------------------
// エラーメッセージ用クラス
//=========================================================

//--- インクルード部
#include <DebugSystem/errorMessage.h>

using namespace MySpace::Debug;

//=========================================================
// HRESULTを解析して文字列を返す
//=========================================================
std::string CErrorMessage::GetErrorDescription(HRESULT hr) 
{
	char buf[512] = { 0 };
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, 0, buf, 511, NULL);
	return buf;
}

//=========================================================
// HRESULTを解析してMessageBoxを表示する
//=========================================================
void CErrorMessage::DispErrorHandle(HRESULT hr, std::string title)
{
	auto desc = GetErrorDescription(hr);
	title = title.empty() ? "HRESULT Error" : title;
	DispMessage(desc, title);
}

//=========================================================
// MessageBoxを表示する
//=========================================================
void CErrorMessage::DispMessage(std::string message, std::string title, int type)
{
	MessageBox(NULL, _T(message.c_str()), _T(title.c_str()), type);
}

