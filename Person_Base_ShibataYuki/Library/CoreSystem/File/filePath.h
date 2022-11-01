//=========================================================
// [filePath.h]
// �쐬:2022/07/26
//-----------------------------
// ����t�H���_���̃t�@�C�������ꊇ�Ŏ擾���邽�߂̸׽
// C++ 17 �̂��߁A�v���p�e�B�ݒ�K�{
// ��΃p�X����̖��O��������
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

//--- �C���N���[�h��
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

			// �J�����g�f�B���N�g�����̎擾
			std::string GetCurrentDirectory();
			// ���΃p�X�����΃p�X�֕ϊ����A�擾
			std::string RelativeToAbsolutePath(std::string path);
			// �t�H���_�̃t�@�C����(���΃p�X)�擾
			std::vector<std::string> GetFileName(std::string path, std::string ext = std::string());
			// �t�H���_�̃t�@�C����(���΃p�X)���T�u�t�H���_�܂ߎ擾
			std::vector<std::string> GetAllFileName(std::string path);
			// �g���q�w��ver
			std::vector<std::string> GetAllFileName(std::string path, std::string ext);
			// Effekseer�p�Bu16�Ƃ��ĕԂ�
			std::vector<std::u16string> GetAllFileNameTo16(std::string path, std::string ext);
		};
	}

}
#endif // !__FILE_PATH_H__
