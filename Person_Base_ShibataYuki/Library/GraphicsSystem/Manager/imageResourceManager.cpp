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
	auto it = m_ResourceMap.find(name);
	if (it != m_ResourceMap.end())
		return true;
	// �쐬
	std::shared_ptr<CImageResource> tex = std::make_shared<CImageResource>();

	// �ǂݍ���
	if (!tex->Load(name)) 
	{
		return false;
	}
	// �i�[
	m_ResourceMap.insert(IMAGE_PAIR(name, tex));
	return true;
}
bool CImageResourceManager::Unload(std::string name)
{
	auto it = m_ResourceMap.find(name);
	if (it != m_ResourceMap.end())
	{
		m_ResourceMap[name].reset();	// ���L���̔j��
		m_ResourceMap.erase(name);		// ���X�g����̏��O
		return true;
	}
	return false;
}
void CImageResourceManager::UnloadAll()
{
	for (auto it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
	{
		it->second.reset();
	}
	m_ResourceMap.clear();
}
// �摜��n��
ImageSharedPtr CImageResourceManager::GetResource(std::string name)
{
	auto it = m_ResourceMap.find(name);
	if (it == m_ResourceMap.end())
	{
		if (!Load(name))
		{
			// �ǂݍ��ݎ��s
			return std::shared_ptr<CImageResource>();
		}
	}

	return m_ResourceMap[name];
}
