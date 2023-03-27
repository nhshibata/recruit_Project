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

//=========================================================
// ���݂���Layer
//=========================================================
const int CVolumeManager::GetBit(const int layer)
{
	int retBit = 0;

	//--- ��v������̂��i�[
	for (auto & vol : m_aIntMap)
	{
		// �G�t�F�N�g���ݒ肳��Ă��Ȃ�
		if (!vol.second->GetEffect())
			continue;

		// ���C���[����v���Ȃ�
		if (!vol.second->IsLayer(layer))
			continue;

		//--- �����B��
		// GameObject��Layer�ԍ����擾���āABit�ϊ�
		// Bit�𗧂Ă�
		retBit |= CLayer::NumberToBit(vol.second->GetLayer());
	}

	return retBit;
}

//==========================================================
// ���C���[�ƈ�v����volume��ID��ǉ�����
//==========================================================
void CVolumeManager::AddRendererCache(const int nLayer, const int nID)
{
	//--- ��v������̂��i�[
	for (auto & vol : m_aIntMap)
	{
		// ���C���[����v���Ȃ�
		if (!vol.second->IsLayer(nLayer))
			continue;

		// ID��ǉ�
		vol.second->AddRendererID(nID);
	}

}

//==========================================================
// �������Ă���Volume�R���|�[�l���g�̃L���b�V�����N���A
//==========================================================
void CVolumeManager::ResetRendererCache()
{
	for (auto & vol : m_aIntMap)
	{
		vol.second->ResetRenderCache();
	}
}