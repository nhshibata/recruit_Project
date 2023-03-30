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
#include <CoreSystem/Util/define.h>

#define COPY_DATA_GAME_OBJECT_PATH		FORDER_DIR(data/SystemData/copyObject.json)
#define PREFAB_FOLDER_PATH				FORDER_DIR(data/prefab/)
#define PREFAB_PATH(name)				FORDER_DIR(data/prefab/name.prefab)
#define TAG_DATA_PATH					FORDER_DIR(data/SystemData/tag.json)
#define LAYER_DATA_PATH					FORDER_DIR(data/SystemData/layer.json)

namespace MySpace
{
	namespace System
	{
		//--- ｸﾗｽ定義
		template <class T>
		class CCerealize
		{
		private:

		public:
			void OutputFile(std::string name, std::string path, T& type);

			T InputFile(std::string path);
		};

		//--- 関数実装
		// コンパイラの関係上、ファイル分割を行うと多大な労力がかかるため、ヘッダーに記載

		// ファイル出力
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

		// ファイル入力
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
