//=========================================================
// [imageResourceManager.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <GraphicsSystem/Manager/imageResourceManager.h>

using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CImageResourceManager::CImageResourceManager()
{
}

//==========================================================
// �f�X�N�g���N�^
//==========================================================
CImageResourceManager::~CImageResourceManager()
{
	UnloadAll();
}

//==========================================================
// �f�[�^�ǂݍ���
//==========================================================
bool CImageResourceManager::Load(std::string name)
{
	//--- �ǂݍ��ݍς݂��m�F
	auto it = m_aResourceMap.find(name);
	if (it != m_aResourceMap.end())
		return true;

	// �������m��
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

//==========================================================
// �w�肳�ꂽ�ް��̉��
//==========================================================
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

//==========================================================
// �S�J��
//==========================================================
void CImageResourceManager::UnloadAll()
{
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
	{
		it->second.reset();
	}
	m_aResourceMap.clear();
}

//==========================================================
// �摜�擾
//==========================================================
ImageSharedPtr CImageResourceManager::GetResource(std::string name)
{
	auto it = m_aResourceMap.find(name);
	//--- ���݂��Ȃ�
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

//==========================================================
// �V�[��������m�F
//==========================================================
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
	for (auto & image : m_aResourceMap)
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