//=========================================================
// [filePath.h]
// 作成:2022/07/26
//-----------------------------
// 参考：ほぼそのまま
// https://qiita.com/tes2840/items/8d295b1caaf10eaf33ad
// https://nompor.com/2019/02/16/post-5089/
//=========================================================

//--- インクルード部
#include <CoreSystem/File/filePath.h>
#include <filesystem>

using namespace MySpace::System;


// カレントディレクトリの取得
std::string CFilePath::GetCurrentDirectory()
{
	std::filesystem::path retPath = std::filesystem::current_path();
	return retPath.string();
}
// 相対パスから絶対パスへ変換
std::string CFilePath::RelativeToAbsolutePath(std::string path)
{
	std::filesystem::path retPath = std::filesystem::absolute(path);
	return retPath.string();
}
std::vector<std::string> CFilePath::GetFileName(std::string path, std::string ext)
{
	using namespace std::filesystem;
	std::vector<std::string> retList;		// 
	directory_iterator it(path),end;		// 

	for(auto file : it)
	{
		if (!ext.empty())
		{
			if (file.path().extension() == ext)
			{
				retList.push_back(file.path().string());		// パス名を格納
				printf("%s\n", retList.back().c_str());		// 最後尾の名前を出力
				continue;
			}
		}
		retList.push_back(file.path().string());		// パス名を格納
		printf("%s\n", retList.back().c_str());		// 最後尾の名前を出力
	}

	return retList;
}
std::vector<std::string> CFilePath::GetAllFileName(std::string path)
{
	if (!std::filesystem::exists(path))return std::vector<std::string>();

	using namespace std::filesystem;
	std::vector<std::string> fileList;		// 
	std::filesystem::recursive_directory_iterator it(path), end;		// 

	for (auto file : it)
	{
		fileList.push_back(file.path().string());		// パス名を格納
		printf("%s\n", fileList.back().c_str());		// 最後尾の名前を出力
	}
	return fileList;
}

std::vector<std::string> CFilePath::GetAllFileName(std::string path, std::string ext)
{
	if (!std::filesystem::exists(path))return std::vector<std::string>();

	using namespace std::filesystem;
	std::vector<std::string> fileList;		// 
	std::filesystem::recursive_directory_iterator it(path), end;		// 

	for (auto file : it)
	{
		if (ext != file.path().extension())continue;	// 拡張子比較
		fileList.push_back(file.path().string());		// パス名を格納
		printf("%s\n", fileList.back().c_str());		// 最後尾の名前を出力
	}
	return fileList;
}

std::vector<std::u16string> CFilePath::GetAllFileNameTo16(std::string path, std::string ext)
{
	using namespace std::filesystem;
	std::vector<std::u16string> retList;
	std::filesystem::recursive_directory_iterator it(path), end;

	for (auto file : it)
	{
		if (file.path().extension() == ext)
		{
			retList.push_back(file.path().u16string());		// パス名を格納
			continue;
		}
	}

	return retList;
}