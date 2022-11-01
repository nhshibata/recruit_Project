//=========================================================
// [cerealCommon.h]
// 作成:2022/07/11
// シリアライズ用インクルードヘッダー
//=========================================================

#ifndef __CEREAL_COMMON_H__
#define __CEREAL_COMMON_H__

// Before including any cereal header file
//#define CEREAL_THREAD_SAFE = 1

#define NOMINMAX

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>

#include <cereal/types/common.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>

//#include <archives/binary.hpp>
//#include <archives/json.hpp>
#include <CoreSystem/Util/define.h>

//--- 以下メモ
/*
バージョンを指定する
	archive(
	cereal::make_nvp("", cereal::base_class<>(this)),
				CEREAL_NVP()
	
	cereal::base_class<>(this)

	CEREAL_CLASS_VERSION(指定ｸﾗｽ, バージョン番号)
	
	CEREAL_REGISTER_TYPE(DerivedTwo)
	
	CEREAL_REGISTER_POLYMORPHIC_RELATION(EmptyBase, DerivedTwo)
*/


#endif // !__CEREAL_COMMON_H__

