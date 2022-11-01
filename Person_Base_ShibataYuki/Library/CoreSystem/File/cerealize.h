//=========================================================
// [cerealize.h]
// �쐬:2022/07/30
//-----------------------------
// cereal�ɂ��serialize
// ���O�͂����h
//=========================================================

#ifndef __CEREALIZE_H__
#define __CEREALIZE_H__

//--- �C���N���[�h��
#include <string>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <iostream>
#include <direct.h>

namespace MySpace
{
	namespace System
	{
		template <class T>
		class CCerealize
		{
		private:

		public:
			void OutputFile(std::string name, std::string path, T& type);

			T InputFile(std::string path);
		};

		// �R���p�C���̊֌W��A�t�@�C���������s���Ƒ���ȘJ�͂������邽�߁A�w�b�_�[�ɋL��

		template<class T>
		void CCerealize<T>::OutputFile(std::string name, std::string path, T& type)
		{
			std::string fileName = path;
			std::ofstream ofs(fileName, std::ios::out);
			{
				cereal::JSONOutputArchive archive(ofs);
				archive(cereal::make_nvp(name, type));
			}
			ofs.close();
		}
		template<class T>
		T CCerealize<T>::InputFile(std::string path)
		{
			T retType;
			std::ifstream ifs(path, std::ios::in);

			if (!ifs)return retType;	// �t�@�C��Open���s
			{
				cereal::JSONInputArchive archive(ifs);
				archive(retType);
			}
			ifs.close();

			return retType;
		}

	}
}

#endif // !__CEREALIZE_H__
