//==========================================================
// [volumeManager.cpp]
//
//---------------------------------------------------------
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/volumeManager.h>
#include <algorithm>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CVolumeManager::CVolumeManager()
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CVolumeManager::~CVolumeManager()
{

}

//==========================================================
// ���C���[�ƈ�v����volume���i�[���A�Ԃ�
//==========================================================
std::vector<CVolume*> CVolumeManager::GetVolume(const int layer)const
{
	std::vector<CVolume*> ret;
	
	//--- ��v������̂��i�[
	for (auto & vol : m_aIntMap)
	{
		if (!vol.second->GetEffect())
			continue;

		if (vol.second->IsLayer(layer))
		{
			ret.push_back(vol.second);
		}
	}

	//--- �D��x�ŏ����ɕ��ёւ�
	std::sort(ret.begin(), ret.end(), [](auto const& a, auto const& b)->bool {
		return a->GetPriority() < b->GetPriority();
	});

	return ret;
}