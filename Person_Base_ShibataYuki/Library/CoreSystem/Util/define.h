//=========================================================
// [define.h] 
// �쐬:2022/07/25
//---------------------------------------------------------
// #define��u���ꏊ
//=========================================================

#ifndef __DEFINE_H__
#define __DEFINE_H__

//--- �C���N���[�h��
#include <string>

//--- �萔��`
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif


// C����̃}�N�������Ŏg����#���Z�q�͈����̓���#������ƈ����̗��[�Ƀ_�u���N�I�[�e�[�V���������āC������������ɂȂ�D
// https://miettal.hatenablog.com/entry/20120624/1340533907
#define address2str(func) #func
#define TODO(str) __pragma(message(__FILE__"(" address2str(__LINE__) ") : �yTODO�z " str))

#define FORDER_DIR(path)		("../"#path)
#define FORDER_DIR2(path)		(L"..\\"#path)
#define SHADER_PATH(path)		(std::wstring(L"..\\") + std::wstring(path)).data()


#endif // !__DEFINE_H__
