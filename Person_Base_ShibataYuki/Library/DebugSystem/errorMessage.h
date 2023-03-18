//=========================================================
// [errorMessage.h]
//---------------------------------------------------------
// 作成:2023/03/16 
//---------------------------------------------------------
// エラーメッセージ用クラス
//=========================================================

#ifndef __ERROR_MESSAGE_H__
#define __ERROR_MESSAGE_H__

//--- インクルード部
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
			// *@HRESULTを解析して文字列を返す
			static std::string GetErrorDescription(HRESULT hr);

			// *@HRESULTを解析してMessageBoxを表示する
			static void DispErrorHandle(HRESULT hr, std::string title = std::string());
			
			// *@MessageBoxを表示する
			static void DispMessage(std::string message, std::string title, int type = MB_OK);

		};
	}
}

#endif // !__ERROR_MESSAGE_H__
