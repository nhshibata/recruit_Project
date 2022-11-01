//=========================================================
// [cerealize.h]
// 作成:2022/07/30
//-----------------------------
// cerealによるserialize
// 名前はご愛敬
//=========================================================

#ifndef __CEREALIZE_H__
#define __CEREALIZE_H__

//--- インクルード部
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

		// コンパイラの関係上、ファイル分割を行うと多大な労力がかかるため、ヘッダーに記載

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

			if (!ifs)return retType;	// ファイルOpen失敗
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
