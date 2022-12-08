//=========================================================
// [define.h] 
// 作成:2022/07/25
//---------------------------------------------------------
// #defineを置く場所
//=========================================================

#ifndef __DEFINE_H__
#define __DEFINE_H__

//--- インクルード部
#include <string>

//--- 定数定義
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif


// C言語のマクロだけで使える#演算子は引数の頭に#をつけると引数の両端にダブルクオーテーションがついて，引数が文字列になる．
// https://miettal.hatenablog.com/entry/20120624/1340533907
#define address2str(func) #func
#define TODO(str) __pragma(message(__FILE__"(" address2str(__LINE__) ") : 【TODO】 " str))

#define FORDER_DIR(path)		("../"#path)
#define FORDER_DIR2(path)		(L"..\\"#path)
#define SHADER_PATH(path)		(std::wstring(L"..\\") + std::wstring(path)).data()


#endif // !__DEFINE_H__
