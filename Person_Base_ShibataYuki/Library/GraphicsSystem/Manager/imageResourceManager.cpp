//=========================================================
// [imageResourceManager.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GraphicsSystem/Manager/imageResourceManager.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;

//--- �萔��`


CImageResourceManager::CImageResourceManager()
{
}
void CImageResourceManager::Uninit()
{
	UnloadAll();
}
bool CImageResourceManager::Load(std::string name)
{
	// �ǂݍ��ݍς݂��m�F
	auto it = m_aResourceMap.find(name);
	if (it != m_aResourceMap.end())
		return true;
	// �쐬
	std::shared_ptr<CImageResource> tex = std::make_shared<CImageResource>();

	// �ǂݍ���
	if (!tex->Load(name)) 
	{
		return false;
	}
	// �i�[
	m_aResourceMap.insert(IMAGE_PAIR(name, tex));
	return true;
}
bool CImageResourceManager::Unload(std::string name)
{
	auto it = m_aResourceMap.find(name);
	if (it != m_aResourceMap.end())
	{
		m_aResourceMap[name].reset();	// ���L���̔j��
		m_aResourceMap.erase(name);		// ���X�g����̏��O
		return true;
	}
	return false;
}
void CImageResourceManager::UnloadAll()
{
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
	{
		it->second.reset();
	}
	m_aResourceMap.clear();
}
// �摜��n��
ImageSharedPtr CImageResourceManager::GetResource(std::string name)
{
	auto it = m_aResourceMap.find(name);
	if (it == m_aResourceMap.end())
	{
		if (!Load(name))
		{
			// �ǂݍ��ݎ��s
			return std::shared_ptr<CImageResource>();
		}
	}

	return m_aResourceMap[name];
}
int CImageResourceManager::SceneUnload()
{
#if _DEBUG	// �m�F
	int cnt = static_cast<int>(m_aResourceMap.size());
	for (auto & image : m_aResourceMap)
	{
		// �����������g���������Ă��Ȃ���Ή��
		if (image.second.use_count() == 1)
		{
			image.second->Unload();
			image.second.reset();
			--cnt;
		}
	}
	return cnt;
#else
	for (auto & image : m_ResourceMap)
	{
		// �����������g���������Ă��Ȃ���Ή��
		if (image.second.use_count() == 1)
		{
			image.second->Unload();
			image.second.reset();
		}
	}
#endif // _DEBUG

	return 0;
}