//=========================================================
// [filePath.h]
// 作成:2022/07/26
//-----------------------------
// 特定フォルダ内のファイル名を一括で取得するためのｸﾗｽ
// C++ 17 のため、プロパティ設定必須
// 絶対パスからの名前が得られる
//=========================================================

//--- インクルードガード
#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

//--- インクルード部
#include <vector>
#include <string>
#include <CoreSystem/Util/define.h>

namespace MySpace
{
	namespace System
	{
		class CFilePath
		{
		private:
		public:

			// カレントディレクトリ名の取得
			std::string GetCurrentDirectory();
			// 相対パスから絶対パスへ変換し、取得
			std::string RelativeToAbsolutePath(std::string path);
			// フォルダのファイル名(相対パス)取得
			std::vector<std::string> GetFileName(std::string path, std::string ext = std::string());
			// フォルダのファイル名(相対パス)をサブフォルダ含め取得
			std::vector<std::string> GetAllFileName(std::string path);
			// 拡張子指定ver
			std::vector<std::string> GetAllFileName(std::string path, std::string ext);
			// Effekseer用。u16として返す
			std::vector<std::u16string> GetAllFileNameTo16(std::string path, std::string ext);
		};
	}

}
#endif // !__FILE_PATH_H__
