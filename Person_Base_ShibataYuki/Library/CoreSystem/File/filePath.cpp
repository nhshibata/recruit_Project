//=========================================================
// [filePath.h]
// �쐬:2022/07/26
//-----------------------------
// �Q�l�F�قڂ��̂܂�
// https://qiita.com/tes2840/items/8d295b1caaf10eaf33ad
// https://nompor.com/2019/02/16/post-5089/
//=========================================================

//--- �C���N���[�h��
#include <CoreSystem/File/filePath.h>
#include <filesystem>

using namespace MySpace::System;


// �J�����g�f�B���N�g���̎擾
std::string CFilePath::GetCurrentDirectory()
{
	std::filesystem::path retPath = std::filesystem::current_path();
	return retPath.string();
}
// ���΃p�X�����΃p�X�֕ϊ�
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
				retList.push_back(file.path().string());		// �p�X�����i�[
				printf("%s\n", retList.back().c_str());		// �Ō���̖��O���o��
				continue;
			}
		}
		retList.push_back(file.path().string());		// �p�X�����i�[
		printf("%s\n", retList.back().c_str());		// �Ō���̖��O���o��
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
		fileList.push_back(file.path().string());		// �p�X�����i�[
		printf("%s\n", fileList.back().c_str());		// �Ō���̖��O���o��
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
		if (ext != file.path().extension())continue;	// �g���q��r
		fileList.push_back(file.path().string());		// �p�X�����i�[
		printf("%s\n", fileList.back().c_str());		// �Ō���̖��O���o��
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
			retList.push_back(file.path().u16string());		// �p�X�����i�[
			continue;
		}
	}

	return retList;
}